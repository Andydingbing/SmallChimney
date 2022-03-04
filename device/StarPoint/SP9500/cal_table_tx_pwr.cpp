#include "cal_table_tx_pwr.h"
#include "fmt/core.h"

using namespace std;
using namespace ns_starpoint;
using namespace ns_sp9500;

data_f_tx_pwr::data_f_tx_pwr()
{
    d_gain = 0.0;
    att0 = 0.0;
    att1 = 0.0;
    att2 = 0.0;
    att3 = 0.0;
    INIT_ARRAY(temp,-100.0);
}

void tx_pwr_table_t::map_from(void *data,uint32_t pts)
{
    data_f_t *d_f = static_cast<data_f_t *>(data);
    data_m_t d_m;

    _data_m.clear();

    for (uint32_t i = 0;i < pts;++i) {
        d_m.d_gain = float(d_f[i].d_gain);
        d_m.att0 = short(d_f[i].att0 * 2);
        d_m.att1 = short(d_f[i].att1 * 2);
        d_m.att2 = short(d_f[i].att2 * 2);
        d_m.att3 = short(d_f[i].att3 * 2);
        d_m.temp_5 = float(d_f[i].temp[1]);
        _data_m.push_back(d_m);
    }
}

string tx_pwr_table_t::each_data_reception(const data_f_t &data,uint32_t row,uint32_t column) const
{
    if (column == 0) {
        return fmt::format("{:f}",data.freq / 1000000.0);
    }
    if (column == 1) {
        return fmt::format("{:.3f}",data.d_gain);
    }
    if (column == 2) {
        return fmt::format("{:d}",data.att0);
    }
    if (column == 3) {
        return fmt::format("{:d}",data.att1);
    }
    if (column == 4) {
        return fmt::format("{:d}",data.att2);
    }
    if (column == 5) {
        return fmt::format("{:d}",data.att3);
    }
    if (column == 6) {
        return fmt::format("{:f}",data.temp[0]);
    }
    if (column == 7) {
        return fmt::format("{:f}",data.temp[1]);
    }
    if (column == 8) {
        return fmt::format("{:f}",data.temp[2]);
    }
    if (column == 9) {
        return fmt::format("{:f}",data.temp[3]);
    }
    if (column == 10) {
        return string_of<string>(data.time);
    }
    return "";
}
