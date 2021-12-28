#ifndef LOGGER_MSG_H
#define LOGGER_MSG_H

#define LoggerMsg logger_msg::instance()

#define LM(condition,...) \
    do { \
        int32_t ret = 0; \
        if ((ret = (condition))) { \
            LoggerMsg.add(ret,__VA_ARGS__); \
            return ret; \
        } \
    } while (0)

#define LM_NO_RETURN(condition,...) \
    do { \
        int32_t ret = 0; \
        if ((ret = (condition))) { \
            LoggerMsg.add(ret,__VA_ARGS__); \
        } \
    } while (0)

#include "logger.hpp"

struct API msg_log_t {
    int32_t result;
    std::string msg;
    std::string time;

    msg_log_t();
};

class API logger_msg : public logger<msg_log_t>
{
public:
    static logger_msg &instance();
    int add(int32_t result,const char *fmt,...);
};

#endif
