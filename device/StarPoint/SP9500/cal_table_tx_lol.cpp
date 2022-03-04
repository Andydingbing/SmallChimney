#include "cal_table_tx_lol.h"
#include "fmt/core.h"

using namespace std;
using namespace ns_starpoint;
using namespace ns_sp9500;

data_f_tx_lol::data_f_tx_lol()
{
    dc_i = dc_q = 0;
    use_sa_or_loopback = -1;
    pwr = -100.0;
    INIT_ARRAY(temp,-100.0);
}


void tx_lol_table_t::map_from(void *data,uint32_t pts)
{
    data_f_t *d_f = static_cast<data_f_t *>(data);
    data_m_t d_m;

    _data_m.clear();

    for (uint32_t i = 0;i < pts;++i) {
        d_m.dc_i = d_f[i].dc_i;
        d_m.dc_q = d_f[i].dc_q;
        d_m.temp_5 = float(d_f[i].temp[1]);
        _data_m.push_back(d_m);
    }
}

string tx_lol_table_t::each_data_reception(const data_f_t &data,uint32_t row,uint32_t column) const
{
    if (column == 0) {
        return fmt::format("{:f}",data.freq / 1000000.0);
    }
    if (column == 1) {
        return fmt::format("{:d}",data.dc_i);
    }
    if (column == 2) {
        return fmt::format("{:d}",data.dc_q);
    }
    if (column == 3) {
        return data.use_sa_or_loopback == 0 ? "Spectrum" : "Loopback";
    }
    if (column == 4) {
        return fmt::format("{:f}",data.pwr);
    }
    if (column == 5) {
        return fmt::format("{:f}",data.temp[0]);
    }
    if (column == 6) {
        return fmt::format("{:f}",data.temp[1]);
    }
    if (column == 7) {
        return fmt::format("{:f}",data.temp[2]);
    }
    if (column == 8) {
        return fmt::format("{:f}",data.temp[3]);
    }
    if (column == 9) {
        return string_of<string>(data.time);
    }
    return "";
}
