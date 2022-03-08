#include "cal_table_rx_filter_offset.h"
#include "fmt/core.h"

using namespace std;
using namespace ns_starpoint;
using namespace ns_sp9500;

data_f_rx_filter_offset::data_f_rx_filter_offset()
{
    ZERO_ARRAY(offset);
    INIT_ARRAY(temp,-100.0);
}


void rx_filter_offset_table_t::map_from(void *data,uint32_t pts)
{
    data_f_t *d_f = static_cast<data_f_t *>(data);
    data_m_t d_m;

    _data_m.clear();

    for (uint32_t i = 0;i < pts;++i) {
        for (uint32_t j = 0;j < ARRAY_SIZE(d_m.offset);++j) {
            d_m.offset[j] = float(d_f[i].offset[j]);
        }
        d_m.temp_5 = float(d_f[i].temp[1]);
        _data_m.push_back(d_m);
    }
}

string rx_filter_offset_table_t::each_data_reception(const data_f_t &data,uint32_t row,uint32_t column) const
{
    if (column == 0) {
        return fmt::format("{:f}",data.freq / 1000000.0);
    }
    if (column == 1) {
        return fmt::format("{:f}",data.offset[0]);
    }
    if (column == 2) {
        return fmt::format("{:f}",data.offset[1]);
    }
    if (column == 3) {
        return fmt::format("{:f}",data.offset[2]);
    }
    if (column == 4) {
        return fmt::format("{:f}",data.temp[0]);
    }
    if (column == 5) {
        return fmt::format("{:f}",data.temp[1]);
    }
    if (column == 6) {
        return fmt::format("{:f}",data.temp[2]);
    }
    if (column == 7) {
        return fmt::format("{:f}",data.temp[3]);
    }
    if (column == 8) {
        return string_of<string>(data.time);
    }
    return "";
}
