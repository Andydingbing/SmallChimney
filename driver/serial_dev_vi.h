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

#ifndef DRIVER_SERIAL_DEV_VI_H
#define DRIVER_SERIAL_DEV_VI_H

#include "serial_dev.h"
#include <vector>

class API serial_dev_vi : public serial_dev
{
public:
    serial_dev_vi();
    ~serial_dev_vi();

public:
    int32_t get_devs(std::vector<std::string> &devs);
    int32_t open(const std::string &dev);
    int32_t is_open(bool &open) const;
    int32_t close();

    int32_t set_baud_rate(const uint32_t rate);
    int32_t get_baud_rate(uint32_t &rate) const;

    int32_t set_flow_ctrl(const serial_dev_flow_ctrl_t &fc);
    int32_t get_flow_ctrl(serial_dev_flow_ctrl_t &fc) const;

    int32_t set_parity_bit(const serial_dev_parity_bit_t &parity);
    int32_t get_parity_bit(serial_dev_parity_bit_t &parity) const;

    int32_t set_stop_bit(const serial_dev_stop_bit_t &stop);
    int32_t get_stop_bit(serial_dev_stop_bit_t &stop) const;

    int32_t set_data_bits(const uint8_t bits);
    int32_t get_data_bits(uint8_t &bits) const;

    int32_t set_timeout(const uint32_t timeout);
    int32_t get_timeout(uint32_t &timeout) const;

    int32_t set_terminate_char_en(const bool en);
    int32_t get_terminate_char_en(bool &en) const;

    int32_t set_terminate_char(const char str);
    int32_t get_terminate_char(char &str) const;

    int32_t write(const std::string &data) const;
    int32_t read();
    int32_t read(std::string &buf);
    int32_t set_read_end_mode(const serial_dev_read_end_mode_t &method);
    int32_t get_read_end_mode(serial_dev_read_end_mode_t &method) const;

private:
    unsigned long _session;
};

#endif
