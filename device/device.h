#ifndef DEVICE_DEVICE_H
#define DEVICE_DEVICE_H

#include "eutra/band.hpp"
#include "rd_types.h"

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

API int32_t radio_api_version(char **version);
API int32_t radio_set_log_switch(const uint32_t sw);
API int32_t radio_set_log_switch_enable(const radio_log_switch_t &sw);
API int32_t radio_set_exception_en(const bool en);

API int32_t radio_boot();
API int32_t radio_get_sn(char *sn);
API int32_t radio_set_io_mode(const uint32_t index,const radio_io_mode_t mode);

API int32_t radio_set_tx_state(const uint32_t index,const bool state);
API int32_t radio_set_tx_power(const uint32_t index,const double power);
API int32_t radio_set_tx_frequency(const uint32_t index,const uint64_t freq);
API int32_t radio_get_tx_frequency(const uint32_t index,uint64_t *freq);

API int32_t radio_set_rx_level(const uint32_t index,const double level);
API int32_t radio_get_rx_level(const uint32_t index,double *level);
API int32_t radio_set_rx_frequency(const uint32_t index,const uint64_t freq);
API int32_t radio_get_rx_frequency(const uint32_t index,uint64_t *freq);

API int32_t radio_set_iq_capture_buffer(const uint32_t index,int16_t **i,int16_t **q);
API int32_t radio_set_iq_capture_start(const uint32_t index);
API int32_t radio_set_iq_capture_abort(const uint32_t index);

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif
