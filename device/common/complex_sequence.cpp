#include "complex_sequence.h"
#include <string.h>
#include <complex>
#include <math.h>
#include "algo_math.hpp"

#include "trace.inline.hpp"

static boost::mutex g_complex_sequence_lock;

complex_sequence::complex_sequence()
{
    _full_scale = 134217728.0; // i/q = 8192,i^2+q^2
    _sr = 245760000;

    _dft_in = nullptr;
    _dft_out = nullptr;
    update_normalized_freq();
}

complex_sequence::~complex_sequence()
{
    fftw_free(_dft_in);
    fftw_free(_dft_out);
}

void complex_sequence::_new(uint32_t samples)
{
    g_complex_sequence_lock.lock();

    size_t samples_befor = this->samples();

    if (samples > this->samples()) {
        fftw_free(_dft_in);
        fftw_free(_dft_out);

        _dft_in  = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * samples);
        _dft_out = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * samples);

    }

    _i.allocate(samples);
    _q.allocate(samples);
    _normalized_freq.allocate(samples);
    _magnitude.allocate(samples);

    if (samples != samples_befor) {
        update_normalized_freq();
    }

    g_complex_sequence_lock.unlock();
}

void complex_sequence::set_sr(uint64_t sr)
{
    if (_sr != sr) {
        _sr = sr;
        update_normalized_freq();
    }
}

double complex_sequence::pwr_total() const
{
    uint64_t sum = 0;

    for (size_t i = 0;i < _i.size();++i) {
        sum += uint64_t(_i.at(i) * _i.at(i) + _q.at(i) * _q.at(i));
    }
    return ad_to_dBc(_full_scale,double(sum) / _i.size());
}

double complex_sequence::pwr(const double freq_star,const double freq_stop) const
{
    double sum = 0.0;
    double I = 0.0;
    double Q = 0.0;
    double normalized_freq_star = 0.0;
    double normalized_freq_step = 0.0;
    size_t idx_star = 0;
    size_t idx_stop = 0;
    size_t idx_zero = samples() / 2;

    if (_normalized_freq.size() > 1) {
        normalized_freq_star = _normalized_freq.at(0);
        normalized_freq_star *= 1e6;
        normalized_freq_step = _normalized_freq.at(1) - _normalized_freq.at(0);
        normalized_freq_step *= 1e6;
    } else {
        return -180.0;
    }

    idx_star = SERIE_INDEX(freq_star,normalized_freq_star,normalized_freq_step);
    idx_stop = SERIE_INDEX(freq_stop,normalized_freq_star,normalized_freq_step) + 1;

    if (freq_star <= 0.0 && freq_stop <= 0) {
        for (size_t i = idx_star;i < idx_stop;++i) {
            I = _dft_out[i + samples() / 2][0] / samples();
            Q = _dft_out[i + samples() / 2][1] / samples();
            sum += I * I + Q * Q;
        }
    } else if (freq_star >= 0 && freq_stop >= 0) {
        for (size_t i = idx_star - idx_zero;i < idx_stop - idx_zero;++i) {
            I = _dft_out[i][0] / samples();
            Q = _dft_out[i][1] / samples();
            sum += I * I + Q * Q;
        }
    } else {
        for (size_t i = idx_star;i < idx_zero;++i) {
            I = _dft_out[i + samples() / 2][0] / samples();
            Q = _dft_out[i + samples() / 2][1] / samples();
            sum += I * I + Q * Q;
        }
        for (size_t i = 0;i < idx_stop - idx_zero;++i) {
            I = _dft_out[i][0] / samples();
            Q = _dft_out[i][1] / samples();
            sum += I * I + Q * Q;
        }
    }

    return ad_to_dBc(_full_scale,double(sum) /*/ (idx_stop - idx_star)*/);
}

void complex_sequence::dft()
{
    fftw_plan plan = fftw_plan_dft_1d(int(samples()),_dft_in,_dft_out,FFTW_FORWARD,FFTW_ESTIMATE);

    for (uint32_t i = 0;i < samples();i ++) {
        _dft_in[i][0] = double(_i[i]);
        _dft_in[i][1] = double(_q[i]);
    }

    fftw_execute(plan);

    for (uint32_t i = 0;i < samples() / 2;i ++) {
        _magnitude.set_with_hold_type(i + samples() / 2,::magnitude(_dft_out[i][0]/samples(),_dft_out[i][1]/samples(),_full_scale));
        _magnitude.set_with_hold_type(i,::magnitude(_dft_out[i + samples() / 2][0]/samples(),_dft_out[i + samples() / 2][1]/samples(),_full_scale));
    }

    fftw_destroy_plan(plan);
}

void complex_sequence::update_normalized_freq()
{
    const size_t spls = samples();

    for (uint32_t i = 0;i < spls / 2;i ++) {
        // positive frequencys
        _normalized_freq[i + spls / 2] = _sr / 2.0 / double(spls / 2.0) * i / 1e6;

        //negative frequencys
        _normalized_freq[spls / 2 - 1 - i] = _sr / 2.0 / double(spls / 2.0) * i / 1e6 * -1;
    }
}
