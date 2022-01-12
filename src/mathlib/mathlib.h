#pragma once

#include <cmath>
#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
#include <cfloat>
#include <numeric>
#include <list>


#include "matplotlibcpp.h"
#include "fparser.hh"

namespace plt = matplotlibcpp;

namespace optf{
  struct MetaData{
    std::vector<double> return_point;
    int num_iteration;
    double value;
    MetaData(std::vector<double> vec, double val, int n);
  };
  MetaData StronginMethod(std::function<double(double)> function, double x0, double x1, double r, double eps);
};

struct DrawData{
  std::vector<double> X;
  std::vector<std::vector<double>> Y_vec;
  std::list<int> range;
  std::pair<double, double> points;
  std::function<double(double)> function;
  void Clear();
  void Draw(std::string last_Y = "");
};

class FunctionContainer
{
private:
  DrawData draw;
  std::vector<FunctionParser> func_vector;
  std::vector<std::pair<double,double>> range_vec;
  int operation_id;
  std::list<int> FindSolution();
public:
  FunctionContainer(const std::vector<std::string>& str_func_vector, const std::string& str_arg, std::vector<std::pair<double,double>>& range);
  optf::MetaData Convolution(const std::vector<double> &conv_arg, double eps);
  void DrawPlot(const std::vector<double> &conv_arg, double eps, bool DataDel = true, std::string last_Y = "");
};
