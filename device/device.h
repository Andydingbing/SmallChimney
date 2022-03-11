#ifndef DEVICE_DEVICE_H
#define DEVICE_DEVICE_H

#include "item_table.hpp"
#include "eutra/band.hpp"
#include "rd_types.h"

class API radio_base
{
public:
    virtual uint32_t channels() = 0;
    virtual void set_init_callback(int32_t (*callback)()) {}
    virtual void set_log_callback(void (*callback)()) {}

    virtual int32_t set_sn(const uint32_t index,const char *sn) = 0;
    virtual int32_t get_sn(const uint32_t index,char *sn) = 0;

    virtual const item_table_base* data_base(const uint32_t index,const int32_t kase) const = 0;
    virtual int32_t data_base_add(const uint32_t index,const int32_t kase,void *data) = 0;
    virtual int32_t prepare_kase(const uint32_t index,const int32_t kase,const std::string freq_str,const bool is_exp) = 0;

    virtual int32_t init() = 0;
};

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
