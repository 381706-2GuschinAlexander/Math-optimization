#include <cmath>
#include <iostream>
#include <vector>
#include <string>
#include <functional>


#include "matplotlibcpp.h"
#include "fparser.hh"

namespace plt = matplotlibcpp;


namespace optf{
  double StronginMethod(std::function<double(double)> function, double x0, double x1, double r);
};

class FunctionContainer
{
private:
  std::vector<FunctionParser> func_vector;
  std::vector<std::pair<double,double>> range_vec;
  int operation_id;
public:
  FunctionContainer(const std::vector<std::string>& str_func_vector, const std::string& str_arg, std::vector<std::pair<double,double>>& range);
  void Convolution(
    //const std::vector<float> &conv_arg
  );
};
