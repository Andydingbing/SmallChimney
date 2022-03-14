#ifndef STARPOINT_SP9500_CAL_TX_LO_LEAK_H
#define STARPOINT_SP9500_CAL_TX_LO_LEAK_H

#include "starpoint_sp9500_global.h"

namespace ns_starpoint {
namespace ns_sp9500 {

//KASE_MODEL(TX_LO_Leak,"Att","Power","Time")

KASE_WIDGET_PREFIX(StarPoint,SP9500,Cal_TX_LO_Leak)

KASE_CONFIG(
    QCheckBox, checkBoxRef, "10M Ref",
    QComboBox, comboBoxMethod, "Method")

KASE_RESULT(
    QLineEdit, lineEditDyRange, "Dynamic Range")

//    RX_RF_VGA_Model model;

KASE_THREAD(Cal_TX_LO_Leak)
//public:
//    void initTXChain();
//    void calX9119();
//    void calUseSA(qint16 &dc_i_m,qint16 &dc_q_m,double &pwrLOL);
//    void calUseLoop(qint16 &dc_i_m,qint16 &dc_q_m,double &pwrLOL);

//    double slopeI_SA(qint16 dc_i_m,qint16 dc_q_m,qint16 delta,double &pwr);
//    double slopeQ_SA(qint16 dc_i_m,qint16 dc_q_m,qint16 delta,double &pwr);
//    int64_t getMinDCOffsetI_Rx(qint16 step,qint16 lr_coef,qint16 *dc_i_l,qint16 *dc_i_r,qint16 *dc_i_m,qint16 *dc_q_m,int64_t *ad);
//    int64_t getMinDCOffsetQ_Rx(qint16 step,qint16 lr_coef,qint16 *dc_q_l,qint16 *dc_q_r,qint16 *dc_i_m,qint16 *dc_q_m,int64_t *ad);
KASE_WIDGET_SUFFIX()

} // namespace ns_sp9500
} // namespace ns_starpoint

#endif
