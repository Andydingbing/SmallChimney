#ifndef RF_DRIVER_H
#define RF_DRIVER_H

#include "rd_sp9500.h"
#include "rd_sp9500x.h"

API int32_t RF_DriverVersion(char **Version);
API int32_t RF_SetLogSwitch(const uint32_t Switch);
API int32_t RF_SetLogSwitchEnable(const RD_Log_Switch &Switch);

#endif
