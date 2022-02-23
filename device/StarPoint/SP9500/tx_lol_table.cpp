#include "tx_lol_table.h"
#include "algo_math.hpp"

using namespace rd;
using namespace ns_sp1401;

// r1a/b
tx_lol_table_r1ab::data_f_t::data_f_t()
{
    freq = 0;
    dc_i = 0;
    dc_q = 0;
}

IMPLEMENT_TX_CAL_ITEM_TABLE(tx_lol_table_r1ab)

void tx_lol_table_r1ab::map2mem()
{
    if (m_data_f == nullptr)
		return;

    uint32_t idx_f = 0;
    uint32_t idx_m = 0;
    uint64_t freq = 0;

    for (idx_m = 0;idx_m < tx_freqs_called;idx_m ++) {
        freq = tx_freq_star + idx_m * tx_freq_step_called;
        idx_f = SERIE_INDEX(freq,tx_freq_star,tx_freq_step_infile);
        m_data_m[idx_m].dc_i = m_data_f[idx_f].dc_i;
        m_data_m[idx_m].dc_q = m_data_f[idx_f].dc_q;
	}
}

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

void tx_lol_table_r1ab::save_as(const char *path)
{

}

// r1c/d
tx_lol_table_r1cd::data_f_t::data_f_t()
{
    use_sa = false;
    pwr = 0.0;
    freq = 0;
    dc_i = 0;
    dc_q = 0;
    for (uint32_t i = 0;i < ARRAY_SIZE(temp);i ++)
        temp[i] = 0.0;
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

void tx_lol_table_r1cd::add(data_f_t *data)
{
    m_data_f[0] = *data;
}

void tx_lol_table_r1cd::map2mem()
{
    if (m_data_f == nullptr)
		return;

    m_data_m[0].dc_i = m_data_f[0].dc_i;
    m_data_m[0].dc_q = m_data_f[0].dc_q;
    m_data_m[0].temp_5 = float(m_data_f[0].temp[1]);	//temp4,5(used),6,7
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

void tx_lol_table_r1cd::save_as(const char *path)
{
    FILE *fp = fopen(path,"w");
    if (fp == nullptr)
		return;
    data_f_t data;
    uint64_t freq = tx_freq_star;
    get(freq,&data);
	fprintf(fp,"dc_i : %-5d \t dc_q : %-5d \t method : %-s \t power : %-.3f \t temp4 : %-.3f \t temp5 : %-.3f \t temp6 : %-.3f \t temp7 : %-.3f \t time : %4d/%02d/%02d %02d:%02d:%02d\n",
            data.dc_i,
            data.dc_q,
            data.use_sa ? "spectrum" : "loopback",
            data.pwr,
            data.temp[0],data.temp[1],data.temp[2],data.temp[3],
            data.time.tm_year + 1900,data.time.tm_mon,data.time.tm_mday,data.time.tm_hour,data.time.tm_min,data.time.tm_sec);
	fclose(fp);
}

//x9119 r1a/b/c/d
void x9119_table::map2mem()
{
    m_data_m[0].value = m_data_f[0].value;
}

void x9119_table::add(data_t *data)
{
    m_data_f[0] = *data;
}

void x9119_table::get(uint16_t &value)
{
    value = m_data_m[0].value;
}

void x9119_table::save_as(const char *path)
{
    FILE *fp = fopen(path,"w");
    if (fp == nullptr)
		return;
    uint16_t value = 0;
    get(value);
    fprintf(fp,"x9119 : %d",value);
	fclose(fp);
}
