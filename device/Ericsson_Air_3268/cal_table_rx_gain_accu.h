#ifndef DEVICE_ERICSSON_AIR_3268_CAL_TABLE_RX_GAIN_ACCU_H
#define DEVICE_ERICSSON_AIR_3268_CAL_TABLE_RX_GAIN_ACCU_H

#include "item_table.hpp"
#include "global_header.h"

namespace ns_ericsson {
namespace ns_air_3268 {

struct API data_f_rx_gain_accu : item_table_base::data_f_base_t
{
    float pwr[4];
    float temp[4];

    data_f_rx_gain_accu();

    float accu() const;
};

struct data_m_rx_gain_accu
{
    float pwr[32];
    float temp_avg;
};

class rx_gain_accu_table_t : public item_table<data_f_rx_gain_accu,data_m_rx_gain_accu>
{
public:
    typedef data_f_rx_gain_accu data_f_t;
    typedef data_m_rx_gain_accu data_m_t;

    void map_from(void *data,uint32_t pts);
    void save_as(const std::string &path);
};

} // namespace ns_air_3268
} // namespace ns_ericsson

#endif
