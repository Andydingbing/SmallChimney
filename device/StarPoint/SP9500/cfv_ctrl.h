#ifndef CFV_CTRL_H
#define CFV_CTRL_H
#include "stdint_common.h"

/*all function names consist of "ForConv..." means converting the calibration file to the next version
  while all names consist of "BckConv..." means converting the file to the previous version*/
/*all backward converting functions just return zero at the version right now
  so note that only support convert to the next version now!!!,remember to backup the file!!!*/

namespace rd {

class cal_file;
class cal_file_r1ab;
class cal_file_r1cd;

class cfv_ctrl
{
public:
    cfv_ctrl(){}
    virtual ~cfv_ctrl(){}
public:
    void target(cal_file *file);
public:
    int32_t for_0000_0001();
    int32_t for_0001_0002();
    int32_t for_0002_0003();
    int32_t for_0003_0004();
    int32_t for_0004_0005();
    int32_t for_0005_0006();
    int32_t for_0006_0007();
    int32_t for_c(int32_t cur_ver);
private:
    cal_file *m_file;
};

} //namespace rd

#endif // CFV_CTRL_H
