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

const static float g_temp_star = 20.0f;
const static float g_temp_stop = 70.0f;
const static float g_temp_step = 0.25f;

class API sp1401 : boost::noncopyable
{
public:
    typedef boost::shared_ptr<sp1401> sptr;
    sp1401();
    virtual ~sp1401() {}
public:
    enum rf_ch_t {
        CH_TX = 0,
        CH_RX = 1
	};
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
        PMS_WFT		= 0x1,  //waiting for trigger
        PMS_TT		= 0x2,  //trigger timeout,manual set to IDLE
        PMS_RUNNING = 0x3,  //calculating
        PMS_DONE	= 0x4   //manual set to IDLE
	};

#define DECLARE_SP1401(class_name) \
public: \
    class_name(uint32_t rf_idx,uint32_t rfu_idx); \
    virtual int32_t open_board(); \
    virtual int32_t close_board(); \
    virtual int32_t set_led(bool tx,bool rx); \
    virtual bool    is_connected(); \
    virtual int32_t set_tx_freq(uint64_t freq); \
    virtual int32_t set_tx_att(double att); \
    virtual int32_t set_tx_modulator_en(bool en); \
    virtual int32_t set_pwr_en(bool en); \
    virtual int32_t set_rx_freq(uint64_t freq); \
    virtual int32_t set_io_mode(io_mode_t mode); \
    virtual int32_t get_io_mode(io_mode_t &mode); \

public:
    virtual int32_t open_board();
    virtual int32_t close_board() = 0;
    virtual int32_t set_led(bool tx,bool rx) = 0;
    virtual bool	is_connected() = 0;
    virtual int32_t set_tx_freq(uint64_t freq) = 0;
    virtual int32_t set_tx_att(double att) = 0;
    virtual int32_t set_tx_modulator_en(bool en) = 0;
    virtual int32_t set_pwr_en(bool en) = 0;
    virtual int32_t set_rx_freq(uint64_t freq) = 0;
    virtual int32_t set_io_mode(io_mode_t mode) = 0;
    virtual int32_t get_io_mode(io_mode_t &mode) = 0;

public:
    ns_sp1401::bw_t get_bw() const { return m_cal_file->bw(); }

    ns_sp1401::hw_ver_t get_hw_ver() const { return m_cal_file->hw_ver(); }

    void set_hw_ver(ns_sp1401::hw_ver_t ver) { m_cal_file->set_hw_ver(ver); }

    uint32_t get_rf_idx() const { return m_cal_file->rf_idx(); }

    uint32_t get_rfu_idx() const { return m_cal_file->rfu_idx(); }

    cal_file* cf() const { return m_cal_file.get(); }

    pci_dev_vi* get_k7() const { return m_k7; }

    void set_tx_en_tc(const bool en) { en_tx_tc = en; }
    void set_rx_en_tc(const bool en) { en_rx_tc = en; }
    bool is_tx_en_tc() const { return en_tx_tc; }
    bool is_rx_en_tc() const { return en_rx_tc; }

    void connect(pci_dev_vi *k7) { m_k7 = k7; }
    int32_t get_k7_ver(uint32_t &ver);
    arb_reader *arb() { return m_arb_reader.get(); }

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

    static  uint32_t ass_ordinal(uint32_t ordinal);
    static  int32_t is_valid_sn(const char *sn);
    static  int32_t get_hw_ver(const char *sn,ns_sp1401::hw_ver_t &ver);
    static  std::string hw_ver2str(ns_sp1401::hw_ver_t ver);
    static  std::string hw_ver2sn_header(ns_sp1401::hw_ver_t ver);

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

// Make test/cal reports available.These may throw runtime_exception.
// Test report related functions are named such as "prepare_tr_rf_tx_freq_res_test();"
// Cal report related  functions are named such as "prepare_cr_tx_lol();"
#define DECL_FUNC_PREPARE_TR(name,item_t) \
public: \
    void prepare_tr_##name() { \
        tr_header_t header;get_sn_major(header.sn); \
        header.item = item_t; \
        header.size = sizeof(name##_data); \
        if (_tr_##name == nullptr) \
            _tr_##name = boost::make_shared<name>(#name,&header,true); \
    } \
    name *tr_##name() const \
    { return _tr_##name.get(); } \
protected: \
    boost::shared_ptr<name> _tr_##name;

#define DECL_FUNC_PREPARE_CR(name,item_t) \
public: \
    void prepare_cr_##name() { \
        tr_header_t header;get_sn_major(header.sn); \
        header.item = item_t; \
        header.size = sizeof(name##_data); \
        if (_cr_##name == nullptr) \
            _cr_##name = boost::make_shared<name>(#name,&header,true); \
    } \
    name *cr_##name() const \
    { return _cr_##name.get(); } \
protected: \
    boost::shared_ptr<name> _cr_##name;

    DECL_FUNC_PREPARE_TR(rf_tx_freq_res_test,TI_RF_TX_FREQ_RES)
    DECL_FUNC_PREPARE_TR(if_tx_freq_res_test,TI_IF_TX_FREQ_RES)
    DECL_FUNC_PREPARE_TR(rf_rx_freq_res_test,TI_RF_RX_FREQ_RES)
    DECL_FUNC_PREPARE_TR(if_rx_freq_res_test,TI_IF_RX_FREQ_RES)
    DECL_FUNC_PREPARE_TR(tx_phase_noise_test,TI_TX_PHASE_NOISE)
    DECL_FUNC_PREPARE_TR(tx_noise_floor_test,TI_TX_NOISE_FLOOR)
    DECL_FUNC_PREPARE_TR(tx_lo_ld_test,TI_TX_LO_LD)
    DECL_FUNC_PREPARE_TR(tx_pwr_mod_sw_test,TI_TX_PWR_MOD_SW)
    DECL_FUNC_PREPARE_TR(tx_filter_sw_test,TI_TX_FILTER_SW)
    DECL_FUNC_PREPARE_TR(tx_io_sw_test,TI_TX_IO_SW)

    DECL_FUNC_PREPARE_CR(tx_passband_freq_res_160_cal,TI_TX_PASSBAND_160)
    DECL_FUNC_PREPARE_CR(rx_passband_freq_res_160_cal,TI_RX_PASSBAND_160)
    DECL_FUNC_PREPARE_CR(tx_base_pwr_cal,TI_TX_BASE_PWR)
    DECL_FUNC_PREPARE_CR(tx_pwr_op_cal,TI_TX_PWR_OP)
    DECL_FUNC_PREPARE_CR(tx_pwr_io_cal,TI_TX_PWR_IO)
    DECL_FUNC_PREPARE_CR(rx_ref_cal,TI_RX_REF)
    DECL_FUNC_PREPARE_CR(rx_pwr_op_cal,TI_RX_PWR_OP)
    DECL_FUNC_PREPARE_CR(rx_pwr_io_cal,TI_RX_PWR_IO)

    DECL_FUNC_PREPARE_CR(rf_tx_freq_res_cal,TI_RF_TX_FREQ_RES)
    DECL_FUNC_PREPARE_CR(rf_rx_freq_res_cal,TI_RF_RX_FREQ_RES)

// FTP support.
#define DECL_FUNC_FTP_PUT(report) \
public: \
    int32_t ftp_put##report();

    DECL_FUNC_FTP_PUT(_tr_rf_tx_freq_res_test)
    DECL_FUNC_FTP_PUT(_tr_if_tx_freq_res_test)
    DECL_FUNC_FTP_PUT(_tr_rf_rx_freq_res_test)
    DECL_FUNC_FTP_PUT(_tr_if_rx_freq_res_test)
    DECL_FUNC_FTP_PUT(_tr_tx_phase_noise_test)
    DECL_FUNC_FTP_PUT(_tr_tx_noise_floor_test)
    DECL_FUNC_FTP_PUT(_tr_tx_lo_ld_test)
    DECL_FUNC_FTP_PUT(_tr_tx_pwr_mod_sw_test)
    DECL_FUNC_FTP_PUT(_tr_tx_filter_sw_test)
    DECL_FUNC_FTP_PUT(_tr_tx_io_sw_test)

    DECL_FUNC_FTP_PUT(_cr_tx_passband_freq_res_160_cal)
    DECL_FUNC_FTP_PUT(_cr_rx_passband_freq_res_160_cal)
    DECL_FUNC_FTP_PUT(_cr_tx_base_pwr_cal)
    DECL_FUNC_FTP_PUT(_cr_tx_pwr_op_cal)
    DECL_FUNC_FTP_PUT(_cr_tx_pwr_io_cal)
    DECL_FUNC_FTP_PUT(_cr_rx_ref_cal)
    DECL_FUNC_FTP_PUT(_cr_rx_pwr_op_cal)
    DECL_FUNC_FTP_PUT(_cr_rx_pwr_io_cal)

    template <typename ftp_call_back_t>
    static void set_ftp_retry_call_back(ftp_call_back_t f)
    { _ftp_retry_call_back = f; }

private:
    int32_t w_eeprom(uint16_t addr,uint32_t length,const char *buf);
    int32_t r_eeprom(uint16_t addr,uint32_t length,char *buf);

protected:
    pci_dev_vi *m_k7;
    boost::shared_ptr<cal_file> m_cal_file;
    boost::shared_ptr<arb_reader> m_arb_reader;
    static boost::function<bool()> _ftp_retry_call_back;

    float m_arb_level_offset;
    bool en_tx_tc; // Enable T/Rx power temperature compansate?
    bool en_rx_tc;
};

#endif
