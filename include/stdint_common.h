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

#ifndef INCLUDE_STDINT_H
#define INCLUDE_STDINT_H

#include "config/compiler.h"

/*
  * No standard stdint define while MSC_VER <= 1500,thanks to the stdint_msc.h.
  */
#ifdef COMPILER_MSC
    #if _MSC_VER <= 1500
        #include "stdint_msc.h"
    #else
        #include <cstdint>
    #endif
#elif defined COMPILER_GCC
    #include "stdint.h"
#endif

#endif
