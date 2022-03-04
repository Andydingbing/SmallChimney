#include "tx_lol_table.h"
#include "algo_math.hpp"

using namespace ns_sp1401;



void tx_lol_table_r1ab::get(uint64_t freq,int16_t &dc_i,int16_t &dc_q)
{
    uint64_t freq_l = 0;
    uint64_t freq_r = 0;
    discretept(freq,tx_freq_step_called,freq_l,freq_r);

    uint32_t idx_l = SERIE_INDEX(freq_l,tx_freq_star,tx_freq_step_called);
    uint32_t idx_r = SERIE_INDEX(freq_r,tx_freq_star,tx_freq_step_called);
    data_m_t data_l = m_data_m[idx_l];
    data_m_t data_r = m_data_m[idx_r];
	
    linear(freq_l,data_l.dc_i,freq_r,data_r.dc_i,freq,dc_i);
    linear(freq_l,data_l.dc_q,freq_r,data_r.dc_q,freq,dc_q);
}


void tx_lol_table_r1cd::get(uint64_t freq,int16_t &dc_i,int16_t &dc_q,float &temp_5)
{
    dc_i = m_data_m[0].dc_i;
    dc_q = m_data_m[0].dc_q;
    temp_5 = m_data_m[0].temp_5;
}

void tx_lol_table_r1cd::get(uint64_t freq,data_m_t *data)
{
    data->dc_i = m_data_m[0].dc_i;
    data->dc_q = m_data_m[0].dc_q;
    data->temp_5 = m_data_m[0].temp_5;
}

void tx_lol_table_r1cd::get(uint64_t freq,data_f_t *data)
{
    *data = m_data_f[0];
    data->freq = freq;
}
