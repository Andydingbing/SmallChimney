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

#ifndef INCLUDE_PREPROCESSOR_PARAM_PICK_3_3_H
#define INCLUDE_PREPROCESSOR_PARAM_PICK_3_3_H

#include "_1_1.h"

#define _3_OF_3_PARAM_7(_1,_2,_3,sep_1,sep_2,sep_3,sep_4) \
    sep_1 _1 sep_2 _2 sep_3 _3 sep_4 \

#define _3_OF_3_VAR_COUNT(count) _3_OF_3_PARAM_##count

#define PARAM_PICK_3_OF_3(...) MACRO_ID(PARAM_PICK_APPLY(_3_OF_3_VAR_COUNT,PP_COUNT(__VA_ARGS__))(__VA_ARGS__))

#endif // INCLUDE_PREPROCESSOR_PARAM_PICK_3_3_H
