#ifndef TX_TC_TABLE_H
#define TX_TC_TABLE_H

#include "../cal_table.h"

template<uint8_t states,uint32_t temps>
class tp_table
{
public:
    struct data_f_t {
        uint64_t freq;
        common_tmps_t temp[temps];
        double pwr[temps];

        data_f_t() { freq = 0; ZERO_ARRAY(temp); ZERO_ARRAY(pwr); }
    };
};

// TX temperature property table
typedef class API tx_tp_table
{
public:
    struct API data_f_t {
        uint64_t freq;
        common_tmps_t temp;
        double pwr[1001];

        data_f_t();
        void set(const double temp,const double new_pwr);
        bool is_ready_to_fit(const double temp_star,const double temp_stop) const;
        uint32_t fit(const double temp_star,const double temp_stop,
                     const uint32_t n_max,std::vector<double> &coef) const;
    };

    void add(const data_f_t *data);
    void get(uint64_t freq,data_f_t *data);
    void map2buf(data_f_t *buf) { m_data_f = buf; }

protected:
    data_f_t *m_data_f;
} rx_tp_table;


// Tx temperature compansate table
typedef class API tx_tc_table
{
public:
    struct API data_f_t {
        uint64_t freq;
        double coef[16];
        double temp_star;
        double temp_stop;
        uint32_t order;
        tm time;
        int64_t rsv_0;

        data_f_t();
    };

    STRUCT_ALIGN_S(data_m_t,8)
        double *coef;
        double temp_star;
        double temp_stop;
        uint32_t order;

        data_m_t() { coef = nullptr; order = 0; }
        ~data_m_t() { SAFE_DEL_ARRAY(coef); }
    STRUCT_ALIGN_E(data_m_t,8)

    void get(const uint64_t freq,data_f_t *data);
    void get(const uint64_t freq,data_m_t &data);

    DECLARE_CAL_ITEM_TABLE(tx_tc_table,ns_sp1401::r1c::tx_temp_comp_freqs)
} rx_tc_table;

#endif
