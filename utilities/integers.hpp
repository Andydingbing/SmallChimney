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

#ifndef RD_UTILITIES_INTEGERS_HPP
#define RD_UTILITIES_INTEGERS_HPP

#include "global_header.h"
#include "traits.hpp"
#include <sstream>

namespace integers {

class integers_t {
public:
    // "123" ---> 123;
    template <typename T,typename data_type = traits_int<T>>
    static void normal_notation(const std::string &str,T &out)
    { std::stringstream ss(str); ss >> out; }

    template <typename T,typename data_type = traits_int<T>>
    static void auto_notation(const std::string &str,T &out);
};

template<typename T> SYM_INLINE void normal_notation(const std::string &str,T &out)
{ integers_t::normal_notation<T>(str,out); }

template<typename T> SYM_INLINE T normal_notation(const std::string &str)
{ T t; integers_t::normal_notation<T>(str,t); return t; }

template<typename T> SYM_INLINE void auto_notation(const std::string &str,T &out)
{ integers_t::auto_notation<T>(str,out); }

template<typename T> SYM_INLINE T auto_notation(const std::string &str)
{ T t; integers_t::auto_notation<T>(str,t); return t; }

} // namespace integers

#include "integers.inl"

#endif // RD_UTILITIES_INTEGERS_HPP

