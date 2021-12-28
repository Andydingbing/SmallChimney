#ifndef INCLUDE_REG_DEF_H
#define INCLUDE_REG_DEF_H

#define CONFIG_FILE_PATH "config.ini"

#include "liblog.h"
#include "preprocessor/preprocessor.h"
#include <boost/current_function.hpp>

/*
 * Usage :
 *
 * 1, Define Register
 *
 *    REG_DEF_PREFIX_32(0x0000)
 *        unsigned x : 16;
 *        unsigned y : 16;
 *    REG_DEF_SUFFIX_32
 *
 *    REG_DEF_PREFIX_32(0x0001,name)
 *        unsigned x : 16;
 *        unsigned y : 16;
 *    REG_DEF_SUFFIX_32
 *
 *    REG_DEF_PREFIX_32_2(0x0002,0x0003)
 *        unsigned x : 16;
 *        unsigned y : 16;
 *    REG_DEF_SUFFIX_32_2
 *
 *    REG_DEF_PREFIX_32_2(0x0004,0x0005,name)
 *        unsigned x : 16;
 *        unsigned y : 16;
 *    REG_DEF_SUFFIX_32_2
 *
 * 2, Reference
 *
 *    REG_DECL(0x0000);
 *    REG_DECL(name);
 *
 *    REG(0x0000).x = 1;
 *    REG(0x0000).y = 2;
 *
 *    REG(name).x = 3;
 *    REG(name).y = 4;
 *
 *    REG_2(0x0002,0x0003).x = 5;
 *    REG_2(0x0002,0x0003).y = 6;
 *
 *    REG_2(name).x = 7;
 *    REG_2(name).y = 8;
 *    ...
 *    ...
 *
 * Details :
 *
 * 1 address
 *
 * union reg_addr {
 *     struct reg_1_t {
 *         a : x;
 *         b : x; field(some_chip)
 *     } r1;
 *     struct reg_2_t {
 *         a : x;
 *         b : x;
 *     } r2;
 *     ...
 *     uintx_t data;
 * };
 *
 * union reg_name {
 *     static CONSTEXPR_OR_CONST uintx_t offset_name = addr;
 *     struct reg_1_t {
 *         a : x;
 *         b : x; field(some_chip)
 *     } r1;
 *     struct reg_2_t {
 *         a : x;
 *         b : x;
 *     } r2;
 *     ...
 *     uintx_t data;
 * };
 *
 *
 * 2 addresses
 *
 * struct reg_0x..._0x..._t {
 *     uintx_t addr[4];
 *     union reg {
 *         struct reg_1_t {
 *             a : x;
 *             b : x; field(some_chip)
 *         } r1;
 *         struct reg_2_t {
 *             a : x;
 *             b : x;
 *         } r2;
 *         ...
 *         uintx_t data;
 *     } u_data;
 * };
 *
 * struct reg_name_t {
 *     uintx_t addr[4];
 *     union reg {
 *         struct reg_1_t {
 *             a : x;
 *             b : x; field(some_chip)
 *         } r1;
 *         struct reg_2_t {
 *             a : x;
 *             b : x;
 *         } r2;
 *         ...
 *         uintx_t data;
 *     } u_data;
 * };
 *
 */


#define field(chips)

#define REG_DEF_APPLY(prefix,param_num) MACRO_ID(prefix(param_num))

#define REG_DEF_ARG_1(arg,...) arg
#define REG_DEF_ARG_2(_1,...) __VA_ARGS__

#define REG_DEF_PREFIX_ARG_1(...) MACRO_ID(REG_DEF_ARG_1(__VA_ARGS__))
#define REG_DEF_PREFIX_ARG_2(...) MACRO_ID(REG_DEF_ARG_2(__VA_ARGS__))

#define REG_PREFIX struct reg_t {
#define REG_SUFFIX } r;

#define REG_1_PREFIX(chip_or_field) struct reg_1_t {
#define REG_1_SUFFIX(chip_or_field) } r_##chip_or_field;

#define REG_2_PREFIX(chip_or_field) struct reg_2_t {
#define REG_2_SUFFIX(chip_or_field) } r_##chip_or_field;

#define REG_3_PREFIX(chip_or_field) struct reg_3_t {
#define REG_3_SUFFIX(chip_or_field) } r_##chip_or_field;


#define REG_DEF_PREFIX_8(addr_or_name)  union reg_##addr_or_name {
#define REG_DEF_PREFIX_16(addr_or_name) union reg_##addr_or_name {
#define REG_DEF_PREFIX_32(addr_or_name) union reg_##addr_or_name {
#define REG_DEF_PREFIX_64(addr_or_name) union reg_##addr_or_name {

#define REG_DEF_SUFFIX_8  uint8_t  data; };
#define REG_DEF_SUFFIX_16 uint16_t data; };
#define REG_DEF_SUFFIX_32 uint32_t data; };
#define REG_DEF_SUFFIX_64 uint64_t data; };

// compatible
#define REG_DEF_CPT_PREFIX_8_PARAM_1(addr)  REG_DEF_PREFIX_8(addr)  REG_PREFIX
#define REG_DEF_CPT_PREFIX_16_PARAM_1(addr) REG_DEF_PREFIX_16(addr) REG_PREFIX
#define REG_DEF_CPT_PREFIX_32_PARAM_1(addr) REG_DEF_PREFIX_32(addr) REG_PREFIX
#define REG_DEF_CPT_PREFIX_64_PARAM_1(addr) REG_DEF_PREFIX_64(addr) REG_PREFIX

#define REG_DEF_CPT_PREFIX_8_PARAM_2(addr,name)  \
    REG_DEF_PREFIX_8(name)  \
    static CONSTEXPR_OR_CONST uint8_t offset_##name = addr; \
    REG_PREFIX

#define REG_DEF_CPT_PREFIX_16_PARAM_2(addr,name) \
    REG_DEF_PREFIX_16(name)  \
    static CONSTEXPR_OR_CONST uint16_t offset_##name = addr; \
    REG_PREFIX

#define REG_DEF_CPT_PREFIX_32_PARAM_2(addr,name) \
    REG_DEF_PREFIX_32(name)  \
    static CONSTEXPR_OR_CONST uint32_t offset_##name = addr; \
    REG_PREFIX

#define REG_DEF_CPT_PREFIX_64_PARAM_2(addr,name) \
    REG_DEF_PREFIX_64(name)  \
    static CONSTEXPR_OR_CONST uint64_t offset_##name = addr; \
    REG_PREFIX

#define REG_DEF_CPT_PREFIX_8_VAR_COUNT(count)  REG_DEF_CPT_PREFIX_8_PARAM_##count
#define REG_DEF_CPT_PREFIX_16_VAR_COUNT(count) REG_DEF_CPT_PREFIX_16_PARAM_##count
#define REG_DEF_CPT_PREFIX_32_VAR_COUNT(count) REG_DEF_CPT_PREFIX_32_PARAM_##count
#define REG_DEF_CPT_PREFIX_64_VAR_COUNT(count) REG_DEF_CPT_PREFIX_64_PARAM_##count

#define REG_DEF_CPT_PREFIX_8(...) \
    MACRO_ID(REG_DEF_APPLY(REG_DEF_CPT_PREFIX_8_VAR_COUNT,PP_COUNT(__VA_ARGS__))(__VA_ARGS__))

#define REG_DEF_CPT_PREFIX_16(...) \
    MACRO_ID(REG_DEF_APPLY(REG_DEF_CPT_PREFIX_16_VAR_COUNT,PP_COUNT(__VA_ARGS__))(__VA_ARGS__))

#define REG_DEF_CPT_PREFIX_32(...) \
    MACRO_ID(REG_DEF_APPLY(REG_DEF_CPT_PREFIX_32_VAR_COUNT,PP_COUNT(__VA_ARGS__))(__VA_ARGS__))

#define REG_DEF_CPT_PREFIX_64(...) \
    MACRO_ID(REG_DEF_APPLY(REG_DEF_CPT_PREFIX_64_VAR_COUNT,PP_COUNT(__VA_ARGS__))(__VA_ARGS__))

#define REG_DEF_CPT_SUFFIX_8  REG_SUFFIX REG_DEF_SUFFIX_8
#define REG_DEF_CPT_SUFFIX_16 REG_SUFFIX REG_DEF_SUFFIX_16
#define REG_DEF_CPT_SUFFIX_32 REG_SUFFIX REG_DEF_SUFFIX_32
#define REG_DEF_CPT_SUFFIX_64 REG_SUFFIX REG_DEF_SUFFIX_64

#define REG_DECL_INIT(ns,addr_or_name,init) \
    union ns::reg_##addr_or_name ns##r##addr_or_name; \
    ns##r##addr_or_name.data = init; NEED_SEMICOLON

#define REG_DECL(ns,addr_or_name) REG_DECL_INIT(ns,addr_or_name,0)
#define REG_DATA(ns,addr_or_name) ns##r##addr_or_name.data
#define REG_CLER(ns,addr_or_name) REG_DATA(ns,addr_or_name) = 0;
#define REG(ns,addr_or_name)  ns##r##addr_or_name.r
#define REG_OF(ns,addr_or_name,chip_or_field) ns##r##addr_or_name.r_##chip_or_field


#define REG_DEF_PREFIX_8_2_PARAM_2(addr0,addr1)  struct reg_##addr0##_##addr1##_t {
#define REG_DEF_PREFIX_16_2_PARAM_2(addr0,addr1) struct reg_##addr0##_##addr1##_t {
#define REG_DEF_PREFIX_32_2_PARAM_2(addr0,addr1) struct reg_##addr0##_##addr1##_t {
#define REG_DEF_PREFIX_64_2_PARAM_2(addr0,addr1) struct reg_##addr0##_##addr1##_t {

#define REG_DEF_PREFIX_8_2_PARAM_3(addr0,addr1,name)   struct reg_##name##_t {
#define REG_DEF_PREFIX_16_2_PARAM_3(addr0,addr1,name)  struct reg_##name##_t {
#define REG_DEF_PREFIX_32_2_PARAM_3(addr0,addr1,name)  struct reg_##name##_t {
#define REG_DEF_PREFIX_64_2_PARAM_3(addr0,addr1,name)  struct reg_##name##_t {

#define REG_DEF_SUFFIX_8_2  uint8_t data;  } u_data; };
#define REG_DEF_SUFFIX_16_2 uint16_t data; } u_data; };
#define REG_DEF_SUFFIX_32_2 uint32_t data; } u_data; };
#define REG_DEF_SUFFIX_64_2 uint64_t data; } u_data; };

// compatible
#define REG_DEF_CPT_PREFIX_8_2_PARAM_2(addr0,addr1)  \
    REG_DEF_PREFIX_8_2_PARAM_2(addr0,addr1) \
        uint8_t  addr[4] = {addr1,addr0,addr1,addr0}; \
        union reg { REG_PREFIX

#define REG_DEF_CPT_PREFIX_16_2_PARAM_2(addr0,addr1) \
    REG_DEF_PREFIX_16_2_PARAM_2(addr0,addr1) \
        uint16_t addr[4] = {addr1,addr0,addr1,addr0}; \
        union reg { REG_PREFIX

#define REG_DEF_CPT_PREFIX_32_2_PARAM_2(addr0,addr1) \
    REG_DEF_PREFIX_32_2_PARAM_2(addr0,addr1) \
        uint32_t addr[4] = {addr1,addr0,addr1,addr0}; \
        union reg { REG_PREFIX

#define REG_DEF_CPT_PREFIX_64_2_PARAM_2(addr0,addr1) \
    REG_DEF_PREFIX_64_2_PARAM_2(addr0,addr1) \
        uint64_t addr[4] = {addr1,addr0,addr1,addr0}; \
        union reg { REG_PREFIX


#define REG_DEF_CPT_PREFIX_8_2_PARAM_3(addr0,addr1,name)  \
    REG_DEF_PREFIX_8_2_PARAM_3(addr0,addr1,name) \
        uint8_t  addr[4] = {addr1,addr0,addr1,addr0}; \
        union reg { REG_PREFIX

#define REG_DEF_CPT_PREFIX_16_2_PARAM_3(addr0,addr1,name) \
    REG_DEF_PREFIX_16_2_PARAM_3(addr0,addr1,name) \
        uint16_t addr[4] = {addr1,addr0,addr1,addr0}; \
        union reg { REG_PREFIX

#define REG_DEF_CPT_PREFIX_32_2_PARAM_3(addr0,addr1,name) \
    REG_DEF_PREFIX_32_2_PARAM_3(addr0,addr1,name) \
        uint32_t addr[4] = {addr1,addr0,addr1,addr0}; \
        union reg { REG_PREFIX

#define REG_DEF_CPT_PREFIX_64_2_PARAM_3(addr0,addr1,name) \
    REG_DEF_PREFIX_64_2_PARAM_3(addr0,addr1,name) \
        uint64_t addr[4] = {addr1,addr0,addr1,addr0}; \
        union reg { REG_PREFIX

#define REG_DEF_CPT_PREFIX_8_2_VAR_COUNT(count)  REG_DEF_CPT_PREFIX_8_2_PARAM_##count
#define REG_DEF_CPT_PREFIX_16_2_VAR_COUNT(count) REG_DEF_CPT_PREFIX_16_2_PARAM_##count
#define REG_DEF_CPT_PREFIX_32_2_VAR_COUNT(count) REG_DEF_CPT_PREFIX_32_2_PARAM_##count
#define REG_DEF_CPT_PREFIX_64_2_VAR_COUNT(count) REG_DEF_CPT_PREFIX_64_2_PARAM_##count

#define REG_DEF_CPT_PREFIX_8_2(...)  \
    MACRO_ID(REG_DEF_APPLY(REG_DEF_CPT_PREFIX_8_2_VAR_COUNT,PP_COUNT(__VA_ARGS__))(__VA_ARGS__))

#define REG_DEF_CPT_PREFIX_16_2(...) \
    MACRO_ID(REG_DEF_APPLY(REG_DEF_CPT_PREFIX_16_2_VAR_COUNT,PP_COUNT(__VA_ARGS__))(__VA_ARGS__))

#define REG_DEF_CPT_PREFIX_32_2(...) \
    MACRO_ID(REG_DEF_APPLY(REG_DEF_CPT_PREFIX_32_2_VAR_COUNT,PP_COUNT(__VA_ARGS__))(__VA_ARGS__))

#define REG_DEF_CPT_PREFIX_64_2(...) \
    MACRO_ID(REG_DEF_APPLY(REG_DEF_CPT_PREFIX_64_2_VAR_COUNT,PP_COUNT(__VA_ARGS__))(__VA_ARGS__))

#define REG_DEF_CPT_SUFFIX_8_2  REG_SUFFIX REG_DEF_SUFFIX_8_2
#define REG_DEF_CPT_SUFFIX_16_2 REG_SUFFIX REG_DEF_SUFFIX_16_2
#define REG_DEF_CPT_SUFFIX_32_2 REG_SUFFIX REG_DEF_SUFFIX_32_2
#define REG_DEF_CPT_SUFFIX_64_2 REG_SUFFIX REG_DEF_SUFFIX_64_2

#define REG_DECL_INIT_2_VAR_COUNT(count) REG_DECL_INIT_2_PARAM_##count
#define REG_DECL_2_VAR_COUNT(count) REG_DECL_2_PARAM_##count
#define REG_DATA_2_VAR_COUNT(count) REG_DATA_2_PARAM_##count
#define REG_CLER_2_VAR_COUNT(count) REG_CLER_2_PARAM_##count
#define REG_2_VAR_COUNT(count) REG_2_PARAM_##count

#define REG_DECL_INIT_2_PARAM_2(ns,name,init) \
    struct ns::reg_##name##_t ns##r##name; \
    ns##r##name.u_data.data = init; NEED_SEMICOLON

#define REG_DECL_INIT_2_PARAM_3(ns,addr0,addr1,init) \
    struct ns::reg_##addr0##_##addr1##_t ns##r##addr0##addr1; \
    ns##r##addr0##addr1.u_data.data = init; NEED_SEMICOLON

#define REG_DECL_2_PARAM_1(ns,name) REG_DECL_INIT_2_PARAM_2(ns,name,0)
#define REG_DATA_2_PARAM_1(ns,name) ns##r##name.u_data.data
#define REG_CLER_2_PARAM_1(ns,name) ns##r##name.u_data.data = 0;
#define REG_2_PARAM_1(ns,name)      ns##r##name.u_data.r

#define REG_DECL_2_PARAM_2(ns,addr0,addr1) REG_DECL_INIT_2_PARAM_3(ns,addr0,addr1,0)
#define REG_DATA_2_PARAM_2(ns,addr0,addr1) ns##r##addr0##addr1.u_data.data
#define REG_CLER_2_PARAM_2(ns,addr0,addr1) ns##r##addr0##addr1.u_data.data = 0;
#define REG_2_PARAM_2(ns,addr0,addr1)      ns##r##addr0##addr1.u_data.r

#define REG_DECL_INIT_2(ns,...) \
    MACRO_ID(REG_DEF_APPLY(REG_DECL_INIT_2_VAR_COUNT,PP_COUNT(__VA_ARGS__))(ns,__VA_ARGS__))

#define REG_DECL_2(ns,...) \
    MACRO_ID(REG_DEF_APPLY(REG_DECL_2_VAR_COUNT,PP_COUNT(__VA_ARGS__))(ns,__VA_ARGS__))

#define REG_DATA_2(ns,...) \
    MACRO_ID(REG_DEF_APPLY(REG_DATA_2_VAR_COUNT,PP_COUNT(__VA_ARGS__))(ns,__VA_ARGS__))

#define REG_CLER_2(ns,...) \
    MACRO_ID(REG_DEF_APPLY(REG_CLER_2_VAR_COUNT,PP_COUNT(__VA_ARGS__))(ns,__VA_ARGS__))

#define REG_2(ns,...) \
    MACRO_ID(REG_DEF_APPLY(REG_2_VAR_COUNT,PP_COUNT(__VA_ARGS__))(ns,__VA_ARGS__))

#define REG_OF_2(ns,addr0,addr1,chip_or_field) ns##r##addr0##addr1.r_##chip_or_field


// IO
#define PCIE_DEV_W(dev,bar,ns,addr) \
    do {int32_t ret = 0; \
        if ((ret = dev->w32(bar,addr<<2,REG_DATA(ns,addr)))) { \
            Log.set_last_err("%s:%s line%d",__FILE__,BOOST_CURRENT_FUNCTION,__LINE__); \
            return ret; \
        } \
        Log.add_reg(ret,dev->descriptor(),addr,REG_DATA(ns,addr)); \
    } while (0); NEED_SEMICOLON

#define PCIE_DEV_R(dev,bar,ns,addr) \
    do {int32_t ret = 0; \
        if ((ret = dev->r32(bar,addr<<2,&REG_DATA(ns,addr)))) { \
            Log.set_last_err("%s:%s line%d",__FILE__,BOOST_CURRENT_FUNCTION,__LINE__); \
            return ret; \
        } \
        Log.add_reg(ret,dev->descriptor(),addr,0xffffffff,REG_DATA(ns,addr)); \
    } while (0); NEED_SEMICOLON

#define PCIE_DEV_W_2(dev,bar,ns,addr0,addr1) \
    do {int32_t ret = 0; \
        uint32_t addr = ns##r##addr0##addr1.addr[_rf_idx]; \
        if ((ret = dev->w32(bar,addr<<2,REG_DATA_2(ns,addr0,addr1)))) { \
            Log.set_last_err("%s:%s line%d",__FILE__,BOOST_CURRENT_FUNCTION,__LINE__); \
            return ret; \
        } \
        Log.add_reg(ret,dev->descriptor(),addr,REG_DATA_2(ns,addr0,addr1)); \
    }while (0); NEED_SEMICOLON

#define PCIE_DEV_R_2(dev,bar,ns,addr0,addr1) \
    do {int32_t ret = 0; \
        uint32_t addr = ns##r##addr0##addr1.addr[_rf_idx]; \
        if ((ret = dev->r32(bar,addr<<2,&REG_DATA_2(ns,addr0,addr1)))) { \
            Log.set_last_err("%s:%s line%d",__FILE__,BOOST_CURRENT_FUNCTION,__LINE__); \
            return ret; \
        } \
        Log.add_reg(ret,dev->descriptor(),addr,0xffffffff,REG_DATA_2(ns,addr0,addr1)); \
    }while (0); NEED_SEMICOLON

#define PCIE_DEV_SET_BITS(dev,addr,bits,data) \
    dev##_R(addr); \
    dev##_REG(addr).bits = data; \
    dev##_W(addr);

#define PCIE_DEV_SET_BITS_2(dev,addr0,addr1,bits,data) \
    dev##_R_2(addr0,addr1); \
    dev##_REG_2(addr0,addr1).bits = data; \
    dev##_W_2(addr0,addr1);

// Rising edge
#define CTRLLER_RE(ctrller,addr,bit) \
    do {ctrller##_REG(addr).bit = 0; \
        ctrller##_W(addr); \
        ctrller##_REG(addr).bit = 1; \
        ctrller##_W(addr); \
    } while (0); NEED_SEMICOLON

#define CTRLLER_RE_2(ctrller,addr0,addr1,bit) \
    do {ctrller##_REG_2(addr0,addr1).bit = 0; \
        ctrller##_W_2(addr0,addr1); \
        ctrller##_REG_2(addr0,addr1).bit = 1; \
        ctrller##_W_2(addr0,addr1); \
    } while (0); NEED_SEMICOLON

// State machine
#define CTRLLER_WAIT_IDLE(ctrller,addr,bit,flag,times) \
    do {bool idle = false; \
        ctrller##_R(addr); \
        for (int32_t i = 0;i < times;++i) { \
            if (flag == ctrller##_REG(addr).bit) { \
                idle = true; \
                break; \
            } \
            ctrller##_R(addr); \
        } \
        if (!idle) { \
            Log.set_last_err("Reg%#06x wait idle timeout",addr); \
            return -1; \
        } \
    } while (0); NEED_SEMICOLON

#define CTRLLER_WAIT_IDLE_2(ctrller,addr0,addr1,bit,flag,times) \
    do {bool idle = false; \
        ctrller##_R_2(addr0,addr1); \
        for (int32_t i = 0;i < times;++i) { \
            if (flag == ctrller##_REG_2(addr0,addr1).bit) { \
                idle = true; \
                break; \
            } \
            ctrller##_R_2(addr0,addr1); \
        } \
        if (!idle) { \
            Log.set_last_err("Reg%#06x%#06x wait idle timeout",addr0,addr1); \
            return -1; \
        } \
    } while (0); NEED_SEMICOLON

#endif
