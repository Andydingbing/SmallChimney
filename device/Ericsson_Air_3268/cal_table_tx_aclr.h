#ifndef DEVICE_ERICSSON_AIR_3268_CAL_TABLE_TX_ACLR_H
#define DEVICE_ERICSSON_AIR_3268_CAL_TABLE_TX_ACLR_H

#include "item_table.hpp"
#include "global_header.h"

namespace ns_ericsson {
namespace ns_air_3268 {

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

    void map_from(void *data,uint32_t pts);
    void save_as(const std::string &path);
};

} // namespace ns_air_3268
} // namespace ns_ericsson

#endif // DEVICE_ERICSSON_AIR_3268_CAL_TABLE_TX_ACLR_H
