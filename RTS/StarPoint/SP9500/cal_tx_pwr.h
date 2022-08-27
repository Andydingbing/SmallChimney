#ifndef CAL_TX_PWR_H
#define CAL_TX_PWR_H

#include "global.h"

KASE_PREFIX(Cal_TX_Pwr)

KASE_CONFIG("Cal",   QCheckBox)
KASE_CONFIG("Check", QCheckBox)
KASE_CONFIG("Freq",  QTextEdit)
KASE_CONFIG("Mode(Output)", QCheckBox)
KASE_CONFIG("Mode(IO)",     QCheckBox)

KASE_WIDGET :

KASE_THREAD :
    set_helper::range_freq<uint64_t> _freq_range;

    void calIt(io_mode_t mode);
    void checkIt(io_mode_t mode);
    virtual void init();
    virtual void initTXBaseStateOP(double &att0,
                                   double &att1,
                                   double &att2,
                                   double &att3,
                                   double &dGain);

    virtual void initTXBaseStateIO(double &att0,
                                   double &att1,
                                   double &att2,
                                   double &att3,
                                   double &dGain);

    virtual void coarseTuning(double target, double &att0, double &att1);
    virtual void fineTuning(double target, double &dGain);
KASE_SUFFIX()

#endif
