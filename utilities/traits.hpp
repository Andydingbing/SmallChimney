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

#ifndef RD_UTILITIES_TRAITS_H
#define RD_UTILITIES_TRAITS_H

#include <boost/rational.hpp>

// floating point numbers
template <typename T>
class float_traits;

template<>
class float_traits<double> {
public:
    typedef double data_t;
    typedef double float_t;
    static float_t value_float(data_t x) { return x; }
    static data_t _0() { return 0.0; }
    static data_t _1() { return 1.0; }
    static bool equal_0(data_t x) { return x == 0.0; }
    static bool equal_1(data_t x) { return x == 1.0; }
};

template<>
class float_traits<float> {
public:
    typedef float data_t;
    typedef float float_t;
    static float_t value_float(data_t x) { return x; }
    static data_t _0() { return 0.0f; }
    static data_t _1() { return 1.0f; }
    static bool equal_0(data_t x) { return x == 0.0f; }
    static bool equal_1(data_t x) { return x == 1.0f; }
};

template <typename int_type>
class float_traits<boost::rational<int_type>> {
public:
    typedef boost::rational<int_type> data_t;
    typedef double float_t;
    static float_t value_float(data_t x) { return float_t(x.numerator()) / float_t(x.denominator()); }
    static data_t _0() { return boost::rational<int_type>(); }
    static data_t _1() { return boost::rational<int_type>(1,1); }
    static bool equal_0(data_t x) { return x == _0(); }
    static bool equal_1(data_t x) { return x == _1(); }
};


// built in types
template<typename T> class built_in_traits;
template<> class built_in_traits<float>    { typedef float    data_t; };
template<> class built_in_traits<double>   { typedef double   data_t; };
template<> class built_in_traits<int8_t>   { typedef int8_t   data_t; };
template<> class built_in_traits<uint8_t>  { typedef uint8_t  data_t; };
template<> class built_in_traits<int16_t>  { typedef int16_t  data_t; };
template<> class built_in_traits<uint16_t> { typedef uint16_t data_t; };
template<> class built_in_traits<int32_t>  { typedef int32_t  data_t; };
template<> class built_in_traits<uint32_t> { typedef uint32_t data_t; };
template<> class built_in_traits<int64_t>  { typedef int64_t  data_t; };
template<> class built_in_traits<uint64_t> { typedef uint64_t data_t; };

// integer types
template<typename T> class traits_int;
template<> class traits_int<int8_t>    { typedef int8_t   data_t; };
template<> class traits_int<uint8_t>   { typedef uint8_t  data_t; };
template<> class traits_int<int16_t>   { typedef int16_t  data_t; };
template<> class traits_int<uint16_t>  { typedef uint16_t data_t; };
template<> class traits_int<int32_t>   { typedef int32_t  data_t; };
template<> class traits_int<uint32_t>  { typedef uint32_t data_t; };
template<> class traits_int<int64_t>   { typedef int64_t  data_t; };
template<> class traits_int<uint64_t>  { typedef uint64_t data_t; };

template<typename T> class traits_uint;
template<> class traits_uint<uint8_t>  { typedef uint8_t  data_t; };
template<> class traits_uint<uint16_t> { typedef uint16_t data_t; };
template<> class traits_uint<uint32_t> { typedef uint32_t data_t; };
template<> class traits_uint<uint64_t> { typedef uint64_t data_t; };

#endif
