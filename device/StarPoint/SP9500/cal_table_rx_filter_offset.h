#ifndef DEVICE_STARPOINT_SP9500_CAL_TABLE_RX_FILTER_OFFSET_H
#define DEVICE_STARPOINT_SP9500_CAL_TABLE_RX_FILTER_OFFSET_H

#include "item_table.hpp"
#include "global_header.h"

namespace ns_starpoint {
namespace ns_sp9500 {

struct API data_f_rx_filter_offset : item_table_base::data_f_base_t
{
    int64_t offset[3];
    double temp[4];

    data_f_rx_filter_offset();
};

struct data_m_rx_filter_offset
{
    int32_t offset[3];
    float temp_5;
};

class API rx_filter_offset_table_t : public item_table<data_f_rx_filter_offset,data_m_rx_filter_offset>
{
public:
    typedef data_f_rx_filter_offset data_f_t;
    typedef data_m_rx_filter_offset data_m_t;

    void map_from(void *data,uint32_t pts) OVERRIDE;
    std::string each_data_reception(const data_f_t &data,uint32_t row,uint32_t column) const OVERRIDE;
    uint32_t each_data_reception_rows() const OVERRIDE { return 1; }
    uint32_t each_data_reception_columns() const OVERRIDE { return 9; }

    void save_as(const std::string &path);
};

} // namespace ns_sp9500
} // namespace ns_starpoint

#endif
