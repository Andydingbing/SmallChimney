#ifndef DEVICE_ERICSSON_RADIO_4415_CAL_TABLE_RX_RF_VGA_H
#define DEVICE_ERICSSON_RADIO_4415_CAL_TABLE_RX_RF_VGA_H

#include "item_table.hpp"
#include "global_header.h"

namespace ns_ericsson {
namespace ns_radio_4415 {

struct API data_f_rx_rf_vga : item_table_base::data_f_base_t
{
    float att[32];
    float pwr[32];
    float temp[4];

    data_f_rx_rf_vga();

    uint32_t valid_results() const;
    float dynamic_range() const;
    bool is_monotonic() const;
};

struct data_m_rx_rf_vga
{
    float att[32];
    float pwr[32];
    float temp_avg;
};

class API rx_rf_vga_table_t : public item_table<data_f_rx_rf_vga,data_m_rx_rf_vga>
{
public:
    typedef data_f_rx_rf_vga data_f_t;
    typedef data_m_rx_rf_vga data_m_t;

    void map_from(void *data,uint32_t pts) OVERRIDE;
    std::string each_data_reception(const data_f_t &data,uint32_t row,uint32_t column) const OVERRIDE;

    uint32_t each_data_reception_rows() const OVERRIDE
    { return ARRAY_SIZE(data_f_t::att); }

    uint32_t each_data_reception_columns() const OVERRIDE
    { return 3; }

    uint32_t each_data_reception_points(const data_f_t &data) const OVERRIDE
    { return data.valid_results(); }

    double each_data_reception_x(const data_f_t &data,uint32_t idx) const OVERRIDE
    { return data.att[idx]; }

    double each_data_reception_y(const data_f_t &data,uint32_t idx) const OVERRIDE
    { return data.pwr[idx]; }

    void save_as(const std::string &path);
};

} // namespace ns_radio_4415
} // namespace ns_ericsson

#endif
