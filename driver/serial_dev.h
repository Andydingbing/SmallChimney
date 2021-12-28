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

#ifndef DRIVER_SERIAL_DEV_H
#define DRIVER_SERIAL_DEV_H

#include "preprocessor/prefix.h"
#include "enum.h"
#include <regex>

ENUM_C_CPP(serial_dev,flow_ctrl,uint8_t,none,software,hardware)
ENUM_C_CPP(serial_dev,flow_ctrl_xonoff,uint8_t,none,input,output,both)
ENUM_C_CPP(serial_dev,parity_bit,uint8_t,none,odd,even,mark,space)
ENUM_C_CPP(serial_dev,stop_bit,uint8_t,_1,_1_5,_2)
ENUM_C_CPP(serial_dev,read_end_mode,uint8_t,none,lastbit,termchar)

class API serial_dev
{
public:
    serial_dev();
    ~serial_dev();

public:
    virtual int32_t get_devs(std::vector<std::string> &devs);

    virtual int32_t open(const std::string &dev);
    virtual int32_t is_open(bool &open) const;
    virtual int32_t close();

    virtual int32_t set_baud_rate(const uint32_t rate);
    virtual int32_t get_baud_rate(uint32_t &rate) const;

    virtual int32_t set_flow_ctrl(const serial_dev_flow_ctrl_t &fc);
    virtual int32_t get_flow_ctrl(serial_dev_flow_ctrl_t &fc) const;

    virtual int32_t set_parity_bit(const serial_dev_parity_bit_t &parity);
    virtual int32_t get_parity_bit(serial_dev_parity_bit_t &parity) const;

    virtual int32_t set_stop_bit(const serial_dev_stop_bit_t &stop);
    virtual int32_t get_stop_bit(serial_dev_stop_bit_t &stop) const;

    virtual int32_t set_data_bits(const uint8_t bits);
    virtual int32_t get_data_bits(uint8_t &bits) const;

    virtual int32_t set_timeout(const uint32_t timeout);
    virtual int32_t get_timeout(uint32_t &timeout) const;

    virtual int32_t set_terminate_char_en(const bool en);
    virtual int32_t get_terminate_char_en(bool &en) const;

    virtual int32_t set_terminate_char(const char str);
    virtual int32_t get_terminate_char(char &str) const;

    virtual int32_t write(const std::string &data) const;

    virtual int32_t read();
    virtual int32_t read(std::string &buf);
    virtual int32_t set_read_end_mode(const serial_dev_read_end_mode_t &method);
    virtual int32_t get_read_end_mode(serial_dev_read_end_mode_t &method) const;

    bool is_open() const;

    int32_t read(std::string &buf,const uint32_t timeout);
    int32_t read_until(const char str);
    int32_t read_until(const char str,const uint32_t timeout);
    int32_t read_until(const std::string &str);
    int32_t read_until(const std::string &str,const uint32_t timeout);
    int32_t read_until(const std::regex &reg);
    int32_t read_until(const std::regex &reg,const uint32_t timeout);

private:
    uint32_t _timeout;
};

#endif // DRIVER_SERIAL_DEV_H
