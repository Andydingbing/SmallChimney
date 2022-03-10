#ifndef SP1401_H
#define SP1401_H

#define SN_NULL  0	// missing eeprom
#define SN_WRONG 1	// sn read ok but wrong
#define SN_RIGHT 2	// sn right

#include "libdriver.h"
#include "sgpmm.h"
#include "../arb_reader.h"
#include "complex_sequence.h"
#include "../test_data.hpp"
#include "cal_file.h"
#include <list>
#include <boost/function.hpp>
#include "freq_string.hpp"
#include "traits.hpp"
#include "frontend.h"

const static float g_temp_star = 20.0f;
const static float g_temp_stop = 70.0f;
const static float g_temp_step = 0.25f;

namespace ns_starpoint {
namespace ns_sp9500 {

class API sp1401 : public frontend
{
public:
    typedef boost::shared_ptr<sp1401> sptr;
    sp1401();
    virtual ~sp1401() {}

public:
    enum trig_src_t	{
        TS_MANUAL = 1,
        TS_EXT_A  = 4,
        TS_EXT_B  = 5,
        TS_EXT_C  = 6,
        TS_EXT_D  = 7
	};

    enum rep_mode_t {
        RM_SINGLE_SLOT,
        RM_CONTINUOUS
	};

    enum multi_seg_trig_src_t {
        MS_TS_MANUAL
	};

    enum multi_seg_rep_mode_t {
        MS_RM_AUTO,
        MS_RM_CONTINUOUS,
        MS_RM_CONTINUOUS_SEAMLESS
	};

    enum rx_if_filter_t {
		_100MHz,
		_160MHz
	};

    typedef enum pwr_meas_src_t {
        PWR_MEAS_FREE_RUN   = 0x00,
        IQ_CAP_SRC_INT_TRIG = 0X01,
        IQ_CAP_SRC_RST_MKR  = 0x09,
        IQ_CAP_SRC_MKR1,
        IQ_CAP_SRC_MKR2,
        IQ_CAP_SRC_MKR3,
        IQ_CAP_SRC_MKR4,
        IQ_CAP_SRC_USR_MKR,
        IQ_CAP_SRC_EXT_TRIG = 0x11,
        IQ_CAP_SRC_EXT_TRIG1,
        IQ_CAP_SRC_EXT_TRIG2,
        IQ_CAP_SRC_EXT_TRIG3,
        IQ_CAP_SRC_EXT_TRIG4,
        PWR_MEAS_IF_PWR     = 0x21
    } iq_cap_src_t;

    enum pwr_meas_state_t {
        PMS_IDLE	= 0x0,
        PMS_WFT		= 0x1,  // waiting for trigger
        PMS_TT		= 0x2,  // trigger timeout,manual set to IDLE
        PMS_RUNNING = 0x3,  // calculating
        PMS_DONE	= 0x4   // manual set to IDLE
	};

#define DECLARE_SP1401(class_name) \
public: \
    class_name(uint32_t rf_idx,uint32_t rfu_idx); \
    virtual int32_t open_board(); \
    virtual int32_t close_board(); \
    virtual int32_t set_led(bool tx,bool rx); \
    virtual int32_t set_tx_freq(uint64_t freq); \
    virtual int32_t set_tx_att(double att); \
    virtual int32_t set_tx_modulator_en(bool en); \
    virtual int32_t set_pwr_en(bool en); \
    virtual int32_t set_rx_freq(uint64_t freq); \
    virtual int32_t set_io_mode(io_mode_t mode); \
    virtual int32_t get_io_mode(io_mode_t &mode); \

public:
    virtual int32_t open_board() OVERRIDE;
    virtual int32_t set_led(bool tx,bool rx) = 0;
    virtual int32_t set_tx_att(double att) = 0;
    virtual int32_t set_tx_modulator_en(bool en) = 0;
    virtual int32_t set_pwr_en(bool en) = 0;
    virtual int32_t get_io_mode(io_mode_t &mode) = 0;

public:
    ns_sp1401::bw_t bw() const { return _cal_file->bw(); }

    int32_t hw_ver() const OVERRIDE { return _cal_file->hw_ver(); }

    void set_hw_ver(ns_sp1401::hw_ver_t ver) { _cal_file->set_hw_ver(ver); }

    cal_file *data_base() const { return _cal_file; }

    pci_dev_vi* k7() const { return _k7; }

    void connect(pci_dev_vi *k7) { _k7 = k7; }
    int32_t get_k7_ver(uint32_t &ver);
    arb_reader *arb() { return _arb_reader; }

    virtual void tx_att_states(std::vector<common_atts_t> &states) { states.clear(); }
    virtual void tx_att_states(std::vector<common_atts_t> &states,std::vector<bool> &checked)
    { states.clear(); checked.clear(); }

    virtual void rx_att_states(std::vector<common_atts_t> &states) { states.clear(); }
    virtual void rx_att_states(std::vector<common_atts_t> &states,std::vector<bool> &checked)
    { states.clear(); checked.clear(); }

    // supported range
    static const double temp_min;
    static const double temp_max;
    static const double temp_granularity;
    static const uint32_t temps;

    template<typename T = double,typename data_type = float_traits<T>>
    static T temperature_min() { return T(temp_min);  }

    template<typename T = double,typename data_type = float_traits<T>>
    static T temperature_max() { return T(temp_max);  }

    template<typename T = double,typename data_type = float_traits<T>>
    static T temperature_granularity() { return T(temp_granularity); }

    template<typename T = double,typename data_type = float_traits<T>>
    static uint32_t temp_idx(const T temp)
    { return SERIE_INDEX(temp,sp1401::temp_min,sp1401::temp_granularity); }

    template <typename T = double,typename data_type = float_traits<T>>
    static bool is_valid_temperature(const T temp) { return !(temp < temp_min || temp > temp_max); }

    template <typename T = double,typename data_type = float_traits<T>>
    static bool is_invalid_temperature(const T temp) { return temp < temp_min || temp > temp_max; }

     // called range
    template <typename T = double,typename data_type = float_traits<T>>
    static T temperature_star() { return T(20.0); }

    template <typename T = double,typename data_type = float_traits<T>>
    static T temperature_stop() { return T(70.0); }

    template <typename T = double,typename data_type = float_traits<T>>
    static T temperature_step() { return T(0.25); }

    static uint32_t temperatures()
    { return SERIE_SIZE(temperature_star(),temperature_stop(),temperature_step()); }

    static uint32_t ass_ordinal(uint32_t ordinal);
    static int32_t is_valid_sn(const char *sn);
    static int32_t get_hw_ver(const char *sn,ns_sp1401::hw_ver_t &ver);
    static std::string hw_ver2str(ns_sp1401::hw_ver_t ver);
    static std::string hw_ver2sn_header(ns_sp1401::hw_ver_t ver);

    virtual int32_t set_arb_segments(uint16_t segs);
    virtual int32_t set_arb_param(uint32_t add_samp,uint32_t cycle,uint32_t rep_mode);
    virtual int32_t set_arb_trigger(bool retrigger, bool auto_start, uint32_t src, uint32_t trig_delay);
    virtual int32_t set_arb_manual_trigger();
    virtual int32_t set_arb_bc_star_pos(uint32_t pos);
    virtual int32_t set_arb_bc_samples(uint32_t samples);
    virtual uint32_t get_arb_current_seg();
    virtual uint32_t get_arb_current_sample();
    virtual uint32_t get_arb_current_cycle();
    virtual double  get_arb_current_timer();
    virtual int32_t arb_start();
    virtual int32_t arb_stop();

    int32_t set_pwr_meas_src(pwr_meas_src_t src,bool posedge);
    SYM_INLINE int32_t set_iq_cap_src(iq_cap_src_t src,bool posedge)
    { return set_pwr_meas_src(src,posedge); }

    int32_t set_pwr_meas_timeout(uint32_t us);
    SYM_INLINE int32_t set_iq_cap_timeout(uint32_t us)
    { return set_pwr_meas_timeout(us); }

    int32_t is_pwr_meas_timeout(bool &timeout);
    SYM_INLINE int32_t is_iq_cap_timeout(bool &timeout)
    { return is_pwr_meas_timeout(timeout); }

    int32_t set_pwr_meas_trig_threshold(double if_pwr);
    SYM_INLINE int32_t set_iq_cap_trig_threshold(double if_pwr)
    { return set_pwr_meas_trig_threshold(if_pwr); }

    int32_t pwr_meas_start();
    int32_t pwr_meas_abort();
    int32_t set_pwr_meas_samples(uint32_t samples);
    int32_t get_pwr_meas_samples(uint32_t &samples);
    int32_t get_pwr_meas_state(pwr_meas_state_t &state);
    int32_t get_pwr_meas_peak(double &pwr);
    int32_t get_pwr_meas_pwr(double &pwr);
public:
    int32_t set_sn_major(const ns_sp1401::hw_ver_t &ver,const uint32_t &ordinal);
    int32_t get_sn_major(char *sn);
    int32_t set_sn_redundant(const ns_sp1401::sn_redundant_t &info,
                             const std::vector<ns_sp1401::tx_trouble_shoot_t> &tx_ts,
                             const std::vector<ns_sp1401::rx_trouble_shoot_t> &rx_ts,
                             const std::string &remark);
    int32_t get_sn_redundant(ns_sp1401::sn_redundant_t &info,
                             std::vector<ns_sp1401::tx_trouble_shoot_t> &tx_ts,
                             std::vector<ns_sp1401::rx_trouble_shoot_t> &rx_ts,
                             std::string &remark);
    int32_t get_ads5474(int64_t &ad);
    int32_t get_ads5474_manual(int64_t &ad);

    int32_t set_rx_if_filter_sw(rx_if_filter_t filter);
    int32_t set_rx_if_filter_sw(int32_t filter)
    { return set_rx_if_filter_sw(rx_if_filter_t(filter)); }

    int32_t get_ad7680(uint32_t &det);

private:
    int32_t w_eeprom(uint16_t addr,uint32_t length,const char *buf);
    int32_t r_eeprom(uint16_t addr,uint32_t length,char *buf);

protected:
    cal_file *_cal_file;
    pci_dev_vi *_k7;
    arb_reader *_arb_reader;
};

} // namespace ns_starpoint
} // namespace ns_sp9500

#endif
