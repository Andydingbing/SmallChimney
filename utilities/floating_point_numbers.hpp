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

#ifndef RD_UTILITIES_FLOATING_POINT_NUMBERS_HPP
#define RD_UTILITIES_FLOATING_POINT_NUMBERS_HPP

#include "global_header.h"
#include "traits.hpp"
#include <sstream>

namespace floating_point_numbers {

class floating_point_numbers_t {
public:
    // "123" ---> 123; "12.3" ---> 12.3
    template <typename T,typename data_type = built_in_traits<T>>
    static void normal_notation(const std::string &str,T &out)
    { std::stringstream ss(str); ss >> out; }

    //"1e2" ---> 100/100.0
    template <typename T,typename data_type = built_in_traits<T>>
    static void scientific_notation(const std::string &str,T &out);

    // normal/scientific notation
    template <typename T,typename data_type = built_in_traits<T>>
    static void non_unit_notation(const std::string &str,T &out);

    /*
     * freq string is a string with this format :
     *
     * a. Normal : "([-+]?)(\\d*)(\\.?)(\\d+)([KMG]?)";
     *
     *    "123";"+12.3";"-.4";"1K";"12.3M";"-.4G"
     *
     * b. Scientific : "([-+]?)(\\d*)(\\.?)(\\d+)(e|E)(\\d*)(\\.?)(\\d+)[KMG]?";
     *
     *    "12e3";"-12E4";"12.3e4";"12.3e.4";"12e3K";"-12E4M";"12.3e1G"
     */
    template <typename T,typename data_type = built_in_traits<T>>
    static void auto_notation(const std::string &str,T &out);
};

template <typename T> SYM_INLINE void normal_notation(const std::string &str,T &out)
{ floating_point_numbers_t::normal_notation<T>(str,out); }

template <typename T> SYM_INLINE void scientific_notation(const std::string &str,T &out)
{ floating_point_numbers_t::scientific_notation<T>(str,out); }

template <typename T> SYM_INLINE void non_unit_notation(const std::string &str,T &out)
{ floating_point_numbers_t::non_unit_notation<T>(str,out); }

template <typename T> SYM_INLINE void auto_notation(const std::string &str,T &out)
{ floating_point_numbers_t::auto_notation<T>(str,out); }

} // namespace floating_point_numbers

#include "floating_point_numbers.inl"

#endif // RD_UTILITIES_FLOATING_POINT_NUMBERS_HPP

