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

#ifndef UTILITIES_RANGE_HPP
#define UTILITIES_RANGE_HPP

#include "exception.hpp"
#include <vector>
#include <boost/format.hpp>
#include "floating_point_numbers.hpp"

template <typename low_type,typename high_type>
class range
{
public:
    range() : _low(0), _high(0) {}
    range(const low_type low,const high_type high) :
        _low(low), _high(high) {}

    low_type _low;
    high_type _high;
};


#endif // UTILITIES_RANGE_HPP

