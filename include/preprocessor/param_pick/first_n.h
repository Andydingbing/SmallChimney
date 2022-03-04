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

#ifndef INCLUDE_PREPROCESSOR_PARAM_PICK_FIRST_N_H
#define INCLUDE_PREPROCESSOR_PARAM_PICK_FIRST_N_H

#include "../count.h"
#include "../common.h"
#include "first_n_1.h"
#include "first_n_2.h"

#define PP_VAR_FIRST(last,var_cnt) PP_VAR_FIRST_N_##last##_##var_cnt
#define PP_VAR_FIRST_APPLY(macro,last,var_cnt) MACRO_ID(macro(last,var_cnt))

#define PP_VAR_FIRST_N_1(...) MACRO_ID(PP_VAR_FIRST_APPLY(PP_VAR_FIRST,1,PP_COUNT_3(__VA_ARGS__))(__VA_ARGS__))
#define PP_VAR_FIRST_N_2(...) MACRO_ID(PP_VAR_FIRST_APPLY(PP_VAR_FIRST,2,PP_COUNT_3(__VA_ARGS__))(__VA_ARGS__))

#endif
