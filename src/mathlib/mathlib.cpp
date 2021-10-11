#include "mathlib.h"

FunctionContainer::FunctionContainer(const std::vector<std::string> &str_func_vector, const std::string &str_arg)
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
  std::vector<double> X(5000);
  std::vector<double> Y(5000);
  double x0 = -1;
  double h = 0.005;
  for(int i =0; i < 5000; ++i)
  {
    X[i] = x0 + i * h;
    Y[i] = func_vector[0].Eval(&X[i]);
  }
  plt::named_plot("function 1", X, Y);
  plt::show();
}