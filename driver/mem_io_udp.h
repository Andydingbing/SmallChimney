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

#ifndef RD_MEM_IO_UDP_H
#define RD_MEM_IO_UDP_H

#include "pci_dev.h"

class API mem_io_udp : public mem_io
{
public:
    mem_io_udp() {}
    virtual ~mem_io_udp() {}

public:
    int32_t allocate(uint32_t size);
    int32_t release();

    int32_t w8(uint8_t *buf,uint32_t size);
    int32_t w16(uint16_t *buf,uint32_t size);
    int32_t w32(uint32_t *buf,uint32_t size);
    int32_t r8(uint8_t *buf,uint32_t size);
    int32_t r16(uint16_t *buf,uint32_t size);
    int32_t r32(uint32_t *buf,uint32_t size);
};

#endif // RD_MEM_IO_UDP_H
