#ifndef DEVICE_STARPOINT_SP9500_CAL_TABLE_X9119_H
#define DEVICE_STARPOINT_SP9500_CAL_TABLE_X9119_H

#include "item_table.hpp"
#include "global_header.h"

namespace ns_starpoint {
namespace ns_sp9500 {

struct API data_f_x9119 : item_table_base::data_f_base_t
{
    uint16_t value;

    data_f_x9119();
};

struct data_m_x9119
{
    uint16_t value;
};

class x9119_table_t : public item_table<data_f_x9119,data_m_x9119>
{
public:
    typedef data_f_x9119 data_f_t;
    typedef data_m_x9119 data_m_t;

    void map_from(void *data,uint32_t pts) OVERRIDE;
    std::string each_data_reception(const data_f_t &data,uint32_t row,uint32_t column) const OVERRIDE;
    uint32_t each_data_reception_rows() const OVERRIDE { return 1; }
    uint32_t each_data_reception_columns() const OVERRIDE { return 2; }
};

} // namespace ns_sp9500
} // namespace ns_starpoint

#endif
