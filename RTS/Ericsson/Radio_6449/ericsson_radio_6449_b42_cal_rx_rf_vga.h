#ifndef ERICSSON_RADIO_6449_B42_CAL_RX_RF_VGA_WIDGET_H
#define ERICSSON_RADIO_6449_B42_CAL_RX_RF_VGA_WIDGET_H

#include "ericsson_radio_6449_b42_global.h"

namespace ns_ericsson {
namespace ns_radio_6449 {

KASE_MODEL(RX_RF_VGA,"Att","Power","Time")
KASE_CURVE(RX_RF_VGA)

KASE_CONFIG(Cal_RX_RF_VGA,
    QTextEdit, textEditFreq, "Freq",
    QLineEdit, lineEditAtt, "Att Bit")

KASE_RESULT(Cal_RX_RF_VGA,
    QCheckBox, checkBoxMonotonic, "Monotonic",
    QLineEdit, lineEditDyRange, "Dynamic Range")

KASE_WIDGET_PREFIX(Ericsson,Radio_6449_B42,Cal_RX_RF_VGA)
    RX_RF_VGA_Model model;
    RX_RF_VGA_Curve curve;

KASE_THREAD(Cal_RX_RF_VGA)

KASE_WIDGET_SUFFIX()

} // namespace ns_radio_6449
} // namespace ns_ericsson

#endif
