#ifndef INSTR_RS_NRP_H
#define INSTR_RS_NRP_H

#include "pm.h"

class rs_nrp : public pm
{
public:
    std::string descriptor() { return std::string("ROHDE&SCHWARZ,NRP"); }
    virtual bool init(const std::string &dev);

public:
    virtual bool reset();
    virtual bool set_freq(double freq);
    virtual bool get_pwr(double &pwr);
    virtual bool get_pwr(double freq,double &pwr);
    virtual bool close();
};

#endif // INSTR_RS_NRP_H
