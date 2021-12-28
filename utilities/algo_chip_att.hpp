#ifndef RD_UTILITIES_ALGO_CHIP_ATT_H
#define RD_UTILITIES_ALGO_CHIP_ATT_H

#include "global_header.h"
#include <cmath>

// HMC624
namespace ns_hmc624 {

SYM_INLINE void att(double *x)
{ *x = (int32_t(*x * 10) / 5) * 5 / 10.0; }

} // namespace ns_hmc624

namespace ns_pe43704 {

// att to truth table/bit,0.25dB step
SYM_INLINE uint8_t att_to_bit_0_25(const double att)
{ return uint8_t(round(att * 4)); }

SYM_INLINE double bit_0_25_to_att(const uint8_t bit)
{ return bit / 4.0; }

} // namespace ns_pe43704

#endif // RD_UTILITIES_ALGO_CHIP_ATT_H
