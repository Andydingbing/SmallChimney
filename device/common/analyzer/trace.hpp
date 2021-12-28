#ifndef COMMON_ANALYZER_TRACE_HPP
#define COMMON_ANALYZER_TRACE_HPP

#include "global_header.h"
#include "enum.h"
#include <vector>

BETTER_ENUM(trace_hold_t, uint32_t, Normal = 0, Average, Max_Hold, Min_Hold, End)

template<typename T>
class trace
{
public:
    trace(const size_t size = 0) {
        _size = 0;

        allocate(size);

        _hold = trace_hold_t::Normal;
        _avg_time = 0;
        _avg_time_cur = 0;
    }
    ~trace();

    std::vector<T> *sequence() const { return &_sequence; }

    T& operator [] (const size_t idx) { return _sequence[idx]; }
    const T& at(const size_t idx) const { return _sequence.at(idx); }

    size_t size() const { return _size; }

    bool allocate(const size_t size);

    void set_hold_type(const trace_hold_t hold) { _hold = hold; }
    void set_avg_time(const uint32_t time) { _avg_time = time; _avg_time_cur = 0; }

    void set(const size_t idx,const T &sample) { _sequence[idx] = sample; }
    void set_with_hold_type(const std::vector<T> &sequence);
    void set_with_hold_type(const size_t idx,const T &sample);
    void set_with_hold_type_normal(const size_t idx,const T &sample);
    void set_with_hold_type_average(const size_t idx,const T &sample);
    void set_with_hold_type_max(const size_t idx, const T &sample);
    void set_with_hold_type_min(const size_t idx, const T &sample);

private:
    size_t _size;
    std::vector<T> _sequence;

    uint32_t _hold;
    uint32_t _avg_time;
    uint32_t _avg_time_cur;
};

#endif // COMMON_ANALYZER_TRACE_HPP
