#ifndef DEVICE_CAL_TABLE_FREQ_RESPONSE_H
#define DEVICE_CAL_TABLE_FREQ_RESPONSE_H

#include "item_table.hpp"
#include "global_header.h"

struct API data_f_freq_response : item_table_base::data_f_base_t
{
    double pwr;

    data_f_freq_response() : pwr(-100.0) {}
};

struct data_m_freq_response
{
    float pwr;
};

class freq_response_table_t : public item_table<data_f_freq_response,data_m_freq_response>
{
public:
    typedef data_f_freq_response data_f_t;
    typedef data_m_freq_response data_m_t;

    void map_from(void *data,uint32_t pts) OVERRIDE;

    std::string each_data_reception(const data_f_t &data,uint32_t row,uint32_t column) const OVERRIDE;
    uint32_t each_data_reception_rows() const OVERRIDE { return 1; }
    uint32_t each_data_reception_columns() const OVERRIDE { return 3; }
};

#endif
