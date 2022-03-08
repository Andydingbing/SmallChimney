#include "cal_table_rx_ref.h"
#include "fmt/core.h"

using namespace std;
using namespace ns_starpoint;
using namespace ns_sp9500;

rx_state_f_t::rx_state_f_t()
{
    INIT_ARRAY(temp,-100.0);
    ad_offset = 0;
    lna_att = 0;
    att_019 = 0;
    att1 = 0.0;
    att2 = 0.0;
    att3 = 0.0;

    rsv_0 = 0;
    rsv_1 = 0.0;
}

void rx_ref_table_t::map_from(void *data,uint32_t pts)
{
    data_f_t *d_f = static_cast<data_f_t *>(data);
    data_m_t d_m;

    _data_m.clear();

    for (uint32_t i = 0;i < pts;++i) {
        for (uint32_t j = 0;j < ARRAY_SIZE(d_m.state);++j) {
            d_m.state[j].att1 = d_f[i].state[j].att1;
            d_m.state[j].att2 = d_f[i].state[j].att2;
            d_m.state[j].att3 = d_f[i].state[j].att3;
            d_m.state[j].temp = float(d_f[i].state[j].temp[0]);
            d_m.state[j].ad_offset = int32_t(d_f[i].state[j].ad_offset);
            d_m.state[j].lna_att = short(d_f[i].state[j].lna_att);
            d_m.state[j].att_019 = short(d_f[i].state[j].att_019);
        }
        _data_m.push_back(d_m);
    }
}

string rx_ref_table_t::each_data_reception(const data_f_t &data,uint32_t row,uint32_t column) const
{
    int ref_idx  = row % 3;

    if (column == 0) {
        return fmt::format("{:f}",data.freq / 1000000.0);
    }
    if (column == 1) {
        return ref_idx == 1 ? "-20" : "10";
    }
    if (column == 2) {
        return data.state[ref_idx].lna_att ? "LNA" : "Att";
    }
    if (column == 3) {
        return data.state[ref_idx].att_019 ? "Att 0" : "Att 19";
    }
    if (column == 4) {
        return fmt::format("{:f}",data.state[ref_idx].att1);
    }
    if (column == 5) {
        return fmt::format("{:f}",data.state[ref_idx].att2);
    }
    if (column == 6) {
        return fmt::format("{:f}",data.state[ref_idx].att3);
    }
    if (column == 7) {
        return fmt::format("{:d}",data.state[ref_idx].ad_offset);
    }
    if (column == 8) {
        return fmt::format("{:f}",data.state[ref_idx].temp[0]);
    }
    if (column == 9) {
        return fmt::format("{:f}",data.state[ref_idx].temp[1]);
    }
    if (column == 10) {
        return fmt::format("{:f}",data.state[ref_idx].temp[2]);
    }
    if (column == 11) {
        return fmt::format("{:f}",data.state[ref_idx].temp[3]);
    }
    if (column == 12) {
        return string_of<string>(data.time);
    }
    return "";
}
