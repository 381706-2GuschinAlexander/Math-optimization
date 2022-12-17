

template<typename ...T, typename F>
bool is_all_equal(F f, T... args){
    return ( (f == args) && ...);
}
