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

#ifndef BD_PCI_DEV_H
#define BD_PCI_DEV_H

#include "global_header.h"
#include <vector>
#include <string>

class API pci_dev : noncopyable
{
public:
    enum addr_space_t {
        AS_LOCAL = 0,
        AS_A16 = 1,
        AS_A24 = 2,
        AS_A32 = 3,
        AS_A64 = 4,
        AS_ALLOC = 9,
        AS_CFG = 10,    // PCI/PCIe Config Space
        AS_BAR0 = 11,   // PCI/PCIe BAR0 Address Space
        AS_BAR1 = 12,   // PCI/PCIe BAR1 Address Space
        AS_BAR2 = 13,   // PCI/PCIe BAR2 Address Space
        AS_BAR3 = 14,   // PCI/PCIe BAR3 Address Space
        AS_BAR4 = 15,   // PCI/PCIe BAR4 Address Space
        AS_BAR5 = 16,   // PCI/PCIe BAR5 Address Space
        AS_OPAQUE = 0xFFFF
    };

public:
    virtual int32_t get_devs(std::vector<std::string> &devs) = 0;

    virtual int32_t init(const std::string &dev) = 0;
    virtual int32_t init(const std::string &dev, uint32_t vid,uint32_t did) = 0;
    virtual void set_descriptor(const std::string &des) { _descriptor = des; }
    virtual std::string descriptor() const { return _descriptor; }

public:
    virtual int32_t w8(addr_space_t addr,uint32_t offset,uint8_t data) = 0;
    virtual int32_t w16(addr_space_t addr,uint32_t offset,uint16_t data) = 0;
    virtual int32_t w32(addr_space_t addr,uint32_t offset,uint32_t data) = 0;
    virtual int32_t w32(addr_space_t addr,uint32_t offset,uint32_t len,uint32_t *buf) = 0;

    virtual int32_t r8(addr_space_t addr,uint32_t offset,uint8_t *data) = 0;
    virtual int32_t r16(addr_space_t addr,uint32_t offset,uint16_t *data) = 0;
    virtual int32_t r32(addr_space_t addr,uint32_t offset,uint32_t *data) = 0;

public:
    virtual ~pci_dev(void) {}

protected:
    std::string _descriptor;
};


class API mem_io : public noncopyable
{
public:
    mem_io() { _addr = nullptr; }
    virtual ~mem_io() {}

public:
    virtual int32_t allocate(uint32_t size) = 0;
    virtual int32_t release() = 0;
    void* addr() { return _addr; }

public:
    virtual int32_t w8(uint8_t *buf,uint32_t size) = 0;
    virtual int32_t w16(uint16_t *buf,uint32_t size) = 0;
    virtual int32_t w32(uint32_t *buf,uint32_t size) = 0;
    virtual int32_t r8(uint8_t *buf,uint32_t size) = 0;
    virtual int32_t r16(uint16_t *buf,uint32_t size) = 0;
    virtual int32_t r32(uint32_t *buf,uint32_t size) = 0;

protected:
    void* _addr; // The virtual/physical address after align
};

#endif // BD_PCI_DEV_H
