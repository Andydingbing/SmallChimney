#ifndef SP1401_R1A_H
#define SP1401_R1A_H

#include "sp1401.h"
#include "cal_file.h"

namespace ns_starpoint {
namespace ns_sp9500 {

class API sp1401_r1a : public sp1401
{
public:
    typedef boost::shared_ptr<sp1401_r1a> sptr;
    virtual ~sp1401_r1a() {}
    DECLARE_SP1401(sp1401_r1a)
public:
    enum lo_t {
        TX_LO_1 = 0,
        TX_LO_2,
        RX_LO_1,
        RX_LO_2
	};
    enum tx_band_t {
        BAND_LOW = 0,
        BAND_HIGH
	};
    enum tx_io_sw_t {
        TX_Off = 0,
        TX_LOOP_ON,
        TX_ON,
        TX_IO_ON
	};
    enum rx_io_sw1_t {
        RX_IO_ON_1 = 0,
        RX_IO_OFF_1
	};
    enum rx_io_sw2_t {
        RX_ON_2 = 0,
        RX_IO_ON_2
	};
    enum rx_io_sw_t {
        RX_LOOP_ON = 0,
        RX_OFF,
        RX_IO_ON,
        RX_ON
	};
    struct tx_chain_state_all_t {
        bool        pwr_sw;
        bool        adl5375;
        tx_band_t   band;
        uint64_t    lo1;
        uint64_t    lo2;
        int32_t     att1;
        int32_t     att2;
        int32_t     att3;
        uint32_t    det;
        double      temp;
        ns_sp1401::r1a::tx_pa_att_t pa_att;
    };
    struct rx_chain_state_all_t {
        double   att1;
        int32_t  att2;
        uint64_t lo1;
        uint64_t lo2;
        uint32_t det;
        double   temp;
        rx_if_filter_t if_filter;
        io_mode_t mode;
        ns_sp1401::r1a::rx_lna_att_t lna_att;
    };

public:
    int32_t tx_freq2lo(uint64_t freq,uint64_t &lo1,uint64_t &lo2,tx_band_t &band);
    int32_t tx_lo2freq(uint64_t lo1,uint64_t lo2,tx_band_t band,uint64_t &freq);
    int32_t rx_freq2lo(uint64_t freq,uint64_t &lo1,uint64_t &lo2);
    int32_t rx_lo2freq(uint64_t lo1,uint64_t lo2,uint64_t &freq);
    int32_t io_mode2io_sw(
            io_mode_t mode,
            tx_io_sw_t &tx_sw,
            rx_io_sw_t &rx_sw,
            rx_io_sw1_t &rx_sw1,
            rx_io_sw2_t &rx_sw2,
            bool &tx_led,bool &rx_led);

    int32_t init_lo();
    int32_t set_lo(lo_t lo,uint64_t freq);
    int32_t det_lo(lo_t lo,bool &lock);

    int32_t set_tx_band_sw(tx_band_t band);
    int32_t set_tx_pa_att_sw(ns_sp1401::r1a::tx_pa_att_t pa_att);
    int32_t set_tx_io_sw(tx_io_sw_t sw);
    int32_t set_tx_lo1_reg(uint32_t data);
    int32_t set_tx_lo2_reg(uint32_t data);
    int32_t det_tx_lo1_lock(bool &lock);
    int32_t det_tx_lo2_lock(bool &lock);

    int32_t set_tx_att1(int32_t att);
    int32_t set_tx_att2(int32_t att);
    int32_t set_tx_att3(int32_t att);
    int32_t set_tx_att(int32_t att1,int32_t att2,int32_t att3);
    int32_t sync_set_tx_gain(int32_t att1,int32_t att2,int32_t att3,double d_gain);
	
    int32_t set_rx_io_sw1(rx_io_sw1_t sw);
    int32_t set_rx_io_sw2(rx_io_sw2_t sw);
    int32_t set_rx_io_sw(rx_io_sw_t sw);
    int32_t set_rx_lo1_reg(uint32_t data);
    int32_t set_rx_lo2_reg(uint32_t data);
    int32_t det_rx_lo1_lock(bool &lock);
    int32_t det_rx_lo2_lock(bool &lock);
    int32_t set_rx_lna_att_sw(ns_sp1401::r1a::rx_lna_att_t lna_att);
    int32_t set_rx_att1(double att,double *actual = nullptr);
    int32_t set_rx_att2(int32_t att,int32_t *actual = nullptr);
    int32_t set_rx_att(double att1,int32_t att2);
    int32_t set_rx_att(double att,double *actual_att1 = nullptr,int32_t *actual_att2 = nullptr);

    int32_t get_tx_temp(double &temp);
    int32_t get_rx_temp(double &temp);

    int32_t set_ltc2666(uint16_t ch,int32_t dac_value);
    int32_t set_ltc2666(uint16_t ch,double voltage);
    int32_t get_ltc2666(uint16_t ch,int32_t &dac_value);

    int32_t set_det_sw(rf_ch_t ch);

    int32_t set_rx_lmh5401N(double vol);
    int32_t set_rx_lmh5401P(double vol);
    int32_t set_tx_lo2_xb1007(double vol);
    int32_t set_rx_if1_hmc694_vgg1(double vol);
    int32_t set_rx_if1_hmc694_vga(double vol);
    int32_t set_rx_if1_hmc694_vgg2(double vol);
};

typedef int32_t (sp1401_r1a::*r1a_set_lo_reg)(uint32_t);
typedef int32_t (sp1401_r1a::*r1a_det_lo_lock)(bool &);

} // namespace ns_starpoint
} // namespace ns_sp9500

#define DECLARE_R1A_SETLOREG_FUNCTION_MAP   \
    r1a_set_lo_reg set_r1a_lo_reg[4] = {    \
        &sp1401_r1a::set_tx_lo1_reg,        \
        &sp1401_r1a::set_tx_lo2_reg,        \
        &sp1401_r1a::set_rx_lo1_reg,        \
        &sp1401_r1a::set_rx_lo2_reg };

#define DECLARE_R1A_DETLOLOCK_FUNCTION_MAP  \
    r1a_det_lo_lock det_r1a_lo_lock[4] = {  \
        &sp1401_r1a::det_tx_lo1_lock,       \
        &sp1401_r1a::det_tx_lo2_lock,       \
        &sp1401_r1a::det_rx_lo1_lock,       \
        &sp1401_r1a::det_rx_lo2_lock };

#endif
