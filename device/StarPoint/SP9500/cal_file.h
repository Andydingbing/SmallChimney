#ifndef CAL_FILE_R1AB_H
#define CAL_FILE_R1AB_H

#include "report.h"
#include "frontend.h"
#include "../cal_table.h"
#include "cal_table_x9119.h"
#include "cal_table_tx_lol.h"
#include "cal_table_tx_sideband.h"
#include "cal_table_tx_filter.h"
#include "cal_table_tx_pwr.h"
#include "cal_table_tx_att.h"
#include "cal_table_tx_filter_offset.h"
#include "cal_table_rx_filter.h"
#include "cal_table_rx_ref.h"
#include "cal_table_rx_att.h"
#include "cal_table_rx_filter_offset.h"

namespace ns_starpoint {

DECL_CAL_TABLE(sp9500,
    Begin = 0,
    TX_SB = 0,
    TX_LOL,
    TX_ATT_OP,
    TX_PWR_OP, // Default BW 160M
    TX_PWR_IO, // Default BW 160M
    RX_REF,
    X9119,
    TX_ATT_IO,
    RX_REF_OP, // Default BW 160M
    RX_REF_IO, // Default BW 160M
    RX_ATT_OP,
    RX_ATT_IO,
    TX_FILTER_80,
    TX_FILTER_160,
    TX_RF_FR_0,
    TX_RF_FR_1,
    TX_IF_FR,
    RX_FILTER_80,
    RX_FILTER_160,
    RX_RF_FR,
    RX_IF_FR,
    TX_FILTER_OFFSET,
    RX_FILTER_OFFSET,
    TX_TP,
    TX_TC,
    RX_TP,
    RX_TC,
    Total)

namespace ns_sp9500 {

class API cal_file : public basic_report<cal_table_t>
{
public:
    cal_file() : basic_report<cal_table_t>(path()) {}
    cal_file(ns_sp1401::hw_ver_t ver,uint32_t rf_idx,uint32_t rfu_idx) {}

    int32_t open()
    {
        basic_report<cal_table_t>::open();
        make_sure_has(cal_table_t::X9119,&_x9119);
        make_sure_has(cal_table_t::TX_LOL,&_tx_lol);
        make_sure_has(cal_table_t::TX_SB,&_tx_sideband);
        make_sure_has(cal_table_t::TX_RF_FR_0,&_tx_rf_fr_0);
        make_sure_has(cal_table_t::TX_RF_FR_1,&_tx_rf_fr_1);
        make_sure_has(cal_table_t::TX_IF_FR,&_tx_if_fr);
        make_sure_has(cal_table_t::TX_FILTER_80,&_tx_filter_80);
        make_sure_has(cal_table_t::TX_FILTER_160,&_tx_filter_160);
        make_sure_has(cal_table_t::TX_PWR_OP,&_tx_pwr_output);
        make_sure_has(cal_table_t::TX_PWR_IO,&_tx_pwr_io);
        make_sure_has(cal_table_t::TX_ATT_OP,&_tx_att_output);
        make_sure_has(cal_table_t::TX_ATT_IO,&_tx_att_io);
        make_sure_has(cal_table_t::TX_FILTER_OFFSET,&_tx_filter_offset);
        make_sure_has(cal_table_t::RX_RF_FR,&_rx_rf_fr);
        make_sure_has(cal_table_t::RX_IF_FR,&_rx_if_fr);
        make_sure_has(cal_table_t::RX_FILTER_80,&_rx_filter_80);
        make_sure_has(cal_table_t::RX_FILTER_160,&_rx_filter_160);
        make_sure_has(cal_table_t::RX_REF_OP,&_rx_ref_output);
        make_sure_has(cal_table_t::RX_REF_IO,&_rx_ref_io);
        make_sure_has(cal_table_t::RX_ATT_OP,&_rx_att_output);
        make_sure_has(cal_table_t::RX_ATT_IO,&_rx_att_io);
        make_sure_has(cal_table_t::RX_FILTER_OFFSET,&_rx_filter_offset);
        return 0;
    }

    std::string path() const
    {
    #ifdef PLATFORM_WIN32
        return "C:\\RTS\\StarPoint\\SP9500\\";
    #elif defined RD_PLAT_FORM_LINUX
        return "";
    #endif
    }

    ns_sp1401::bw_t bw() const { return _bw; }
    ns_sp1401::hw_ver_t hw_ver() const { return _hw_ver; }
    uint32_t ordinal() const { return _ordinal; }

    void set_bw(ns_sp1401::bw_t bw) { _bw = bw; }
    void set_hw_ver(ns_sp1401::hw_ver_t ver) { _hw_ver = ver; }

    uint32_t _ordinal;
    ns_sp1401::bw_t _bw;
    ns_sp1401::hw_ver_t _hw_ver;

    x9119_table_t            _x9119;
    tx_lol_table_t           _tx_lol;
    tx_sideband_table_t      _tx_sideband;
    freq_response_table_t    _tx_rf_fr_0;
    freq_response_table_t    _tx_rf_fr_1;
    freq_response_table_t    _tx_if_fr;
    tx_filter_table_t        _tx_filter_80;
    tx_filter_table_t        _tx_filter_160;
    tx_pwr_table_t           _tx_pwr_output;
    tx_pwr_table_t           _tx_pwr_io;
    tx_att_op_table_t        _tx_att_output;
    tx_att_io_table_t        _tx_att_io;
    tx_filter_offset_table_t _tx_filter_offset;
    freq_response_table_t    _rx_rf_fr;
    freq_response_table_t    _rx_if_fr;
    rx_filter_80_table_t     _rx_filter_80;
    rx_filter_160_table_t    _rx_filter_160;
    rx_ref_table_t           _rx_ref_output;
    rx_ref_table_t           _rx_ref_io;
    rx_att_op_table_t        _rx_att_output;
    rx_att_io_table_t        _rx_att_io;
    rx_filter_offset_table_t _rx_filter_offset;
};

} // namespace ns_sp9500
} // namespace ns_starpoint

#endif
