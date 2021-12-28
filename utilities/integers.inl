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

#include "integers.hpp"
#include "exception.hpp"
#include <cmath>
#include <algorithm>
#include "boost/format.hpp"
#include <boost/algorithm/string/trim.hpp>

template <typename T,typename data_type>
void integers::integers_t::auto_notation(const std::string &str,T &out)
{
    if (str.length() < 1) {
        throw syntax_error("Invalid string : \"" + str + "\",it's empty.");
    }

    std::string str_copy = boost::trim_copy(str);
    std::string str_numerical = str_copy;
    std::string::const_reference unit = str_copy.back();

    out = 0;
    bool with_unit = false;

    if (unit == 'K' || unit == 'k' || unit == 'M' || unit == 'm' || unit == 'G' || unit == 'g') {
        str_numerical = std::string(str_copy,0,str_copy.length() - 1);
        with_unit = true;
    }

    normal_notation<T>(str_numerical,out);

    if (with_unit) {
        out *= ((unit == 'K' || unit == 'k') ? 100 :
               ((unit == 'M' || unit == 'm') ? 1000000  :
               ((unit == 'G' || unit == 'g') ? 1000000000  : 1)));
    }
}

