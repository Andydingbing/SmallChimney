#include "api.h"
#include "libdriver.h"
#include "set_helper.hpp"
#include "4415.h"
#include "4415_b3.h"

using namespace std;
using namespace ns_ericsson;
using namespace ns_radio_4415;

static CONSTEXPR_OR_CONST uint32_t g_channels = 4;
radio_4415 g_radio_4415[g_channels];
#define g_radio g_radio_4415[index]

radio& radio::instance()
{
    static radio g_Radio;
    return g_Radio;
}

void radio::set_init_callback(int32_t (*callback)())
{
    return radio_4415::set_init_callback(callback);
}

void radio::set_log_callback(void (*callback)())
{
    return radio_4415::logger()->set_callback(callback);
}

int32_t radio::set_sn(const uint32_t index,const char *sn)
{
    for (uint8_t i = 0;i < channels();++i) {
        INT_CHECK(g_radio_4415[i].set_sn(sn));
    }
    return 0;
}

int32_t radio::get_sn(const uint32_t index,char *sn)
{
    return 0;
}

const item_table_base* radio::data_base(const uint32_t index,const int32_t kase) const
{
    return g_radio.data_base()->db(kase);
}

int32_t radio::data_base_add(const uint32_t index,const int32_t kase,void *data)
{
    g_radio.data_base()->add(cal_table_t::_from_integral(kase),data);
    return 0;
}

int32_t radio::prepare_kase(const uint32_t index,const int32_t kase,const string freq_str,const bool is_exp)
{
    cal_table_t cal_table = cal_table_t::_from_integral(kase);

    return g_radio.prepare_case(cal_table,freq_str,is_exp);
}

uint32_t radio::com_loggers()
{ return radio_4415::logger()->size(); }

const char* radio::com_logger_write(const uint32_t n)
{ return radio_4415::logger()->at(n)->write.c_str(); }

const char* radio::com_logger_read(const uint32_t n)
{ return radio_4415::logger()->at(n)->read.c_str(); }

const char* radio::com_logger_time(const uint32_t n)
{ return radio_4415::logger()->at(n)->time.c_str(); }

int32_t radio::com_logger_result(const uint32_t n)
{ return radio_4415::logger()->at(n)->result; }

cal_file* radio::data_base(const uint32_t index) const
{
    return g_radio.data_base();
}

/*
 * init procedure :
 *
 *
 *                                        N
 *                              ps on? ------> ps on
 *                                |              |
 *                               Y|              |
 *                                |              |
 *                                v              v
 *                         sweep com ports <--------------<---------------
 *                                |                                      |
 *                                |                                      |
 *                                |                                      |
 *                     Y          v                                      ^
 *           -------<----------- end?                                    |
 *           |                    |                                      |
 *           |                   N|                                      |
 *           |                    |                                      |
 *           v                    v       N                            N |
 *           |                com read ------> w/r particular command ----
 *           |                    |                       |
 *           |                    |                      Y|
 *           |                    |                       |
 *           |                    |                       v            N
 *           v          Y(booting)v                command not found? ----
 *           |                    |                       |              |
 *           |                    |                      Y|              |
 *           |                    |                       |              |
 *           v                    v                       v              v
 *           |               com selected <----------------              |
 *           |                    |                                      |
 *           |                    v                                      v
 *           |                  "wpi"                                    |
 *           |                    |                                      |
 *           |                    | <-----<---------------<---------------
 *           |                    |
 *           v                    v
 *   serial not connected        done
 *
 */
int32_t radio::init()
{
    serial_dev *serial = new serial_dev_vi;
    vector<string> devs;
    vector<string>::iterator iter;
    string write = "rxrfvga\n";
    string read;

    radio_4415::set_en_expt(true);

    LM(radio_4415::_init_callback(),"error");

    LM(serial->get_devs(devs),"Serial internal error");
    LM(devs.size() == 0,"Serial not found");

    for (iter = devs.begin();iter != devs.end();++iter) {
        if (serial->open(*iter) ||
            serial->set_baud_rate(115200) ||
            serial->set_timeout(60000) ||
            serial->set_data_bits(8) ||
            serial->set_stop_bit(serial_dev_stop_bit__1) ||
            serial->set_parity_bit(serial_dev_parity_bit_none) ||
            serial->set_flow_ctrl(serial_dev_flow_ctrl_none) ||
            serial->set_read_end_mode(serial_dev_read_end_mode_termchar) ||
            serial->set_terminate_char_en(true) ||
            serial->set_terminate_char('$')) {
            continue;
        }

        if (serial->read() == 0) {
            radio_4415::wpi(serial);
            break;
        } else if (serial->write(write)) {
            continue;
        } else if (serial->read(read)) {
            continue;
        } else if (read.find("command not found") != string::npos) {
            radio_4415::wpi(serial);
            break;
        } else {
            break;
        }
    }

    LM(iter == devs.end(),"Serial not found");

    for (uint32_t i = 0;i < channels();++i) {
        g_radio_4415[i].set_rf_idx(i);
        g_radio_4415[i].connect(serial);
        g_radio_4415[i].open_board();
    }

    return 0;
}

uint32_t radio::channels()
{
    return g_channels;
}

int32_t radio::serial_write(const char *str)
{ return serial_write(string(str)); }

int32_t radio::serial_write(const std::string &str)
{
    const uint32_t index = 0;
    return g_radio.serial_write(g_radio.serial(),str);
}

int32_t radio::set_tx_frequency(const uint32_t index,const uint64_t freq)
{ return g_radio.set_tx_freq(freq); }

int32_t radio::txon(const uint32_t index)
{ return g_radio.txon(); }

int32_t radio::txoff(const uint32_t index)
{ return g_radio.txoff(); }

int32_t radio::txtype(const uint32_t index,const radio_system_t &system,const uint64_t bw)
{
    tx_type_t type = tx_type_t::LTE50;

    switch (system) {
    case GSM :
    case CDMA :
    case LTE : {
        if (bw == FREQ_M(5)) { type = tx_type_t::LTE50; break;}
    }
    case NR : break;
    }

    g_radio.txtype(type);
    return 0;
}

int32_t radio::txstepattmain(const uint32_t index,const double att)
{ return g_radio.txstepattmain(att); }

int32_t radio::txattmain(const uint32_t index,const uint32_t dac)
{ return g_radio.txattmain(dac); }

int32_t radio::pabias(const uint32_t index)
{ return g_radio.pabias(); }

int32_t radio::pabias()
{ return serial_write("pabias"); }

int32_t radio::pacm(const uint32_t index)
{ return g_radio.pacm(); }

int32_t radio::set_rx_frequency(const uint32_t index,const uint64_t freq)
{ return g_radio.set_rx_freq(freq); }

int32_t radio::rx(const uint32_t index,const uint64_t freq)
{ return g_radio.rx(freq); }

int32_t radio::rxtype(const uint32_t index, const radio_system_t &system, const uint64_t bw)
{
    rx_type_t type = rx_type_t::LTE50;

    switch (system) {
    case GSM :
    case CDMA :
    case LTE : {
        if (bw == FREQ_M(5)) { type = rx_type_t::LTE50; break;}
    }
    case NR : break;
    }

    return g_radio.rxtype(type);
}

int32_t radio::rxrfsw(const uint32_t index,const bool en_lna,const bool en_att)
{ return g_radio.rxrfsw(en_lna,en_att); }

int32_t radio::rxrfvga(const uint32_t index,const double att)
{ return g_radio.rxrfvga(att); }

int32_t radio::rxrfvgaswp(const uint32_t index,const std::string &str,void *data)
{ return g_radio.rxrfvgaswp(str,(data_f_rx_rf_vga *)data); }

int32_t radio::rxagc(const uint32_t index,const bool en)
{ return g_radio.rxagc(en); }

int32_t radio::rxcpriconf(const uint32_t index)
{ return g_radio.rxcpriconf(); }

int32_t radio::biasctrl(const uint32_t index,const bool en)
{ return g_radio.biasctrl(en); }

int32_t radio::ccctrl(const uint32_t index,const bool en)
{ return g_radio.ccctrl(en); }

int32_t radio::iqcomp3(const uint32_t index)
{ return g_radio.iqcomp3(); }

int32_t radio::intdldcw(const uint32_t index,const double pwr,const bool en)
{ return g_radio.intdldcw(pwr,en); }

int32_t radio::rxulg(const uint32_t index,const double gain)
{ return g_radio.rxulg(gain); }

int32_t radio::ulil(const uint32_t index,double *pwr)
{ return g_radio.ulil(*pwr); }

int32_t radio::ulils(double *a,double *b,double *c,double *d)
{
    const int32_t index = 0;
    vector<double> pwrs;

    g_radio.ulil(pwrs);
    *a = pwrs[0];
    *b = pwrs[1];
    *c = pwrs[2];
    *d = pwrs[3];
    return 0;
}

int32_t radio::mpa(const uint32_t index,const bool en)
{ return g_radio.mpa(en); }
