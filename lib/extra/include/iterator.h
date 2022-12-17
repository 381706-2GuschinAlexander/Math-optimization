#include <vector>


class Iterator{
    std::vector<double> _max_vec;
    std::vector<double> _min_vec;

    std::vector<double> _val_vec;
    std::vector<double> _step_vec;

    bool _not_done = true;
public:
    Iterator(const std::vector<double>& min_vec, const std::vector<double>& max_vec, const std::vector<double>& step_vec) {
        if(!(max_vec.size() == min_vec.size() && min_vec.size() == step_vec.size()))
            throw std::exception();

        _max_vec = max_vec;
        _min_vec = min_vec;
        _val_vec = min_vec;
        _step_vec = step_vec;
    }

    Iterator& operator++(){
        for(int i = 0; i < _step_vec.size(); ++i){

            _val_vec[i] += _step_vec[i];

            if(_val_vec[i] > _max_vec[i])
            {
                _val_vec[i] = _min_vec[i];
                if (i == _step_vec.size() - 1) 
                    _not_done = false;
            }
            else
                break;

            
        }

        return *this;
    }

    bool notDone(){
        return _not_done;
    }

    const std::vector<double>& getVec(){
        return _val_vec;
    }

};