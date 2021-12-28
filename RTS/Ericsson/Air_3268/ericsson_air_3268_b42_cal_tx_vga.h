#ifndef ERICSSON_RADIO_4415_B3_CAL_TX_VGA_WIDGET_H
#define ERICSSON_RADIO_4415_B3_CAL_TX_VGA_WIDGET_H

#include "thread_widget.h"
#include "ui_ericsson_radio_4415_b3_cal_tx_vga.h"
#include "device_ericsson_radio_4415.h"

namespace ns_ericsson {
namespace ns_radio_4415 {

KASE_MODEL(TX_VGA,"DAC","Range","Time")
KASE_CURVE(TX_VGA)

KASE_CONFIG(Cal_TX_VGA,
    QTextEdit, textEditFreq, "Freq",
    QLineEdit, lineEditAtt, "")

KASE_RESULT(Cal_TX_VGA,
    QLineEdit, lineEditDyRange, "Dynamic Range")

KASE_WIDGET_PREFIX(Ericsson,Radio_4415_B3,Cal_TX_VGA)
    TX_VGA_Model model;
    TX_VGA_Curve curve;

KASE_THREAD(Cal_TX_VGA)

KASE_WIDGET_SUFFIX()

} // namespace ns_radio_4415
} // namespace ns_ericsson

#endif // ERICSSON_RADIO_4415_B3_CAL_TX_VGA_WIDGET_H
