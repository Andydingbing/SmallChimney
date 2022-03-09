#ifndef ERICSSON_RADIO_6449_B42_CAL_TX_VGA_WIDGET_H
#define ERICSSON_RADIO_6449_B42_CAL_TX_VGA_WIDGET_H

#include "ericsson_radio_6449_b42_global.h"

namespace ns_ericsson {
namespace ns_radio_6449 {

KASE_MODEL(TX_VGA,"DAC","Range","Time")
KASE_CURVE(TX_VGA)

KASE_CONFIG(Cal_TX_VGA,
    QTextEdit, textEditFreq, "Freq",
    QLineEdit, lineEditAtt, "")

KASE_RESULT(Cal_TX_VGA,
    QLineEdit, lineEditDyRange, "Dynamic Range")

KASE_WIDGET_PREFIX(Ericsson,Radio_6449_B42,Cal_TX_VGA)
    TX_VGA_Model model;
    TX_VGA_Curve curve;

KASE_THREAD(Cal_TX_VGA)

KASE_WIDGET_SUFFIX()

} // namespace ns_radio_6449
} // namespace ns_ericsson

#endif
