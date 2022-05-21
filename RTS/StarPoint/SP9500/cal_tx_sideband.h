#ifndef CAL_TX_SIDEBAND_H
#define CAL_TX_SIDEBAND_H

#include "global.h"

//KASE_MODEL(TX_LO_Leak,"Att","Power","Time")

KASE_PREFIX(Cal_TX_Sideband)

KASE_CONFIG("Freq",    QLineEdit)
KASE_CONFIG("Method",  QComboBox)

KASE_WIDGET :
//    RX_RF_VGA_Model model;

KASE_THREAD :
    void initTXChain();
    void calUseSA(double &thM,uint16_t &am_i_m,uint16_t &am_q_m,double &pwrSb);

    double slopeTh_SA(double th,double delta,double &pwr);
    double slopeAmI_SA(uint16_t am_i,uint16_t am_q,uint16_t delta,double &pwr);
    double slopeAmQ_SA(uint16_t am_i,uint16_t am_q,uint16_t delta,double &pwr);
KASE_SUFFIX()

#endif
