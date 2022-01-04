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

#ifndef INCLUDE_CONFIG_PLATFORM_H
#define INCLUDE_CONFIG_PLATFORM_H

#if (defined(linux) || defined(__linux) || defined(__linux__) || defined(__GLIBC__)) && !defined(_CRAYC) && !defined(__FREEBSD_kernel__) && !defined(__GNU__)
    #define PLATFORM_LINUX
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    #define PLATFORM_WIN_X86
#elif defined(_WIN64) || defined(__WIN64__) || defined(WIN64)
    #define PLATFORM_WIN_X64
#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
    #define PLATFORM_MACOS
#elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__FreeBSD_kernel__)
    #define PLATFORM_BSD
#endif

#if defined(PLATFORM_WIN_X86) || defined(PLATFORM_WIN_X64)
    #define PLATFORM_WIN32
#endif

#endif
