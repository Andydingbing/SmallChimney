#ifndef LOGGER_COM_H
#define LOGGER_COM_H

#include "logger.hpp"

struct API com_log_t {
    int32_t result;
    std::string write;
    std::string read;
    std::string time;

    com_log_t();
};


class API logger_com : public logger<com_log_t>
{
public:
    int32_t add(int32_t result,const std::string &write,const std::string &read);
};

#endif
