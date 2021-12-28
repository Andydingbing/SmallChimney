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

#ifndef INCLUDE_CONFIG_COMPILER_H
#define INCLUDE_CONFIG_COMPILER_H

#if defined(__MINGW32__) || defined(__MINGW64__)
    #define COMPILER_GCC
#elif defined(__GNUC__) && !defined(__ibmxl__)
    #define COMPILER_GCC
#elif defined(__clang__) && !defined(__ibmxl__) && !defined(__CODEGEARC__)
    #define COMPILER_CLANG
#elif defined(_MSC_VER)
    #define COMPILER_MSC
#else
    #define COMPILER_UNKNOWN
    #error "unknown compiler"
#endif

#ifdef COMPILER_GCC
    #include "gcc.h"
#endif

#ifdef COMPILER_MSC
    #include "msc.h"
#endif

#endif // INCLUDE_CONFIG_COMPILER_H
