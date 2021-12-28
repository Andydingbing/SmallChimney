#ifndef RD_UTILITIES_ALGO_CHIP_TEMP_SENSOR_H
#define RD_UTILITIES_ALGO_CHIP_TEMP_SENSOR_H

#include "global_header.h"

namespace ns_tc1047 {

SYM_INLINE double voltage_to_temp(double vol_mv)
{ return (vol_mv - 500.0) / 10.0; }

} // namespace ns_tc1047

#endif // RD_UTILITIES_ALGO_CHIP_TEMP_SENSOR_H
