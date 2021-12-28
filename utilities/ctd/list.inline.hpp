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

#ifndef CTD_LIST_INLINE_HPP
#define CTD_LIST_INLINE_HPP

#include "list.hpp"

using namespace ctd;

template<typename ptr_t>
dlist<ptr_t>::dlist()
{
    _fst = _cur = _lst = nullptr;
    _size = 0;
    _idx_mid_l = 0;
    _idx_mid_r = 0;
    _idx_cur   = 0;
}

template<typename ptr_t>
dlist<ptr_t>::~dlist()
{
    dlist_node<ptr_t> *node = _fst;
    dlist_node<ptr_t> *next = nullptr;

    for (size_t i = 0;i < _size;++i) {
        next = node->_next;
        delete node->_data;
        delete node;
        node = next;
    }
}

template<typename ptr_t>
template<typename int_t,typename traits_t>
ptr_t dlist<ptr_t>::at(const int_t idx)
{
    size_t i = 0;
    dlist_node<ptr_t> *searching;

    if (idx < _idx_mid_l) {
        searching = _fst;
        while (i < idx) {
            searching = searching->_next;
            ++i;
        }
    } else if (idx > _idx_mid_r) {
        searching = _lst;
        while (i < _size - idx - 1) {
            searching = searching->_prev;
            ++i;
        }
    } else {
        searching = _cur;
        if (idx > _idx_cur) {
            while (i < idx - _idx_cur) {
                searching = searching->_next;
                ++i;
            }
        } else {
            while (i < _idx_cur - idx) {
                searching = searching->_prev;
                ++i;
            }
        }
    }

    _cur = searching;
    update_helpers(idx);
    return searching->_data;
}

template<typename ptr_t>
void dlist<ptr_t>::push_back(const ptr_t data)
{
    dlist_node<ptr_t> *node = new dlist_node<ptr_t>;
    node->_data = const_cast<ptr_t>(data);

    if (_fst == nullptr) {
        _fst = node;
        _fst->_prev = nullptr;
        _fst->_next = nullptr;
    } else {
        _lst->_next = node;
        node->_prev = _lst;
        node->_next = nullptr;
    }

    _cur = _lst = node;
    update_helpers(_size);
    ++_size;
}

#endif
