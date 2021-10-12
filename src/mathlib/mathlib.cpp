#include "mathlib.h"


double optf::StronginMethod(FunctionParser& parser, double x0, double x1, double L){
  FunctionParser local_parser = parser;
  local_parser.ForceDeepCopy();
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
  int num_of_iter = (x1 - x0) / h;
  std::vector<double> X(num_of_iter);
   for (int i = 0; i < num_of_iter; ++i)
    X[i] = x0 + i * h;
  for (auto &func : func_vector)
  {
    std::vector<double> Y(num_of_iter);
    
    for (int i = 0; i < num_of_iter; ++i)
      Y[i] = func.Eval(&X[i]);
    plt::plot(X, Y);
  }

  plt::show();
}