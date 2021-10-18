#include "mathlib.h"


double optf::StronginMethod(std::function<double(double)> function, double a, double b, double r){
  std::vector<double> x(2);
  x[0] = a;
  x[1] = b;
  double global_min = x[0];
  double func_val_at_min = function(global_min);

  int N = 100;


  std::vector<double> R(N);
  std::vector<double> M(N);
  int n = 2;
  while(n < N){
    
    for(int i = 0; i < n - 1; ++i)
      M[i] = abs(function(x[i + 1]) - function(x[i])) / (x[i+1] - x[i]);
      
    double M_max = *std::max_element(M.begin(), M.begin()+n);
    
    double m = M_max < 1.e-5 ? 1 : r * M_max;

    for(int i = 0; i < n - 1; ++i)
      R[i] = m * (x[i + 1] - x[i]) 
      + pow(function(x[i + 1]) - function(x[i]), 2./(m * (x[i + 1] - x[i])))
      - 2 * (function(x[i + 1]) + function(x[i]));

    int ROI = std::distance(R.begin(), std::max_element(R.begin(), R.begin()+n));
    double x_new = .5* (x[ROI + 1] + x[ROI]) - (function(x[ROI + 1]) - function(x[ROI]))/(2*m);
    x.push_back(x_new);
    std::sort(x.begin(), x.end());

    if(function(x_new) < func_val_at_min){
      global_min = x_new;
      func_val_at_min = function(x_new);
    }

    ++n;
  }

  return global_min;
}

FunctionContainer::FunctionContainer(const std::vector<std::string> &str_func_vector, const std::string &str_arg, std::vector<std::pair<double,double>>& range) :  range_vec(range)
{
  for (auto &str_func : str_func_vector)
  {
    FunctionParser tmp;
    tmp.Parse(str_func, str_arg);
    func_vector.push_back(std::move(tmp));
  }
}

void FunctionContainer::Convolution(
  //const std::vector<float> &conv_arg
  )
{
  
  double x0 = std::min(range_vec[0].first, range_vec[0].second);
  double x1 = std::max(range_vec[0].first, range_vec[0].second);
  double h = 0.005;
  size_t num_of_iter = (x1 - x0) / h;
  std::vector<double> X(num_of_iter);
   for (size_t i = 0; i < num_of_iter; ++i)
    X[i] = x0 + i * h;
  for (auto &func : func_vector)
  {
    std::vector<double> Y(num_of_iter);
    
    for (size_t i = 0; i < num_of_iter; ++i)
      Y[i] = func.Eval(&X[i]);
    plt::plot(X, Y);



    auto la = [&](double x)->double{
      return func.Eval(&x);
    };

    double x_min = optf::StronginMethod(la, x0, x1, 25);
    plt::plot({x_min}, {func.Eval(&x_min)}, "ro");
  }

  //plt::plot({1,2}, {1,2},"ro");
  plt::show();
}