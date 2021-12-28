#ifndef RD_UTILITIES_ALGO_CHIP_ADC_H
#define RD_UTILITIES_ALGO_CHIP_ADC_H

#include "global_header.h"

namespace ns_ad7949 {

enum ref_t {
    INTERNAL_2_5_V = 0,
    INTERNAL_4_096_V = 1,
    EXTERNAL_TEMP = 2,
    EXTERNAL_TEMP_INTERNAL = 3,
    EXTERNAL = 6,
    EXTERNAL_INTERNAL = 7
};

SYM_INLINE double ad_to_voltage_mv(int16_t ad,ref_t ref = INTERNAL_4_096_V,double ext_ref_v = 0.0)
{
    if (ref == ns_ad7949::INTERNAL_2_5_V) {
        return ad / 16383.0 * 2500;
    } else if (ref == ns_ad7949::INTERNAL_4_096_V) {
        return ad / 16383.0 * 4096;
    }
    return ad / 16383.0 * ext_ref_v * 1000.0;
}

} // namespace ns_ad7949

#endif // RD_UTILITIES_ALGO_CHIP_ADC_H
