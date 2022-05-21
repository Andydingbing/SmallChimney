#ifndef CAL_TX_LO_LEAK_H
#define CAL_TX_LO_LEAK_H

#include "global.h"

//KASE_MODEL(TX_LO_Leak,"Att","Power","Time")

KASE_PREFIX(Cal_TX_LO_Leak)

KASE_CONFIG("Freq",    QLineEdit)
KASE_CONFIG("10M Ref", QCheckBox)
KASE_CONFIG("Method",  QComboBox)

KASE_WIDGET :
//    RX_RF_VGA_Model model;

KASE_THREAD :
    void initTXChain();
    void calX9119();
    void calUseSA(qint16 &dc_i_m,qint16 &dc_q_m,double &pwrLOL);
    void calUseLoop(qint16 &dc_i_m,qint16 &dc_q_m,double &pwrLOL);

    double slopeI_SA(qint16 dc_i_m,qint16 dc_q_m,qint16 delta,double &pwr);
    double slopeQ_SA(qint16 dc_i_m,qint16 dc_q_m,qint16 delta,double &pwr);
KASE_SUFFIX()

#endif
