#ifndef INCLUDE_REG_DEF_3308_F_EPLD_H
#define INCLUDE_REG_DEF_3308_F_EPLD_H

#include "reg_def.h"

#define EPLD_REG_DECL(addr) REG_DECL(ns_epld,addr)
#define EPLD_REG_DATA(addr) REG_DATA(ns_epld,addr)
#define EPLD_REG(addr) REG(ns_epld,addr)

namespace ns_epld {

REG_DEF_CPT_PREFIX_8(0x10)
    unsigned sw_10_2_v2 : 1;
    unsigned sw_10_2_v1 : 1;
    unsigned sw_10_1_v2 : 1;
    unsigned sw_10_1_v1 : 1;
    unsigned sw_9 : 1;
    unsigned rsv0 : 3;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x13)
    unsigned att : 5;
    unsigned rsv0 : 2;
    unsigned busy : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x14)
    unsigned att : 5;
    unsigned rsv0 : 2;
    unsigned busy : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x15)
    unsigned att : 5;
    unsigned rsv0 : 2;
    unsigned busy : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x16)
    unsigned att : 5;
    unsigned rsv0 : 2;
    unsigned busy : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x17)
    unsigned att : 5;
    unsigned rsv0 : 2;
    unsigned busy : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x18)
    unsigned att : 5;
    unsigned rsv0 : 2;
    unsigned busy : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x23)
    unsigned att : 5;
    unsigned rsv0 : 2;
    unsigned busy : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x24)
    unsigned att : 5;
    unsigned rsv0 : 2;
    unsigned busy : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x25)
    unsigned att : 1;
    unsigned lna : 1;
    unsigned rsv0 : 6;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x26)
    unsigned att : 7;
    unsigned busy : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x27)
    unsigned att : 7;
    unsigned busy : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x28)
    unsigned att : 7;
    unsigned busy : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x29)
    unsigned att : 7;
    unsigned busy : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x2a)
    unsigned att : 7;
    unsigned busy : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x2b)
    unsigned att : 7;
    unsigned busy : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x2c)
    unsigned att : 7;
    unsigned busy : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x2d)
    unsigned att : 7;
    unsigned busy : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x33)
    unsigned pll : 4;
    unsigned rsv0 : 4;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x3e)
    unsigned sw : 4;
    unsigned rsv0 : 4;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x50)
    unsigned rsv0 : 7;
    unsigned en : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x51)
    unsigned aux_sw1 : 1;
    unsigned aux_sw0 : 1;
    unsigned main_0_3_sw1 : 1;
    unsigned main_0_3_sw0 : 1;
    unsigned main_4_7_sw1 : 1;
    unsigned main_4_7_sw0 : 1;
    unsigned rsv0 : 2;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x52)
    unsigned _0 : 1;
    unsigned _1 : 1;
    unsigned _2 : 1;
    unsigned _3 : 1;
    unsigned _4 : 1;
    unsigned _5 : 1;
    unsigned _6 : 1;
    unsigned _7 : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x53)
    unsigned rsv0 : 6;
    unsigned sw_tx_main_aux : 1; // port 1~8/9
    unsigned en_tx : 1; // all port
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x54)
    unsigned _0 : 1;
    unsigned _1 : 1;
    unsigned _2 : 1;
    unsigned _3 : 1;
    unsigned _4 : 1;
    unsigned _5 : 1;
    unsigned _6 : 1;
    unsigned _7 : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x56)
    unsigned att : 1;
    unsigned lna : 1;
    unsigned rsv0 : 6;
REG_DEF_CPT_SUFFIX_8

} // ns_epld

#endif // INCLUDE_REG_DEF_3308_F_EPLD_H
