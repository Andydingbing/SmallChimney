#ifndef DEVICE_ERICSSON_RADIO_6449_API_H
#define DEVICE_ERICSSON_RADIO_6449_API_H

#include "cal_file.h"
#include "../eutra/band.hpp"
#include "../rd_types.h"
#include "../device.h"

enum ericsson_radio_6449_kase {
    Begin = 0,
    TX_ACLR = 0,
    RX_Gain_Accu,
    TX_VGA,
    RX_RF_VGA,
    Total
};


namespace ns_ericsson {
namespace ns_radio_6449 {

#define Radio radio::instance()

class API radio : public radio_base
{
public:
    static radio &instance();

public:
    uint32_t channels() OVERRIDE;
    void set_init_callback(int32_t (*callback)()) OVERRIDE;
    void set_log_callback(void (*callback)()) OVERRIDE;

    int32_t set_sn(const uint32_t index,const char *sn) OVERRIDE;
    int32_t get_sn(const uint32_t index,char *sn) OVERRIDE;

    const item_table_base* data_base(const uint32_t index,const int32_t kase) const OVERRIDE;
    int32_t data_base_add(const uint32_t index,const int32_t kase,void *data) OVERRIDE;
    int32_t prepare_kase(const uint32_t index,const int32_t kase,const std::string freq_str,const bool is_exp) OVERRIDE;

    int32_t init() OVERRIDE;

public:
    uint32_t    com_loggers();
    const char* com_logger_write(const uint32_t n);
    const char* com_logger_read(const uint32_t n);
    const char* com_logger_time(const uint32_t n);
    int32_t     com_logger_result(const uint32_t n);

    cal_file* data_base(const uint32_t index) const;

    int32_t serial_write(const char *str);
    int32_t serial_write(const std::string &str);

public:
    int32_t set_tx_frequency(const uint32_t index,const uint64_t freq);
    int32_t txon(const uint32_t index);
    int32_t txoff(const uint32_t index);
    int32_t txtype(const uint32_t index,const radio_system_t &system,const uint64_t bw);
    int32_t txstepattmain(const uint32_t index,const double att);
    int32_t txattmain(const uint32_t index,const uint32_t dac);
    int32_t pabias(const uint32_t index);
    int32_t pabias();
    int32_t pacm(const uint32_t index);

public:
    int32_t set_rx_frequency(const uint32_t index,const uint64_t freq);
    int32_t rx(const uint32_t index,const uint64_t freq);
    int32_t rxtype(const uint32_t index,const radio_system_t &system,const uint64_t bw);
    int32_t rxrfsw(const uint32_t index,const bool en_lna,const bool en_att);
    int32_t rxrfvga(const uint32_t index,const double att);
    int32_t rxrfvgaswp(const uint32_t index,const std::string &str,void *data = nullptr);
    int32_t rxagc(const uint32_t index,const bool en);
    int32_t rxcpriconf(const uint32_t index);

    int32_t biasctrl(const uint32_t index,const bool en);
    int32_t ccctrl(const uint32_t index,const bool en);

public:
    int32_t iqcomp3(const uint32_t index);
    int32_t intdldcw(const uint32_t index,const double pwr,const bool en);
    int32_t rxulg(const uint32_t index,const double gain);
    int32_t ulil(const uint32_t index,double *pwr);
    int32_t ulils(double *a,double *b,double *c,double *d);
    int32_t mpa(const uint32_t index,const bool en);
};

} // namespace ns_radio_6449
} // namespace ns_ericsson

#endif
