#ifndef DEVICE_ERICSSON_RADIO_4415_CAL_FILE_H
#define DEVICE_ERICSSON_RADIO_4415_CAL_FILE_H

#include "../report/report.h"
#include "cal_table_tx_aclr.h"
#include "cal_table_tx_vga.h"
#include "cal_table_rx_rf_vga.h"
#include "cal_table_rx_gain_accu.h"

namespace ns_ericsson {

DECL_CAL_TABLE(radio_4415,
    Begin = 0,
    TX_ACLR = 0,
    RX_Gain_Accu,
    TX_VGA,
    RX_RF_VGA,
    Total)

namespace ns_radio_4415 {

class API cal_file : public basic_report<cal_table_t>
{
public:
    cal_file() : basic_report<cal_table_t>(path()) {}
    virtual ~cal_file() {}

    int32_t open()
    {
        basic_report<cal_table_t>::open();
        make_sure_has(cal_table_t::TX_ACLR,&_tx_aclr);
        make_sure_has(cal_table_t::RX_Gain_Accu,&_rx_gain_accu);
        make_sure_has(cal_table_t::TX_VGA,&_tx_vga);
        make_sure_has(cal_table_t::RX_RF_VGA,&_rx_rf_vga);
        return 0;
    }

    std::string path() const;

    tx_aclr_table_t *tx_aclr() { return &_tx_aclr; }
    rx_gain_accu_table_t *rx_gain_accu() { return &_rx_gain_accu; }
    tx_vga_table_t *tx_vga() { return &_tx_vga; }
    rx_rf_vga_table_t *rx_rf_vga() { return &_rx_rf_vga; }

private:
    tx_aclr_table_t _tx_aclr;
    rx_gain_accu_table_t _rx_gain_accu;
    tx_vga_table_t _tx_vga;
    rx_rf_vga_table_t _rx_rf_vga;
};

} // namespace ns_radio_4415
} // namespace ns_ericsson

#endif
