#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "logger.h"
#include "list.hpp"
#include <string>

template<typename T>
class logger : public logger_base
{
public:
    logger() : logger_base() {}
    logger(const std::string &path) : logger_base() {}

public:
    ctd::dlist<T *> *logs()
    { return &_logs; }

    size_t size() const
    { return _logs.size(); }

    T *at(const size_t idx)
    { return _logs.at(idx); }

protected:
    ctd::dlist<T *> _logs;
};

#endif // LOGGER_HPP
