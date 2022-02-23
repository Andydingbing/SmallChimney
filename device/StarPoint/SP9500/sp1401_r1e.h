#ifndef SP1401_R1E_H
#define SP1401_R1E_H

#include "sp1401_r1c.h"

namespace rd {

class API sp1401_r1e : public sp1401_r1c
{
public:
    typedef boost::shared_ptr<sp1401_r1e> sptr;
    sp1401_r1e(uint32_t rf_idx,uint32_t rfu_idx) :
        sp1401_r1c(rf_idx,rfu_idx) {}

public:
    enum tx_io_sw1_t {
        TX_OFF_1 = 0,
        TX_ON_1,
        TX_LOOP_ON,
        TX_IO_ON
    };

    struct gpio_a_t {
        unsigned pd_rxif12_txif2 : 1;
        unsigned pd_rxcomb_other : 1;
        unsigned op_green_led	 : 1;
        unsigned op_red_led		 : 1;
        unsigned tx_io_sw2		 : 1;
        unsigned tx_io_sw1		 : 2;
        unsigned io_green_led	 : 1;
        unsigned rsv			 : 24;
    public:
        gpio_a_t();
    };

    struct gpio_b_t {
        unsigned io_red_led : 1;
        unsigned rx_io_sw1	  : 1;
        unsigned rx_io_sw2	  : 1;
        unsigned rx_io_sw3	  : 2;
        unsigned loop_sw	  : 1;
        unsigned rx_att019_sw : 1;
        unsigned rx_lnaatt_sw : 1;
        unsigned rsv		  : 24;
    public:
        gpio_b_t();
    };

public:
    int32_t io_mode2io_sw(io_mode_t mode,
                          tx_io_sw1_t &tx_sw1,
                          tx_io_sw2_t &tx_sw2,
                          rx_io_sw1_t &rx_sw1,
                          rx_io_sw2_t &rx_sw2,
                          rx_io_sw3_t &rx_sw3,
                          loop_sw_t &loop_sw,
                          bool &tx_led,
                          bool &rx_led);
    int32_t set_tx_io_sw1(tx_io_sw1_t sw);
    int32_t set_led(bool tx,bool rx);
    int32_t set_io_mode(io_mode_t mode);

    int32_t set_tx_att0(double att);
    int32_t set_tx_att1(double att);
    int32_t set_tx_att2(double att);
    int32_t set_tx_att3(double att);

    int32_t set_rx_att1(double att);
    virtual int32_t set_rx_att2(double att);
};

} // namespace rd

#endif // SP1401_R1E_H
