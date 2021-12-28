#ifndef DEVICE_CHIP_LNA_HPP
#define DEVICE_CHIP_LNA_HPP

#include "global_header.h"

class lna
{
public:
    lna() : is_bypass(false) {}

public:
    bool is_bypass;
};


#endif // DEVICE_CHIP_LNA_HPP
