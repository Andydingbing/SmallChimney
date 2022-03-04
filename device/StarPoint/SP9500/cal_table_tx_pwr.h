#ifndef DEVICE_STARPOINT_SP9500_CAL_TABLE_TX_PWR_H
#define DEVICE_STARPOINT_SP9500_CAL_TABLE_TX_PWR_H

#include "item_table.hpp"
#include "global_header.h"

namespace ns_starpoint {
namespace ns_sp9500 {

struct API data_f_tx_pwr : item_table_base::data_f_base_t
{
    double d_gain;
    double att0;
    double att1;
    double att2;
    double att3;
    double temp[4];

    data_f_tx_pwr();
};

struct data_m_tx_pwr
{
    float d_gain;
    float temp_5;
    short att0 : 8;	// att = attx / 2.0,so this can be a short.
    short att1 : 8;
    short att2 : 8;
    short att3 : 8;
};

class tx_pwr_table_t : public item_table<data_f_tx_pwr,data_m_tx_pwr>
{
public:
    typedef data_f_tx_pwr data_f_t;
    typedef data_m_tx_pwr data_m_t;

    void map_from(void *data,uint32_t pts) OVERRIDE;
    std::string each_data_reception(const data_f_t &data,uint32_t row,uint32_t column) const OVERRIDE;
    uint32_t each_data_reception_rows() const OVERRIDE { return 1; }
    uint32_t each_data_reception_columns() const OVERRIDE { return 11; }
    void save_as(const std::string &path);
};

} // namespace ns_sp9500
} // namespace ns_starpoint

#endif
