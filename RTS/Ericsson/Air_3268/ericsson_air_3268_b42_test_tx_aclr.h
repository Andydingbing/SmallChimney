#ifndef ERICSSON_RADIO_4415_B3_TEST_TX_ACLR_WIDGET_H
#define ERICSSON_RADIO_4415_B3_TEST_TX_ACLR_WIDGET_H

#include "thread_widget.h"
#include "ui_ericsson_radio_4415_b3_test_tx_aclr.h"
#include "device_ericsson_radio_4415.h"

namespace ns_ericsson {
namespace ns_radio_4415 {

KASE_MODEL(TX_ACLR,"-10M","-5M","5M","10M","Time")

KASE_CONFIG(Test_TX_ACLR,
    QTextEdit, textEditFreq, "Freq",
    QLineEdit, lineEditAtt, "Att Bit")

KASE_RESULT(Test_TX_ACLR,
    QLineEdit, lineEditAtt, "Att Bit")

KASE_WIDGET_PREFIX(Ericsson,Radio_4415_B3,Test_TX_ACLR)
    TX_ACLR_Model model;

KASE_THREAD(Test_TX_ACLR)

KASE_WIDGET_SUFFIX()

} // namespace ns_radio_4415
} // namespace ns_ericsson

#endif // ERICSSON_RADIO_4415_B3_TEST_TX_ACLR_WIDGET_H
