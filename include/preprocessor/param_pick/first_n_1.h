/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option,sep) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef INCLUDE_PREPROCESSOR_PARAM_PICK_FIRST_N_1_H
#define INCLUDE_PREPROCESSOR_PARAM_PICK_FIRST_N_1_H

#define PP_VAR_FIRST_N_1_2(sep,prefix,suffix,_1,_2) \
        prefix _1 suffix

#define PP_VAR_FIRST_N_1_3(sep,prefix,suffix,_1,_2,_3) \
        PP_VAR_FIRST_N_1_2(sep,prefix,suffix,_1,_2) sep  prefix _2 suffix

#define PP_VAR_FIRST_N_1_4(sep,prefix,suffix,_1,_2,_3,_4) \
        PP_VAR_FIRST_N_1_3(sep,prefix,suffix,_1,_2,_3) sep prefix _3 suffix

#define PP_VAR_FIRST_N_1_5(sep,prefix,suffix,_1,_2,_3,_4,_5) \
        PP_VAR_FIRST_N_1_4(sep,prefix,suffix,_1,_2,_3,_4) sep prefix _4 suffix

#define PP_VAR_FIRST_N_1_6(sep,prefix,suffix,_1,_2,_3,_4,_5,_6) \
        PP_VAR_FIRST_N_1_5(sep,prefix,suffix,_1,_2,_3,_4,_5) sep prefix _5 suffix

#define PP_VAR_FIRST_N_1_7(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7) \
        PP_VAR_FIRST_N_1_6(sep,prefix,suffix,_1,_2,_3,_4,_5,_6) sep prefix _6 suffix

#define PP_VAR_FIRST_N_1_8(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7,_8) \
        PP_VAR_FIRST_N_1_7(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7) sep prefix _7 suffix

#define PP_VAR_FIRST_N_1_9(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7,_8,_9) \
        PP_VAR_FIRST_N_1_8(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7,_8) sep prefix _8 suffix

#define PP_VAR_FIRST_N_1_10(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10) \
        PP_VAR_FIRST_N_1_9(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7,_8,_9) sep prefix _9 suffix

#define PP_VAR_FIRST_N_1_11(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11) \
        PP_VAR_FIRST_N_1_10(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10) sep prefix _10 suffix

#define PP_VAR_FIRST_N_1_12(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12) \
        PP_VAR_FIRST_N_1_11(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11) sep prefix _11 suffix

#define PP_VAR_FIRST_N_1_13(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13) \
        PP_VAR_FIRST_N_1_12(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12) sep prefix _12 suffix

#define PP_VAR_FIRST_N_1_14(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14) \
        PP_VAR_FIRST_N_1_13(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13) sep prefix _13 suffix

#define PP_VAR_FIRST_N_1_15(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15) \
        PP_VAR_FIRST_N_1_14(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14) sep prefix _14 suffix

#define PP_VAR_FIRST_N_1_16(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16) \
        PP_VAR_FIRST_N_1_15(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15) sep prefix _15 suffix

#define PP_VAR_FIRST_N_1_17(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17) \
        PP_VAR_FIRST_N_1_16(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16) sep prefix _16 suffix

#define PP_VAR_FIRST_N_1_18(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18) \
        PP_VAR_FIRST_N_1_17(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17) sep prefix _17 suffix

#define PP_VAR_FIRST_N_1_19(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19) \
        PP_VAR_FIRST_N_1_18(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18) sep prefix _18 suffix

#define PP_VAR_FIRST_N_1_20(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20) \
        PP_VAR_FIRST_N_1_19(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19) sep prefix _19 suffix

#define PP_VAR_FIRST_N_1_21(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21) \
        PP_VAR_FIRST_N_1_20(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20) sep prefix _20 suffix

#define PP_VAR_FIRST_N_1_22(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22) \
        PP_VAR_FIRST_N_1_21(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21) sep prefix _21 suffix

#define PP_VAR_FIRST_N_1_23(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23) \
        PP_VAR_FIRST_N_1_22(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22) sep prefix _22 suffix

#define PP_VAR_FIRST_N_1_24(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24) \
        PP_VAR_FIRST_N_1_23(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23) sep prefix _23 suffix

#define PP_VAR_FIRST_N_1_25(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25) \
        PP_VAR_FIRST_N_1_24(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24) sep prefix _24 suffix

#define PP_VAR_FIRST_N_1_26(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26) \
        PP_VAR_FIRST_N_1_25(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25) sep prefix _25 suffix

#define PP_VAR_FIRST_N_1_27(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27) \
        PP_VAR_FIRST_N_1_26(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26) sep prefix _26 suffix

#define PP_VAR_FIRST_N_1_28(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28) \
        PP_VAR_FIRST_N_1_27(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27) sep prefix _27 suffix

#define PP_VAR_FIRST_N_1_29(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29) \
        PP_VAR_FIRST_N_1_28(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28) sep prefix _28 suffix

#define PP_VAR_FIRST_N_1_30(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30) \
        PP_VAR_FIRST_N_1_29(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29) sep prefix _29 suffix

#define PP_VAR_FIRST_N_1_31(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31) \
        PP_VAR_FIRST_N_1_30(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30) sep prefix _30 suffix

#define PP_VAR_FIRST_N_1_32(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32) \
        PP_VAR_FIRST_N_1_31(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31) sep prefix _31 suffix

#define PP_VAR_FIRST_N_1_33(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33) \
        PP_VAR_FIRST_N_1_32(sep,prefix,suffix,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32) sep prefix _32 suffix

#endif
