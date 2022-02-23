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

#include "floating_point_numbers.hpp"
#include "exception.hpp"
#include <regex>
#include <cmath>
#include <algorithm>
#include "boost/format.hpp"
#include <boost/algorithm/string/trim.hpp>

template <typename T,typename data_type>
void floating_point_numbers::floating_point_numbers_t::scientific_notation(const std::string &str,T &out)
{
    size_t pos_e = str.find('e');
    size_t pos_E = std::string::npos;
    size_t pos = pos_e;
    double base = 0.0;
    double exponent = 0.0;
    double value = 0.0;

    if (pos_e == std::string::npos) {
        pos_E = str.find('E');
        pos = pos_E;
    }

    normal_notation(str.substr(0,pos),base);
    normal_notation(str.substr(pos + 1,str.length()),exponent);
    value = base * pow(10.0,exponent);
    out = T(value);
}

template <typename T,typename data_type>
void floating_point_numbers::floating_point_numbers_t::non_unit_notation(const std::string &str,T &out)
{
    if (str.length() < 1) {
        throw syntax_err("Invalid string : \"" + str + "\",it's empty.");
    }

    std::string str_copy = boost::trim_copy(str);
    std::regex reg_normal("([-+]?)(\\d*)(\\.?)(\\d+)");
    std::regex reg_scientific("([-+]?)(\\d*)(\\.?)(\\d+)(e|E)(\\d*)(\\.?)(\\d+)");

    if (str_copy.find(' ') != std::string::npos) {
        throw syntax_err("Invalid string : \"" + str_copy + "\",with space character.");
    }

    if (std::regex_match(str_copy,reg_normal)) {
        normal_notation<T>(str_copy,out);
    } else if (std::regex_match(str_copy,reg_scientific)) {
        scientific_notation<T>(str_copy,out);
    } else {
        throw syntax_err("Invalid string : \""  + str_copy + "\".");
    }
}

/*
 * freq string is a string with this format :
 *
 * a. Normal : "([-+]?)(\\d*)(\\.?)(\\d+)([KMG]?)";
 *
 *    "123";"+12.3";"-.4";"1K";"12.3M";"-.4G"
 *
 * b. Scientific : "([-+]?)(\\d*)(\\.?)(\\d+)(e|E)(\\d*)(\\.?)(\\d+)[KMG]?";
 *
 *    "12e3";"-12E4";"12.3e4";"12.3e.4";"12e3K";"-12E4M";"12.3e1G"
 */
template <typename T,typename data_type>
void floating_point_numbers::floating_point_numbers_t::auto_notation(const std::string &str,T &out)
{
    if (str.length() < 1) {
        throw syntax_err("Invalid string : \"" + str + "\",it's empty.");
    }

    std::string str_copy = boost::trim_copy(str);
    std::string str_freq_numerical = str_copy;
    std::string::const_reference unit = str_copy.back();

    double freq_numerical = 0.0;
    bool with_unit = false;

    if (unit == 'K' || unit == 'k' || unit == 'M' || unit == 'm' || unit == 'G' || unit == 'g') {
        str_freq_numerical = std::string(str_copy,0,str_copy.length() - 1);
        with_unit = true;
    }

    non_unit_notation(str_freq_numerical,freq_numerical);

    if (with_unit) {
        freq_numerical *= ((unit == 'K' || unit == 'k') ? 1e3  :
                          ((unit == 'M' || unit == 'm') ? 1e6  :
                          ((unit == 'G' || unit == 'g') ? 1e9  : 1)));
    }

    out = T(freq_numerical);
}

