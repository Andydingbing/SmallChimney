#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H

#include "memory_physical.hpp"
#include <vector>

class memory_allocator : boost::noncopyable
{
public:
    memory_allocator() {}
    ~memory_allocator() { release(); }

public:
    std::vector<memory_physical *> *memory() const
    { return const_cast<std::vector<memory_physical *> *>(&_memory); }

    uint32_t *logic() const
    { return _memory.size() > 0 ? _memory[0]->logic() : nullptr; }

    uint64_t phy_addr(uint32_t idx);

    uint32_t total_size();
    uint32_t block_size(uint32_t idx);
    uint32_t total_block() { return uint32_t(_memory.size()); }

    int32_t allocate(const uint32_t size,uint32_t **logic_suggest = nullptr,bool continue_logic = false);
    int32_t release();

    int32_t r32(const uint32_t *buf,const uint32_t size,const uint64_t offset = 0) const;

    int32_t memory_idx(const uint64_t offset_logic,uint32_t &idx,uint32_t &offset_phy) const;

private:
    std::vector<memory_physical *> _memory;
};

#endif // memory_allocator_H


