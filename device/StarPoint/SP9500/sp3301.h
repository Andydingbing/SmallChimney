#ifndef SP3301_H
#define SP3301_H

#include "../device.h"
#include "sp1401.h"
#include "sp1401_r1a.h"
#include "sp1401_r1b.h"
#include "sp1401_r1c.h"
#include "sp1401_r1d.h"
#include "sp1401_r1e.h"
#include "sp1401_r1f.h"
#include "sp2401_r1a.h"
#include "../sp9500/dma_mgr.h"

#define SP3301_0 sp3301::instance(0)
#define SP3301_1 sp3301::instance(1)
#define SP3301_2 sp3301::instance(2)
#define SP3301_3 sp3301::instance(3)
#define SP3301_4 sp3301::instance(4)

/*
 * RF_0 RF_1 RF_2 RF_3
 *   |    |    |    |
 *   ------    ------
 *    K7_1      K7_0
 */
#define K7_IDX(idx) ((ns_sp9500::g_max_rf - 1 - idx) / 2)

/*
 * RF_0 RF_1 RF_2 RF_3
 *   |    |    |    |
 *   ------    ------
 *   brother   brother
 */
static SYM_INLINE bool is_brother_l_r(uint32_t index_l,uint32_t index_r)
{ return index_r - index_l == 1 ? (index_l != 1 ? true : false) : false; }

static SYM_INLINE bool is_brother_r_l(uint32_t index_r,uint32_t index_l)
{ return is_brother_l_r(index_l,index_r); }

static SYM_INLINE uint32_t brother_idx(uint32_t index)
{ return index / 2 * 2 + (index + 1) % 2; }


namespace ns_starpoint {
namespace ns_sp9500 {

class API sp3301 : public radio_base
{
public:
    struct active_t {
        bool k7_0;
        bool k7_1;
        bool s6;
        bool sp1401[ns_sp9500::g_max_rf];

        active_t();
    };

    struct API rfu_info_t {
        char k7_0[32];
        char k7_1[32];
        char s6[32];
        char rsv0[32];
        char sn[128];
        uint32_t k7_0_ver;
        uint32_t k7_1_ver;
        uint32_t s6_ver;
    };

    sp3301(uint32_t rfu_idx);
    virtual ~sp3301();
    static sp3301 &instance(uint32_t rfu_idx);

public:
    uint32_t channels() OVERRIDE;

    int32_t set_sn(const uint32_t index,const char *sn) OVERRIDE;
    int32_t get_sn(const uint32_t index,char *sn) OVERRIDE;

    const item_table_base* data_base(const uint32_t index,const int32_t kase) const OVERRIDE;
    int32_t data_base_add(const uint32_t index,const int32_t kase,void *data) OVERRIDE;
    int32_t prepare_kase(const uint32_t index,const int32_t kase,const std::string freq_str,const bool is_exp) OVERRIDE;

    int32_t init() OVERRIDE;

public:
    uint32_t rfu_idx() { return m_rfu_idx; }
    int32_t get_ocxo(uint16_t &value);
    int32_t set_program_bit(char *path,bool k7_0 = true,bool k7_1 = true);
    int32_t program_bit();
    int32_t get_rf_port();
    ns_sp1401::hw_ver_t get_rf_ver(uint32_t index);
    int32_t get_sn(char *sn);
    int32_t get_ver(char *ver);
    static std::string ass_k7_name(uint32_t k7_idx,uint32_t rfu_idx);
    static std::string ass_s6_name(uint32_t rfu_idx);
    static char rf_ver2child_dir(ns_sp1401::hw_ver_t ver0,ns_sp1401::hw_ver_t ver1);

    active_t is_actived();
    rfu_info_t get_info() { return m_rfu_info; }

public:
    int32_t set_tx_en_tc(const uint32_t index,const bool en);
    int32_t set_tx_state(uint32_t index,bool state);
    int32_t set_tx_pwr(uint32_t index,float pwr);
    int32_t set_tx_freq(uint32_t index,uint64_t freq);
    int32_t get_tx_freq(uint32_t index,uint64_t &freq);
    int32_t set_tx_bw(uint32_t index,ns_sp1401::bw_t bw);
    int32_t set_tx_src(uint32_t index,sp2401_r1a::da_src_t src);
    int32_t set_src_freq(uint32_t index,uint64_t freq);

    int32_t arb_load(uint32_t index,const char *path);
    int32_t set_arb_en(uint32_t index,bool en);
    int32_t set_arb_cnt(uint32_t index,int cnt);

    int32_t set_rx_en_tc(const uint32_t index,const bool en);
    int32_t set_rx_level(uint32_t index,double level);
    int32_t set_rx_freq(uint32_t index,uint64_t freq);
    int32_t get_rx_freq(uint32_t index,uint64_t &freq);
    int32_t set_rx_bw(uint32_t index,ns_sp1401::bw_t bw);

    int32_t set_io_mode(uint32_t index,io_mode_t mode);
    int32_t get_temp(uint32_t index,double &tx_temp,double &rx_temp);
    int32_t get_cal_temp(uint32_t index,double &temp);

    int32_t init_pwr_meas(uint32_t index);
    int32_t abort_pwr_meas(uint32_t index);
    int32_t get_pwr_meas_proc(uint32_t index,sp1401::pwr_meas_state_t &proc);
    int32_t get_pwr_meas_result(uint32_t index,float &pwr,float &crest);

    int32_t set_iq_cap_samples(uint32_t index,uint32_t samples);
    int32_t get_iq_cap_samples(const uint32_t index,uint32_t &samples) const;
    int32_t set_iq_cap_buffer(uint32_t index,int16_t *I,int16_t *Q);
    int32_t get_iq_cap_buffer(uint32_t index,int16_t **I,int16_t **Q);
    int32_t set_iq_cap_trig_src(uint32_t index,sp1401::iq_cap_src_t src);
    int32_t set_iq_cap_trig_level(uint32_t index,float level);
    int32_t set_iq_cap_frame_trig_src(uint32_t index,sp2401_r1a::frame_trig_src_t src);
    int32_t set_iq_cap_frame_trig_frame(uint32_t index,uint32_t frame);
    int32_t set_iq_cap_frame_trig_offset(uint32_t index,int32_t offset);
    int32_t set_iq_cap_frame_trig_mod_x_y(uint32_t index,uint16_t x,uint16_t y);
    int32_t iq_cap(uint32_t index);
    int32_t iq_cap_abort(uint32_t index);
    int32_t iq_cap_iq2buf(uint32_t index);
    int32_t iq_cap_iq2buf(uint32_t index,uint32_t samples);

    int32_t self_cal_tx_lol(uint32_t index);
    int32_t self_cal_tx_sb(uint32_t index);

public:
    template<typename int_t,typename data_type = traits_int<int_t>>
    sp1401 *working_sp1401(int_t index) { return _sp1401[index]; }

    template<typename int_t,typename data_type = traits_int<int_t>>
    sp2401_r1a *working_sp2401(int_t index) { return _sp2401_r1a[index]; }

    sp1401_r1a *working_sp1401_r1a(uint32_t index) { return _sp1401_r1a[index]; }
    sp1401_r1b *working_sp1401_r1b(uint32_t index) { return _sp1401_r1a[index]; }
    sp1401_r1c *working_sp1401_r1c(uint32_t index) { return _sp1401_r1c[index]; }
    sp1401_r1d *working_sp1401_r1d(uint32_t index) { return _sp1401_r1c[index]; }
    sp1401_r1e *working_sp1401_r1e(uint32_t index) { return _sp1401_r1e[index]; }
    sp1401_r1f *working_sp1401_r1f(uint32_t index) { return _sp1401_r1f[index]; }

    dma_mgr *ddr(uint32_t index) { return _dma_mgr[K7_IDX(index)]; }

private:
    int32_t instance_sp1401(uint32_t index);

private:
    std::array<sp1401 *,g_max_rf> _sp1401;
    std::array<sp1401_r1a *,g_max_rf> _sp1401_r1a;
    std::array<sp1401_r1c *,g_max_rf> _sp1401_r1c;
    std::array<sp1401_r1e *,g_max_rf> _sp1401_r1e;
    std::array<sp1401_r1f *,g_max_rf> _sp1401_r1f;
    std::array<sp2401_r1a *,g_max_rf> _sp2401_r1a;

    std::array<dma_mgr *,g_max_rf/2> _dma_mgr;

    std::array<int16_t *,g_max_rf> _I;
    std::array<int16_t *,g_max_rf> _Q;

    pci_dev_vi m_k7_0;
    pci_dev_vi m_k7_1;
    pci_dev_vi m_s6;

    rfu_info_t m_rfu_info;
    uint32_t m_rfu_idx;

private:
    io_mode_t::_integral m_io_mode[ns_sp9500::g_max_rf];
    uint64_t m_tx_freq[ns_sp9500::g_max_rf];
    uint64_t m_rx_freq[ns_sp9500::g_max_rf];
    double m_tx_pwr[ns_sp9500::g_max_rf];
    double m_ref[ns_sp9500::g_max_rf];
    bool m_is_program_k7_0;
    bool m_is_program_k7_1;
    char m_bit_path[128];
    active_t m_active;

    static double temp_cal_star;
    static double temp_cal_stop;
    static double tx_tc_coef[12][6];
    static double rx_tc_coef[12][6];
};

} // namespace ns_starpoint
} // namespace ns_sp9500

#endif
