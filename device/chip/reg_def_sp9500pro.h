#ifndef REG_DEF_SP9500PRO_H
#define REG_DEF_SP9500PRO_H

#include "reg_def.h"

#define SP9500PRO_RFU_V9_CHECK \
if (_v9 == nullptr) { \
    Log.set_last_err("Device:V9 disconnected"); \
    return -1; \
}

#define SP9500PRO_S6_CHECK \
if (_s6 == nullptr) { \
    Log.set_last_err("Device:S6 disconnected"); \
    return -1; \
}

// Register
#define SP9500PRO_RFU_V9_REG_DECL(addr) REG_DECL(ns_reg_sp9500pro_rfu_v9,addr)
#define SP9500PRO_RFU_V9_REG_DATA(addr) REG_DATA(ns_reg_sp9500pro_rfu_v9,addr)
#define SP9500PRO_RFU_V9_REG_CLER(addr) REG_CLER(ns_reg_sp9500pro_rfu_v9,addr)
#define SP9500PRO_RFU_V9_REG(addr)      REG(ns_reg_sp9500pro_rfu_v9,addr)

#define SP9500PRO_RFU_V9_REG_DECL_2(addr0,addr1) REG_DECL_2(ns_reg_sp9500pro_rfu_v9,addr0,addr1)
#define SP9500PRO_RFU_V9_REG_DATA_2(addr0,addr1) REG_DATA_2(ns_reg_sp9500pro_rfu_v9,addr0,addr1)
#define SP9500PRO_RFU_V9_REG_CLER_2(addr0,addr1) REG_CLER_2(ns_reg_sp9500pro_rfu_v9,addr0,addr1)
#define SP9500PRO_RFU_V9_REG_2(addr0,addr1)      REG_2(ns_reg_sp9500pro_rfu_v9,addr0,addr1)

#define SP9500PRO_S6_REG_DECL(addr) REG_DECL(ns_reg_sp9500pro_s6,addr)
#define SP9500PRO_S6_REG_DATA(addr) REG_DATA(ns_reg_sp9500pro_s6,addr)
#define SP9500PRO_S6_REG_CLER(addr) REG_CLER(ns_reg_sp9500pro_s6,addr)
#define SP9500PRO_S6_REG(addr)      REG(ns_reg_sp9500pro_s6,addr)

#define SP1403_S6_REG_DECL(addr) REG_DECL(ns_reg_sp1403_s6,addr)
#define SP1403_S6_REG_DATA(addr) REG_DATA(ns_reg_sp1403_s6,addr)
#define SP1403_S6_REG_CLER(addr) REG_CLER(ns_reg_sp1403_s6,addr)
#define SP1403_S6_REG(addr)      REG(ns_reg_sp1403_s6,addr)

#define SP9500PRO_RRH_REG_DECL(addr) REG_DECL(ns_reg_sp9500pro_rrh,addr)
#define SP9500PRO_RRH_REG_DATA(addr) REG_DATA(ns_reg_sp9500pro_rrh,addr)
#define SP9500PRO_RRH_REG_CLER(addr) REG_CLER(ns_reg_sp9500pro_rrh,addr)
#define SP9500PRO_RRH_REG(addr)      REG(ns_reg_sp9500pro_rrh,addr)

// IO
#define SP9500PRO_RFU_V9_W(addr) \
    SP9500PRO_RFU_V9_CHECK PCIE_DEV_W(_v9,pci_dev::AS_BAR0,ns_reg_sp9500pro_rfu_v9,addr)

#define SP9500PRO_RFU_V9_R(addr) \
    SP9500PRO_RFU_V9_CHECK PCIE_DEV_R(_v9,pci_dev::AS_BAR0,ns_reg_sp9500pro_rfu_v9,addr)

#define SP9500PRO_RFU_V9_W_2(addr0,addr1) \
    SP9500PRO_RFU_V9_CHECK PCIE_DEV_W_2(_v9,pci_dev::AS_BAR0,ns_reg_sp9500pro_rfu_v9,addr0,addr1)

#define SP9500PRO_RFU_V9_R_2(addr0,addr1) \
    SP9500PRO_RFU_V9_CHECK PCIE_DEV_R_2(_v9,pci_dev::AS_BAR0,ns_reg_sp9500pro_rfu_v9,addr0,addr1)

#define SP9500PRO_S6_W(addr) \
    SP9500PRO_S6_CHECK PCIE_DEV_W(_s6,pci_dev::AS_BAR0,ns_reg_sp9500pro_s6,addr)

#define SP9500PRO_S6_R(addr) \
    SP9500PRO_S6_CHECK PCIE_DEV_R(_s6,pci_dev::AS_BAR0,ns_reg_sp9500pro_s6,addr)

#define SP9500PRO_RFU_V9_SET_BITS(addr,bits,data) \
    PCIE_DEV_SET_BITS(SP9500PRO_RFU_V9,addr,bits,data)

#define SP9500PRO_RFU_V9_SET_BITS_2(addr0,addr1,bits,data) \
    PCIE_DEV_SET_BITS_2(SP9500PRO_RFU_V9,addr0,addr1,bits,data)

// Rising edge
#define SP9500PRO_RFU_V9_RE(addr,bit) CTRLLER_RE(SP9500PRO_RFU_V9,addr,bit)
#define SP9500PRO_RFU_V9_OP(addr) SP9500PRO_RFU_V9_RE(addr,op)

#define SP9500PRO_RFU_V9_RE_2(addr0,addr1,bit) CTRLLER_RE_2(SP9500PRO_RFU_V9,addr0,addr1,bit)
#define SP9500PRO_RFU_V9_OP_2(addr0,addr1) SP9500PRO_RFU_V9_RE_2(addr0,addr1,op)

#define SP9500PRO_S6_RE(addr,bit) CTRLLER_RE(SP9500PRO_S6,addr,bit)
#define SP9500PRO_S6_OP(addr) SP9500PRO_S6_RE(addr,op)

// State machine
#define SP9500PRO_RFU_V9_WAIT_IDLE(addr,flag,times) \
    CTRLLER_WAIT_IDLE(SP9500PRO_RFU_V9,addr,busy,flag,times)

#define SP9500PRO_RFU_V9_WAIT_IDLE_2(addr0,addr1,flag,times) \
    CTRLLER_WAIT_IDLE_2(SP9500PRO_RFU_V9,addr0,addr1,busy,flag,times)

#define SP2406_S6_WAIT_IDLE(addr,flag,times) \
    CTRLLER_WAIT_IDLE(SP2406_S6,addr,busy,flag,times)

// Register Define

namespace ns_reg_sp9500pro_rfu_v9 {

REG_DEF_CPT_PREFIX_32(0x0000)
    unsigned op : 1;  // DMA state-machine reset signal
    unsigned rsv : 7;
    unsigned ver : 8; // firmware version
    unsigned cdw : 4;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x004c)
    unsigned slot_time : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x004d)
    unsigned ul : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x004e)
    unsigned dl : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x00c0)
    unsigned cpri : 8;
    unsigned rsv0 : 24;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x00c1)
    unsigned cpri : 8;
    unsigned rsv0 : 24;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x00c2)
    unsigned cpri : 8;
    unsigned rsv0 : 24;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x00c3)
    unsigned cpri : 8;
    unsigned rsv0 : 24;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x00c4)
    unsigned cpri : 8;
    unsigned rsv0 : 24;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x00c5)
    unsigned cpri : 8;
    unsigned rsv0 : 24;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x00c6)
    unsigned cpri : 8;
    unsigned rsv0 : 24;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x00c7)
    unsigned cpri : 8;
    unsigned rsv0 : 24;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x00c8)
    unsigned cpri : 8;
    unsigned rsv0 : 24;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x00c9)
    unsigned cpri : 8;
    unsigned rsv0 : 24;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x00ca)
    unsigned cpri : 8;
    unsigned rsv0 : 24;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x00cb)
    unsigned cpri : 8;
    unsigned rsv0 : 24;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x00cc)
    unsigned cpri : 8;
    unsigned rsv0 : 24;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x00cd)
    unsigned cpri : 8;
    unsigned rsv0 : 24;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x00ce)
    unsigned cpri : 8;
    unsigned rsv0 : 24;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x00cf)
    unsigned cpri : 8;
    unsigned rsv0 : 24;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32_2(0x00d0,0x00db)
    unsigned sr0 : 4;
    unsigned sr1 : 4;
    unsigned sr2 : 4;
    unsigned sr3 : 4;
    unsigned sr4 : 4;
    unsigned sr5 : 4;
    unsigned sr6 : 4;
    unsigned sr7 : 4;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x00d1,0x00dc)
    unsigned src0 : 4;
    unsigned src1 : 4;
    unsigned src2 : 4;
    unsigned src3 : 4;
    unsigned src4 : 4;
    unsigned src5 : 4;
    unsigned src6 : 4;
    unsigned src7 : 4;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32(0x00d2)
    unsigned i : 16;
    unsigned q : 16;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x00d3)
    unsigned op : 1;
    unsigned rsv0 : 31;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x00d4)
    unsigned freq : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x00d5)
    unsigned phase : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x00d6)
    unsigned gain : 16;
    unsigned rsv0 : 16;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x00f0)
    unsigned cpri0 : 8;
    unsigned cpri1 : 8;
    unsigned rsv0 : 16;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x00f1)
    unsigned src : 4;
    unsigned rsv0 : 28;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x00f2)
    unsigned sr : 4;
    unsigned rsv0 : 28;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32_2(0x0103,0x0123)
    unsigned rrh : 1;
    unsigned pll : 1;
    unsigned sw : 1;
    unsigned att : 1;
    unsigned ch : 4;
    unsigned rsv0 : 24;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x0105,0x0125)
    unsigned addr : 7;
    unsigned wr : 1;
    unsigned op : 1;
    unsigned rsv0 : 1;
    unsigned busy : 1;
    unsigned rsv1 : 21;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x0106,0x0126)
    unsigned data : 32;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x0107,0x0127)
    unsigned data : 32;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x0108,0x0128)
    unsigned data : 16;
    unsigned addr : 8;
    unsigned wr : 1;
    unsigned op : 1;
    unsigned sync : 1;
    unsigned lock : 1;
    unsigned rsv0 : 1;
    unsigned busy : 1;
    unsigned rsv1 : 2;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x0109,0x0129)
    unsigned data : 16;
    unsigned addr : 8;
    unsigned wr : 1;
    unsigned op : 1;
    unsigned sync : 1;
    unsigned lock : 1;
    unsigned rsv0 : 1;
    unsigned busy : 1;
    unsigned rsv1 : 2;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x010a,0x012a)
    unsigned data : 16;
    unsigned addr : 8;
    unsigned wr : 1;
    unsigned op : 1;
    unsigned sync : 1;
    unsigned lock : 1;
    unsigned rsv0 : 1;
    unsigned busy : 1;
    unsigned rsv1 : 2;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x010b,0x012b)
    unsigned data : 16;
    unsigned rsv0 : 16;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x010c,0x012c)
    unsigned data : 16;
    unsigned rsv0 : 16;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x010d,0x012d)
    unsigned data : 16;
    unsigned rsv0 : 16;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x010e,0x012e)
    unsigned data : 16;
    unsigned addr : 3;
    unsigned wr : 1;
    unsigned op : 1;
    unsigned rsv0 : 1;
    unsigned busy : 1;
    unsigned rsv1 : 9;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x010f,0x012f)
    unsigned data : 16;
    unsigned rsv0 : 16;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x0110,0x0130)
    unsigned data : 8;
    unsigned addr : 8;
    unsigned wr : 1;
    unsigned op : 1;
    unsigned rsv0 : 1;
    unsigned busy : 1;
    unsigned rsv1 : 12;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x0111,0x0131)
    unsigned busy : 1;
    unsigned rsv0 : 1;
    unsigned rb  : 1;
    unsigned seq : 2;
    unsigned ref : 3;
    unsigned bw : 1;
    unsigned in : 3;
    unsigned incc : 3;
    unsigned cfg : 1;
    unsigned rsv1 : 15;
    unsigned op : 1;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x0112,0x0132)
    unsigned ad : 14;
    unsigned rsv0 : 18;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x0113,0x0133)
    unsigned busy : 1;
    unsigned rsv0 : 31;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32(0x0180)
    unsigned op : 1;
    unsigned rsv0 : 31;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0181)
    unsigned cnt : 8;
    unsigned rsv0 : 24;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0182)
    unsigned n : 8;
    unsigned rsv0 : 24;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0183)
    unsigned addr_low : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0184)
    unsigned addr_high : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0185)
    unsigned length : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0186)
    unsigned addr_low : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0187)
    unsigned addr_high : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0188)
    unsigned length : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0189)
    unsigned op : 1;
    unsigned rsv0 : 31;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x018a)
    unsigned length : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x018b)
    unsigned addr_low : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x018c)
    unsigned addr_high : 1;
    unsigned rsv0 : 31;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x018d)
    unsigned byte : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x018e)
    unsigned op : 1;
    unsigned rsv0 : 31;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x018f)
    unsigned length : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0190)
    unsigned addr_low : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0191)
    unsigned addr_high : 1;
    unsigned rsv0 : 31;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0192)
    unsigned byte : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0240)
    unsigned op : 1;
    unsigned rsv0 : 31;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0241)
    unsigned abort : 1;
    unsigned rsv0 : 31;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0242)
    unsigned mode : 1;
    unsigned rsv0 : 31;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0243)
    unsigned trig_src : 7;
    unsigned rsv0 : 25;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0244)
    unsigned trig_edge : 1;
    unsigned rsv0 : 31;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0245)
    unsigned threshold : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0246)
    unsigned trig_offset : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0247)
    unsigned time : 31;
    unsigned timeout : 1;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0248)
    unsigned trig_gap : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0249)
    unsigned sr : 4;
    unsigned rsv0 : 28;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x024a)
    unsigned frame : 10;
    unsigned rsv0 : 22;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x024b)
    unsigned x : 10;
    unsigned rsv0 : 22;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x024c)
    unsigned y : 10;
    unsigned rsv0 : 22;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x024d)
    unsigned addr_low : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x024e)
    unsigned addr_high : 1;
    unsigned rsv0 : 31;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x024f)
    unsigned samples : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0280)
    unsigned op : 1;
    unsigned rsv0 : 31;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0281)
    unsigned op : 1;
    unsigned rsv0 : 31;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0282)
    unsigned op : 1;
    unsigned rsv0 : 31;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0283)
    unsigned retrig : 1;
    unsigned rsv0 : 31;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0284)
    unsigned rep : 1;
    unsigned rsv0 : 31;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0285)
    unsigned trig_src : 3;
    unsigned rsv0 : 29;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0286)
    unsigned trig_edge : 1;
    unsigned rsv0 : 31;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0287)
    unsigned rep : 2;
    unsigned rsv0 : 30;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0288)
    unsigned op : 1;
    unsigned rsv0 : 31;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0289)
    unsigned next_seg : 6;
    unsigned rsv0 : 26;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x028a)
    unsigned op : 1;
    unsigned rsv0 : 31;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x028b)
    unsigned seg : 6;
    unsigned rsv0 : 26;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x028c)
    unsigned samples : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x028d)
    unsigned addr_low : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x028e)
    unsigned addr_high : 1;
    unsigned rsv0 : 31;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x028f)
    unsigned cycle : 12;
    unsigned rsv0 : 20;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0290)
    unsigned samples : 17;
    unsigned rsv0 : 15;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0291)
    unsigned segs : 6;
    unsigned rsv0 : 26;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0292)
    unsigned sr : 4;
    unsigned rsv0 : 28;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32_2(0x0200,0x0400)
    unsigned bypass_lpf : 8;
    unsigned bypass_hbf0 : 8;
    unsigned bypass_hbf1 : 8;
    unsigned bypass_rc : 1; // rate conversion filters
    unsigned bypass_cf : 1; // compensate filter
    unsigned bypass_gc : 1; // gain compensate
    unsigned rsv0 : 5;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x04c1,0x0401)
    unsigned i : 16;
    unsigned q : 16;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x0202,0x0402)
    unsigned op : 8;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x0203,0x0403)
    unsigned freq : 32;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x0205,0x0405)
    unsigned freq : 32;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x0207,0x0407)
    unsigned freq : 32;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x0209,0x0409)
    unsigned freq : 32;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x020b,0x040b)
    unsigned freq : 32;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x020d,0x040d)
    unsigned freq : 32;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x020f,0x040f)
    unsigned freq : 32;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x0211,0x0411)
    unsigned freq : 32;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x0213,0x0413)
    unsigned op0 : 1;
    unsigned op1 : 1;
    unsigned rsv0 : 30;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x0214,0x0414)
    unsigned freq : 32;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x0215,0x0415)
    unsigned phase : 32;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x0216,0x0416)
    unsigned freq : 32;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x0217,0x0417)
    unsigned phase : 32;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x0218,0x0418)
    unsigned freq : 32;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x0219,0x0419)
    unsigned phase : 32;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x021a,0x041a)
    unsigned freq : 32;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x021b,0x041b)
    unsigned phase : 32;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x022d,0x042d)
    unsigned src : 4;
    unsigned rsv0 : 28;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x022e,0x042e)
    unsigned ch : 8;
    unsigned rsv0 : 24;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32(0x0301)
    unsigned i : 16;
    unsigned q : 16;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0302)
    unsigned op : 8;
    unsigned rsv0 : 24;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0313)
    unsigned op : 8;
    unsigned rsv0 : 24;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32_2(0x021c,0x041c)
    unsigned op : 8;
    unsigned rsv0 : 24;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x021d,0x041d)
    unsigned tap : 8;
    unsigned rsv0 : 24;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x021e,0x041e)
    unsigned idx : 8;
    unsigned rsv0 : 24;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x021f,0x041f)
    unsigned coef : 16;
    unsigned rsv0 : 16;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x0220,0x0420)
    unsigned coef : 16;
    unsigned rsv0 : 16;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x0221,0x0421)
    unsigned coef : 16;
    unsigned rsv0 : 16;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x0222,0x0422)
    unsigned coef : 16;
    unsigned rsv0 : 16;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x0223,0x0423)
    unsigned coef : 16;
    unsigned rsv0 : 16;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x0224,0x0424)
    unsigned coef : 16;
    unsigned rsv0 : 16;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x0225,0x0425)
    unsigned coef : 16;
    unsigned rsv0 : 16;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x0226,0x0426)
    unsigned coef : 16;
    unsigned rsv0 : 16;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x0227,0x0427)
    unsigned coef : 16;
    unsigned rsv0 : 16;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x04e8,0x0428)
    unsigned op : 1;
    unsigned rsv0 : 31;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x04e9,0x0429)
    unsigned tap : 8;
    unsigned rsv0 : 24;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x04ea,0x042a)
    unsigned idx : 8;
    unsigned rsv0 : 24;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x04eb,0x042b)
    unsigned real : 16;
    unsigned imag : 16;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32_2(0x04ec,0x042c)
    unsigned real : 16;
    unsigned imag : 16;
REG_DEF_CPT_SUFFIX_32_2

REG_DEF_CPT_PREFIX_32(0x0440)
    unsigned reset_dl : 1;
    unsigned reset_ul : 1;
    unsigned rsv0 : 30;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0441)
    unsigned en_dl : 1;
    unsigned en_ul : 1;
    unsigned rsv0 : 30;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0443)
    unsigned sync : 2;
    unsigned rsv0 : 30;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0444)
    unsigned unsync_cnt_dl : 16;
    unsigned unsync_cnt_ul : 16;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0445)
    unsigned op : 1;
    unsigned rsv0 : 31;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0450)
    unsigned src0 : 4;
    unsigned src1 : 4;
    unsigned rsv0 : 24;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0451)
    unsigned i : 16;
    unsigned q : 16;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0452)
    unsigned op : 1;
    unsigned rsv0 : 31;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0453)
    unsigned mode : 3;
    unsigned rsv0 : 29;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0454)
    unsigned period : 3;
    unsigned rsv0 : 29;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0455)
    unsigned interval : 3;
    unsigned rsv0 : 29;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0456)
    unsigned freq : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0457)
    unsigned freq : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0459)
    unsigned gain : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0462)
    unsigned rx_0 : 1;
    unsigned rx_1 : 1;
    unsigned tx_0 : 1;
    unsigned tx_1 : 1;
    unsigned rsv0 : 28;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0470)
    unsigned op : 1;
    unsigned rsv0 : 31;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0471)
    unsigned rx_0 : 1;
    unsigned rx_1 : 1;
    unsigned tx_0 : 1;
    unsigned tx_1 : 1;
    unsigned rsv0 : 28;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0472)
    unsigned iq : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0480)
    unsigned mode : 1;
    unsigned rsv0 : 31;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0481)
    unsigned op : 1;
    unsigned rsv0 : 31;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0482)
    unsigned speed : 8;
    unsigned reg : 8;
    unsigned wr : 1;
    unsigned id : 7;
    unsigned rsv0 : 8;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0485)
    unsigned mode : 1;
    unsigned rsv0 : 31;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0486)
    unsigned op : 1;
    unsigned rsv0 : 31;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0487)
    unsigned speed : 8;
    unsigned reg : 8;
    unsigned wr : 1;
    unsigned id : 7;
    unsigned rsv0 : 8;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0800)
    unsigned bypass_lpf : 8;
    unsigned bypass_hbf2 : 8;
    unsigned bypass_hbf1 : 8;
    unsigned bypass_cf : 1;
    unsigned bypass_gc : 1;
    unsigned rsv0 : 6;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0801)
    unsigned i : 16;
    unsigned q : 16;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0802)
    unsigned op : 8;
    unsigned rsv0 : 24;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0803)
    unsigned freq : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0805)
    unsigned freq : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0807)
    unsigned freq : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0809)
    unsigned freq : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x080b)
    unsigned freq : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x080d)
    unsigned freq : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x080f)
    unsigned freq : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0811)
    unsigned freq : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0813)
    unsigned op0 : 1;
    unsigned op1 : 1;
    unsigned rsv0 : 30;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0814)
    unsigned freq : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0815)
    unsigned phase : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0816)
    unsigned freq : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0817)
    unsigned phase : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0818)
    unsigned freq : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0819)
    unsigned phase : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x081a)
    unsigned freq : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x081b)
    unsigned phase : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x081c)
    unsigned op : 8;
    unsigned rsv0 : 24;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x081d)
    unsigned tap : 8;
    unsigned rsv0 : 24;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x081e)
    unsigned idx : 8;
    unsigned rsv0 : 24;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x081f)
    unsigned coef : 16;
    unsigned rsv0 : 16;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0820)
    unsigned coef : 16;
    unsigned rsv0 : 16;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0821)
    unsigned coef : 16;
    unsigned rsv0 : 16;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0822)
    unsigned coef : 16;
    unsigned rsv0 : 16;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0823)
    unsigned coef : 16;
    unsigned rsv0 : 16;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0824)
    unsigned coef : 16;
    unsigned rsv0 : 16;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0825)
    unsigned coef : 16;
    unsigned rsv0 : 16;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0826)
    unsigned coef : 16;
    unsigned rsv0 : 16;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0827)
    unsigned coef : 16;
    unsigned rsv0 : 16;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0828)
    unsigned op : 1;
    unsigned rsv0 : 31;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0829)
    unsigned tap : 8;
    unsigned rsv0 : 24;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x082a)
    unsigned idx : 8;
    unsigned rsv0 : 24;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x082b)
    unsigned real : 16;
    unsigned imag : 16;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x082c)
    unsigned real : 16;
    unsigned imag : 16;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x082d)
    unsigned src : 4;
    unsigned rsv0 : 28;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x082e)
    unsigned ch : 8;
    unsigned rsv0 : 24;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0840)
    unsigned mode : 1;
    unsigned rsv0 : 31;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0841)
    unsigned data : 8;
    unsigned addr : 15;
    unsigned wr : 1;
    unsigned op : 1;
    unsigned rsv0 : 1;
    unsigned busy : 1;
    unsigned rsv1 : 5;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0842)
    unsigned data : 8;
    unsigned rsv0 : 24;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x1030)
    unsigned data : 8;
    unsigned addr : 8;
    unsigned wr : 1;
    unsigned op : 1;
    unsigned rsv0 : 2;
    unsigned busy : 1;
    unsigned rsv1 : 11;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x1031)
    unsigned data : 8;
    unsigned addr : 8;
    unsigned wr : 1;
    unsigned op : 1;
    unsigned rsv0 : 2;
    unsigned busy : 1;
    unsigned rsv1 : 11;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x1032)
    unsigned data : 8;
    unsigned addr : 8;
    unsigned wr : 1;
    unsigned op : 1;
    unsigned rsv0 : 2;
    unsigned busy : 1;
    unsigned rsv1 : 11;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x1033)
    unsigned data : 8;
    unsigned addr : 8;
    unsigned wr : 1;
    unsigned op : 1;
    unsigned rsv0 : 2;
    unsigned busy : 1;
    unsigned rsv1 : 11;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x1034)
    unsigned data : 8;
    unsigned addr : 8;
    unsigned wr : 1;
    unsigned op : 1;
    unsigned rsv0 : 2;
    unsigned busy : 1;
    unsigned rsv1 : 11;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x1035)
    unsigned data : 8;
    unsigned addr : 8;
    unsigned wr : 1;
    unsigned op : 1;
    unsigned rsv0 : 2;
    unsigned busy : 1;
    unsigned rsv1 : 11;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x1036)
    unsigned data : 8;
    unsigned addr : 8;
    unsigned wr : 1;
    unsigned op : 1;
    unsigned rsv0 : 2;
    unsigned busy : 1;
    unsigned rsv1 : 11;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x1037)
    unsigned data : 8;
    unsigned addr : 8;
    unsigned wr : 1;
    unsigned op : 1;
    unsigned rsv0 : 2;
    unsigned busy : 1;
    unsigned rsv1 : 11;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x1038)
    unsigned data : 8;
    unsigned addr : 8;
    unsigned wr : 1;
    unsigned op : 1;
    unsigned rsv0 : 2;
    unsigned busy : 1;
    unsigned rsv1 : 11;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x1039)
    unsigned data : 8;
    unsigned addr : 8;
    unsigned wr : 1;
    unsigned op : 1;
    unsigned rsv0 : 2;
    unsigned busy : 1;
    unsigned rsv1 : 11;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x103a)
    unsigned ch : 4;
    unsigned rsv0 : 28;
REG_DEF_CPT_SUFFIX_32

} // namespace ns_reg_sp9500pro_rfu_v9

namespace ns_reg_sp9500pro_rrh {

REG_DEF_CPT_PREFIX_32(0x03)
    unsigned rx_ref : 15;
    unsigned rx_ref_sign : 1;
    unsigned tx_pwr : 15;
    unsigned tx_pwr_sign : 1;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x07)
    unsigned det : 16;
    unsigned if_loop : 1;
    unsigned state : 1;
    unsigned sw : 2;
    unsigned rsv0 : 12;
REG_DEF_CPT_SUFFIX_32

} // ns_reg_sp9500pro_rrh


namespace ns_reg_sp1403_s6 {

REG_DEF_CPT_PREFIX_16(0x1)
    unsigned tx0_sw1 : 1;
    unsigned tx0_sw2 : 1;
    unsigned tx0_sw3 : 1;
    unsigned tx0_sw4 : 1;
    unsigned tx0_sw5 : 1;
    unsigned tx0_sw6 : 1;
    unsigned tx0_sw7 : 1;
    unsigned tx0_sw8 : 1;
    unsigned tx0_sw9 : 1;
    unsigned tx0_sw10 : 1;
    unsigned tx0_sw11 : 1;
    unsigned tx0_sw12 : 1;
    unsigned rsv0 : 4;
REG_DEF_CPT_SUFFIX_16

REG_DEF_CPT_PREFIX_16(0x2)
    unsigned tx1_sw1 : 1;
    unsigned tx1_sw2 : 1;
    unsigned tx1_sw3 : 1;
    unsigned tx1_sw4 : 1;
    unsigned tx1_sw5 : 1;
    unsigned tx1_sw6 : 1;
    unsigned rsv0 : 10;
REG_DEF_CPT_SUFFIX_16

REG_DEF_CPT_PREFIX_16(0x3)
    unsigned rx_sw1 : 1;
    unsigned rx_sw2 : 1;
    unsigned rx_sw3 : 1;
    unsigned rx_sw4 : 1;
    unsigned rx_sw5 : 1;
    unsigned rx_sw6 : 1;
    unsigned rx_sw7 : 1;
    unsigned rsv0 : 9;
REG_DEF_CPT_SUFFIX_16

REG_DEF_CPT_PREFIX_16(0x4)
    unsigned lo_sw1 : 1;
    unsigned lo_sw2 : 1;
    unsigned det_sw : 1;
    unsigned rsv0 : 13;
REG_DEF_CPT_SUFFIX_16

REG_DEF_CPT_PREFIX_16(0x5)
    unsigned tx0_red : 1;
    unsigned tx0_green : 1;
    unsigned rsv0 : 2;
    unsigned tx1_red : 1;
    unsigned tx1_green : 1;
    unsigned rsv1 : 2;
    unsigned rx_red : 1;
    unsigned rx_green : 1;
    unsigned rsv2 : 6;
REG_DEF_CPT_SUFFIX_16

} // namespace ns_reg_sp1403_s6


namespace ns_reg_sp9500pro_s6 {

REG_DEF_CPT_PREFIX_32(0x0000)
    unsigned ver : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0002)
    unsigned op : 1;
    unsigned rsv0 : 31;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0004)
    unsigned op : 1;
    unsigned rsv0 : 31;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0100)
    unsigned buf : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0200)
    unsigned buf : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0101)
    unsigned program : 1;
    unsigned rsv0 : 31;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0102)
    unsigned mode : 1;
    unsigned rsv0 : 31;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0103)
    unsigned fpga : 2;
    unsigned rsv0 : 30;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0104)
    unsigned cfg_init_0 : 1;
    unsigned cfg_done_0 : 1;
    unsigned fifo_prog_full_0 : 1;
    unsigned fifo_prog_empty_0 : 1;
    unsigned rsv0 : 12;
    unsigned cfg_init_1 : 1;
    unsigned cfg_done_1 : 1;
    unsigned fifo_prog_full_1 : 1;
    unsigned fifo_prog_empty_1 : 1;
    unsigned rsv1 : 12;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0120)
    unsigned cfg_done : 1;
    unsigned rsv0 : 31;
REG_DEF_CPT_SUFFIX_32

} // namespace ns_reg_sp9500pro_s6

#endif // REG_DEF_SP9500X_H
