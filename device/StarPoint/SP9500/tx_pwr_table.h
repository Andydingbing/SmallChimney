#ifndef TX_PWR_TABLE_H
#define TX_PWR_TABLE_H

#include "../cal_table.h"

class API tx_pwr_table_r1ab
{
public:
    typedef struct tx_state_f_t {
        uint64_t freq;
        double	 d_gain;
        int32_t  att1;
        int32_t  att2;
        int32_t  pa_att;
        int32_t  att3;
    }tx_state_f_t;

    typedef struct API data_f_t {
        tx_state_f_t state[5];
    }data_f_t;

    STRUCT_ALIGN_S(tx_state_m_t,4)
        float d_gain;
        short att1   : 8;
        short att2   : 8;
        short pa_att : 8;
        short att3   : 8;
    STRUCT_ALIGN_E(tx_state_m_t,4)

    STRUCT_ALIGN_S(data_m_t,8)
        tx_state_m_t state[5];
    STRUCT_ALIGN_E(data_m_t,8)

    DECLARE_CAL_ITEM_TABLE(tx_pwr_table_r1ab,ns_sp1401::tx_freqs_called)

public:
    uint32_t get_pwr_idx(double pwr);
    static void guess_base_op(int32_t &att1,int32_t &att2,int32_t &att3);
    static void guess_base_io(int32_t &att1,int32_t &att2,int32_t &att3);
    static void split(double att,double *rf,double *d);
    static void split(int32_t att,int32_t &att1,int32_t &att2,int32_t &att3);
    void get_op(uint64_t freq,double pwr,
                double &d_gain,int32_t &att1,int32_t &att2,int32_t &pa_att,int32_t &att3);
    void get_io(uint64_t freq,double pwr,
                double &d_gain,int32_t &att1,int32_t &att2,int32_t &pa_att,int32_t &att3);
};

class API tx_pwr_table_r1c
{
public:
    struct API data_f_t {
        uint64_t freq;
        double d_gain;
        double att0;
        double att1;
        double att2;
        double att3;
        double temp[4];
        tm time;
        int32_t rsv_0;
    public:
        data_f_t() :
            freq(0), d_gain(-5.0),
            att0(5.0), att1(20.0),
            att2(0.0), att3(1.0)
        {
            for (uint32_t i = 0;i < ARRAY_SIZE(temp);i ++) {
                temp[i] = 0.0;
            }
            time.tm_sec = 0;
            time.tm_min = 0;
            time.tm_hour = 0;
            time.tm_mday = 0;
            time.tm_mon = 0;
            time.tm_year = 0;
            time.tm_wday = 0;
            time.tm_yday = 0;
            time.tm_isdst = 0;
        }
    };

    STRUCT_ALIGN_S(data_m_t,4)
        float d_gain;
        float temp_5;
        short att0 : 8;	// att = attx / 2.0,so this can be a short.
        short att1 : 8;
        short att2 : 8;
        short att3 : 8;
    STRUCT_ALIGN_E(data_m_t,4)

public:
    virtual void get_base(uint64_t freq,data_m_t *data) = 0;
    virtual void get_base(uint64_t freq,data_f_t *data) = 0;
    virtual void get(ns_sp1401::hw_ver_t ver,uint64_t freq,double pwr,data_m_t *data) = 0;
    virtual void get(ns_sp1401::hw_ver_t ver,uint64_t freq,double pwr,data_f_t *data) = 0;
};

#define DECLARE_TX_PWR_TABLE_R1C \
public: \
    typedef tx_pwr_table_r1c::data_f_t data_f_t; \
    typedef tx_pwr_table_r1c::data_m_t data_m_t; \
    void get_base(uint64_t freq,data_m_t *data); \
    void get_base(uint64_t freq,data_f_t *data); \
    void get(ns_sp1401::hw_ver_t ver,uint64_t freq,double pwr,data_m_t *data); \
    void get(ns_sp1401::hw_ver_t ver,uint64_t freq,double pwr,data_f_t *data); \
    static void get_config_table_r1c(int32_t pwr,data_m_t data_base,data_m_t *data); \
    static void get_config_table_r1f(int32_t pwr,data_m_t data_base,data_m_t *data); \
    static void guess_base_r1c(double &att0,double &att1,double &att2,double &att3,double &d_gain); \
    static void guess_base_r1f(double &att0,double &att1,double &att2,double &d_gain);

class API tx_pwr_op_table_r1c : public tx_pwr_table_r1c
{
    DECLARE_TX_PWR_TABLE_R1C
    DECLARE_CAL_ITEM_TABLE(tx_pwr_op_table_r1c,ns_sp1401::tx_freqs_called)
};

class API tx_pwr_io_table_r1c : public tx_pwr_table_r1c
{
    DECLARE_TX_PWR_TABLE_R1C
    DECLARE_CAL_ITEM_TABLE(tx_pwr_io_table_r1c,ns_sp1401::tx_freqs_called)
};

#endif
