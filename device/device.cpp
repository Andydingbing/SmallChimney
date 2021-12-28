#include "device.h"
#include "device_ericsson_radio_4415.h"
#include "gen_ini_file.inline.hpp"
#include "algo_math.inline.hpp"
#include "algo_chip_pll.inl.hpp"
#include "memory_allocator.inline.hpp"
#include "memory_physical.inline.hpp"

static const char *ver = "1.1.15330";

int32_t radio_api_version(char **Version)
{
    *Version = const_cast<char *>(ver);
    return 0;
}

int32_t radio_set_exception_en(const bool en)
{
    return 0;
}

int32_t radio_boot()
{
    return ericsson_radio_4415_init();
}

int32_t radio_get_sn(char *sn)
{
    return 0;
}

int32_t radio_set_io_mode(const uint32_t index,const radio_io_mode_t mode)
{
    return 0;
}

int32_t radio_set_tx_state(const uint32_t index,const bool state)
{
    return 0;
}

int32_t radio_set_tx_power(const uint32_t index,const double power)
{
    return 0;
}

int32_t radio_set_tx_frequency(const uint32_t index,const uint64_t freq)
{
    ericsson_radio_4415_set_tx_frequency(index,freq);
    return 0;
}

int32_t radio_get_tx_frequency(const uint32_t index,uint64_t *freq)
{
    return 0;
}

int32_t radio_set_rx_level(const uint32_t index,const double level)
{
    return 0;
}

int32_t radio_get_rx_level(const uint32_t index,double *level)
{
    return 0;
}

int32_t radio_set_rx_frequency(const uint32_t index,const uint64_t freq)
{
    ericsson_radio_4415_set_rx_frequency(index,freq);
    return 0;
}

int32_t radio_get_rx_frequency(const uint32_t index,uint64_t *freq)
{
    return 0;
}

int32_t radio_set_iq_capture_buffer(const uint32_t index,int16_t **i,int16_t **q)
{
    return 0;
}

int32_t radio_set_iq_capture_start(const uint32_t index)
{
    return 0;
}

int32_t radio_set_iq_capture_abort(const uint32_t index)
{
    return 0;
}
