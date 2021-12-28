#ifndef DEVICE_ERICSSON_RADIO_4415_API_H
#define DEVICE_ERICSSON_RADIO_4415_API_H

#include "cal_file.h"
#include "../eutra/band.hpp"
#include "../rd_types.h"
#include "../device_radio_c_api.h"
#include "../device_radio_cpp_api.h"
#include "../device_radio_cpp_constructor.h"

#if defined(__cplusplus) || defined(__cplusplus__)
    #define PRODUCT_PREFIX(vendor,product) vendor##_##product##_
    #define API_RADIO(vendor,product,...) \
        extern "C" { RADIO_C_API(__VA_ARGS__,vendor##_##product##_) } \
        namespace ns_##vendor { \
        namespace ns_##product { \
            class API radio_helper { public: \
                radio_helper() { RADIO_CPP_CONSTRUCTOR(__VA_ARGS__,vendor##_##product##_) } \
                RADIO_CPP_API(__VA_ARGS__) \
            }; \
            static radio_helper radio; \
        } /*namespace ns_##vendor*/  \
        } /*namespace ns_##product*/
#else
    #define API_RADIO(vendor,product,...) \
        RADIO_C_API(__VA_ARGS__,vendor##_##product##_)
#endif


enum ericsson_radio_4415_kase {
    Begin = 0,
    TX_ACLR = 0,
    RX_Gain_Accu,
    TX_VGA,
    RX_RF_VGA,
    Total
};


namespace ns_ericsson {
namespace ns_radio_4415 {

#define Radio radio::instance()

class API radio : noncopyable
{
public:
    static radio &instance();

public:
    void set_init_callback(int32_t (*callback)());
    void set_log_callback(void (*callback)());

    int32_t set_sn(const char *sn);
    int32_t get_sn(char *sn);

    uint32_t    com_loggers();
    const char* com_logger_write(const uint32_t n);
    const char* com_logger_read(const uint32_t n);
    const char* com_logger_time(const uint32_t n);
    int32_t     com_logger_result(const uint32_t n);

    cal_file* data_base(const uint32_t index) const;

    int32_t init();
    uint32_t channels();
    int32_t prepare_case(const uint32_t index,const ericsson_radio_4415_kase kase,const std::string freq_str,const bool is_exp);
    int32_t db_add(const uint32_t index,const ericsson_radio_4415_kase kase,void *data);
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

} // namespace ns_radio_4415
} // namespace ns_ericsson


#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

API void ericsson_radio_4415_set_init_callback(int32_t (*callback)());
API void ericsson_radio_4415_set_log_callback(void (*callback)());

API int32_t ericsson_radio_4415_set_sn(const char *sn);
API int32_t ericsson_radio_4415_get_sn(char *sn);

API uint32_t    ericsson_radio_4415_com_loggers();
API const char* ericsson_radio_4415_com_logger_write(const uint32_t n);
API const char* ericsson_radio_4415_com_logger_read(const uint32_t n);
API const char* ericsson_radio_4415_com_logger_time(const uint32_t n);
API int32_t     ericsson_radio_4415_com_logger_result(const uint32_t n);

API int32_t ericsson_radio_4415_init();
API uint32_t ericsson_radio_4415_channels();
API int32_t ericsson_radio_4415_prepare_case(const uint32_t index,const ericsson_radio_4415_kase kase,const char *freq_str,const bool is_exp);
API int32_t ericsson_radio_4415_db_add(const uint32_t index,const ericsson_radio_4415_kase kase,void *data);

API int32_t ericsson_radio_4415_serial_write(const char *str);

API int32_t ericsson_radio_4415_set_tx_frequency(const uint32_t index,const uint64_t freq);
API int32_t ericsson_radio_4415_txon(const uint32_t index);
API int32_t ericsson_radio_4415_txoff(const uint32_t index);
API int32_t ericsson_radio_4415_txtype(const uint32_t index,const radio_system_t &system,const uint64_t bw);
API int32_t ericsson_radio_4415_txstepattmain(const uint32_t index,const double att);
API int32_t ericsson_radio_4415_txattmain(const uint32_t index,const uint32_t dac);
API int32_t ericsson_radio_4415_pabias(const uint32_t index);
API int32_t ericsson_radio_4415_all_pabias();
API int32_t ericsson_radio_4415_pacm(const uint32_t index);

API int32_t ericsson_radio_4415_set_rx_frequency(const uint32_t index,const uint64_t freq);
API int32_t ericsson_radio_4415_rx(const uint32_t index,const uint64_t freq);
API int32_t ericsson_radio_4415_rxtype(const uint32_t index,const radio_system_t &system,const uint64_t bw);
API int32_t ericsson_radio_4415_set_rx_lna_sw(const uint32_t index,const bool en);
API int32_t ericsson_radio_4415_set_rx_att_sw(const uint32_t index,const bool en);
API int32_t ericsson_radio_4415_rxrfsw(const uint32_t index,const bool en_lna,const bool en_att);
API int32_t ericsson_radio_4415_rxrfvga(const uint32_t index,const double att);
API int32_t ericsson_radio_4415_rxrfvgaswp(const uint32_t index,const char *str,void *data);
API int32_t ericsson_radio_4415_rxagc(const uint32_t index,const bool en);
API int32_t ericsson_radio_4415_rxcpriconf(const uint32_t index);

API int32_t ericsson_radio_4415_biasctrl(const uint32_t index,const bool en);
API int32_t ericsson_radio_4415_ccctrl(const uint32_t index,const bool en);

API int32_t ericsson_radio_4415_iqcomp3(const uint32_t index);
API int32_t ericsson_radio_4415_intdldcw(const uint32_t index,const double pwr,const bool en);
API int32_t ericsson_radio_4415_rxulg(const uint32_t index,const double gain);
API int32_t ericsson_radio_4415_ulil(const uint32_t index,double *pwr);
API int32_t ericsson_radio_4415_ulils(double *a,double *b,double *c,double *d);
API int32_t ericsson_radio_4415_mpa(const uint32_t index,const bool en);

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif
