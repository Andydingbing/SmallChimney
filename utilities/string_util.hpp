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

#ifndef UTILITIES_STRING_UTIL_HPP
#define UTILITIES_STRING_UTIL_HPP

#include "global_header.h"
#include "traits.hpp"
#include <string>
#include "stdarg.h"
#include "stdio.h"

/*
 * trim_front(&ptr,args) :
 * trim_front(src,&ptr,args) :
 *
 *    src                        src
 *     |                          |
 *     v                          v
 *   _________________          _________________
 *   |' '|a|b|c|d|...|   --->   |' '|a|b|c|d|...|
 *   -----------------          -----------------
 *     ^                             ^
 *     |                             |
 *    ptr                           ptr
 *
 *
 * trim_front(ptr,args) :
 *   __________________         _________________
 *   |' '|' '|a|b|'\0'|   --->  |a|b|'\0'|b|'\0'|
 *   ------------------         -----------------
 *     ^                         ^
 *     |                         |
 *    ptr                       ptr
 */
template<template<typename T = char,typename allocator_t = std::allocator<T>> class container_t>
uint32_t trim_front(char **ptr,container_t<char> &args);

template<template<typename T = char,typename allocator_t = std::allocator<T>> class container_t>
uint32_t trim_front(const char* const src,const char **ptr,container_t<char> &args);

template<typename T = size_t,typename traits_t = traits_int<T>>
uint32_t trim_front(char **ptr,const T n,...)
{
    std::list<char> args;
    va_list ap;

    va_start(ap,n);

    for (size_t i = 0;i < n;++i) {
        args.push_back(char(va_arg(ap,int)));
    }

    va_end(ap);
    return trim_front(ptr,args);
}

template<template<typename T = char,typename allocator_t = std::allocator<T>> class container_t>
uint32_t trim_front(char *ptr,container_t<char> &args);

template<typename T = size_t,typename traits_t = traits_int<T>>
uint32_t trim_front(char *ptr,const T n,...)
{
    std::list<char> args;
    va_list ap;

    va_start(ap,n);

    for (size_t i = 0;i < n;++i) {
        args.push_back(char(va_arg(ap,int)));
    }

    va_end(ap);
    return trim_front(ptr,args);
}

template<typename str_t,
         template<typename T = char,typename allocator_t = std::allocator<T>> class container_t>
uint32_t trim_front(str_t &str,container_t<char> &args)
{
    char *ptr = const_cast<char *>(str.c_str());
    uint32_t n = trim_front(&ptr,args);

    str.erase(0,n);
    return n;
}

template<typename T = size_t,typename traits_t = traits_int<T>>
uint32_t trim_front(std::string &str,const T n,...)
{
    std::list<char> args;
    va_list ap;

    va_start(ap,n);

    for (size_t i = 0;i < n;++i) {
        args.push_back(char(va_arg(ap,int)));
    }

    va_end(ap);
    return trim_front<std::string>(str,args);
}


/*
 * trim_back(&ptr,args) :
 * trim_back(src,&ptr,args) :
 *
 *   ________________          ____________
 *   |a|b|c|' '|'\0'|   --->   |a|b|c|'\0'|
 *   ----------------          ------------
 *    ^                         ^
 *    |                         |
 *   ptr                       ptr
 *
 */
template<template<typename T = char,typename allocator_t = std::allocator<T>> class container_t>
uint32_t trim_back(char **ptr,container_t<char> &args);

template<template<typename T = char,typename allocator_t = std::allocator<T>> class container_t>
uint32_t trim_back(const char* const src,const char **ptr,container_t<char> &args);

template<typename T = size_t,typename traits_t = traits_int<T>>
uint32_t trim_back(char **ptr,const T n,...)
{
    std::list<char> args;
    va_list ap;

    va_start(ap,n);

    for (size_t i = 0;i < n;++i) {
        args.push_back(char(va_arg(ap,int)));
    }

    va_end(ap);
    return trim_back(ptr,args);
}

template<template<typename T = char,typename allocator_t = std::allocator<T>> class container_t>
uint32_t trim_back(char *ptr,container_t<char> &args);

template<typename T = size_t,typename traits_t = traits_int<T>>
uint32_t trim_back(char *ptr,const T n,...)
{
    std::list<char> args;
    va_list ap;

    va_start(ap,n);

    for (size_t i = 0;i < n;++i) {
        args.push_back(char(va_arg(ap,int)));
    }

    va_end(ap);
    return trim_back(ptr,args);
}

template<template<typename T = char,typename allocator_t = std::allocator<T>> class container_t = std::list>
uint32_t trim_back(std::string &str,container_t<char> &args)
{
    char *ptr = const_cast<char *>(str.c_str());
    uint32_t n = trim_back(ptr,args);

    for (uint32_t i = 0;i < n;++i) {
        str.pop_back();
    }
    return n;
}

template<typename T = size_t,typename traits_t = traits_int<T>>
uint32_t trim_back(std::string &str,const T n,...)
{
    std::list<char> args;
    va_list ap;

    va_start(ap,n);

    for (size_t i = 0;i < n;++i) {
        args.push_back(char(va_arg(ap,int)));
    }

    va_end(ap);
    return trim_back(str,args);
}

template<typename seperator_string_t,
         typename result_string_t,
         template<typename _Tp = seperator_string_t,typename allocator_t = std::allocator<_Tp>> class seperator_container_t,
         template<typename _Tp = result_string_t,   typename allocator_t = std::allocator<_Tp>> class result_container_t>
void split(const char *ptr,const seperator_container_t<seperator_string_t> &seperator,result_container_t<result_string_t> &result);

#include "string_util.inl"

#endif
