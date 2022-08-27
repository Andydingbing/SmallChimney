#ifndef SP1401_R1F_H
#define SP1401_R1F_H

#include "sp1401_r1e.h"

namespace ns_starpoint {
namespace ns_sp9500 {

class API sp1401_r1f : public sp1401_r1e
{
public:
    sp1401_r1f(uint32_t rf_idx,uint32_t rfu_idx) :
        sp1401_r1e(rf_idx,rfu_idx) {}

    void tx_guess_base_state(io_mode_t &mode,data_f_tx_pwr &data) OVERRIDE;
    void rx_guess_base_state(io_mode_t &mode,data_f_rx_ref &data) OVERRIDE;

    int32_t open_board();
    int32_t set_adf5355(lo_t lo,uint64_t freq);

    int32_t tx_freq2lo(
            uint64_t freq,
            uint64_t &lo1,
            uint64_t &lo2,
            uint64_t &lo3,
            tx_filter_t &filter1,
            tx_filter_t &filter2);

    int32_t set_rx_att2(double att) { return set_rx_att3(att); }

    int32_t set_rx_att(double att1,double att2,double att3)
    {
        boost::ignore_unused(att3);
        INT_CHECK(set_rx_att1(att1));
        INT_CHECK(set_rx_att2(att2));
        return 0;
    }

    int32_t get_temp(uint32_t idx,double &temp);
    bool get_tx_avg_temp(double &temp);
    bool get_rx_avg_temp(double &temp);

    void tx_att_states(std::vector<common_atts_t> &states);
    void tx_att_states(std::vector<common_atts_t> &states,std::vector<bool> &checked);

    void rx_att_states(std::vector<common_atts_t> &states);
    void rx_att_states(std::vector<common_atts_t> &states,std::vector<bool> &checked);
};

} // namespace ns_starpoint
} // namespace ns_sp9500

#endif
