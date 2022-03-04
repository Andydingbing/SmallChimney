#ifndef CAL_FILE_H
#define CAL_FILE_H

#include "liblog.h"
#include "libdriver.h"
#include "tx_att_table.h"
#include "rx_ref_table.h"
#include "rx_att_table.h"
#include "tx_filter_table.h"
#include "rx_filter_table.h"
#include "tx_filter_offset_table.h"
#include "rx_filter_offset_table.h"
#include "tx_tc_table.h"
#include <boost/smart_ptr.hpp>
#include <boost/noncopyable.hpp>


class API cal_file : ::noncopyable
{
public:
    cal_file(ns_sp1401::hw_ver_t ver,uint32_t rf_idx,uint32_t rfu_idx);

};

#endif
