#include "sp1401_r1a.h"
#include "../chip/reg_def_sp9500.h"
#include <math.h>
#include <stdlib.h>
#include "sleep_common.h"
#include "algo_chip.hpp"

using namespace std;
using namespace ns_starpoint;
using namespace ns_sp9500;
using namespace ns_sp1401;
using namespace ns_sp1401::r1a;

sp1401_r1a::sp1401_r1a(uint32_t rf_idx,uint32_t rfu_idx)
{

}

int32_t sp1401_r1a::open_board()
{
    INT_CHECK(_cal_file->open());
    INT_CHECK(set_pwr_en(true));
    INT_CHECK(set_io_mode(io_mode_t::OUTPUT));
    INT_CHECK(init_lo());
    INT_CHECK(set_tx_modulator_en(true));
    INT_CHECK(set_tx_band_sw(BAND_HIGH));
    INT_CHECK(set_tx_att(10,20,20));
    INT_CHECK(set_tx_pa_att_sw(TX_ATT));
    INT_CHECK(set_rx_lna_att_sw(RX_ATT));
    INT_CHECK(set_rx_att(20.0,20));
    INT_CHECK(set_rx_if_filter_sw(_100MHz));
    INT_CHECK(set_rx_lmh5401N(-2.5));        // LT2666 DAC0
    INT_CHECK(set_rx_lmh5401P(-2.5));        // LT2666 DAC1
    INT_CHECK(set_tx_lo2_xb1007(-2.5));      // LT2666 DAC2
    INT_CHECK(set_ltc2666(3,-2.5));          // LT2666 DAC3
    INT_CHECK(set_rx_if1_hmc694_vgg1(-0.8)); // LT2666 DAC4
    INT_CHECK(set_rx_if1_hmc694_vga(-1.25)); // LT2666 DAC5
    INT_CHECK(set_rx_if1_hmc694_vgg2(-0.8)); // LT2666 DAC6
    INT_CHECK(set_ltc2666(7,-2.5));          // LT2666 DAC7
    return sp1401::open_board();
}

int32_t sp1401_r1a::init_lo()
{
    uint32_t reg[13];

    ns_adf5355::freq_formula_in param_in;
    ns_adf5355::freq_formula_out param_out;

    uint64_t freq_vco = 0;
    uint64_t freq_lo[4] = {0,0,0,0};
    tx_band_t band = BAND_LOW;

    ZERO_ARRAY(reg);

    reg[3] = 0x00000013;
    reg[4] = 0x30008394;
    reg[5] = 0x00800025;
    reg[7] = 0x12000067;
    reg[8] = 0x102d0428;
    reg[9] = 0x28323de9;
    reg[10] = 0x00c025ba;
    reg[11] = 0x0061300b;
    reg[12] = 0x4e20041c;

    DECLARE_R1A_SETLOREG_FUNCTION_MAP

    tx_freq2lo(2400000000,freq_lo[0],freq_lo[1],band);
    rx_freq2lo(2400000000,freq_lo[2],freq_lo[3]);
	
    for (int32_t lo_idx = int32_t(TX_LO_1);lo_idx <= int32_t(RX_LO_2);lo_idx ++) {
        freq_vco = freq_lo[lo_idx];
        if (freq_lo[lo_idx] > 6800000000) freq_vco = freq_lo[lo_idx] / 2;
        if (freq_lo[lo_idx] < 3400000000) freq_vco = freq_lo[lo_idx] * 2;

        param_in.freq_vco = freq_vco;

        ns_adf5355::freq_formula(param_in,param_out);
        reg[0] = ns_adf5355::reg0(param_out._int,0,1);
        reg[1] = ns_adf5355::reg1(param_out._frac1);
        reg[2] = ns_adf5355::reg2(param_out._mod2,param_out._frac2);
        reg[6] = freq_lo[lo_idx] < 3400000000 ? 0x3520a076 : 0x3500a076;
        for (int i = 0;i < 13;i ++) {
            (this->*set_r1a_lo_reg[lo_idx])(reg[12 - i]);
        }
	}
    set_lo(RX_LO_2,freq_lo[int32_t(RX_LO_2)]);
	return 0;
}

int32_t sp1401_r1a::close_board()
{
	return 0;
}

int32_t sp1401_r1a::set_led(bool tx,bool rx)
{
    RFU_K7_REG_DECLARE_2(0x1667,0x16e7);
    RFU_K7_REG_2(0x1667,0x16e7).tx_led = tx;
    RFU_K7_REG_2(0x1667,0x16e7).rx_led = rx;
    RFU_K7_W_2(0x1667,0x16e7);
	return 0;
}

bool sp1401_r1a::is_connected()
{
    double tx_temp = 0.0;
    INT_CHECKB(get_tx_temp(tx_temp));
    if (tx_temp <= 0) {
        double rx_temp = 0.0;
        INT_CHECKB(get_rx_temp(rx_temp));
        if (rx_temp <= 0) {
            Log.set_last_err(-2,"missing rfu%u--->rf%u",rfu_idx(),rf_idx());
			return false;
		}
	}
	return true;
}

int32_t sp1401_r1a::tx_freq2lo(uint64_t freq,uint64_t &lo1,uint64_t &lo2,tx_band_t &band)
{
    if (freq > 0 && freq <= 3100000000) {
        band = BAND_LOW;
        lo1 = 4100000000;
        lo2 = freq + 4100000000;
    } else {
        band = BAND_HIGH;
        lo1 = freq;
        lo2 = 6800000000;
	}
	return 0;
}

int32_t sp1401_r1a::tx_lo2freq(uint64_t lo1,uint64_t lo2,tx_band_t band,uint64_t &freq)
{
    freq = 0;
    if (BAND_LOW == band) {
        if (lo2 > lo1) {
            freq = lo2 - lo1;
        } else {
			return -1;
        }
    } else {
        freq = lo1;
    }
	return 0;
}

int32_t sp1401_r1a::rx_freq2lo(uint64_t freq,uint64_t &lo1,uint64_t &lo2)
{
    lo1 = freq + 7500000000;
    lo2 = 7224000000;
	return 0;
}

int32_t sp1401_r1a::rx_lo2freq(uint64_t lo1,uint64_t lo2,uint64_t &freq)
{
    freq = 0;
    if (lo1 > 7500000000) {
        freq = lo1 - 7500000000;
    } else {
		return -1;
    }
	return 0;
}

int32_t sp1401_r1a::io_mode2io_sw(
        io_mode_t mode,
        tx_io_sw_t &tx_sw,
        rx_io_sw_t &rx_sw,
        rx_io_sw1_t &rx_sw1,
        rx_io_sw2_t &rx_sw2,
        bool &tx_led,bool &rx_led)
{
    switch (mode) {
    case io_mode_t::IO : {	   // sw6---sw1 11 10 1 0
        tx_sw = TX_IO_ON;
        rx_sw = RX_IO_ON;
        rx_sw1 = RX_IO_ON_1;
        rx_sw2 = RX_IO_ON_2;
        tx_led = false;
        rx_led = true;
        return 0; }
    case io_mode_t::OUTPUT : { // sw6---sw1 10 11 0 0
        tx_sw = TX_ON;
        rx_sw = RX_ON;
        rx_sw1 = RX_IO_ON_1;
        rx_sw2 = RX_ON_2;
        tx_led = true;
        rx_led = true;
        return 0; }
    case io_mode_t::LOOP : {   // sw6---sw1 01 00 1 1
        tx_sw = TX_LOOP_ON;
        rx_sw = RX_LOOP_ON;
        rx_sw1 = RX_IO_OFF_1;
        rx_sw2 = RX_IO_ON_2;
        tx_led = false;
        rx_led = false;
        return 0; }
    default : break;
	}
	return 0;
}

int32_t sp1401_r1a::set_pwr_en(bool en)
{
    RFU_K7_REG_DECLARE_2(0x1660,0x16e0);
    RFU_K7_REG_2(0x1660,0x16e0).pow_en = (en ? 1 : 0);
    RFU_K7_W_2(0x1660,0x16e0);
	return 0;
}

int32_t sp1401_r1a::set_tx_modulator_en(bool en)
{
    RFU_K7_REG_DECLARE_2(0x1662,0x16e2);
    RFU_K7_REG_2(0x1662,0x16e2).mod_en = (en ? 0 : 1);
    RFU_K7_W_2(0x1662,0x16e2);
	return 0;
}

int32_t sp1401_r1a::set_io_mode(io_mode_t mode)
{
    RFU_K7_REG_DECLARE_2(0x1666,0x16e6);
    tx_io_sw_t tx_sw = TX_ON;
    rx_io_sw_t rx_sw = RX_ON;
    rx_io_sw1_t rx_sw1 = RX_IO_ON_1;
    rx_io_sw2_t rx_sw2 = RX_ON_2;
    bool tx_led = true;
    bool rx_led = true;

    io_mode2io_sw(mode,tx_sw,rx_sw,rx_sw1,rx_sw2,tx_led,rx_led);

    RFU_K7_REG_2(0x1666,0x16e6).tx_io_sw = unsigned(tx_sw);
    RFU_K7_REG_2(0x1666,0x16e6).rx_io_sw = unsigned(rx_sw);
    RFU_K7_REG_2(0x1666,0x16e6).rx_io_sw1 = unsigned(rx_sw1);
    RFU_K7_REG_2(0x1666,0x16e6).rx_io_sw2 = unsigned(rx_sw2);
    RFU_K7_W_2(0x1666,0x16e6);

    INT_CHECK(set_led(tx_led,rx_led));
	return 0;
}

int32_t sp1401_r1a::get_io_mode(io_mode_t &mode)
{
    RFU_K7_REG_DECLARE_2(0x1666,0x16e6);
    RFU_K7_R_2(0x1666,0x16e6);
    if (0x0000002c == RFU_K7_REG_DATA_2(0x1666,0x16e6)) {
        mode = io_mode_t::OUTPUT;
    }
    if (0x0000003a == RFU_K7_REG_DATA_2(0x1666,0x16e6)) {
        mode = io_mode_t::IO;
    }
    if (0x00000013 == RFU_K7_REG_DATA_2(0x1666,0x16e6)) {
        mode = io_mode_t::LOOP;
    }
	return 0;
}

int32_t sp1401_r1a::set_tx_band_sw(tx_band_t band)
{
    RFU_K7_REG_DECLARE_2(0x1663,0x16e3);
    RFU_K7_REG_2(0x1663,0x16e3).band_sw = (BAND_LOW == band ? 0 : 1);
    RFU_K7_REG_2(0x1663,0x16e3).q = (BAND_LOW == band ? 0 : 1);
    RFU_K7_W_2(0x1663,0x16e3);
	return 0;
}

int32_t sp1401_r1a::set_tx_pa_att_sw(tx_pa_att_t pa_att)
{
    RFU_K7_REG_DECLARE_2(0x1665,0x16e5);
    RFU_K7_REG_2(0x1665,0x16e5).att_pa_sw = (TX_PA == pa_att ? 0 : 1);
    RFU_K7_W_2(0x1665,0x16e5);
	return 0;
}

int32_t sp1401_r1a::set_tx_io_sw(tx_io_sw_t sw)
{
    RFU_K7_REG_DECLARE_2(0x1666,0x16e6);
    RFU_K7_R_2(0x1666,0x16e6);
    RFU_K7_REG_2(0x1666,0x16e6).tx_io_sw = unsigned(sw);
    RFU_K7_W_2(0x1666,0x16e6);
	return 0;
}

int32_t sp1401_r1a::set_lo(lo_t lo,uint64_t freq)
{
	uint32_t reg[13];
    for (int i = 0;i < 13;i ++) {
		reg[i] = 0;
    }

    uint64_t freq_vco = freq;
    if (freq > 6800000000) freq_vco = freq / 2;
    if (freq < 3400000000) freq_vco = freq * 2;

    ns_adf5355::freq_formula_in param_in;
    ns_adf5355::freq_formula_out param_out;

    param_in.freq_vco = freq_vco;

    ns_adf5355::freq_formula(param_in,param_out);
    reg[0] = ns_adf5355::reg0(param_out._int,0,1);
    reg[1] = ns_adf5355::reg1(param_out._frac1);
    reg[2] = ns_adf5355::reg2(param_out._mod2,param_out._frac2);
	reg[4] = 0x30008394;
    reg[6] = freq < 3400000000 ? 0x3520a076 : 0x3500a076;

    DECLARE_R1A_SETLOREG_FUNCTION_MAP

    (this->*set_r1a_lo_reg[lo])(reg[6]);
	reg[4] = reg[4] | (1 << 4);
    (this->*set_r1a_lo_reg[lo])(reg[4]);
    (this->*set_r1a_lo_reg[lo])(reg[2]);
    (this->*set_r1a_lo_reg[lo])(reg[1]);
    reg[0] = reg[0] & uint32_t(~(1 << 21));
    (this->*set_r1a_lo_reg[lo])(reg[0]);
    reg[4] = reg[4] & uint32_t(~(1 << 4));
    (this->*set_r1a_lo_reg[lo])(reg[4]);
    sleep_ms(100);
	reg[0] = reg[0] | (1 << 21);
    (this->*set_r1a_lo_reg[lo])(reg[0]);
	return 0;
}

int32_t sp1401_r1a::det_lo(lo_t lo,bool &lock)
{
    sleep_ms(100);

    DECLARE_R1A_DETLOLOCK_FUNCTION_MAP

    return (this->*det_r1a_lo_lock[lo])(lock);
}

int32_t sp1401_r1a::set_tx_lo1_reg(uint32_t data)
{	
    RFU_K7_REG_DECLARE_2(0x160d,0x168d);
    RFU_K7_REG_DECLARE_2(0x160e,0x168e);
    RFU_K7_REG_2(0x160e,0x168e).data = data;
    RFU_K7_WAIT_IDLE_2(0x160d,0x168d,0,1000);
    RFU_K7_W_2(0x160e,0x168e);
    RFU_K7_WAIT_IDLE_2(0x160d,0x168d,0,1000);
    RFU_K7_OP_2(0x160d,0x168d);
    RFU_K7_WAIT_IDLE_2(0x160d,0x168d,0,1000);
	return 0;
}

int32_t sp1401_r1a::set_tx_lo2_reg(uint32_t data)
{
    RFU_K7_REG_DECLARE_2(0x1609,0x1689);
    RFU_K7_REG_DECLARE_2(0x160a,0x168a);
    RFU_K7_REG_2(0x160a,0x168a).data = data;
    RFU_K7_WAIT_IDLE_2(0x1609,0x1689,0,1000);
    RFU_K7_W_2(0x160a,0x168a);
    RFU_K7_WAIT_IDLE_2(0x1609,0x1689,0,1000);
    RFU_K7_OP_2(0x1609,0x1689);
    RFU_K7_WAIT_IDLE_2(0x1609,0x1689,0,1000);
	return 0;
}

int32_t sp1401_r1a::det_tx_lo1_lock(bool &lock)
{
    RFU_K7_REG_DECLARE_2(0x160f,0x168f);
    RFU_K7_R_2(0x160f,0x168f);
    lock = (RFU_K7_REG_2(0x160f,0x168f).muxout == 0 ? false : true);
	return 0;
}

int32_t sp1401_r1a::det_tx_lo2_lock(bool &lock)
{
    RFU_K7_REG_DECLARE_2(0x160b,0x168b);
    RFU_K7_R_2(0x160b,0x168b);
    lock = (RFU_K7_REG_2(0x160b,0x168b).muxout == 0 ? false : true);
	return 0;
}

int32_t sp1401_r1a::set_tx_freq(uint64_t freq)
{
    uint64_t lo1 = 0,lo2 = 0;
    tx_band_t band = BAND_LOW;
    tx_freq2lo(freq,lo1,lo2,band);
    INT_CHECK(set_tx_band_sw(band));
    INT_CHECK(set_lo(TX_LO_1,lo1));
    INT_CHECK(set_lo(TX_LO_2,lo2));
	return 0;
}

int32_t sp1401_r1a::set_tx_att1(int32_t att)
{
    att = abs(att);
    att = int32_t(att / 10.0) * 10;
    unsigned c1 = (att == 30 || att == 10) ? 0 : 1;
    unsigned c2 = (att == 30 || att == 20) ? 0 : 1;

    RFU_K7_REG_DECLARE_2(0x1664,0x16e4);
    RFU_K7_R_2(0x1664,0x16e4);
    RFU_K7_REG_2(0x1664,0x16e4).tx_att1_c1 = c1;
    RFU_K7_REG_2(0x1664,0x16e4).tx_att1_c2 = c2;
    RFU_K7_W_2(0x1664,0x16e4);
	return 0;
}

int32_t sp1401_r1a::set_tx_att2(int32_t att)
{
    att = abs(att);
    att = int32_t(att / 10.0) * 10;
    unsigned c1 = (att == 30 || att == 10) ? 0 : 1;
    unsigned c2 = (att == 30 || att == 20) ? 0 : 1;

    RFU_K7_REG_DECLARE_2(0x1664,0x16e4);
    RFU_K7_R_2(0x1664,0x16e4);
    RFU_K7_REG_2(0x1664,0x16e4).tx_att2_c1 = c1;
    RFU_K7_REG_2(0x1664,0x16e4).tx_att2_c2 = c2;
    RFU_K7_W_2(0x1664,0x16e4);
	return 0;
}

int32_t sp1401_r1a::set_tx_att3(int32_t att)
{
    att = abs(att);
    att = int32_t(att / 10.0) * 10;
    unsigned c1 = (att == 30 || att == 10) ? 0 : 1;
    unsigned c2 = (att == 30 || att == 20) ? 0 : 1;

    RFU_K7_REG_DECLARE_2(0x1664,0x16e4);
    RFU_K7_R_2(0x1664,0x16e4);
    RFU_K7_REG_2(0x1664,0x16e4).tx_att3_c1 = c1;
    RFU_K7_REG_2(0x1664,0x16e4).tx_att3_c2 = c2;
    RFU_K7_W_2(0x1664,0x16e4);
	return 0;
}

int32_t sp1401_r1a::set_tx_att(int32_t att1,int32_t att2,int32_t att3)
{
    INT_CHECK(set_tx_att1(att1));
    INT_CHECK(set_tx_att2(att2));
    INT_CHECK(set_tx_att3(att3));
	return 0;
}

int32_t sp1401_r1a::set_tx_att(double att)
{
    int32_t att1 = att >= 30.0 ? 30 : (int32_t(att / 10.0)) * 10;
    int32_t att2 = (att - att1) >= 30 ? 30 : (int32_t((att - att1) / 10.0)) * 10;
    int32_t att3 = (att - att1 - att2) >= 30 ? 30 : (int32_t((att - att1 - att2) / 10.0)) * 10;
    INT_CHECK(set_tx_att(att1,att2,att3));
	return 0;
}

int32_t sp1401_r1a::sync_set_tx_gain(int32_t att1,int32_t att2,int32_t att3,double d_gain)
{
    RFU_K7_REG_DECLARE_2(0x0094,0x2094);
    RFU_K7_REG_DECLARE_2(0x00e6,0x20e6);

    RFU_K7_REG_2(0x00e6,0x20e6).op_0 = 0;
    RFU_K7_REG_2(0x00e6,0x20e6).op_1 = 0;
    RFU_K7_W_2(0x00e6,0x20e6);
    RFU_K7_REG_2(0x00e6,0x20e6).op_1 = 1;
    RFU_K7_W_2(0x00e6,0x20e6);

    INT_CHECK(set_tx_att(att1,att2,att3));
    RFU_K7_REG_2(0x0094,0x2094).i = uint16_t(d_gain * 512.0);
    RFU_K7_REG_2(0x0094,0x2094).q = uint16_t(d_gain * 512.0);
    RFU_K7_W_2(0x0094,0x2094);
	
    RFU_K7_REG_2(0x00e6,0x20e6).op_0 = 1;
    RFU_K7_W_2(0x00e6,0x20e6);

    RFU_K7_REG_2(0x00e6,0x20e6).op_0 = 0;
    RFU_K7_REG_2(0x00e6,0x20e6).op_1 = 0;
    RFU_K7_W_2(0x00e6,0x20e6);
	return 0;
}

int32_t sp1401_r1a::set_rx_lna_att_sw(rx_lna_att_t lna_att)
{
    RFU_K7_REG_DECLARE_2(0x166a,0x16ea);
    RFU_K7_REG_2(0x166a,0x16ea).lna_att_sw = (RX_ATT == lna_att ? 0 : 1);
    RFU_K7_W_2(0x166a,0x16ea);
	return 0;
}

int32_t sp1401_r1a::set_rx_freq(uint64_t freq)
{
    uint64_t lo1 = 0,lo2 = 0;
    rx_freq2lo(freq,lo1,lo2);
    INT_CHECK(set_lo(RX_LO_1,lo1));
	return 0;
}

int32_t sp1401_r1a::set_rx_io_sw1(rx_io_sw1_t sw)
{
    RFU_K7_REG_DECLARE_2(0x1666,0x16e6);
    RFU_K7_R_2(0x1666,0x16e6);
    RFU_K7_REG_2(0x1666,0x16e6).rx_io_sw1 = unsigned(sw);
    RFU_K7_W_2(0x1666,0x16e6);
	return 0;
}

int32_t sp1401_r1a::set_rx_io_sw2(rx_io_sw2_t sw)
{
    RFU_K7_REG_DECLARE_2(0x1666,0x16e6);
    RFU_K7_R_2(0x1666,0x16e6);
    RFU_K7_REG_2(0x1666,0x16e6).rx_io_sw2 = unsigned(sw);
    RFU_K7_W_2(0x1666,0x16e6);
	return 0;
}

int32_t sp1401_r1a::set_rx_io_sw(rx_io_sw_t sw)
{
    RFU_K7_REG_DECLARE_2(0x1666,0x16e6);
    RFU_K7_R_2(0x1666,0x16e6);
    RFU_K7_REG_2(0x1666,0x16e6).rx_io_sw = unsigned(sw);
    RFU_K7_W_2(0x1666,0x16e6);
	return 0;
}

int32_t sp1401_r1a::set_rx_lo1_reg(uint32_t data)
{
    RFU_K7_REG_DECLARE_2(0x1601,0x1681);
    RFU_K7_REG_DECLARE_2(0x1602,0x1682);
    RFU_K7_REG_2(0x1602,0x1682).data = data;
    RFU_K7_WAIT_IDLE_2(0x1601,0x1681,0,1000);
    RFU_K7_W_2(0x1602,0x1682);
    RFU_K7_WAIT_IDLE_2(0x1601,0x1681,0,1000);
    RFU_K7_OP_2(0x1601,0x1681);
    RFU_K7_WAIT_IDLE_2(0x1601,0x1681,0,1000);
	return 0;
}

int32_t sp1401_r1a::det_rx_lo1_lock(bool &lock)
{
    RFU_K7_REG_DECLARE_2(0x1603,0x1683);
    RFU_K7_R_2(0x1603,0x1683);
    lock = (RFU_K7_REG_2(0x1603,0x1683).muxout == 0 ? false : true);
	return 0;
}

int32_t sp1401_r1a::set_rx_lo2_reg(uint32_t data)
{
    RFU_K7_REG_DECLARE_2(0x1605,0x1685);
    RFU_K7_REG_DECLARE_2(0x1606,0x1686);
    RFU_K7_REG_2(0x1606,0x1686).data = data;
    RFU_K7_WAIT_IDLE_2(0x1605,0x1685,0,1000);
    RFU_K7_W_2(0x1606,0x1686);
    RFU_K7_WAIT_IDLE_2(0x1605,0x1685,0,1000);
    RFU_K7_OP_2(0x1605,0x1685);
    RFU_K7_WAIT_IDLE_2(0x1605,0x1685,0,1000);
	return 0;
}

int32_t sp1401_r1a::det_rx_lo2_lock(bool &lock)
{
    RFU_K7_REG_DECLARE_2(0x1607,0x1687);
    RFU_K7_R_2(0x1607,0x1687);
    lock = (RFU_K7_REG_2(0x1607,0x1687).muxout == 0 ? false : true);
	return 0;
}

int32_t sp1401_r1a::set_rx_att1(double att,double *actual)
{
    att = abs(att);
    att = (int32_t(att * 10) / 5) * 5 / 10.0;
    if (actual)
        *actual = att;
    int32_t d0_5 = 63 - int32_t(63 * att / 31.5);

    RFU_K7_REG_DECLARE_2(0x1641,0x16c1);
    RFU_K7_REG_DECLARE_2(0x1642,0x16c2);
    RFU_K7_REG_2(0x1642,0x16c2).data_word = unsigned(d0_5);
    RFU_K7_WAIT_IDLE_2(0x1641,0x16c1,0,1000);
    RFU_K7_W_2(0x1642,0x16c2);
    RFU_K7_WAIT_IDLE_2(0x1641,0x16c1,0,1000);
    RFU_K7_OP_2(0x1641,0x16c1);
    RFU_K7_WAIT_IDLE_2(0x1641,0x16c1,0,1000);
	return 0;
}

int32_t sp1401_r1a::set_rx_att2(int32_t att,int32_t *actual)
{
    att = abs(att);
    att = int32_t(att / 10.0) * 10;
    if (actual != nullptr) {
        *actual = att;
    }
    unsigned c1 = (att == 30 || att == 10) ? 0 : 1;
    unsigned c2 = (att == 30 || att == 20) ? 0 : 1;

    RFU_K7_REG_DECLARE_2(0x1669,0x16e9);
    RFU_K7_REG_2(0x1669,0x16e9).rx_att2_c1 = c1;
    RFU_K7_REG_2(0x1669,0x16e9).rx_att2_c2 = c2;
    RFU_K7_W_2(0x1669,0x16e9);
	return 0;
}

int32_t sp1401_r1a::set_rx_att(double att1,int32_t att2)
{
    INT_CHECK(set_rx_att1(att1));
    INT_CHECK(set_rx_att2(att2));
	return 0;
}

int32_t sp1401_r1a::set_rx_att(double att,double *actual_att1,int32_t *actual_att2)
{
    att = abs(att);
    att = att > 60.0 ? 60.0 : att;

    int32_t att_2 = att > 30.0 ? (int32_t((att - 30) / 10) + 1) * 10 : 0;
	att_2 = att_2 > 30 ? 30 : att_2;
    INT_CHECK(set_rx_att2(att_2,actual_att2));

    double att_1 = att - *actual_att2;
	att_1 = att_1 > 30 ? 30 : att_1;
    INT_CHECK(set_rx_att1(att_1,actual_att1));

	return 0;
}

int32_t sp1401_r1a::get_tx_temp(double &temp)
{
    unsigned temp_hex = 0;

    RFU_K7_REG_DECLARE_2(0x1618,0x1698);
    RFU_K7_R_2(0x1618,0x1698);
    temp_hex = RFU_K7_REG_2(0x1618,0x1698).tmp;

    if ((temp_hex & 0x300) == 0x200) {
        temp = signed((temp_hex & 0xff) - 0x00) / 4.0 + (-128.0);
    }
    if ((temp_hex & 0x300) == 0x300) {
        temp = signed((temp_hex & 0xff) - 0xfc) / 4.0 + (-1.0);
    }
    if ((temp_hex & 0x300) == 0x000) {
        temp = signed((temp_hex & 0xff) - 0x00) / 4.0 + (0.0);
    }
    if ((temp_hex & 0x300) == 0x100) {
        temp = signed((temp_hex & 0xff) - 0x90) / 4.0 + (100.0);
    }
	return 0;
}

int32_t sp1401_r1a::get_rx_temp(double &temp)
{
    unsigned temp_hex = 0;

    RFU_K7_REG_DECLARE_2(0x1615,0x1695);
    RFU_K7_R_2(0x1615,0x1695);
    temp_hex = RFU_K7_REG_2(0x1615,0x1695).tmp;

    if ((temp_hex & 0x300) == 0x200) {
        temp = signed((temp_hex & 0xff) - 0x00) / 4.0 + (-128.0);
    }
    if ((temp_hex & 0x300) == 0x300) {
        temp = signed((temp_hex & 0xff) - 0xfc) / 4.0 + (-1.0);
    }
    if ((temp_hex & 0x300) == 0x000) {
        temp = signed((temp_hex & 0xff) - 0x00) / 4.0 + (0.0);
    }
    if ((temp_hex & 0x300) == 0x100) {
        temp = signed((temp_hex & 0xff) - 0x90) / 4.0 + (100.0);
    }
	return 0;
}

int32_t sp1401_r1a::set_ltc2666(uint16_t ch,int32_t dac_value)
{
    RFU_K7_REG_DECLARE_2(0x163d,0x16bd);
    RFU_K7_REG_DECLARE_2(0x163f,0x16bf);
    RFU_K7_REG_2(0x163f,0x16bf).dont_care    = 0;
    RFU_K7_REG_2(0x163f,0x16bf).command_word = 3; //b0011 write to n,update n
    RFU_K7_REG_2(0x163f,0x16bf).address_word = ch;
    RFU_K7_REG_2(0x163f,0x16bf).data_word    = unsigned(dac_value << 4);
    RFU_K7_WAIT_IDLE_2(0x163d,0x16bd,0,1000);
    RFU_K7_W_2(0x163f,0x16bf);
    RFU_K7_WAIT_IDLE_2(0x163d,0x16bd,0,1000);
    RFU_K7_OP_2(0x163d,0x16bd);
    RFU_K7_WAIT_IDLE_2(0x163d,0x16bd,0,1000);
	return 0;
}

int32_t sp1401_r1a::set_ltc2666(uint16_t ch,double voltage)
{
    double v_min = -5.0;
    double v_max = 5.0;
    int32_t dac_value = int32_t(4095.0 * ((voltage - v_min) / (v_max - v_min)));
    INT_CHECK(set_ltc2666(ch,dac_value));
	return 0;
}

int32_t sp1401_r1a::get_ltc2666(uint16_t ch,int32_t &dac_value)
{
    RFU_K7_REG_DECLARE_2(0x163d,0x16bd);
    RFU_K7_REG_DECLARE_2(0x1640,0x16c0);
    RFU_K7_WAIT_IDLE_2(0x163d,0x16bd,0,1000);
    RFU_K7_R_2(0x1640,0x16c0);
    RFU_K7_WAIT_IDLE_2(0x163d,0x16bd,0,1000);
    dac_value = int32_t(RFU_K7_REG_2(0x1640,0x16c0).data_word);
	return 0;
}

int32_t sp1401_r1a::set_det_sw(rf_ch_t ch)
{
    RFU_K7_REG_DECLARE_2(0x1661,0x16e1);
    RFU_K7_REG_2(0x1661,0x16e1).det_sw = (ch == CH_TX ? 0 : 1);
    RFU_K7_W_2(0x1661,0x16e1);
	return 0;
}

int32_t sp1401_r1a::set_rx_lmh5401N(double vol)
{
    INT_CHECK(set_ltc2666(0,vol));
	return 0;
}

int32_t sp1401_r1a::set_rx_lmh5401P(double vol)
{
    INT_CHECK(set_ltc2666(1,vol));
	return 0;
}

int32_t sp1401_r1a::set_tx_lo2_xb1007(double vol)
{
    INT_CHECK(set_ltc2666(2,vol));
	return 0;
}

int32_t sp1401_r1a::set_rx_if1_hmc694_vgg1(double vol)
{
    INT_CHECK(set_ltc2666(4,vol));
	return 0;
}

int32_t sp1401_r1a::set_rx_if1_hmc694_vga(double vol)
{
    INT_CHECK(set_ltc2666(5,vol));
	return 0;
}

int32_t sp1401_r1a::set_rx_if1_hmc694_vgg2(double vol)
{
    INT_CHECK(set_ltc2666(6,vol));
	return 0;
}

void sp1401_r1a::tx_guess_base_state(io_mode_t &mode,data_f_tx_pwr &data)
{
    ignore_unused(mode);

    data.pwr = -30.0;
    data.d_gain = 0.0;
    data.att1 = 10.0;
    data.att2 = 0.0;
    data.att3 = 0.0;
}

void sp1401_r1a::rx_guess_base_state(io_mode_t &mode,data_f_rx_ref &data)
{
    ignore_unused(mode);

    data.state[0].ref = -10.0;
    data.state[0].lna_att = RX_ATT;
    data.state[0].att1 = 0.0;
    data.state[0].att2 = 0.0;

    data.state[1].ref = -30.0;
    data.state[1].lna_att = RX_LNA;
    data.state[1].att1 = 0.0;
    data.state[1].att2 = 0.0;
}
