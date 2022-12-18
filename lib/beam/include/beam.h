#include <functional>
#include <vector>
#include <cmath>

std::function<double(std::vector<double>)> get_f1(){

    return [=](const std::vector<double>& x){
        return 2*x[1]*x[3] + x[2]*(x[0] - 2 * x[3]);
    };
}

std::function<double(std::vector<double>)> get_f2(){
    double E = 2e4;
    double P = 600;
    double Q = 50;
    double L = 200;

    auto I = [](const std::vector<double>& x){
        return (
            x[2] * pow((x[0] - 2 * x[3]),  3) + 
            2*x[1]*x[3]*(4*pow(x[3], 2) + 3*x[0]*(x[0] - 2*x[3]))
            )/12;
    };

    return [=](const std::vector<double>& x){
        return P*pow(L, 3) / (48*E*I(x));
    };
}

std::function<double(std::vector<double>)> get_pen()
{
    double MY = 30e3;
    double MZ = 25e3;
    double sigma = 16;

    return [=](const std::vector<double> &x)
    {
        auto WY = [](const std::vector<double> &x){
            return (
                       x[2] * pow((x[0] - 2 * x[3]), 3) +
                       2 * x[1] * x[3] * (4 * pow(x[3], 2) + 3 * x[0] * (x[0] - 2 * x[3]))) /
                   (6 * x[0]);
        };

        auto WZ = [](const std::vector<double> &x){
            return (
                       (x[0] - 2 * x[3]) * pow(x[2], 3) +
                       2 * x[3] * pow(x[1], 3)) /
                   (6 * x[1]);
        };

        return std::max(0.0, MY / WY(x) + MZ / WZ(x) - sigma);
    };
}