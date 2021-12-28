#ifndef DEVICE_ERICSSON_AIR_3268_CAL_TABLE_TX_VGA_H
#define DEVICE_ERICSSON_AIR_3268_CAL_TABLE_TX_VGA_H

#include "item_table.hpp"
#include "global_header.h"

namespace ns_ericsson {
namespace ns_air_3268 {

struct API data_f_tx_vga : item_table_base::data_f_base_t
{
    uint32_t dac[64];
    float att[64];
    float temp[4];

    data_f_tx_vga();

    uint32_t valid_results() const;
    float dynamic_range() const;
};

struct data_m_tx_vga
{
    uint32_t dac[64];
    float att[64];
    float temp_avg;
};

class tx_vga_table_t : public item_table<data_f_tx_vga,data_m_tx_vga>
{
public:
    typedef data_f_tx_vga data_f_t;
    typedef data_m_tx_vga data_m_t;

    void map_from(void *data,uint32_t pts);
    void save_as(const std::string &path);
};

} // namespace ns_air_3268
} // namespace ns_ericsson

#endif // DEVICE_ERICSSON_AIR_3268_CAL_TABLE_TX_VGA_H
