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

#ifndef INCLUDE_PREPROCESSRO_WARNING_H
#define INCLUDE_PREPROCESSRO_WARNING_H

#include "../config/compiler.h"

#ifdef COMPILER_MSC
    #pragma warning(disable : 4996)
    #pragma warning(disable : 4100)
    #pragma warning(disable : 4251)
    #pragma warning(disable : 4275)
#elif defined COMPILER_GCC
    #pragma GCC diagnostic ignored "-Wunused-parameter"
    #pragma GCC diagnostic ignored "-Wunused-variable"
    #pragma GCC diagnostic ignored "-Wunused-function"
    #pragma GCC diagnostic ignored "-Wmisleading-indentation"
    #pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#endif

#endif // INCLUDE_PREPROCESSRO_WARNING_H
