#include "tx_filter_offset_table.h"
#include "algo_math.hpp"

using namespace ns_sp1401;

tx_filter_offset_table::data_f_t::data_f_t()
{
    freq = 0;
    offset = 0.0;
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

IMPLEMENT_TX_CAL_ITEM_TABLE(tx_filter_offset_table)

void tx_filter_offset_table::map2mem()
{
    if (m_data_f == nullptr)
		return;

    uint32_t idx_f = 0;
    uint32_t idx_m = 0;
    uint64_t freq = 0;

    for (idx_m = 0;idx_m < tx_freqs_called;idx_m ++) {
        freq = tx_freq_star + idx_m * tx_freq_step_called;
        idx_f = SERIE_INDEX(freq,tx_freq_star,tx_freq_step_infile);
        m_data_m[idx_m].offset = float(m_data_f[idx_f].offset);
        m_data_m[idx_m].temp_5 = float(m_data_f[idx_f].temp[1]);	//temp4,5(used),6,7
	}
}

void tx_filter_offset_table::get(uint64_t freq,data_m_t *data)
{
    uint64_t freq_l = 0,freq_r = 0;
    discretept(freq,tx_freq_step_called,freq_l,freq_r);

    uint32_t idx_l = SERIE_INDEX(freq_l,tx_freq_star,tx_freq_step_called);
    *data = m_data_m[idx_l];
}

void tx_filter_offset_table::get(uint64_t freq,data_f_t *data)
{
    uint64_t freq_l = 0,freq_r = 0;
    discretept(freq,tx_freq_step_called,freq_l,freq_r);

    uint32_t idx_l = SERIE_INDEX(freq_l,tx_freq_star,tx_freq_step_infile);
    *data = m_data_f[idx_l];
    data->freq = freq;
}

void tx_filter_offset_table::save_as(const char *path)
{

}
