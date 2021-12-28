#include "cal_table_tx_vga.h"

using namespace ns_ericsson;
using namespace ns_air_3268;

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
