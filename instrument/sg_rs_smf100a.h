#ifndef INSTR_RS_SMF100A_H
#define INSTR_RS_SMF100A_H

#include "sg_keysight_x.h"

class rs_smf100a : public sg
{
public:
    virtual std::string descriptor() { return std::string("SMF100A"); }

    bool set_mod_en(bool en)
    { return en ? w("MOD:STAT ON") : w("MOD:STAT OFF"); }
};

class rs_sma100a : public rs_smf100a
{
public:
    std::string descriptor() { return std::string("SMA100A"); }
};

#endif // INSTR_RS_SMF100A_H
