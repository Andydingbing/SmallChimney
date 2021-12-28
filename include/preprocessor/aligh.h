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

#ifndef INCLUDE_PREPROCESSOR_ALIGN_H
#define INCLUDE_PREPROCESSOR_ALIGN_H

#include "config/compiler.h"

#ifdef COMPILER_MSC
    #define STRUCT_ALIGN_S(name,x) \
            __declspec(align(x)) struct name {

    #define STRUCT_ALIGN_E(name,x) \
            };

    #define STRUCT_ALIGN_API_S(name,x) \
            __declspec(align(x)) struct API name {

    #define STRUCT_ALIGN_API_E(name,x) \
            };

    #define CLASS_ALIGN_S(name,x) \
            __declspec(align(x)) class class_name {

    #define CLASS_ALIGN_E(name,x) \
            };

    #define STRUCT_ALIGN_INHERIT_S(name,parent,x) \
            __declspec(align(x)) struct name : public parent {

    #define STRUCT_ALIGN_INHERIT_E(name,x) \
            };

    #define CLASS_ALIGN_INHERIT_S(name,parent,x) \
            __declspec(align(x)) class name : public parent {

    #define CLASS_ALIGN_INHERIT_E(name,x) \
            };
#elif defined COMPILER_GCC
    #define STRUCT_ALIGN_S(name,x) \
            struct name {

    #define STRUCT_ALIGN_E(name,x) \
            }__attribute__((aligned(x)));

    #define STRUCT_ALIGN_API_S(name,x) \
            struct API name {

    #define STRUCT_ALIGN_API_E(name,x) \
            STRUCT_ALIGN_E(name,x)

    #define CLASS_ALIGN_S(name,x) \
            class name {

    #define CLASS_ALIGN_E(name,x) \
            }__attribute__((aligned(x)));

    #define STRUCT_ALIGN_INHERIT_S(name,parent,x) \
            struct name : public parent {

    #define STRUCT_ALIGN_INHERIT_E(name,x) \
            }__attribute__((aligned(x)));

    #define STRUCT_ALIGN_INHERIT_API_S(name,parent,x) \
            struct API name : public parent {

    #define STRUCT_ALIGN_INHERIT_API_E(name,x) \
            }__attribute__((aligned(x)));

    #define CLASS_ALIGN_INHERIT_S(name,parent,x) \
            class name : public parent {

    #define CLASS_ALIGN_INHERIT_E(name,x) \
            }__attribute((aligned(x)));
#endif

#endif // INCLUDE_PREPROCESSOR_ALIGN_H
