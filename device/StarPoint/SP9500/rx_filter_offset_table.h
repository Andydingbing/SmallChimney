#ifndef RX_FILTER_OFFSET_TABLE_H
#define RX_FILTER_OFFSET_TABLE_H

#include "../cal_table.h"

namespace rd {

class API rx_filter_offset_table
{
public:
    struct API data_f_t {
        uint64_t freq;
        int64_t  offset[3];
        double   temp[4];
        tm       time;
        int32_t  rsv_0;
	public:
        data_f_t();
    };

    STRUCT_ALIGN_S(data_m_t, 4)
        int32_t offset[3];
        float   temp_5;
    STRUCT_ALIGN_E(data_m_t, 4)

    DECLARE_CAL_ITEM_TABLE(rx_filter_offset_table,ns_sp1401::r1c::rx_freqs_called)

public:
    void get(uint64_t freq,data_m_t *data);
    void get(uint64_t freq,data_f_t *data);
};

} //namespace rd

#endif
