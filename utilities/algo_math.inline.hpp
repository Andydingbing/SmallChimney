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

#include "algo_math.hpp"
#include "exception.hpp"

double round(double x,uint32_t digit)
{
    uint64_t divider = 1;
    for (uint32_t i = 0;i < digit;i ++) {
        divider *= 10;
    }
    double a = x * divider;
    int64_t b = int64_t(a + (a >= 0 ? 0.5 : -0.5));

    return double(b) / divider;
}

float inv_sqrt(float x)
{
    float x_half = 0.5f * x;
    int i = *(int *)&x;
    i = 0x5f375a86 - (i >> 1);
    x = *(float *)&i;
    x = x * (1.5f - x_half * x * x); // newton iteration
    return x;
}
