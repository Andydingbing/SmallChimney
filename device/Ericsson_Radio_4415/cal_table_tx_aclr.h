#ifndef DEVICE_ERICSSON_RADIO_4415_CAL_TABLE_TX_ACLR_H
#define DEVICE_ERICSSON_RADIO_4415_CAL_TABLE_TX_ACLR_H

#include "item_table.hpp"
#include "global_header.h"

namespace ns_ericsson {
namespace ns_radio_4415 {

struct API data_f_tx_aclr : item_table_base::data_f_base_t
{
    float acp[4];
    float temp[4];

    data_f_tx_aclr();
};

struct data_m_tx_aclr
{
    float acp[4];
    float temp_avg;
};

class tx_aclr_table_t : public item_table<data_f_tx_aclr,data_m_tx_aclr>
{
public:
    typedef data_f_tx_aclr data_f_t;
    typedef data_m_tx_aclr data_m_t;

    void map_from(void *data,uint32_t pts) OVERRIDE;
    std::string each_data_reception(const data_f_t &data,uint32_t row,uint32_t column) const OVERRIDE;
    uint32_t each_data_reception_rows() const OVERRIDE { return ARRAY_SIZE(data_f_t::acp); }
    uint32_t each_data_reception_columns() const OVERRIDE { return 3; }
    void save_as(const std::string &path);
};

} // namespace ns_radio_4415
} // namespace ns_ericsson

#endif
