#include "cal_table_tx_filter.h"
#include "fmt/core.h"
#include "utilities.hpp"

using namespace std;

void freq_response_table_t::map_from(void *data,uint32_t pts)
{
    data_f_t *d_f = static_cast<data_f_t *>(data);
    data_m_t d_m;

    _data_m.clear();

    for (uint32_t i = 0;i < pts;++i) {
        d_m.pwr = float(d_f[i].pwr);
        _data_m.push_back(d_m);
    }
}

string freq_response_table_t::each_data_reception(const data_f_t &data,uint32_t row,uint32_t column) const
{
    if (column == 0) {
        return fmt::format("{:f}",data.freq / 1000000.0);
    }
    if (column == 1) {
        return fmt::format("{:f}",data.pwr);
    }
    if (column == 2) {
        return string_of<string>(data.time);
    }
    return "";
}
