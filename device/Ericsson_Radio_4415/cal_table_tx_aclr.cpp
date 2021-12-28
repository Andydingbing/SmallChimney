#include "cal_table_tx_aclr.h"
#include "fmt/core.h"

using namespace std;
using namespace ns_ericsson;
using namespace ns_radio_4415;

data_f_tx_aclr::data_f_tx_aclr()
{
    ZERO_ARRAY(acp);
    INIT_ARRAY(temp,-100.0);
}

void tx_aclr_table_t::map_from(void *data,uint32_t pts)
{
    data_f_t *d_f = static_cast<data_f_t *>(data);
    data_m_t d_m;

    _data_m.clear();

    for (uint32_t i = 0;i < pts;++i) {
        for (uint32_t j = 0;j < ARRAY_SIZE(data_f_t::acp);++j) {
            d_m.acp[j] = d_f[i].acp[j];
        }
        _data_m.push_back(d_m);
    }
}

string tx_aclr_table_t::each_data_reception(const data_f_t &data,uint32_t row,uint32_t column) const
{
    if (column == 0) {
        return fmt::format("{:f}",data.acp[row]);
    }
    if (column == 1) {
        return string_of<string>(data.time);
    }
    return "";
}
