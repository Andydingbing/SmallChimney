#include "cal_file.h"
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "sp1401.h"

using namespace std;
using namespace ns_sp9500;
using namespace ns_sp1401;
using namespace ns_sp2401;

cal_file::cal_file(hw_ver_t ver, uint32_t rf_idx, uint32_t rfu_idx)
{
    m_hw_ver = ver;
    m_rf_idx = rf_idx;
    m_rfu_idx = rfu_idx;
    m_ordinal = 0;
    m_bw = _160M;
#ifdef RD_C_MSC
	_set_fmode(_O_BINARY);
#endif
}



