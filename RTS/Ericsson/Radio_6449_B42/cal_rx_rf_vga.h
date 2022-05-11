#ifndef CAL_RX_RF_VGA_WIDGET_H
#define CAL_RX_RF_VGA_WIDGET_H

#include "global.h"

KASE_MODEL(RX_RF_VGA,"Att","Power","Time")
KASE_CURVE(RX_RF_VGA)

KASE_PREFIX(Cal_RX_RF_VGA)

KASE_CONFIG("Freq",    QTextEdit)
KASE_CONFIG("Att Bit", QLineEdit)

KASE_RESULT("Monotonic",     QCheckBox)
KASE_RESULT("Dynamic Range", QLineEdit)

KASE_WIDGET :
RX_RF_VGA_Model model;
RX_RF_VGA_Curve curve;

KASE_THREAD :

KASE_SUFFIX()

#endif
