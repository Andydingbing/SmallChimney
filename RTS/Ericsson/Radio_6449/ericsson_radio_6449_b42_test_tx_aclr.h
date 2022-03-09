#ifndef ERICSSON_RADIO_6449_B42_TEST_TX_ACLR_WIDGET_H
#define ERICSSON_RADIO_6449_B42_TEST_TX_ACLR_WIDGET_H

#include "ericsson_radio_6449_b42_global.h"

namespace ns_ericsson {
namespace ns_radio_6449 {

KASE_MODEL(TX_ACLR,"-10M","-5M","5M","10M","Time")

KASE_CONFIG(Test_TX_ACLR,
    QTextEdit, textEditFreq, "Freq",
    QLineEdit, lineEditAtt, "Att Bit")

KASE_RESULT(Test_TX_ACLR,
    QLineEdit, lineEditAtt, "Att Bit")

KASE_WIDGET_PREFIX(Ericsson,Radio_6449_B42,Test_TX_ACLR)
    TX_ACLR_Model model;

KASE_THREAD(Test_TX_ACLR)

KASE_WIDGET_SUFFIX()

} // namespace ns_radio_6449
} // namespace ns_ericsson

#endif
