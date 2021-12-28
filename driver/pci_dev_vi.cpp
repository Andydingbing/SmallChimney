/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include "pci_dev_vi.h"
#include "visa.h"

#ifndef NIVISA_PXI
#define NIVISA_PXI
#endif

// Define shared data seg "viRsrcMgr(vi Resource Manager)"

//#ifdef COMPILER_MSC
//#pragma data_seg("viRsrcMgr")
//    static ViSession g_rsrc_mgr = 0;
//#pragma data_seg()
//#pragma comment(linker,"/section:viRsrcMgr,s")
//#elif defined COMPILER_GCC
//    ViSession g_rsrc_mgr __attribute__((section("viRsrcMgr"))) = 0;
//#endif

static ViSession g_rsrc_mgr = 0;

using namespace std;

pci_dev_vi::pci_dev_vi() :
    pci_dev(),
    _session(0)
{
    _descriptor = "";
    open_default_rm();
}

pci_dev_vi::pci_dev_vi(const string &descriptor) :
    pci_dev(),
    _session(0)
{
    _descriptor = descriptor;
    open_default_rm();
}

pci_dev_vi::~pci_dev_vi()
{
    if (_session) {
        viClose(_session);
    }
}

int32_t pci_dev_vi::get_devs(vector<string> &devs)
{
    ViStatus status = VI_SUCCESS;
    ViFindList find_list = 0;
    ViUInt32 ret_cnt = 0;
    ViChar buf[256] = {0};
    ViChar expr[32] = "PXI?*INSTR";

    devs.clear();

    if ((status = viFindRsrc(g_rsrc_mgr,expr,&find_list,&ret_cnt,buf)) < VI_SUCCESS) {
        return status;
    }

    devs.push_back(buf);

    while (ret_cnt --) {
        if ((status = viFindNext(find_list,buf)) == VI_ERROR_INV_OBJECT) {
            return status;
        }
        devs.push_back(buf);
	}

    devs.pop_back();
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            viClose(find_list);

    return status;
}

int32_t pci_dev_vi::init(const string &dev)
{
    return init(const_cast<ViRsrc>(dev.c_str()));
}

int32_t pci_dev_vi::init(const char *dev)
{
    ViSession session = 0;
    ViStatus status = VI_SUCCESS;

    if ((status = viOpen(g_rsrc_mgr,const_cast<ViRsrc>(dev),VI_NULL,VI_NULL,&session)) < VI_SUCCESS) {
        _session = 0;
        return status;
	}
    if ((status = viSetAttribute(session,VI_ATTR_DEST_INCREMENT,0)) < VI_SUCCESS) {
        viClose(session);
        _session = 0;
        return status;
    }
    _session = session;
    return status;
}

int32_t pci_dev_vi::init(const string &dev,uint32_t vid,uint32_t did)
{
    return init(const_cast<ViRsrc>(dev.c_str()),vid,did);
}

int32_t pci_dev_vi::init(const char *dev,uint32_t vid,uint32_t did)
{
    ViSession session = 0;
    ViStatus status = VI_SUCCESS;
    ViUInt32 manf_id = 0;
    ViUInt32 model_code = 0;

    if ((status = viOpen(g_rsrc_mgr,const_cast<ViRsrc>(dev),VI_NULL,VI_NULL,&session)) < VI_SUCCESS) {
        _session = 0;
        return status;
	}

    if ((status = viGetAttribute(session,VI_ATTR_MANF_ID,&manf_id)) < VI_SUCCESS || manf_id != vid) {
        viClose(session);
        _session = 0;
        return status;
	}
    if ((status = viGetAttribute(session,VI_ATTR_MODEL_CODE,&model_code)) < VI_SUCCESS || model_code != did) {
        viClose(session);
        _session = 0;
        return status;
	}
    if ((status = viSetAttribute(session,VI_ATTR_DEST_INCREMENT,0)) < VI_SUCCESS) {
        viClose(session);
        _session = 0;
        return status;
    }
    _session = session;
    return status;
}

SYM_INLINE int32_t pci_dev_vi::w8(addr_space_t addr,uint32_t offset,uint8_t data)
{
    return viOut8(_session,ViUInt16(addr),offset,data);
}

SYM_INLINE int32_t pci_dev_vi::w16(addr_space_t addr,uint32_t offset,uint16_t data)
{
    return viOut16(_session,ViUInt16(addr),offset,data);
}

SYM_INLINE int32_t pci_dev_vi::w32(addr_space_t addr,uint32_t offset,uint32_t data)
{
    return viOut32(_session,ViUInt16(addr),offset,data);
}

SYM_INLINE int32_t pci_dev_vi::w32(addr_space_t addr,uint32_t offset,uint32_t len,uint32_t *buf)
{
    return viMoveOut32(_session,ViUInt16(addr),offset,len,ViAUInt32(buf));
}

SYM_INLINE int32_t pci_dev_vi::r8(addr_space_t addr,uint32_t offset,uint8_t *data)
{
    return viIn8(_session,ViUInt16(addr),offset,data);
}

SYM_INLINE int32_t pci_dev_vi::r16(addr_space_t addr,uint32_t offset,uint16_t *data)
{
    return viIn16(_session,ViUInt16(addr),offset,data);
}

SYM_INLINE int32_t pci_dev_vi::r32(addr_space_t addr,uint32_t offset,uint32_t *data)
{
    return viIn32(_session,ViUInt16(addr),offset,ViPUInt32(data));
}

SYM_INLINE int32_t pci_dev_vi::lla_w8(uint32_t offset,uint8_t data)
{
    viPoke8(_session,ViPByte(_map_addr) + offset,data);
    return 0;
}

SYM_INLINE int32_t pci_dev_vi::lla_w16(uint32_t offset,uint16_t data)
{
    viPoke16(_session,ViPByte(_map_addr) + offset,data);
    return 0;
}

SYM_INLINE int32_t pci_dev_vi::lla_w32(uint32_t offset,uint32_t data)
{
    viPoke32(_session,ViPByte(_map_addr) + offset,data);
	return 0;
}

SYM_INLINE int32_t pci_dev_vi::lla_r8(uint32_t offset,uint8_t *data)
{
    viPeek8(_session,ViPByte(_map_addr) + offset,ViPUInt8(data));
    return 0;
}

SYM_INLINE int32_t pci_dev_vi::lla_r16(uint32_t offset,uint16_t *data)
{
    viPeek16(_session,ViPByte(_map_addr) + offset,ViPUInt16(data));
    return 0;
}

SYM_INLINE int32_t pci_dev_vi::lla_r32(uint32_t offset,uint32_t *data)
{
    viPeek32(_session,ViPByte(_map_addr) + offset,ViPUInt32(data));
	return 0;
}

int32_t pci_dev_vi::open_default_rm()
{
    if (!g_rsrc_mgr) {
        return viOpenDefaultRM(&g_rsrc_mgr);
    }
    return 0;
}

unsigned long pci_dev_vi::get_default_rm()
{
    return g_rsrc_mgr;
}
