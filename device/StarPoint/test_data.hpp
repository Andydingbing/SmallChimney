/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef UTILITIES_TEST_DATA_HPP
#define UTILITIES_TEST_DATA_HPP

#include "test_report.hpp"
#include "spec.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

// Test/Cal Report spport.These may throw runtime_exception.
#define DECL_PREPARE_TR(r,name,item_t) \
public: \
    void prepare_##r_##name() { \
        tr_header_t header; \
        get_sn_major(header.sn); \
        header.item = item_t; \
        header.size = sizeof(name##_data); \
        if (_##r_##name == nullptr) { \
            _##r_##name = boost::make_shared<name>(#name,&header,true); \
        } \
    } \
    name *##r_##name() const { return _##r_##name.get(); } \
protected: boost::shared_ptr<name> _##r_##name;

namespace rd {

struct test_common_data
{
    enum result_t {
        FAIL = 0,
        PASS
    };

    uint16_t year;
    uint16_t month;
    uint16_t day;
    uint16_t hour;
    uint16_t minute;
    uint16_t second;
    char _result[8]; // "Pass" or "Fail"

    test_common_data() :
        year(2000),month(1),day(1),
        hour(0),minute(0),second(0)
    {
        _result[0] = 'F';
        _result[1] = 'a';
        _result[2] = 'i';
        _result[3] = 'l';
        _result[4] = '\0';
    }

    std::string time() const
    {
        boost::format fmt("%5d/%02d/%02d-%02d:%02d:%02d");
        fmt % year % month % day;
        fmt % hour % minute % second;
        return fmt.str();
    }

    bool result() const { return strcmp(_result,"Pass") == 0; }

    void parse_time_string(const std::string &time)
    {
        RD_ASSERT_THROW(time.length() == 19);

        floating_point_numbers::normal_notation(time.substr(0,4),year);
        floating_point_numbers::normal_notation(time.substr(5,7),month);
        floating_point_numbers::normal_notation(time.substr(8,10),day);
        floating_point_numbers::normal_notation(time.substr(11,13),hour);
        floating_point_numbers::normal_notation(time.substr(14,16),minute);
        floating_point_numbers::normal_notation(time.substr(17,19),second);
    }

    void set_time()
    {
        boost::posix_time::ptime time = boost::posix_time::microsec_clock::local_time();
        boost::posix_time::time_duration time_day = time.time_of_day();
        boost::gregorian::date date = time.date();
        year = date.year();
        month = date.month();
        day = date.day();
        hour = uint16_t(time_day.hours());
        minute = uint16_t(time_day.minutes());
        second = uint16_t(time_day.seconds());
    }

    void set_result(const result_t res)
    {
        ZERO_ARRAY(_result);

        if (res == FAIL) set_result(std::string("Fail"));
        if (res == PASS) set_result(std::string("Pass"));
    }

    void set_result(const bool res) { set_result(res ? PASS : FAIL); }

private:
    void set_result(const std::string &res)
    {
        RD_ASSERT_THROW(res.length() < 8);
        ZERO_ARRAY(_result);

        uint8_t i = 0;
        for (i = 0;i < res.length();i ++) {
            _result[i] = res.at(i);
        }
        _result[i] = '\0';
    }
};


// Test/Cal report : RF/IF TX/RX frequency response.
typedef struct freq_res_data : test_common_data
{
    double pwr;
    freq_res_data() : pwr(-200.0) {}
    std::string format_it() const
    {
        std::string str = "Power: ";
        boost::format fmt("%-10.2f%10s%25s");
        fmt % pwr % _result % time();
        return str + fmt.str();
    }
}
rf_tx_freq_res_test_data,
if_tx_freq_res_test_data,
rf_rx_freq_res_test_data,
if_rx_freq_res_test_data,
rf_tx_freq_res_cal_data,
rf_rx_freq_res_cal_data;

DECL_TEST_REPORT_S(rf_tx_freq_res_test)
void map_from(std::ifstream &stream, const tr_header_t &header)
{
    int64_t freq = 0;
    std::string str_freq;
    std::string str_time;
    std::string str_tags;
    rf_tx_freq_res_test_data each_data;

    for (uint32_t i = 0;i < header.pt;i ++) {
        stream >> str_freq;
        stream >> str_tags; // "Power: "
        stream >> each_data.pwr;
        stream >> each_data._result;
        stream >> str_time;
        stream.seekg(1,std::ios::cur);
        RD_ASSERT_THROW(!stream.bad());

        freq = freq_string_to_int64_t(str_freq);
        each_data.parse_time_string(str_time);
        _data.insert(std::map<int64_t,data_type>::value_type(freq,each_data));
    }
}
DECL_TEST_REPORT_E(rf_tx_freq_res_test)

typedef rf_tx_freq_res_test
        rf_rx_freq_res_test,
        if_tx_freq_res_test,
        if_rx_freq_res_test,
        rf_tx_freq_res_cal,
        rf_rx_freq_res_cal;


// Test report : TX phase noise.
struct tx_phase_noise_test_data : test_common_data
{
    double phase_noise[3];
    tx_phase_noise_test_data() { ZERO_ARRAY(phase_noise); }
    std::string format_it() const
    {
        std::string str;
        boost::format fmt("%-10.2f");
        fmt % phase_noise[0]; str += "@1K: " + fmt.str();
        fmt % phase_noise[1]; str += "@10K: " + fmt.str();
        fmt % phase_noise[2]; str += "@100K: " + fmt.str();
        str += (boost::format("%10s%25s") % _result % time()).str();
        return str;
    }
};

DECL_TEST_REPORT_S(tx_phase_noise_test)
void map_from(std::ifstream &stream, const tr_header_t &header)
{
    int64_t freq = 0;
    std::string str_freq;
    std::string str_time;
    std::string str_tags;
    tx_phase_noise_test_data each_data;

    for (uint32_t i = 0;i < header.pt;i ++) {
        stream >> str_freq;
        stream >> str_tags; // "@1K: "
        stream >> each_data.phase_noise[0];
        stream >> str_tags; // "@10K: "
        stream >> each_data.phase_noise[1];
        stream >> str_tags; // "@100K: "
        stream >> each_data.phase_noise[2];
        stream >> each_data._result;
        stream >> str_time;
        stream.seekg(1,std::ios::cur);
        RD_ASSERT_THROW(!stream.bad());

        freq = freq_string_to_int64_t(str_freq);
        each_data.parse_time_string(str_time);
        _data.insert(std::map<int64_t,data_type>::value_type(freq,each_data));
    }
}
DECL_TEST_REPORT_E(tx_phase_noise_test)


// Test report : TX noise floor.
struct tx_noise_floor_test_data : test_common_data
{
    double noise_floor[2];
    tx_noise_floor_test_data() { ZERO_ARRAY(noise_floor); }
    std::string format_it() const
    {
        std::string str;
        boost::format fmt("%-10.2f");
        fmt % noise_floor[0]; str += "@10dBm: " + fmt.str();
        fmt % noise_floor[1]; str += "@-40dBm: " + fmt.str();
        str += (boost::format("%10s%25s") % _result % time()).str();
        return str;
    }
};

DECL_TEST_REPORT_S(tx_noise_floor_test)
void map_from(std::ifstream &stream, const tr_header_t &header)
{
    int64_t freq = 0;
    std::string str_freq;
    std::string str_time;
    std::string str_tags;
    tx_noise_floor_test_data each_data;

    for (uint32_t i = 0;i < header.pt;i ++) {
        stream >> str_freq;
        stream >> str_tags; // "@10dBm: "
        stream >> each_data.noise_floor[0];
        stream >> str_tags; // "@-30dBm: "
        stream >> each_data.noise_floor[1];
        stream >> each_data._result;
        stream >> str_time;
        stream.seekg(1,std::ios::cur);
        RD_ASSERT_THROW(!stream.bad());

        freq = freq_string_to_int64_t(str_freq);
        each_data.parse_time_string(str_time);
        _data.insert(std::map<int64_t,data_type>::value_type(freq,each_data));
    }
}
DECL_TEST_REPORT_E(tx_noise_floor_test)


// Test report : TX LOs lock detect.
struct tx_lo_ld_test_data : test_common_data
{
    bool is_locked[3];
    tx_lo_ld_test_data() { ZERO_ARRAY(is_locked); }
    std::string format_it() const
    {
        std::string str;
        for (uint32_t i = 0;i < ARRAY_SIZE(is_locked);i ++) {
            str += (boost::format("LO%s: ") % (i + 1)).str();
            str += is_locked[i] ? "Pass  " : "Fail  ";
        }
        str += (boost::format("%10s%25s") % _result % time()).str();
        return str;
    }
};

DECL_TEST_REPORT_S(tx_lo_ld_test)
void map_from(std::ifstream &stream, const tr_header_t &header)
{
    int64_t freq = 0;
    std::string str_freq;
    std::string str_time;
    std::string str_tags;
    std::string str_each_res;
    tx_lo_ld_test_data each_data;

    for (uint32_t i = 0;i < header.pt;i ++) {
        stream >> str_freq;
        for (uint32_t i = 0;i < ARRAY_SIZE(each_data.is_locked);i ++) {
            stream >> str_tags;     // LO1: /LO2: /...
            stream >> str_each_res; // Pass/Fail
            each_data.is_locked[i] = str_each_res == "Pass" ? true : false;
        }
        stream >> each_data._result;
        stream >> str_time;
        stream.seekg(1,std::ios::cur);
        RD_ASSERT_THROW(!stream.bad());

        freq = freq_string_to_int64_t(str_freq);
        each_data.parse_time_string(str_time);
        _data.insert(std::map<int64_t,data_type>::value_type(freq,each_data));
    }
}
DECL_TEST_REPORT_E(tx_lo_ld_test)


// Test report : TX power & modulator switch.
struct tx_pwr_mod_sw_test_data : test_common_data
{
    bool is_ok[2];
    tx_pwr_mod_sw_test_data() { ZERO_ARRAY(is_ok); }
    std::string format_it() const
    {
        std::string str;
        str += "Power_switch: ";
        str += is_ok[0] ? "Pass  " : "Fail  ";
        str += "Mod_switch: ";
        str += is_ok[1] ? "Pass  " : "Fail  ";
        str += (boost::format("%10s%25s") % _result % time()).str();
        return str;
    }
};

DECL_TEST_REPORT_S(tx_pwr_mod_sw_test)
void map_from(std::ifstream &stream, const tr_header_t &header)
{
    int64_t freq = 0;
    std::string str_freq;
    std::string str_time;
    std::string str_tags;
    std::string str_each_res;
    tx_pwr_mod_sw_test_data each_data;

    for (uint32_t i = 0;i < header.pt;i ++) {
        stream >> str_freq;
        stream >> str_tags;     // Power_switch
        stream >> str_each_res; // Pass/Fail
        each_data.is_ok[0] = str_each_res == "Pass" ? true : false;
        stream >> str_tags;     // Mod_switch
        stream >> str_each_res; // Pass/Fail
        each_data.is_ok[1] = str_each_res == "Pass" ? true : false;
        stream >> each_data._result;
        stream >> str_time;
        stream.seekg(1,std::ios::cur);
        RD_ASSERT_THROW(!stream.bad());

        freq = freq_string_to_int64_t(str_freq);
        each_data.parse_time_string(str_time);
        _data.insert(std::map<int64_t,data_type>::value_type(freq,each_data));
    }
}
DECL_TEST_REPORT_E(tx_pwr_mod_sw_test)


// Test report : TX filter switch.
struct tx_filter_sw_test_data : test_common_data
{
    bool is_ok[2];
    tx_filter_sw_test_data() { ZERO_ARRAY(is_ok); }
    std::string format_it() const
    {
        std::string str;
        str += "Filter1: ";
        str += is_ok[0] ? "Pass  " : "Fail  ";
        str += "Filter2: ";
        str += is_ok[1] ? "Pass  " : "Fail  ";
        str += (boost::format("%10s%25s") % _result % time()).str();
        return str;
    }
};

DECL_TEST_REPORT_S(tx_filter_sw_test)
void map_from(std::ifstream &stream, const tr_header_t &header)
{
    int64_t freq = 0;
    std::string str_freq;
    std::string str_time;
    std::string str_tags;
    std::string str_each_res;
    tx_filter_sw_test_data each_data;

    for (uint32_t i = 0;i < header.pt;i ++) {
        stream >> str_freq;
        stream >> str_tags;     // Filter1
        stream >> str_each_res; // Pass/Fail
        each_data.is_ok[0] = str_each_res == "Pass" ? true : false;
        stream >> str_tags;     // Filter2
        stream >> str_each_res; // Pass/Fail
        each_data.is_ok[1] = str_each_res == "Pass" ? true : false;
        stream >> each_data._result;
        stream >> str_time;
        stream.seekg(1,std::ios::cur);
        RD_ASSERT_THROW(!stream.bad());

        freq = freq_string_to_int64_t(str_freq);
        each_data.parse_time_string(str_time);
        _data.insert(std::map<int64_t,data_type>::value_type(freq,each_data));
    }
}
DECL_TEST_REPORT_E(tx_filter_sw_test)


struct tx_io_sw_test_data : test_common_data
{
    bool is_ok;
    tx_io_sw_test_data() { is_ok = false; }
    std::string format_it() const
    {
        std::string str;
        str += is_ok ? "Pass  " : "Fail  ";
        str += (boost::format("%10s%25s") % _result % time()).str();
        return str;
    }
};

DECL_TEST_REPORT_S(tx_io_sw_test)
void map_from(std::ifstream &stream, const tr_header_t &header)
{
    int64_t freq = 0;
    std::string str_freq;
    std::string str_time;
    std::string str_each_res;
    tx_io_sw_test_data each_data;
    for (uint32_t i = 0;i < header.pt;i ++) {
        stream >> str_freq;
        stream >> str_each_res;
        each_data.is_ok = str_each_res == "Pass" ? true : false;
        stream >> each_data._result;
        stream >> str_time;
        stream.seekg(1,std::ios::cur);
        RD_ASSERT_THROW(!stream.bad());

        freq = freq_string_to_int64_t(str_freq);
        each_data.parse_time_string(str_time);
        _data.insert(std::map<int64_t,data_type>::value_type(freq,each_data));
    }
}
DECL_TEST_REPORT_E(tx_io_sw_test)


// Cal report : passband frequency response.
typedef struct passband_freq_res_cal_data : test_common_data {
    double flatness;
    double ripple;
    double pwr[256]; // Min : 245.76M / 1M
    passband_freq_res_cal_data() :
        flatness(0.0),
        ripple(0.0) { ZERO_ARRAY(pwr); }
    std::string format_it() const
    {
        std::string str;
        boost::format fmt_pwr("%10.2f");

        str += "Flatness: " + (boost::format("%-10.2f") % flatness).str();
        str += "Ripple: "   + (boost::format("%-10.2f") % ripple).str();

        str += "Powers: ";
        for (uint32_t i = 0;i < ARRAY_SIZE(pwr);i ++) {
            fmt_pwr % pwr[i];
            str += fmt_pwr.str();
        }
        str += (boost::format("%10s%25s") % _result % time()).str();
        return str;
    }
} tx_passband_freq_res_160_cal_data, rx_passband_freq_res_160_cal_data;

DECL_TEST_REPORT_S(passband_freq_res_cal)
void map_from(std::ifstream &stream, const tr_header_t &header)
{
    int64_t freq = 0;
    std::string str_freq;
    std::string str_time;
    std::string str_tags;
    passband_freq_res_cal_data each_data;

    for (uint32_t i = 0;i < header.pt;i ++) {
        stream >> str_freq;
        stream >> str_tags; // "Flatness: "
        stream >> each_data.flatness;
        stream >> str_tags; // "Ripple: "
        stream >> each_data.ripple;

        stream >> str_tags; // "Powers: "
        for (uint32_t j = 0;i < ARRAY_SIZE(each_data.pwr);i ++) {
            stream >> each_data.pwr[j];
        }
        stream >> each_data._result;
        stream >> str_time;
        stream.seekg(1,std::ios::cur);
        RD_ASSERT_THROW(!stream.bad());

        freq = freq_string_to_int64_t(str_freq);
        each_data.parse_time_string(str_time);
        _data.insert(std::map<int64_t,data_type>::value_type(freq,each_data));
    }
}
DECL_TEST_REPORT_E(passband_freq_res_cal)

typedef passband_freq_res_cal
        tx_passband_freq_res_160_cal,
        rx_passband_freq_res_160_cal;


// Cal report : TX base power.
struct tx_base_pwr_cal_data : test_common_data {
    double pwr_op;
    double pwr_io;
    bool res_op;
    bool res_io;
    tx_base_pwr_cal_data() : pwr_op(0.0), pwr_io(0.0), res_op(false), res_io(false) {}
    std::string format_it() const
    {
        std::string str;
        str += "Output: " + (boost::format("%-10.4f") % pwr_op).str();
        str += res_op ? "Pass " : "Fail ";
        str += "IO: "     + (boost::format("%-10.4f") % pwr_io).str();
        str += res_io ? "Pass " : "Fail ";
        str += (boost::format("%10s%25s") % _result % time()).str();
        return str;
    }
};

DECL_TEST_REPORT_S(tx_base_pwr_cal)
void map_from(std::ifstream &stream, const tr_header_t &header)
{
    int64_t freq = 0;
    std::string str_freq;
    std::string str_time;
    std::string str_tags;
    tx_base_pwr_cal_data each_data;

    for (uint32_t i = 0;i < header.pt;i ++) {
        stream >> str_freq;
        stream >> str_tags; // "Output: "
        stream >> each_data.pwr_op;
        stream >> str_tags; // "Pass "/"Fail "
        each_data.res_op = str_tags == "Pass ";
        stream >> str_tags; // "IO: "
        stream >> each_data.pwr_io;
        stream >> str_tags; // "Pass "/"Fail "
        each_data.res_io = str_tags == "Pass ";
        stream >> each_data._result;
        stream >> str_time;
        stream.seekg(1,std::ios::cur);
        RD_ASSERT_THROW(!stream.bad());

        freq = freq_string_to_int64_t(str_freq);
        each_data.parse_time_string(str_time);
        _data.insert(std::map<int64_t,data_type>::value_type(freq,each_data));
    }
}
DECL_TEST_REPORT_E(tx_base_pwr_cal)


// Cal report : TX power.
typedef struct tx_pwr_cal_data : test_common_data {
    double pwr[150];
    tx_pwr_cal_data() { ZERO_ARRAY(pwr); }
    std::string format_it() const
    {
        std::string str;
        boost::format fmt_pwr("%10.2f");

        for (uint32_t i = 0;i < ARRAY_SIZE(pwr);i ++) {
            fmt_pwr % pwr[i];
            str += fmt_pwr.str();
        }
        str += (boost::format("%10s%25s") % _result % time()).str();
        return str;
    }
}
tx_pwr_op_cal_data,
tx_pwr_io_cal_data,
rx_pwr_cal_data,
rx_pwr_op_cal_data,
rx_pwr_io_cal_data;

DECL_TEST_REPORT_S(tx_pwr_cal)
void map_from(std::ifstream &stream, const tr_header_t &header)
{
    int64_t freq = 0;
    std::string str_freq;
    std::string str_time;
    std::string str_tags;
    tx_pwr_cal_data each_data;

    for (uint32_t i = 0;i < header.pt;i ++) {
        stream >> str_freq;
        for (uint32_t j = 0;j < ARRAY_SIZE(each_data.pwr);j ++) {
            stream >> each_data.pwr[j];
        }
        stream >> each_data._result;
        stream >> str_time;
        stream.seekg(1,std::ios::cur);
        RD_ASSERT_THROW(!stream.bad());

        freq = freq_string_to_int64_t(str_freq);
        each_data.parse_time_string(str_time);
        _data.insert(std::map<int64_t,data_type>::value_type(freq,each_data));
    }
}
DECL_TEST_REPORT_E(tx_pwr_cal)

typedef tx_pwr_cal tx_pwr_op_cal,tx_pwr_io_cal,rx_pwr_op_cal,rx_pwr_io_cal;


// Cal report : RX reference.
struct rx_ref_cal_data : test_common_data {
    double pwr_op[8];
    double pwr_io[8];
    bool res_op[8];
    bool res_io[8];
    rx_ref_cal_data()
    { ZERO_ARRAY(pwr_op);ZERO_ARRAY(pwr_io);ZERO_ARRAY(res_op);ZERO_ARRAY(res_io); }

    std::string format_it() const
    {
        std::string str;
        boost::format fmt("%-10.4f");
        str += "Output: ";
        for (uint32_t i = 0;i < ARRAY_SIZE(pwr_op);i ++) {
            fmt % pwr_op[i];
            str += fmt.str();
            str += res_op[i] ? "Pass " : "Fail ";
        }
        str += "IO: ";
        for (uint32_t i = 0;i < ARRAY_SIZE(pwr_io);i ++) {
            fmt % pwr_io[i];
            str += fmt.str();
            str += res_io[i] ? "Pass " : "Fail ";
        }
        str += (boost::format("%10s%25s") % _result % time()).str();
        return str;
    }
};

DECL_TEST_REPORT_S(rx_ref_cal)
void map_from(std::ifstream &stream, const tr_header_t &header)
{
    int64_t freq = 0;
    std::string str_freq;
    std::string str_time;
    std::string str_tags;
    rx_ref_cal_data each_data;

    for (uint32_t i = 0;i < header.pt;i ++) {
        stream >> str_freq;
        stream >> str_tags; // "Output: "
        for (uint32_t j = 0;j < ARRAY_SIZE(each_data.pwr_op);j ++) {
            stream >> each_data.pwr_op[j];
            stream >> str_tags; // "Pass "/"Fail "
            each_data.res_op[j] = str_tags == "Pass ";
        }
        stream >> str_tags; // "IO: "
        for (uint32_t j = 0;j < ARRAY_SIZE(each_data.pwr_io);j ++) {
            stream >> each_data.pwr_io[j];
            stream >> str_tags; // "Pass "/"Fail "
            each_data.res_io[j] = str_tags == "Pass ";
        }
        stream >> each_data._result;
        stream >> str_time;
        stream.seekg(1,std::ios::cur);
        RD_ASSERT_THROW(!stream.bad());

        freq = freq_string_to_int64_t(str_freq);
        each_data.parse_time_string(str_time);
        _data.insert(std::map<int64_t,data_type>::value_type(freq,each_data));
    }
}
DECL_TEST_REPORT_E(rx_ref_cal)


} // namespace rd

#endif // UTILITIES_TEST_DATA_HPP

