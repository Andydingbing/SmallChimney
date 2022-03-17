#ifndef ERICSSON_RADIO_4415_B3_TEST_TX_ACLR_WIDGET_H
#define ERICSSON_RADIO_4415_B3_TEST_TX_ACLR_WIDGET_H

#include "global.h"

namespace ns_ericsson {
namespace ns_radio_4415 {

KASE_MODEL(TX_ACLR,"-10M","-5M","5M","10M","Time")

KASE_WIDGET_PREFIX(Test_TX_ACLR)

KASE_CONFIG(
    QTextEdit, textEditFreq, "Freq",
    QLineEdit, lineEditAtt, "Att Bit")

KASE_RESULT(
    QLineEdit, lineEditAtt, "Att Bit")

    TX_ACLR_Model model;

KASE_THREAD(Test_TX_ACLR)

KASE_WIDGET_SUFFIX()

} // namespace ns_radio_4415
} // namespace ns_ericsson

#endif
