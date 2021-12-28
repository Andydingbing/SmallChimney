#ifndef RD_UTILITIES_ALGO_CHIP_DAC_H
#define RD_UTILITIES_ALGO_CHIP_DAC_H

#include "global_header.h"

namespace ns_ltc2666 {

SYM_INLINE int32_t voltage_to_digital(const double v)
{ return int32_t(4095.0 * (v + 5.0) / 10.0); }

SYM_INLINE double digital_to_voltage(const int32_t d)
{ return -5.0 + 10.0 * d / 4095.0; }

}

namespace ns_ad9122 {

SYM_INLINE void iq_phase_adj_angel_to_reg(const double angel,uint16_t &reg)
{ reg = uint16_t(angel / 1.75 * 512.0); }

}

namespace ns_ad908x {

/*
 *         f_nco
 *  ftw = ------- * 2^48
 *          fs
 */
SYM_INLINE uint64_t ftw_integer_mode(const double f_nco,const double fs)
{ return uint64_t(f_nco / fs * (uint64_t(1) << 48)); }

SYM_INLINE double nco_integer_mode(const uint64_t ftw,const double fs)
{ return double(ftw) / (uint64_t(1) << 48) * fs; }

}

#endif // RD_UTILITIES_ALGO_CHIP_DAC_H
