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

#include "sequence.hpp"

namespace sequence {

template<typename T>
size_t arithmetic_sequence_t<T>::pts() const
{
    return size_t((stop - star) / step + 1);
}

template<typename T>
void arithmetic_sequence_t<T>::to_string(std::string &str) const
{
    str.clear();

    str = pts() > 1 ? (freq_string_from(star) + ":" +
                       freq_string_from(step) + ":" +
                       freq_string_from(stop) + ";") :
                       freq_string_from(star) + ";";
}


template<typename T,
template<typename E = arithmetic_sequence_t<T>,typename allocator_t = std::allocator<E>> class container_t>
int32_t parse(const std::string &str,container_t<arithmetic_sequence_t<T>> &out)
{
    uint32_t sec = set_helper::set_helper_t<T>::sections(str);
    range_string each_range;
    arithmetic_sequence_t<T> each_sequence;

    out.clear();

    if (sec == 0) {
        return 0;
    }

    std::vector<std::string> splitted_str;

    set_helper::set_helper_t<T>::split(str,sec,splitted_str);

    for (uint32_t i = 0;i < sec;i ++) {
        set_helper::split(splitted_str[i],each_range);

        floating_point_numbers::auto_notation<T>(each_range.star,each_sequence.star);
        floating_point_numbers::auto_notation<T>(each_range.step,each_sequence.step);
        floating_point_numbers::auto_notation<T>(each_range.stop,each_sequence.stop);

        out.insert(out.end(),each_sequence);
    }

    return 0;
}

template<typename T,typename container_t>
void parse(const container_t &in,std::string &str)
{
    std::string star;
    std::string step;
    std::string stop;

    typename container_t::const_iterator iter_in;

    str.clear();

    if (in.size() == 0) {
        return;
    }

    for (iter_in = in.cbegin();iter_in != in.cend();++iter_in) {
        str += iter_in->to_string();
    }
}

} // namespace sequence
