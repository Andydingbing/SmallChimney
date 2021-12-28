#include "memory_physical.hpp"
#include "visa.h"
#include "algo_math.hpp"
#include "../../log/log.h"

#ifndef NIVISA_PXI
#define NIVISA_PXI
#endif

using namespace rd;

static ViSession g_vi_drm = 0;

uint32_t memory_physical::_size_granularity_min = 1024 * 64;
uint32_t memory_physical::_size_granularity_max = 1048576 * 4;

memory_physical::memory_physical() :
    _session(0),
    _phy_addr(0),
    _phy_addr_bfr_align(0),
    _size(0),
    _logic(nullptr)
{

}

memory_physical::~memory_physical()
{
    release();
}

int32_t memory_physical::allocate(const uint32_t size,uint32_t **logic_suggest,const uint32_t align)
{
    if (_phy_addr != 0) {
        return 0;
    }

    ViStatus status = VI_SUCCESS;
    ViAddr addr = nullptr;
    uint32_t actual_size = size;
    uint32_t retry = 0;
    bool is_alloced = false;

    limit_between(_size_granularity_min,_size_granularity_max,actual_size);
//    actual_size = actual_size > _size_granularity_min ? actual_size : _size_granularity_min;
//    actual_size = actual_size < _size_granularity_max ? actual_size : _size_granularity_max;

    if (g_vi_drm == 0) {
        VI_CHECK(viOpenDefaultRM(&g_vi_drm));
    }

    if (_session == 0) {
        status = viOpen(g_vi_drm,ViRsrc("PXI::MEMACC"),VI_NULL,VI_NULL,ViPSession(&_session));
        if (status < VI_SUCCESS) {
            _session = 0;
            return status;
        }
    }

    status = VI_SUCCESS - 1;
    actual_size += align;

    while (status < VI_SUCCESS && retry < 10) {
        for (uint32_t i = 0;i < 3;++i) {
            if ((status = viMemAllocEx(_session,actual_size,&_phy_addr)) == VI_SUCCESS) {
                _phy_addr_bfr_align = _phy_addr;
                _phy_addr = uint64_t(_phy_addr / align + 1) * align;
                _size = actual_size - uint32_t(_phy_addr - _phy_addr_bfr_align);
                _size = _size / align * align;
                is_alloced = true;
                break;
            }
        }

        if (is_alloced == true) {
            break;
        } else {
            actual_size -= align;
            actual_size /= 2;
            actual_size += align;
            actual_size = actual_size > _size_granularity_min ? actual_size : _size_granularity_min;
        }
        ++retry;
    }

    if (is_alloced == false) {
        Log.set_last_err("alloc %d(actual %d)",size,actual_size);
        return -1;
    }

    VI_CHECK(viSetAttribute(_session,VI_ATTR_DEST_INCREMENT,1));
    VI_CHECK(viMapAddressEx(_session,
                            VI_PXI_ALLOC_SPACE,
                            _phy_addr,
                            _size,
                            VI_FALSE,
                            logic_suggest == nullptr ? nullptr : *logic_suggest,
                            &addr));

    _logic = static_cast<uint32_t *>(addr);

    if (logic_suggest == nullptr) {
        return status;
    }
    if (*logic_suggest != nullptr && *logic_suggest != _logic) {
        Log.set_last_err("logic mismatch");
        return -1;
    } else {
        *logic_suggest = _logic;
    }
    return status;
}

int32_t memory_physical::release()
{
    ViStatus status = VI_SUCCESS;

    if (_session != 0) {
        VI_CHECK(viUnmapAddress(_session));
    }
    if (_phy_addr_bfr_align != 0) {
        VI_CHECK(viMemFreeEx(_session,_phy_addr_bfr_align));
    }
    if (_session != 0) {
        VI_CHECK(viClose(_session));
    }
    _session = 0;
    return 0;
}

int32_t memory_physical::w8(const uint8_t *buf,const uint32_t size,const uint64_t offset) const
{
    return viMoveOut8(_session,VI_PXI_ALLOC_SPACE,_phy_addr + offset,size,ViAUInt8(buf));
}

int32_t memory_physical::w16(const uint16_t *buf,const uint32_t size,const uint64_t offset) const
{
    return viMoveOut16(_session,VI_PXI_ALLOC_SPACE,_phy_addr + offset,size,ViAUInt16(buf));
}

int32_t memory_physical::w32(const uint32_t *buf,const uint32_t size,const uint64_t offset) const
{
    return viMoveOut32(_session,VI_PXI_ALLOC_SPACE,_phy_addr + offset,size,ViAUInt32(buf));
}

int32_t memory_physical::r8(const uint8_t *buf,const uint32_t size,const uint64_t offset) const
{
    return viMoveIn8(_session,VI_PXI_ALLOC_SPACE,_phy_addr + offset,size,ViAUInt8(buf));
}

int32_t memory_physical::r16(const uint16_t *buf,const uint32_t size,const uint64_t offset) const
{
    return viMoveIn16(_session,VI_PXI_ALLOC_SPACE,_phy_addr + offset,size,ViAUInt16(buf));
}

int32_t memory_physical::r32(const uint32_t *buf,const uint32_t size,const uint64_t offset) const
{
    return viMoveIn32(_session,VI_PXI_ALLOC_SPACE,_phy_addr + offset,size,ViAUInt32(buf));
}
