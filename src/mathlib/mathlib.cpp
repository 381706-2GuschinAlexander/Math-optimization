#include "mathlib.h"


float func(float x) {
  FunctionParser fp;
  fp.Parse("x+sin(x)", "x");
  double X = 10;
  plt::plot({ 1,3,2,4 });
  plt::show();
  return fp.Eval(&X);
}



