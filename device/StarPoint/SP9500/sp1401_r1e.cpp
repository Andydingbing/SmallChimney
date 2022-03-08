#include "sp1401_r1e.h"
#include "../chip/reg_def_sp9500.h"
#include "sleep_common.h"

using namespace ns_starpoint;
using namespace ns_sp9500;
using namespace ns_sp1401;

sp1401_r1e::gpio_a_t::gpio_a_t()
{
    pd_rxif12_txif2 = 0;
    pd_rxcomb_other = 0;
    op_green_led    = 0;
    op_red_led      = 0;
    tx_io_sw2       = 0;
    tx_io_sw1       = 0;
    io_green_led    = 0;
    rsv             = 0;
}

sp1401_r1e::gpio_b_t::gpio_b_t()
{
    io_red_led   = 0;
    rx_io_sw1    = 0;
    rx_io_sw2    = 0;
    rx_io_sw3    = 0;
    loop_sw      = 0;
    rx_att019_sw = 0;
    rx_lnaatt_sw = 0;
    rsv          = 0;
}

int32_t sp1401_r1e::io_mode2io_sw(io_mode_t mode,tx_io_sw1_t &tx_sw1,tx_io_sw2_t &tx_sw2,rx_io_sw1_t &rx_sw1,rx_io_sw2_t &rx_sw2,rx_io_sw3_t &rx_sw3,loop_sw_t &loop_sw,bool &tx_led,bool &rx_led)
{
    if (mode._to_integral() == io_mode_t::IO) {
        tx_sw1 = TX_IO_ON;
        tx_sw2 = TX_OFF_2;
        rx_sw1 = RX_IO_ON_1;
        rx_sw2 = RX_IO_ON_2;
        rx_sw3 = RX_IO_ON_3;
        loop_sw = LOOP_OFF;
        tx_led = false;
        rx_led = true;
    } else if (mode._to_integral() == io_mode_t::OUTPUT) {
        tx_sw1 = TX_ON_1;
        tx_sw2 = TX_ON_2;
        rx_sw1 = RX_IO_ON_1;
        rx_sw2 = RX_ON_2;
        rx_sw3 = RX_ON_3;
        loop_sw  = LOOP_OFF;
        tx_led = true;
        rx_led = true;
    } else if (mode._to_integral() == io_mode_t::LOOP ) {
        tx_sw1 = TX_LOOP_ON;
        tx_sw2 = TX_OFF_2;
        rx_sw1 = RX_IO_OFF_1;
        rx_sw2 = RX_IO_ON_2;
        rx_sw3 = RX_LOOP_ON;
        loop_sw  = LOOP_ON;
        tx_led = false;
        rx_led = false;
    }
    return 0;
}

int32_t sp1401_r1e::set_tx_io_sw1(tx_io_sw1_t sw)
{
    uint32_t reg_gpa = 0;
    INT_CHECK(get_mcp23s17_reg(0x12,reg_gpa));
    gpio_a_t gpa = *(gpio_a_t *)&reg_gpa;
    gpa.tx_io_sw1 = unsigned(sw);
    reg_gpa = *(uint32_t *)&gpa;
    INT_CHECK(set_mcp23s17_reg(0x12,reg_gpa));
    return 0;
}

int32_t sp1401_r1e::set_led(bool tx,bool rx)
{
    uint32_t reg_gpa = 0,reg_gpb = 0;
    INT_CHECK(get_mcp23s17_reg(0x12,reg_gpa));
    INT_CHECK(get_mcp23s17_reg(0x13,reg_gpb));
    gpio_a_t gpa = *(gpio_a_t *)&reg_gpa;
    gpio_b_t gpb = *(gpio_b_t *)&reg_gpb;
    gpa.op_green_led = tx ? 0 : 1;
    gpa.op_red_led   = tx ? 1 : 0;
    gpa.io_green_led = rx ? 0 : 1;
    gpb.io_red_led   = rx ? 1 : 0;
    reg_gpa = *(uint32_t *)&gpa;
    reg_gpb = *(uint32_t *)&gpb;
    INT_CHECK(set_mcp23s17_reg(0x12,reg_gpa));
    INT_CHECK(set_mcp23s17_reg(0x13,reg_gpb));
    return 0;
}

int32_t sp1401_r1e::set_io_mode(io_mode_t mode)
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
    gpb.io_red_led   = io_red   ? 1 : 0;
    gpa.io_green_led = io_green ? 1 : 0;
    reg_gpa = *(uint32_t *)&gpa;
    reg_gpb = *(uint32_t *)&gpb;
    INT_CHECK(set_mcp23s17_reg(0x12,reg_gpa));
    INT_CHECK(set_mcp23s17_reg(0x13,reg_gpb));
    return 0;
}

int32_t sp1401_r1e::set_tx_att0(double att)
{
    int32_t d0_5= int32_t(4.0 * att);

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

int32_t sp1401_r1e::set_tx_att1(double att)
{
    int32_t d0_5 = int32_t(4.0 * att);

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

int32_t sp1401_r1e::set_tx_att2(double att)
{
    int32_t d0_5 = int32_t(4.0 * att);

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

int32_t sp1401_r1e::set_tx_att3(double att)
{
    int32_t d0_5 = int32_t(4.0 * att);

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

int32_t sp1401_r1e::set_rx_att1(double att)
{
    int32_t d0_5 = int32_t(4.0 * att);

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

int32_t sp1401_r1e::set_rx_att2(double att)
{
    int32_t d0_5 = int32_t(4.0 * att);

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
