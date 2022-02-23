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

#ifndef CTD_LIST_HPP
#define CTD_LIST_HPP

#include "global_header.h"
#include "stddef.h"
#include "traits.hpp"

namespace ctd {

// double linked list node
template<typename ptr_t>
class dlist_node {
public:
    ptr_t _data;
    dlist_node *_prev;
    dlist_node *_next;
};

/*
 * node                       node              node
 *  |                          |                 |
 *  v                          v                 v
 *  +------------*-------------+--------*--------+
 *  ^            ^             ^        ^        ^
 *  |            |             |        |        |
 * first         |          current     |       last
 *               |                      |
 *            mid_left               mid_right
 *
 */

template<typename ptr_t>
class dlist
{
public:
    dlist();
    ~dlist();

    size_t size() const { return _size; }

    template<typename int_t,typename traits_t = traits_int<int_t>>
    ptr_t at(const int_t idx);

    void push_back(const ptr_t data);

private:
    SYM_INLINE void update_helpers(const size_t idx_cur)
    {
        _idx_cur = idx_cur;
        _idx_mid_l = _idx_cur >> 1;
        _idx_mid_r = (_size << 1) - _idx_cur;
    }

private:
    dlist_node<ptr_t> *_fst;
    dlist_node<ptr_t> *_cur;
    dlist_node<ptr_t> *_lst;
    size_t _size;
    size_t _idx_mid_l;
    size_t _idx_mid_r;
    size_t _idx_cur;
};

} // namespace ctd

#include "list.inline.hpp"

#endif
