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

#ifndef INCLUDE_NONCOPYABLE_H
#define INCLUDE_NONCOPYABLE_H

#include "config/compiler.h"
#include "preprocessor/preprocessor.h"

class noncopyable
{
protected:

#if !defined(NO_CXX11_DEFAULTED_FUNCTIONS) && !defined(NO_CXX11_NON_PUBLIC_DEFAULTED_FUNCTIONS)
      CONSTEXPR noncopyable() = default;
      ~noncopyable() = default;
#else
      noncopyable() {}
      ~noncopyable() {}
#endif

#if !defined(NO_CXX11_DELETED_FUNCTIONS)
      noncopyable(const noncopyable&) = delete;
      noncopyable& operator=(const noncopyable&) = delete;
#else
private:
      noncopyable(const noncopyable&);
      noncopyable& operator=(const noncopyable&);
#endif
};

#endif