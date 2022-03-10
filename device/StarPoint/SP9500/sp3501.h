#ifndef SP3501_H
#define SP3501_H

#define SP3501 sp3501::instance()

#include "global_header.h"
#include "liblog.h"
#include "libdriver.h"

namespace ns_starpoint {
namespace ns_sp9500 {

class API sp3501 : boost::noncopyable
{
public:
    struct fan_map_t {
        uint32_t _fpga_addr;
        uint32_t _addr;
        uint32_t _fan_sel;

        fan_map_t(uint32_t fpga_addr,uint32_t addr,uint32_t fan_sel) :
            _fpga_addr(fpga_addr),_addr(addr),_fan_sel(fan_sel) {}
    };

public:
    sp3501();
    virtual ~sp3501();
    static sp3501 &instance();

public:
    int32_t get_s6_ver(uint32_t &ver);
    int32_t open_board();
    int32_t close_board();

    int32_t vol_9119(uint16_t val);
    int32_t vol_9119(double vol);

    int32_t set_fan(uint32_t idx,uint32_t speed);
    int32_t set_fan(uint32_t idx,int32_t speed) { return set_fan(idx,uint32_t(speed)); }

    int32_t set_fan(uint32_t speed);
    int32_t set_fan(int32_t speed) { return set_fan(uint32_t(speed)); }

    /*
     * Left View:    Right View :
     *
     * ---------     -----------
     * | 4 | 2 |     | 8  | 10 |
     * ---------     -----------
     * | 1 | 6 |     | 12 | 7  |
     * ---------     -----------
     * | 5 | 3 |     | 9  | 11 |
     * ---------     -----------
     *
     * Group 0 : 2,  6,  3
     * Group 1 : 4,  1,  5
     * Group 2 : 8,  12, 9
     * Group 3 : 10, 7,  11
     */
    int32_t set_fan_group(uint32_t idx,uint32_t speed);

public:
    pci_dev_vi *_s6;
    uint32_t _fan_group_speed[4];
};

} // namespace ns_starpoint
} // namespace ns_sp9500

#endif
