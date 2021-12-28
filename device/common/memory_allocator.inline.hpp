#include "memory_allocator.hpp"
#include "visa.h"
#include "log.h"

uint64_t memory_allocator::phy_addr(uint32_t idx)
{
    if (idx > _memory.size()) {
        return 0;
    }
    return _memory[idx]->phy_addr();
}

uint32_t memory_allocator::total_size()
{
    uint32_t total = 0;

    for (size_t i = 0;i < _memory.size();i ++) {
        total += _memory[i]->size();
    }
    return total;
}

uint32_t memory_allocator::block_size(uint32_t idx)
{
    if (idx > _memory.size()) {
        return 0;
    }
    return _memory[idx]->size();
}

int32_t memory_allocator::allocate(const uint32_t size,uint32_t **logic_suggest,bool continue_logic)
{
    if (size == 0) {
        return 0;
    }

    uint32_t *logic = (logic_suggest == nullptr ? nullptr : *logic_suggest);
    uint32_t size_cur = total_size();
    int32_t size_left = 0;
    int32_t ret = 0;
    memory_physical *memory = nullptr;

    if (size_cur > 0 && size_cur < size) {
        release();
    }

    if (size_cur == size) {
        return 0;
    }

    size_left = int32_t(size);

    while (size_left > 0) {
        memory = new memory_physical();

        ret = memory->allocate(uint32_t(size_left),continue_logic ? &logic : nullptr,64 * 1024);

        if (continue_logic == true && ret != 0) {
            INT_CHECK(release());
            return -1;
        }

        size_left -= memory->size();
        logic += memory->size() / 4;

        _memory.push_back(memory);
    }

    return 0;
}

int32_t memory_allocator::release()
{
    for (size_t i = 0;i < _memory.size();++i) {
        delete _memory[i];
    }
    _memory.clear();
    return 0;
}

int32_t memory_allocator::r32(const uint32_t *buf,const uint32_t size,const uint64_t offset) const
{
    uint32_t idx = 0;
    uint32_t offset_phy = 0;

    uint32_t size_read = 0;
    int32_t size_left = int32_t(size);
    uint32_t *buf_cur = const_cast<uint32_t *>(buf);

    INT_CHECK(memory_idx(offset,idx,offset_phy));

    size_read = _memory[idx]->size() - offset_phy;
    size_read = size_read > uint32_t(size_left) ? uint32_t(size_left) : size_read;
    INT_CHECK(_memory[idx]->r32(buf_cur,size_read,offset_phy));

    size_left -= size_read;
    buf_cur += size_read;
    ++idx;

    while (size_left > 0) {
        if (idx > _memory.size() - 1) {
            return 0;
        }

        size_read = uint32_t(size_left) > _memory[idx]->size() ? _memory[idx]->size() : uint32_t(size_left);
        INT_CHECK(_memory[idx]->r32(buf_cur,size_read));

        ++idx;
    }
    return 0;
}

int32_t memory_allocator::memory_idx(const uint64_t offset_logic,uint32_t &idx,uint32_t &offset_phy) const
{
    uint64_t size = 0;

    for (size_t i = 0;i < _memory.size();++i) {
        size += _memory[i]->size();

        if (size > offset_logic) {
            idx = uint32_t(i);
            offset_phy = uint32_t(_memory[i]->size() - (size - offset_logic));
            return 0;
        }
    }
    return -1;
}
