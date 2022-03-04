#include "cal_table_x9119.h"
#include "fmt/core.h"

using namespace std;
using namespace ns_starpoint;
using namespace ns_sp9500;

data_f_x9119::data_f_x9119()
{
    value = 0;
}

void x9119_table_t::map_from(void *data,uint32_t pts)
{
    data_f_t *d_f = static_cast<data_f_t *>(data);
    data_m_t d_m;

    _data_m.clear();

    for (uint32_t i = 0;i < pts;++i) {
        d_m.value = d_f[i].value;
        _data_m.push_back(d_m);
    }
}

string x9119_table_t::each_data_reception(const data_f_t &data,uint32_t row,uint32_t column) const
{
    if (column == 0) {
        return fmt::format("{:d}",data.value);
    }
    if (column == 1) {
        return string_of<string>(data.time);
    }
    return "";
}
