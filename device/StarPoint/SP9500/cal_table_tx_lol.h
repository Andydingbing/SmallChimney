#ifndef DEVICE_STARPOINT_SP9500_CAL_TABLE_TX_LOL_H
#define DEVICE_STARPOINT_SP9500_CAL_TABLE_TX_LOL_H

#include "item_table.hpp"
#include "global_header.h"

namespace ns_starpoint {
namespace ns_sp9500 {

struct API data_f_tx_lol : item_table_base::data_f_base_t
{
    int16_t dc_i;
    int16_t dc_q;
    int16_t use_sa_or_loopback;
    int16_t rsv_1;
    double temp[4];
    double pwr;

    data_f_tx_lol();
};

struct data_m_tx_lol
{
    int16_t dc_i;
    int16_t dc_q;
    float temp_5;
};

class API tx_lol_table_t : public item_table<data_f_tx_lol,data_m_tx_lol>
{
public:
    typedef data_f_tx_lol data_f_t;
    typedef data_m_tx_lol data_m_t;

    void map_from(void *data,uint32_t pts) OVERRIDE;
    std::string each_data_reception(const data_f_t &data,uint32_t row,uint32_t column) const OVERRIDE;
    uint32_t each_data_reception_rows() const OVERRIDE { return 1; }
    uint32_t each_data_reception_columns() const OVERRIDE { return 10; }

    void save_as(const std::string &path);
};

} // namespace ns_sp9500
} // namespace ns_starpoint

#endif
