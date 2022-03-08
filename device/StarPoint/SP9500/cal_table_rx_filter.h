#ifndef DEVICE_STARPOINT_SP9500_CAL_TABLE_RX_FILTER_H
#define DEVICE_STARPOINT_SP9500_CAL_TABLE_RX_FILTER_H

#include "cal_table_freq_response.h"
#include "cal_table_filter.h"
#include "../cal_table.h"

namespace ns_starpoint {
namespace ns_sp9500 {

typedef data_f_filter<ns_sp2401::ul_filter_tap,4> data_f_rx_filter_80;
typedef data_m_filter<ns_sp2401::ul_filter_tap>   data_m_rx_filter_80;

typedef data_f_filter<ns_sp2401::ul_filter_tap_2i3d,4> data_f_rx_filter_160;
typedef data_m_filter<ns_sp2401::ul_filter_tap_2i3d>   data_m_rx_filter_160;

typedef filter_table_t<data_f_rx_filter_80, data_m_rx_filter_80, ns_sp2401::ul_filter_tap>      rx_filter_80_table_t;
typedef filter_table_t<data_f_rx_filter_160,data_m_rx_filter_160,ns_sp2401::ul_filter_tap_2i3d> rx_filter_160_table_t;

} // namespace ns_sp9500
} // namespace ns_starpoint

#endif
