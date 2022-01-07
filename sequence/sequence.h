#ifndef SEQUENCE_SEQUENCE_H
#define SEQUENCE_SEQUENCE_H

#include <string>
#include <list>
#include "preprocessor/preprocessor.h"

class API sequence
{
public:
    sequence(const std::string &path);

    std::string err() const { return _err; }
    size_t err_line() const { return _err_line; }

    int32_t parse_vendor();
    int32_t parse_product();
    int32_t parse_tree_node(char **str);
    int32_t parse();

private:
    uint32_t tree_node_floor();
    int32_t read_one_line(char **str = nullptr,const bool trim_front = true,const bool trim_back = true,const bool err_if_empty = false);

public:
    std::string vendor;
    std::list<std::string> product;
    std::list<std::list<std::string>> tree;

private:
    std::string _path;
    FILE *_fp;

    char _buf[256];

private:
    bool _continue_when_err;
    size_t _cur_line;
    size_t _err_line;
    std::string _err;

};

#endif
