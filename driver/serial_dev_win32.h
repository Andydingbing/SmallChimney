#ifndef DRIVER_SERIAL_DEV_WIN32_H
#define DRIVER_SERIAL_DEV_WIN32_H

#include "global_header.h"
#include "windows.h"

#include <vector>
#include <string>

namespace ns_serial_dev {

BETTER_ENUM(flow_ctrl_t,uint8_t,
            None,
            Software,
            Hardware)

BETTER_ENUM(flow_ctrl_xonoff_t,uint8_t,
            None,
            Input,
            Output,
            Both)

BETTER_ENUM(parity_bit_t,uint8_t,
            None,
            Odd,
            Even,
            Mark,
            Space)

BETTER_ENUM(stop_bit_t,uint8_t,
            _1,
            _1_5,
            _2)

BETTER_ENUM(read_end_mode_t,uint8_t,
            None,
            LastBit,
            TermChar)
}

class serial_dev_win32 : noncopyable
{
public:
    serial_dev_win32();
    ~serial_dev_win32();

public:
    virtual int32_t get_devs(std::vector<std::string> &devs);

    virtual int32_t open(const std::string &dev);
    virtual int32_t is_open(bool &open) const;
    virtual int32_t close();

    int32_t set_baud_rate(const uint32_t rate);
    int32_t get_baud_rate(uint32_t &rate) const;

    int32_t set_flow_ctrl(const ns_serial_dev::flow_ctrl_t &fc){return 0;}
    int32_t get_flow_ctrl(ns_serial_dev::flow_ctrl_t &fc) const{return 0;}

    int32_t set_flow_ctrl_xonoff(const ns_serial_dev::flow_ctrl_xonoff_t &fc);
    int32_t get_flow_ctrl_xonoff(ns_serial_dev::flow_ctrl_xonoff_t &fc) const;

    int32_t set_parity_bit(const ns_serial_dev::parity_bit_t &parity);
    int32_t get_parity_bit(ns_serial_dev::parity_bit_t &parity) const;

    int32_t set_stop_bit(const ns_serial_dev::stop_bit_t &stop);
    int32_t get_stop_bit(ns_serial_dev::stop_bit_t &stop) const;

    int32_t set_data_bits(const uint8_t bits);
    int32_t get_data_bits(uint8_t &bits) const;

    int32_t set_timeout(const uint32_t timeout){return 0;}
    int32_t get_timeout(uint32_t &timeout) const{return 0;}

    int32_t set_terminate_char_en(const bool en){return 0;}
    int32_t get_terminate_char_en(bool &en) const{return 0;}

    int32_t set_terminate_char(const char str){return 0;}
    int32_t get_terminate_char(char &str) const{return 0;}

    int32_t write(const std::string &data) const{return 0;}

    int32_t read(std::string &buf){return 0;}
    int32_t set_read_end_mode(const ns_serial_dev::read_end_mode_t &method){return 0;}
    int32_t get_read_end_mode(ns_serial_dev::read_end_mode_t &method) const{return 0;}

private:
    int32_t await_write_completion();
    void close_overlapped();

private:
    bool is_writing;
    bool is_reading;
    HANDLE handle;
    OVERLAPPED write_overlapped;
    OVERLAPPED read_overlapped;
    OVERLAPPED wait_overlapped;
};

#endif // DRIVER_SERIAL_DEV_WIN32_H
