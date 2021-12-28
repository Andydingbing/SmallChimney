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

#ifndef RD_UTILITIES_EXCEPTION_HPP
#define RD_UTILITIES_EXCEPTION_HPP

#include "global_header.h"
#include <stdexcept>
#include <string>
#include <boost/format.hpp>
#include <boost/functional/hash.hpp>
#include <boost/current_function.hpp>
#include <boost/core/ignore_unused.hpp>

/*
 * Define common exceptions used throughout the code:
 *
 * - The python built-in exceptions were used as inspiration.
 * - Exceptions inherit from std::exception to provide what().
 * - Exceptions inherit from sp_rd::exception to provide code().
 *
 * The code() provides an error code which allows the application
 * the option of printing a cryptic error message from the 1990s.
 *
 * The dynamic_clone() and dynamic_throw() methods allow us to:
 * catch an exception by dynamic type (i.e. derived class), save it,
 * and later rethrow it, knowing only the static type (i.e. base class),
 * and then finally to catch it again using the derived type.
 *
 * http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2006/n2106.html
 */

#ifdef RD_EXCEPTION
    #define RD_TRY try {

    #define RD_CATCH_ALL_EXCEPTION \
        } catch (expt &e) { \
            boost::ignore_unused(e); \
        }

    #define RD_LOG_ALL_EXCEPTION \
        } catch (expt &e) { \
            Log.add_msg(-1,"%s",e.what()); \
        }
#else
    #define RD_TRY
    #define RD_CATCH_ALL_EXCEPTION
#endif

// Base class of all exceptions.
struct expt : std::runtime_error
{
    expt(const std::string& what) : runtime_error(what) {}
    virtual uint32_t code(void) const            = 0;
    virtual exception* dynamic_clone(void) const = 0;
    virtual void dynamic_throw(void) const       = 0;
};

#define DECL_RD_EXCEPTION(expt_class, base, name) \
    struct expt_class : base { \
        expt_class(const std::string &what) :  \
            base((boost::format("%s: %s") % name % what).str()) {} \
        virtual uint32_t code(void) const \
            { return boost::hash<std::string>()(#expt_class) & 0xfff; } \
        virtual expt_class* dynamic_clone(void) const \
            { return new expt_class(*this); } \
        virtual void dynamic_throw(void) const \
            { throw *this;} \
    };

// Raised when an assert statement fails,such as RD_ASSERT_THROW().
DECL_RD_EXCEPTION(assertion_error, expt, "AssertionError")

// Raised when a key or index is invalid.
DECL_RD_EXCEPTION(lookup_error, expt, "LookupError")
// Raised when a key or index is invalid.
DECL_RD_EXCEPTION(index_error, lookup_error, "IndexError")
// Raised when a dictionary-like object is trying to be indexed by an invalid key.
DECL_RD_EXCEPTION(key_error, lookup_error, "KeyError")

// Raised when an operation or function is executed with a value of incorrect type.
DECL_RD_EXCEPTION(type_error, expt, "TypeError")

// Raised when an argument has the right type but invalid value.
DECL_RD_EXCEPTION(value_error, expt, "ValueError")
// Raised when a value is inappropriate because it can't be narrowed.
DECL_RD_EXCEPTION(narrowing_error, value_error, "NarrowingError")

// Raised when an error is detected that doesn't fall into any of the categories.
DECL_RD_EXCEPTION(runtime_error, expt, "RuntimeError")

// Base class for errors that occur outside of UHD.
DECL_RD_EXCEPTION(environment_error, expt, "EnvironmentError")
// Raised when an I/O operation fails for an I/O-related reason.
DECL_RD_EXCEPTION(io_error, environment_error, "IOError")
// Raised when a function returns a system-related error.
DECL_RD_EXCEPTION(os_error, environment_error, "OSError")

// Raised when a parser encounters a syntax error.
DECL_RD_EXCEPTION(syntax_error, expt, "SyntaxError")

/*
 * Create a formatted string with throw-site information.
 * Fills in the function name, file name, and line number.
 * \param what the std::exception message
 * \return the formatted exception message
 */
#define RD_THROW_SITE_INFO(what)                                                         \
    std::string(std::string(what) + "\n" + "  in " + std::string(BOOST_CURRENT_FUNCTION) \
                + "\n" + "  at " + std::string(__FILE__) + ":"                           \
                + BOOST_STRINGIZE(__LINE__) + "\n")

/*
 * Assert the result of the code evaluation.
 * If the code evaluates to false, throw an assertion error.
 * \param code the code that resolved to a boolean
 */
#define RD_ASSERT_THROW(code) { \
    if (!(code)) \
        throw assertion_error(RD_THROW_SITE_INFO(#code)); \
    }

#endif // RD_UTILITIES_EXCEPTION_HPP
