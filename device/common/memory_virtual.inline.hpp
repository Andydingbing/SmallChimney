#include "memory_virtual.hpp"
#include <winbase.h>

memory_virtual::memory_virtual()
{

}

memory_virtual::~memory_virtual()
{

}

void* memory_virtual::reserve(uint32_t size,char *addr_suggest)
{
    uint32_t retry = 3;
    void *res = nullptr;

    for (uint32_t i = 0;i < retry;++i) {
        res = VirtualAlloc(addr_suggest,size,MEM_RESERVE,PAGE_READWRITE);
        if (res != nullptr) {
            break;
        }
    }

    return res;
}

int32_t memory_virtual::reserve(const uint32_t size)
{
    _addr = static_cast<uint32_t *>(reserve(size,nullptr));

    if (_addr != nullptr) {
        _size = size;
        return 0;
    }
    return -1;
}

int32_t memory_virtual::release()
{
    if (VirtualFree(_addr,0,MEM_RELEASE)) {
        return 0;
    }
    return -1;
}
