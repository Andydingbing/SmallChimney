#ifndef CAL_FILE_R1AB_H
#define CAL_FILE_R1AB_H

#include "report.h"
#include "frontend.h"
#include "../cal_table.h"
#include "cal_table_x9119.h"
#include "cal_table_tx_lol.h"
#include "cal_table_tx_sideband.h"
#include "cal_table_tx_pwr.h"
#include "cal_table_tx_att.h"

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
    TX_FILTER_OFFSET_80,
    RX_FILTER_OFFSET_80,
    TX_TP,
    TX_TC,
    RX_TP,
    RX_TC,
    Total)

namespace ns_sp9500 {

#define DECL_ITEM(...) \
public: \
    cal_file() : basic_report<cal_table_t>(path()) {} \
    const item_table_base *db(const uint32_t table) const \
    { \
        switch (table) { \
        PARAM_PICK_2_OF_3_1_3(,case cal_table_t::,: return &,;,__VA_ARGS__) \
        default : return nullptr; \
        } \
    } \
    PARAM_PICK_2_OF_3_2(,,,;,__VA_ARGS__)



class API cal_file : public basic_report<cal_table_t>
{
    DECL_ITEM(
        X9119,     x9119_table_t,       _x9119,
        TX_LOL,    tx_lol_table_t,      _tx_lol,
        TX_SB,     tx_sideband_table_t, _tx_sideband,
        TX_PWR_OP, tx_pwr_table_t,      _tx_pwr_output,
        TX_PWR_IO, tx_pwr_table_t,      _tx_pwr_io,
        TX_ATT_OP, tx_att_op_table_t,   _tx_att_output,
        TX_ATT_IO, tx_att_io_table_t,   _tx_att_io)

    cal_file(ns_sp1401::hw_ver_t ver,uint32_t rf_idx,uint32_t rfu_idx);

    int32_t open()
    {
        basic_report<cal_table_t>::open();
        make_sure_has(cal_table_t::X9119,&_x9119);
        make_sure_has(cal_table_t::TX_LOL,&_tx_lol);
        make_sure_has(cal_table_t::TX_SB,&_tx_sideband);
        make_sure_has(cal_table_t::TX_PWR_OP,&_tx_pwr_output);
        make_sure_has(cal_table_t::TX_PWR_IO,&_tx_pwr_io);
        make_sure_has(cal_table_t::TX_ATT_OP,&_tx_att_output);
        make_sure_has(cal_table_t::TX_ATT_IO,&_tx_att_io);
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

    ns_sp1401::bw_t bw() { return _bw; }
    ns_sp1401::hw_ver_t hw_ver() { return _hw_ver; }
    uint32_t rf_idx()  { return _rf_idx; }
    uint32_t rfu_idx() { return _rfu_idx; }
    uint32_t ordinal() { return _ordinal; }

    void set_bw(ns_sp1401::bw_t bw) { _bw = bw; }
    void set_hw_ver(ns_sp1401::hw_ver_t ver) { _hw_ver = ver; }

    uint32_t _rf_idx;
    uint32_t _rfu_idx;
    uint32_t _ordinal;
    ns_sp1401::bw_t _bw;
    ns_sp1401::hw_ver_t _hw_ver;
};

} // namespace ns_sp9500
} // namespace ns_starpoint

#endif
