#ifndef INSTR_PM_H
#define INSTR_PM_H

#include "global_header.h"
#include "visa.h"
#include <string>

class pm : noncopyable
{
public:
    virtual ~pm(void) {}
    virtual std::string descriptor() = 0;
    virtual bool init(const std::string &dev) = 0;

public:
    virtual bool reset() = 0;
    virtual bool set_freq(double freq) = 0;
    virtual bool get_pwr(double &pwr) = 0;
    virtual bool get_pwr(double freq,double &pwr) = 0;
    virtual bool close() = 0;

protected:
    unsigned long m_session;
};

#endif
