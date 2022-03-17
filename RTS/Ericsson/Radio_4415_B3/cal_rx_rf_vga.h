#ifndef ERICSSON_RADIO_4415_B3_CAL_RX_RF_VGA_WIDGET_H
#define ERICSSON_RADIO_4415_B3_CAL_RX_RF_VGA_WIDGET_H

#include "global.h"

namespace ns_ericsson {
namespace ns_radio_4415 {

KASE_MODEL(RX_RF_VGA,"Att","Power","Time")
KASE_CURVE(RX_RF_VGA)

KASE_WIDGET_PREFIX(Cal_RX_RF_VGA)

KASE_CONFIG(
    QTextEdit, textEditFreq, "Freq",
    QLineEdit, lineEditAtt, "Att Bit")

KASE_RESULT(
    QCheckBox, checkBoxMonotonic, "Monotonic",
    QLineEdit, lineEditDyRange, "Dynamic Range")

    RX_RF_VGA_Model model;
    RX_RF_VGA_Curve curve;

KASE_THREAD(Cal_RX_RF_VGA)

KASE_WIDGET_SUFFIX()

} // namespace ns_radio_4415
} // namespace ns_ericsson

#endif
