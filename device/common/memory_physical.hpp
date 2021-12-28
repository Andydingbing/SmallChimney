#ifndef MEMORY_PHYSICAL_H
#define MEMORY_PHYSICAL_H

#include "global_header.h"
#include <boost/noncopyable.hpp>

class memory_physical : boost::noncopyable
{
public:
    memory_physical();
    ~memory_physical();

public:
    static void set_size_granularity_min(const uint32_t size)
    { _size_granularity_min = size; }

    static void set_size_granularity_max(const uint32_t size)
    { _size_granularity_max = size; }

    int32_t allocate(const uint32_t size,uint32_t **logic_suggest = nullptr,const uint32_t align = 4096);
    int32_t release();
    uint64_t phy_addr() const { return _phy_addr; }
    uint32_t *logic() const { return _logic; }
    uint32_t size() const { return _size; }

    int32_t w8 (const uint8_t  *buf,const uint32_t size,const uint64_t offset = 0) const;
    int32_t w16(const uint16_t *buf,const uint32_t size,const uint64_t offset = 0) const;
    int32_t w32(const uint32_t *buf,const uint32_t size,const uint64_t offset = 0) const;
    int32_t r8 (const uint8_t  *buf,const uint32_t size,const uint64_t offset = 0) const;
    int32_t r16(const uint16_t *buf,const uint32_t size,const uint64_t offset = 0) const;
    int32_t r32(const uint32_t *buf,const uint32_t size,const uint64_t offset = 0) const;

private:
    uint32_t _session;
    uint64_t _phy_addr;           // After  align
    uint64_t _phy_addr_bfr_align; // Before align
    uint32_t _size;
    uint32_t *_logic;

private:
    static uint32_t _size_granularity_min;
    static uint32_t _size_granularity_max;
};

#endif // MEMORY_PHYSICAL_H


