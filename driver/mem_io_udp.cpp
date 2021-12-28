#include "mem_io_udp.h"
#include "string.h"

int32_t mem_io_udp::allocate(uint32_t size)
{
    if (_addr != nullptr) {
        return 0;
    }

    return ((_addr = new int8_t[size]) == nullptr) ? -1 : 0;
}

int32_t mem_io_udp::release()
{
    if (_addr != nullptr) {
        delete (int8_t *)_addr;
        _addr = nullptr;
    }

    return 0;
}

int32_t mem_io_udp::w8(uint8_t *buf,uint32_t size)
{
    memcpy(_addr,buf,size);
    return 0;
}

int32_t mem_io_udp::w16(uint16_t *buf,uint32_t size)
{
    memcpy(_addr,buf,size);
    return 0;
}

int32_t mem_io_udp::w32(uint32_t *buf,uint32_t size)
{
    memcpy(_addr,buf,size);
    return 0;
}

int32_t mem_io_udp::r8(uint8_t *buf,uint32_t size)
{
    buf = (uint8_t *)_addr;
    return 0;
}

int32_t mem_io_udp::r16(uint16_t *buf,uint32_t size)
{
    buf = (uint16_t *)_addr;
    return 0;
}

int32_t mem_io_udp::r32(uint32_t *buf,uint32_t size)
{
    buf = (uint32_t *)_addr;
    return 0;
}
