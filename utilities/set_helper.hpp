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

#ifndef RD_UTILITIES_SET_HELPER_HPP
#define RD_UTILITIES_SET_HELPER_HPP

#include "global_header.h"
#include <string>
#include <set>
#include <boost/any.hpp>

// "star" : "step" : "stop"
typedef struct range_string {
    std::string star;
    std::string step;
    std::string stop;
}
range_freq_string,
range_pwr_string,
range_temp_string;

namespace set_helper {

template<typename T,typename container_t = std::set<T>>
struct set_helper_t
{
    container_t data;
    container_t star;
    container_t stop;
    T max;
    T min;

//    bool has(const T &element)
//    { return data.find(element) != data.end(); }

    int32_t section(const T &element);
    uint32_t parse_from(const std::string &str);

    container_t after(const T &lower_bound) const;
    container_t before(const T &upper_bound) const;
    container_t between(const T &lower_bound,const T &upper_bound) const;

    uint32_t pts_after(const T &lower_bound) const;
    uint32_t pts_before(const T &upper_bound) const;
    uint32_t pts_between(const T &lower_bound,const T &upper_bound) const;

    static uint32_t pts(const std::string &str);

    static uint32_t sections(const std::string &str);
    static uint32_t sections(const char *str);
    static std::string add_as_freq_string(const std::string &str1,const std::string &str2);

    // Usually called after sections().So there is no syntax check.
    // e.g. "1G;2G;3G" ---> {"1G","2G","3G"}
    template<typename specialized_container_t>
    static void split(const std::string &str,uint32_t sections,specialized_container_t &out);

    // std::vector<>,std::list<>...
    template<typename str_t = std::string,
    template<typename T_p = str_t,typename allocator_t = std::allocator<T_p>> class container>
    static void split(const std::string &str,uint32_t sections,container<T> &out)
    { split<container<str_t>>(str,sections,out); }

    // QVector...
    template<typename str_t = std::string,template<typename T_p = str_t> class container>
    static void split(const std::string &str,uint32_t sections,container<str_t> &out)
    { split<container<str_t>>(str,sections,out); }

    static void split(const char *str,uint32_t sections,char (*out)[32]);

    static void split(const std::string &str,range_string &out);
};

template<typename T>
struct range_freq : public set_helper_t<T,std::set<T>> {
    std::set<T> &freq = this->data;
};

template<typename T>
struct range_pwr : public set_helper_t<T,std::set<T>> {
    std::set<T> &pwr = this->data;
};

SYM_INLINE uint32_t sections(const std::string &str)
{ return set_helper_t<boost::any>::sections(str); }

SYM_INLINE uint32_t sections(const char *str)
{ return set_helper_t<boost::any>::sections(str); }

template<typename T = std::string,
template<typename E = T,typename allocator_t = std::allocator<E>> class container>
SYM_INLINE void split(const std::string &str,uint32_t sections,container<T> &out)
{ return set_helper_t<boost::any>::split(str,sections,out); }

template<typename T = std::string,template<typename E = T> class container>
SYM_INLINE void split(const std::string &str,uint32_t sections,container<T> &out)
{ return set_helper_t<boost::any>::split(str,sections,out); }

SYM_INLINE void split(const char *str,uint32_t sections,char (*out)[32])
{ return set_helper_t<boost::any>::split(str,sections,out); }

SYM_INLINE void split(const std::string &str,range_string &out)
{ return set_helper_t<boost::any>::split(str,out); }

template<typename T>
SYM_INLINE uint32_t parse(const std::string &str,set_helper_t<T> &set)
{ return set.parse_from(str); }

template<typename T>
SYM_INLINE uint32_t parse(const std::string &str,range_freq<T> &out)
{ return out.parse_from(str); }

template<typename T>
SYM_INLINE uint32_t parse(const std::string &str,range_pwr<T> &out)
{ return out.parse_from(str); }

template<typename T>
SYM_INLINE set_helper_t<T> parse(const std::string &str)
{ set_helper_t<T> set;set.parse_from(str);return set; }

template<typename T>
SYM_INLINE range_freq<T> parse_to_freq(const std::string &str)
{ range_freq<T> out;out.parse_from(str);return out; }

template<typename T>
SYM_INLINE range_pwr<T> parse_to_pwr(const std::string &str)
{ range_pwr<T> out; out.parse_from(str); return out; }

template<typename T>
SYM_INLINE uint32_t pts(const std::string &str)
{ return set_helper_t<T>::pts(str); }

} // namespace set_helper

#include "set_helper.inl"


template<typename T,typename container_t = std::set<T>>
void sequence_string_of(const container_t &data,std::string &str);

template<typename T,typename container_t = std::set<T>>
SYM_INLINE std::string sequence_string_of(const container_t &data)
{ std::string str; sequence_string_of<T,container_t>(data,str); return str; }

template<typename T,typename container_t = std::set<T>>
SYM_INLINE void sequence_string_of(const set_helper::set_helper_t<T> &set,std::string &str)
{ sequence_string_of<T,container_t>(set.data,str); }

template<typename T,typename container_t = std::set<T>>
SYM_INLINE std::string sequence_string_of(const set_helper::set_helper_t<T> &set)
{ std::string str; sequence_string_of<T,container_t>(set,str); return str; }

template<typename T>
uint32_t parse_range_freq_string(const range_freq_string &freq_string, range_freq<T> &freq_out)
{
    uint32_t section_star = set_helper::sections(freq_string.star);
    uint32_t section_stop = set_helper::sections(freq_string.stop);
    uint32_t section_step = set_helper::sections(freq_string.step);

    if (section_star == 0 || section_stop == 0 || section_step == 0) {
        freq_out = range_freq<T>();
        return 0;
    }

    RD_ASSERT_THROW(section_star == section_stop);

    freq_out.freqs.clear();
    freq_out.star.clear();
    freq_out.stop.clear();

    T T_freq_star = 0;
    T T_freq_stop = 0;
    T T_freq_step = 0;
    std::vector<std::string> splitted_freq_star;
    std::vector<std::string> splitted_freq_stop;
    std::vector<std::string> splitted_freq_step;
    typename std::vector<T>::iterator iter_freqs_repeated_begin;

    set_helper::split(freq_string.star,section_star,splitted_freq_star);
    set_helper::split(freq_string.stop,section_stop,splitted_freq_stop);
    set_helper::split(freq_string.step,section_step,splitted_freq_step);

    for (uint32_t i = 0;i < section_star;i ++) {
        T_freq_star = freq_string_to<T>(splitted_freq_star.at(i));
        T_freq_stop = freq_string_to<T>(splitted_freq_stop.at(i));
        T_freq_step = freq_string_to<T>(splitted_freq_step.at(std::min(i,section_step - 1)));

        freq_out.star.push_back(T_freq_star);
        freq_out.stop.push_back(T_freq_stop);

        for (uint32_t j = 0;j < SERIE_SIZE(T_freq_star,T_freq_stop,T_freq_step);j ++) {
            freq_out.freqs.push_back(T_freq_star + T(j) * T_freq_step);
        }
    }

    std::sort(freq_out.freqs.begin(),freq_out.freqs.end());

    iter_freqs_repeated_begin = unique(freq_out.freqs.begin(),freq_out.freqs.end());
    freq_out.freqs.erase(iter_freqs_repeated_begin,freq_out.freqs.end());

    freq_out.max = freq_out.freqs.at(freq_out.freqs.size() - 1);
    freq_out.min = freq_out.freqs.at(0);

    return uint32_t(freq_out.freqs.size());
}

template <typename T>
SYM_INLINE uint32_t parse_range_pwr_string(const range_pwr_string &pwr_string, range_pwr<T> &pwr_out)
{
    range_freq<T> fake_pwr_out;
    uint32_t pts = parse_range_freq_string(pwr_string,fake_pwr_out);
    pwr_out = fake_pwr_out;
    sort(pwr_out.pwrs.begin(),pwr_out.pwrs.end(),std::greater<T>());
    return pts;
}

#endif // RD_UTILITIES_SET_HELPER_HPP

