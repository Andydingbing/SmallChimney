#include "sp1401_r1c.h"
#include "../chip/reg_def_sp9500.h"
#include <math.h>
#include "sleep_common.h"
#include "algo_chip.hpp"

using namespace ns_starpoint;
using namespace ns_sp9500;
using namespace ns_sp1401;
using namespace ns_sp1401::r1c;

sp1401_r1c::gpio_a_t::gpio_a_t()
{
    pd_rxif12_txif2 = 0;
    pd_rxcomb_other = 0;
    op_green_led	= 0;
    op_red_led		= 0;
    tx_io_sw1		= 0;
    io_red_led		= 0;
    tx_io_sw2		= 0;
    rsv			    = 0;
}

sp1401_r1c::gpio_b_t::gpio_b_t()
{
    io_green_led = 0;
    rx_io_sw1	 = 0;
    rx_io_sw2	 = 0;
    rx_io_sw3	 = 0;
    loop_sw	     = 0;
    rx_att019_sw = 0;
    rx_lnaatt_sw = 0;
    rsv		     = 0;
}

sp1401_r1c::sp1401_r1c(uint32_t rf_idx,uint32_t rfu_idx)
{
    m_tx_chain_state.att0 = 5.0;
    m_tx_chain_state.att1 = 20.0;
    m_tx_chain_state.att2 = 0.0;
    m_tx_chain_state.att3 = 1.0;
    m_tx_chain_state.gain = 0.0;
    m_rx_chain_state.att1 = 5.0;
    m_rx_chain_state.att2 = 15.0;
    m_rx_chain_state.att3 = 10.0;
    m_rx_chain_state.lna_att = RX_ATT;
    m_rx_chain_state.att_019 = RX_ATT_19;
}

int32_t sp1401_r1c::open_board()
{
    INT_CHECK(_cal_file->open());
    INT_CHECK(set_pwr_en(true));
    INT_CHECK(init_adf5355());
    INT_CHECK(init_hmc1197());
    INT_CHECK(init_mcp23s17());
    INT_CHECK(set_io_mode(io_mode_t::OUTPUT));
    INT_CHECK(set_tx_modulator_en(true));
    return sp1401::open_board();
}

int32_t sp1401_r1c::init_adf5355()
{
    uint32_t reg[13];
	for (int i = 0;i < 13;i ++)
        reg[i] = 0;

    uint64_t freq_vco = 4000000000;
    uint64_t freq_lo[4] = {0,0,0,0};
    tx_filter_t filter1 = LPF;
    tx_filter_t filter2 = LPF;

    ns_adf5355::freq_formula_in param_in;
    ns_adf5355::freq_formula_out param_out;

    reg[3] = 0x00000013;
    reg[4] = 0x30008594;
    reg[5] = 0x00800025;
    reg[7] = 0x12000067;
    reg[8] = 0x102d0428;
    reg[9] = 0x28323de9;
    reg[10] = 0x00c025ba;
    reg[11] = 0x0061300b;
    reg[12] = 0x4e20041c;

    DECLARE_R1C_SETLOREG_FUNCTION_MAP

    tx_freq2lo(2400000000,freq_lo[0],freq_lo[1],freq_lo[2],filter1,filter2);
    rx_freq2lo(2400000000,freq_lo[3],freq_lo[1]);

    for (int32_t lo_idx = int32_t(TX_LO_2);lo_idx <= int32_t(RX_LO_1);lo_idx ++) {
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
            (this->*set_r1c_lo_reg[lo_idx])(reg[12 - i]);
        }
	}
    set_adf5355(TX_LO_2,freq_lo[int32_t(TX_LO_2)]);
	return 0;
}

int32_t sp1401_r1c::init_hmc1197()
{
    uint32_t reg[32];
	for (int32_t i = 0;i < 32;i ++)
        reg[i] = 0;

    DECLARE_R1C_SETLOREG_FUNCTION_MAP

    reg[0x00] = 0x00000000;	(this->*set_r1c_lo_reg[TX_LO_1])(reg[0x00]);
    reg[0x01] = 0x00000208;	(this->*set_r1c_lo_reg[TX_LO_1])(reg[0x01]);
    reg[0x02] = 0x00000110;	(this->*set_r1c_lo_reg[TX_LO_1])(reg[0x02]);
//	reg[0x03] = 0x00002718;	(this->*set_r1c_lo_reg[TX_LO_1])(reg[0x03]);	//r1c	975MHz * 4
    reg[0x03] = 0x00002518;	(this->*set_r1c_lo_reg[TX_LO_1])(reg[0x03]);	//r1d	925MHz * 4
    reg[0x04] = 0x00000020;	(this->*set_r1c_lo_reg[TX_LO_1])(reg[0x04]);
    reg[0x05] = 0x00000028;	(this->*set_r1c_lo_reg[TX_LO_1])(reg[0x05]);
    reg[0x06] = 0x03078830;	(this->*set_r1c_lo_reg[TX_LO_1])(reg[0x06]);
    reg[0x07] = 0x20094438;	(this->*set_r1c_lo_reg[TX_LO_1])(reg[0x07]);
    reg[0x08] = 0x41bfff40;	(this->*set_r1c_lo_reg[TX_LO_1])(reg[0x08]);
    reg[0x09] = 0x00326448;	(this->*set_r1c_lo_reg[TX_LO_1])(reg[0x09]);
    reg[0x0a] = 0x00204650;	(this->*set_r1c_lo_reg[TX_LO_1])(reg[0x0a]);
    reg[0x0b] = 0x07806158;	(this->*set_r1c_lo_reg[TX_LO_1])(reg[0x0b]);
    reg[0x0c] = 0x00000060;	(this->*set_r1c_lo_reg[TX_LO_1])(reg[0x0c]);
    reg[0x0f] = 0x00008178;	(this->*set_r1c_lo_reg[TX_LO_1])(reg[0x0f]);
    reg[0x14] = 0x000220a0;	(this->*set_r1c_lo_reg[TX_LO_1])(reg[0x14]);
    reg[0x15] = 0x0f48a0a8;	(this->*set_r1c_lo_reg[TX_LO_1])(reg[0x15]);
    reg[0x16] = 0x0006c4b0;	(this->*set_r1c_lo_reg[TX_LO_1])(reg[0x16]);
    reg[0x17] = 0x00012bb8;	(this->*set_r1c_lo_reg[TX_LO_1])(reg[0x17]);
    reg[0x18] = 0x0054c3c0;	(this->*set_r1c_lo_reg[TX_LO_1])(reg[0x18]);
    reg[0x19] = 0x000ab2c8;	(this->*set_r1c_lo_reg[TX_LO_1])(reg[0x19]);
    reg[0x1a] = 0xb29d0bd0;	(this->*set_r1c_lo_reg[TX_LO_1])(reg[0x1a]);

	return 0;
}

int32_t sp1401_r1c::close_board()
{
	return 0;
}

int32_t sp1401_r1c::set_led(bool tx,bool rx)
{
    uint32_t reg_gpa = 0,reg_gpb = 0;
    INT_CHECK(get_mcp23s17_reg(0x12,reg_gpa));
    INT_CHECK(get_mcp23s17_reg(0x13,reg_gpb));
    gpio_a_t gpa = *(gpio_a_t *)&reg_gpa;
    gpio_b_t gpb = *(gpio_b_t *)&reg_gpb;
    gpa.op_green_led = tx ? 0 : 1;
    gpa.op_red_led   = tx ? 1 : 0;
    gpb.io_green_led = rx ? 0 : 1;
    gpa.io_red_led   = rx ? 1 : 0;
    reg_gpa = *(uint32_t *)&gpa;
    reg_gpb = *(uint32_t *)&gpb;
    INT_CHECK(set_mcp23s17_reg(0x12,reg_gpa));
    INT_CHECK(set_mcp23s17_reg(0x13,reg_gpb));
    return 0;
}

bool sp1401_r1c::is_connected()
{
    double temp = 0.0;
    for (uint32_t i = 0;i < 8;i ++) {
        INT_CHECKB(get_temp(i,temp));
        if (temp > 0.0 && temp < 100.0) {
			return true;
        }
    }
    Log.set_last_err(-2,"missing rfu%u--->rf%u",rfu_idx(),rf_idx());
	return false;
}

int32_t sp1401_r1c::tx_freq2lo(
        uint64_t freq,
        uint64_t &lo1,
        uint64_t &lo2,
        uint64_t &lo3,
        tx_filter_t &filter1,
        tx_filter_t &filter2)
{
// 	lo1 = 975000000;	    // R1C
// 	lo2 = 6524000000;	    // R1C
    lo1 = 925000000;	    // R1D/E/F
    lo2 = 6574000000;	    // R1D/E/F
    lo3 = freq + lo1 + lo2; // R1C/D/E/F
    filter1 = (freq >= 3100000000 ? BPF : LPF); // R1C/D/E
    filter2 = filter1;                          // R1C/D/E
	return 0;
}

int32_t sp1401_r1c::tx_lo2freq(uint64_t lo1,uint64_t lo2,uint64_t lo3,uint64_t &freq)
{
    freq = 0;
//	if (975000000 == lo1 && 6524000000 == lo2) {	//r1c
    if (925000000 == lo1 && 6574000000 == lo2) {	//r1d
        if (lo3 > 7499000000 && lo3 <= 13499000000) {
            freq = lo3 - 7499000000;
        } else {
            return -1;
        }
    } else {
        return -1;
    }
	return 0;
}

int32_t sp1401_r1c::rx_freq2lo(uint64_t freq,uint64_t &lo1,uint64_t &lo2)
{
//	lo2 = 6524000000;	//r1c
    lo2 = 6574000000;	//r1d
    lo1 = freq + lo2 + 276000000;
	return 0;
}

int32_t sp1401_r1c::rx_lo2freq(uint64_t lo1,uint64_t lo2,uint64_t &freq)
{
    freq = 0;
//	if (lo1 > 6800000000)	//r1c
    if (lo1 > 6850000000) {	//r1d
        freq = lo1 - lo2 - 276000000;
    } else {
		return -1;
    }
	return 0;
}

int32_t sp1401_r1c::io_mode2io_sw(
        io_mode_t mode,
        tx_io_sw1_t &tx_sw1,
        tx_io_sw2_t &tx_sw2,
        rx_io_sw1_t &rx_sw1,
        rx_io_sw2_t &rx_sw2,
        rx_io_sw3_t &rx_sw3,
        loop_sw_t &loop_sw,
        bool &tx_led,
        bool &rx_led)
{
    switch (mode) {
    case io_mode_t::IO : {
        tx_sw1 = TX_IO_ON;
        tx_sw2 = TX_OFF_2;
        rx_sw1 = RX_IO_ON_1;
        rx_sw2 = RX_IO_ON_2;
        rx_sw3 = RX_IO_ON_3;
        loop_sw  = LOOP_OFF;
        tx_led = false;
        rx_led = true;
        return 0;
    }
    case io_mode_t::OUTPUT : {
        tx_sw1 = TX_ON_1;
        tx_sw2 = TX_ON_2;
        rx_sw1 = RX_IO_ON_1;
        rx_sw2 = RX_ON_2;
        rx_sw3 = RX_ON_3;
        loop_sw  = LOOP_OFF;
        tx_led = true;
        rx_led = true;
        return 0;
    }
    case io_mode_t::LOOP : {
        tx_sw1 = TX_LOOP_ON;
        tx_sw2 = TX_OFF_2;
        rx_sw1 = RX_IO_OFF_1;
        rx_sw2 = RX_IO_ON_2;
        rx_sw3 = RX_LOOP_ON;
        loop_sw  = LOOP_ON;
        tx_led = false;
        rx_led = false;
        return 0;
    }
    default : return 0;
    }
}

int32_t sp1401_r1c::io_mode2led(io_mode_t mode,bool &op_green,bool &op_red,bool &io_green,bool &io_red)
{
    switch (mode) {
    case io_mode_t::IO : {
        op_green = true;
        op_red   = false;
        io_green = false;
        io_red   = true;
        return 0; }
    case io_mode_t::OUTPUT : {
        op_green = false;
        op_red   = true;
        io_green = false;
        io_red   = true;
        return 0; }
    case io_mode_t::LOOP : {
        op_green = true;
        op_red   = false;
        io_green = true;
        io_red   = false;
        return 0; }
    default : return 0;
    }
}

int32_t sp1401_r1c::set_pwr_en(bool en)
{
    RFU_K7_REG_DECLARE_2(0x1829,0x2829);
    RFU_K7_REG_2(0x1829,0x2829).pow_en = (en ? 1 : 0);
    RFU_K7_W_2(0x1829,0x2829);
	return 0;
}

int32_t sp1401_r1c::set_tx_modulator_en(bool en)
{
    RFU_K7_REG_DECLARE_2(0x1804,0x2804);

    RFU_K7_WAIT_IDLE_2(0x1804,0x2804,0,1000);
    RFU_K7_R_2(0x1804,0x2804);
    RFU_K7_WAIT_IDLE_2(0x1804,0x2804,0,1000);
    RFU_K7_REG_2(0x1804,0x2804).mod = en ? 1 : 0;
    RFU_K7_W_2(0x1804,0x2804);
    RFU_K7_WAIT_IDLE_2(0x1804,0x2804,0,1000);
	return 0;
}

int32_t sp1401_r1c::set_io_mode(io_mode_t mode)
{
    tx_io_sw1_t tx_sw1 = TX_IO_ON;
    tx_io_sw2_t tx_sw2 = TX_OFF_2;
    rx_io_sw1_t rx_sw1 = RX_IO_ON_1;
    rx_io_sw2_t rx_sw2 = RX_IO_ON_2;
    rx_io_sw3_t rx_sw3 = RX_IO_ON_3;
    loop_sw_t  loop_sw = LOOP_OFF;
    bool tx_led = false,rx_led = true;
    bool op_green = false,op_red = true,io_green = false,io_red = true;
    uint32_t reg_gpa = 0,reg_gpb = 0;

    io_mode2io_sw(mode,tx_sw1,tx_sw2,rx_sw1,rx_sw2,rx_sw3,loop_sw,tx_led,rx_led);
    io_mode2led(mode,op_green,op_red,io_green,io_red);
    INT_CHECK(get_mcp23s17_reg(0x12,reg_gpa));
    INT_CHECK(get_mcp23s17_reg(0x13,reg_gpb));
    gpio_a_t gpa = *(gpio_a_t *)&reg_gpa;
    gpio_b_t gpb = *(gpio_b_t *)&reg_gpb;
    gpa.tx_io_sw1 = unsigned(tx_sw1);
    gpa.tx_io_sw2 = unsigned(tx_sw2);
    gpb.rx_io_sw1 = unsigned(rx_sw1);
    gpb.rx_io_sw2 = unsigned(rx_sw2);
    gpb.rx_io_sw3 = unsigned(rx_sw3);
    gpb.loop_sw   = unsigned(loop_sw);
    gpa.op_green_led = op_green ? 1 : 0;
    gpa.op_red_led	 = op_red   ? 1 : 0;
    gpb.io_green_led = io_green ? 1 : 0;
    gpa.io_red_led   = io_red   ? 1 : 0;
    reg_gpa = *(uint32_t *)&gpa;
    reg_gpb = *(uint32_t *)&gpb;
    INT_CHECK(set_mcp23s17_reg(0x12,reg_gpa));
    INT_CHECK(set_mcp23s17_reg(0x13,reg_gpb));
	return 0;
}

int32_t sp1401_r1c::get_io_mode(io_mode_t &mode)
{
    uint32_t reg_gpa = 0,reg_gpb = 0;

    INT_CHECK(get_mcp23s17_reg(0x12,reg_gpa));
    INT_CHECK(get_mcp23s17_reg(0x13,reg_gpb));

    if (0xd8 == reg_gpa && 0x38 == reg_gpb) {
        mode = io_mode_t::OUTPUT;
    }
    if (0xe4 == reg_gpa && 0x34 == reg_gpb) {
        mode = io_mode_t::IO;
    }
    if (0x24 == reg_gpa && 0x07 == reg_gpb) {
        mode = io_mode_t::LOOP;
    }
    return 0;
}


int32_t sp1401_r1c::set_adf5355(lo_t lo,uint64_t freq)
{
	uint32_t reg[13];
    ZERO_ARRAY(reg);

    uint64_t freq_vco = freq;
    if (freq > 6800000000) { freq_vco = freq / 2; }
    if (freq < 3400000000) { freq_vco = freq * 2; }

    ns_adf5355::freq_formula_in param_in;
    ns_adf5355::freq_formula_out param_out;

    param_in.freq_vco = freq_vco;

    ns_adf5355::freq_formula(param_in,param_out);
    reg[0] = ns_adf5355::reg0(param_out._int,0,1);
    reg[1] = ns_adf5355::reg1(param_out._frac1);
    reg[2] = ns_adf5355::reg2(param_out._mod2,param_out._frac2);
    reg[4] = 0x30008594;
    reg[6] = freq < 3400000000 ? 0x3520a076 : 0x3500a076;

    DECLARE_R1C_SETLOREG_FUNCTION_MAP

    (this->*set_r1c_lo_reg[lo])(reg[6]);
	reg[4] = reg[4] | (1 << 4);
    (this->*set_r1c_lo_reg[lo])(reg[4]);
    (this->*set_r1c_lo_reg[lo])(reg[2]);
    (this->*set_r1c_lo_reg[lo])(reg[1]);
    reg[0] = reg[0] & uint32_t(~(1 << 21));
    (this->*set_r1c_lo_reg[lo])(reg[0]);
    reg[4] = reg[4] & uint32_t(~(1 << 4));
    (this->*set_r1c_lo_reg[lo])(reg[4]);
    sleep_ms(100);
	reg[0] = reg[0] | (1 << 21);
    (this->*set_r1c_lo_reg[lo])(reg[0]);
	return 0;
}

int32_t sp1401_r1c::set_hmc1197(uint64_t freq)
{
	return 0;
}

int32_t sp1401_r1c::det_lo(lo_t lo,bool &lock)
{
    sleep_ms(100);

    DECLARE_R1C_DETLOLOCK_FUNCTION_MAP

    return (this->*det_r1c_lo_lock[lo])(lock);
}

int32_t sp1401_r1c::set_tx_lo1_reg(uint32_t data)
{
    return set_tx_lo1_reg_open_mode(data);
}

int32_t sp1401_r1c::get_tx_lo1_reg(uint32_t reg,uint32_t &data)
{
    return get_tx_lo1_reg_open_mode(reg,data);
}

int32_t sp1401_r1c::set_tx_lo1_reg_open_mode(uint32_t data)
{	
    RFU_K7_REG_DECLARE_2(0x1804,0x2804);
    RFU_K7_REG_DECLARE_2(0x1805,0x2805);

    RFU_K7_WAIT_IDLE_2(0x1804,0x2804,0,1000);
    RFU_K7_R_2(0x1804,0x2804);
    RFU_K7_WAIT_IDLE_2(0x1804,0x2804,0,1000);
    RFU_K7_REG_2(0x1804,0x2804).wr = 0;
    RFU_K7_W_2(0x1804,0x2804);
    RFU_K7_WAIT_IDLE_2(0x1804,0x2804,0,1000);
    RFU_K7_REG_2(0x1805,0x2805).data = data;
    RFU_K7_W_2(0x1805,0x2805);
    RFU_K7_WAIT_IDLE_2(0x1804,0x2804,0,1000);
    RFU_K7_OP_2(0x1804,0x2804);
    RFU_K7_WAIT_IDLE_2(0x1804,0x2804,0,1000);
	return 0;
}

int32_t sp1401_r1c::get_tx_lo1_reg_open_mode(uint32_t reg,uint32_t &data)
{
    RFU_K7_REG_DECLARE_2(0x1804,0x2804);
    RFU_K7_REG_DECLARE_2(0x1805,0x2805);
    RFU_K7_REG_DECLARE_2(0x1807,0x2807);

    RFU_K7_WAIT_IDLE_2(0x1804,0x2804,0,1000);
    RFU_K7_R_2(0x1804,0x2804);
    RFU_K7_WAIT_IDLE_2(0x1804,0x2804,0,1000);
    RFU_K7_REG_2(0x1804,0x2804).wr = 1;
    RFU_K7_W_2(0x1804,0x2804);
    RFU_K7_WAIT_IDLE_2(0x1804,0x2804,0,1000);
    RFU_K7_REG_2(0x1805,0x2805).data = (0 | (reg << 8));
    RFU_K7_W_2(0x1805,0x2805);
    RFU_K7_WAIT_IDLE_2(0x1804,0x2804,0,1000);
    RFU_K7_OP_2(0x1804,0x2804);
    RFU_K7_WAIT_IDLE_2(0x1804,0x2804,0,1000);
    RFU_K7_R_2(0x1807,0x2807);
    RFU_K7_WAIT_IDLE_2(0x1804,0x2804,0,1000);
    data = uint32_t(RFU_K7_REG_2(0x1807,0x2807).data) >> 8;
	return 0;
}

int32_t sp1401_r1c::set_tx_lo1_reg_hmc_mode(uint32_t data)
{
    RFU_K7_REG_DECLARE_2(0x1836,0x2836);
    RFU_K7_REG_DECLARE_2(0x1837,0x2837);

    RFU_K7_REG_2(0x1836,0x2836).wr = 0;
    RFU_K7_REG_2(0x1836,0x2836).addr = (data & 0x000000f8) >> 3;
    RFU_K7_W_2(0x1836,0x2836);
    RFU_K7_REG_2(0x1837,0x2837).data = (data & 0xffffff00) >> 8;
    RFU_K7_W_2(0x1837,0x2837);

    RFU_K7_WAIT_IDLE_2(0x1836,0x2836,0,1000);
    RFU_K7_OP_2(0x1836,0x2836);
    RFU_K7_WAIT_IDLE_2(0x1836,0x2836,0,1000);
	return 0;
}

int32_t sp1401_r1c::get_tx_lo1_reg_hmc_mode(uint32_t reg,uint32_t &data)
{
    RFU_K7_REG_DECLARE_2(0x1836,0x2836);
    RFU_K7_REG_DECLARE_2(0x1838,0x2838);

    RFU_K7_REG_2(0x1836,0x2836).wr = 1;
    RFU_K7_REG_2(0x1836,0x2836).addr = reg;
    RFU_K7_W_2(0x1836,0x2836);

    RFU_K7_WAIT_IDLE_2(0x1836,0x2836,0,1000);
    RFU_K7_OP_2(0x1836,0x2836);
    RFU_K7_WAIT_IDLE_2(0x1836,0x2836,0,1000);

    RFU_K7_R_2(0x1838,0x2838);
    data = uint32_t(RFU_K7_REG_2(0x1838,0x2838).data);
	return 0;
}

int32_t sp1401_r1c::det_tx_lo1_lock(bool &lock)
{
    RFU_K7_REG_DECLARE_2(0x160f,0x168f);
    RFU_K7_R_2(0x160f,0x168f);
    lock = (RFU_K7_REG_2(0x160f,0x168f).muxout == 0 ? false : true);
	return 0;
}

int32_t sp1401_r1c::set_tx_lo2_reg(uint32_t data)
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

int32_t sp1401_r1c::det_tx_lo2_lock(bool &lock)
{
    RFU_K7_REG_DECLARE_2(0x160b,0x168b);
    RFU_K7_R_2(0x160b,0x168b);
    lock = (RFU_K7_REG_2(0x160b,0x168b).muxout == 0 ? false : true);
	return 0;
}

int32_t sp1401_r1c::set_tx_lo3_reg(uint32_t data)
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

int32_t sp1401_r1c::det_tx_lo3_lock(bool &lock)
{
    RFU_K7_REG_DECLARE_2(0x160f,0x168f);
    RFU_K7_R_2(0x160f,0x168f);
    lock = (RFU_K7_REG_2(0x160f,0x168f).muxout == 0 ? false : true);
	return 0;
}

int32_t sp1401_r1c::set_tx_freq(uint64_t freq)
{
    uint64_t lo1 = 0,lo2 = 0,lo3 = 0;
    tx_filter_t filter1 = LPF;
    tx_filter_t filter2 = LPF;
    tx_freq2lo(freq,lo1,lo2,lo3,filter1,filter2);
    INT_CHECK(set_tx_filter_sw(filter1,filter2));
    INT_CHECK(set_adf5355(TX_LO_3,lo3));
	return 0;
}

int32_t sp1401_r1c::set_tx_filter_sw(tx_filter_t filter1,tx_filter_t filter2)
{
    RFU_K7_REG_DECLARE_2(0x1828,0x2828);
    RFU_K7_REG_DECLARE_2(0x1835,0x2835);
    RFU_K7_REG_2(0x1828,0x2828).filter = (LPF == filter1 ? 0 : 1);
    RFU_K7_REG_2(0x1835,0x2835).filter = (LPF == filter2 ? 0 : 1);
    RFU_K7_W_2(0x1828,0x2828);
    RFU_K7_W_2(0x1835,0x2835);
	return 0;
}

int32_t sp1401_r1c::set_tx_io_sw1(tx_io_sw1_t sw)
{
    uint32_t reg_gpa = 0;
    INT_CHECK(get_mcp23s17_reg(0x12,reg_gpa));
    gpio_a_t gpa = *(gpio_a_t *)&reg_gpa;
    gpa.tx_io_sw1 = unsigned(sw);
    reg_gpa = *(uint32_t *)&gpa;
    INT_CHECK(set_mcp23s17_reg(0x12,reg_gpa));
    return 0;
}

int32_t sp1401_r1c::set_tx_io_sw2(tx_io_sw2_t sw)
{
    uint32_t reg_gpa = 0;
    INT_CHECK(get_mcp23s17_reg(0x12,reg_gpa));
    gpio_a_t gpa = *(gpio_a_t *)&reg_gpa;
    gpa.tx_io_sw2 = unsigned(sw);
    reg_gpa = *(uint32_t *)&gpa;
    INT_CHECK(set_mcp23s17_reg(0x12,reg_gpa));
	return 0;
}

int32_t sp1401_r1c::set_loop_sw(loop_sw_t sw)
{
    uint32_t reg_gpb = 0;
    INT_CHECK(get_mcp23s17_reg(0x13,reg_gpb));
    gpio_b_t gpb = *(gpio_b_t *)&reg_gpb;
    gpb.loop_sw = unsigned(sw);
    reg_gpb = *(uint32_t *)&gpb;
    INT_CHECK(set_mcp23s17_reg(0x13,reg_gpb));
	return 0;
}

int32_t sp1401_r1c::set_tx_att0(double att)
{
    int32_t d0_5 = 63 - int32_t(63 * att / 31.5);

    RFU_K7_REG_DECLARE_2(0x1810,0x2810);
    RFU_K7_REG_DECLARE_2(0x1811,0x2811);
    RFU_K7_REG_2(0x1811,0x2811).data_word = unsigned(d0_5);
    RFU_K7_WAIT_IDLE_2(0x1810,0x2810,0,1000);
    RFU_K7_W_2(0x1811,0x2811);
    RFU_K7_WAIT_IDLE_2(0x1810,0x2810,0,1000);
    RFU_K7_OP_2(0x1810,0x2810);
    RFU_K7_WAIT_IDLE_2(0x1810,0x2810,0,1000);
    m_tx_chain_state.att0 = att;
	return 0;
}

int32_t sp1401_r1c::set_tx_att1(double att)
{
    int32_t d0_5 = 63 - int32_t(63 * att / 31.5);

    RFU_K7_REG_DECLARE_2(0x1814,0x2814);
    RFU_K7_REG_DECLARE_2(0x1815,0x2815);
    RFU_K7_REG_2(0x1815,0x2815).data_word = unsigned(d0_5);
    RFU_K7_WAIT_IDLE_2(0x1814,0x2814,0,1000);
    RFU_K7_W_2(0x1815,0x2815);
    RFU_K7_WAIT_IDLE_2(0x1814,0x2814,0,1000);
    RFU_K7_OP_2(0x1814,0x2814);
    RFU_K7_WAIT_IDLE_2(0x1814,0x2814,0,1000);
    m_tx_chain_state.att1 = att;
	return 0;
}

int32_t sp1401_r1c::set_tx_att2(double att)
{
    int32_t d0_5 = 63 - int32_t(63 * att / 31.5);

    RFU_K7_REG_DECLARE_2(0x1818,0x2818);
    RFU_K7_REG_DECLARE_2(0x1819,0x2819);
    RFU_K7_REG_2(0x1819,0x2819).data_word = unsigned(d0_5);
    RFU_K7_WAIT_IDLE_2(0x1818,0x2818,0,1000);
    RFU_K7_W_2(0x1819,0x2819);
    RFU_K7_WAIT_IDLE_2(0x1818,0x2818,0,1000);
    RFU_K7_OP_2(0x1818,0x2818);
    RFU_K7_WAIT_IDLE_2(0x1818,0x2818,0,1000);
    m_tx_chain_state.att2 = att;
	return 0;
}

int32_t sp1401_r1c::set_tx_att3(double att)
{
    int32_t d0_5 = 63 - int32_t(63 * att / 31.5);

    RFU_K7_REG_DECLARE_2(0x181c,0x281c);
    RFU_K7_REG_DECLARE_2(0x181d,0x281d);
    RFU_K7_REG_2(0x181d,0x281d).data_word = unsigned(d0_5);
    RFU_K7_WAIT_IDLE_2(0x181c,0x281c,0,1000);
    RFU_K7_W_2(0x181d,0x281d);
    RFU_K7_WAIT_IDLE_2(0x181c,0x281c,0,1000);
    RFU_K7_OP_2(0x181c,0x281c);
    RFU_K7_WAIT_IDLE_2(0x181c,0x281c,0,1000);
    m_tx_chain_state.att3 = att;
	return 0;
}

int32_t sp1401_r1c::set_tx_att(double att0,double att1,double att2,double att3)
{
    INT_CHECK(set_tx_att0(att0));
    INT_CHECK(set_tx_att1(att1));
    INT_CHECK(set_tx_att2(att2));
    INT_CHECK(set_tx_att3(att3));
	return 0;
}

int32_t sp1401_r1c::set_tx_att(double att)
{
	return 0;
}

int32_t sp1401_r1c::sync_set_tx_gain(double att0,double att1,double att2,double att3,double d_gain)
{
    RFU_K7_REG_DECLARE_2(0x0094,0x2094);
    RFU_K7_REG_DECLARE_2(0x00e6,0x20e6);

    RFU_K7_REG_2(0x00e6,0x20e6).op_0 = 0;
    RFU_K7_REG_2(0x00e6,0x20e6).op_1 = 0;
    RFU_K7_W_2(0x00e6,0x20e6);
    RFU_K7_REG_2(0x00e6,0x20e6).op_1 = 1;
    RFU_K7_W_2(0x00e6,0x20e6);

    INT_CHECK(set_tx_att(att0,att1,att2,att3));
    RFU_K7_REG_2(0x0094,0x2094).i = uint16_t(d_gain * 512.0);
    RFU_K7_REG_2(0x0094,0x2094).q = uint16_t(d_gain * 512.0);
    RFU_K7_W_2(0x0094,0x2094);

    RFU_K7_REG_2(0x00e6,0x20e6).op_0 = 1;
    RFU_K7_W_2(0x00e6,0x20e6);

    RFU_K7_REG_2(0x00e6,0x20e6).op_0 = 0;
    RFU_K7_REG_2(0x00e6,0x20e6).op_1 = 0;
    RFU_K7_W_2(0x00e6,0x20e6);

    m_tx_chain_state.att0 = att0;
    m_tx_chain_state.att1 = att1;
    m_tx_chain_state.att2 = att2;
    m_tx_chain_state.att3 = att3;
    m_tx_chain_state.gain = d_gain;
	return 0;
}

int32_t sp1401_r1c::set_rx_freq(uint64_t freq)
{
    uint64_t lo1 = 0,lo2 = 0;
    rx_freq2lo(freq,lo1,lo2);
    INT_CHECK(set_adf5355(RX_LO_1,lo1));
    m_rx_chain_state.lo1 = lo1;
    m_rx_chain_state.lo2 = lo2;
	return 0;
}

int32_t sp1401_r1c::set_rx_io_sw1(rx_io_sw1_t sw)
{
    uint32_t reg_gpb = 0;
    INT_CHECK(get_mcp23s17_reg(0x13,reg_gpb));
    gpio_b_t gpb = *(gpio_b_t *)&reg_gpb;
    gpb.rx_io_sw1 = unsigned(sw);
    reg_gpb = *(uint32_t *)&gpb;
    INT_CHECK(set_mcp23s17_reg(0x13,reg_gpb));
	return 0;
}

int32_t sp1401_r1c::set_rx_io_sw2(rx_io_sw2_t sw)
{
    uint32_t reg_gpb = 0;
    INT_CHECK(get_mcp23s17_reg(0x13,reg_gpb));
    gpio_b_t gpb = *(gpio_b_t *)&reg_gpb;
    gpb.rx_io_sw2 = unsigned(sw);
    reg_gpb = *(uint32_t *)&gpb;
    INT_CHECK(set_mcp23s17_reg(0x13,reg_gpb));
	return 0;
}

int32_t sp1401_r1c::set_rx_io_sw3(rx_io_sw3_t sw)
{
    uint32_t reg_gpb = 0;
    INT_CHECK(get_mcp23s17_reg(0x13,reg_gpb));
    gpio_b_t gpb = *(gpio_b_t *)&reg_gpb;
    gpb.rx_io_sw3 = unsigned(sw);
    reg_gpb = *(uint32_t *)&gpb;
    INT_CHECK(set_mcp23s17_reg(0x13,reg_gpb));
	return 0;
}

int32_t sp1401_r1c::set_rx_lo1_reg(uint32_t data)
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

int32_t sp1401_r1c::det_rx_lo1_lock(bool &lock)
{
    RFU_K7_REG_DECLARE_2(0x1603,0x1683);
    RFU_K7_R_2(0x1603,0x1683);
    lock = (RFU_K7_REG_2(0x1603,0x1683).muxout == 0 ? false : true);
	return 0;
}

int32_t sp1401_r1c::set_rx_att_019_sw(rx_att_019_t att)
{
    uint32_t reg_gpb = 0;
    INT_CHECK(get_mcp23s17_reg(0x13,reg_gpb));
    gpio_b_t gpb = *(gpio_b_t*)&reg_gpb;
    gpb.rx_att019_sw = unsigned(att);
    reg_gpb = *(uint32_t *)&gpb;
    INT_CHECK(set_mcp23s17_reg(0x13,reg_gpb));
    m_rx_chain_state.att_019 = att;
	return 0;
}

int32_t sp1401_r1c::set_rx_lna_att_sw(rx_lna_att_t lna_att)
{
    uint32_t reg_gpb = 0;
    INT_CHECK(get_mcp23s17_reg(0x13,reg_gpb));
    gpio_b_t gpb = *(gpio_b_t *)&reg_gpb;
    gpb.rx_lnaatt_sw = unsigned(lna_att);
    reg_gpb = *(uint32_t *)&gpb;
    INT_CHECK(set_mcp23s17_reg(0x13,reg_gpb));
    m_rx_chain_state.lna_att = lna_att;
	return 0;
}

int32_t sp1401_r1c::set_rx_att1(double att)
{
    int32_t d0_5 = 63 - int32_t(63 * att / 31.5);

    RFU_K7_REG_DECLARE_2(0x1820,0x2820);
    RFU_K7_REG_DECLARE_2(0x1821,0x2821);
    RFU_K7_REG_2(0x1821,0x2821).data_word = unsigned(d0_5);
    RFU_K7_WAIT_IDLE_2(0x1820,0x2820,0,1000);
    RFU_K7_W_2(0x1821,0x2821);
    RFU_K7_WAIT_IDLE_2(0x1820,0x2820,0,1000);
    RFU_K7_OP_2(0x1820,0x2820);
    RFU_K7_WAIT_IDLE_2(0x1820,0x2820,0,1000);
    m_rx_chain_state.att1 = att;
	return 0;
}

int32_t sp1401_r1c::set_rx_att2(double att)
{
    int32_t d0_5 = 63 - int32_t(63 * att / 31.5);

    RFU_K7_REG_DECLARE_2(0x1824,0x2824);
    RFU_K7_REG_DECLARE_2(0x1825,0x2825);
    RFU_K7_REG_2(0x1825,0x2825).data_word = unsigned(d0_5);
    RFU_K7_WAIT_IDLE_2(0x1824,0x2824,0,1000);
    RFU_K7_W_2(0x1825,0x2825);
    RFU_K7_WAIT_IDLE_2(0x1824,0x2824,0,1000);
    RFU_K7_OP_2(0x1824,0x2824);
    RFU_K7_WAIT_IDLE_2(0x1824,0x2824,0,1000);
    m_rx_chain_state.att2 = att;
	return 0;
}

int32_t sp1401_r1c::set_rx_att3(double att)
{
    m_rx_chain_state.att3 = att;
    int32_t d0_5 = 63 - int32_t(63 * att / 31.5);

    RFU_K7_REG_DECLARE_2(0x182b,0x282b);
    RFU_K7_REG_DECLARE_2(0x182c,0x282c);
    RFU_K7_REG_2(0x182c,0x282c).data_word = unsigned(d0_5);
    RFU_K7_WAIT_IDLE_2(0x182b,0x282b,0,1000);
    RFU_K7_W_2(0x182c,0x282c);
    RFU_K7_WAIT_IDLE_2(0x182b,0x282b,0,1000);
    RFU_K7_OP_2(0x182b,0x282b);
    RFU_K7_WAIT_IDLE_2(0x182b,0x282b,0,1000);
	return 0;
}

int32_t sp1401_r1c::set_rx_att(double att1,double att2,double att3)
{
    INT_CHECK(set_rx_att1(att1));
    INT_CHECK(set_rx_att2(att2));
    INT_CHECK(set_rx_att3(att3));
	return 0;
}

int32_t sp1401_r1c::set_rx_att(const rx_state_m_t &data)
{
    INT_CHECK(set_rx_lna_att_sw(rx_lna_att_t(data.lna_att)));
    INT_CHECK(set_rx_att_019_sw(rx_att_019_t(data.att_019)));
    INT_CHECK(set_rx_att(data.att1,data.att2,data.att3));
    return 0;
}

int32_t sp1401_r1c::get_temp(uint32_t idx,double &temp)
{
    uint32_t ad = 0;
    INT_CHECK(get_mcp3208(idx,ad));
    temp = ns_tc1047::voltage_to_temp(ns_mcp3208::ad_to_voltage_mv(int16_t(ad)));
    return 0;
}

bool sp1401_r1c::get_tx_avg_temp(double &temp)
{
    double temp_curr[4] = {0.0,0.0,0.0,0.0};
    const uint32_t retry = 3;
    uint32_t i = 0;

    for (uint32_t j = 0;j < 4;j ++) {
        for (i = 0;i < retry;i ++) {
            get_temp(j + 4,temp_curr[j]);
            if (temp_curr[j] < 0.0 || temp_curr[j] > 100.0)  {
                continue;
            } else {
                break;
            }
        }
        if (i == retry) {
            return false;
        }
    }
    temp = (temp_curr[0] + temp_curr[1] + temp_curr[2] + temp_curr[3]) / 4.0;
    return true;
}

bool sp1401_r1c::get_rx_avg_temp(double &temp)
{
    double temp_curr[4] = {0.0,0.0,0.0,0.0};
    const uint32_t retry = 3;
    uint32_t i = 0;

    for (uint32_t j = 0;j < 4;j ++) {
        for (i = 0;i < retry;i ++) {
            get_temp(j,temp_curr[j]);
            if (temp_curr[j] < 0.0 || temp_curr[j] > 100.0)  {
                continue;
            } else {
                break;
            }
        }
        if (i == retry) {
            return false;
        }
    }
    temp = (temp_curr[0] + temp_curr[1] + temp_curr[2] + temp_curr[3]) / 4.0;
    return true;
}

int32_t sp1401_r1c::get_ad7680_tx_if2(uint32_t &det)
{
    RFU_K7_REG_DECLARE_2(0x1827,0x2827);

    uint32_t reg_gpa = 0;
    INT_CHECK(get_mcp23s17_reg(0x12,reg_gpa));
    gpio_a_t gpa = *(gpio_a_t *)&reg_gpa;
    RFU_K7_REG_2(0x1827,0x2827).sw = 0;
    gpa.pd_rxif12_txif2 = 1;
    gpa.pd_rxcomb_other = 0;
    reg_gpa = *(uint32_t *)&gpa;
    RFU_K7_W_2(0x1827,0x2827);
    INT_CHECK(set_mcp23s17_reg(0x12,reg_gpa));
    INT_CHECK(get_ad7680(det));
	return 0;
}

int32_t sp1401_r1c::get_ad7680_rx_comb(uint32_t &det)
{
    RFU_K7_REG_DECLARE_2(0x1827,0x2827);

    uint32_t reg_gpa = 0;
    INT_CHECK(get_mcp23s17_reg(0x12,reg_gpa));
    gpio_a_t gpa = *(gpio_a_t *)&reg_gpa;
    RFU_K7_REG_2(0x1827,0x2827).sw = 1;
    gpa.pd_rxif12_txif2 = 0;
    gpa.pd_rxcomb_other = 1;
    reg_gpa = *(uint32_t *)&gpa;
    RFU_K7_W_2(0x1827,0x2827);
    INT_CHECK(set_mcp23s17_reg(0x12,reg_gpa));
    INT_CHECK(get_ad7680(det));
	return 0;
}

int32_t sp1401_r1c::get_ad7680_rx_if1(uint32_t &det)
{
    RFU_K7_REG_DECLARE_2(0x1827,0x2827);

    uint32_t reg_gpa = 0;
    INT_CHECK(get_mcp23s17_reg(0x12,reg_gpa));
    gpio_a_t gpa = *(gpio_a_t *)&reg_gpa;
    RFU_K7_REG_2(0x1827,0x2827).sw = 0;
    gpa.pd_rxif12_txif2 = 0;
    gpa.pd_rxcomb_other = 0;
    reg_gpa = *(uint32_t *)&gpa;
    RFU_K7_W_2(0x1827,0x2827);
    INT_CHECK(set_mcp23s17_reg(0x12,reg_gpa));
    INT_CHECK(get_ad7680(det));
	return 0;
}

int32_t sp1401_r1c::get_ad7680_rx_if2(uint32_t &det)
{
    RFU_K7_REG_DECLARE_2(0x1827,0x2827);

    uint32_t reg_gpa = 0;
    INT_CHECK(get_mcp23s17_reg(0x12,reg_gpa));
    gpio_a_t gpa = *(gpio_a_t *)&reg_gpa;
    RFU_K7_REG_2(0x1827,0x2827).sw = 1;
    gpa.pd_rxif12_txif2 = 0;
    gpa.pd_rxcomb_other = 0;
    reg_gpa = *(uint32_t *)&gpa;
    RFU_K7_W_2(0x1827,0x2827);
    INT_CHECK(set_mcp23s17_reg(0x12,reg_gpa));
    INT_CHECK(get_ad7680(det));
	return 0;
}

int32_t sp1401_r1c::set_mcp23s17_reg(uint32_t addr,uint32_t data)
{
    RFU_K7_REG_DECLARE_2(0x182d,0x282d);
    RFU_K7_REG_DECLARE_2(0x182e,0x282e);

    RFU_K7_WAIT_IDLE_2(0x182d,0x282d,0,1000);
    RFU_K7_REG_2(0x182d,0x282d).addr = addr;
    RFU_K7_REG_2(0x182d,0x282d).wr = 0;
    RFU_K7_REG_2(0x182e,0x282e).data = data;
    RFU_K7_W_2(0x182d,0x282d);
    RFU_K7_W_2(0x182e,0x282e);
    RFU_K7_OP_2(0x182d,0x282d);
    RFU_K7_WAIT_IDLE_2(0x182d,0x282d,0,1000);
	return 0;
}

int32_t sp1401_r1c::get_mcp23s17_reg(uint32_t addr,uint32_t &data)
{
    RFU_K7_REG_DECLARE_2(0x182d,0x282d);
    RFU_K7_REG_DECLARE_2(0x182f,0x282f);

    RFU_K7_WAIT_IDLE_2(0x182d,0x282d,0,1000);
    RFU_K7_REG_2(0x182d,0x282d).addr = addr;
    RFU_K7_REG_2(0x182d,0x282d).wr = 1;
    RFU_K7_W_2(0x182d,0x282d);
    RFU_K7_OP_2(0x182d,0x282d);
    RFU_K7_WAIT_IDLE_2(0x182d,0x282d,0,1000);
    RFU_K7_R_2(0x182f,0x282f);
    data = RFU_K7_REG_2(0x182f,0x282f).data;
	return 0;
}

int32_t sp1401_r1c::init_mcp23s17()
{
    INT_CHECK(set_mcp23s17_reg(0,0x00));
    INT_CHECK(set_mcp23s17_reg(1,0x00));
	return 0;
}

int32_t sp1401_r1c::get_mcp3208(uint32_t addr,uint32_t &data)
{
    RFU_K7_REG_DECLARE_2(0x1832,0x2832);
    RFU_K7_REG_DECLARE_2(0x1833,0x2833);

    RFU_K7_REG_2(0x1832,0x2832).addr = addr;
    RFU_K7_OP_2(0x1832,0x2832);
    RFU_K7_WAIT_IDLE_2(0x1832,0x2832,0,1000);
    RFU_K7_R_2(0x1833,0x2833);
    data = RFU_K7_REG_2(0x1833,0x2833).ad;
	return 0;
}

int32_t sp1401_r1c::get_tx_chain_state(tx_chain_state_all_t  &tx_chain_state)
{
    tx_chain_state = m_tx_chain_state;
    return 0;
}

int32_t sp1401_r1c::get_rx_chain_state(rx_chain_state_all_t  &rx_chain_state)
{
    rx_chain_state = m_rx_chain_state;
    return 0;
}
