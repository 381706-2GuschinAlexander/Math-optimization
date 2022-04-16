#include "mathlib.h"
#include <iostream>
#include <chrono>

int main(){
  std::string str_arg = "x,y";
  std::vector<std::string> func_vec = {"sin(x*y)", "0.3*x*y", "(x^(1/2))*y"};
  std::vector<double> coef_vec = {1, 1, 1};
  std::vector<std::pair<double,double>> func_range;
  func_range.push_back(std::make_pair(1, 100));
  func_range.push_back(std::make_pair(1, 2));
  auto cont = FunctionContainer(func_vec, str_arg, func_range, 1e-3, 0.01);

  auto begin = std::chrono::steady_clock::now();
  auto old_v = cont.Convolution(coef_vec);
  auto end = std::chrono::steady_clock::now();

  std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms\n";
  std::cout << ((abs(old_v.return_point[0] - 4.64057 ) < 1e-5) ? "True" : "False" )<< "\n";

  //cont.DrawPlot(coef_vec, 1e-3, "Conv");
  return 0;
}