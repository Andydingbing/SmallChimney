#include "tx_pwr_table.h"
#include "algo_math.hpp"

using namespace ns_sp1401;

void tx_pwr_op_table_r1c::get_config_table_r1c(int32_t pwr,data_m_t data_base,data_m_t *data)
{
	//		DGain	Att0	Att1	Att2	Att3	//	dBm

	// 		{-5,	 -5,	 -5,	  0,	 -1}	//	10
	// 		{-5,	 -5,	-10,	  0,	 -1}	//	5
	// 		{-5,	 -5,	-15,	  0,	 -1}	//	0
	////////{-5,	 -5,	-20,	  0,	 -1}//////	-5		//

	// 		{-5,	 -5,	-20,	 -5,	 -1}	//	-10
	// 		{-5,	 -5,	-20,	-10,	 -1}	//	-15
	// 		{-5,	 -5,	-20,	-15,	 -1}	//	-20		//

	// 		{-5,	 -5,	-20,	-15,	 -6}	//	-25
	// 		{-5,	 -5,	-20,	-15,	-11}	//	-30
	// 		{-5,	 -5,	-20,	-15,	-16}	//	-35		//

	// 		{-5,	 -5,	-20,	-20,	-16}	//	-40		//

	// 		{-5,	 -5,	-20,	-20,	-21}	//	-45	
	// 		{-5,	 -5,	-20,	-20,	-26}	//	-50		//

	// 		{-5,	 -5,	-20,	-25,	-26}	//	-55		//

	// 		{-5,	 -5,	-25,	-25,	-26}	//	-60		//

	// 		{-5,	-10,	-25,	-25,	-26}	//	-65		//

	// 		{-5,	-10,	-25,	-25,	-31}	//	-70		//

	// 		{-5,	-10,	-25,	-30,	-31}	//	-75		//

	// 		{-5,	-10,	-30,	-30,	-31}	//	-80		//
	// 		{-5,	-20,	-30,	-30,	-31}	//	-90
	// 		{-15,	-20,	-30,	-30,	-31}	//	-100
	// 		{-25,	-20,	-30,	-30,	-31}	//	-110
	// 		{-35,	-20,	-30,	-30,	-31}	//	-120

    double att0 = data_base.att0 / 2.0;
    double att1 = data_base.att1 / 2.0;
    double att2 = data_base.att2 / 2.0;
    double att3 = data_base.att3 / 2.0;
    double d_gain = double(data_base.d_gain);

    if (pwr >= r1c::tx_base_pwr_op) {
        att1 -= pwr - r1c::tx_base_pwr_op;
        if (att1 < 0) {
            d_gain -= att1;
            att1 = 0;
		}
    } else if (pwr >= r1c::tx_base_pwr_op - 15.0) {
        att2 = -5.0 - pwr;
    } else if (pwr >= r1c::tx_base_pwr_op - 30.0) {
        att2 += 15.0;
        att3 = -19.0 - pwr;
    } else if (pwr >= r1c::tx_base_pwr_op - 35.0) {
        att2 = -20 - pwr;
        att3 += 15.0;
    } else if (pwr >= r1c::tx_base_pwr_op - 45.0) {
        att2 += 20.0;
        att3 = -24.0 - pwr;
    } else if (pwr >= r1c::tx_base_pwr_op - 50.0) {
        att2 = -30.0 - pwr;
        att3 += 25.0;
    } else if (pwr >= r1c::tx_base_pwr_op - 55.0) {
        att1 += -55.0 - pwr;
        att2 += 25.0;
        att3 += 25.0;
    } else if (pwr >= r1c::tx_base_pwr_op - 60.0) {
        att0 += -60.0 - pwr;
        att1 += 5.0;
        att2 += 25.0;
        att3 += 25.0;
    } else if (pwr >= r1c::tx_base_pwr_op - 65.0) {
        att0 += 5.0;
        att1 += 5.0;
        att2 += 25.0;
        att3 = -39.0 - pwr;
    } else if (pwr >= r1c::tx_base_pwr_op - 70.0) {
        att0 += 5.0;
        att1 += 5.0;
        att2 = -45.0 - pwr;
        att3 += 30.0;
    } else if (pwr >= r1c::tx_base_pwr_op - 75.0) {
        att0 += 5.0;
        att1 += -70.0 - pwr;
        att2 += 30.0;
        att3 += 30.0;
    } else if (pwr >= r1c::tx_base_pwr_op - 85.0) {
        att0 += -75.0 - pwr;
        att1 += 10.0;
        att2 += 30.0;
        att3 += 30.0;
    } else {
        att0 += 15.0;
        att1 += 10.0;
        att2 += 30.0;
        att3 += 30.0;
        d_gain -= r1c::tx_base_pwr_op - 85.0 - pwr;
    }

    if (att0 > 31.5) {
        d_gain -= (att0 - 31.5);
        att0 = 31.5;
    }

    data->att0 = short(att0 * 2);
    data->att1 = short(att1 * 2);
    data->att2 = short(att2 * 2);
    data->att3 = short(att3 * 2);
    data->d_gain = float(d_gain);
    data->temp_5 = data_base.temp_5;
}

void tx_pwr_op_table_r1c::get_config_table_r1f(int32_t pwr,data_m_t data_base,data_m_t *data)
{
    /*
     *  DGain	Att0	Att1	Att2	//	dBm
     *
     * {   0,	  0,	  0,	  0 }	//	10
     * { -10,	  0,	  0,	  0 }	//	0
     * { -10,	 -5,	  0,	  0 }	//	-5  <--- Base
     * { -10,	 -5,	 -5,	  0 }   //	-10
     * { -10,	 -5,	 -5,	-30 }	//	-40
     * { -10,	-30,	 -5,	-30 }	//	-65
     * { -10,	-30,    -30,	-30 }	//	-90
     */

//    double att0 = data_base.att0 / 2.0;
//    double att1 = data_base.att1 / 2.0;
    double att0 = data_base.att1 / 2.0;
    double att1 = data_base.att0 / 2.0;

    double att2 = data_base.att2 / 2.0;
    double d_gain = double(data_base.d_gain);

    if (pwr >= r1c::tx_base_pwr_op + 5.0) {
        double att0_copy = att0;
        att0 -= 5.0;
        if (att0 < 0) {
            d_gain -= att0;
            att0 = 0;
        }
        d_gain += (pwr - r1c::tx_base_pwr_op) - (att0_copy - att0);
    } else if (pwr >= r1c::tx_base_pwr_op) {
        att0 -= pwr - r1c::tx_base_pwr_op;
        if (att0 < 0) {
            d_gain -= att0;
            att0 = 0;
        }
    } else if (pwr >= r1c::tx_base_pwr_op - 5.0) {
        att1 = -5.0 - pwr;
    } else if (pwr >= r1c::tx_base_pwr_op - 35.0) {
        att1 += 5.0;
        att2 = -10.0 - pwr;
    } else if (pwr >= r1c::tx_base_pwr_op - 60.0) {
        att0 += -40.0 - pwr;
        att1 += 5.0;
        att2 += 30.0;
    } else if (pwr >= r1c::tx_base_pwr_op - 85.0) {
        att0 += 25.0;
        att1 += -60.0 - pwr;
        att2 += 30.0;
    } else {
        att0 += 25.0;
        att1 += 30.0;
        att2 += 30.0;
        d_gain -= r1c::tx_base_pwr_op - 85.0 - pwr;
    }

    if (att0 > 31.5) {
        d_gain -= (att0 - 31.5);
        att0 = 31.5;
    }

//    data->att0 = short(att0 * 2);
//    data->att1 = short(att1 * 2);
    data->att0 = short(att1 * 2);
    data->att1 = short(att0 * 2);

    data->att2 = short(att2 * 2);
    data->d_gain = float(d_gain);
    data->temp_5 = data_base.temp_5;
}

void tx_pwr_op_table_r1c::guess_base_r1c(double &att0,double &att1,double &att2,double &att3,double &d_gain)
{
    att0 = 5.0;
    att1 = 20.0;
    att2 = 0.0;
    att3 = 1.0;
    d_gain = -5.0;
}

void tx_pwr_op_table_r1c::guess_base_r1f(double &att0,double &att1,double &att2,double &d_gain)
{
//    att0 = 5.0;
//    att1 = 0.0;
    att0 = 0.0;
    att1 = 5.0;

    att2 = 0.0;
    d_gain = -10.0;
}

void tx_pwr_op_table_r1c::get_base(uint64_t freq,data_m_t *data)
{
    freq = freq < tx_freq_star ? tx_freq_star : freq;
    freq = freq > tx_freq_stop ? tx_freq_stop : freq;

    uint64_t freq_l = 0,freq_r = 0;
    discretept(freq,tx_freq_step_called,freq_l,freq_r);

    uint32_t freq_idx_l = SERIE_INDEX(freq_l,tx_freq_star,tx_freq_step_called);
    uint32_t freq_idx_r = SERIE_INDEX(freq_r,tx_freq_star,tx_freq_step_called);

    data_m_t data_l = m_data_m[freq_idx_l];
    data_m_t data_r = m_data_m[freq_idx_r];

    if (data_l.att0 == data_r.att0 && data_l.att1 == data_r.att1 &&
        data_l.att2 == data_r.att2 && data_l.att3 == data_r.att3 &&
        data_l.temp_5 - data_r.temp_5 == 0.0f) {
            linear(freq_l,data_l.d_gain,freq_r,data_r.d_gain,freq,data->d_gain);
    } else {
        data->d_gain = data_l.d_gain;
    }

    data->att0  = data_l.att0;
    data->att1  = data_l.att1;
    data->att2  = data_l.att2;
    data->att3  = data_l.att3;
    data->temp_5 = data_l.temp_5;
}

void tx_pwr_op_table_r1c::get_base(uint64_t freq,data_f_t *data)
{
    freq = freq < tx_freq_star ? tx_freq_star : freq;
    freq = freq > tx_freq_stop ? tx_freq_stop : freq;

    data_m_t data_m;
    get_base(freq,&data_m);

    uint64_t freq_l = 0,freq_r = 0;
    discretept(freq,tx_freq_step_called,freq_l,freq_r);

    *data = m_data_f[SERIE_INDEX(freq_l,tx_freq_star,tx_freq_step_infile)];
    data->freq = freq;
    data->att0 = double(data_m.att0 / 2.0);
    data->att1 = double(data_m.att1 / 2.0);
    data->att2 = double(data_m.att2 / 2.0);
    data->att3 = double(data_m.att3 / 2.0);
    data->d_gain = double(data_m.d_gain);
}

void tx_pwr_op_table_r1c::get(hw_ver_t ver,uint64_t freq,double pwr,data_m_t *data)
{
    data_m_t data_base;
    pwr = pwr > double(R1C_TX_ATT_OP_POWER_STAR) ? double(R1C_TX_ATT_OP_POWER_STAR) : pwr;
    double pwr_l = 0.0,pwr_r = 0.0;

    discretept(pwr,R1C_TX_ATT_STEP,pwr_l,pwr_r);
    get_base(freq,&data_base);

    if (ver == R1C || ver == R1D || ver == R1E) {
        tx_pwr_op_table_r1c::get_config_table_r1c(int32_t(pwr_l),data_base,data);
    } else if (ver == R1F) {
        tx_pwr_op_table_r1c::get_config_table_r1f(int32_t(pwr_l),data_base,data);
    }

    data->d_gain -= float(pwr_l - pwr);
}

void tx_pwr_op_table_r1c::get(hw_ver_t ver,uint64_t freq,double pwr,data_f_t *data)
{
    data_m_t data_m;
    get(ver,freq,pwr,&data_m);
    get_base(freq,data);
    data->att0 = double(data_m.att0 / 2.0);
    data->att1 = double(data_m.att1 / 2.0);
    data->att2 = double(data_m.att2 / 2.0);
    data->att3 = double(data_m.att3 / 2.0);
    data->d_gain = double(data_m.d_gain);
}



void tx_pwr_io_table_r1c::get_config_table_r1c(int32_t pwr,data_m_t data_base,data_m_t *data)
{
	//		DGain	Att0	Att1	Att2	Att3	//	dBm

	// 		{-5,	 -5,	 -3,	  0,	 -1}	//	5
	////////{-5,	 -5,	-20,	  0,	 -1}//////	-12		//
	// 		{-5,	 -5,	-20,	-15,	 -1}	//	-27		//
	// 		{-5,	 -5,	-20,	-15,	-16}	//	-42		//
	// 		{-5,	 -5,	-20,	-20,	-16}	//	-47		//
	// 		{-5,	 -5,	-20,	-20,	-26}	//	-57		//
	// 		{-5,	 -5,	-20,	-25,	-26}	//	-62		//
	//		{-5,	 -5,	-25,	-25,	-26}	//	-67		//
	// 		{-5,	-10,	-25,	-25,	-26}	//	-72		//
	// 		{-5,	-10,	-25,	-25,	-31}	//	-77		//
	// 		{-5,	-10,	-25,	-30,	-31}	//	-82		//
	//		{-5,	-10,	-30,	-30,	-31}	//	-87		//
	//		{-5,	-20,	-30,	-30,	-31}	//	-97		//
	//		{-28,	-20,	-30,	-30,	-31}	//	-120	//

    double att0 = data_base.att0 / 2.0;
    double att1 = data_base.att1 / 2.0;
    double att2 = data_base.att2 / 2.0;
    double att3 = data_base.att3 / 2.0;
    double d_gain = double(data_base.d_gain);

    if (pwr >= r1c::tx_base_pwr_io) {
        att1 += r1c::tx_base_pwr_io - pwr;
        if (att1 < 0) {
            d_gain -= att1;
            att1 = 0;
		}
    } else if (pwr >= r1c::tx_base_pwr_io - 15.0) {
        att2 += r1c::tx_base_pwr_io - pwr;
    } else if (pwr >= r1c::tx_base_pwr_io - 30.0) {
        att2 += 15.0;
        att3 += r1c::tx_base_pwr_io - pwr - 15.0;
    } else if (pwr >= r1c::tx_base_pwr_io - 35.0) {
        att2 += r1c::tx_base_pwr_io - pwr - 15.0;
        att3 += 15.0;
    } else if (pwr >= r1c::tx_base_pwr_io - 45.0) {
        att2 += 20.0;
        att3 += r1c::tx_base_pwr_io - pwr - 20.0;
    } else if (pwr >= r1c::tx_base_pwr_io - 50.0) {
        att3 += 25.0;
        att2 += r1c::tx_base_pwr_io - pwr - 25.0;
    } else if (pwr >= r1c::tx_base_pwr_io - 55.0) {
        att2 += 25.0;
        att3 += 25.0;
        att1 += r1c::tx_base_pwr_io - pwr - 50.0;
    } else if (pwr >= r1c::tx_base_pwr_io - 60.0) {
        att1 += 5.0;
        att2 += 25.0;
        att3 += 25.0;
        att0 += r1c::tx_base_pwr_io - pwr - 55.0;
    } else if (pwr >= r1c::tx_base_pwr_io - 65.0) {
        att0 += 5.0;
        att1 += 5.0;
        att2 += 25.0;
        att3 += r1c::tx_base_pwr_io - pwr - 35.0;
    } else if (pwr >= r1c::tx_base_pwr_io - 70.0) {
        att0 += 5.0;
        att1 += 5.0;
        att3 += 30.0;
        att2 += r1c::tx_base_pwr_io - pwr - 40.0;
    } else if (pwr >= r1c::tx_base_pwr_io - 75.0) {
        att0 += 5.0;
        att2 += 30.0;
        att3 += 30.0;
        att1 += r1c::tx_base_pwr_io - pwr - 65.0;
    } else if (pwr >= r1c::tx_base_pwr_io - 85.0) {
        att1 += 10.0;
        att2 += 30.0;
        att3 += 30.0;
        att0 += r1c::tx_base_pwr_io - pwr - 70.0;
    } else {
        att0 += 15.0;
        att1 += 10.0;
        att2 += 30.0;
        att3 += 30.0;
        d_gain -= r1c::tx_base_pwr_io - pwr - 85.0;
	}

    if (att0 > 31.5) {
        d_gain -= (att0 - 31.5);
        att0 = 31.5;
    }

    data->att0 = short(att0 * 2);
    data->att1 = short(att1 * 2);
    data->att2 = short(att2 * 2);
    data->att3 = short(att3 * 2);
    data->d_gain = float(d_gain);
    data->temp_5 = data_base.temp_5;
}

void tx_pwr_io_table_r1c::get_config_table_r1f(int32_t pwr,data_m_t data_base,data_m_t *data)
{
    /*
     *  DGain	Att0	Att1	Att2	//	dBm
     *
     * {   0,	  0,	  0,	  0 }	//	1
     * { -10,	  0,	  0,	  0 }	//	-9
     * { -10,    -3,      0,      0 }   //  -12  <--- Base
     * { -10,	 -5,	  0,	  0 }	//	-14
     * { -10,	 -5,	 -5,	  0 }   //	-19
     * { -10,	 -5,	 -5,	-30 }	//	-49
     * { -10,	-30,	 -5,	-30 }	//	-74
     * { -10,	-30,    -30,	-30 }	//	-99
     */

//    double att0 = data_base.att0 / 2.0;
//    double att1 = data_base.att1 / 2.0;
    double att0 = data_base.att1 / 2.0;
    double att1 = data_base.att0 / 2.0;

    double att2 = data_base.att2 / 2.0;
    double d_gain = double(data_base.d_gain);

    if (pwr >= r1c::tx_base_pwr_io + 3.0) {
        double att0_copy = att0;
        att0 -= 3.0;
        if (att0 < 0) {
            d_gain -= att0;
            att0 = 0;
        }
        d_gain += (pwr - r1c::tx_base_pwr_io) - (att0_copy - att0);
    } else if (pwr >= r1c::tx_base_pwr_io) {
        att0 -= pwr - r1c::tx_base_pwr_io;
        if (att0 < 0) {
            d_gain -= att0;
            att0 = 0;
        }
    } else if (pwr >= r1c::tx_base_pwr_io - 2.0) {
        att0 += r1c::tx_base_pwr_io - pwr;
    } else if (pwr >= r1c::tx_base_pwr_io - 7.0) {
        att0 += 2.0;
        att1 += r1c::tx_base_pwr_io - pwr - 2.0;
    } else if (pwr >= r1c::tx_base_pwr_io - 37.0) {
        att0 += 2.0;
        att1 += 5.0;
        att2 += r1c::tx_base_pwr_io - pwr - 7.0;
    } else if (pwr >= r1c::tx_base_pwr_io - 62.0) {
        att0 += r1c::tx_base_pwr_io - pwr - 35.0;
        att1 += 5.0;
        att2 += 30.0;
    } else if (pwr >= r1c::tx_base_pwr_io - 87.0) {
        att0 += 27.0;
        att1 += r1c::tx_base_pwr_io - pwr - 57.0;
        att2 += 30.0;
    } else {
        att0 += 27.0;
        att1 += 30.0;
        att2 += 30.0;
        d_gain -= r1c::tx_base_pwr_io - pwr - 87.0;
    }

    if (att0 > 31.5) {
        d_gain -= (att0 - 31.5);
        att0 = 31.5;
    }

//    data->att0 = short(att0 * 2);
//    data->att1 = short(att1 * 2);
    data->att0 = short(att1 * 2);
    data->att1 = short(att0 * 2);

    data->att2 = short(att2 * 2);
    data->d_gain = float(d_gain);
    data->temp_5 = data_base.temp_5;
}

void tx_pwr_io_table_r1c::guess_base_r1c(double &att0,double &att1,double &att2,double &att3,double &d_gain)
{
    att0 = 5.0;
    att1 = 20.0;
    att2 = 0.0;
    att3 = 1.0;
    d_gain = -5.0;
}

void tx_pwr_io_table_r1c::guess_base_r1f(double &att0,double &att1,double &att2,double &d_gain)
{
//    att0 = 5.0;
//    att1 = 0.0;
    att0 = 0.0;
    att1 = 5.0;

    att2 = 0.0;
    d_gain = -10.0;
}

void tx_pwr_io_table_r1c::get_base(uint64_t freq,data_m_t *data)
{
    freq = freq < tx_freq_star ? tx_freq_star : freq;
    freq = freq > tx_freq_stop ? tx_freq_stop : freq;

    uint64_t freq_l = 0,freq_r = 0;
    discretept(freq,tx_freq_step_called,freq_l,freq_r);

    uint32_t freq_idx_l = SERIE_INDEX(freq_l,tx_freq_star,tx_freq_step_called);
    uint32_t freq_idx_r = SERIE_INDEX(freq_r,tx_freq_star,tx_freq_step_called);

    data_m_t data_l = m_data_m[freq_idx_l];
    data_m_t data_r = m_data_m[freq_idx_r];

    if (data_l.att0 == data_r.att0 && data_l.att1 == data_r.att1 &&
        data_l.att2 == data_r.att2 && data_l.att3 == data_r.att3 &&
        data_l.temp_5 - data_r.temp_5 == 0.0f) {
            linear(freq_l,data_l.d_gain,freq_r,data_r.d_gain,freq,data->d_gain);
    } else {
        data->d_gain = data_l.d_gain;
    }

    data->att0  = data_l.att0;
    data->att1  = data_l.att1;
    data->att2  = data_l.att2;
    data->att3  = data_l.att3;
    data->temp_5 = data_l.temp_5;
}

void tx_pwr_io_table_r1c::get_base(uint64_t freq,data_f_t *data)
{
    freq = freq < tx_freq_star ? tx_freq_star : freq;
    freq = freq > tx_freq_stop ? tx_freq_stop : freq;

    data_m_t data_m;
    get_base(freq,&data_m);

    uint64_t freq_l = 0,freq_r = 0;
    discretept(freq,tx_freq_step_called,freq_l,freq_r);

    *data = m_data_f[SERIE_INDEX(freq_l,tx_freq_star,tx_freq_step_infile)];
    data->freq = freq;
    data->att0 = double(data_m.att0 / 2.0);
    data->att1 = double(data_m.att1 / 2.0);
    data->att2 = double(data_m.att2 / 2.0);
    data->att3 = double(data_m.att3 / 2.0);
    data->d_gain = double(data_m.d_gain);
}

void tx_pwr_io_table_r1c::get(hw_ver_t ver,uint64_t freq,double pwr,data_m_t *data)
{
    data_m_t data_base;
    pwr = pwr > double(R1C_TX_ATT_IO_POWER_STAR) ? double(R1C_TX_ATT_IO_POWER_STAR) : pwr;
    double pwr_l = 0.0,pwr_r = 0.0;

    discretept(pwr,R1C_TX_ATT_STEP,pwr_l,pwr_r);
    get_base(freq,&data_base);

    if (ver == R1C || ver == R1D || ver == R1E) {
        tx_pwr_io_table_r1c::get_config_table_r1c(int32_t(pwr_l),data_base,data);
    } else if (ver == R1F) {
        tx_pwr_io_table_r1c::get_config_table_r1f(int32_t(pwr_l),data_base,data);
    }

    data->d_gain -= float(pwr_l - pwr);
}

void tx_pwr_io_table_r1c::get(hw_ver_t ver,uint64_t freq,double pwr,data_f_t *data)
{
    data_m_t data_m;
    get(ver,freq,pwr,&data_m);
    get_base(freq,data);
    data->att0 = double(data_m.att0 / 2.0);
    data->att1 = double(data_m.att1 / 2.0);
    data->att2 = double(data_m.att2 / 2.0);
    data->att3 = double(data_m.att3 / 2.0);
    data->d_gain = double(data_m.d_gain);
}
