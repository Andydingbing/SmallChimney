#include "tx_sb_table.h"
#include "algo_math.hpp"

using namespace ns_sp1401;

// r1a/b




void tx_sb_table_r1ab::get(uint64_t freq,double &th,uint16_t &am_i,uint16_t &am_q)
{
    uint64_t freq_l = 0,freq_r = 0;
    float f_th = 0.0;
    discretept(freq,tx_freq_step_called,freq_l,freq_r);

    uint32_t idx_l = SERIE_INDEX(freq_l,tx_freq_star,tx_freq_step_called);
    uint32_t idx_r = SERIE_INDEX(freq_r,tx_freq_star,tx_freq_step_called);
    data_m_t data_l = m_data_m[idx_l];
    data_m_t data_r = m_data_m[idx_r];

    linear(freq_l,data_l.th,freq_r,data_r.th,freq,f_th);
    th = double(f_th);
    linear(freq_l,data_l.am_i,freq_r,data_r.am_i,freq,am_i);
    linear(freq_l,data_l.am_q,freq_r,data_r.am_q,freq,am_q);
}


void tx_sb_table_r1cd::get(uint64_t freq,double &th,uint16_t &am_i,uint16_t &am_q,float &temp_5)
{
    th = double(m_data_m[0].th);
    am_i = m_data_m[0].am_i;
    am_q = m_data_m[0].am_q;
    temp_5 = m_data_m[0].temp_5;
}

void tx_sb_table_r1cd::get(uint64_t freq,data_m_t *data)
{
    data->th = m_data_m[0].th;
    data->am_i = m_data_m[0].am_i;
    data->am_q = m_data_m[0].am_q;
    data->temp_5 = m_data_m[0].temp_5;
}

void tx_sb_table_r1cd::get(uint64_t freq,data_f_t *data)
{
    *data = m_data_f[0];
    data->freq = freq;
}
