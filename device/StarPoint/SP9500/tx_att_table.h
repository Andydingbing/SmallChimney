#ifndef TX_ATT_TABLE_H
#define TX_ATT_TABLE_H

#include "../cal_table.h"

namespace rd {

class API tx_att_table_r1ab
{
public:
    typedef struct data_f_t {
        uint64_t freq;
        float	 att_10;
        float	 att_20;
        float	 att_30;
        float	 att_40;
        float	 att_50;
        float	 att_60;
        float	 att_70;
        float	 att_80;
        float	 att_90;
        int32_t  rsv_0;
    }data_f_t;

    STRUCT_ALIGN_S(data_m_t,4)
        float att[9];
    STRUCT_ALIGN_E(data_m_t,4)

    DECLARE_CAL_ITEM_TABLE(tx_att_table_r1ab,ns_sp1401::tx_freqs_called)

public:
    void get(uint64_t freq, float *att);
    void get(uint64_t freq, int32_t att,double &offset);
};

class API tx_att_op_table_r1cd
{
public:
    typedef struct API data_f_t {
        uint64_t freq;
        float    offset[R1C_TX_ATT_OP_POWER_PTS];
        double   temp[4];
        tm       time;
        int32_t  rsv_0;
	public:
        data_f_t();
    }data_f_t;

    STRUCT_ALIGN_S(data_m_t,4)
        float offset[R1C_TX_ATT_OP_POWER_PTS];
        float temp_5;
    STRUCT_ALIGN_E(data_m_t,4)

    DECLARE_CAL_ITEM_TABLE(tx_att_op_table_r1cd,ns_sp1401::tx_freqs_called)

public:
    float get(uint64_t freq,double pwr);
    void get(uint64_t freq,data_m_t *data);
    void get(uint64_t freq,data_f_t *data);
};

class API tx_att_io_table_r1cd
{
public:
    typedef struct API data_f_t {
        uint64_t freq;
        float    offset[R1C_TX_ATT_IO_POWER_PTS];
        int32_t  rsv_0;
        double   temp[4];
        tm       time;
        int32_t  rsv_1;
	public:
        data_f_t();
    }data_f_t;

    STRUCT_ALIGN_S(data_m_t,4)
        float offset[R1C_TX_ATT_IO_POWER_PTS];
        float temp_5;
    STRUCT_ALIGN_E(data_m_t,4)

    DECLARE_CAL_ITEM_TABLE(tx_att_io_table_r1cd,ns_sp1401::tx_freqs_called)

public:
    float get(uint64_t freq,double pwr);
    void get(uint64_t freq,data_m_t *data);
    void get(uint64_t freq,data_f_t *data);
};

} //namespace rd

#endif
