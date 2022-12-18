#include <vector>
#include <iostream>

template<typename ...T, typename F>
bool is_all_equal(F f, T... args){
    return ( (f == args) && ...);
}

template<typename T>
void print_vec(std::vector<T> vec)
{
    for(auto c : vec){
        std::cout << c << " ";
    }
    std::cout <<"\n";
}