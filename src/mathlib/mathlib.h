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
#include "omp.h"

namespace plt = matplotlibcpp;

namespace optf{
  struct MetaData{
    std::vector<double> return_point;
    int num_iteration;
    double value;
    MetaData(const std::vector<double>& vec, double val, int n);
  };
  MetaData StronginMethod(std::function<double(double)> function, double x0, double x1, double r, double eps);
};

class FunctionContainer
{
private:
  //for drawing
  std::function<double(double)> function;
  std::pair<double, double> point;
  //
  std::vector<FunctionParser> func_vector;
  std::vector<std::pair<double,double>> range_vec;
  int operation_id;
  std::list<int> FindSolution(const std::vector<std::vector<double>>& Y_vec);
  void Draw(const std::vector<double>& X,const std::vector<std::vector<double>>& Y_vec, const std::list<int>& range, std::string last_Y = "");
public:
  FunctionContainer(const std::vector<std::string>& str_func_vector, const std::string& str_arg, std::vector<std::pair<double,double>>& range);
  optf::MetaData Convolution(const std::vector<double> &conv_arg, double eps);
  void DrawPlot(const std::vector<double> &conv_arg, double eps, std::string last_Y = "");
};
