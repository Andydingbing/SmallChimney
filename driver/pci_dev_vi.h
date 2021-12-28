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

#ifndef BD_PCI_DEV_VI_H
#define BD_PCI_DEV_VI_H

#include "pci_dev.h"

class API pci_dev_vi : public pci_dev
{
public:
    pci_dev_vi();
    pci_dev_vi(const std::string &descriptor);
    ~pci_dev_vi();

public:
    int32_t get_devs(std::vector<std::string> &devs);
    int32_t init(const std::string &dev);
    int32_t init(const char *dev);
    int32_t init(const std::string &dev,uint32_t vid,uint32_t did);
    int32_t init(const char *dev,uint32_t vid,uint32_t did);

    SYM_INLINE int32_t w8(addr_space_t addr,uint32_t offset,uint8_t data);
    SYM_INLINE int32_t w16(addr_space_t addr,uint32_t offset,uint16_t data);
    SYM_INLINE int32_t w32(addr_space_t addr,uint32_t offset,uint32_t data);
    SYM_INLINE int32_t w32(addr_space_t addr,uint32_t offset,uint32_t len,uint32_t *buf);
    SYM_INLINE int32_t r8(addr_space_t addr,uint32_t offset,uint8_t *data);
    SYM_INLINE int32_t r16(addr_space_t addr,uint32_t offset,uint16_t *data);
    SYM_INLINE int32_t r32(addr_space_t addr,uint32_t offset,uint32_t *data);

    // low level access
    SYM_INLINE int32_t lla_w8(uint32_t offset,uint8_t data);
    SYM_INLINE int32_t lla_w16(uint32_t offset,uint16_t data);
    SYM_INLINE int32_t lla_w32(uint32_t offset,uint32_t data);
    SYM_INLINE int32_t lla_r8(uint32_t offset,uint8_t *data);
    SYM_INLINE int32_t lla_r16(uint32_t offset,uint16_t *data);
    SYM_INLINE int32_t lla_r32(uint32_t offset,uint32_t *data);

public:
    static int32_t open_default_rm();
    static unsigned long get_default_rm();

private:
    unsigned long _session;
    void *_map_addr;
};

#endif // BD_PCI_DEV_VI_H
