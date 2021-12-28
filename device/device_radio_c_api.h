#ifndef DEVICE_DEVICE_RADIO_C_API_H
#define DEVICE_DEVICE_RADIO_C_API_H

#include "preprocessor/preprocessor.h"

#define RADIO_C_API_PARAM_4(_1,_2,_3,prefix) \
    API _1 prefix ## _2 _3;

#define RADIO_C_API_PARAM_7(_1,_2,_3,_4,_5,_6,prefix) \
    API _1 prefix ## _2 _3; \
    API _4 prefix ## _5 _6;

#define RADIO_C_API_PARAM_10(_1,_2,_3,_4,_5,_6,_7,_8,_9,prefix) \
    API _1 prefix ## _2 _3; \
    API _4 prefix ## _5 _6; \
    API _7 prefix ## _8 _9;

#define RADIO_C_API_PARAM_13(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,prefix) \
    API _1 prefix ## _2 _3; \
    API _4 prefix ## _5 _6; \
    API _7 prefix ## _8 _9; \
    API _10 prefix ## _11 _12;

#define RADIO_C_API_PARAM_16(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,prefix) \
    API _1 prefix ## _2 _3; \
    API _4 prefix ## _5 _6; \
    API _7 prefix ## _8 _9; \
    API _10 prefix ## _11 _12; \
    API _13 prefix ## _14 _15;

#define RADIO_C_API_PARAM_19(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,prefix) \
    API _1 prefix ## _2 _3; \
    API _4 prefix ## _5 _6; \
    API _7 prefix ## _8 _9; \
    API _10 prefix ## _11 _12; \
    API _13 prefix ## _14 _15; \
    API _16 prefix ## _17 _18;

#define RADIO_C_API_PARAM_22(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,prefix) \
    API _1 prefix ## _2 _3; \
    API _4 prefix ## _5 _6; \
    API _7 prefix ## _8 _9; \
    API _10 prefix ## _11 _12; \
    API _13 prefix ## _14 _15; \
    API _16 prefix ## _17 _18; \
    API _19 prefix ## _20 _21;

#define RADIO_C_API_PARAM_25(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21, \
                               _22,_23,_24,prefix) \
    API _1 prefix ## _2 _3; \
    API _4 prefix ## _5 _6; \
    API _7 prefix ## _8 _9; \
    API _10 prefix ## _11 _12; \
    API _13 prefix ## _14 _15; \
    API _16 prefix ## _17 _18; \
    API _19 prefix ## _20 _21; \
    API _22 prefix ## _23 _24;

#define RADIO_C_API_PARAM_28(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21, \
                               _22,_23,_24,_25,_26,_27,prefix) \
    API _1 prefix ## _2 _3; \
    API _4 prefix ## _5 _6; \
    API _7 prefix ## _8 _9; \
    API _10 prefix ## _11 _12; \
    API _13 prefix ## _14 _15; \
    API _16 prefix ## _17 _18; \
    API _19 prefix ## _20 _21; \
    API _22 prefix ## _23 _24; \
    API _25 prefix ## _26 _27;

#define RADIO_C_API_PARAM_31(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21, \
                               _22,_23,_24,_25,_26,_27,_28,_29,_30,prefix) \
    API _1 prefix ## _2 _3; \
    API _4 prefix ## _5 _6; \
    API _7 prefix ## _8 _9; \
    API _10 prefix ## _11 _12; \
    API _13 prefix ## _14 _15; \
    API _16 prefix ## _17 _18; \
    API _19 prefix ## _20 _21; \
    API _22 prefix ## _23 _24; \
    API _25 prefix ## _26 _27; \
    API _28 prefix ## _29 _30;

#define RADIO_C_API_PARAM_34(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21, \
                               _22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,prefix) \
    API _1 prefix ## _2 _3; \
    API _4 prefix ## _5 _6; \
    API _7 prefix ## _8 _9; \
    API _10 prefix ## _11 _12; \
    API _13 prefix ## _14 _15; \
    API _16 prefix ## _17 _18; \
    API _19 prefix ## _20 _21; \
    API _22 prefix ## _23 _24; \
    API _25 prefix ## _26 _27; \
    API _28 prefix ## _29 _30; \
    API _31 prefix ## _32 _33;

#define RADIO_C_API_PARAM_37(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21, \
                               _22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,prefix) \
    API _1 prefix ## _2 _3; \
    API _4 prefix ## _5 _6; \
    API _7 prefix ## _8 _9; \
    API _10 prefix ## _11 _12; \
    API _13 prefix ## _14 _15; \
    API _16 prefix ## _17 _18; \
    API _19 prefix ## _20 _21; \
    API _22 prefix ## _23 _24; \
    API _25 prefix ## _26 _27; \
    API _28 prefix ## _29 _30; \
    API _31 prefix ## _32 _33; \
    API _34 prefix ## _35 _36;

#define RADIO_C_API_PARAM_40(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21, \
                               _22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39,prefix) \
    API _1 prefix ## _2 _3; \
    API _4 prefix ## _5 _6; \
    API _7 prefix ## _8 _9; \
    API _10 prefix ## _11 _12; \
    API _13 prefix ## _14 _15; \
    API _16 prefix ## _17 _18; \
    API _19 prefix ## _20 _21; \
    API _22 prefix ## _23 _24; \
    API _25 prefix ## _26 _27; \
    API _28 prefix ## _29 _30; \
    API _31 prefix ## _32 _33; \
    API _34 prefix ## _35 _36; \
    API _37 prefix ## _38 _39;

#define RADIO_C_API_PARAM_43(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21, \
                               _22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39, \
                               _40,_41,_42,prefix) \
    API _1 prefix ## _2 _3; \
    API _4 prefix ## _5 _6; \
    API _7 prefix ## _8 _9; \
    API _10 prefix ## _11 _12; \
    API _13 prefix ## _14 _15; \
    API _16 prefix ## _17 _18; \
    API _19 prefix ## _20 _21; \
    API _22 prefix ## _23 _24; \
    API _25 prefix ## _26 _27; \
    API _28 prefix ## _29 _30; \
    API _31 prefix ## _32 _33; \
    API _34 prefix ## _35 _36; \
    API _37 prefix ## _38 _39; \
    API _40 prefix ## _41 _42;

#define RADIO_C_API_PARAM_46(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21, \
                               _22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39, \
                               _40,_41,_42,_43,_44,_45,prefix) \
    API _1 prefix ## _2 _3; \
    API _4 prefix ## _5 _6; \
    API _7 prefix ## _8 _9; \
    API _10 prefix ## _11 _12; \
    API _13 prefix ## _14 _15; \
    API _16 prefix ## _17 _18; \
    API _19 prefix ## _20 _21; \
    API _22 prefix ## _23 _24; \
    API _25 prefix ## _26 _27; \
    API _28 prefix ## _29 _30; \
    API _31 prefix ## _32 _33; \
    API _34 prefix ## _35 _36; \
    API _37 prefix ## _38 _39; \
    API _40 prefix ## _41 _42; \
    API _43 prefix ## _44 _45;

#define RADIO_C_API_PARAM_49(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21, \
                               _22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39, \
                               _40,_41,_42,_43,_44,_45,_46,_47,_48,prefix) \
    API _1 prefix ## _2 _3; \
    API _4 prefix ## _5 _6; \
    API _7 prefix ## _8 _9; \
    API _10 prefix ## _11 _12; \
    API _13 prefix ## _14 _15; \
    API _16 prefix ## _17 _18; \
    API _19 prefix ## _20 _21; \
    API _22 prefix ## _23 _24; \
    API _25 prefix ## _26 _27; \
    API _28 prefix ## _29 _30; \
    API _31 prefix ## _32 _33; \
    API _34 prefix ## _35 _36; \
    API _37 prefix ## _38 _39; \
    API _40 prefix ## _41 _42; \
    API _43 prefix ## _44 _45; \
    API _46 prefix ## _47 _48;

#define RADIO_C_API_PARAM_52(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21, \
                               _22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39, \
                               _40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,prefix) \
    API _1 prefix ## _2 _3; \
    API _4 prefix ## _5 _6; \
    API _7 prefix ## _8 _9; \
    API _10 prefix ## _11 _12; \
    API _13 prefix ## _14 _15; \
    API _16 prefix ## _17 _18; \
    API _19 prefix ## _20 _21; \
    API _22 prefix ## _23 _24; \
    API _25 prefix ## _26 _27; \
    API _28 prefix ## _29 _30; \
    API _31 prefix ## _32 _33; \
    API _34 prefix ## _35 _36; \
    API _37 prefix ## _38 _39; \
    API _40 prefix ## _41 _42; \
    API _43 prefix ## _44 _45; \
    API _46 prefix ## _47 _48; \
    API _49 prefix ## _50 _51;

#define RADIO_C_API_PARAM_55(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21, \
                               _22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39, \
                               _40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,prefix) \
    API _1 prefix ## _2 _3; \
    API _4 prefix ## _5 _6; \
    API _7 prefix ## _8 _9; \
    API _10 prefix ## _11 _12; \
    API _13 prefix ## _14 _15; \
    API _16 prefix ## _17 _18; \
    API _19 prefix ## _20 _21; \
    API _22 prefix ## _23 _24; \
    API _25 prefix ## _26 _27; \
    API _28 prefix ## _29 _30; \
    API _31 prefix ## _32 _33; \
    API _34 prefix ## _35 _36; \
    API _37 prefix ## _38 _39; \
    API _40 prefix ## _41 _42; \
    API _43 prefix ## _44 _45; \
    API _46 prefix ## _47 _48; \
    API _49 prefix ## _50 _51; \
    API _52 prefix ## _53 _54;

#define RADIO_C_API_PARAM_58(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21, \
                               _22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39, \
                               _40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57,prefix) \
    API _1 prefix ## _2 _3; \
    API _4 prefix ## _5 _6; \
    API _7 prefix ## _8 _9; \
    API _10 prefix ## _11 _12; \
    API _13 prefix ## _14 _15; \
    API _16 prefix ## _17 _18; \
    API _19 prefix ## _20 _21; \
    API _22 prefix ## _23 _24; \
    API _25 prefix ## _26 _27; \
    API _28 prefix ## _29 _30; \
    API _31 prefix ## _32 _33; \
    API _34 prefix ## _35 _36; \
    API _37 prefix ## _38 _39; \
    API _40 prefix ## _41 _42; \
    API _43 prefix ## _44 _45; \
    API _46 prefix ## _47 _48; \
    API _49 prefix ## _50 _51; \
    API _52 prefix ## _53 _54; \
    API _55 prefix ## _56 _57;

#define RADIO_C_API_PARAM_61(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21, \
                               _22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39, \
                               _40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57, \
                               _58,_59,_60,prefix) \
    API _1 prefix ## _2 _3; \
    API _4 prefix ## _5 _6; \
    API _7 prefix ## _8 _9; \
    API _10 prefix ## _11 _12; \
    API _13 prefix ## _14 _15; \
    API _16 prefix ## _17 _18; \
    API _19 prefix ## _20 _21; \
    API _22 prefix ## _23 _24; \
    API _25 prefix ## _26 _27; \
    API _28 prefix ## _29 _30; \
    API _31 prefix ## _32 _33; \
    API _34 prefix ## _35 _36; \
    API _37 prefix ## _38 _39; \
    API _40 prefix ## _41 _42; \
    API _43 prefix ## _44 _45; \
    API _46 prefix ## _47 _48; \
    API _49 prefix ## _50 _51; \
    API _52 prefix ## _53 _54; \
    API _55 prefix ## _56 _57; \
    API _58 prefix ## _59 _60;

#define RADIO_C_API_PARAM_64(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21, \
                               _22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39, \
                               _40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57, \
                               _58,_59,_60,_61,_62,_63,prefix) \
    API _1 prefix ## _2 _3; \
    API _4 prefix ## _5 _6; \
    API _7 prefix ## _8 _9; \
    API _10 prefix ## _11 _12; \
    API _13 prefix ## _14 _15; \
    API _16 prefix ## _17 _18; \
    API _19 prefix ## _20 _21; \
    API _22 prefix ## _23 _24; \
    API _25 prefix ## _26 _27; \
    API _28 prefix ## _29 _30; \
    API _31 prefix ## _32 _33; \
    API _34 prefix ## _35 _36; \
    API _37 prefix ## _38 _39; \
    API _40 prefix ## _41 _42; \
    API _43 prefix ## _44 _45; \
    API _46 prefix ## _47 _48; \
    API _49 prefix ## _50 _51; \
    API _52 prefix ## _53 _54; \
    API _55 prefix ## _56 _57; \
    API _58 prefix ## _59 _60; \
    API _61 prefix ## _62 _63;

#define RADIO_C_API_PARAM_67(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21, \
                               _22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39, \
                               _40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57, \
                               _58,_59,_60,_61,_62,_63,_64,_65,_66,prefix) \
    API _1 prefix ## _2 _3; \
    API _4 prefix ## _5 _6; \
    API _7 prefix ## _8 _9; \
    API _10 prefix ## _11 _12; \
    API _13 prefix ## _14 _15; \
    API _16 prefix ## _17 _18; \
    API _19 prefix ## _20 _21; \
    API _22 prefix ## _23 _24; \
    API _25 prefix ## _26 _27; \
    API _28 prefix ## _29 _30; \
    API _31 prefix ## _32 _33; \
    API _34 prefix ## _35 _36; \
    API _37 prefix ## _38 _39; \
    API _40 prefix ## _41 _42; \
    API _43 prefix ## _44 _45; \
    API _46 prefix ## _47 _48; \
    API _49 prefix ## _50 _51; \
    API _52 prefix ## _53 _54; \
    API _55 prefix ## _56 _57; \
    API _58 prefix ## _59 _60; \
    API _61 prefix ## _62 _63; \
    API _64 prefix ## _65 _66;

#define RADIO_C_API_PARAM_70(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21, \
                               _22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39, \
                               _40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57, \
                               _58,_59,_60,_61,_62,_63,_64,_65,_66,_67,_68,_69,prefix) \
    API _1 prefix ## _2 _3; \
    API _4 prefix ## _5 _6; \
    API _7 prefix ## _8 _9; \
    API _10 prefix ## _11 _12; \
    API _13 prefix ## _14 _15; \
    API _16 prefix ## _17 _18; \
    API _19 prefix ## _20 _21; \
    API _22 prefix ## _23 _24; \
    API _25 prefix ## _26 _27; \
    API _28 prefix ## _29 _30; \
    API _31 prefix ## _32 _33; \
    API _34 prefix ## _35 _36; \
    API _37 prefix ## _38 _39; \
    API _40 prefix ## _41 _42; \
    API _43 prefix ## _44 _45; \
    API _46 prefix ## _47 _48; \
    API _49 prefix ## _50 _51; \
    API _52 prefix ## _53 _54; \
    API _55 prefix ## _56 _57; \
    API _58 prefix ## _59 _60; \
    API _61 prefix ## _62 _63; \
    API _64 prefix ## _65 _66; \
    API _67 prefix ## _68 _69;

#define RADIO_C_API_PARAM_73(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21, \
                               _22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39, \
                               _40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57, \
                               _58,_59,_60,_61,_62,_63,_64,_65,_66,_67,_68,_69,_70,_71,_72,prefix) \
    API _1 prefix ## _2 _3; \
    API _4 prefix ## _5 _6; \
    API _7 prefix ## _8 _9; \
    API _10 prefix ## _11 _12; \
    API _13 prefix ## _14 _15; \
    API _16 prefix ## _17 _18; \
    API _19 prefix ## _20 _21; \
    API _22 prefix ## _23 _24; \
    API _25 prefix ## _26 _27; \
    API _28 prefix ## _29 _30; \
    API _31 prefix ## _32 _33; \
    API _34 prefix ## _35 _36; \
    API _37 prefix ## _38 _39; \
    API _40 prefix ## _41 _42; \
    API _43 prefix ## _44 _45; \
    API _46 prefix ## _47 _48; \
    API _49 prefix ## _50 _51; \
    API _52 prefix ## _53 _54; \
    API _55 prefix ## _56 _57; \
    API _58 prefix ## _59 _60; \
    API _61 prefix ## _62 _63; \
    API _64 prefix ## _65 _66; \
    API _67 prefix ## _68 _69; \
    API _70 prefix ## _71 _72;

#define RADIO_C_API_PARAM_76(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21, \
                               _22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39, \
                               _40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57, \
                               _58,_59,_60,_61,_62,_63,_64,_65,_66,_67,_68,_69,_70,_71,_72,_73,_74,_75,prefix) \
    API _1 prefix ## _2 _3; \
    API _4 prefix ## _5 _6; \
    API _7 prefix ## _8 _9; \
    API _10 prefix ## _11 _12; \
    API _13 prefix ## _14 _15; \
    API _16 prefix ## _17 _18; \
    API _19 prefix ## _20 _21; \
    API _22 prefix ## _23 _24; \
    API _25 prefix ## _26 _27; \
    API _28 prefix ## _29 _30; \
    API _31 prefix ## _32 _33; \
    API _34 prefix ## _35 _36; \
    API _37 prefix ## _38 _39; \
    API _40 prefix ## _41 _42; \
    API _43 prefix ## _44 _45; \
    API _46 prefix ## _47 _48; \
    API _49 prefix ## _50 _51; \
    API _52 prefix ## _53 _54; \
    API _55 prefix ## _56 _57; \
    API _58 prefix ## _59 _60; \
    API _61 prefix ## _62 _63; \
    API _64 prefix ## _65 _66; \
    API _67 prefix ## _68 _69; \
    API _70 prefix ## _71 _72; \
    API _73 prefix ## _74 _75;

#define RADIO_C_API_APPLY(macro,count) MACRO_ID(macro(count))
#define RADIO_C_API_VAR_COUNT(count) RADIO_C_API_PARAM_##count
#define RADIO_C_API(...) MACRO_ID(RADIO_C_API_APPLY(RADIO_C_API_VAR_COUNT,PP_COUNT(__VA_ARGS__))(__VA_ARGS__))

#endif // DEVICE_DEVICE_RADIO_C_API_H
