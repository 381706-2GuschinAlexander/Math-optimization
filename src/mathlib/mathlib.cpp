#include "mathlib.h"

optf::MetaData::MetaData(const std::vector<double>& vec, double val, int n) : return_point(vec), num_iteration(n), value(val){}

optf::MetaData::MetaData(const MetaData& data) : return_point(data.return_point), num_iteration(data.num_iteration), value(data.value){}

optf::MetaData::MetaData() : return_point(), num_iteration(0), value(0){}

optf::MetaData& optf::MetaData::operator=(const MetaData& data)
{
  if (this == &data)
    return *this;

  return_point = data.return_point;
  num_iteration =  data.num_iteration;
  value = data.value;
  return *this;
}

optf::MetaData optf::StronginMethod(std::function<double(double)> function, double a, double b, double r, double eps){
  int N = 10000;
  std::vector<double> x(2);
  x.reserve(N);
  std::vector<double> Q(2);
  std::vector<double> R(N, -DBL_MAX);

  Q.reserve(N);

  x[0] = a;
  x[1] = b;

  Q[0]= function(x[0]);
  Q[1]= function(x[1]);

  double global_min;
  double func_val_at_min;

  if(Q[0] < Q[1]){
    global_min = x[0];
    func_val_at_min = Q[0];
  } else{
    global_min = x[1];
    func_val_at_min = Q[1];
  }

  auto get_M = [&](int i){
    return abs(Q[i + 1] - Q[i]) / (x[i+1] - x[i]);
  };

  double M_max =  get_M(0);
  
  int n = 2;
  while(n < N){
    double L = M_max < 1.e-5 ? 1 : r * M_max;
    
    for(int i = 0; i < n - 1; ++i)
      R[i] = L * (x[i + 1] - x[i]) 
      + pow(Q[i + 1] - Q[i], 2) / (L * (x[i + 1] - x[i]))
      - 2 * (Q[i + 1] + Q[i]);

    int ROI = std::distance(R.begin(), std::max_element(R.begin(), R.begin()+n));
    double x_new = .5* (x[ROI + 1] + x[ROI]) - (Q[ROI + 1] - Q[ROI])/(2*L);

    Q.insert(Q.begin() + ROI + 1, function(x_new));
    auto lower_x = std::lower_bound(x.begin(), x.end(), x_new);
    x.insert(lower_x, x_new);
    
    M_max = std::max({M_max, get_M(ROI), get_M(ROI + 1)});
    
    if(Q[ROI + 1] < func_val_at_min){
      global_min = x_new;
      func_val_at_min = Q[ROI + 1];
    }

    if(std::abs(*lower_x - *(--lower_x)) < eps)
      return MetaData({global_min}, function(global_min), n);
      
    ++n;
  }
  
  return MetaData({global_min}, function(global_min), n);
}


FunctionContainer::FunctionContainer(const str_v &str_func_vector, const std::string &str_arg, const range_v& range, double _eps, double _h_y) : range_vec(range), eps(_eps), h_y(_h_y)
{
  for (auto &str_func : str_func_vector)
  {
    FunctionParser tmp;
    tmp.Parse(str_func, str_arg);
    func_vector.push_back(std::move(tmp));
  }
}

optf::MetaData FunctionContainer::Convolution(const std::vector<double> &conv_arg)
{
  double x0 = std::min(range_vec[0].first, range_vec[0].second);
  double x1 = std::max(range_vec[0].first, range_vec[0].second);
  
  double y0 = std::min(range_vec[1].first, range_vec[1].second);
  double y1 = std::max(range_vec[1].first, range_vec[1].second);

  int outer_n = (y1 - y0) / h_y;

  double coef_sum = 0;
  for(auto c: conv_arg)
    coef_sum += c;

  int func_vec_size = func_vector.size();
  std::vector<double> max_val(func_vec_size, -DBL_MAX);
  std::vector<double> min_val(func_vec_size, DBL_MAX);

  std::vector<FunctionParser> tmp_func(2 * func_vec_size);

  //half of vector is a shallow-copy, another half is a deep-copy 
  for(int i = 0; i < func_vec_size; ++i)
  {
    tmp_func[i] = func_vector[i];
    tmp_func[i + func_vec_size] = func_vector[i];
    tmp_func[i + func_vec_size].ForceDeepCopy();
  }

  
  for (int outer_i = 0; outer_i < outer_n ; ++outer_i)
  {
    double y = y0 + h_y * outer_i;

    // finding all min,max for functions
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < 2 * func_vec_size; ++i)
    {
      if (i < func_vec_size)
      {
        auto la_min = [&](double x) -> double
        {
          double local_args[] = {x, y};
          return tmp_func[i].Eval(local_args);
        };
        // check for new lowest value of function
        if(double val = optf::StronginMethod(la_min, x0, x1, 25, eps).value; val < min_val[i])
          min_val[i] = val;
      }
      else
      {
        auto la_max = [&](double x) -> double
        {
          double local_args[] = {x, y};
          return -tmp_func[i].Eval(local_args);
        };

        if(double val = -optf::StronginMethod(la_max, x0, x1, 25, eps).value; val > max_val[i - func_vec_size])
          max_val[i - func_vec_size] = val;
      }
    }
  }
  optf::MetaData res_data;
  double min_conv = DBL_MAX;
  double y_conv = DBL_MAX;
  for (int outer_i = 0; outer_i < outer_n ; ++outer_i)
  {
    double y = y0 + h_y * outer_i;
    // convolution lamda
    auto la = [=](double x) mutable -> double
    {
      double local_args[] = {x, y};
      double res = 0;
      for (int i = 0; i < func_vector.size(); ++i)
      {
        res += (conv_arg[i] / coef_sum) * ((func_vector[i].Eval(local_args) - min_val[i]) / (max_val[i] - min_val[i]));
      }
      return res;
    };

    if(auto res = optf::StronginMethod(la,x0,x1,25, eps); res.value < min_conv)
    {
      res_data = res;
      function = la;
      y_conv = y;
    }
  }

  
  res_data.return_point.push_back(y_conv);

  data = res_data;
  point = std::make_pair(res_data.return_point[0], res_data.value);
  return res_data;
}


void FunctionContainer::DrawPlot(const std::vector<double> &conv_arg, std::string last_Y){
  double x0 = std::min(range_vec[0].first, range_vec[0].second);
  double x1 = std::max(range_vec[0].first, range_vec[0].second);
  double h = 0.005;
  size_t num_of_iter = (x1 - x0) / h;
  std::vector<double> X(num_of_iter);
  std::vector<std::vector<double>> Y_vec;
  for (size_t i = 0; i < num_of_iter; ++i)
    X[i] = x0 + i * h;

  
  for (auto &func : func_vector){
    std::vector<double> Y(num_of_iter);
    
    for (size_t i = 0; i < num_of_iter; ++i)
    {
      double local_args[] = {X[i], data.return_point[1]};
      Y[i] = func.Eval(local_args);
    }
    Y_vec.push_back(std::move(Y));
  }
  
  
  if(function){
    std::vector<double> Y(num_of_iter);
    for (size_t i = 0; i < num_of_iter; ++i)
      Y[i] = function(X[i]);
    Y_vec.push_back(std::move(Y));
  }

  Draw(X, Y_vec, FindSolution(Y_vec), "conv");
}

void FunctionContainer::Draw(const std::vector<double>& X,const std::vector<std::vector<double>>& Y_vec,const std::list<int>& range, std::string last_Y){
  //Draw all function excluding Conv
  for(auto it = Y_vec.begin(); it != Y_vec.end() - 1; ++it)
    plt::plot(X, *it);

  //Draw Conv
  if(last_Y != ""){
    plt::named_plot(last_Y, X, *(Y_vec.end() - 1), "b");
  } else {
    plt::plot(X, *(Y_vec.end() - 1));
  }

  //Paretto set 
  std::vector<double> X_range;
  X_range.reserve(range.size());
  for(auto c: range)
    X_range.push_back(X[c]);

  for(int i = 0; i < Y_vec.size() - 1; ++i){
    std::vector<double> Y_range;
    Y_range.reserve(range.size());
    for(auto c: range)
      Y_range.push_back(Y_vec[i][c]);

    plt::plot(X_range, Y_range, "ro");
  }

  //lib function call
  plt::plot({point.first}, {point.second}, "g*");
  plt::legend();
  plt::show();
}

std::list<int> FunctionContainer::FindSolution(const std::vector<std::vector<double>>& Y_vec){
  std::list<int> range(Y_vec[0].size());
  std::iota(range.begin(), range.end(), 0);
  for(auto it = range.begin(); it != range.end(); ++it){
    for(auto it2 = range.begin(); it2 != range.end(); it2){
      if(it != it2){
        bool delete_index = true;
        for(int i = 0; i < Y_vec.size() - 1; ++i)
          if(Y_vec[i][*it2] <= Y_vec[i][*it])
            delete_index = false;
        if(delete_index)
          it2 = range.erase(it2);
        else
          ++it2;
      } else {
        ++it2;
      }
    }
  }

  return range;
}
