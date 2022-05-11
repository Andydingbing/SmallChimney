#ifndef TEST_RX_GAIN_ACCURACY_WIDGET_H
#define TEST_RX_GAIN_ACCURACY_WIDGET_H

#include "global.h"

KASE_MODEL(RX_Gain_Accu,"Branch","Power","Time")

KASE_PREFIX(Test_RX_Gain_Accuracy)

KASE_CONFIG("Freq", QTextEdit)
KASE_CONFIG("Att Bit", QLineEdit)

KASE_RESULT("Accuracy", QLineEdit)

KASE_WIDGET :
RX_Gain_Accu_Model model;

KASE_THREAD :

KASE_SUFFIX()

#endif
