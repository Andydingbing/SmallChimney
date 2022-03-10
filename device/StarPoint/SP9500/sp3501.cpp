#include "sp3501.h"
#include "../chip/reg_def_sp9500.h"
#include "gen_ini_file.hpp"
#include <string>
#include "algo_chip.hpp"
#include "../cal_table.h"
#include "sleep_common.h"

using namespace ns_starpoint;
using namespace ns_sp9500;

sp3501::sp3501()
{
    _s6 = new pci_dev_vi("s6");

    for (uint32_t i = 0;i < ARRAY_SIZE(_fan_group_speed);++i) {
        _fan_group_speed[i] = 0;
    }
}

sp3501::~sp3501()
{
    SAFE_DEL(_s6);
}

sp3501 &sp3501::instance()
{
    static sp3501 g_sp3501;
    return g_sp3501;
}

int32_t sp3501::get_s6_ver(uint32_t &ver)
{
	return 0;
}

int32_t sp3501::open_board()
{
    char str_product_form[64];
    char str_rsrc_section[64];
    char str_dev_key[64];
    char str_rsrc_name[32];
    gen_ini_file ini_file(CONFIG_FILE_PATH);

    memset(str_product_form,0,ARRAY_SIZE(str_product_form));
    memset(str_rsrc_section,0,ARRAY_SIZE(str_rsrc_section));
    memset(str_dev_key,0,ARRAY_SIZE(str_dev_key));
    memset(str_rsrc_name,0,ARRAY_SIZE(str_rsrc_name));
    ini_file.get_config_str_value("Product Form","Form",str_product_form);
    strcpy(str_rsrc_section,"RESOURCE");
    strcat(str_rsrc_section,str_product_form);

    ini_file.get_config_str_value(str_rsrc_section,"CXU_S6",str_rsrc_name);

    INT_CHECK(_s6->init(str_rsrc_name,0x10ee,0x3501));

    for (uint32_t i = 0;i < ARRAY_SIZE(_fan_group_speed);++i) {
        _fan_group_speed[i] = 0;
    }

    INT_CHECK(set_fan_group(0,120));
    INT_CHECK(set_fan_group(2,120));
    INT_CHECK(set_fan_group(1,120));
    INT_CHECK(set_fan_group(3,80));
	return 0;
}

int32_t sp3501::close_board()
{
	return 0;
}

int32_t sp3501::vol_9119(uint16_t val)
{
    CXU_S6_REG_DECLARE(0x000e);
    CXU_S6_REG_DECLARE(0x000f);
    CXU_S6_REG(0x000f).sign = 0;
    CXU_S6_REG(0x000f).val = val;
    CXU_S6_W(0x000f);
    CXU_S6_OP(0x000e);
	return 0;
}

int32_t sp3501::vol_9119(double vol)
{
    return vol_9119(ns_x9119::voltage_to_tap(vol));
}

int32_t sp3501::set_fan(uint32_t idx,uint32_t speed)
{
    CXU_S6_REG_DECLARE(0x0012);
    CXU_S6_REG_DECLARE(0x0013);

    fan_map_t blower_map[g_max_fan] = {
        fan_map_t(0x0012,0x0032,0),
        fan_map_t(0x0012,0x0032,1),
        fan_map_t(0x0012,0x0033,0),
        fan_map_t(0x0012,0x0033,1),
        fan_map_t(0x0012,0x0034,0),
        fan_map_t(0x0012,0x0034,1),
        fan_map_t(0x0013,0x0032,0),
        fan_map_t(0x0013,0x0032,1),
        fan_map_t(0x0013,0x0033,0),
        fan_map_t(0x0013,0x0033,1),
        fan_map_t(0x0013,0x0034,0),
        fan_map_t(0x0013,0x0034,1)
    };
    if (idx < 6) {
        CXU_S6_REG(0x0012).speed = speed;
        CXU_S6_REG(0x0012).addr = blower_map[idx]._addr;
        CXU_S6_REG(0x0012).blower = blower_map[idx]._fan_sel;
        CXU_S6_OP(0x0012);
    } else {
        CXU_S6_REG(0x0013).speed = speed;
        CXU_S6_REG(0x0013).addr = blower_map[idx]._addr;
        CXU_S6_REG(0x0013).blower = blower_map[idx]._fan_sel;
        CXU_S6_OP(0x0013);
    }
    sleep_ms(10);
    return 0;
}

int32_t sp3501::set_fan(uint32_t speed)
{
    for (uint32_t i = 0;i < g_max_fan;i ++) {
        INT_CHECK(set_fan(i,speed));
    }
	return 0;
}

int32_t sp3501::set_fan_group(uint32_t idx,uint32_t speed)
{
    if (_fan_group_speed[idx] == speed) {
        return 0;
    }

    if (idx == 0) {
        INT_CHECK(set_fan(2, speed));
        INT_CHECK(set_fan(3, speed));
        INT_CHECK(set_fan(6, speed));
    } else if (idx == 1) {
        INT_CHECK(set_fan(1, speed));
        INT_CHECK(set_fan(4, speed));
        INT_CHECK(set_fan(5, speed));
    } else if (idx == 2)  {
        INT_CHECK(set_fan(8, speed));
        INT_CHECK(set_fan(9, speed));
        INT_CHECK(set_fan(12,speed));
    } else if (idx == 3)  {
        INT_CHECK(set_fan(7, speed));
        INT_CHECK(set_fan(10,speed));
        INT_CHECK(set_fan(11,speed));
    }

    _fan_group_speed[idx] = speed;
    return 0;
}
