#ifndef TEST_TX_ACLR_WIDGET_H
#define TEST_TX_ACLR_WIDGET_H

#include "global.h"

KASE_MODEL(TX_ACLR,"-10M","-5M","5M","10M","Time")

KASE_WIDGET_PREFIX(Test_TX_ACLR)

KASE_CONFIG("Freq",    QTextEdit)
KASE_CONFIG("Att Bit", QLineEdit)

KASE_RESULT("Att Bit", QLineEdit)

KASE_WIDGET :
TX_ACLR_Model model;

KASE_THREAD(Test_TX_ACLR)

KASE_WIDGET_SUFFIX()

#endif
