#include "trace.hpp"

//template<typename T>
//trace<T>::trace(const size_t size)
//{
//    _size = 0;

//    allocate(size);

//    _hold = trace_hold_t::Normal;
//    _avg_time = 0;
//    _avg_time_cur = 0;
//}

template<typename T>
trace<T>::~trace()
{
    _size = 0;
}

template<typename T>
void trace<T>::set_with_hold_type(const std::vector<T> &sequence)
{
    size_t i = 0;

    if (_hold == trace_hold_t::Normal) {
        _sequence.assign(sequence.begin(),sequence.end());
    } else if (_hold == trace_hold_t::Average) {

    } else if (_hold == trace_hold_t::Max_Hold) {
        for (;i < sequence.size();++i) {
            set_with_hold_type_max(i,sequence[i]);
        }
    } else {
        for (;i < sequence.size();++i) {
            set_with_hold_type_min(i,sequence[i]);
        }
    }
}

template<typename T>
void trace<T>::set_with_hold_type(const size_t idx,const T &sample)
{
    typedef void (trace<T>::*fp_set_with_hold_type)(const size_t idx,const T &sample);

    static fp_set_with_hold_type funcs[trace_hold_t::End] = {
        &trace::set_with_hold_type_normal,
        &trace::set_with_hold_type_average,
        &trace::set_with_hold_type_max,
        &trace::set_with_hold_type_min
    };

    (this->*funcs[_hold])(idx,sample);
}

template<typename T>
void trace<T>::set_with_hold_type_normal(const size_t idx,const T &sample)
{
    _sequence[idx] = sample;
}

template<typename T>
void trace<T>::set_with_hold_type_average(const size_t idx,const T &sample)
{

}

template<typename T>
void trace<T>::set_with_hold_type_max(const size_t idx,const T &sample)
{
    if (_sequence[idx] < sample) {
        _sequence[idx] = sample;
    }
}

template<typename T>
void trace<T>::set_with_hold_type_min(const size_t idx,const T &sample)
{
    if (_sequence[idx] > sample) {
        _sequence[idx] = sample;
    }
}

template<typename T>
bool trace<T>::allocate(const size_t size)
{
    if (size > _sequence.capacity()) {
        _sequence.resize(size);
    }
    _size = size;
    return true;
}
