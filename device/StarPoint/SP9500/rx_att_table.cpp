#include "rx_att_table.h"
#include "algo_math.hpp"

using namespace rd;
using namespace ns_sp1401;

// r1c/d output mode
rx_att_op_table_r1cd::data_f_t::data_f_t()
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

IMPLEMENT_RX_CAL_ITEM_TABLE(rx_att_op_table_r1cd)

void rx_att_op_table_r1cd::map2mem()
{
    if (m_data_f == nullptr)
		return;

    uint32_t idx_f = 0;
    uint32_t idx_m = 0;
    uint64_t freq = 0;

    for (idx_m = 0;idx_m < r1c::rx_freqs_called;idx_m ++) {
        freq = rx_freq_star + idx_m * r1c::rx_freq_step_called;
        idx_f = SERIE_INDEX(freq,rx_freq_star,rx_freq_step_infile);
        for (uint32_t i = 0;i < ARRAY_SIZE(m_data_m[0].offset);i ++) {
            m_data_m[idx_m].offset[i] = int32_t(m_data_f[idx_f].offset[i]);
            m_data_m[idx_m].temp = float(m_data_f[idx_f].temp[0]);
		}
	}
}

int32_t rx_att_op_table_r1cd::get(hw_ver_t ver,uint64_t freq,double ref)
{
    data_m_t data;
    get(freq,&data);

    // Ceil and limit the ref.
    // 1.1 / 1.9 ---> 2
    // -1.1 / -1.9 ---> -1
    // 30.0 / > 30.0 ---> 30.0
    // -40.0 / < -40.0 ---> -40.0
    int32_t i_ref = int32_t(ceil(ref));
    //int32_t i_ref = (ref >= 0.0 ? int32_t(ref) + 1 : int32_t(ref));
    i_ref = i_ref > R1C_RX_REF_OP_STAR ? R1C_RX_REF_OP_STAR : i_ref;
    i_ref = i_ref < R1C_RX_REF_OP_STOP ? R1C_RX_REF_OP_STOP : i_ref;

    uint32_t idx_ref = SERIE_INDEX(i_ref,R1C_RX_REF_OP_STAR,R1C_RX_REF_STEP);

    if (is_rf_ver_between(ver,R1C,R1E)) {
        if (i_ref > 10) {
            return data.offset[idx_ref];
        } else if (i_ref == 10 || i_ref == -20) {
            return 0;
        } else if (i_ref > -20) {
            return data.offset[idx_ref - 1];
        } else {
            return data.offset[idx_ref - 2];
        }
    } else if (is_rf_ver_after(ver,R1F)) {
        if (i_ref > 10) {
            return data.offset[idx_ref];
        } else if (i_ref == 10 || i_ref == -10) {
            return 0;
        } else if (i_ref > -10) {
            return data.offset[idx_ref - 1];
        } else {
            return data.offset[idx_ref - 2];
        }
    }
    return 0;
}

void rx_att_op_table_r1cd::get(uint64_t freq,data_m_t *data)
{
    uint64_t freq_l = 0,freq_r = 0;
    discretept(freq,r1c::rx_freq_step_called,freq_l,freq_r);

    uint32_t idx_l = SERIE_INDEX(freq_l,rx_freq_star,r1c::rx_freq_step_called);
    *data = m_data_m[idx_l];
}

void rx_att_op_table_r1cd::get(uint64_t freq,data_f_t *data)
{
    data_m_t data_m;
    get(freq,&data_m);

    uint64_t freq_l = 0,freq_r = 0;
    discretept(freq,r1c::rx_freq_step_called,freq_l,freq_r);

    *data = m_data_f[SERIE_INDEX(freq_l,rx_freq_star,rx_freq_step_infile)];
    data->freq = freq;
    for (uint32_t i = 0;i < ARRAY_SIZE((*data).offset);i ++) {
        data->offset[i] = data_m.offset[i];
    }
}

void rx_att_op_table_r1cd::save_as(const char *path)
{
    FILE *fp = fopen(path,"w");
    if (fp == nullptr)
		return;

// 	float fAtt[9];
// 	memset(fAtt,0,ARRAY_SIZE(fAtt));
// 
// 	fprintf(fp,"freq \t att1_10 \t att1_20 \t att1_30 \t att2_10 \t att2_20 \t att2_30 \t att3_10 \t att3_20 \t att3_30\n");
// 	for (uint64_t freq = CAL_RF_TX_FREQ_STAR;freq <= CAL_RF_TX_FREQ_STOP;freq += CAL_RF_TX_FREQ_STEP_INFILE) {
// 		get(freq,fAtt);
// 		fprintf(fp,"freq : %llu \t %f \t %f \t %f \t %f \t %f \t %f \t %f \t %f \t %f \n",
// 					freq,fAtt[0],fAtt[1],fAtt[2],fAtt[3],fAtt[4],fAtt[5],fAtt[6],fAtt[7],fAtt[8]);
// 	}
	fclose(fp);
}

// r1c/d io mode
rx_att_io_table_r1cd::data_f_t::data_f_t()
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

IMPLEMENT_RX_CAL_ITEM_TABLE(rx_att_io_table_r1cd)

void rx_att_io_table_r1cd::map2mem()
{
    if (nullptr == m_data_f)
		return;

    uint32_t idx_f = 0,idx_m = 0;
    uint64_t freq = 0;
    for (idx_m = 0;idx_m < r1c::rx_freqs_called;idx_m ++) {
        freq = rx_freq_star + idx_m * r1c::rx_freq_step_called;
        idx_f = SERIE_INDEX(freq,rx_freq_star,rx_freq_step_infile);
        for (uint32_t i = 0;i < ARRAY_SIZE(m_data_m[0].offset);i ++) {
            m_data_m[idx_m].offset[i] = int32_t(m_data_f[idx_f].offset[i]);
            m_data_m[idx_m].temp = float(m_data_f[idx_f].temp[0]);
		}
	}
}

int32_t rx_att_io_table_r1cd::get(hw_ver_t ver,uint64_t freq,double ref)
{
    data_m_t data;
    get(freq,&data);

    int32_t i_ref = int32_t(ceil(ref));
//    int32_t i_ref = (ref >= 0 ? int32_t(ref) + 1 : int32_t(ref));
    i_ref = i_ref > R1C_RX_REF_IO_STAR ? R1C_RX_REF_IO_STAR : i_ref;
    i_ref = i_ref < R1C_RX_REF_IO_STOP ? R1C_RX_REF_IO_STOP : i_ref;

    uint32_t idx_ref = SERIE_INDEX(i_ref,R1C_RX_REF_IO_STAR,R1C_RX_REF_STEP);

    if (is_rf_ver_between(ver,R1C,R1E)) {
        if (i_ref > 10) {
            return data.offset[idx_ref];
        } else if (i_ref == 10 || i_ref == -20) {
            return 0;
        } else if (i_ref > -20) {
            return data.offset[idx_ref - 1];
        } else {
            return data.offset[idx_ref - 2];
        }
    } else if (is_rf_ver_after(ver,R1F)) {
        if (i_ref > 10) {
            return data.offset[idx_ref];
        } else if (i_ref == 10 || i_ref == 0) {
            return 0;
        } else if (i_ref > 0) {
            return data.offset[idx_ref - 1];
        } else {
            return data.offset[idx_ref - 2];
        }
    }
    return 0;
}

void rx_att_io_table_r1cd::get(uint64_t freq,data_m_t *data)
{
    uint64_t freq_l = 0,freq_r = 0;
    discretept(freq,r1c::rx_freq_step_called,freq_l,freq_r);

    uint32_t idx_l = SERIE_INDEX(freq_l,rx_freq_star,r1c::rx_freq_step_called);
    *data = m_data_m[idx_l];
}

void rx_att_io_table_r1cd::get(uint64_t freq,data_f_t *data)
{
    data_m_t data_m;
    get(freq,&data_m);

    uint64_t freq_l = 0,freq_r = 0;
    discretept(freq,r1c::rx_freq_step_called,freq_l,freq_r);

    *data = m_data_f[SERIE_INDEX(freq_l,rx_freq_star,rx_freq_step_infile)];
    data->freq = freq;
    for (uint32_t i = 0;i < ARRAY_SIZE((*data).offset);i ++) {
        data->offset[i] = data_m.offset[i];
    }
}

void rx_att_io_table_r1cd::save_as(const char *path)
{
    FILE *fp = fopen(path,"w");
    if (fp == nullptr)
		return;

	// 	float fAtt[9];
	// 	memset(fAtt,0,ARRAY_SIZE(fAtt));
	// 
	// 	fprintf(fp,"freq \t att1_10 \t att1_20 \t att1_30 \t att2_10 \t att2_20 \t att2_30 \t att3_10 \t att3_20 \t att3_30\n");
    // 	for (uint64_t freq = CAL_RF_TX_FREQ_STAR;freq <= CAL_RF_TX_FREQ_STOP;freq += CAL_RF_TX_FREQ_STEP_INFILE) {
    // 		get(freq,fAtt);
	// 		fprintf(fp,"freq : %llu \t %f \t %f \t %f \t %f \t %f \t %f \t %f \t %f \t %f \n",
    // 					freq,fAtt[0],fAtt[1],fAtt[2],fAtt[3],fAtt[4],fAtt[5],fAtt[6],fAtt[7],fAtt[8]);
	// 	}
	fclose(fp);
}
