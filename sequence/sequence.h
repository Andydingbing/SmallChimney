#ifndef SEQUENCE_SEQUENCE_H
#define SEQUENCE_SEQUENCE_H

#include <string>
#include <vector>
#include <list>
#include "preprocessor/preprocessor.h"

struct config_node_t
{
    uint32_t floor;
    std::string str;
    std::string config;
    std::list<config_node_t> sub_node;
};

struct config_root_node_t : config_node_t
{
    size_t tree_line;
};

class API sequence
{
public:
    enum syntax_t {
        Plugin,
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
    sequence();
    sequence(const std::string &path);
    ~sequence();

    std::string path() const { return _path; }
    std::string err() const;
    size_t err_line() const { return _err_line; }

    void set_path(const std::string &path);
    void set_tree(std::list<std::list<std::string>> *t)
    { tree_builtin = t; }

    int32_t compile();
    int32_t parse_plugin();
    int32_t parse_vendor();
    int32_t parse_product();

    void resort(const std::list<std::list<std::string>> &t,std::list<std::list<std::string *>> &sorted_tree);

private:
    void init();
    void add_line(const syntax_t s,void *d = nullptr)
    { lines.push_back(line_t(s,d)); }

    uint32_t tree_node_floor();
    int32_t read_one_line(char **str = nullptr,const bool trim_front = true,const bool trim_back = true,const bool err_if_empty = false);
    int32_t syntax(char *str);

    int32_t syntax_include(char *str);

    int32_t syntax_tree(char *str);
    int32_t syntax_tree_header(char *str);
    int32_t syntax_tree_tree(char *str,const uint32_t floor);

    int32_t syntax_config(char *str);
    int32_t syntax_config_header(char *str);
    int32_t syntax_config_config(char *str,const uint32_t floor);

    void last_config_node(config_node_t **node,config_node_t **parent);

    uint32_t trim_front(char **ptr);
    uint32_t trim_front(char *ptr);
    uint32_t trim_front(std::string &str);
    uint32_t trim_back(char *ptr);
    uint32_t trim_back(std::string &str);

public:
    void print_plugin();
    void print_vendor();
    void print_product();
    void print_include();
    void print_tree();
    void print_config();

public:
    std::list<line_t> lines;
    std::string plugin;
    std::string vendor;
    std::list<std::string> product;
    std::list<sequence> sub_sequence;
    std::list<std::list<std::string>> tree;
    std::list<config_root_node_t> config_root_node;

private:
    std::string _path;
    FILE *_fp;

    char _buf[256];

    std::list<std::list<std::string>> *tree_builtin;

private:
    bool _continue_when_err;
    size_t _cur_line;
    size_t _err_line;
    std::string _err;
};

#endif
