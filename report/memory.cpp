#include "memory.h"

using namespace std;

convert_buf_t &convert_buf_t::instance()
{
    static convert_buf_t g_convert_buf;
    return g_convert_buf;
}

void convert_buf_t::allocate(uint32_t new_size)
{
    if (size == 0) {
        buf = new char[new_size / sizeof(char)];
    } else if (size < new_size) {
        delete []static_cast<char *>(buf);
        buf = new char[new_size / sizeof(char)];
    }
    size = size_table = new_size;
}
