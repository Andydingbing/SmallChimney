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

#ifndef RD_UTILITIES_ALGO_CHIP_HPP
#define RD_UTILITIES_ALGO_CHIP_HPP

#include "algo_chip_adc.hpp"
#include "algo_chip_dac.hpp"
#include "algo_chip_pll.hpp"
#include "algo_chip_att.hpp"
#include "algo_chip_temp_sensor.hpp"

// MICROCHIP MCP3208
namespace ns_mcp3208 {

SYM_INLINE double ad_to_voltage_mv(int16_t ad)
{ return ad * 5000.0 / 4096.0; }

} // namespace ns_mcp3208


// intersil X9119
// 1024 resistor taps - 10-bit resolution
namespace ns_x9119 {

SYM_INLINE uint16_t voltage_to_tap(double vol_v)
{ return uint16_t(vol_v / 5.0 * 1023.0); }

SYM_INLINE double tap_to_voltage_v(uint16_t tap)
{ return 5.0 * tap / 1023.0; }

} // namespace ns_x9119

#endif // RD_UTILITIES_ALGO_CHIP_HPP
