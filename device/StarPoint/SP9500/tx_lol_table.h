#ifndef TX_LOL_TABLE_H
#define TX_LOL_TABLE_H

#include "../cal_table.h"

class API tx_lol_table_r1ab
{
public:
    struct API data_f_t {
        uint64_t freq;
        int16_t  dc_i;
        int16_t  dc_q;
        int32_t  rsv_0;
    public:
        data_f_t();
    };

    STRUCT_ALIGN_S(data_m_t,2)
        int16_t dc_i : 16;
        int16_t dc_q : 16;
    STRUCT_ALIGN_E(data_m_t,2)

    DECLARE_CAL_ITEM_TABLE(tx_lol_table_r1ab,ns_sp1401::tx_freqs_called)
public:
    void get(uint64_t freq,int16_t &dc_i,int16_t &dc_q);
};

class API tx_lol_table_r1cd
{
public:
    typedef struct API data_f_t : public tx_lol_table_r1ab::data_f_t {
        bool    use_sa;	//use sa or loopback
        double  pwr;     //lo leakage power
        double  temp[4];
        tm      time;
        int32_t rsv_1;
    public:
        data_f_t();
    }data_f_t;

    STRUCT_ALIGN_INHERIT_S(data_m_t,tx_lol_table_r1ab::data_m_t,4)
        float temp_5;	//hmc1197 temperature
    STRUCT_ALIGN_INHERIT_E(data_m_t,4)

    DECLARE_CAL_ITEM_TABLE(tx_lol_table_r1cd,1)

public:
    void get(uint64_t freq,int16_t &dc_i,int16_t &dc_q,float &temp_5);
    void get(uint64_t freq,data_m_t *data);
    void get(uint64_t freq,data_f_t *data);
};

class API x9119_table
{
public:
    struct data_t {
        uint16_t value;
        data_t() { value = 0; }
    };

public:
    void add(data_t *data);
    void map2buf(data_t *buf) { m_data_f = buf; }
    void map2mem();
    void get(uint16_t &value);
    void save_as(const char *path);

private:
    data_t *m_data_f;
    data_t m_data_m[1];
};

#endif
