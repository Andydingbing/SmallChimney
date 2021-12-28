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

#ifndef INCLUDE_NULLPTR_H
#define INCLUDE_NULLPTR_H

#include "config/compiler.h"

// nullptr_t & nullptr for compiler not support C++11 and later.

#if (defined(COMPILER_MSC) && (_MSC_VER < 1700))

const class nullptr_t
{
public:
    template<class T>
    inline operator T*() const {return 0;}

    template<class C,class T>
    inline operator T C::*() const {return 0;}

private:
    void operator&() const;
} nullptr = {};

#endif

#endif // INCLUDE_NULLPTR_H
