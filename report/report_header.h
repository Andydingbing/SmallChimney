#ifndef REPORT_HEADER_H
#define REPORT_HEADER_H

#include "global_header.h"

struct API report_header_t {
    // Encryption
    uint64_t encryption;
    uint32_t hash[256];

    uint32_t ver;
    uint32_t ver_driver;
    uint32_t ver_tool;
    uint32_t rsv0;
    char     sn[64];
    uint32_t tables;
    uint32_t rsv1;
    uint32_t rsv2[4];

    report_header_t();
};

struct API item_header_t {
    int32_t  id;       // <cal_table_t>
    uint32_t pts;      // total points
    uint32_t per_size; // size(byte) of each <data_f_t> type point
    uint32_t rsv0;
    char freq_str[512];

    item_header_t();
    int32_t set_freq_str(const std::string &str);
};

#endif
