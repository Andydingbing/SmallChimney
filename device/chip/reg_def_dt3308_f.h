#ifndef INCLUDE_REG_DEF_DT3308_F_H
#define INCLUDE_REG_DEF_DT3308_F_H

#include "reg_def.h"

#ifdef _DEBUG
#define KUS_ASSERT() \
    if (m_k7 == nullptr) { \
        Log.set_last_err("device:kus disconnect"); \
        return -1; \
    }
#else
#define KUS_ASSERT()
#endif

#define KUS__W(ns,addr) \
    do {KUS_ASSERT();  int32_t ret = 0; \
        if ((ret=m_k7->w32(pci_dev::AS_BAR0,uint32_t(addr<<2),REG_DATA(ns,addr)))) { \
            Log.set_last_err("%s:%s line%d",__FILE__,BOOST_CURRENT_FUNCTION,__LINE__); \
            return -1; \
        } Log.add_reg(ret,m_k7->descriptor(),addr,REG_DATA(ns,addr)); \
    }while (0);

#define KUS__R(ns,addr) \
    do {KUS_ASSERT(); int32_t ret = 0; \
        if ((ret=m_k7->r32(pci_dev::AS_BAR0,uint32_t(addr<<2),&REG_DATA(ns,addr)))) { \
            Log.set_last_err("%s:%s line%d",__FILE__,BOOST_CURRENT_FUNCTION,__LINE__); \
            return -1; \
        } Log.add_reg(ret,m_k7->descriptor(),addr,0xffffffff,REG_DATA(ns,addr)); \
    }while (0);

#define KUSWAIT_IDLE(ns,addr,bit,flag,times) \
    do {bool idle = false; KUS__R(ns,addr); \
        for (int32_t i = 0;i < times;i ++) { \
            if (flag == REG(ns,addr).bit) { \
                idle = true; \
                break; \
            } \
            KUS__R(ns,addr); \
        } \
        if (!idle) { \
            Log.set_last_err("reg%#06x wait idle timeout",addr); \
            return -1; \
        } \
    } while (0);

#define KUS_REG_DECLARE(addr) REG_DECL(ns_reg_kus,addr)
#define KUS_REG(addr) REG(ns_reg_kus,addr)
#define KUS_REG_U32(addr) REG_U32(ns_reg_kus,addr)
#define KUS_REG_CLEAR(addr) REG_CLEAR(ns_reg_kus,addr)
#define KUSOP(addr,bit) K7OP(ns_reg_kus,addr,bit)
#define KUS_OP(addr) K7_OP(ns_reg_kus,addr)
#define KUS_OP_DELAY(addr,time) K7_OP_DELAY(ns_reg_kus,addr,time)
#define KUS_RE(addr) K7_RE(ns_reg_kus,addr)
#define KUS_FE(addr) K7_FE(ns_reg_kus,addr)
#define KUS_WAIT_IDLE(addr,flag,times) KUSWAIT_IDLE(ns_reg_kus,addr,busy,flag,times)
#define KUS_WAIT_IDLE_BUSY2(addr,flag,times) K7_WAIT_IDLE_BUSY2(ns_reg_kus,addr,flag,times)
#define KUS_W(addr) KUS__W(ns_reg_kus,addr)
#define KUS_R(addr) KUS__R(ns_reg_kus,addr)

namespace ns_reg_kus {

REG_DEF_CPT_PREFIX_32(0x800008)
    unsigned op : 1;
    unsigned rsv0 : 31;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x800009)
    unsigned length : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x80000b)
    unsigned trig : 8;
    unsigned src : 8;
    unsigned rsv0 : 16;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x80000c)
    unsigned trig : 1;
    unsigned busy : 1;
    unsigned rsv0 : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x900000)
    unsigned en : 1;
    unsigned rsv0 : 31;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x900001)
    unsigned en : 1;
    unsigned rsv0 : 31;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x900004)
    unsigned src : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x900005)
    unsigned sr : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_PREFIX_32(0x960000)
REG_1_PREFIX(epld)
    unsigned data : 16;
    unsigned addr : 8 ;
    unsigned rsv0 : 7;
    unsigned wr : 1;
REG_1_SUFFIX(epld)
REG_2_PREFIX(pll)
    unsigned rsv0 : 1;
    unsigned data : 24;
    unsigned addr : 6;
    unsigned wr : 1;
REG_2_SUFFIX(pll)
REG_3_PREFIX(vco)
    unsigned rsv0 : 4;
    unsigned addr_5 : 4;
    unsigned data : 16; // vco subsystem reg data
    unsigned rsv1 : 1;
    unsigned addr : 7; // = 0x05
REG_3_SUFFIX(vco)
REG_DEF_SUFFIX_32

/* EPLD reg write mode
 * sw :
 * 0 : data write to reg will be redirected to other devices
 * 1 : just set reg
 */
REG_DEF_CPT_PREFIX_32(0x960001)
    unsigned sw : 1;
    unsigned rsv0 : 31;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x960002)
    unsigned data : 24;
    unsigned addr : 4;
    unsigned rsv0 : 3;
    unsigned busy : 1;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x960010)
    unsigned data : 8;
    unsigned addr : 7;
    unsigned op : 1;
    unsigned rsv0 : 16;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x970100)
    unsigned comp : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x970101)
    unsigned comp : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x970102)
    unsigned ddc : 30;
    unsigned rsv0 : 2;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x970103)
    unsigned duc : 29;
    unsigned rsv0 : 3;
REG_DEF_CPT_SUFFIX_32

}

#endif // INCLUDE_REG_DEF_DT3308_F_H
