#ifndef CAL_RX_RF_VGA_WIDGET_H
#define CAL_RX_RF_VGA_WIDGET_H

#include "global.h"

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

#endif
