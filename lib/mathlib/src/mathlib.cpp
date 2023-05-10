#include "mathlib.h"

const double SM_const_R = 50;  

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
  const int N = 70;
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
    double  upper = *lower_x;
    double  bottom = x_new;
    x.insert(lower_x, x_new);
    
    M_max = std::max({M_max, get_M(ROI), get_M(ROI + 1)});
    
    if(Q[ROI + 1] < func_val_at_min){
      global_min = x_new;
      func_val_at_min = Q[ROI + 1];
    }

    if (upper - bottom < eps)
      return MetaData({global_min}, function(global_min), n);
      
    ++n;
  }
  
  return MetaData({global_min}, function(global_min), n);
}

FunctionContainer::FunctionContainer(const std::vector<function>& str_func_vector, const range_v& range, double eps, const std::vector<double>& step, function error_func)
{
  if (range.size() !=  step.size())
    throw std::exception();

  _eps = eps;

  _func_vector = str_func_vector;
  
  for(auto pair : range){
    _min_vec.push_back(pair.first);
    _max_vec.push_back(pair.second);
  }
  _error_func = error_func;
  _step = step;
}

std::tuple<std::vector<double>, std::vector<double>> FunctionContainer::GetMinMax(){
  int func_vec_size = _func_vector.size();
  std::vector<double> max_val(func_vec_size, -DBL_MAX);
  std::vector<double> min_val(func_vec_size, DBL_MAX);

  auto min_vec = std::vector<double>(_min_vec.begin() + 1, _min_vec.end());
  auto max_vec = std::vector<double>(_max_vec.begin() + 1, _max_vec.end());
  auto step    = std::vector<double>(   _step.begin() + 1, _step.end());

  for(int i = 0; i < func_vec_size; ++i){

    for(Iterator it = Iterator(min_vec,max_vec, step); it.notDone(); ++it){
      std::vector<double> local_args = it.getVec();
      local_args.insert(local_args.begin(), 0);

      auto la_min = [&](double x) -> double
      {
        local_args[0] = x;
        return _func_vector[i](local_args);
      };

      auto la_max = [&](double x) -> double
      {
        local_args[0] = x;
        return -_func_vector[i](local_args);
      };

      if(double val = optf::StronginMethod(la_min, _min_vec[0], _max_vec[0], SM_const_R, _eps).value; val < min_val[i])
          min_val[i] = val;
      if(double val = -optf::StronginMethod(la_max, _min_vec[0], _max_vec[0], SM_const_R, _eps).value; val > max_val[i])
          max_val[i] = val;
    }
  }

  return std::make_tuple(min_val, max_val);
}

optf::MetaData FunctionContainer::Convolution(const std::vector<double> &conv_arg)
{
  //print_vec(conv_arg);
  optf::MetaData result;
  double coef_sum = 0;
  for(auto c: conv_arg)
    coef_sum += c;

  auto [min_val, max_val] = GetMinMax();
  double min_conv = DBL_MAX;

  auto min_vec = std::vector<double>(_min_vec.begin() + 1, _min_vec.end());
  auto max_vec = std::vector<double>(_max_vec.begin() + 1, _max_vec.end());
  auto step    = std::vector<double>(   _step.begin() + 1, _step.end());
  
  for(Iterator it = Iterator(min_vec,max_vec, step); it.notDone(); ++it){
    //auto local_f = getFunction(_func_vector[i], it.getVec());

    std::vector<double> local_args = it.getVec();
    local_args.insert(local_args.begin(), 0);


    auto la = [=](double x) mutable -> double
    {
      local_args[0] = x;
      double res = 0;
      for (int i = 0; i < _func_vector.size(); ++i)
      {
        res += (conv_arg[i] / coef_sum) * ((_func_vector[i](local_args) - min_val[i]) / (max_val[i] - min_val[i]));
      }
      res += _error_func(local_args);
      return res;

    };


    if(auto meta = optf::StronginMethod(la, _min_vec[0], _max_vec[0], SM_const_R, _eps); meta.value < min_conv){
      result = meta;
      min_conv = result.value;
      for (auto points = local_args.begin()+1; points != local_args.end(); ++points)
        result.return_point.push_back(*points);
    }
  }

  return result;
}

