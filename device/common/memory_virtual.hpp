#ifndef MEMORY_VIRTUAL_H
#define MEMORY_VIRTUAL_H

#include "rd.h"
#include <boost/noncopyable.hpp>

class memory_virtual : boost::noncopyable
{
public:
    memory_virtual();
    ~memory_virtual();

public:
    static void *reserve(uint32_t size,char *addr_suggest);

    int32_t reserve(const uint32_t size);
    int32_t release();
    uint32_t *addr() const { return _addr; }
    uint32_t size() const { return _size; }

private:
    uint32_t _size;
    uint32_t *_addr;
};

#endif // MEMORY_VIRTUAL_H


