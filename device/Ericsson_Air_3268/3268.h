#ifndef DEVICE_ERICSSON_AIR_3268_3268_H
#define DEVICE_ERICSSON_AIR_3268_3268_H

#include "../frontend.h"
#include "cal_file.h"

namespace ns_ericsson {
namespace ns_air_3268 {

BETTER_ENUM(rx_type_t,uint8_t,LTE50)
typedef rx_type_t tx_type_t;

} // ns_air_3268

class API air_3268 : public frontend
{
public:
    air_3268();
    virtual ~air_3268();

    static logger_com *logger() { return &g_log; }
    serial_dev *serial() const { return _serial; }
    ns_air_3268::cal_file *data_base() const { return _cal_file; }

    static void set_init_callback(int32_t (*init_callback)());
    static void set_en_expt(const bool en);
    static int32_t wpi(serial_dev *serial);

    bool connect(const serial_dev *serial);
    bool is_connected() OVERRIDE;

    int32_t set_tx_freq(const uint64_t freq) OVERRIDE;
    int32_t set_rx_freq(const uint64_t freq) OVERRIDE;

    int32_t tx(const uint64_t freq);
    int32_t txon() const;
    int32_t txoff() const;
    int32_t txstepattmain(const double att) const;
    int32_t txattmain(const uint32_t dac) const;
    int32_t pabias() const;
    int32_t pacm() const;
    int32_t txtype(const ns_air_3268::tx_type_t &type) const;
    int32_t rxrfsw(const bool en_lna,const bool en_att);
    int32_t rxrfvga(const double att) const;
    int32_t rxrfvgaswp(const std::string &str,ns_air_3268::data_f_rx_rf_vga *data = nullptr) const;
    int32_t rxagc(const bool en) const;
    int32_t rx(const uint64_t freq);
    int32_t rxtype(const ns_air_3268::rx_type_t &type) const;
    int32_t biasctrl(const bool en) const;
    int32_t ccctrl(const bool en) const;
    int32_t rxcpriconf() const;
    int32_t iqcomp3() const;
    int32_t intdldcw(const double pwr,const bool en) const;
    int32_t rxulg(const double gain) const;
    int32_t ulil(double &pwr) const;
    int32_t ulil(std::vector<double> &pwrs) const;
    int32_t mpa(const bool en) const;

    virtual int32_t set_sn(const std::string &sn);
    virtual int32_t open_board() OVERRIDE;
    virtual int32_t close_board() OVERRIDE;

    static int32_t serial_write_read(serial_dev *serial,const std::string &w,std::string &r);
    static int32_t serial_write(serial_dev *serial,const std::string &w);

    int32_t prepare_case(const ns_air_3268::cal_table_t &kase,const std::string freq_str,const bool is_exp);

public:
    char rf_idx() const;

public:
    static logger_com g_log;
    static int32_t (*_init_callback)();

protected:
    static bool _en_expt;

    uint64_t &_tx_freq;
    uint64_t &_rx_freq;
    bool _is_lna_en;
    bool _is_att_en;
    serial_dev *_serial;
    ns_air_3268::cal_file *_cal_file;
};

} // namespace ns_ericsson

#endif
