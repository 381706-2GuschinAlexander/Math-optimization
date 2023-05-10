
#include <string>
#include <vector>
#include <iostream>

#include "mathlib.h"
#include "beam.h"

int main(int argc, char **argv)
{
    if(argc != 3) throw std::exception();

    std::vector<double> coef_vec = { std::stod(argv[1]),  std::stod(argv[2]) };

    std::cout << "Coef: " << coef_vec[0] << " " << coef_vec[1] << "\n";

    std::vector<std::pair<double, double>> func_range;

    func_range.push_back(std::make_pair(10, 80));
    func_range.push_back(std::make_pair(10, 50));
    func_range.push_back(std::make_pair(0.9, 5));
    func_range.push_back(std::make_pair(0.9, 5));
    auto f1 = get_f1();
    auto f2 = get_f2();
    auto pen = get_pen();
    std::vector<function> fs = {f1, f2};

    std::vector<double> step = {1, 0.5, 0.1, 0.1};
    FunctionContainer MO_method(fs, func_range, 1e-2, step, pen);
    auto res = MO_method.Convolution(coef_vec);
    for (auto c : res.return_point)
        std::cout << c << " ";
    std::cout << "\n";
    std::cout << "penalty " << pen(res.return_point) << "\n";

    std::cout << "result f1 " << f1(res.return_point) << "\n";
    std::cout << "result f2 " << f2(res.return_point) << "\n\n";
    
}