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

#ifndef INCLUDE_PREPROCESSOR_PARAM_PICK_1_2_H
#define INCLUDE_PREPROCESSOR_PARAM_PICK_1_2_H

#include "../count.h"
#include "../common.h"
#include "_1_2_1.h"
#include "_1_2_2.h"
#include "_1_of_2_1.h"
#include "_1_of_2_2.h"

#define _1_2_1_VAR_COUNT(count)  _1_2_1_##count
#define _1_2_2_VAR_COUNT(count)  _1_2_2_##count
#define _1_OF_2_1_VAR_COUNT(count) _1_OF_2_1_##count
#define _1_OF_2_2_VAR_COUNT(count) _1_OF_2_2_##count

#define PARAM_PICK_1_2_1(...) MACRO_ID(PARAM_PICK_APPLY(sep,prefix,suffix,_1_2_1_VAR_COUNT,PP_COUNT_3(__VA_ARGS__))(__VA_ARGS__))
#define PARAM_PICK_1_2_2(...) MACRO_ID(PARAM_PICK_APPLY(sep,prefix,suffix,_1_2_2_VAR_COUNT,PP_COUNT_3(__VA_ARGS__))(__VA_ARGS__))

#define PARAM_PICK_1_OF_2_1(...) MACRO_ID(PARAM_PICK_APPLY(sep,prefix,suffix,_1_OF_2_1_VAR_COUNT,PP_COUNT_3(__VA_ARGS__))(__VA_ARGS__))
#define PARAM_PICK_1_OF_2_2(...) MACRO_ID(PARAM_PICK_APPLY(sep,prefix,suffix,_1_OF_2_2_VAR_COUNT,PP_COUNT_3(__VA_ARGS__))(__VA_ARGS__))

#endif // INCLUDE_PREPROCESSOR_PARAM_PICK_1_2_H
