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

#ifndef INCLUDE_PREPROCESSOR_PREFIX_H
#define INCLUDE_PREPROCESSOR_PREFIX_H

#include "warning.h"
#include "param_pick/_1_2.h"
#include "param_pick/_1_3.h"
#include "param_pick/_2_3.h"
#include "param_pick/first_n.h"
#include "enum.h"

#if defined(_MSC_VER)
    #define SYM_EXPORT       __declspec(dllexport)
    #define SYM_IMPORT       __declspec(dllimport)
    #define SYM_INLINE       __forceinline
    #define SYM_DEPRECATED   __declspec(deprecated)
#elif defined(__MINGW32__) || defined(__MINGW64__)
    #define SYM_EXPORT       __declspec(dllexport)
    #define SYM_IMPORT       __declspec(dllimport)
    #define SYM_INLINE       inline
    #define SYM_DEPRECATED   __declspec(deprecated)
#elif defined(__GNUC__) && __GNUC__ >= 4
    #define SYM_EXPORT       __attribute__((visibility("default")))
    #define SYM_IMPORT       __attribute__((visibility("default")))
    #define SYM_INLINE       inline __attribute__((always_inline))
    #define SYM_DEPRECATED   __attribute__((deprecated))
#elif defined(__clang__)
    #define SYM_EXPORT       __attribute__((visibility("default")))
    #define SYM_IMPORT       __attribute__((visibility("default")))
    #define SYM_INLINE       inline __attribute__((always_inline))
    #define SYM_DEPRECATED   __attribute__((deprecated))
#else
    #define SYM_EXPORT
    #define SYM_IMPORT
    #define SYM_INLINE       inline
    #define SYM_DEPRECATED
#endif

#ifdef DLL_EXPORT
    #define API SYM_EXPORT
#else
    #define API SYM_IMPORT
#endif

#define ENUM_C_CPP(Namespace,Enum,Underlying,...) \
    namespace ns_##Namespace { \
        BETTER_ENUM(Enum##_t,Underlying,__VA_ARGS__) \
    } \
    enum Namespace##_##Enum##_t { \
        PARAM_PICK_1_OF_1(,Namespace##_##Enum##_,,__VA_ARGS__) \
    };

#endif
