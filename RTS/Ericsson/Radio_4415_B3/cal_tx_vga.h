#ifndef CAL_TX_VGA_WIDGET_H
#define CAL_TX_VGA_WIDGET_H

#include "global.h"

KASE_MODEL(TX_VGA,"DAC","Range","Time")
KASE_CURVE(TX_VGA)

KASE_WIDGET_PREFIX(Cal_TX_VGA)

KASE_CONFIG("Freq", QTextEdit)
KASE_CONFIG("",     QLineEdit)

KASE_RESULT("Dynamic Range", QLineEdit)

KASE_WIDGET :
TX_VGA_Model model;
TX_VGA_Curve curve;

KASE_THREAD(Cal_TX_VGA)

KASE_WIDGET_SUFFIX()

#endif
