#ifndef ERICSSON_RADIO_6449_B42_TEST_RX_GAIN_ACCURACY_WIDGET_H
#define ERICSSON_RADIO_6449_B42_TEST_RX_GAIN_ACCURACY_WIDGET_H

#include "ericsson_radio_6449_b42_global.h"

namespace ns_ericsson {
namespace ns_radio_6449 {

KASE_MODEL(RX_Gain_Accu,"Branch","Power","Time")

KASE_CONFIG(Test_RX_Gain_Accuracy,
    QTextEdit, textEditFreq, "Freq",
    QLineEdit, lineEditAtt, "Att Bit")

KASE_RESULT(Test_RX_Gain_Accuracy,
    QLineEdit, lineEditAccu, "Accuracy")

KASE_WIDGET_PREFIX(Ericsson,Radio_6449_B42,Test_RX_Gain_Accuracy)
    RX_Gain_Accu_Model model;

KASE_THREAD(Test_RX_Gain_Accuracy)

KASE_WIDGET_SUFFIX()

} // namespace ns_radio_6449
} // namespace ns_ericsson

#endif
