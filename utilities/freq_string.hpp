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

#ifndef RD_UTILITIES_FREQ_STRING_HPP
#define RD_UTILITIES_FREQ_STRING_HPP

#include "exception.hpp"
#include <vector>
#include <boost/format.hpp>
#include "floating_point_numbers.hpp"

/*
 *
 * freq_string is a std::string type string with this format :
 *
 *     1. "freq_abbrev_string"(;|,)"freq_abbrev_string"(;|,)......([;,]?)
 *
 *     2. And freq_abbrev_string showing above must be one of these formats :
 *
 *        a. Normal : "([-+]?)(\\d*)(\\.?)(\\d+)([KMG]?)";
 *
 *           "123"  "+12.3"  "-.4"  "1K"  "12.3M"  "-.4G"
 *
 *        b. Scientific : "([-+]?)(\\d*)(\\.?)(\\d+)(e|E)(\\d*)(\\.?)(\\d+)[KMG]?";
 *
 *           "12e3"  "-12E4" "12.3e4" "12.3e.4"  "12e3K"  "-12E4M"  "12.3e1G"
 *
 *     3. These are valid freq strings :
 *
 *        "1M;2M;3M"  "1G,2G,3G,"  "1.2M;1e1M,.1G;"
 *
 *
 * range_freq_string is a group of
 * star_freq_string,stop_freq_string and step_freq_string :
 *
 *     1. Must : Section of star_freq_string = stop_freq_string.
 *
 *     2. No limit of step_freq_string as long as its section > 0.
 *
 *     3. Repeated results are ignored!
 *
 *     4. This is a valid range_freq_string :
 *
 *        range_freq_string
 *        .star           .stop             .step
 *          |               |                 |
 *        "10M;100M;1G"   "20M;150M;1.1G"   "10M"
 *
 *        It can be parsed to range_freqs<uint64_t>:
 *
 *        .freqs = {10M,20M,   100M,110M,120M,130M,140M,150M,   1G,1.01G,...1G}
 *
 *        .star = {10M,100M,1G}
 *
 *        .stop = {20M,150M,1.1G}
 *
 *        .max = 1.1G
 *
 *        .min = 10M
 *
 */

template <typename T>
struct range_freq {
    std::vector<T> freqs;
    std::vector<T> star;
    std::vector<T> stop;
    T max;
    T min;

    bool has(const T &freq)
    {
        for (size_t i = 0;i < freqs.size();i ++) {
            if (freq - freqs.at(i) == T(0)) {
                return true;
            }
        }
        return false;
    }
};

template <typename T>
struct range_pwr {
    std::vector<T> pwrs;
    std::vector<T> star;
    std::vector<T> stop;
    T max;
    T min;
    range_pwr<T> & operator = (const range_freq<T> &fake_pwr)
    {
        this->pwrs = fake_pwr.freqs;
        this->star = fake_pwr.star;
        this->stop = fake_pwr.stop;
        this->max = fake_pwr.max;
        this->min = fake_pwr.min;
        return *this;
    }
};

typedef range_pwr<float> range_temp;

template <typename T>
int32_t freq_section(T freq, range_freq<T> &range)
{
    RD_ASSERT_THROW(range.star.size() == range.stop.size());
    RD_ASSERT_THROW(freq >= range.min);
    RD_ASSERT_THROW(freq <= range.max);

    for (size_t i = 0;i < range.star.size();i ++) {
        if (freq >= range.star.at(i) && freq <= range.stop.at(i)) {
            return int32_t(i);
        }
    }
    return 0;
}

/*
 * Frequency abbreviation string to type T.
 *
 * string -----> int/float/double/int8_t......
 * "123" ------> 123(int)(u?int8_t)(u?int16_t)(u?int32_t)(u?int64_t)
 * "1G" -------> 1000000000(int)(u?int32_t)(u?int64_t)
 * "2G" -------> 2000000000.0(float)(double)
 * "1.2M" -----> 1200000(int)(u?int32_t)(u?int64_t)
 * "1.2M" -----> 1200000.0(float)(double)
 * "2e3" ------> 2000(int)(u?int16_t)(u?int32_t)(u?int64_t)
 * "2.5E3" ----> 2500(int)(u?int16_t)(u?int32_t)(u?int64_t)
 * "2.5E1M" ---> 25000000.0(float)(double)
 *
 * ......
 */
template <typename T>
T freq_string_to(const std::string &str)
{
    T out;
    floating_point_numbers::auto_notation<T>(str,out);
    return out;
}

#define IMPL_FREQ_STRING_TO_(type) \
SYM_INLINE type freq_string_to_##type(const std::string &str) \
{ return freq_string_to<type>(str); }

IMPL_FREQ_STRING_TO_(int)
IMPL_FREQ_STRING_TO_(float)
IMPL_FREQ_STRING_TO_(double)
IMPL_FREQ_STRING_TO_(int8_t)
IMPL_FREQ_STRING_TO_(int16_t)
IMPL_FREQ_STRING_TO_(int32_t)
IMPL_FREQ_STRING_TO_(int64_t)
IMPL_FREQ_STRING_TO_(uint8_t)
IMPL_FREQ_STRING_TO_(uint16_t)
IMPL_FREQ_STRING_TO_(uint32_t)
IMPL_FREQ_STRING_TO_(uint64_t)


/*
 * Frequency abbreviation string from type T.
 * Only support integer types.
 *
 * int/int8_t/int16_t...... ---> String
 * 123 ------------------------> "123"
 * 1200 -----------------------> "1.2K"
 * 2000000 --------------------> "2M"
 * 3400000000 -----------------> "3.4G"
 *
 * Type specialized functions :
 * freq_string_from_int(xxx);
 * freq_string_from_int8_t(xxx);
 * freq_string_from_int16_t(xxx);
 * ......
 */
enum freq_string_unit_priority_t {
    FSU_G,   // GHz
    FSU_M,   // MHz
    FSU_K,   // KHz
    FSU_NONE // None
};

template <typename T>
std::string freq_string_from(const T &freq, freq_string_unit_priority_t priority = FSU_M)
{
    char unit = '\0';
    double freq_copy = double(freq);
    std::string str;

    if (freq_copy < 0) {
        freq_copy *= -1.0;
        str += "-";
    }

    if (priority == FSU_G) {
        if (freq_copy >= 1e9) {
            freq_copy /= 1e9;
            unit = 'G';
        } else if (freq_copy >= 1e6) {
            freq_copy /= 1e6;
            unit = 'M';
        } else if (freq_copy >= 1e3) {
            freq_copy /= 1e3;
            unit = 'K';
        }
    } else if (priority == FSU_M) {
        if (freq_copy >= 1e6) {
            freq_copy /= 1e6;
            unit = 'M';
        } else if (freq_copy >= 1e3) {
            freq_copy /= 1e3;
            unit = 'K';
        }
    } else if (priority == FSU_K) {
        if (freq_copy >= 1e3) {
            freq_copy /= 1e3;
            unit = 'K';
        }
    }

    boost::format fmt("%g");
    fmt % freq_copy;
    str += fmt.str();

    if (unit != '\0') {
        str += unit;
    }

    return str;
}

#define IMPL_FREQ_STRING_FROM_(type) \
SYM_INLINE std::string freq_string_from_##type(const type &freq) \
{ return freq_string_from<type>(freq); }

IMPL_FREQ_STRING_FROM_(int)
IMPL_FREQ_STRING_FROM_(int8_t)
IMPL_FREQ_STRING_FROM_(int16_t)
IMPL_FREQ_STRING_FROM_(int32_t)
IMPL_FREQ_STRING_FROM_(int64_t)
IMPL_FREQ_STRING_FROM_(uint8_t)
IMPL_FREQ_STRING_FROM_(uint16_t)
IMPL_FREQ_STRING_FROM_(uint32_t)
IMPL_FREQ_STRING_FROM_(uint64_t)


//SYM_INLINE uint32_t parse_range_temp_string(const range_temp_string &temp_string, range_temp &temp_out)
//{ return parse_range_pwr_string(temp_string,temp_out); }

#endif // RD_UTILITIES_FREQ_STRING_HPP

