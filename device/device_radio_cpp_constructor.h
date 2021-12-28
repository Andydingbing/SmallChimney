#ifndef DEVICE_DEVICE_RADIO_CPP_CONSTRUCTOR_H
#define DEVICE_DEVICE_RADIO_CPP_CONSTRUCTOR_H

#include "preprocessor/preprocessor.h"

#define RADIO_CPP_CONSTRUCTOR_PARAM_4(_1,_2,_3,prefix) \
    this->_2 = ::prefix ## _2;

#define RADIO_CPP_CONSTRUCTOR_PARAM_7(_1,_2,_3,_4,_5,_6,prefix) \
    this->_2 = ::prefix ## _2; \
    this->_5 = ::prefix ## _5;

#define RADIO_CPP_CONSTRUCTOR_PARAM_10(_1,_2,_3,_4,_5,_6,_7,_8,_9,prefix) \
    this->_2 = ::prefix ## _2; \
    this->_5 = ::prefix ## _5; \
    this->_8 = ::prefix ## _8;

#define RADIO_CPP_CONSTRUCTOR_PARAM_13(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,prefix) \
    this->_2 = ::prefix ## _2; \
    this->_5 = ::prefix ## _5; \
    this->_8 = ::prefix ## _8; \
    this->_11 = ::prefix ## _11;

#define RADIO_CPP_CONSTRUCTOR_PARAM_16(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,prefix) \
    this->_2 = ::prefix ## _2; \
    this->_5 = ::prefix ## _5; \
    this->_8 = ::prefix ## _8; \
    this->_11 = ::prefix ## _11; \
    this->_14 = ::prefix ## _14;

#define RADIO_CPP_CONSTRUCTOR_PARAM_19(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,prefix) \
    this->_2 = ::prefix ## _2; \
    this->_5 = ::prefix ## _5; \
    this->_8 = ::prefix ## _8; \
    this->_11 = ::prefix ## _11; \
    this->_14 = ::prefix ## _14; \
    this->_17 = ::prefix ## _17;

#define RADIO_CPP_CONSTRUCTOR_PARAM_22(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,prefix) \
    this->_2 = ::prefix ## _2; \
    this->_5 = ::prefix ## _5; \
    this->_8 = ::prefix ## _8; \
    this->_11 = ::prefix ## _11; \
    this->_14 = ::prefix ## _14; \
    this->_17 = ::prefix ## _17; \
    this->_20 = ::prefix ## _20;

#define RADIO_CPP_CONSTRUCTOR_PARAM_25(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21, \
                               _22,_23,_24,prefix) \
    this->_2 = ::prefix ## _2; \
    this->_5 = ::prefix ## _5; \
    this->_8 = ::prefix ## _8; \
    this->_11 = ::prefix ## _11; \
    this->_14 = ::prefix ## _14; \
    this->_17 = ::prefix ## _17; \
    this->_20 = ::prefix ## _20; \
    this->_23 = ::prefix ## _23;

#define RADIO_CPP_CONSTRUCTOR_PARAM_28(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21, \
                               _22,_23,_24,_25,_26,_27,prefix) \
    this->_2 = ::prefix ## _2; \
    this->_5 = ::prefix ## _5; \
    this->_8 = ::prefix ## _8; \
    this->_11 = ::prefix ## _11; \
    this->_14 = ::prefix ## _14; \
    this->_17 = ::prefix ## _17; \
    this->_20 = ::prefix ## _20; \
    this->_23 = ::prefix ## _23; \
    this->_26 = ::prefix ## _26;

#define RADIO_CPP_CONSTRUCTOR_PARAM_31(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21, \
                               _22,_23,_24,_25,_26,_27,_28,_29,_30,prefix) \
    this->_2 = ::prefix ## _2; \
    this->_5 = ::prefix ## _5; \
    this->_8 = ::prefix ## _8; \
    this->_11 = ::prefix ## _11; \
    this->_14 = ::prefix ## _14; \
    this->_17 = ::prefix ## _17; \
    this->_20 = ::prefix ## _20; \
    this->_23 = ::prefix ## _23; \
    this->_26 = ::prefix ## _26; \
    this->_29 = ::prefix ## _29;

#define RADIO_CPP_CONSTRUCTOR_PARAM_34(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21, \
                               _22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,prefix) \
    this->_2 = ::prefix ## _2; \
    this->_5 = ::prefix ## _5; \
    this->_8 = ::prefix ## _8; \
    this->_11 = ::prefix ## _11; \
    this->_14 = ::prefix ## _14; \
    this->_17 = ::prefix ## _17; \
    this->_20 = ::prefix ## _20; \
    this->_23 = ::prefix ## _23; \
    this->_26 = ::prefix ## _26; \
    this->_29 = ::prefix ## _29; \
    this->_32 = ::prefix ## _32;

#define RADIO_CPP_CONSTRUCTOR_PARAM_37(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21, \
                               _22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,prefix) \
    this->_2 = ::prefix ## _2; \
    this->_5 = ::prefix ## _5; \
    this->_8 = ::prefix ## _8; \
    this->_11 = ::prefix ## _11; \
    this->_14 = ::prefix ## _14; \
    this->_17 = ::prefix ## _17; \
    this->_20 = ::prefix ## _20; \
    this->_23 = ::prefix ## _23; \
    this->_26 = ::prefix ## _26; \
    this->_29 = ::prefix ## _29; \
    this->_32 = ::prefix ## _32; \
    this->_35 = ::prefix ## _35;

#define RADIO_CPP_CONSTRUCTOR_PARAM_40(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21, \
                               _22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39,prefix) \
    this->_2 = ::prefix ## _2; \
    this->_5 = ::prefix ## _5; \
    this->_8 = ::prefix ## _8; \
    this->_11 = ::prefix ## _11; \
    this->_14 = ::prefix ## _14; \
    this->_17 = ::prefix ## _17; \
    this->_20 = ::prefix ## _20; \
    this->_23 = ::prefix ## _23; \
    this->_26 = ::prefix ## _26; \
    this->_29 = ::prefix ## _29; \
    this->_32 = ::prefix ## _32; \
    this->_35 = ::prefix ## _35; \
    this->_38 = ::prefix ## _38;

#define RADIO_CPP_CONSTRUCTOR_PARAM_43(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21, \
                               _22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39, \
                               _40,_41,_42,prefix) \
    this->_2 = ::prefix ## _2; \
    this->_5 = ::prefix ## _5; \
    this->_8 = ::prefix ## _8; \
    this->_11 = ::prefix ## _11; \
    this->_14 = ::prefix ## _14; \
    this->_17 = ::prefix ## _17; \
    this->_20 = ::prefix ## _20; \
    this->_23 = ::prefix ## _23; \
    this->_26 = ::prefix ## _26; \
    this->_29 = ::prefix ## _29; \
    this->_32 = ::prefix ## _32; \
    this->_35 = ::prefix ## _35; \
    this->_38 = ::prefix ## _38; \
    this->_41 = ::prefix ## _41;

#define RADIO_CPP_CONSTRUCTOR_PARAM_46(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21, \
                               _22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39, \
                               _40,_41,_42,_43,_44,_45,prefix) \
    this->_2 = ::prefix ## _2; \
    this->_5 = ::prefix ## _5; \
    this->_8 = ::prefix ## _8; \
    this->_11 = ::prefix ## _11; \
    this->_14 = ::prefix ## _14; \
    this->_17 = ::prefix ## _17; \
    this->_20 = ::prefix ## _20; \
    this->_23 = ::prefix ## _23; \
    this->_26 = ::prefix ## _26; \
    this->_29 = ::prefix ## _29; \
    this->_32 = ::prefix ## _32; \
    this->_35 = ::prefix ## _35; \
    this->_38 = ::prefix ## _38; \
    this->_41 = ::prefix ## _41; \
    this->_44 = ::prefix ## _44;

#define RADIO_CPP_CONSTRUCTOR_PARAM_49(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21, \
                               _22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39, \
                               _40,_41,_42,_43,_44,_45,_46,_47,_48,prefix) \
    this->_2 = ::prefix ## _2; \
    this->_5 = ::prefix ## _5; \
    this->_8 = ::prefix ## _8; \
    this->_11 = ::prefix ## _11; \
    this->_14 = ::prefix ## _14; \
    this->_17 = ::prefix ## _17; \
    this->_20 = ::prefix ## _20; \
    this->_23 = ::prefix ## _23; \
    this->_26 = ::prefix ## _26; \
    this->_29 = ::prefix ## _29; \
    this->_32 = ::prefix ## _32; \
    this->_35 = ::prefix ## _35; \
    this->_38 = ::prefix ## _38; \
    this->_41 = ::prefix ## _41; \
    this->_44 = ::prefix ## _44; \
    this->_47 = ::prefix ## _47;

#define RADIO_CPP_CONSTRUCTOR_PARAM_52(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21, \
                               _22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39, \
                               _40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,prefix) \
    this->_2 = ::prefix ## _2; \
    this->_5 = ::prefix ## _5; \
    this->_8 = ::prefix ## _8; \
    this->_11 = ::prefix ## _11; \
    this->_14 = ::prefix ## _14; \
    this->_17 = ::prefix ## _17; \
    this->_20 = ::prefix ## _20; \
    this->_23 = ::prefix ## _23; \
    this->_26 = ::prefix ## _26; \
    this->_29 = ::prefix ## _29; \
    this->_32 = ::prefix ## _32; \
    this->_35 = ::prefix ## _35; \
    this->_38 = ::prefix ## _38; \
    this->_41 = ::prefix ## _41; \
    this->_44 = ::prefix ## _44; \
    this->_47 = ::prefix ## _47; \
    this->_50 = ::prefix ## _50;

#define RADIO_CPP_CONSTRUCTOR_PARAM_55(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21, \
                               _22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39, \
                               _40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,prefix) \
    this->_2 = ::prefix ## _2; \
    this->_5 = ::prefix ## _5; \
    this->_8 = ::prefix ## _8; \
    this->_11 = ::prefix ## _11; \
    this->_14 = ::prefix ## _14; \
    this->_17 = ::prefix ## _17; \
    this->_20 = ::prefix ## _20; \
    this->_23 = ::prefix ## _23; \
    this->_26 = ::prefix ## _26; \
    this->_29 = ::prefix ## _29; \
    this->_32 = ::prefix ## _32; \
    this->_35 = ::prefix ## _35; \
    this->_38 = ::prefix ## _38; \
    this->_41 = ::prefix ## _41; \
    this->_44 = ::prefix ## _44; \
    this->_47 = ::prefix ## _47; \
    this->_50 = ::prefix ## _50; \
    this->_53 = ::prefix ## _53;

#define RADIO_CPP_CONSTRUCTOR_PARAM_58(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21, \
                               _22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39, \
                               _40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57,prefix) \
    this->_2 = ::prefix ## _2; \
    this->_5 = ::prefix ## _5; \
    this->_8 = ::prefix ## _8; \
    this->_11 = ::prefix ## _11; \
    this->_14 = ::prefix ## _14; \
    this->_17 = ::prefix ## _17; \
    this->_20 = ::prefix ## _20; \
    this->_23 = ::prefix ## _23; \
    this->_26 = ::prefix ## _26; \
    this->_29 = ::prefix ## _29; \
    this->_32 = ::prefix ## _32; \
    this->_35 = ::prefix ## _35; \
    this->_38 = ::prefix ## _38; \
    this->_41 = ::prefix ## _41; \
    this->_44 = ::prefix ## _44; \
    this->_47 = ::prefix ## _47; \
    this->_50 = ::prefix ## _50; \
    this->_53 = ::prefix ## _53; \
    this->_56 = ::prefix ## _56;

#define RADIO_CPP_CONSTRUCTOR_PARAM_61(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21, \
                               _22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39, \
                               _40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57, \
                               _58,_59,_60,prefix) \
    this->_2 = ::prefix ## _2; \
    this->_5 = ::prefix ## _5; \
    this->_8 = ::prefix ## _8; \
    this->_11 = ::prefix ## _11; \
    this->_14 = ::prefix ## _14; \
    this->_17 = ::prefix ## _17; \
    this->_20 = ::prefix ## _20; \
    this->_23 = ::prefix ## _23; \
    this->_26 = ::prefix ## _26; \
    this->_29 = ::prefix ## _29; \
    this->_32 = ::prefix ## _32; \
    this->_35 = ::prefix ## _35; \
    this->_38 = ::prefix ## _38; \
    this->_41 = ::prefix ## _41; \
    this->_44 = ::prefix ## _44; \
    this->_47 = ::prefix ## _47; \
    this->_50 = ::prefix ## _50; \
    this->_53 = ::prefix ## _53; \
    this->_56 = ::prefix ## _56; \
    this->_59 = ::prefix ## _59;

#define RADIO_CPP_CONSTRUCTOR_PARAM_64(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21, \
                               _22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39, \
                               _40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57, \
                               _58,_59,_60,_61,_62,_63,prefix) \
    this->_2 = ::prefix ## _2; \
    this->_5 = ::prefix ## _5; \
    this->_8 = ::prefix ## _8; \
    this->_11 = ::prefix ## _11; \
    this->_14 = ::prefix ## _14; \
    this->_17 = ::prefix ## _17; \
    this->_20 = ::prefix ## _20; \
    this->_23 = ::prefix ## _23; \
    this->_26 = ::prefix ## _26; \
    this->_29 = ::prefix ## _29; \
    this->_32 = ::prefix ## _32; \
    this->_35 = ::prefix ## _35; \
    this->_38 = ::prefix ## _38; \
    this->_41 = ::prefix ## _41; \
    this->_44 = ::prefix ## _44; \
    this->_47 = ::prefix ## _47; \
    this->_50 = ::prefix ## _50; \
    this->_53 = ::prefix ## _53; \
    this->_56 = ::prefix ## _56; \
    this->_59 = ::prefix ## _59; \
    this->_62 = ::prefix ## _62;

#define RADIO_CPP_CONSTRUCTOR_PARAM_67(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21, \
                               _22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39, \
                               _40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57, \
                               _58,_59,_60,_61,_62,_63,_64,_65,_66,prefix) \
    this->_2 = ::prefix ## _2; \
    this->_5 = ::prefix ## _5; \
    this->_8 = ::prefix ## _8; \
    this->_11 = ::prefix ## _11; \
    this->_14 = ::prefix ## _14; \
    this->_17 = ::prefix ## _17; \
    this->_20 = ::prefix ## _20; \
    this->_23 = ::prefix ## _23; \
    this->_26 = ::prefix ## _26; \
    this->_29 = ::prefix ## _29; \
    this->_32 = ::prefix ## _32; \
    this->_35 = ::prefix ## _35; \
    this->_38 = ::prefix ## _38; \
    this->_41 = ::prefix ## _41; \
    this->_44 = ::prefix ## _44; \
    this->_47 = ::prefix ## _47; \
    this->_50 = ::prefix ## _50; \
    this->_53 = ::prefix ## _53; \
    this->_56 = ::prefix ## _56; \
    this->_59 = ::prefix ## _59; \
    this->_62 = ::prefix ## _62; \
    this->_65 = ::prefix ## _65;

#define RADIO_CPP_CONSTRUCTOR_PARAM_70(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21, \
                               _22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39, \
                               _40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57, \
                               _58,_59,_60,_61,_62,_63,_64,_65,_66,_67,_68,_69,prefix) \
    this->_2 = ::prefix ## _2; \
    this->_5 = ::prefix ## _5; \
    this->_8 = ::prefix ## _8; \
    this->_11 = ::prefix ## _11; \
    this->_14 = ::prefix ## _14; \
    this->_17 = ::prefix ## _17; \
    this->_20 = ::prefix ## _20; \
    this->_23 = ::prefix ## _23; \
    this->_26 = ::prefix ## _26; \
    this->_29 = ::prefix ## _29; \
    this->_32 = ::prefix ## _32; \
    this->_35 = ::prefix ## _35; \
    this->_38 = ::prefix ## _38; \
    this->_41 = ::prefix ## _41; \
    this->_44 = ::prefix ## _44; \
    this->_47 = ::prefix ## _47; \
    this->_50 = ::prefix ## _50; \
    this->_53 = ::prefix ## _53; \
    this->_56 = ::prefix ## _56; \
    this->_59 = ::prefix ## _59; \
    this->_62 = ::prefix ## _62; \
    this->_65 = ::prefix ## _65; \
    this->_68 = ::prefix ## _68;

#define RADIO_CPP_CONSTRUCTOR_PARAM_73(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21, \
                               _22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39, \
                               _40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57, \
                               _58,_59,_60,_61,_62,_63,_64,_65,_66,_67,_68,_69,_70,_71,_72,prefix) \
    this->_2 = ::prefix ## _2; \
    this->_5 = ::prefix ## _5; \
    this->_8 = ::prefix ## _8; \
    this->_11 = ::prefix ## _11; \
    this->_14 = ::prefix ## _14; \
    this->_17 = ::prefix ## _17; \
    this->_20 = ::prefix ## _20; \
    this->_23 = ::prefix ## _23; \
    this->_26 = ::prefix ## _26; \
    this->_29 = ::prefix ## _29; \
    this->_32 = ::prefix ## _32; \
    this->_35 = ::prefix ## _35; \
    this->_38 = ::prefix ## _38; \
    this->_41 = ::prefix ## _41; \
    this->_44 = ::prefix ## _44; \
    this->_47 = ::prefix ## _47; \
    this->_50 = ::prefix ## _50; \
    this->_53 = ::prefix ## _53; \
    this->_56 = ::prefix ## _56; \
    this->_59 = ::prefix ## _59; \
    this->_62 = ::prefix ## _62; \
    this->_65 = ::prefix ## _65; \
    this->_68 = ::prefix ## _68; \
    this->_71 = ::prefix ## _71;

#define RADIO_CPP_CONSTRUCTOR_PARAM_76(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21, \
                               _22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39, \
                               _40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57, \
                               _58,_59,_60,_61,_62,_63,_64,_65,_66,_67,_68,_69,_70,_71,_72,_73,_74,_75,prefix) \
    this->_2 = ::prefix ## _2; \
    this->_5 = ::prefix ## _5; \
    this->_8 = ::prefix ## _8; \
    this->_11 = ::prefix ## _11; \
    this->_14 = ::prefix ## _14; \
    this->_17 = ::prefix ## _17; \
    this->_20 = ::prefix ## _20; \
    this->_23 = ::prefix ## _23; \
    this->_26 = ::prefix ## _26; \
    this->_29 = ::prefix ## _29; \
    this->_32 = ::prefix ## _32; \
    this->_35 = ::prefix ## _35; \
    this->_38 = ::prefix ## _38; \
    this->_41 = ::prefix ## _41; \
    this->_44 = ::prefix ## _44; \
    this->_47 = ::prefix ## _47; \
    this->_50 = ::prefix ## _50; \
    this->_53 = ::prefix ## _53; \
    this->_56 = ::prefix ## _56; \
    this->_59 = ::prefix ## _59; \
    this->_62 = ::prefix ## _62; \
    this->_65 = ::prefix ## _65; \
    this->_68 = ::prefix ## _68; \
    this->_71 = ::prefix ## _71; \
    this->_74 = ::prefix ## _74;

#define RADIO_CPP_CONSTRUCTOR_APPLY(macro,count) MACRO_ID(macro(count))
#define RADIO_CPP_CONSTRUCTOR_VAR_COUNT(count) RADIO_CPP_CONSTRUCTOR_PARAM_##count
#define RADIO_CPP_CONSTRUCTOR(...) MACRO_ID(RADIO_CPP_CONSTRUCTOR_APPLY(RADIO_CPP_CONSTRUCTOR_VAR_COUNT,PP_COUNT(__VA_ARGS__))(__VA_ARGS__))

#endif // DEVICE_DEVICE_RADIO_CPP_CONSTRUCTOR_H
