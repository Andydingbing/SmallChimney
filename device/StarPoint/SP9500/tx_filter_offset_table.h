#ifndef TX_FILTER_OFFSET_TABLE_H
#define TX_FILTER_OFFSET_TABLE_H

#include "../cal_table.h"

namespace rd {

class API tx_filter_offset_table
{
public:
    typedef struct API data_f_t {
        uint64_t freq;
        double   offset;
        double   temp[4];
        tm       time;
        int32_t  rsv_0;
	public:
        data_f_t();
    }data_f_t;

    STRUCT_ALIGN_S(data_m_t, 4)
        float offset;
        float temp_5;
    STRUCT_ALIGN_E(data_m_t, 4)

    DECLARE_CAL_ITEM_TABLE(tx_filter_offset_table,ns_sp1401::tx_freqs_called)

public:
    void get(uint64_t freq,data_m_t *data);
    void get(uint64_t freq,data_f_t *data);
};

} //namespace rd

#endif
