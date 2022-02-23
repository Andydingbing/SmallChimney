#include "cfv_ctrl.h"
#include "cfv_ctrl_0000_0001.hpp"
#include "cfv_ctrl_0001_0002.hpp"
#include "cfv_ctrl_0002_0003.hpp"
#include "cfv_ctrl_0003_0004.hpp"
#include "cfv_ctrl_0004_0005.hpp"
#include "cfv_ctrl_0005_0006.hpp"
#include "cfv_ctrl_0006_0007.hpp"

using namespace rd;

typedef int32_t (cfv_ctrl::*for_conv)();
typedef int32_t (cfv_ctrl::*bck_conv)();

void cfv_ctrl::target(cal_file *file)
{
    m_file = file;
}

int32_t cfv_ctrl::for_0000_0001()
{
    return ::for_0000_0001(m_file);
}

int32_t cfv_ctrl::for_0001_0002()
{
    return ::for_0001_0002(m_file);
}

int32_t cfv_ctrl::for_0002_0003()
{
    return ::for_0002_0003(m_file);
}

int32_t cfv_ctrl::for_0003_0004()
{
    return ::for_0003_0004(m_file);
}

int32_t cfv_ctrl::for_0004_0005()
{
    return ::for_0004_0005(m_file);
}

int32_t cfv_ctrl::for_0005_0006()
{
    return ::for_0005_0006(m_file);
}

int32_t cfv_ctrl::for_0006_0007()
{
    return ::for_0006_0007(m_file);
}

int32_t cfv_ctrl::for_c(int32_t cur_ver)
{
    for_conv for_c[] = {
        &cfv_ctrl::for_0000_0001,
        &cfv_ctrl::for_0001_0002,
        &cfv_ctrl::for_0002_0003,
        &cfv_ctrl::for_0003_0004,
        &cfv_ctrl::for_0004_0005,
        &cfv_ctrl::for_0005_0006,
        &cfv_ctrl::for_0006_0007
    };

    for (int32_t i = cur_ver;i < CAL_FILE_VER;i ++) {
        (this->*for_c[i])();
	}
	return 0;
}
