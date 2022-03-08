#ifndef DEVICE_STARPOINT_SP9500_CAL_TABLE_RX_REF_H
#define DEVICE_STARPOINT_SP9500_CAL_TABLE_RX_REF_H

#include "item_table.hpp"
#include "global_header.h"

namespace ns_starpoint {
namespace ns_sp9500 {

struct API rx_state_f_t {
    double temp[4];
    int64_t ad_offset;
    int16_t lna_att;
    int16_t att_019;
    int32_t rsv_0;
    float att1;
    float att2;
    float att3;
    float rsv_1;

    rx_state_f_t();
};

struct rx_state_m_t {
    float temp;
    float att1;
    float att2;
    float att3;
    int32_t ad_offset;
    short lna_att : 8;
    short att_019 : 8;
};

struct API data_f_rx_ref : item_table_base::data_f_base_t
{
    rx_state_f_t state[3];
};

struct data_m_rx_ref
{
    rx_state_m_t state[3];
};

class rx_ref_table_t : public item_table<data_f_rx_ref,data_m_rx_ref>
{
public:
    typedef data_f_rx_ref data_f_t;
    typedef data_m_rx_ref data_m_t;

    void map_from(void *data,uint32_t pts) OVERRIDE;
    std::string each_data_reception(const data_f_t &data,uint32_t row,uint32_t column) const OVERRIDE;
    uint32_t each_data_reception_rows() const OVERRIDE { return 3; }
    uint32_t each_data_reception_columns() const OVERRIDE { return 13; }
    void save_as(const std::string &path);
};

} // namespace ns_sp9500
} // namespace ns_starpoint

#endif
