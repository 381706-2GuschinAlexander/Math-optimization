
#include <string>
#include <vector>
#include <iostream>

#include "mathlib.h"


int main(int argc, char **argv)
{
    std::cout << "sin(x + 1) * x from 0 to 20\n";  
    auto res = optf::StronginMethod([](double x){return sin(x + 1) * x; }, 0, 20, 10, 1e-4);

    std::cout << res.value << ":\t";
    for (auto c : res.return_point) {
        std::cout << c << " ";
    }
    std::cout << "\n";


    std::cout << " (4*x*x*x - 6*x*x + 1)/((x+1)*(3-x))\n";  
    res = optf::StronginMethod([](double x){return (4*x*x*x - 6*x*x + 1)/((x+1)*(3-x)); }, -0.4, 1.5, 20, 1e-4);

    std::cout << res.value << ":\t";
    for (auto c : res.return_point) {
        std::cout << c << " ";
    }
    std::cout << "\n";


    auto la = [](const std::vector<double>& x){
        return x[0]*x[0] - x[1]*x[1];
    };

    auto pen = [](const std::vector<double>& x){
        return 0;
    };

    std::vector<std::pair<double, double>> func_range;

    func_range.push_back(std::make_pair(-1.2, 1));
    func_range.push_back(std::make_pair(-1, 0.5));

    std::vector<double> step = {1, 0.01};
    std::vector<function> fs = {la};


    std::cout << "x^2 - y^2\n";
    FunctionContainer MO_method(fs, func_range, 1e-5, step, pen);
    res = MO_method.Convolution({1});

    std::cout << res.value << ":\t";
    for (auto c : res.return_point) {
        std::cout << c << " ";
    }
    std::cout << "\n";





    auto la1 = [](const std::vector<double>& x){
        return x[0]*x[0]*x[0];
    };

    auto la2 = [](const std::vector<double>& x){
        return (x[0] - 1)*(x[0] - 1);
    };

    func_range = {};
    func_range.push_back(std::make_pair(-3, 3));

    step = {1};
    fs = {la1, la2};


    std::cout << "x^3 +  (x-1)^2\n";
    FunctionContainer MO_method2(fs, func_range, 1e-5, step, pen);
    res = MO_method2.Convolution({1, 3});

    std::cout << res.value << ":\t";
    for (auto c : res.return_point) {
        std::cout << c << " ";
    }
    std::cout << "\n";
}