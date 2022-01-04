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

#ifndef INCLUDE_CONFIG_MSC_H
#define INCLUDE_CONFIG_MSC_H

/*
 * https://docs.microsoft.com/en-us/cpp/preprocessor/predefined-macros?view=msvc-160
 *
 * Visual Studio version              _MSC_VER
 *
 * Visual Studio 6.0                  1200
 * Visual Studio .NET 2002 (7.0)      1300
 * Visual Studio .NET 2003 (7.1)      1310
 * Visual Studio 2005 (8.0)           1400
 * Visual Studio 2008 (9.0)           1500
 * Visual Studio 2010 (10.0)          1600
 * Visual Studio 2012 (11.0)          1700
 * Visual Studio 2013 (12.0)          1800
 * Visual Studio 2015 (14.0)          1900
 * Visual Studio 2017 RTW (15.0)      1910
 * Visual Studio 2017 version 15.3    1911
 * Visual Studio 2017 version 15.5    1912
 * Visual Studio 2017 version 15.6    1913
 * Visual Studio 2017 version 15.7    1914
 * Visual Studio 2017 version 15.8    1915
 * Visual Studio 2017 version 15.9    1916
 * Visual Studio 2019 RTW (16.0)      1920
 * Visual Studio 2019 version 16.1    1921
 * Visual Studio 2019 version 16.2    1922
 * Visual Studio 2019 version 16.3    1923
 * Visual Studio 2019 version 16.4    1924
 * Visual Studio 2019 version 16.5    1925
 * Visual Studio 2019 version 16.6    1926
 * Visual Studio 2019 version 16.7    1927
 * Visual Studio 2019 version 16.8    1928
 */

/*
 * https://dev.to/yumetodo/list-of-mscver-and-mscfullver-8nd#note-t1-1
 *
 * Abbreviation        Product name [Visual Studio version]    VC ++ version   _MSC_VER    _MSC_FULL_VER
 * 2019 Update 8       Visual Studio 2019 version 16.8.2       14.28           1928        192829334
 * 2019 Update 8       Visual Studio 2019 version 16.8.1       14.28           1928        192829333
 * 2019 Update 7       Visual Studio 2019 version 16.7         14.27           1927        192729112
 * 2019 Update 6       Visual Studio 2019 version 16.6.2       14.26           1926        192628806
 * 2019 Update 5       Visual Studio 2019 version 16.5.1       14.25           1925        192528611
 * 2019 Update 4       Visual Studio 2019 version 16.4.0       14.24           1924        192428314
 * 2019 Update 3       Visual Studio 2019 version 16.3.2       14.21           1923        192328105
 * 2019 Update 2       Visual Studio 2019 version 16.2.3       14.21           1922        192227905
 * 2019 Update 1       Visual Studio 2019 version 16.1.2       14.21           1921        192127702
 * 2019                Visual Studio 2019 version 16.0.0       14.20           1920        192027508
 * 2017 Update 9       Visual Studio 2017 version 15.9.11      14.16           1916        191627030
 * 2017 Update 9       Visual Studio 2017 version 15.9.7       14.16           1916        191627027
 * 2017 Update 9       Visual Studio 2017 version 15.9.5       14.16           1916        191627026
 * 2017 Update 9       Visual Studio 2017 version 15.9.4       14.16           1916        191627025
 * 2017 Update 9       Visual Studio 2017 version 15.9.1       14.16           1916        191627023
 * 2017 Update 9       Visual Studio 2017 version 15.9.0       14.16           1916
 * 2017 Update 8       Visual Studio 2017 version 15.8.0       14.15           1915
 * 2017 Update 7       Visual Studio 2017 version 15.7.5       14.14           1914        191426433
 * 2017 Update 7       Visual Studio 2017 version 15.7.3       14.14           1914        191426430
 * 2017 Update 7       Visual Studio 2017 version 15.7.2       14.14           1914        191426429
 * 2017 Update 7       Visual Studio 2017 version 15.7.1       14.14           1914        191426428
 * 2017 Update 6       Visual Studio 2017 version 15.6.7       14.13           1913        191326132
 * 2017 Update 6       Visual Studio 2017 version 15.6.6       14.13           1913        191326131
 * 2017 Update 6       Visual Studio 2017 version 15.6.4       14.13           1913        191326129
 * 2017 Update 6       Visual Studio 2017 version 15.6.3       14.13           1913        191326129
 * 2017 Update 6       Visual Studio 2017 version 15.6.2       14.13           1913        191326128
 * 2017 Update 6       Visual Studio 2017 version 15.6.1       14.13           1913        191326128
 * 2017 Update 6       Visual Studio 2017 version 15.6.0       14.13           1913        191326128
 * 2017 Update 5       Visual Studio 2017 version 15.5.7       14.12           1912        191225835
 * 2017 Update 5       Visual Studio 2017 version 15.5.6       14.12           1912        191225835
 * 2017 Update 5       Visual Studio 2017 version 15.5.4       14.12           1912        191225834
 * 2017 Update 5       Visual Studio 2017 version 15.5.3       14.12           1912        191225834
 * 2017 Update 5       Visual Studio 2017 version 15.5.2       14.12           1912        191225831
 * 2017 Update 4       Visual Studio 2017 version 15.4.5       14.11           1911        191125547
 * 2017 Update 4       Visual Studio 2017 version 15.4.4       14.11           1911        191125542
 * 2017 Update 3       Visual Studio 2017 version 15.3.3       14.11           1911        191125507
 * 2017 Update 2       Visual Studio 2017 version 15.2         14.10           1910        191025017
 * 2017 Update 1       Visual Studio 2017 version 15.1         14.10           1910        191025017
 * 2017                Visual Studio 2017 version 15.0         14.10           1910        191025017
 * 2015 Update 3       Visual Studio 2015 Update 3 [14.0]      14.0            1900        190024210
 * 2015 Update 2       Visual Studio 2015 Update 2 [14.0]      14.0            1900        190023918
 * 2015 Update 1       Visual Studio 2015 Update 1 [14.0]      14.0            1900        190023506
 * 2015                Visual Studio 2015 [14.0]               14.0            1900        190023026
 * 2013 Nobemver CTP   Visual Studio 2013 Nobemver CTP [12.0]  12.0            1800        180021114
 * 2013 Update 5       Visual Studio 2013 Update 5 [12.0]      12.0            1800        180040629
 * 2013 Update 4       Visual Studio 2013 Update 4 [12.0]      12.0            1800        180031101
 * 2013 Update 3       Visual Studio 2013 Update 3 [12.0]      12.0            1800        180030723
 * 2013 Update 2       Visual Studio 2013 Update 2 [12.0]      12.0            1800        180030501
 * 2013 Update2 RC     Visual Studio 2013 Update2 RC [12.0]    12.0            1800        180030324
 * 2013 Update 1       Visual Studio 2013 Update 1 [12.0]      12.0            1800        180021005
 * 2013                Visual Studio 2013 [12.0]               12.0            1800        180021005
 * 2013 RC             Visual Studio 2013 RC [12.0]            12.0            1800        180020827
 * 2013 Preview        Visual Studio 2013 Preview [12.0]       12.0            1800        180020617
 * 2012 November CTP   Visual Studio 2012 November CTP [11.0]  11.0            1700        170051025
 * 2012 Update 4       Visual Studio 2012 Update 4 [11.0]      11.0            1700        170061030
 * 2012 Update 3       Visual Studio 2012 Update 3 [11.0]      11.0            1700        170060610
 * 2012 Update 2       Visual Studio 2012 Update 2 [11.0]      11.0            1700        170060315
 * 2012 Update 1       Visual Studio 2012 Update 1 [11.0]      11.0            1700        170051106
 * 2012                Visual Studio 2012 [11.0]               11.0            1700        170050727
 * 2010 SP1            Visual Studio 2010 SP1 [10.0]
 *                     Visual C++ 2010 SP1 [10.0]              10.0            1600        160040219
 * 2010                Visual Studio 2010 [10.0]
 *                     Visual C++ 2010 [10.0]                  10.0            1600        160030319
 * 2010 Beta 2         Visual Studio 2010 Beta 2 [10.0]        10.0            1600        160021003
 * 2010 Beta 1         Visual Studio 2010 Beta 1 [10.0]        10.0            1600        160020506
 * 2008 SP1            Visual Studio 2008 SP1 [9.0]
 *                     Visual C++ 2008 SP1 [9.0]               9.0             1500        150030729
 * 2008                Visual Studio 2008 [9.0]
 *                     Visual C++ 2008 [9.0]                   9.0             1500        150021022
 * 2008 Beta 2         Visual Studio 2008 Beta 2 [9.0]         9.0             1500        150020706
 * 2005 SP1            Visual Studio 2005 SP1 [8.0]
 *                     Visual C++ 2005 SP1 [8.0]               8.0             1400        140050727
 * 2005                Visual Studio 2005 [8.0]
 *                     Visual C++ 2005 [8.0]                   8.0             1400        140050320
 * 2005 Beta 2         Visual Studio 2005 Beta 2 [8.0]         8.0             1400        140050215
 * 2005 Beta 1         Visual Studio 2005 Beta 1 [8.0]         8.0             1400        140040607
 *                     Windows Server 2003 SP1 DDK (for AMD64)                 1400        140040310
 * 2003 SP1            Visual Studio .NET 2003 SP1 [7.1]
 *                     Visual C++ .NET 2003 SP1 [7.1]          7.1             1310        13106030
 *                     Windows Server 2003 SP1 DDK                             1310        13104035
 * 2003                Visual Studio .NET 2003 [7.1]
 *                     Visual C++ .NET 2003 [7.1]              7.1             1310        13103077
 *                     Visual Studio Toolkit 2003 [7.1]        7.1             1310        13103052
 * 2003 Beta           Visual Studio .NET 2003 Beta [7.1]      7.1             1310        13102292
 *                     Windows Server 2003 DDK                                 1310        13102179
 * 2002                Visual Studio .NET 2002 [7.0]
 *                     Visual C++ .NET 2002 [7.0]              7.0             1300        13009466
 *                     Windows XP SP1 DDK                                      1300        13009176
 * 6.0 SP6             Visual Studio 6.0 SP6
 *                     Visual C++ 6.0 SP6                      6.0             1200        12008804
 * 6.0 SP5             Visual Studio 6.0 SP5
 *                     Visual C++ 6.0 SP5                      6.0             1200        12008804
 *                     Visual Studio 97 [5.0]
 *                     Visual C++ 5.0                          5.0             1100
 *                     Visual C++ 4.2                          4.2             1020
 *                     Visual C++ 4.1                          4.1             1010
 *                     Visual C++ 4.0                          4.0             1000
 *                     Visual C++ 2.0                          2.0             900
 *                     Visual C++ 1.0                          1.0             800
 *                     Microsoft C/C++ 7.0                                     700
 *                     Microsoft C 6.0                                         600
 */


#if (_MSC_VER < 1700)
    #define NO_CXX11_FINAL
    #define NO_CXX11_RANGE_BASED_FOR
    #define NO_CXX11_SCOPED_ENUMS
    #define NO_CXX11_OVERRIDE
#endif // _MSC_VER < 1700

#if _MSC_FULL_VER < 180020827
    #define NO_CXX11_DEFAULTED_FUNCTIONS
    #define NO_CXX11_DELETED_FUNCTIONS
    #define NO_CXX11_EXPLICIT_CONVERSION_OPERATORS
    #define NO_CXX11_FUNCTION_TEMPLATE_DEFAULT_ARGS
    #define NO_CXX11_RAW_LITERALS
    #define NO_CXX11_TEMPLATE_ALIASES
    #define NO_CXX11_TRAILING_RESULT_TYPES
    #define NO_CXX11_VARIADIC_TEMPLATES
    #define NO_CXX11_UNIFIED_INITIALIZATION_SYNTAX
    #define NO_CXX11_DECLTYPE_N3276
#endif

#if (_MSC_FULL_VER < 190024210)
    #define NO_CXX14_VARIABLE_TEMPLATES
    #define NO_SFINAE_EXPR
    #define NO_CXX11_CONSTEXPR
#endif // _MSC_FULL_VER < 190024210

#if (_MSC_VER < 1915) || (_MSVC_LANG < 201402)
    // Still gives internal compiler error for msvc-15.5:
    #define NO_CXX14_CONSTEXPR
#endif

#define ZERO_OBJ(obj) { memset(&obj,0,sizeof(obj)); } NEED_SEMICOLON

#endif
