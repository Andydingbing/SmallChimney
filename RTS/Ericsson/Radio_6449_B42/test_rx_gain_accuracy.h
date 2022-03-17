#ifndef ERICSSON_RADIO_6449_B42_TEST_RX_GAIN_ACCURACY_WIDGET_H
#define ERICSSON_RADIO_6449_B42_TEST_RX_GAIN_ACCURACY_WIDGET_H

#include "global.h"

namespace ns_ericsson {
namespace ns_radio_6449 {

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

} // namespace ns_radio_6449
} // namespace ns_ericsson

#endif
