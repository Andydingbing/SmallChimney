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

#ifndef INCLUDE_PREPROCESSOR_SUFFIX_H
#define INCLUDE_PREPROCESSOR_SUFFIX_H

#include "warning.h"

#ifdef NO_CXX11_OVERRIDE
    #define OVERRIDE
#else
    #define OVERRIDE override
#endif

#if defined(NO_CXX11_CONSTEXPR)
    #define CONSTEXPR
    #define CONSTEXPR_OR_CONST const
#else
    #define CONSTEXPR constexpr
    #define CONSTEXPR_OR_CONST constexpr
#endif

#if defined(NO_CXX14_CONSTEXPR)
    #define CXX14_CONSTEXPR
#else
    #define CXX14_CONSTEXPR constexpr
#endif

#endif // INCLUDE_PREPROCESSOR_SUFFIX_H
