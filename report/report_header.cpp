#include "report_header.h"

using namespace std;

report_header_t::report_header_t()
{
    memset(this,0,sizeof(report_header_t));
}

item_header_t::item_header_t() :
    id(-1),pts(0),per_size(0),rsv0(0)
{
    memset(freq_str,0,sizeof(freq_str));
}

int32_t item_header_t::set_freq_str(const string &str)
{
    size_t length = str.length();

    if (length > ARRAY_SIZE(freq_str) - 1) {
        return -1;
    }

    memset(freq_str,0,sizeof(freq_str));
    str.copy(freq_str,str.length());
    freq_str[length] = '\0';
    return 0;
}
