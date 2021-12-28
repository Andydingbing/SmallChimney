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

#ifndef RD_UTILITIES_ALGO_SORT_H
#define RD_UTILITIES_ALGO_SORT_H

#include "rd.h"
#include "exception.hpp"

// bubble sort,1 dimension,ascend mode
template<typename T>
void sort_bubble(std::vector<T> &x)
{
    size_t n = x.size();
    T x_min = x[0];

    for (size_t i = 0;i < n - 1;++i) {
        x_min = x[i];
        for (size_t j = i;j < n;++j) {
            if (x[j] < x_min) {
                x_min = x[j];
                x[j] = x[i];
                x[i] = x_min;
            }
        }
    }
}

// bubble sort,2 dimension,both x,y ascend mode
template<typename T_x,typename T_y>
void sort_bubble(std::vector<T_x> &x,std::vector<T_y> &y)
{
    RD_ASSERT_THROW(x.size() == y.size());

    size_t n = x.size();
    T_x x_min = x[0];
    T_y y_temp = T_y(0.0);

    for (size_t i = 0;i < n - 1;++i) {
        x_min = x[i];
        for (size_t j = i + 1;j < n;++j) {
            if (x[j] < x_min) {
                x_min = x[j];
                x[j] = x[i];
                x[i] = x_min;
                y_temp = y[j];
                y[j] = y[i];
                y[i] = y_temp;
            }
        }
    }

    size_t num_y_dup = 0;

    for (size_t i = 0;i < n - 1;++i) {
        if (x[i + 1] != x[i]) {
            continue;
        } else {
            num_y_dup = 1;
            while (x[i + num_y_dup + 1] == x[i + num_y_dup]) {
                num_y_dup ++;
            }
            num_y_dup ++;
            // sort_bubble(y + i,num_y_dup);
            i += num_y_dup - 1;
        }
    }
}


#endif // RD_UTILITIES_ALGO_SORT_H
