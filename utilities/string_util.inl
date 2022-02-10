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

#include "string_util.hpp"
#include <vector>

template<template<typename T = char,typename allocator_t = std::allocator<T>> class container_t>
uint32_t trim_front(char **ptr,container_t<char> &args)
{
    if (ptr == nullptr || *ptr == nullptr || strlen(*ptr) == 0) {
        return 0;
    }

    typename container_t<char>::const_iterator iter;
    uint32_t sum = 0;

    while (1) {
        if (**ptr == '\0') {
            break;
        }
        for (iter = args.cbegin();iter != args.cend();++iter) {
            if (**ptr == *iter) {
                sum ++;
                (*ptr) ++;
                break;
            }
        }
        if (iter == args.cend()) {
            break;
        }
    }
    return sum;
}

template<template<typename T = char,typename allocator_t = std::allocator<T>> class container_t>
uint32_t trim_front(const char* const src,const char **ptr,container_t<char> &args)
{
    *ptr = (char *)src;
    return trim_front(&ptr,args);
}

uint32_t trim_front(char **ptr,const size_t n,...)
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
uint32_t trim_front(char *ptr,container_t<char> &args)
{
    char *p = ptr;
    uint32_t n = trim_front(&p,args);

    memcpy(ptr,p,strlen(p) + 1);
    return n;
}

uint32_t trim_front(char *ptr,const size_t n,...)
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
uint32_t trim_back(char **ptr,container_t<char> &args)
{
    if (ptr == nullptr || *ptr == nullptr || strlen(*ptr) == 0) {
        return 0;
    }

    typename container_t<char>::const_iterator iter;
    uint32_t sum = 0;

    while (1) {
        for (iter = args.cbegin();iter != args.cend();++iter) {
            if (**ptr == *iter) {
                sum ++;
                (*ptr) --;
                break;
            }
        }
        if (iter == args.cend()) {
            break;
        }
    }
    return sum;
}

template<template<typename T = char,typename allocator_t = std::allocator<T>> class container_t>
uint32_t trim_back(const char* const src,const char **ptr,container_t<char> &args)
{
    *ptr = src + strlen(src) - 1;
    return trim_back(&ptr,args);
}

uint32_t trim_back(char **ptr,const size_t n,...)
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
uint32_t trim_back(char *ptr,container_t<char> &args)
{
    char *p = ptr + strlen(ptr) - 1;
    uint32_t n = trim_back(&p,args);

    if (strlen(p) > 0) {
        *(p + 1) = '\0';
    }

    return n;
}

uint32_t trim_back(char *ptr,const size_t n,...)
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


template<typename seperator_string_t,
         typename result_string_t,
         template<typename _Tp = seperator_string_t,typename allocator_t = std::allocator<_Tp>> class seperator_container_t,
         template<typename _Tp = result_string_t,   typename allocator_t = std::allocator<_Tp>> class result_container_t>
void split(char *ptr,const seperator_container_t<seperator_string_t> &seperator,result_container_t<result_string_t> &result)
{
    if (ptr == nullptr) {
        return;
    }

    result_string_t each_string;
    char *left = ptr;
    char *right = ptr;
    typename seperator_container_t<seperator_string_t>::const_iterator iter;

    while (right < ptr + strlen(ptr)) {
        for (iter = seperator.cbegin();iter != seperator.cend();++iter) {
            if ((strncmp(right,iter->c_str(),iter->length())) == 0) {
                each_string.clear();
                each_string.append(left,right);
                result.push_back(each_string);
                left = right + 1;
            }
        }
        if (iter == seperator.cend()) {
            right ++;
        }
    }

    each_string.clear();
    each_string.append(left,right);
    result.push_back(each_string);
}
