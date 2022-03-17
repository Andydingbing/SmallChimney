#include "6449.h"
#include "sleep_common.h"
#include "set_helper.hpp"
#include "utilities.hpp"
#include "floating_point_numbers.hpp"
#include <fmt/core.h>

using namespace std;
using namespace ns_ericsson;
using namespace ns_radio_6449;
using namespace set_helper;

#define assert_serial_wr(func,err) \
    do { \
        int32_t ret = func; \
        if (ret) { \
            g_log.add(ret,w,r);\
            if (_en_expt) { \
                throw ::runtime_error(err); \
            } \
            return ret; \
        } \
    } while(0)

#define assert_serial_r(condition,err) \
    do { \
        if ((condition)) { \
            g_log.add(-1,w,r); \
            if (_en_expt) { \
                throw ::runtime_error(std::string("Serial read error : ") + std::string(err)); \
            } \
            return -1; \
        } \
    } while(0)


#define assert_serial(func) LM(func,"%s",#func)

#define SERIAL_W(w) \
    do { \
        const std::string &write = w; \
        std::string read; \
        INT_CHECK(serial_write_read(_serial,write,read)); \
    } while(0); NEED_SEMICOLON

#define SERIAL_W_R(w,r) \
    do { \
        INT_CHECK(serial_write_read(_serial,w,r)); \
    } while(0); NEED_SEMICOLON


int32_t (*radio_6449::_init_callback)() = nullptr;
bool radio_6449::_en_expt = false;
logger_com radio_6449::g_log;

radio_6449::radio_6449() :
    frontend(),
    _tx_freq(*(_tx_freqs.begin())),
    _rx_freq(*(_rx_freqs.begin())),
    _is_lna_en(false),
    _is_att_en(false),
    _serial(nullptr),
    _cal_file(nullptr)
{

}

radio_6449::~radio_6449()
{

}

void radio_6449::set_init_callback(int32_t (*init_callback)())
{
    _init_callback = init_callback;
}

void radio_6449::set_en_expt(const bool en)
{
    radio_6449::_en_expt = en;
}

int32_t radio_6449::wpi(serial_dev *serial)
{
    INT_CHECK(serial_write(serial,"wpi"));
    sleep_ms(10000);
    return 0;
}

bool radio_6449::connect(const serial_dev *serial)
{
    _serial = const_cast<serial_dev *>(serial);
    return true;
}

bool radio_6449::is_connected()
{
    return _serial != nullptr;
}

int32_t radio_6449::tx(const uint64_t freq)
{
    SERIAL_W(fmt::format("tx -b {:c} {:lld} 0 460 0 -100",rf_idx(),freq / 1000));
    _tx_freq = freq;
    return 0;
}

int32_t radio_6449::set_tx_freq(const uint64_t freq)
{
    return tx(freq);
}

int32_t radio_6449::txon() const
{
    SERIAL_W("txon");
    return 0;
}

int32_t radio_6449::txoff() const
{
    SERIAL_W("txoff");
    return 0;
}

int32_t radio_6449::txstepattmain(const double att) const
{
    SERIAL_W(fmt::format("txstepattmain -b {:c} {:d}",rf_idx(),uint32_t(att * 10)));
    return 0;
}

int32_t radio_6449::txattmain(const uint32_t dac) const
{
    SERIAL_W(fmt::format("txattmain -b {:c} att {:d}",rf_idx(),dac));
    return 0;
}

int32_t radio_6449::pabias() const
{
    SERIAL_W(fmt::format("pabias -b {:c}",rf_idx()));
    return 0;
}

int32_t radio_6449::pacm() const
{
    SERIAL_W(fmt::format("pacm -b {:c}",rf_idx()));
    return 0;
}

int32_t radio_6449::txtype(const ns_radio_6449::tx_type_t &type) const
{
    string str_type = type._to_string();
    boost::to_lower(str_type);

    SERIAL_W(fmt::format("txtype {:s}",str_type));
    return 0;
}

int32_t radio_6449::rxrfsw(const bool en_lna,const bool en_att)
{
    SERIAL_W(fmt::format("rxrfsw -b {:c} {:d} {:d}",rf_idx(),(en_lna ? 1 : 0),(en_att ? 1 : 0)));
    _is_lna_en = en_lna;
    _is_att_en = en_att;
    return 0;
}

int32_t radio_6449::rxrfvga(const double att) const
{
    SERIAL_W(fmt::format("rxrfvga {:lld}",uint32_t(att)));
    return 0;
}

int32_t radio_6449::rxrfvgaswp(const std::string &str,ns_radio_6449::data_f_rx_rf_vga *data) const
{
    string write;
    string read;
    string separator = "\r\n";
    size_t pos[2] = {0,0};
    set_helper_t<uint8_t> atts;
    set<uint8_t>::iterator atts_iter;

    float pwr = 0.0;
    size_t idx = 0;

    atts.parse_from(str.empty() ? "0:1:16,23" : str);
    atts_iter = atts.data.begin();

    write = fmt::format("rxrfvgaswp -b {:c} {:s}",rf_idx(),string_of<string,uint8_t>(atts.data," "));

    SERIAL_W_R(write,read);
    read.append(separator);

    if (data == nullptr) {
        return 0;
    }

    data->freq = _rx_freq;

    while ((pos[1] = read.find_first_of("\r\n",pos[0])) != string::npos) {
        floating_point_numbers::normal_notation(read.substr(pos[0],pos[1]),pwr);
        data->att[idx] = *atts_iter;
        data->pwr[idx] = pwr;
        ++idx;
        ++atts_iter;
        pos[0] = pos[1] + separator.length();
    }

    _cal_file->add(cal_table_t::RX_RF_VGA,data);
    _cal_file->persistence(cal_table_t::RX_RF_VGA);
    return 0;
}

int32_t radio_6449::rxagc(const bool en) const
{
    SERIAL_W(en ? "rxagc 1" : "rxagc 0");
    return 0;
}

int32_t radio_6449::rx(const uint64_t freq)
{
    SERIAL_W(fmt::format("rx -b {:c} -v 12 {:lld}",rf_idx(),freq / 1000));
    _rx_freq = freq;
    return 0;
}

int32_t radio_6449::set_rx_freq(const uint64_t freq)
{
    return rx(freq);
}

int32_t radio_6449::rxtype(const ns_radio_6449::rx_type_t &type) const
{
    string str_type = type._to_string();
    boost::to_lower(str_type);

    SERIAL_W(fmt::format("rxtype {:s}",str_type));
    return 0;
}

int32_t radio_6449::biasctrl(const bool en) const
{
    SERIAL_W(en ? "biasctrl 1" : "biasctrl 0");
    return 0;
}

int32_t radio_6449::ccctrl(const bool en) const
{
    SERIAL_W(en ? "ccctrl 1" : "ccctrl 0");
    return 0;
}

int32_t radio_6449::rxcpriconf() const
{
    SERIAL_W(fmt::format("rxcpriconf -b {:c}",rf_idx()));
    return 0;
}

int32_t radio_6449::iqcomp3() const
{
    SERIAL_W(fmt::format("iqcomp3 -b {:c}",rf_idx()));
    return 0;
}

int32_t radio_6449::intdldcw(const double pwr,const bool en) const
{
    SERIAL_W(fmt::format("intdldcw -b {:c} {:d} {:d}",rf_idx(),int32_t(pwr * 10),(en ? 1 : 0)));
    return 0;
}

int32_t radio_6449::rxulg(const double gain) const
{
    SERIAL_W(fmt::format("rxulg {:d}",uint32_t(gain)));
    return 0;
}

int32_t radio_6449::ulil(double &pwr) const
{
    vector<double> pwrs;

    INT_CHECK(ulil(pwrs));
    pwr = pwrs.at(_rf_idx);
    return 0;
}

int32_t radio_6449::ulil(vector<double> &pwrs) const
{
    string read;
    string separator = "\r\n";
    size_t pos[2] = {0,0};
    double each_pwr = 0.0;

    SERIAL_W_R("ulil -c0 -c0",read);
    read.append(separator);

    while ((pos[1] = read.find_first_of("\r\n",pos[0])) != string::npos) {
        floating_point_numbers::normal_notation(read.substr(pos[0],pos[1]),each_pwr);
        pwrs.push_back(each_pwr);
        pos[0] = pos[1] + separator.length();
    }

    return 0;
}

int32_t radio_6449::mpa(const bool en) const
{
    SERIAL_W(fmt::format("mpa {:d}",en ? 1 : 0));
    return 0;
}

int32_t radio_6449::set_sn(const string &sn)
{
    SAFE_NEW(_cal_file,cal_file);

    _cal_file->set_name(sn);
    _cal_file->open();
    return 0;
}

int32_t radio_6449::open_board()
{
    return 0;
}

int32_t radio_6449::close_board()
{
    INT_CHECK(_serial->close());

    return 0;
}

int32_t radio_6449::serial_write_read(serial_dev *serial,const std::string &w,std::string &r)
{
    size_t length_common = w.length() - 1;
    size_t offset_read = 0;


    assert_serial_wr(serial->write(w + '\n'),"Serial Write");
    assert_serial_wr(serial->read(r),"Serial Read");

    assert_serial_r(r.length() < length_common + 1,
                    "not enough read chars");

    assert_serial_r(r.substr(0,length_common) != w.substr(0,length_common),
                    "internal error");

    assert_serial_r((offset_read = r.find_first_of("\r\n",length_common)) == std::string::npos,
                    "internal error");

    assert_serial_r(r.back() != '$',
                    "not end with $");

    r = r.substr(offset_read + 2);
    r.pop_back();

    for (size_t i = r.size();i > 0;--i) {
        if (r.at(i - 1) - ' ' == 0) {
            r.pop_back();
        } else {
            break;
        }
    }

    if (!r.empty()) {
        if (r.back() - '\n' == 0) { r.pop_back(); }
        if (r.back() - '\r' == 0) { r.pop_back(); }
    }

    g_log.add(0,w,r);

    return 0;
}

int32_t radio_6449::serial_write(serial_dev *serial,const string &w)
{
    string r;
    return serial_write_read(serial,w,r);
}

int32_t radio_6449::prepare_case(const ns_radio_6449::cal_table_t &kase,const string freq_str,const bool is_exp)
{
    if (_cal_file == nullptr) {
        return -1;
    }

    set_helper::range_freq<uint64_t> freqs;
    set_helper::parse(freq_str,freqs);

    return _cal_file->prepare(kase,freqs.freq,is_exp);
}

char radio_6449::rf_idx() const
{
    return 'a' + _rf_idx;
}
