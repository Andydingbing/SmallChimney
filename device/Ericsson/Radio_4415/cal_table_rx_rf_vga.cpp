#include "cal_table_rx_rf_vga.h"
#include "fmt/core.h"

using namespace std;
using namespace ns_ericsson;
using namespace ns_radio_4415;

data_f_rx_rf_vga::data_f_rx_rf_vga()
{
    ZERO_ARRAY(att);
    ZERO_ARRAY(pwr);
    INIT_ARRAY(temp,-100.0);
}

uint32_t data_f_rx_rf_vga::valid_results() const
{
    uint32_t i = 0;

    for (;i < ARRAY_SIZE(pwr);++i) {
        if (pwr[i] == 0) {
            return i;
        }
    }
    return i;
}


float data_f_rx_rf_vga::dynamic_range() const
{
    float min = 16384.0;
    float max = -min;

    for (uint32_t i = 0;i < ARRAY_SIZE(pwr);++i) {
        if (pwr[i] == 0.0) {
            break;
        }
        if (max < pwr[i]) { max = pwr[i]; }
        if (min > pwr[i]) { min = pwr[i]; }
    }

    return max - min;
}

bool data_f_rx_rf_vga::is_monotonic() const
{
    bool increasing = true;
    bool increasing_pre = true;

    if (pwr[0] == 0.0 || pwr[1] == 0) {
        return true;
    }

    increasing_pre = pwr[1] >= pwr[0];

    for (uint32_t i = 2;i < ARRAY_SIZE(pwr);++i) {
        if (pwr[i] == 0.0) {
            break;
        }

        increasing = pwr[i] >= pwr[i - 1];

        if (increasing != increasing_pre) {
            return false;
        }

        increasing_pre = increasing;
    }
    return true;
}

void rx_rf_vga_table_t::map_from(void *data,uint32_t pts)
{
    data_f_t *d_f = static_cast<data_f_t *>(data);
    data_m_t d_m;

    _data_m.clear();

    for (uint32_t i = 0;i < pts;++i) {
        for (uint32_t j = 0;j < ARRAY_SIZE(data_f_t::att);++j) {
            d_m.att[j] = d_f[i].att[j];
        }
        for (uint32_t j = 0;j < ARRAY_SIZE(data_f_t::pwr);++j) {
            d_m.pwr[j] = d_f[i].pwr[j];
        }
        _data_m.push_back(d_m);
    }
}

string rx_rf_vga_table_t::each_data_reception(const data_f_t &data,uint32_t row,uint32_t column) const
{
    if (column == 0) {
        return fmt::format("{:f}",data.att[row]);
    }
    if (column == 1) {
        return fmt::format("{:f}",data.pwr[row]);
    }
    if (column == 2) {
        return string_of<string>(data.time);
    }
    return "";
}
