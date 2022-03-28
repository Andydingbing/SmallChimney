#ifndef TEST_RX_GAIN_ACCURACY_WIDGET_H
#define TEST_RX_GAIN_ACCURACY_WIDGET_H

#include "global.h"

KASE_MODEL(RX_Gain_Accu,"Branch","Power","Time")

KASE_WIDGET_PREFIX(Test_RX_Gain_Accuracy)

KASE_CONFIG(
    QTextEdit, textEditFreq, "Freq",
    QLineEdit, lineEditAtt, "Att Bit")

KASE_RESULT(
    QLineEdit, lineEditAccu, "Accuracy")

    RX_Gain_Accu_Model model;

KASE_THREAD(Test_RX_Gain_Accuracy)

KASE_WIDGET_SUFFIX()

#endif
