#ifndef SP1403_R1A_H
#define SP1403_R1A_H

#include "sp1403.h"

namespace ns_sp1403 {
namespace r1a {
// tx
BETTER_ENUM(tx_sw1_t,   uint32_t, _300_3000,  _3000_8000)
BETTER_ENUM(tx_sw2_t,   uint32_t, _300_3000,  _3000_8000)
BETTER_ENUM(tx_sw3_t,   uint32_t, _6000_8000, _3000_6000)
BETTER_ENUM(tx_sw4_t,   uint32_t, _4800_6000, _3000_4800)
BETTER_ENUM(tx0_sw5_t,  uint32_t, TO_OUT,      TO_IO)
BETTER_ENUM(tx0_sw6_t,  uint32_t, TO_LOOP,     TO_OUT)
BETTER_ENUM(tx0_sw7_t,  uint32_t, TO_OFF,      TO_OUT)
BETTER_ENUM(tx0_sw8_t,  uint32_t, TO_OFF,      TO_LOOP)
BETTER_ENUM(tx0_sw9_t,  uint32_t, TO_TX1_LOOP, TO_TX0_IN)
BETTER_ENUM(tx0_sw10_t, uint32_t, TO_RX,       TO_TX0_LOOP)
BETTER_ENUM(tx0_sw11_t, uint32_t, TO_RX,       TO_OFF)
BETTER_ENUM(tx0_sw12_t, uint32_t, TO_RX,       TO_OFF)
BETTER_ENUM(tx1_sw5_t,  uint32_t, TO_LOOP,     TO_OUT)
BETTER_ENUM(tx1_sw6_t,  uint32_t, TO_OFF,      TO_OUT)

// rx
BETTER_ENUM(rx_bw_t,  uint32_t, _400M, _800M)
BETTER_ENUM(rx_sw1_t, uint32_t, TO_ATT, TO_LNA)
BETTER_ENUM(rx_sw2_t, uint32_t, TO_ATT_0, TO_ATT_10)
BETTER_ENUM(rx_sw3_t, uint32_t, _300_1000, _1000_8000)
typedef rx_bw_t rx_sw4_t;

BETTER_ENUM(rx_lna_att_t, uint32_t,
            RX_ATT_0 = 0,
            RX_ATT_10 = 1,
            Reserved,
            RX_LNA = 3)

BETTER_ENUM(temp_t, uint32_t,
            TX_LO1 = 0,
            TX0_SW = 1,
            TX0_PA = 4,
            RX_LNA = 5,
            TX1_PA = 6)

enum att_t {
    TX0_ATT0,TX0_ATT1,TX0_ATT2,TX0_ATT3,
    TX1_ATT0,TX1_ATT1,TX1_ATT2,TX1_ATT3,
    RX_ATT0,RX_ATT1,ATT_MAX
};
} // namespace r1a
} // namespace ns_sp1403

class API sp1403_r1a : virtual public sp1403
{
public:
    typedef ns_sp1403::r1a::tx_sw1_t   tx_sw1_t;
    typedef ns_sp1403::r1a::tx_sw2_t   tx_sw2_t;
    typedef ns_sp1403::r1a::tx_sw3_t   tx_sw3_t;
    typedef ns_sp1403::r1a::tx_sw4_t   tx_sw4_t;
    typedef ns_sp1403::r1a::tx0_sw5_t  tx0_sw5_t;
    typedef ns_sp1403::r1a::tx0_sw6_t  tx0_sw6_t;
    typedef ns_sp1403::r1a::tx0_sw7_t  tx0_sw7_t;
    typedef ns_sp1403::r1a::tx0_sw8_t  tx0_sw8_t;
    typedef ns_sp1403::r1a::tx0_sw9_t  tx0_sw9_t;
    typedef ns_sp1403::r1a::tx0_sw10_t tx0_sw10_t;
    typedef ns_sp1403::r1a::tx0_sw11_t tx0_sw11_t;
    typedef ns_sp1403::r1a::tx0_sw12_t tx0_sw12_t;
    typedef ns_sp1403::r1a::tx1_sw5_t  tx1_sw5_t;
    typedef ns_sp1403::r1a::tx1_sw6_t  tx1_sw6_t;

    typedef ns_sp1403::r1a::rx_bw_t  rx_bw_t;
    typedef ns_sp1403::r1a::rx_sw1_t rx_sw1_t;
    typedef ns_sp1403::r1a::rx_sw2_t rx_sw2_t;
    typedef ns_sp1403::r1a::rx_sw3_t rx_sw3_t;
    typedef ns_sp1403::r1a::rx_sw4_t rx_sw4_t;
    typedef ns_sp1403::r1a::rx_lna_att_t rx_lna_att_t;

    typedef ns_sp1403::r1a::att_t  att_t;
    typedef ns_sp1403::r1a::temp_t temp_t;

    sp1403_r1a();

    virtual int32_t set_att(const att_t,const double) const = 0;
    virtual int32_t get_att(const att_t,double &) const = 0;

    virtual int32_t set_tx0_sw1(const tx_sw1_t &sw) const = 0;
    virtual int32_t get_tx0_sw1(tx_sw1_t &sw) const = 0;
    virtual int32_t set_tx0_sw2(const tx_sw2_t &sw) const = 0;
    virtual int32_t get_tx0_sw2(tx_sw2_t &sw) const = 0;
    virtual int32_t set_tx0_sw3(const tx_sw3_t &sw) const = 0;
    virtual int32_t get_tx0_sw3(tx_sw3_t &sw) const = 0;
    virtual int32_t set_tx0_sw4(const tx_sw4_t &sw) const = 0;
    virtual int32_t get_tx0_sw4(tx_sw4_t &sw) const = 0;
    virtual int32_t set_tx0_sw5(const tx0_sw5_t &sw) const = 0;
    virtual int32_t get_tx0_sw5(tx0_sw5_t &sw) const = 0;
    virtual int32_t set_tx0_sw6(const tx0_sw6_t &sw) const = 0;
    virtual int32_t get_tx0_sw6(tx0_sw6_t &sw) const = 0;
    virtual int32_t set_tx0_sw7(const tx0_sw7_t &sw) const = 0;
    virtual int32_t get_tx0_sw7(tx0_sw7_t &sw) const = 0;
    virtual int32_t set_tx0_sw8(const tx0_sw8_t &sw) const = 0;
    virtual int32_t get_tx0_sw8(tx0_sw8_t &sw) const = 0;
    virtual int32_t set_tx0_sw9(const tx0_sw9_t &sw) const = 0;
    virtual int32_t get_tx0_sw9(tx0_sw9_t &sw) const = 0;
    virtual int32_t set_tx0_sw10(const tx0_sw10_t &sw) const = 0;
    virtual int32_t get_tx0_sw10(tx0_sw10_t &sw) const = 0;
    virtual int32_t set_tx0_sw11(const tx0_sw11_t &sw) const = 0;
    virtual int32_t get_tx0_sw11(tx0_sw11_t &sw) const = 0;
    virtual int32_t set_tx0_sw12(const tx0_sw12_t &sw) const = 0;
    virtual int32_t get_tx0_sw12(tx0_sw12_t &sw) const = 0;

    virtual int32_t set_tx1_sw1(const tx_sw1_t &sw) const = 0;
    virtual int32_t get_tx1_sw1(tx_sw1_t &sw) const = 0;
    virtual int32_t set_tx1_sw2(const tx_sw2_t &sw) const = 0;
    virtual int32_t get_tx1_sw2(tx_sw2_t &sw) const = 0;
    virtual int32_t set_tx1_sw3(const tx_sw3_t &sw) const = 0;
    virtual int32_t get_tx1_sw3(tx_sw3_t &sw) const = 0;
    virtual int32_t set_tx1_sw4(const tx_sw4_t &sw) const = 0;
    virtual int32_t get_tx1_sw4(tx_sw4_t &sw) const = 0;
    virtual int32_t set_tx1_sw5(const tx1_sw5_t &sw) const = 0;
    virtual int32_t get_tx1_sw5(tx1_sw5_t &sw) const = 0;
    virtual int32_t set_tx1_sw6(const tx1_sw6_t &sw) const = 0;
    virtual int32_t get_tx1_sw6(tx1_sw6_t &sw) const = 0;

    virtual int32_t set_rx_lna_att_sw(const rx_lna_att_t) const = 0;
    virtual int32_t get_rx_lna_att_sw(ns_sp1403::r1a::rx_lna_att_t &) const = 0;

    virtual int32_t set_rx_bw(const rx_bw_t) const = 0;
    virtual int32_t get_rx_bw(rx_bw_t &) const = 0;

    virtual int32_t set_rx_sw1(const rx_sw1_t &sw) const = 0;
    virtual int32_t get_rx_sw1(rx_sw1_t &sw) const = 0;
    virtual int32_t set_rx_sw2(const rx_sw2_t &sw) const = 0;
    virtual int32_t get_rx_sw2(rx_sw2_t &sw) const = 0;
    virtual int32_t set_rx_sw3(const rx_sw3_t &sw) const = 0;
    virtual int32_t get_rx_sw3(rx_sw3_t &sw) const = 0;
    virtual int32_t set_rx_sw4(const rx_sw4_t &sw) const = 0;
    virtual int32_t get_rx_sw4(rx_sw4_t &sw) const = 0;

    virtual int32_t get_temp(const temp_t &,double &) const = 0;

    virtual int32_t init_lo(const lo_t) = 0;
    virtual int32_t set_lo(const lo_t,const uint64_t) = 0;

    virtual int32_t set_lo_reg(const lo_t lo,const uint8_t addr,const uint16_t data) = 0;
    virtual int32_t get_lo_reg(const lo_t lo,const uint8_t addr,uint16_t &data) = 0;

    lo_info_t *lo(const lo_t id) const
    { return (lo_info_t *)(&_tx_lmx2594_0 + id * sizeof(lo_t *)); }

    const lo_info_t *tx_lmx2594_0() const { return &_tx_lmx2594_0; }
    const lo_info_t *tx_lmx2594_1() const { return &_tx_lmx2594_1; }
    const lo_info_t *rx_lmx2594_0() const { return &_rx_lmx2594_0; }

    std::string lo_freq_string(const lo_t id) const
    { return freq_string_from_uint64_t(lo(id)->freq); }

    std::string tx_lmx2594_0_freq_string() const
    { return freq_string_from_uint64_t(_tx_lmx2594_0.freq); }

    std::string tx_lmx2594_1_freq_string() const
    { return freq_string_from_uint64_t(_tx_lmx2594_1.freq); }

    std::string rx_lmx2594_0_freq_string() const
    { return freq_string_from_uint64_t(_rx_lmx2594_0.freq); }

    virtual int32_t set_tx_lmx2594_0_reg(const uint8_t addr,const uint16_t data) = 0;
    virtual int32_t get_tx_lmx2594_0_reg(const uint8_t addr,uint16_t &data) = 0;
    virtual int32_t set_tx_lmx2594_1_reg(const uint8_t addr,const uint16_t data) = 0;
    virtual int32_t get_tx_lmx2594_1_reg(const uint8_t addr,uint16_t &data) = 0;

    virtual int32_t set_rx_lmx2594_0_reg(const uint8_t addr,const uint16_t data) = 0;
    virtual int32_t get_rx_lmx2594_0_reg(const uint8_t addr,uint16_t &data) = 0;

    virtual void tx_freq_to_lo(const uint64_t freq)
    {
        if (freq <= FREQ_G(3)) {
            return;
        }

        _tx_lmx2594_0.freq = FREQ_M(10300);
        _tx_lmx2594_1.freq = (freq + FREQ_M(11500)) / 2;
    }

    virtual void rx_freq_to_lo(const uint64_t freq)
    {
        if (freq <= FREQ_G(1)) {
            return;
        }

        _rx_lmx2594_0.freq = FREQ_M(10800) + freq;

        if (_rx_lmx2594_0.freq > FREQ_M(14500)) {
            _rx_lmx2594_0.freq /= 2;
        }
    }

public:
    lo_info_t _tx_lmx2594_0;
    lo_info_t _tx_lmx2594_1;
    lo_info_t _rx_lmx2594_0;
};

#endif // SP1403_R1A_H
