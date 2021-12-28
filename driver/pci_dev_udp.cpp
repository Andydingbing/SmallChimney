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

#include "pci_dev_udp.h"

using namespace std;

pci_dev_udp::pci_dev_udp() :
    pci_dev()
{
    _descriptor = "";
}

pci_dev_udp::pci_dev_udp(const string &descriptor) :
    pci_dev()
{
    _descriptor = descriptor;
}

int32_t pci_dev_udp::get_devs(vector<string> &devs)
{
    return 0;
}

int32_t pci_dev_udp::init(const string &dev)
{
    return 0;
}

int32_t pci_dev_udp::init(const char *dev)
{
    return 0;
}

int32_t pci_dev_udp::init(const string &dev,uint32_t vid,uint32_t did)
{
    return 0;
}

int32_t pci_dev_udp::init(const char *dev,uint32_t vid,uint32_t did)
{
    return 0;
}

int32_t pci_dev_udp::init(const string &ip,uint32_t port)
{
    return 0;
}

int32_t pci_dev_udp::w8(addr_space_t addr,uint32_t offset,uint8_t data)
{
    return 0;
}

int32_t pci_dev_udp::w16(addr_space_t addr,uint32_t offset,uint16_t data)
{
    return 0;
}

int32_t pci_dev_udp::w32(addr_space_t addr,uint32_t offset,uint32_t data)
{
    return 0;
}

int32_t pci_dev_udp::w32(addr_space_t addr,uint32_t offset,uint32_t len,uint32_t *buf)
{
    return 0;
}

int32_t pci_dev_udp::r8(addr_space_t addr,uint32_t offset,uint8_t *data)
{
    return 0;
}

int32_t pci_dev_udp::r16(addr_space_t addr,uint32_t offset,uint16_t *data)
{
    return 0;
}

int32_t pci_dev_udp::r32(addr_space_t addr,uint32_t offset,uint32_t *data)
{
    return 0;
}
