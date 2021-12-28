#ifndef SP1403_R1B_H
#define SP1403_R1B_H

#include "sp1403_r1a.h"

namespace ns_sp1403 {
namespace r1b {
BETTER_ENUM(tx_sw2_t, uint32_t, _3000_6000, _6000_8000)
BETTER_ENUM(rx_bw_t,  uint32_t, _100M, _200M, _400M, _800M)
BETTER_ENUM(rx_sw7_t, uint32_t, _300_1000, _5100)
BETTER_ENUM(det_sw_t, uint32_t, TX, RX)

} // namespace r1b
} // namespace ns_sp1403

class API sp1403_r1b : virtual public sp1403_r1a
{
public:
    typedef ns_sp1403::r1b::rx_bw_t  rx_bw_t;
    typedef ns_sp1403::r1b::rx_sw7_t rx_sw7_t;
    typedef ns_sp1403::r1b::det_sw_t det_sw_t;

    sp1403_r1b();

    virtual int32_t set_rx_sw7(const rx_sw7_t &sw) const = 0;
    virtual int32_t get_rx_sw7(rx_sw7_t &sw) const = 0;

    virtual int32_t set_rx_bw(const rx_bw_t) const = 0;
    virtual int32_t get_rx_bw(rx_bw_t &) const = 0;

    virtual int32_t set_det_sw(const det_sw_t &sw) const = 0;
    virtual int32_t get_det_sw(det_sw_t &sw) const = 0;
    virtual int32_t get_ad7680(uint16_t &det) const = 0;
};

#endif // SP1403_R1B_H
