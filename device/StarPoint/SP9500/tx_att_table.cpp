#include "tx_att_table.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "algo_math.hpp"

using namespace ns_sp1401;



void tx_att_op_table_r1cd::map2mem()
{
    if (m_data_f == nullptr)
		return;

    uint32_t idx_f = 0;
    uint32_t idx_m = 0;
    uint64_t freq = 0;

    for (idx_m = 0;idx_m < tx_freqs_called;idx_m ++) {
        freq = tx_freq_star + idx_m * tx_freq_step_called;
        idx_f = SERIE_INDEX(freq,tx_freq_star,tx_freq_step_infile);
        for (uint32_t i = 0;i < ARRAY_SIZE(m_data_m[0].offset);i ++)
            m_data_m[idx_m].offset[i] = m_data_f[idx_f].offset[i];
        m_data_m[idx_m].temp_5 = float(m_data_f[idx_f].temp[1]);
	}
}

float tx_att_op_table_r1cd::get(uint64_t freq,double pwr)
{
    data_m_t data;
    get(freq,&data);

    int32_t i_pwr = pwr >= 0 ? int32_t(pwr) + 1 : int32_t(pwr);
    i_pwr = i_pwr > R1C_TX_ATT_OP_POWER_STAR ? R1C_TX_ATT_OP_POWER_STAR : i_pwr;
    i_pwr = i_pwr < R1C_TX_ATT_OP_POWER_STOP ? R1C_TX_ATT_OP_POWER_STOP : i_pwr;

    uint32_t pwr_idx = SERIE_INDEX(i_pwr,R1C_TX_ATT_OP_POWER_STAR,R1C_TX_ATT_STEP);
    if (r1c::tx_base_pwr_op < i_pwr) {
        return data.offset[pwr_idx];
    } else if (r1c::tx_base_pwr_op == double(i_pwr)) {
		return 0.0;
    } else {
        return data.offset[pwr_idx -1];
    }
}

void tx_att_op_table_r1cd::get(uint64_t freq,data_m_t *data)
{
    uint64_t freq_l = 0,freq_r = 0;
    discretept(freq,tx_freq_step_called,freq_l,freq_r);

    uint32_t idx_l = SERIE_INDEX(freq_l,tx_freq_star,tx_freq_step_called);
    *data = m_data_m[idx_l];
}

void tx_att_op_table_r1cd::get(uint64_t freq,data_f_t *data)
{
    data_m_t data_m;
    get(freq,&data_m);

    uint64_t freq_l = 0,freq_r = 0;
    discretept(freq,tx_freq_step_called,freq_l,freq_r);

    *data = m_data_f[SERIE_INDEX(freq_l,tx_freq_star,tx_freq_step_infile)];
    data->freq = freq;
    for (uint32_t i = 0;i < ARRAY_SIZE((*data).offset);i ++) {
        data->offset[i] = data_m.offset[i];
    }
}


// r1c/d io mode

void tx_att_io_table_r1cd::map2mem()
{
    if (m_data_f == nullptr)
		return;

    uint32_t idx_f = 0;
    uint32_t idx_m = 0;
    uint64_t freq = 0;

    for (idx_m = 0;idx_m < tx_freqs_called;idx_m ++) {
        freq = tx_freq_star + idx_m * tx_freq_step_called;
        idx_f = SERIE_INDEX(freq,tx_freq_star,tx_freq_step_infile);
        for (uint32_t i = 0;i < ARRAY_SIZE(m_data_m[0].offset);i ++)
            m_data_m[idx_m].offset[i] = m_data_f[idx_f].offset[i];
        m_data_m[idx_m].temp_5 = float(m_data_f[idx_f].temp[1]);
	}
}

float tx_att_io_table_r1cd::get(uint64_t freq,double pwr)
{
    data_m_t data;
    get(freq,&data);

    int32_t i_pwr = pwr >= 0 ? int32_t(pwr) + 1 : int32_t(pwr);
    i_pwr = i_pwr > R1C_TX_ATT_IO_POWER_STAR ? R1C_TX_ATT_IO_POWER_STAR : i_pwr;
    i_pwr = i_pwr < R1C_TX_ATT_IO_POWER_STOP ? R1C_TX_ATT_IO_POWER_STOP : i_pwr;

    uint32_t pwr_idx = SERIE_INDEX(i_pwr,R1C_TX_ATT_IO_POWER_STAR,R1C_TX_ATT_STEP);
    if (r1c::tx_base_pwr_io < i_pwr) {
        return data.offset[pwr_idx];
    } else if (r1c::tx_base_pwr_io == double(i_pwr)) {
		return 0.0;
    } else {
        return data.offset[pwr_idx -1];
    }
}

void tx_att_io_table_r1cd::get(uint64_t freq,data_m_t *data)
{
    uint64_t freq_l = 0,freq_r = 0;
    discretept(freq,tx_freq_step_called,freq_l,freq_r);

    uint32_t idx_l = SERIE_INDEX(freq_l,tx_freq_star,tx_freq_step_called);
    *data = m_data_m[idx_l];
}

void tx_att_io_table_r1cd::get(uint64_t freq,data_f_t *data)
{
    data_m_t data_m;
    get(freq,&data_m);

    uint64_t freq_l = 0,freq_r = 0;
    discretept(freq,tx_freq_step_called,freq_l,freq_r);

    *data = m_data_f[SERIE_INDEX(freq_l,tx_freq_star,tx_freq_step_infile)];
    data->freq = freq;
    for (uint32_t i = 0;i < ARRAY_SIZE((*data).offset);i ++)
        data->offset[i] = data_m.offset[i];
}

void tx_att_io_table_r1cd::save_as(const char *path)
{

}
