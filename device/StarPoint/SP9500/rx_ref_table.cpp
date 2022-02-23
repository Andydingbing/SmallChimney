#include "rx_ref_table.h"
#include "algo_math.hpp"

using namespace rd;
using namespace rd::ns_sp9500;
using namespace ns_sp1401;

// R1A/B
rx_ref_table_r1ab::data_f_t::data_f_t()
{
    freq = 0;
    for (uint32_t i = 0;i < R1A_RX_REF_OP_PTS;i ++) {
        state_op[i].att1 = 0.0;
        state_op[i].ad_offset = 0;
        state_op[i].att2 = 0;
        state_op[i].lna_att = 0;
	}
    for (uint32_t i = 0;i < R1A_RX_REF_IO_PTS;i ++) {
        state_io[i].att1 = 0.0;
        state_io[i].ad_offset = 0;
        state_io[i].att2 = 0;
        state_io[i].lna_att = 0;
	}
}

IMPLEMENT_RX_CAL_ITEM_TABLE(rx_ref_table_r1ab)

// This is reserved for changing calibration step later.
void rx_ref_table_r1ab::map2mem()
{
    if (m_data_f == nullptr) {
		return;
    }

    uint32_t idx_f = 0;
    uint32_t idx_m = 0;
    uint64_t freq = 0;

    for (idx_m = 0;idx_m < r1a::rx_freqs_called;idx_m ++) {
        freq = rx_freq_star + idx_m * r1a::rx_freq_step_called;
        idx_f = SERIE_INDEX(freq,rx_freq_star,rx_freq_step_infile);
        for (uint32_t i = 0;i < R1A_RX_REF_OP_PTS;i ++) {
            m_data_m[idx_m].state_op[i].att1      = float(m_data_f[idx_f].state_op[i].att1);
            m_data_m[idx_m].state_op[i].ad_offset = int32_t(m_data_f[idx_f].state_op[i].ad_offset);
            m_data_m[idx_m].state_op[i].att2      = short(m_data_f[idx_f].state_op[i].att2);
            m_data_m[idx_m].state_op[i].lna_att   = short(m_data_f[idx_f].state_op[i].lna_att);
		}
        for (uint32_t i = 0;i < R1A_RX_REF_IO_PTS;i ++) {
            m_data_m[idx_m].state_io[i].att1      = float(m_data_f[idx_f].state_io[i].att1);
            m_data_m[idx_m].state_io[i].ad_offset = int32_t(m_data_f[idx_f].state_io[i].ad_offset);
            m_data_m[idx_m].state_io[i].att2      = short(m_data_f[idx_f].state_io[i].att2);
            m_data_m[idx_m].state_io[i].lna_att   = short(m_data_f[idx_f].state_io[i].lna_att);
		}
	}
}

int32_t rx_ref_table_r1ab::get_sg_pwr(int32_t ref)
{
    if (ref >= 10) {
		return 10;
    }
// 	if (ref <= -30) {
// 		return -30;
//  }
    return ref;
}

void rx_ref_table_r1ab::guess_base_op(int32_t &lna_att,double &att1,int32_t &att2)
{
    rx_ref_table_r1ab::rx_state_f_t rx_state_op[] = {
		/*********************************************************************/
                                                //Ref   //SG    //
        {0,int32_t(r1a::RX_ATT),0,-30,-20,0},	//30	//10	//_0dBFS_L - 20dBm
        {0,int32_t(r1a::RX_ATT),0,-20,-20,0},	//20	//10	//_0dBFS_L - 10dBm
        {0,int32_t(r1a::RX_ATT),0,-20,-10,0},	//10	//10	//_0dBFS_L
        {0,int32_t(r1a::RX_ATT),0,-15,-10,0},	//0		//0		//_0dBFS_L
        {0,int32_t(r1a::RX_ATT),0,-10, 0 ,0},	//-10	//-10	//_0dBFS_L (IL)
        {0,int32_t(r1a::RX_ATT),0,  0, 0 ,0},	//-20	//-20	//_0dBFS_L

        {0,int32_t(r1a::RX_LNA),0,  0, 0 ,0},	//-30	//-30	//_0dBFS_H
		/*********************************************************************/
        {0,int32_t(r1a::RX_ATT),0,  0, 0 ,0},	//-30			//_0dBFS_L - 10dB
        {0,int32_t(r1a::RX_ATT),0,  0, 0 ,0},	//-40			//_0dBFS_L - 20dB
        {0,int32_t(r1a::RX_ATT),0,  0, 0 ,0},	//-50			//_0dBFS_L - 30dB

        {0,int32_t(r1a::RX_LNA),0,  0, 0 ,0},	//-60			//_0dBFS_H - 30dB
        {0,int32_t(r1a::RX_LNA),0,  0, 0 ,0},	//-70			//_0dBFS_H - 40dB
        {0,int32_t(r1a::RX_LNA),0,  0, 0 ,0},	//-80			//_0dBFS_H - 50dB
        {0,int32_t(r1a::RX_LNA),0,  0, 0 ,0},	//-90			//_0dBFS_H - 60dB
        {0,int32_t(r1a::RX_LNA),0,  0, 0 ,0},	//-100			//_0dBFS_H - 70dB
	};

    lna_att = rx_state_op[SERIE_INDEX(0,R1A_RX_REF_OP_STAR,-10)].lna_att;
    att1    = rx_state_op[SERIE_INDEX(0,R1A_RX_REF_OP_STAR,-10)].att1;
    att2    = rx_state_op[SERIE_INDEX(0,R1A_RX_REF_OP_STAR,-10)].att2;
}

void rx_ref_table_r1ab::guess_base_io(int32_t &lna_att,double &att1,int32_t &att2)
{
    rx_ref_table_r1ab::rx_state_f_t rx_state_io[] = {
		/*********************************************************************/
                                                //Ref	//SG	//
        {0,int32_t(r1a::RX_ATT),0,-30,-20,0},	//30	//10	//_0dBFS_L - 20dBm
        {0,int32_t(r1a::RX_ATT),0,-20,-20,0},	//20	//10	//_0dBFS_L - 10dBm
        {0,int32_t(r1a::RX_ATT),0,-20,-10,0},	//10	//10	//_0dBFS_L
        {0,int32_t(r1a::RX_ATT),0,-10,-10,0},	//0		//0		//_0dBFS_L
        {0,int32_t(r1a::RX_ATT),0,-10, 0 ,0},	//-10	//-10	//_0dBFS_L (IL)
        {0,int32_t(r1a::RX_ATT),0, 0,  0 ,0},	//-20	//-20	//_0dBFS_L

        {0,int32_t(r1a::RX_LNA),0, 0,  0 ,0},	//-30	//-30	//_0dBFS_H
		/*********************************************************************/
        {0,int32_t(r1a::RX_ATT),0, 0,  0 ,0},	//-30			//_0dBFS_L - 10dB
        {0,int32_t(r1a::RX_ATT),0, 0,  0 ,0},	//-40			//_0dBFS_L - 20dB
        {0,int32_t(r1a::RX_ATT),0, 0,  0 ,0},	//-50			//_0dBFS_L - 30dB

        {0,int32_t(r1a::RX_LNA),0, 0,  0 ,0},	//-60			//_0dBFS_H - 30dB
        {0,int32_t(r1a::RX_LNA),0, 0,  0 ,0},	//-70			//_0dBFS_H - 40dB
        {0,int32_t(r1a::RX_LNA),0, 0,  0 ,0},	//-80			//_0dBFS_H - 50dB
        {0,int32_t(r1a::RX_LNA),0, 0,  0 ,0},	//-90			//_0dBFS_H - 60dB
        {0,int32_t(r1a::RX_LNA),0, 0,  0 ,0},	//-100			//_0dBFS_H - 70dB
	};
    lna_att = rx_state_io[SERIE_INDEX(0,R1A_RX_REF_IO_STAR,-10)].lna_att;
    att1    = rx_state_io[SERIE_INDEX(0,R1A_RX_REF_IO_STAR,-10)].att1;
    att2    = rx_state_io[SERIE_INDEX(0,R1A_RX_REF_IO_STAR,-10)].att2;
}

void rx_ref_table_r1ab::get_io(uint64_t freq,double ref,int64_t &ad_0dbfs,int32_t &lna_att,double &att1,int32_t &att2)
{
    freq = freq < rx_freq_star ? rx_freq_star : freq;
    freq = freq > rx_freq_stop ? rx_freq_stop : freq;
    ref = ref > R1A_RX_REF_IO_STAR ? R1A_RX_REF_IO_STAR : ref;
    ref = ref < R1A_RX_REF_IO_STOP ? R1A_RX_REF_IO_STOP : ref;

    uint64_t freq_l = 0,freq_r = 0;
    double ref_l = 0.0,ref_r = 0.0;
	
    discretept(freq,r1a::rx_freq_step_called,freq_l,freq_r);
    discretept(ref,R1A_RX_REF_STEP,ref_l,ref_r);

    uint32_t freq_idx_l = SERIE_INDEX(freq_l,rx_freq_star,r1a::rx_freq_step_called);
    uint32_t freq_idx_r = freq >= rx_freq_stop ? freq_idx_l : SERIE_INDEX(freq_r,rx_freq_star,r1a::rx_freq_step_called);
    uint32_t ref_idx_l = SERIE_INDEX(ref_l,R1A_RX_REF_IO_STAR,R1A_RX_REF_STEP);

    data_m_t data_m_m;
    data_m_t data_m_l = m_data_m[freq_idx_l];
    data_m_t data_m_r = m_data_m[freq_idx_r];

    for (uint32_t i = 0;i < ARRAY_SIZE(data_m_m.state_io);i ++) {
        if (data_m_l.state_io[i].lna_att == data_m_r.state_io[i].lna_att &&
            data_m_l.state_io[i].att1 == data_m_r.state_io[i].att1 &&
            data_m_l.state_io[i].att2 == data_m_r.state_io[i].att2) {
                linear(freq_l,data_m_l.state_io[i].ad_offset,
                       freq_r,data_m_r.state_io[i].ad_offset,
                       freq,  data_m_m.state_io[i].ad_offset);
        } else {
            data_m_m.state_io[i].ad_offset = data_m_l.state_io[i].ad_offset;
        }
		
        data_m_m.state_io[i].lna_att = data_m_l.state_io[i].lna_att;
        data_m_m.state_io[i].att1    = data_m_l.state_io[i].att1;
        data_m_m.state_io[i].att2    = data_m_l.state_io[i].att2;
	}
    ad_0dbfs = dBc_to_ad(g_0dBFS - data_m_l.state_io[ref_idx_l].ad_offset,ref_l - ref);
    lna_att  = data_m_m.state_io[ref_idx_l].lna_att;
    att1     = double(data_m_m.state_io[ref_idx_l].att1);
    att2     = data_m_m.state_io[ref_idx_l].att2;
}

void rx_ref_table_r1ab::get_op(uint64_t freq,double ref,int64_t &ad_0dbfs,int32_t &lna_att,double &att1,int32_t &att2)
{
    freq = freq < rx_freq_star ? rx_freq_star : freq;
    freq = freq > rx_freq_stop ? rx_freq_stop : freq;
    ref = ref > R1A_RX_REF_OP_STAR ? R1A_RX_REF_OP_STAR : ref;
    ref = ref < R1A_RX_REF_OP_STOP ? R1A_RX_REF_OP_STOP : ref;

    uint64_t freq_l = 0,freq_r = 0;
    double ref_l = 0.0,ref_r = 0.0;

    discretept(freq,r1a::rx_freq_step_called,freq_l,freq_r);
    discretept(ref,R1A_RX_REF_STEP,ref_l,ref_r);

    uint32_t freq_idx_l = SERIE_INDEX(freq_l,rx_freq_star,r1a::rx_freq_step_called);
    uint32_t freq_idx_r = freq >= rx_freq_stop ? freq_idx_l : SERIE_INDEX(freq_r,rx_freq_star,r1a::rx_freq_step_called);
    uint32_t ref_idx_l = SERIE_INDEX(ref_l,R1A_RX_REF_OP_STAR,R1A_RX_REF_STEP);

    data_m_t data_m_m;
    data_m_t data_m_l = m_data_m[freq_idx_l];
    data_m_t data_m_r = m_data_m[freq_idx_r];
	
    for (uint32_t i = 0;i < ARRAY_SIZE(data_m_m.state_op);i ++) {
        if (data_m_l.state_op[i].lna_att == data_m_r.state_op[i].lna_att &&
            data_m_l.state_op[i].att1 == data_m_r.state_op[i].att1 &&
            data_m_l.state_op[i].att2 == data_m_r.state_op[i].att2) {
                linear(freq_l,data_m_l.state_op[i].ad_offset,
                       freq_r,data_m_r.state_op[i].ad_offset,
                       freq,  data_m_m.state_op[i].ad_offset);
        } else {
            data_m_m.state_op[i].ad_offset = data_m_l.state_op[i].ad_offset;
        }

        data_m_m.state_op[i].lna_att = data_m_l.state_op[i].lna_att;
        data_m_m.state_op[i].att1	  = data_m_l.state_op[i].att1;
        data_m_m.state_op[i].att2   = data_m_l.state_op[i].att2;
	}
	
    ad_0dbfs = dBc_to_ad(g_0dBFS - data_m_l.state_op[ref_idx_l].ad_offset,ref_l - ref);
    lna_att  = data_m_m.state_op[ref_idx_l].lna_att;
    att1     = double(data_m_m.state_op[ref_idx_l].att1);
    att2     = data_m_m.state_op[ref_idx_l].att2;
}

void rx_ref_table_r1ab::get(uint64_t freq,double ref,io_mode_t mode,int64_t &ad_0dbfs,r1a::rx_lna_att_t &lna_att,double &att1,int32_t &att2)
{
    int32_t i_lna_att = 0;
    if (mode._to_integral() == io_mode_t::OUTPUT) {
        get_op(freq,ref,ad_0dbfs,i_lna_att,att1,att2);
    } else if (mode._to_integral() == io_mode_t::IO) {
        get_io(freq,ref,ad_0dbfs,i_lna_att,att1,att2);
    }
    lna_att = (0 == i_lna_att ? r1a::RX_ATT : r1a::RX_LNA);
}

void rx_ref_table_r1ab::save_as(const char *path)
{
}

// R1C/D/E/F
rx_ref_table_r1cd::data_f_t::data_f_t()
{
    freq = 0;
    for (uint32_t i = 0;i < ARRAY_SIZE(state);i ++) {
        state[i].lna_att = 0;
        state[i].att_019 = 1;
        state[i].ad_offset = 0;
        state[i].att1 = 0.0;
        state[i].att2 = 0.0;
        state[i].att3 = 0.0;
        for (uint32_t j = 0;j < ARRAY_SIZE(state[i].temp);j ++) {
            state[i].temp[j] = 0.0;
        }
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

// Output mode
IMPLEMENT_RX_CAL_ITEM_TABLE(rx_ref_op_table_r1cd)

void rx_ref_op_table_r1cd::map2mem()
{
    if (m_data_f == nullptr) {
		return;
    }

    uint32_t idx_f = 0;
    uint32_t idx_m = 0;
    uint64_t freq = 0;

    for (idx_m = 0;idx_m < r1c::rx_freqs_called;idx_m ++) {
        freq = rx_freq_star + idx_m * r1c::rx_freq_step_called;
        idx_f = SERIE_INDEX(freq,rx_freq_star,rx_freq_step_infile);
        for (uint32_t i = 0;i < ARRAY_SIZE(m_data_m[idx_m].state);i ++) {
            m_data_m[idx_m].state[i].att1 = m_data_f[idx_f].state[i].att1;
            m_data_m[idx_m].state[i].att2 = m_data_f[idx_f].state[i].att2;
            m_data_m[idx_m].state[i].att3 = m_data_f[idx_f].state[i].att3;
            m_data_m[idx_m].state[i].temp = float(m_data_f[idx_f].state[i].temp[0]);
            m_data_m[idx_m].state[i].ad_offset = int32_t(m_data_f[idx_f].state[i].ad_offset);
            m_data_m[idx_m].state[i].lna_att = short(m_data_f[idx_f].state[i].lna_att);
            m_data_m[idx_m].state[i].att_019 = short(m_data_f[idx_f].state[i].att_019);
		}
	}
}

void rx_ref_op_table_r1cd::get_config_table_r1c(int32_t ref,data_m_t data_ref,rx_state_m_t *state)
{
    /*
     * ref   att0   att1   att2   att3
     *  30    -20	 -25	-15	   -10
     *  20    -20	 -15	-15	   -10
     *	10    -20	  -5	-15	   -10 // state[0]
     *
     *	10      0	 -25	-15	   -10 // state[1]
     *	 0      0	 -15	-15	   -10
     * -10      0	  -5	-15	   -10
     * -20      0	  -5	 -5	   -10
     *
     * -20     14	 -10	-15	   -10 // state[2]
     * -30     14	   0	-15	   -10
     * -40     14	   0	 -5	   -10
     */

    short lna_att = 0;
    short att_019 = 0;
    float att1 = 0.0;
    float att2 = 0.0;
    float att3 = 0.0;
    int32_t ad_offset = 0;
    float temp = 0.0;

    if (ref > 10) {
        lna_att = data_ref.state[0].lna_att;
        att_019 = data_ref.state[0].att_019;
        att1    = data_ref.state[0].att1 + float(ref - 10);
        att2    = data_ref.state[0].att2;
        att3    = data_ref.state[0].att3;
        ad_offset = data_ref.state[0].ad_offset;
        temp = data_ref.state[0].temp;
    } else if (ref >= -10) {
        lna_att = data_ref.state[1].lna_att;
        att_019 = data_ref.state[1].att_019;
        att1    = data_ref.state[1].att1 + float(ref - 10);
        att2    = data_ref.state[1].att2;
        att3    = data_ref.state[1].att3;
        ad_offset = data_ref.state[1].ad_offset;
        temp = data_ref.state[1].temp;
    } else if (ref > -20) {
        lna_att = data_ref.state[1].lna_att;
        att_019 = data_ref.state[1].att_019;
        att1    = data_ref.state[1].att1 - 20.0f;
        att2    = data_ref.state[1].att2 + float(ref + 10);
        att3    = data_ref.state[1].att3;
        ad_offset = data_ref.state[1].ad_offset;
        temp = data_ref.state[1].temp;
    } else if (ref >= -30) {
        lna_att = data_ref.state[2].lna_att;
        att_019 = data_ref.state[2].att_019;
        att1    = data_ref.state[2].att1 + float(ref + 20);
        att2    = data_ref.state[2].att2;
        att3    = data_ref.state[2].att3;
        ad_offset = data_ref.state[2].ad_offset;
        temp = data_ref.state[2].temp;
    } else if (ref >= -40) {
        lna_att = data_ref.state[2].lna_att;
        att_019 = data_ref.state[2].att_019;
        att1    = data_ref.state[2].att1 - 10.0f;
        att2    = data_ref.state[2].att2 + float(ref + 30);
        att3    = data_ref.state[2].att3;
        ad_offset = data_ref.state[2].ad_offset;
        temp = data_ref.state[2].temp;
	}

    if (att1 < 0) {
        ad_offset += int32_t(g_0dBFS - dBc_to_ad(g_0dBFS,-att1));
        att1 = 0;
	}
    if (att2 < 0) {
        ad_offset += int32_t(g_0dBFS - dBc_to_ad(g_0dBFS,-att2));
        att2 = 0;
	}

    state->lna_att = lna_att;
    state->att_019 = att_019;
    state->att1 = att1;
    state->att2 = att2;
    state->att3 = att3;
    state->ad_offset = ad_offset;
    state->temp = temp;
}

void rx_ref_op_table_r1cd::get_config_table_r1f(int32_t ref,data_m_t data_ref,rx_state_m_t *state)
{
    /*
     * Ref   Att0   Att1   Att2
     *  30    -20	 -20    -10
     *  10    -20      0    -10 // state[0]
     *  10      0    -20    -10 // state[1]
     * -10      0	   0	-10
     * -10     14	 -15    -10 // state[2]
     * -25     14	   0    -10
     * -30     14      0     -5
     */

    short lna_att = 0;
    short att_019 = 0;
    float att1 = 0.0;
    float att2 = 0.0;
    int32_t ad_offset = 0;
    float temp = 0.0;

    if (ref > 10) {
        lna_att = data_ref.state[0].lna_att;
        att_019 = data_ref.state[0].att_019;
        att1    = data_ref.state[0].att1 + float(ref - 10);
        att2    = data_ref.state[0].att2;
        ad_offset = data_ref.state[0].ad_offset;
        temp = data_ref.state[0].temp;
    } else if (ref > -10) {
        lna_att = data_ref.state[1].lna_att;
        att_019 = data_ref.state[1].att_019;
        att1    = data_ref.state[1].att1 + float(ref - 10);
        att2    = data_ref.state[1].att2;
        ad_offset = data_ref.state[1].ad_offset;
        temp = data_ref.state[1].temp;
    } else if (ref >= -25) {
        lna_att = data_ref.state[2].lna_att;
        att_019 = data_ref.state[2].att_019;
        att1    = data_ref.state[2].att1 + float(ref + 10);
        att2    = data_ref.state[2].att2;
        ad_offset = data_ref.state[2].ad_offset;
        temp = data_ref.state[2].temp;
    } else if (ref >= -40) {
        lna_att = data_ref.state[2].lna_att;
        att_019 = data_ref.state[2].att_019;
        att1    = data_ref.state[2].att1 - 15.0f;
        att2    = data_ref.state[2].att2 + float(ref + 25);
        ad_offset = data_ref.state[2].ad_offset;
        temp = data_ref.state[2].temp;
    }

    if (att1 < 0) {
        ad_offset += int32_t(g_0dBFS - dBc_to_ad(g_0dBFS,-att1));
        att1 = 0;
    }
    if (att2 < 0) {
        ad_offset += int32_t(g_0dBFS - dBc_to_ad(g_0dBFS,-att2));
        att2 = 0;
    }

    state->lna_att = lna_att;
    state->att_019 = att_019;
    state->att1 = att1;
    state->att2 = att2;
    state->att3 = 0;
    state->ad_offset = ad_offset;
    state->temp = temp;
}

int32_t rx_ref_op_table_r1cd::get_ref_seg_idx(double ref)
{
    if (ref > 10.0) {
		return 0;
    } else if (ref > -20.0) {
		return 1;
    } else {
		return 2;
    }
}

// idx means state[0]/[1]/[2]
void rx_ref_op_table_r1cd::guess_base_r1c(int32_t idx,int32_t &ref,int32_t &lna_att,int32_t &att_019,double &att1,double &att2,double &att3)
{
    att3 = 10.0;
    att2 = 15.0;

    if (idx == 0) {
        ref = 10;
        lna_att = r1c::RX_ATT;
        att_019 = r1c::RX_ATT_19;
        att1 = 5.0;
    } else if (idx == 1) {
        ref = 10;
        lna_att = r1c::RX_ATT;
        att_019 = r1c::RX_ATT_0;
        att1 = 25.0;
    } else if (idx == 2) {
        ref = -20;
        lna_att = r1c::RX_LNA;
        att_019 = r1c::RX_ATT_19;
        att1 = 10.0;
	}
}

void rx_ref_op_table_r1cd::guess_base(hw_ver_t ver,uint32_t idx,int32_t &ref,rx_state_m_t *state)
{
    rx_state_m_t s;

    if (is_rf_ver_between(ver,R1C,R1E)) {
        s.att3 = 10.0;
        s.att2 = 15.0;

        switch (idx) {
        case 0 : {ref = 10;  s.lna_att = r1c::RX_ATT; s.att_019 = r1c::RX_ATT_19; s.att1 = 5.0;  return;}
        case 1 : {ref = 10;  s.lna_att = r1c::RX_ATT; s.att_019 = r1c::RX_ATT_0;  s.att1 = 25.0; return;}
        case 2 : {ref = -20; s.lna_att = r1c::RX_LNA; s.att_019 = r1c::RX_ATT_19; s.att1 = 10.0; return;}
        }
    }

    if (is_rf_ver_after(ver,R1F)) {
        s.att2 = 13.0;

        switch (idx) {
        case 0 : {ref = 10;  s.lna_att = r1c::RX_ATT; s.att_019 = r1c::RX_ATT_19; s.att1 = 0.0;  return;}
        case 1 : {ref = 10;  s.lna_att = r1c::RX_ATT; s.att_019 = r1c::RX_ATT_0;  s.att1 = 20.0; return;}
        case 2 : {ref = -10; s.lna_att = r1c::RX_LNA; s.att_019 = r1c::RX_ATT_19; s.att1 = 15.0; return;}
        }
    }

    if (state != nullptr) {
        *state = s;
    }
}

void rx_ref_op_table_r1cd::guess_base_r1f(int32_t idx,int32_t &ref,int32_t &lna_att,int32_t &att_019,double &att1,double &att2)
{
    att2 = 13.0;

    if (idx == 0) {
        ref = 10;
        lna_att = r1c::RX_ATT;
        att_019 = r1c::RX_ATT_19;
        att1 = 0.0;
    } else if (idx == 1) {
        ref = 10;
        lna_att = r1c::RX_ATT;
        att_019 = r1c::RX_ATT_0;
        att1 = 20.0;
    } else if (idx == 2) {
        ref = -10;
        lna_att = r1c::RX_LNA;
        att_019 = r1c::RX_ATT_19;
        att1 = 15.0;
    }
}

void rx_ref_op_table_r1cd::get_base(uint64_t freq,data_m_t *data)
{
    freq = freq < rx_freq_star ? rx_freq_star : freq;
    freq = freq > rx_freq_stop ? rx_freq_stop : freq;

    uint64_t freq_l = 0;
    uint64_t freq_r = 0;
    discretept(freq,r1c::rx_freq_step_called,freq_l,freq_r);

    uint32_t freq_idx_l = SERIE_INDEX(freq_l,rx_freq_star,r1c::rx_freq_step_called);
    uint32_t freq_idx_r = freq >= rx_freq_stop ? freq_idx_l : SERIE_INDEX(freq_r,rx_freq_star,r1c::rx_freq_step_called);

    data_m_t data_m_l = m_data_m[freq_idx_l];
    data_m_t data_m_r = m_data_m[freq_idx_r];

    for (uint32_t i = 0;i < ARRAY_SIZE(data_m_l.state);i ++) {
        if (data_m_l.state[i].lna_att == data_m_r.state[i].lna_att &&
            data_m_l.state[i].att_019 == data_m_r.state[i].att_019 &&
            data_m_l.state[i].att1 == data_m_r.state[i].att1 &&
            data_m_l.state[i].att2 == data_m_r.state[i].att2 &&
            data_m_l.state[i].att3 == data_m_r.state[i].att3 &&
            data_m_l.state[i].temp == data_m_r.state[i].temp) {
                linear(freq_l,data_m_l.state[i].ad_offset,
                       freq_r,data_m_r.state[i].ad_offset,
                       freq,data->state[i].ad_offset);
        } else {
            data->state[i].ad_offset = data_m_l.state[i].ad_offset;
        }

        data->state[i].lna_att = data_m_l.state[i].lna_att;
        data->state[i].att_019 = data_m_l.state[i].att_019;
        data->state[i].att1 = data_m_l.state[i].att1;
        data->state[i].att2 = data_m_l.state[i].att2;
        data->state[i].att3 = data_m_l.state[i].att3;
        data->state[i].temp = data_m_l.state[i].temp;
	}
}

void rx_ref_op_table_r1cd::get_base(uint64_t freq,data_f_t *data)
{
    data_m_t data_m;
    get_base(freq,&data_m);

    uint64_t freq_l = 0;
    uint64_t freq_r = 0;
    discretept(freq,r1c::rx_freq_step_called,freq_l,freq_r);

    *data = m_data_f[SERIE_INDEX(freq_l,rx_freq_star,rx_freq_step_infile)];
    data->freq = freq;
    for (uint32_t i = 0;i < ARRAY_SIZE(data_m.state);i ++) {
        data->state[i].ad_offset = data_m.state[i].ad_offset;
    }
}

void rx_ref_op_table_r1cd::get(hw_ver_t ver,uint64_t freq,double ref,rx_state_m_t *state)
{
    data_m_t data_ref;

    ref = ref > R1C_RX_REF_OP_STAR ? R1C_RX_REF_OP_STAR : ref;
    double ref_l = 0.0;
    double ref_r = 0.0;

    discretept(ref,R1C_RX_REF_STEP,ref_l,ref_r);
    get_base(freq,&data_ref);
    if (ver >= R1C && ver <= R1E) {
        rx_ref_op_table_r1cd::get_config_table_r1c(int32_t(ref_l),data_ref,state);
    } else if (ver >= R1F) {
        rx_ref_op_table_r1cd::get_config_table_r1f(int32_t(ref_l),data_ref,state);
    }
    state->ad_offset -= int32_t(dBc_to_ad(g_0dBFS,ref_l - ref)) - g_0dBFS;
}

void rx_ref_op_table_r1cd::get(hw_ver_t ver,uint64_t freq,double ref,rx_state_f_t *state,tm *time)
{
    rx_state_m_t state_m;
    data_f_t data;
    get(ver,freq,ref,&state_m);
    get_base(freq,&data);
    state->lna_att = int16_t(state_m.lna_att);
    state->att_019 = int16_t(state_m.att_019);
    state->att1 = state_m.att1;
    state->att2 = state_m.att2;
    state->att3 = state_m.att3;
    state->ad_offset = state_m.ad_offset;

    if (ver >= R1C && ver <= R1E) {
        if (ref > 10.0) {
            for (uint32_t i = 0;i < ARRAY_SIZE(state->temp);i ++) {
                state->temp[i] = data.state[0].temp[i];
            }
        } else if (ref > -20.0) {
            for (uint32_t i = 0;i < ARRAY_SIZE(state->temp);i ++) {
                state->temp[i] = data.state[1].temp[i];
            }
        } else {
            for (uint32_t i = 0;i < ARRAY_SIZE(state->temp);i ++) {
                state->temp[i] = data.state[2].temp[i];
            }
        }
    } else if (ver >= R1F) {
        if (ref > 10.0) {
            for (uint32_t i = 0;i < ARRAY_SIZE(state->temp);i ++) {
                state->temp[i] = data.state[0].temp[i];
            }
        } else if (ref > -10.0) {
            for (uint32_t i = 0;i < ARRAY_SIZE(state->temp);i ++) {
                state->temp[i] = data.state[1].temp[i];
            }
        } else {
            for (uint32_t i = 0;i < ARRAY_SIZE(state->temp);i ++) {
                state->temp[i] = data.state[2].temp[i];
            }
        }
    }

    if (time != nullptr) {
        *time = data.time;
    }
}

void rx_ref_op_table_r1cd::save_as(const char *path)
{
}

// IO mode
IMPLEMENT_RX_CAL_ITEM_TABLE(rx_ref_io_table_r1cd)

void rx_ref_io_table_r1cd::map2mem()
{
    if (m_data_f == nullptr) {
		return;
    }

    uint32_t idx_f = 0;
    uint32_t idx_m = 0;
    uint64_t freq = 0;

    for (idx_m = 0;idx_m < r1c::rx_freqs_called;idx_m ++) {
        freq = rx_freq_star + idx_m * r1c::rx_freq_step_called;
        idx_f = SERIE_INDEX(freq,rx_freq_star,rx_freq_step_infile);
        for (uint32_t i = 0;i < ARRAY_SIZE(m_data_m[idx_m].state);i ++) {
            m_data_m[idx_m].state[i].att1 = m_data_f[idx_f].state[i].att1;
            m_data_m[idx_m].state[i].att2 = m_data_f[idx_f].state[i].att2;
            m_data_m[idx_m].state[i].att3 = m_data_f[idx_f].state[i].att3;
            m_data_m[idx_m].state[i].temp = float(m_data_f[idx_f].state[i].temp[0]);
            m_data_m[idx_m].state[i].ad_offset = int32_t(m_data_f[idx_f].state[i].ad_offset);
            m_data_m[idx_m].state[i].lna_att = short(m_data_f[idx_f].state[i].lna_att);
            m_data_m[idx_m].state[i].att_019 = short(m_data_f[idx_f].state[i].att_019);
		}
	}
}

void rx_ref_io_table_r1cd::get_config_table_r1c(int32_t ref,data_m_t data_ref,rx_state_m_t *state)
{
    /*
     * ref   att0   att1   att2   att3
     *  30    -20	 -20	-13	   -10
     *  20	  -20	 -10    -13	   -10
     *  10	  -20	  -0    -13	   -10 // state[0]
     *  10	    0	 -20    -13	   -10 // state[1]
     *   0	    0	 -10    -13	   -10
     * -10		0	   0    -13	   -10
     * -20      0	   0     -3	   -10
     * -20     14	  -4    -13	   -10 // state[2]
     * -24     14	   0    -13	   -10
     * -30     14	   0     -7	   -10
     */

    short lna_att = 0;
    short att_019 = 0;
    float att1 = 0.0;
    float att2 = 0.0;
    float att3 = 0.0;
    int32_t ad_offset = 0;
    float temp = 0.0;

    if (ref > 10) {
        lna_att = data_ref.state[0].lna_att;
        att_019 = data_ref.state[0].att_019;
        att1    = data_ref.state[0].att1 + float(ref - 10);
        att2    = data_ref.state[0].att2;
        att3    = data_ref.state[0].att3;
        ad_offset = data_ref.state[0].ad_offset;
        temp = data_ref.state[0].temp;
    } else if (ref >= -10) {
        lna_att = data_ref.state[1].lna_att;
        att_019 = data_ref.state[1].att_019;
        att1    = data_ref.state[1].att1 + float(ref - 10);
        att2    = data_ref.state[1].att2;
        att3    = data_ref.state[1].att3;
        ad_offset = data_ref.state[1].ad_offset;
        temp = data_ref.state[1].temp;
    } else if (ref > -20) {
        lna_att = data_ref.state[1].lna_att;
        att_019 = data_ref.state[1].att_019;
        att1    = data_ref.state[1].att1 - 20.0f;
        att2    = data_ref.state[1].att2 + float(ref + 10);
        att3    = data_ref.state[1].att3;
        ad_offset = data_ref.state[1].ad_offset;
        temp = data_ref.state[1].temp;
    } else if (ref >= -24) {
        lna_att = data_ref.state[2].lna_att;
        att_019 = data_ref.state[2].att_019;
        att1    = data_ref.state[2].att1 + float(ref + 20);
        att2    = data_ref.state[2].att2;
        att3    = data_ref.state[2].att3;
        ad_offset = data_ref.state[2].ad_offset;
        temp = data_ref.state[2].temp;
    } else if (ref >= -30) {
        lna_att = data_ref.state[2].lna_att;
        att_019 = data_ref.state[2].att_019;
        att1    = data_ref.state[2].att1 - 4.0f;
        att2    = data_ref.state[2].att2 + float(ref + 24);
        att3    = data_ref.state[2].att3;
        ad_offset = data_ref.state[2].ad_offset;
        temp = data_ref.state[2].temp;
	}

    if (att1 < 0) {
        ad_offset += int32_t(g_0dBFS - dBc_to_ad(g_0dBFS,-att1));
        att1 = 0;
	}
    if (att2 < 0) {
        ad_offset += int32_t(g_0dBFS - dBc_to_ad(g_0dBFS,-att2));
        att2 = 0;
	}

    state->lna_att = lna_att;
    state->att_019 = att_019;
    state->att1 = att1;
    state->att2 = att2;
    state->att3 = att3;
    state->ad_offset = ad_offset;
    state->temp = temp;
}

void rx_ref_io_table_r1cd::get_config_table_r1f(int32_t ref,data_m_t data_ref,rx_state_m_t *state)
{
    //
    // Ref   Att0   Att1   Att2
    //  30      0    -30    -13
    //  10      0    -10    -13 // state[0]
    //  10      0    -10    -13 // state[1] (=state[0])
    //   0      0      0    -13
    //   0     14    -15    -13 // state[2]
    // -15     14      0    -13
    // -20     14      0     -8
    //

    short lna_att = 0;
    short att_019 = 0;
    float att1 = 0.0;
    float att2 = 0.0;
    int32_t ad_offset = 0;
    float temp = 0.0;

    if (ref > 10) {
        lna_att = data_ref.state[0].lna_att;
        att_019 = data_ref.state[0].att_019;
        att1    = data_ref.state[0].att1 + float(ref - 10);
        att2    = data_ref.state[0].att2;
        ad_offset = data_ref.state[0].ad_offset;
        temp = data_ref.state[0].temp;
    } else if (ref > 0) {
        lna_att = data_ref.state[1].lna_att;
        att_019 = data_ref.state[1].att_019;
        att1    = data_ref.state[1].att1 + float(ref - 10);
        att2    = data_ref.state[1].att2;
        ad_offset = data_ref.state[1].ad_offset;
        temp = data_ref.state[1].temp;
    } else if (ref >= -15) {
        lna_att = data_ref.state[2].lna_att;
        att_019 = data_ref.state[2].att_019;
        att1    = data_ref.state[2].att1 + float(ref);
        att2    = data_ref.state[2].att2;
        ad_offset = data_ref.state[2].ad_offset;
        temp = data_ref.state[2].temp;
    } else if (ref >= - 30) {
        lna_att = data_ref.state[2].lna_att;
        att_019 = data_ref.state[2].att_019;
        att1    = data_ref.state[2].att1 - 15.0f;
        att2    = data_ref.state[2].att2 + float(ref + 15);
        temp    = data_ref.state[2].temp;
    }

    if (att1 < 0) {
        ad_offset += int32_t(g_0dBFS - dBc_to_ad(g_0dBFS,-att1));
        att1 = 0;
    }
    if (att2 < 0) {
        ad_offset += int32_t(g_0dBFS - dBc_to_ad(g_0dBFS,-att2));
        att2 = 0;
    }

    state->lna_att = lna_att;
    state->att_019 = att_019;
    state->att1 = att1;
    state->att2 = att2;
    state->att3 = 0;
    state->ad_offset = ad_offset;
    state->temp = temp;
}

void rx_ref_io_table_r1cd::guess_base_r1c(int32_t idx,int32_t &ref,int32_t &lna_att,int32_t &att_019,double &att1,double &att2,double &att3)
{
    att3 = 10.0;

    if (idx == 0) {
        ref = 10;
        lna_att = r1c::RX_ATT;
        att_019 = r1c::RX_ATT_19;
        att1 = 0.0;
        att2 = 13.0;
    } else if (idx == 1) {
        ref = 10;
        lna_att = r1c::RX_ATT;
        att_019 = r1c::RX_ATT_0;
        att1 = 20.0;
        att2 = 13.0;
    } else if (idx == 2) {
        ref = -20;
        lna_att = r1c::RX_LNA;
        att_019 = r1c::RX_ATT_19;
        att1 = 4.0;
        att2 = 13.0;
	}
}

void rx_ref_io_table_r1cd::guess_base(hw_ver_t ver,uint32_t idx,int32_t &ref,rx_state_m_t *state)
{
    rx_state_m_t s;

    if (is_rf_ver_between(ver,R1C,R1E)) {
        s.att3 = 10.0;

        switch (idx) {
        case 0 : {ref = 10;  s.lna_att = r1c::RX_ATT; s.att_019 = r1c::RX_ATT_19; s.att1 = 0.0;  s.att2 = 13.0; return;}
        case 1 : {ref = 10;  s.lna_att = r1c::RX_ATT; s.att_019 = r1c::RX_ATT_0;  s.att1 = 20.0; s.att2 = 13.0; return;}
        case 2 : {ref = -20; s.lna_att = r1c::RX_LNA; s.att_019 = r1c::RX_ATT_19; s.att1 = 4.0;  s.att2 = 13.0; return;}
        }
    }

    if (is_rf_ver_after(ver,R1F)) {
        s.att2 = 18.0;

        switch (idx) {
        case 0 :
        case 1 : {ref = 10; s.lna_att = r1c::RX_ATT; s.att_019 = r1c::RX_ATT_0;  s.att1 = 10.0; return;}
        case 2 : {ref = 0;  s.lna_att = r1c::RX_LNA; s.att_019 = r1c::RX_ATT_19; s.att1 = 15.0; return;}
        }
    }

    if (state != nullptr) {
        *state = s;
    }
}

void rx_ref_io_table_r1cd::guess_base_r1f(int32_t idx,int32_t &ref,int32_t &lna_att,int32_t &att_019,double &att1,double &att2)
{
    att2 = 18.0;

    if (idx == 0 || idx == 1) {
        ref = 10;
        lna_att = r1c::RX_ATT;
        att_019 = r1c::RX_ATT_0;
        att1 = 10.0;
    } else if (idx == 2) {
        ref = 0;
        lna_att = r1c::RX_LNA;
        att_019 = r1c::RX_ATT_19;
        att1 = 15.0;
    }
}

int32_t rx_ref_io_table_r1cd::get_ref_seg_idx(double ref)
{
    if (ref > 10.0) {
		return 0;
    } else if (ref > -20.0) {
		return 1;
    } else {
		return 2;
    }
}

void rx_ref_io_table_r1cd::get_base(uint64_t freq,data_m_t *data)
{
    freq = freq < rx_freq_star ? rx_freq_star : freq;
    freq = freq > rx_freq_stop ? rx_freq_stop : freq;

    uint64_t freq_l = 0,freq_r = 0;
    discretept(freq,r1c::rx_freq_step_called,freq_l,freq_r);

    uint32_t freq_idx_l = SERIE_INDEX(freq_l,rx_freq_star,r1c::rx_freq_step_called);
    uint32_t freq_idx_r = freq >= rx_freq_stop ? freq_idx_l : SERIE_INDEX(freq_r,rx_freq_star,r1c::rx_freq_step_called);

    data_m_t data_m_l = m_data_m[freq_idx_l];
    data_m_t data_m_r = m_data_m[freq_idx_r];

    for (uint32_t i = 0;i < ARRAY_SIZE(data_m_l.state);i ++) {
        if (data_m_l.state[i].lna_att == data_m_r.state[i].lna_att &&
            data_m_l.state[i].att_019 == data_m_r.state[i].att_019 &&
            data_m_l.state[i].att1 == data_m_r.state[i].att1 &&
            data_m_l.state[i].att2 == data_m_r.state[i].att2 &&
            data_m_l.state[i].att3 == data_m_r.state[i].att3 &&
            data_m_l.state[i].temp == data_m_r.state[i].temp) {
                linear(freq_l,data_m_l.state[i].ad_offset,
                       freq_r,data_m_r.state[i].ad_offset,
                       freq,data->state[i].ad_offset);
        } else {
            data->state[i].ad_offset = data_m_l.state[i].ad_offset;
        }

        data->state[i].lna_att = data_m_l.state[i].lna_att;
        data->state[i].att_019 = data_m_l.state[i].att_019;
        data->state[i].att1 = data_m_l.state[i].att1;
        data->state[i].att2 = data_m_l.state[i].att2;
        data->state[i].att3 = data_m_l.state[i].att3;
        data->state[i].temp = data_m_l.state[i].temp;
	}
}

void rx_ref_io_table_r1cd::get_base(uint64_t freq,data_f_t *data)
{
    data_m_t data_m;
    get_base(freq,&data_m);

    uint64_t freq_l = 0,freq_r = 0;
    discretept(freq,r1c::rx_freq_step_called,freq_l,freq_r);

    *data = m_data_f[SERIE_INDEX(freq_l,rx_freq_star,rx_freq_step_infile)];
    data->freq = freq;
    for (uint32_t i = 0;i < ARRAY_SIZE(data_m.state);i ++) {
        data->state[i].ad_offset = data_m.state[i].ad_offset;
    }
}

void rx_ref_io_table_r1cd::get(hw_ver_t ver,uint64_t freq,double ref,rx_state_m_t *state)
{
    data_m_t data_ref;

    ref = ref > R1C_RX_REF_IO_STAR ? R1C_RX_REF_IO_STAR : ref;
    double ref_l = 0.0,ref_r = 0.0;

    discretept(ref,R1C_RX_REF_STEP,ref_l,ref_r);
    get_base(freq,&data_ref);
    if (ver >= R1C && ver <= R1E) {
        rx_ref_io_table_r1cd::get_config_table_r1c(int32_t(ref_l),data_ref,state);
    } else if (ver >= R1F) {
        rx_ref_io_table_r1cd::get_config_table_r1f(int32_t(ref_l),data_ref,state);
    }
    state->ad_offset -= int32_t(dBc_to_ad(g_0dBFS,ref_l - ref)) - g_0dBFS;
}

void rx_ref_io_table_r1cd::get(hw_ver_t ver,uint64_t freq,double ref,rx_state_f_t *state,tm *time)
{
    rx_ref_op_table_r1cd::rx_state_m_t state_m;
    data_f_t data;
    get(ver,freq,ref,&state_m);
    get_base(freq,&data);
    state->lna_att = int16_t(state_m.lna_att);
    state->att_019 = int16_t(state_m.att_019);
    state->att1 = state_m.att1;
    state->att2 = state_m.att2;
    state->att3 = state_m.att3;
    state->ad_offset = state_m.ad_offset;

    if (ver >= R1C && ver <= R1E) {
        if (ref > 10.0) {
            for (uint32_t i = 0;i < ARRAY_SIZE(state->temp);i ++) {
                state->temp[i] = data.state[0].temp[i];
            }
        } else if (ref > -20.0) {
            for (uint32_t i = 0;i < ARRAY_SIZE(state->temp);i ++) {
                state->temp[i] = data.state[1].temp[i];
            }
        } else {
            for (uint32_t i = 0;i < ARRAY_SIZE(state->temp);i ++) {
                state->temp[i] = data.state[2].temp[i];
            }
        }
    } else if (ver >= R1F) {
        if (ref > 0.0) {
            for (uint32_t i = 0;i < ARRAY_SIZE(state->temp);i ++) {
                state->temp[i] = data.state[0].temp[i];
            }
        } else {
            for (uint32_t i = 0;i < ARRAY_SIZE(state->temp);i ++) {
                state->temp[i] = data.state[1].temp[i];
            }
        }
    }

    if (time != nullptr) {
        *time = data.time;
    }
}

void rx_ref_io_table_r1cd::save_as(const char *path)
{
}
