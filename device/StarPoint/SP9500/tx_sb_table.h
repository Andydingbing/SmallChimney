#ifndef TX_SB_TABLE_H
#define TX_SB_TABLE_H

#include "../cal_table.h"

class API tx_sb_table_r1ab
{
public:
    struct API data_f_t {
        uint64_t freq;
        double   th;
        uint16_t am_i;
        uint16_t am_q;
        int32_t  rsv_0;
    public:
        data_f_t();
    };

    STRUCT_ALIGN_S(data_m_t,4)
        float    th;
        uint16_t am_i : 16;
        uint16_t am_q : 16;
    STRUCT_ALIGN_E(data_m_t,4)

    DECLARE_CAL_ITEM_TABLE(tx_sb_table_r1ab,ns_sp1401::tx_freqs_called)
public:
    void get(uint64_t freq,double &th,uint16_t &am_i,uint16_t &am_q);
};

class API tx_sb_table_r1cd
{
public:
    struct API data_f_t : public tx_sb_table_r1ab::data_f_t {
        bool    use_sa;	 // use sa or loopback
        double  pwr;     // sideband power
        double  temp[4];
        tm      time;
        int32_t rsv_1;

        data_f_t();
    };

    STRUCT_ALIGN_INHERIT_S(data_m_t,tx_sb_table_r1ab::data_m_t,4)
        float temp_5;	//hmc1197 temperature
    STRUCT_ALIGN_INHERIT_E(data_m_t,4)

    DECLARE_CAL_ITEM_TABLE(tx_sb_table_r1cd,1)
public:
    void get(uint64_t freq,double &th,uint16_t &am_i,uint16_t &am_q,float &temp_5);
    void get(uint64_t freq,data_m_t *data);
    void get(uint64_t freq,data_f_t *data);
};

#endif
