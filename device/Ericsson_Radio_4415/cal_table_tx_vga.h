#ifndef DEVICE_ERICSSON_RADIO_4415_CAL_TABLE_TX_VGA_H
#define DEVICE_ERICSSON_RADIO_4415_CAL_TABLE_TX_VGA_H

#include "item_table.hpp"
#include "global_header.h"

namespace ns_ericsson {
namespace ns_radio_4415 {

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

class API tx_vga_table_t : public item_table<data_f_tx_vga,data_m_tx_vga>
{
public:
    typedef data_f_tx_vga data_f_t;
    typedef data_m_tx_vga data_m_t;

    void map_from(void *data,uint32_t pts) OVERRIDE;
    std::string each_data_reception(const data_f_t &data,uint32_t row,uint32_t column) const OVERRIDE;
    uint32_t each_data_reception_rows() const OVERRIDE { return ARRAY_SIZE(data_f_t::att); }
    uint32_t each_data_reception_columns() const OVERRIDE { return 3; }
    uint32_t each_data_reception_points(const data_f_t &data) const OVERRIDE;
    double   each_data_reception_x(const data_f_t &data,uint32_t idx) const OVERRIDE;
    double   each_data_reception_y(const data_f_t &data,uint32_t idx) const OVERRIDE;
    void save_as(const std::string &path);
};

} // namespace ns_radio_4415
} // namespace ns_ericsson

#endif
