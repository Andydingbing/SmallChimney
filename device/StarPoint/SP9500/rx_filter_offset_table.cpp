#include "rx_filter_offset_table.h"
#include "algo_math.hpp"

using namespace ns_sp1401;

rx_filter_offset_table::data_f_t::data_f_t()
{
    freq = 0;
    for (uint32_t i = 0;i < ARRAY_SIZE(offset);i ++) {
        offset[i] = 0;
    }
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

IMPLEMENT_RX_CAL_ITEM_TABLE(rx_filter_offset_table)

void rx_filter_offset_table::map2mem()
{
    if (nullptr == m_data_f) {
		return;
    }

    uint32_t idx_f = 0;
    uint32_t idx_m = 0;
    uint64_t freq = 0;

    for (idx_m = 0;idx_m < r1c::rx_freqs_called;idx_m ++) {
        freq = rx_freq_star + idx_m * r1c::rx_freq_step_called;
        idx_f = SERIE_INDEX(freq,rx_freq_star,rx_freq_step_infile);
        for (uint32_t i = 0;i < ARRAY_SIZE(m_data_m[idx_m].offset);i ++) {
            m_data_m[idx_m].offset[i] = int32_t(m_data_f[idx_f].offset[i]);
        }
        m_data_m[idx_m].temp_5 = float(m_data_f[idx_f].temp[1]);	//temp4,5(used),6,7
	}
}

void rx_filter_offset_table::get(uint64_t freq,data_m_t *data)
{
    uint64_t freq_l = 0,freq_r = 0;
    discretept(freq,r1c::rx_freq_step_called,freq_l,freq_r);

    uint32_t idx_l = SERIE_INDEX(freq_l,rx_freq_star,r1c::rx_freq_step_called);
    *data = m_data_m[idx_l];
}

void rx_filter_offset_table::get(uint64_t freq,data_f_t *data)
{
    uint64_t freq_l = 0,freq_r = 0;
    discretept(freq,r1c::rx_freq_step_called,freq_l,freq_r);

    uint32_t idx_l = SERIE_INDEX(freq_l,rx_freq_star,rx_freq_step_infile);
    *data = m_data_f[idx_l];
    data->freq = freq;
}

void rx_filter_offset_table::save_as(const char *path)
{

}
