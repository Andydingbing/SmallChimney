#include "cal_file.h"

using namespace std;
using namespace ns_ericsson;
using namespace ns_radio_4415;

string cal_file::path() const
{
#ifdef PLATFORM_WIN32
    return "C:\\RTS\\Ericsson\\Radio_4415\\";
#elif defined RD_PLAT_FORM_LINUX
    return "";
#endif
}
