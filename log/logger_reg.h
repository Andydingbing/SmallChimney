#ifndef LOGGER_REG_H
#define LOGGER_REG_H

#include "logger.hpp"

struct API reg_log_t {
    int32_t  result;
    uint32_t addr;
    uint32_t w;
    uint32_t r;
    std::string chip;
    std::string time;

    reg_log_t();
};

class API logger_reg : public logger<reg_log_t>
{
public:
    static logger_reg &instance();
    int add(int32_t result,const char *fmt,...);
    int add(int32_t result,const std::string &chip,uint32_t addr,uint32_t w = 0xFFFFFFFF,uint32_t r = 0xFFFFFFFF);
};

#endif
