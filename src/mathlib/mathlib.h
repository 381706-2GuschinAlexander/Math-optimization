#include <cmath>
#include <iostream>
#include <vector>
#include <string>

#include "matplotlibcpp.h"
#include "fparser.hh"

namespace plt = matplotlibcpp;

class FunctionContainer
{
private:
  std::vector<FunctionParser> func_vector;
  int operation_id;
public:
  FunctionContainer(const std::vector<std::string> &str_func_vector, const std::string &str_arg);
  void Convolution(
    //const std::vector<float> &conv_arg
  );
};
