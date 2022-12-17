#pragma once

#include <cmath>
#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
#include <cfloat>
#include <numeric>
#include <algorithm>
#include <iterator>
#include <list>


#include "iterator.h"
#include "utils.h"
#include "omp.h"


namespace optf{
  struct MetaData{
    std::vector<double> return_point;
    int num_iteration;
    double value;
    MetaData(const std::vector<double>& vec, double val, int n);
    MetaData();
    MetaData(const MetaData& data);
    MetaData& operator=(const MetaData& data);
  };
  MetaData StronginMethod(std::function<double(double)> function, double x0, double x1, double r, double eps);
};

using function = std::function<std::vector<double>&(double)>;
using range_v = std::vector<std::pair<double,double>>;

class FunctionContainer
{
private:
  std::vector<function> _func_vector;
  std::vector<double> _min_vec;
  std::vector<double> _max_vec;
  double _eps;
  std::vector<double> _step;
  int operation_id;

  //
  std::tuple<std::vector<double>, std::vector<double>> GetMinMax();
public:
  FunctionContainer(const std::vector<function>& str_func_vector, const range_v& range, double eps, std::vector<double>& step);

  
  optf::MetaData Convolution(const std::vector<double> &conv_arg);
  void DrawPlot(const std::vector<double> &conv_arg, std::string last_Y = "");
};
