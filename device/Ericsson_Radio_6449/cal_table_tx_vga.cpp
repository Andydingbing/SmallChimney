#include "cal_table_tx_vga.h"
#include "fmt/core.h"

using namespace std;
using namespace ns_ericsson;
using namespace ns_radio_6449;

data_f_tx_vga::data_f_tx_vga()
{
    ZERO_ARRAY(dac);
    ZERO_ARRAY(att);
    INIT_ARRAY(temp,-100.0);
}

uint32_t data_f_tx_vga::valid_results() const
{
    uint32_t i = 0;

    for (;i < ARRAY_SIZE(dac);++i) {
        if (dac[i] == 0) {
            return i;
        }
    }
    return i;
}

float data_f_tx_vga::dynamic_range() const
{
    uint32_t n = valid_results();
    return n > 0 ? att[valid_results() - 1] : 0.0f;
}

void tx_vga_table_t::map_from(void *data,uint32_t pts)
{
    data_f_t *d_f = static_cast<data_f_t *>(data);
    data_m_t d_m;

    _data_m.clear();

    for (uint32_t i = 0;i < pts;++i) {
        for (uint32_t j = 0;j < ARRAY_SIZE(data_f_t::dac);++j) {
            d_m.dac[j] = d_f[i].dac[j];
        }
        for (uint32_t j = 0;j < ARRAY_SIZE(data_f_t::att);++j) {
            d_m.att[j] = d_f[i].att[j];
        }
        _data_m.push_back(d_m);
    }
}

string tx_vga_table_t::each_data_reception(const data_f_t &data,uint32_t row,uint32_t column) const
{
    if (column == 0) {
        return fmt::format("{:f}",data.att[row]);
    }
    if (column == 1) {
        return fmt::format("{:d}",data.dac[row]);
    }
    if (column == 2) {
        return string_of<string>(data.time);
    }
    return "";
}

uint32_t tx_vga_table_t::each_data_reception_points(const data_f_t &data) const
{
    return data.valid_results();
}

double tx_vga_table_t::each_data_reception_x(const data_f_t &data,uint32_t idx) const
{
    return data.att[idx];
}

double tx_vga_table_t::each_data_reception_y(const data_f_t &data,uint32_t idx) const
{
    return data.dac[idx];
}
