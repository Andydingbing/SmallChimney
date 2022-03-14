#ifndef ERICSSON_RADIO_4415_B3_TEST_RX_GAIN_ACCURACY_WIDGET_H
#define ERICSSON_RADIO_4415_B3_TEST_RX_GAIN_ACCURACY_WIDGET_H

#include "ericsson_radio_4415_b3_global.h"

namespace ns_ericsson {
namespace ns_radio_4415 {

KASE_MODEL(RX_Gain_Accu,"Branch","Power","Time")

KASE_WIDGET_PREFIX(Ericsson,Radio_4415_B3,Test_RX_Gain_Accuracy)

KASE_CONFIG(QTextEdit, textEditFreq, "Freq",
            QLineEdit, lineEditAtt, "Att Bit")

KASE_RESULT(QLineEdit, lineEditAccu, "Accuracy")

    RX_Gain_Accu_Model model;

KASE_THREAD(Test_RX_Gain_Accuracy)

KASE_WIDGET_SUFFIX()

} // namespace ns_radio_4415
} // namespace ns_ericsson

#endif
