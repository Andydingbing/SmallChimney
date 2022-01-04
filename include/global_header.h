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

#ifndef INCLUDE_GLOABAL_H
#define INCLUDE_GLOABAL_H

#include "preprocessor/preprocessor.h"
#include "config/platform.h"
#include "stdint_common.h"
#include "enum.h"
#include "noncopyable.h"
#include <boost/thread.hpp>

#ifdef BOOST_THREAD_VERSION
#undef BOOST_THREAD_VERSION
#endif

#define BOOST_THREAD_VERSION 5

#define FAKE_TEMPLATE_CLASS_PREFIX(name) \
template<int T = 0> class name##_t { public :

#define FAKE_TEMPLATE_CLASS_INHERIT_PREFIX(name,parent) \
template<int T = 0> class name##_t : public parent { public :

#define FAKE_TEMPLATE_CLASS_SUFFIX(name) \
};typedef name##_t<> name;

#define FAKE_TEMPLATE_CLASS_FUNC(name) \
template<int T> int32_t name##_t<T>

#define AD908X_FUNC  FAKE_TEMPLATE_CLASS_FUNC(ad908x)
#define ADF5355_FUNC FAKE_TEMPLATE_CLASS_FUNC(adf5355)
#define LMX2594_FUNC FAKE_TEMPLATE_CLASS_FUNC(lmx2594)

#define VI_CHECK(func) { if ((status = (func)) < VI_SUCCESS) { return status; } } NEED_SEMICOLON

#define BOOST_SPTR_SAFE_MAKE(type,sptr) { if (!sptr) sptr = boost::make_shared<type>(); }

enum duplex_mode_t { FDD,TDD };

enum link_t { UL,DL };

struct iq_data_t {
    int16_t i : 16;
    int16_t q : 16;
};

//STRUCT_ALIGN_S(coef_complex_t,2)
//    int16_t real : 16;
//    int16_t imag : 16;
//STRUCT_ALIGN_E(coef_complex_t,2)

template<typename x_t,typename y_t>
struct point_2d { x_t x; y_t y; };


#endif
