#ifndef RX_ATT_TABLE_H
#define RX_ATT_TABLE_H

#include "../cal_table.h"

namespace rd {

class API rx_att_op_table_r1cd
{
public:
    typedef struct API data_f_t {
        uint64_t freq;
        int64_t  offset[R1C_RX_REF_OP_PTS - 3 + 1];
        double   temp[4];
        tm       time;
        int32_t  rsv_0;
	public:
        data_f_t();
    }data_f_t;

    STRUCT_ALIGN_S(data_m_t,4)
        int32_t offset[R1C_RX_REF_OP_PTS - 3 + 1];
        float   temp;
    STRUCT_ALIGN_E(data_m_t,4)

    DECLARE_CAL_ITEM_TABLE(rx_att_op_table_r1cd,ns_sp1401::r1c::rx_freqs_called)

public:
    int32_t get(ns_sp1401::hw_ver_t ver,uint64_t freq,double ref);
    void get(uint64_t freq,data_m_t *data);
    void get(uint64_t freq,data_f_t *data);
};

class API rx_att_io_table_r1cd
{
public:
    typedef struct API data_f_t {
        uint64_t freq;
        int64_t  offset[R1C_RX_REF_IO_PTS - 3 + 1];
        double   temp[4];
        tm       time;
        int32_t  rsv_0;
	public:
        data_f_t();
    }data_f_t;

    STRUCT_ALIGN_S(data_m_t,4)
        int32_t offset[R1C_RX_REF_IO_PTS - 3 + 1];
        float   temp;
    STRUCT_ALIGN_E(data_m_t,4)

    DECLARE_CAL_ITEM_TABLE(rx_att_io_table_r1cd,ns_sp1401::r1c::rx_freqs_called)

public:
    int32_t get(ns_sp1401::hw_ver_t ver,uint64_t freq,double ref);
    void get(uint64_t freq,data_m_t *data);
    void get(uint64_t freq,data_f_t *data);
};

} //namespace rd

#endif
