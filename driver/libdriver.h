#ifndef LIBBD_H
#define LIBBD_H

#include "pci_dev.h"
#include "pci_dev_vi.h"
#include "pci_dev_wd.h"
#include "pci_dev_udp.h"
#include "mem_io_vi.h"
#include "mem_io_udp.h"
#include "gpib_dev.h"
#include "serial_dev_vi.h"

//#if defined(__cplusplus) || defined(__cplusplus__)
//extern "C" {
//#endif

//enum serial_flow_ctrl_t {
//    SFC_None,
//    SFC_Software,
//    SFC_Hardware
//};

//enum serial_parity_bit_t {
//    SPB_None,
//    SPB_Odd,
//    SPB_Even
//};

//enum serial_stop_bit_t {
//    SSB_1,
//    SSB_1_5,
//    SSB_2
//};

//int32_t serial_dev_open(const std::string &dev,uint32_t &resource);
//int32_t serial_dev_set_baud_rate(const uint32_t resource,const uint32_t rate);
//int32_t serial_dev_set_flow_ctrl(const uint32_t resource,const serial_flow_ctrl_t &fc);
//int32_t serial_dev_set_parity_bit(const uint32_t resource,const serial_parity_bit_t &parity);
//int32_t serial_dev_set_stop_bit(const uint32_t resource,const serial_stop_bit_t &stop);
//int32_t serial_dev_set_data_bits(const uint32_t resource,const uint8_t bits);

//int32_t serial_dev_write(const uint32_t resource,const std::string &data);
//int32_t serial_dev_read(const uint32_t resource,std::string &buf,uint32_t to = 3000);
//int32_t serial_dev_close(const uint32_t &resource);

//#if defined(__cplusplus) || defined(__cplusplus__)
//}
//#endif

#endif // LIBBD_H
