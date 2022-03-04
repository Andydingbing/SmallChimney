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

#ifndef INCLUDE_PREPROCESSOR_PICK_2_3_H
#define INCLUDE_PREPROCESSOR_PICK_2_3_H

#include "_1_1.h"
#include "_2_3_1.h"
#include "_2_3_2.h"
#include "_2_of_3_1.h"
#include "_2_of_3_2.h"
#include "_2_of_3_1_3.h"
#include "_2_of_3_2_1.h"
#include "_2_of_3_3_1.h"

#define _2_3_1_VAR_COUNT(count)  _2_3_1_##count
#define _2_3_2_VAR_COUNT(count)  _2_3_2_##count
#define _2_OF_3_1_VAR_COUNT(count) _2_OF_3_1_##count
#define _2_OF_3_2_VAR_COUNT(count) _2_OF_3_2_##count
#define _2_OF_3_1_3_VAR_COUNT(count) _2_OF_3_1_3_##count
#define _2_OF_3_2_1_VAR_COUNT(count) _2_OF_3_2_1_##count
#define _2_OF_3_3_1_VAR_COUNT(count) _2_OF_3_3_1_##count

#define PARAM_PICK_2_3_1(...) MACRO_ID(PARAM_PICK_APPLY(_2_3_1_VAR_COUNT,PP_COUNT_3(__VA_ARGS__))(__VA_ARGS__))
#define PARAM_PICK_2_3_2(...) MACRO_ID(PARAM_PICK_APPLY(_2_3_2_VAR_COUNT,PP_COUNT_3(__VA_ARGS__))(__VA_ARGS__))

#define PARAM_PICK_2_OF_3_1(...) MACRO_ID(PARAM_PICK_APPLY(_2_OF_3_1_VAR_COUNT,PP_COUNT_4(__VA_ARGS__))(__VA_ARGS__))
#define PARAM_PICK_2_OF_3_2(...) MACRO_ID(PARAM_PICK_APPLY(_2_OF_3_2_VAR_COUNT,PP_COUNT_4(__VA_ARGS__))(__VA_ARGS__))

#define PARAM_PICK_2_OF_3_1_3(...) MACRO_ID(PARAM_PICK_APPLY(_2_OF_3_1_3_VAR_COUNT,PP_COUNT_4(__VA_ARGS__))(__VA_ARGS__))
#define PARAM_PICK_2_OF_3_2_1(...) MACRO_ID(PARAM_PICK_APPLY(_2_OF_3_2_1_VAR_COUNT,PP_COUNT_4(__VA_ARGS__))(__VA_ARGS__))
#define PARAM_PICK_2_OF_3_3_1(...) MACRO_ID(PARAM_PICK_APPLY(_2_OF_3_3_1_VAR_COUNT,PP_COUNT_4(__VA_ARGS__))(__VA_ARGS__))

#endif
