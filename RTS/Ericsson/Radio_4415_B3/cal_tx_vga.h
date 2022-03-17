#ifndef ERICSSON_RADIO_4415_B3_CAL_TX_VGA_WIDGET_H
#define ERICSSON_RADIO_4415_B3_CAL_TX_VGA_WIDGET_H

#include "global.h"

namespace ns_ericsson {
namespace ns_radio_4415 {

KASE_MODEL(TX_VGA,"DAC","Range","Time")
KASE_CURVE(TX_VGA)

KASE_WIDGET_PREFIX(Cal_TX_VGA)

KASE_CONFIG(
    QTextEdit, textEditFreq, "Freq",
    QLineEdit, lineEditAtt, "")

KASE_RESULT(
    QLineEdit, lineEditDyRange, "Dynamic Range")

    TX_VGA_Model model;
    TX_VGA_Curve curve;

KASE_THREAD(Cal_TX_VGA)

KASE_WIDGET_SUFFIX()

} // namespace ns_radio_4415
} // namespace ns_ericsson

#endif