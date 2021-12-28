#ifndef RD_COMMON_CAL_FILE_MEMORY_H
#define RD_COMMON_CAL_FILE_MEMORY_H

#include "global_header.h"

#define convert_buf convert_buf_t::instance()

class API convert_buf_t : noncopyable
{
public:
    void     *buf;
    char     sn[64];
    int32_t  table;
    uint32_t size;
    uint32_t size_table;

    convert_buf_t() : buf(nullptr),table(-1),size(0),size_table(0) { ZERO_ARRAY(sn); }
    ~convert_buf_t() { if (size > 0) { delete []static_cast<char *>(buf); } }
    static convert_buf_t &instance();

    void allocate(uint32_t new_size);
};

#endif
