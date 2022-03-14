#ifndef COMPLEX_SEQUENCE_H
#define COMPLEX_SEQUENCE_H

#include "global_header.h"
#include "analyzer/trace.hpp"
//#include "span.hpp"
#include "fftw3.h"

class API complex_sequence
{
public:
    complex_sequence();
    ~complex_sequence();

public:
    void _new(uint32_t samples);

    SYM_INLINE size_t samples() const { return _i.size(); }

    double rbw() const { return double(_sr) / double(samples()); }

    void set_full_scale(double full_scale) { _full_scale = full_scale; }
    SYM_INLINE double full_scale() const { return _full_scale; }

    void set_sr(uint64_t sr);
    SYM_INLINE uint64_t sr() const { return _sr; }

    const int16_t *i() const { return _i.size() ? &(_i.at(0)) : nullptr; }
    const int16_t *q() const { return _q.size() ? &(_q.at(0)) : nullptr; }

    int16_t *i() { return _i.size() ? &(_i[0]) : nullptr; }
    int16_t *q() { return _q.size() ? &(_q[0]) : nullptr; }

    int16_t i(uint32_t idx) const { return _i.at(idx); }
    int16_t q(uint32_t idx) const { return _q.at(idx); }

    const double *normalized_freq() const { return &(_normalized_freq.at(0)); }
    double  normalized_freq(uint32_t idx) const { return _normalized_freq.at(idx); }

    const double *magnitude() const { return &(_magnitude.at(0)); }
    double *magnitude() { return &(_magnitude[0]); }
    double  magnitude(const size_t idx)   const { return _magnitude.at(idx); }
    double *magnitude(const double freq) const
    { return const_cast<double *>(magnitude()) + uint32_t(samples() * (freq + _sr / 2) / _sr); }

    double pwr_total() const;
    double pwr(const double freq_star,const double freq_stop) const;

    void dft();

    void trace_set_hold(trace_hold_t type) { _magnitude.set_hold_type(type); }

private:
    void update_normalized_freq();

public:
    trace<int16_t> _i;
    trace<int16_t> _q;
    trace<double>  _normalized_freq;
    trace<double>  _magnitude;

    double _full_scale;
    uint64_t _sr;

private:
    fftw_complex *_dft_in;
    fftw_complex *_dft_out;
};

#endif
