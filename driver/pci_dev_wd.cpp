#include "pci_dev_wd.h"
//#include "windrvr.h"
//#include "wdc_defs.h"
//#include "wdc_lib.h"
//#include "status_strings.h"
#include <boost/core/ignore_unused.hpp>

#define LICENSE "6C3CC2CFE89E7AD042444C655646F21A0BF3FDEB.EMBRACE"

using namespace std;
using namespace boost;

pci_dev_wd::pci_dev_wd()
{
    _descriptor = "";
//	WDC_DriverOpen(WDC_DRV_OPEN_DEFAULT,LICENSE);
}

pci_dev_wd::pci_dev_wd(const string &descriptor)
{
    _descriptor = descriptor;
//	WDC_DriverOpen(WDC_DRV_OPEN_DEFAULT,LICENSE);
}

pci_dev_wd::~pci_dev_wd()
{
}

int32_t pci_dev_wd::get_devs(vector<string> &devs)
{
    ignore_unused(devs);
    return 0;
//    WDC_PCI_SCAN_RESULT scan_result;
//    uint32_t status = WD_STATUS_SUCCESS;
//    char buf[256] = {0};
//    string dev = "";

//    if ((status = WDC_PciScanDevices(0,0,&scan_result)) != WD_STATUS_SUCCESS)
//        return status;
//    for (uint32_t i = 0;i < scan_result.dwNumDevices;i ++) {
//        memset(buf,0,sizeof(buf));
//        dev.clear();
//        sprintf(buf,"device:%#x,VID:%6#x,DID:%6#x",i,scan_result.deviceId[i].dwVendorId,scan_result.deviceId[i].dwDeviceId);
//        dev = buf;
//        devs.push_back(dev);
//    }
}

int32_t pci_dev_wd::init(const string &dev)
{
    ignore_unused(dev);
    return 0;
}

int32_t pci_dev_wd::init(const string &dev,uint32_t vid,uint32_t did)
{
    ignore_unused(dev,vid,did);
    return 0;
}

int32_t pci_dev_wd::w8(addr_space_t addr,uint32_t offset,uint8_t data)
{
    ignore_unused(addr,offset,data);
    return 0;
}

int32_t pci_dev_wd::w16(addr_space_t addr,uint32_t offset,uint16_t data)
{
    ignore_unused(addr,offset,data);
    return 0;
}

int32_t pci_dev_wd::w32(addr_space_t addr,uint32_t offset,uint32_t data)
{
    ignore_unused(addr,offset,data);
    return 0;
}

int32_t pci_dev_wd::w32(addr_space_t addr,uint32_t offset,uint32_t len,uint32_t *buf)
{
    ignore_unused(addr,offset,len,buf);
    return 0;
}

int32_t pci_dev_wd::r8(addr_space_t addr, uint32_t offset, uint8_t *data)
{
    ignore_unused(addr,offset,data);
    return 0;
}

int32_t pci_dev_wd::r16(addr_space_t addr, uint32_t offset, uint16_t *data)
{
    ignore_unused(addr,offset,data);
    return 0;
}

int32_t pci_dev_wd::r32(addr_space_t addr,uint32_t offset,uint32_t *data)
{
    ignore_unused(addr,offset,data);
    return 0;
}
