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

#ifndef INCLUDE_PREPROCESSOR_PARAM_PICK_2_2_H
#define INCLUDE_PREPROCESSOR_PARAM_PICK_2_2_H

#include "_1_1.h"

#define _2_OF_2_PARAM_7(_1,_2,macro_prefix,sep_1,sep_2,sep_3,macro_suffix) \
    macro_prefix \
    sep_1 _1 sep_2 _2 sep_3 \
    macro_suffix

#define _2_OF_2_PARAM_9(_1,_2,_3,_4,macro_prefix,sep_1,sep_2,sep_3,macro_suffix) \
    macro_prefix \
    sep_1 _1 sep_2 _2 sep_3 \
    sep_1 _3 sep_2 _4 sep_3 \
    macro_suffix

#define _2_OF_2_PARAM_11(_1,_2,_3,_4,_5,_6,macro_prefix,sep_1,sep_2,sep_3,macro_suffix) \
    macro_prefix \
    sep_1 _1 sep_2 _2 sep_3 \
    sep_1 _3 sep_2 _4 sep_3 \
    sep_1 _5 sep_2 _6 sep_3 \
    macro_suffix

#define _2_OF_2_PARAM_13(_1,_2,_3,_4,_5,_6,_7,_8,macro_prefix,sep_1,sep_2,sep_3,macro_suffix) \
    macro_prefix \
    sep_1 _1 sep_2 _2 sep_3 \
    sep_1 _3 sep_2 _4 sep_3 \
    sep_1 _5 sep_2 _6 sep_3 \
    sep_1 _7 sep_2 _8 sep_3 \
    macro_suffix


#define _2_OF_2_VAR_COUNT(count) _2_OF_2_PARAM_##count

#define PARAM_PICK_2_OF_2(...) MACRO_ID(PARAM_PICK_APPLY(_2_OF_2_VAR_COUNT,PP_COUNT(__VA_ARGS__))(__VA_ARGS__))

#endif
