#ifndef ERICSSON_AIR_3268_B3_CAL_RX_RF_VGA_WIDGET_H
#define ERICSSON_AIR_3268_B3_CAL_RX_RF_VGA_WIDGET_H

#include "ericsson_air_3268_b42_global.h"

namespace ns_ericsson {
namespace ns_air_3268 {

KASE_MODEL(RX_RF_VGA,"Att","Power","Time")
KASE_CURVE(RX_RF_VGA)

KASE_WIDGET_PREFIX(Ericsson,Air_3268_B42,Cal_RX_RF_VGA)

KASE_CONFIG(QTextEdit, textEditFreq, "Freq",
            QLineEdit, lineEditAtt, "Att Bit")

KASE_RESULT(QCheckBox, checkBoxMonotonic, "Monotonic",
            QLineEdit, lineEditDyRange, "Dynamic Range")

    RX_RF_VGA_Model model;
    RX_RF_VGA_Curve curve;

KASE_THREAD(Cal_RX_RF_VGA)

KASE_WIDGET_SUFFIX()

} // namespace ns_air_3268
} // namespace ns_ericsson

#endif
