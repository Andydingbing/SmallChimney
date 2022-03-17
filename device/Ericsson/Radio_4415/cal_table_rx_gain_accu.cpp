#include "cal_table_rx_gain_accu.h"
#include "fmt/core.h"

using namespace std;
using namespace ns_ericsson;
using namespace ns_radio_4415;

data_f_rx_gain_accu::data_f_rx_gain_accu()
{
    ZERO_ARRAY(pwr);
    INIT_ARRAY(temp,-100.0);
}

float data_f_rx_gain_accu::accu() const
{
    float max = pwr[0];
    float min = pwr[0];

    for (uint32_t i = 1;i < ARRAY_SIZE(pwr);++i) {
        if (max < pwr[i]) { max = pwr[i]; }
        if (min > pwr[i]) { min = pwr[i]; }
    }

    return max - min;
}

void rx_gain_accu_table_t::map_from(void *data,uint32_t pts)
{
    data_f_t *d_f = static_cast<data_f_t *>(data);
    data_m_t d_m;

    _data_m.clear();

    for (uint32_t i = 0;i < pts;++i) {
        for (uint32_t j = 0;j < ARRAY_SIZE(data_f_t::pwr);++j) {
            d_m.pwr[j] = d_f[i].pwr[j];
        }
        _data_m.push_back(d_m);
    }
}

string rx_gain_accu_table_t::each_data_reception(const data_f_t &data,uint32_t row,uint32_t column) const
{
    if (column == 0) {
        return fmt::format("{:f}",data.pwr[row]);
    }
    if (column == 1) {
        return string_of<string>(data.time);
    }
    return "";
}
