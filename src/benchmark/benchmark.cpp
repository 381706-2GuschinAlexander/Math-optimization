#include "mathlib.h"
#include <iostream>
#include <chrono>

int main(){
  std::string str_arg = "x";
  std::vector<std::string> func_vec = {"sin(x)", "0.3*x", "x^(1/2)"};
  std::vector<double> coef_vec = {1, 1, 1};
  std::vector<std::pair<double,double>> func_range;
  func_range.push_back(std::make_pair(1, 100));
  auto cont = FunctionContainer(func_vec, str_arg, func_range);

  auto begin = std::chrono::steady_clock::now();
  auto old_v = cont.Convolution(coef_vec, 1e-3);
  auto end = std::chrono::steady_clock::now();

  std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms\n";
  std::cout << ((abs(old_v.return_point[0] - 4.64057 ) < 1e-5) ? "True" : "False" )<< "\n";
  return 0;
}