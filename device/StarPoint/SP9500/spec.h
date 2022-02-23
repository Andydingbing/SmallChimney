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

#ifndef UTILITIES_SPEC_HPP
#define UTILITIES_SPEC_HPP

#include "../../frontend.h"
#include "../cal_table.h"
#include "set_helper.hpp"

namespace rd {

class API spec
{
public:
    static double test_rf_tx_freq_res_max();
    static double test_rf_tx_freq_res_min();
    static double test_rf_tx_freq_res();

    static double test_if_tx_freq_res_max();
    static double test_if_tx_freq_res_min();
    static double test_if_tx_freq_res();

    static double test_rf_rx_freq_res_max();
    static double test_rf_rx_freq_res_min();
    static double test_rf_rx_freq_res();

    static double test_if_rx_freq_res_max();
    static double test_if_rx_freq_res_min();
    static double test_if_rx_freq_res();

    static void test_tx_phase_noise_freq(range_freq_string &freq);
    static void test_tx_phase_noise_offset(std::string &offset);
    static void test_tx_phase_noise_span(std::string &span);
    static void test_tx_phase_noise_rbw(std::string &rbw);
    static double test_tx_phase_noise(uint64_t freq,double offset);

    static void test_tx_noise_floor_freq(range_freq_string &freq);
    static void test_tx_noise_floor_span(std::string &span);
    static void test_tx_noise_floor_rbw(std::string &rbw);
    static double test_tx_noise_floor(uint64_t freq,double pwr);

public:
    static double cal_tx_lol_up();
    static double cal_tx_lol_down();
    static double cal_tx_sb_up();
    static double cal_tx_sb_down();

    static void cal_tx_filter_freq(range_freq_string &freq);
    static double cal_tx_filter_flatness();
    static double cal_tx_filter_ripple();

    static void cal_tx_base_pwr_freq(range_freq_string &freq);
    static double cal_tx_base_pwr_accuracy();

    static void cal_tx_pwr_freq(range_freq_string &freq);
    static void cal_tx_pwr_pwr(const io_mode_t mode,range_pwr_string &pwr);
    static double cal_tx_pwr_accuracy();

    static void cal_rx_ref_freq(range_freq_string &freq);
    static double cal_rx_ref_accuracy();

    static void cal_rx_pwr_freq(range_freq_string &freq);
    static void cal_rx_pwr_pwr(const io_mode_t mode,range_pwr_string &pwr);
    static double cal_rx_pwr_accuracy();

private:
    static std::string path();
};

} // namespace rd

#endif

