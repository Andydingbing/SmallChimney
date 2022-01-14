#ifndef SEQUENCE_SEQUENCE_H
#define SEQUENCE_SEQUENCE_H

#include <string>
#include <list>
#include "preprocessor/preprocessor.h"

struct config_node {
    uint32_t floor;
    std::string str;
    std::list<config_node *> sub_node;
};

class API sequence
{
public:
    enum syntax_t {
        Vendor,
        Product,
        Include,
        Tree_Node,
        Config_Node_Header,
        Config_Node_Config,
        Config_Other_Header,
        Config_Other_Config,
        Annotation,
        Empty,
        Error
    };

    struct line_t {
        line_t(const syntax_t s,void *d = nullptr) : syntax(s),data(d) {}
        syntax_t syntax;
        void *data;
    };

public:
    sequence(const std::string &path);

    std::string err() const { return _err; }
    size_t err_line() const { return _err_line; }

    int32_t parse_vendor();
    int32_t parse_product();
    int32_t parse_tree_node(char **str);
    int32_t parse();

private:
    void add_line(const syntax_t s,void *d = nullptr)
    { lines.push_back(line_t(s,d)); }

    uint32_t tree_node_floor();
    int32_t read_one_line(char **str = nullptr,const bool trim_front = true,const bool trim_back = true,const bool err_if_empty = false);
    syntax_t syntax(char *str);
    int32_t syntax_config(char *str);
    int32_t syntax_config_header(char *str);

public:
    std::list<line_t> lines;
    std::string vendor;
    std::list<std::string> product;
    std::list<sequence> sub_sequence;
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
