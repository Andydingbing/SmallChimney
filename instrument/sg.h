#ifndef INSTR_SG_H
#define INSTR_SG_H

#include "gpib_dev.h"

class sg : public gpib_dev
{
public:
    sg();
    virtual bool set_cw(double freq);
    virtual bool get_cw(double &freq);
    virtual bool set_pl(double pwr);
    virtual bool get_pl(double &pwr);
    virtual bool set_output_en(bool en);
    virtual bool set_mod_en(bool en);

protected:
    double _pwr;
};

#endif // INSTR_SG_H
