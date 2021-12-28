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

#ifndef INCLUDE_CONFIG_GCC_H
#define INCLUDE_CONFIG_GCC_H

#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)

#if defined(__GXX_EXPERIMENTAL_CXX0X__) || (__cplusplus >= 201103L)
    #define GCC_CXX11
#endif


#if (GCC_VERSION < 40400) || !defined(GCC_CXX11)
    #define NO_CXX11_AUTO_DECLARATIONS
    #define NO_CXX11_AUTO_MULTIDECLARATIONS
    #define NO_CXX11_CHAR16_T
    #define NO_CXX11_CHAR32_T
    #define NO_CXX11_HDR_INITIALIZER_LIST
    #define NO_CXX11_DEFAULTED_FUNCTIONS
    #define NO_CXX11_DELETED_FUNCTIONS
    #define NO_CXX11_TRAILING_RESULT_TYPES
    #define NO_CXX11_INLINE_NAMESPACES
    #define NO_CXX11_VARIADIC_TEMPLATES
#endif

// GCC 4.5 forbids declaration of defaulted functions in private or protected sections
#if __GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ == 5) || !defined(GCC_CXX11)
#  define NO_CXX11_NON_PUBLIC_DEFAULTED_FUNCTIONS
#endif

#if (GCC_VERSION < 40700) || !defined(GCC_CXX11)
    // constexpr is partly supported in gcc-4.6,it's a pre-std version with several bugs
    #define NO_CXX11_CONSTEXPR

    #define NO_CXX11_FINAL
    #define NO_CXX11_TEMPLATE_ALIASES
    #define NO_CXX11_USER_DEFINED_LITERALS
    #define NO_CXX11_FIXED_LENGTH_VARIADIC_TEMPLATE_EXPANSION_PACKS
    #define NO_CXX11_OVERRIDE
#endif

#if !defined(__cpp_constexpr) || (__cpp_constexpr < 201304)
    #define NO_CXX14_CONSTEXPR
#endif

#if (GCC_VERSION < 80000)
    #define ZERO_OBJ(obj) { memset(&obj,0,sizeof(obj)); }
#else
    #define ZERO_OBJ(obj) { memset(static_cast<void *>(&obj),0,sizeof(obj)); }
#endif

#endif // INCLUDE_CONFIG_GCC_H
