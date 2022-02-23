#ifndef CAL_TABLE_H
#define CAL_TABLE_H

#include <time.h>
#include "liblog.h"
#include "libdriver.h"
#include "global_header.h"

#define DECLARE_CAL_ITEM_TABLE(class_name,pt) \
    public: \
        class_name() { m_data_m = new data_m_t[pt]; } \
        virtual ~class_name() { SAFE_DEL_ARRAY(m_data_m); } \
        void virtual add(data_f_t *data); \
        void map2mem(); \
        void map2buf(data_f_t *buf){ m_data_f = buf; } \
        void save_as(const char *path); \
    private: \
        data_f_t *m_data_f; \
        data_m_t *m_data_m;

#define IMPLEMENT_TX_CAL_ITEM_TABLE(class_name) \
    void class_name::add(data_f_t *data) { \
        uint32_t idx = SERIE_INDEX(data->freq,tx_freq_star,tx_freq_step_infile); \
        m_data_f[idx] = *data; \
    }

#define IMPLEMENT_RX_CAL_ITEM_TABLE(class_name) \
    void class_name::add(data_f_t *data) { \
        uint32_t idx = SERIE_INDEX(data->freq,rx_freq_star,rx_freq_step_infile); \
        m_data_f[idx] = *data; \
    }

#define R1A_TX_ATT_STEP_MIN 10
#define R1A_TX_ATT_DR 90
#define R1A_DGAIN_MAX 8
#define R1A_DGAIN_MIN -4

#define R1C_TX_ATT_STEP -1
#define R1C_DGAIN_MAX 2
#define R1C_DGAIN_MIN -8
#define R1F_DGAIN_MAX -3
#define R1F_DGAIN_MIN -13

#define R1C_TX_ATT_OP_POWER_STAR 10
#define R1C_TX_ATT_OP_POWER_STOP -90

#define R1C_TX_ATT_OP_POWER_PTS \
    (SERIE_SIZE(R1C_TX_ATT_OP_POWER_STAR,R1C_TX_ATT_OP_POWER_STOP,R1C_TX_ATT_STEP) - 1)

#define R1C_TX_ATT_IO_POWER_STAR 0
#define R1C_TX_ATT_IO_POWER_STOP -97

#define R1C_TX_ATT_IO_POWER_PTS \
    (SERIE_SIZE(R1C_TX_ATT_IO_POWER_STAR,R1C_TX_ATT_IO_POWER_STOP,R1C_TX_ATT_STEP) - 1)
// Calibration : TX attenuation
/*****************************/

/***************************/
// Calibration : RX reference
#define R1A_RX_REF_STEP    -1
#define R1A_RX_REF_OP_STAR 30
#define R1A_RX_REF_OP_STOP -40
#define R1A_RX_REF_IO_STAR 30
#define R1A_RX_REF_IO_STOP -30

#define R1C_RX_REF_STEP    -1
#define R1C_RX_REF_OP_STAR 30
#define R1C_RX_REF_OP_STOP -40
#define R1C_RX_REF_IO_STAR 30
#define R1C_RX_REF_IO_STOP -30

#define R1F_RX_REF_STEP    -1
#define R1F_RX_REF_OP_STAR 30
#define R1F_RX_REF_OP_STOP -40
#define R1F_RX_REF_IO_STAR 30
#define R1F_RX_REF_IO_STOP -30

#define R1A_RX_REF_OP_PTS \
    SERIE_SIZE(R1A_RX_REF_OP_STAR,R1A_RX_REF_OP_STOP,R1A_RX_REF_STEP)

#define R1A_RX_REF_IO_PTS \
    SERIE_SIZE(R1A_RX_REF_IO_STAR,R1A_RX_REF_IO_STOP,R1A_RX_REF_STEP)

#define R1C_RX_REF_OP_PTS \
    SERIE_SIZE(R1C_RX_REF_OP_STAR,R1C_RX_REF_OP_STOP,R1C_RX_REF_STEP)

#define R1C_RX_REF_IO_PTS \
    SERIE_SIZE(R1C_RX_REF_IO_STAR,R1C_RX_REF_IO_STOP,R1C_RX_REF_STEP)

#define R1F_RX_REF_OP_PTS \
    SERIE_SIZE(R1F_RX_REF_OP_STAR,R1F_RX_REF_OP_STOP,R1F_RX_REF_STEP)

#define R1F_RX_REF_IO_PTS \
    SERIE_SIZE(R1F_RX_REF_IO_STAR,R1F_RX_REF_IO_STOP,R1F_RX_REF_STEP)

namespace rd {

STRUCT_ALIGN_S(coef_complex_t,2)
    int16_t real : 16;
    int16_t imag : 16;
STRUCT_ALIGN_E(coef_complex_t,2)

namespace ns_sp2401 {
static const uint16_t dl_filter_tap_2i = 41;
static const double   dl_filter_freq_step = 2000000.0;
static const double   dl_filter_160M_freq_star = -83000000.0; // (160 / 2) + 1.5 * 2
static const double   dl_filter_160M_freq_stop = +83000000.0;

static const uint32_t dl_filter_160M_freqs =
        SERIE_SIZE(dl_filter_160M_freq_star,dl_filter_160M_freq_stop,dl_filter_freq_step);

static const double   dl_filter_80M_freq_star = -43000000.0;  // (80  / 2) + 1.5 * 2
static const double   dl_filter_80M_freq_stop = +43000000.0;

static const uint32_t dl_filter_80M_freqs =
        SERIE_SIZE(dl_filter_80M_freq_star,dl_filter_80M_freq_stop,dl_filter_freq_step);

static const uint16_t ul_filter_tap = 96;
static const uint16_t ul_filter_tap_2i3d = 144;
static const double   ul_filter_freq_step = 2000000.0;
static const double   ul_filter_80M_freq_star  = -43000000.0;  // (80  / 2) + 1.5 * 2
static const double   ul_filter_80M_freq_stop  = +43000000.0;
static const double   ul_filter_160M_freq_star = -83000000.0; // (160 / 2) + 1.5 * 2
static const double   ul_filter_160M_freq_stop = +83000000.0;

static const uint32_t ul_filter_80M_freqs =
        SERIE_SIZE(ul_filter_80M_freq_star,ul_filter_80M_freq_stop,ul_filter_freq_step);

static const uint32_t ul_filter_160M_freqs =
        SERIE_SIZE(ul_filter_160M_freq_star,ul_filter_160M_freq_stop,ul_filter_freq_step);
} // namespace ns_sp2401

namespace ns_sp9500 {
static const uint8_t g_max_rfu = 5;
static const uint8_t g_max_rf  = 4; // each RFU
static const uint8_t g_max_fan = 12;

static const uint64_t g_rf_freq_space = 1000000;
static const uint64_t g_0dBFS = 80000000;
} // namespace ns_sp9500

namespace ns_dt3308 {
static const uint64_t g_0dBFS = 100000000;

enum hw_ver_t { HW_ERROR = -1, HW_VER_F = 0, HW_VER_MAX };

namespace f {
    enum rx_lna_att_0_t { RX_LNA_0,RX_ATT_0 };
    enum rx_lna_att_1_t { RX_LNA_1,RX_ATT_1 };
    enum rx_if_filter_t { RX_IF_40M = 0x2,RX_IF_100M = 0x8,RX_IF_160M = 0x7 };
} // namespace f
} // namespace dt3308

namespace ns_sp1403 {
static const uint64_t tx_freq_star = FREQ_M(50);
static const uint64_t tx_freq_sec0 = FREQ_M(3000);
static const uint64_t tx_freq_sec1 = FREQ_M(4800);
static const uint64_t tx_freq_sec2 = FREQ_M(6000);
static const uint64_t tx_freq_sec3 = FREQ_M(7500);
static const uint64_t tx_freq_sec4 = FREQ_M(24250);
static const uint64_t tx_freq_sec5 = FREQ_M(29500);
static const uint64_t tx_freq_sec6 = FREQ_M(37000);
static const uint64_t tx_freq_stop = FREQ_M(43500);

static const uint64_t rx_freq_star = FREQ_M(7500);
} // namespace ns_sp1403

namespace ns_sp2406 {
static const uint16_t dl_cf_tap = 196;
static const uint16_t ul_cf_tap = 196;
} // namespace ns_sp2406

namespace ns_sp9500x {
static const uint32_t g_max_fan = 16;
} // namespace ns_sp9500x
} // namespace rd



namespace ns_sp1401 {

enum hw_ver_t { HW_ERROR = -1, R1A = 0, R1B, R1C, R1D, R1E, R1F, HW_VER_SP9500_MAX };
enum bw_t { _80M, _160M };

#define TX_TROUBLES                 \
        TX_TS_BEGIN = 0x0000,       \
        TX_TS_LO_Leakage_High,      \
        TX_TS_Sideband_High,        \
        TX_TS_Passband_Ripple_High, \
        TX_TS_Transband,            \
        TX_TS_Stopband,             \
        TX_TS_Power_High,           \
        TX_TS_Power_Low,            \
        TX_TS_Att_Offset_High,      \
        TX_TS_Att_Offset_Low,       \
        TX_TS_Output_P1_Low,        \
        TX_TS_IMD3,                 \
        TX_TS_Supurious,            \
        TX_TS_MAX

#define RX_TROUBLES                 \
        RX_TS_BEGIN = 0x7fff,       \
        RX_TS_Passband_Ripple_High, \
        RX_TS_Transband,            \
        RX_TS_Stopband,             \
        RX_TS_Power_High,           \
        RX_TS_Power_Low,            \
        RX_TS_Att_Offset_High,      \
        RX_TS_Att_Offset_Low,       \
        RX_TS_Input_P1_Low,         \
        RX_TS_Output_P1_Low,        \
        RX_TS_Supurious,            \
        RX_TS_MAX

#define DECLARE_TX_TS_ENUM(name) enum name { TX_TROUBLES };
#define DECLARE_RX_TS_ENUM(name) enum name { RX_TROUBLES };

DECLARE_TX_TS_ENUM(tx_trouble_shoot_t)
DECLARE_RX_TS_ENUM(rx_trouble_shoot_t)

struct tx_trouble_shoot_item {
    tx_trouble_shoot_t t;
    bool is_attached;
    tx_trouble_shoot_item() : is_attached(false) {}
    tx_trouble_shoot_item(tx_trouble_shoot_t ts) : t(ts),is_attached(false) {}
    tx_trouble_shoot_item(uint32_t code) : t(tx_trouble_shoot_t(code)),is_attached(false) {}
};

struct rx_trouble_shoot_item {
    rx_trouble_shoot_t t;
    bool is_attached;
    rx_trouble_shoot_item() : is_attached(false) {}
    rx_trouble_shoot_item(rx_trouble_shoot_t ts) : t(ts),is_attached(false) {}
    rx_trouble_shoot_item(uint32_t code) : t(rx_trouble_shoot_t(code)),is_attached(false) {}
};

struct sn_redundant_t {
    char mac_0[32];      // MAC address 0
    char mac_1[32];      // MAC address 1(reserved)
    char sn_rfu[32];     // RFU SN(reserved)
    uint32_t tx_ts_num;  // TX trouble-shoot number
    uint32_t rx_ts_num;  // RX trouble-shoot number
    uint32_t remark_num; // User-definded remark
    sn_redundant_t() : tx_ts_num(0),rx_ts_num(0),remark_num(0) {}
};

static const uint64_t tx_freq_star = 50000000;
static const uint64_t tx_freq_stop = 6000000000;
static const uint64_t tx_freq_step_called = 10000000;
static const uint64_t tx_freq_step_infile = 5000000;
static const uint32_t tx_freqs_called = 596;
static const uint32_t tx_freqs_infile = 1191;

static const uint64_t tx_filter_freq_star_0 = 17000000;   // (100  - 83)MHz
static const uint64_t tx_filter_freq_stop_0 = 3183000000; // (3100 + 83)MHz
static const uint64_t tx_filter_freq_star_1 = 3017000000; // (3100 - 83)MHz
static const uint64_t tx_filter_freq_stop_1 = 6083000000; // (6000 + 83)MHz
static const uint64_t tx_filter_freq_step   = 2000000;

static const uint32_t tx_filter_freqs_0 =
        SERIE_SIZE(tx_filter_freq_star_0,tx_filter_freq_stop_0,tx_filter_freq_step);

static const uint32_t tx_filter_freqs_1 =
        SERIE_SIZE(tx_filter_freq_star_1,tx_filter_freq_stop_1,tx_filter_freq_step);

static const uint32_t tx_filter_freqs = tx_filter_freqs_0 + tx_filter_freqs_1;

static const uint64_t rx_freq_star = 100000000;
static const uint64_t rx_freq_stop = 6000000000;
static const uint64_t rx_freq_step_infile = 5000000;

static const uint32_t rx_freqs_infile = SERIE_SIZE(rx_freq_star,rx_freq_stop,rx_freq_step_infile);

static const uint64_t rx_filter_freq_star = 17000000;   // (100  - 83) * 1000000
static const uint64_t rx_filter_freq_stop = 6083000000; // (6000 + 83) * 1000000
static const uint64_t rx_filter_freq_step = 2000000;

static const uint32_t rx_filter_freqs =
        SERIE_SIZE(rx_filter_freq_star,rx_filter_freq_stop,rx_filter_freq_step);

static const uint8_t g_sn_length = 24;

SYM_INLINE bool is_rf_ver_between(hw_ver_t ver,hw_ver_t first,hw_ver_t second)
{ return (ver >= first && ver <= second); }

SYM_INLINE bool is_rf_ver_before(hw_ver_t ver,hw_ver_t max)
{ return (ver <= max); }

SYM_INLINE bool is_rf_ver_after(hw_ver_t ver,hw_ver_t min)
{ return (ver >= min); }

namespace r1a {
enum tx_pa_att_t { TX_PA,TX_ATT };
enum rx_lna_att_t { RX_ATT,RX_LNA };

static const uint64_t rx_freq_step_called = 5000000;
static const uint32_t rx_freqs_called = 1181;

static const double tx_pwr_star = +10.0;
static const double tx_pwr_stop = -20.0;
static const double tx_pwr_step = -10.0;
} // namespace r1a

namespace r1c {
enum rx_lna_att_t { RX_ATT,RX_LNA };
enum rx_att_019_t { RX_ATT_19,RX_ATT_0 };

static const uint64_t rx_freq_step_called = 10000000;
static const uint32_t rx_freqs_called = 591;

static const double tx_base_pwr_op = -5.0;
static const double tx_base_pwr_io = -12.0;

static const uint64_t tx_temp_comp_freq_step = 500000000;
static const uint32_t tx_temp_comp_freqs = 12; // (6000 - 50) / 500 + 1

static const uint64_t rx_temp_comp_freq_step = 500000000;
static const uint32_t rx_temp_comp_freqs = 12;
} // namespace r1c

namespace r1b = r1a;
namespace r1d = r1c;
namespace r1e = r1c;
namespace r1f = r1c;

} // namespace ns_sp1401

struct lo_info_t {
//    enum lo_type_t{ HIGH,LOW };
    uint64_t freq;
//    lo_type_t type : 8;
//    bool is_opened : 8;
};

struct common_atts_t {
    float att[7];
    float d_gain;

    union {
        int32_t rsv[8];
        struct {
            ns_sp1401::r1a::rx_lna_att_t lna_att;
        }
        rx_sp1401_r1a,
        rx_sp1401_r1b;
        struct {
            ns_sp1401::r1c::rx_lna_att_t lna_att;
            ns_sp1401::r1c::rx_att_019_t att_019;
        }
        rx_sp1401_r1c,
        rx_sp1401_r1d,
        rx_sp1401_r1e,
        rx_sp1401_r1f;
    };
};

struct common_tmps_t {
    float temp[4];
};

#endif
