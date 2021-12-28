#ifndef CAL_TABLE_H
#define CAL_TABLE_H

#include <time.h>
#include "liblog.h"
#include "libdriver.h"
#include "global_header.h"

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
    };
};

struct common_tmps_t {
    float temp[4];
};

#endif // CAL_TABLE_H
