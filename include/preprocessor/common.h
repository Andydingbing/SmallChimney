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

#ifndef INCLUDE_PREPROCESSOR_COMMON_H
#define INCLUDE_PREPROCESSOR_COMMON_H

#include "warning.h"
#include <boost/core/ignore_unused.hpp>

using boost::ignore_unused;

#define MACRO_ID(x) x

#define PARAM_PICK_APPLY(macro,count) MACRO_ID(macro(count))

#define NEED_SEMICOLON ignore_unused()

#define ARRAY_SIZE(array) (sizeof(array) / sizeof((array)[0]))

#define ZERO_ARRAY(array) { memset(array,0,sizeof(array)); } NEED_SEMICOLON

#define INIT_ARRAY(array,value) \
{   for (size_t i = 0;i < ARRAY_SIZE(array);++i) { \
        array[i] = (value); \
    } \
} NEED_SEMICOLON

#define SAFE_NEW(ptr,t)         { if ((ptr) == nullptr) (ptr) = new (t); }  NEED_SEMICOLON
#define SAFE_NEW_ARRAY(ptr,t,n) { if ((ptr) == nullptr) (ptr) = new t[n]; } NEED_SEMICOLON
#define SAFE_DEL(ptr)           { if ((ptr) != nullptr) { delete (ptr);(ptr) = nullptr; } } NEED_SEMICOLON
#define SAFE_DEL_ARRAY(ptr)     { if ((ptr) != nullptr) { delete []ptr;(ptr) = nullptr; } } NEED_SEMICOLON

#define SERIE_SIZE(star,stop,step)  (uint32_t(((stop) - (star)) / (step) + 1))
#define SERIE_INDEX(data,star,step) (uint32_t(((data) - (star)) / (step)))
#define SERIE_VALUE(star,step,i)    ((star) + (step) * i)

#define BOOL_CHECK(func) { if (         !(func)) { return false; } } NEED_SEMICOLON
#define INT_CHECK(func)  { if (int ret = (func)) { return ret;   } } NEED_SEMICOLON
#define INT_CHECKB(func) { if (int ret = (func)) { return false; } } NEED_SEMICOLON
#define INT_CHECKV(func) { if (int ret = (func)) { return;       } } NEED_SEMICOLON
#define PTR_CHECKV(ptr)  { if ((ptr) == nullptr) { return;       } } NEED_SEMICOLON

#define FREQ_K(freq) freq##000ll
#define FREQ_M(freq) freq##000000ll
#define FREQ_G(freq) freq##000000000ll

#endif // INCLUDE_PREPROCESSOR_COMMON_H
