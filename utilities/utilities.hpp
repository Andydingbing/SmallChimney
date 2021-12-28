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

#ifndef RD_UTILITIES_UTILITIES_HPP
#define RD_UTILITIES_UTILITIES_HPP

#include "integers.hpp"
#include <fmt/core.h>
#include <string>
#include <boost/format.hpp>

/*
 * Size abbreviation string from type T.
 * Only support integer types.
 *
 * int/int8_t/int16_t...... ---> String
 * 1048576 --------------------> "1M"
 *
 * Type specialized functions :
 * size_abbrev_from_int(xxx);
 * size_abbrev_from_int8_t(xxx);
 * size_abbrev_from_int16_t(xxx);
 * ......
 */
template <typename T>
std::string size_abbrev_from(const T &freq)
{
    char unit = '\0';
    double freq_copy = double(freq);
    std::string str;

    if (freq_copy < 0) {
        freq_copy *= -1.0;
        str += "-";
    }

    if (freq_copy >= (1024 * 1024 * 1024)) {
        freq_copy /= (1024 * 1024 * 1024);
        unit = 'G';
    } else if (freq_copy >= (1024 * 1024)) {
        freq_copy /= (1024 * 1024);
        unit = 'M';
    } else if (freq_copy >= 1024) {
        freq_copy /= 1024;
        unit = 'K';
    }

    boost::format fmt("%g");
    fmt % freq_copy;
    str += fmt.str();

    if (unit != '\0') {
        str += unit;
    }

    return str;
}

#define MAKE_FUNC_SIZE_ABBREV_FROM_(type) \
SYM_INLINE std::string size_abbrev_from_##type(const type &freq) \
{ return size_abbrev_from<type>(freq); }

MAKE_FUNC_SIZE_ABBREV_FROM_(int)
MAKE_FUNC_SIZE_ABBREV_FROM_(int8_t)
MAKE_FUNC_SIZE_ABBREV_FROM_(int16_t)
MAKE_FUNC_SIZE_ABBREV_FROM_(int32_t)
MAKE_FUNC_SIZE_ABBREV_FROM_(int64_t)
MAKE_FUNC_SIZE_ABBREV_FROM_(uint8_t)
MAKE_FUNC_SIZE_ABBREV_FROM_(uint16_t)
MAKE_FUNC_SIZE_ABBREV_FROM_(uint32_t)
MAKE_FUNC_SIZE_ABBREV_FROM_(uint64_t)


template<typename int_t,typename traits_type = traits_int<int_t>>
SYM_INLINE bool is_even(int_t value)
{
    return (value & 1) == 0;
}

template<typename str_t,typename int_t,typename traits_t = traits_int<int_t>>
str_t string_of(const int_t *in,const size_t size,const str_t &separator)
{
    str_t str;
    boost::format fmt("%d");

    for (size_t i = 0;i < size;++i) {
        fmt % in[i];
        str += fmt.str().c_str();
        str += separator;
    }
    return str;
}

template<typename str_t,typename int_t,typename container_t,typename traits_type = traits_int<int_t>>
str_t string_of(const container_t &in,const str_t &separator)
{
    str_t str;
    typename container_t::const_iterator iter;

    for (iter = in.begin();iter != in.end();++iter) {
        str += (fmt::format("{:d}",*iter)).c_str();
        str += separator;
    }
    return str;
}

// std::vector<>,std::list<>...
template<typename str_t,typename int_t,
template<typename T_p = int_t,typename allocator_t = std::allocator<T_p>> class container_t,
typename traits_type = traits_int<int_t>>
str_t string_of(const container_t<int_t> &in,const str_t &separator)
{ return string_of<str_t,int_t,container_t<int_t>>(in,separator); }

// QVector...
template<typename str_t,typename int_t,
template<typename T_p = int_t> class container_t,
typename traits_type = traits_int<int_t>>
str_t string_of(const container_t<int_t> &in,const str_t &separator)
{ return string_of<str_t,int_t,container_t<int_t>>(in,separator); }


template<typename int_t,typename container_t,typename traits_t = traits_int<int_t>>
void string_to_container(const std::string &str,const char separator,container_t &out)
{
    std::string each_str;
    int_t each_int = 0;

    for (size_t i = 0;i < str.length();++i) {
        if (str[i] == separator) {
            integers::normal_notation<int_t>(each_str,each_int);
            out.push_back(each_int);

//            ++i;
            each_str.clear();
        } else {
            each_str.push_back(str[i]);
        }
    }
}

#endif // RD_UTILITIES_UTILITIES_HPP
