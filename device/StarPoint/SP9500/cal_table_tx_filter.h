#ifndef DEVICE_STARPOINT_SP9500_CAL_TABLE_TX_FILTER_H
#define DEVICE_STARPOINT_SP9500_CAL_TABLE_TX_FILTER_H

#include "cal_table_freq_response.h"
#include "cal_table_filter.h"
#include "../cal_table.h"

namespace ns_starpoint {
namespace ns_sp9500 {

typedef data_f_filter<ns_sp2401::dl_filter_tap_2i,4> data_f_tx_filter;
typedef data_m_filter<ns_sp2401::dl_filter_tap_2i>   data_m_tx_filter;

typedef filter_table_t<data_f_tx_filter,data_m_tx_filter,ns_sp2401::dl_filter_tap_2i> tx_filter_table_t;

} // namespace ns_sp9500
} // namespace ns_starpoint

#endif
