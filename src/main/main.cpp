#include <cmath>
#include <iostream>

#include "matplotlibcpp.h"
#include "fparser.hh"

namespace plt = matplotlibcpp;

float func(float x) {
  return x + sin(x);
}



int main(int argc, char** argv){
  FunctionParser fp;
  fp.Parse("x+sin(x)", "x");
  double X = 10;
  std::cout << fp.Eval(&X) << std::endl;
  
  plt::plot({ 1,3,2,4 });
  plt::show();
  return 0;
}
