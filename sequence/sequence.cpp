#include "sequence.h"
#include "preprocessor/prefix.h"
#include "algo_math.hpp"
#include "string_util.hpp"
#include <fmt/core.h>
#include <iostream>
#include <vector>
#include <cstdarg>

#define COMPILER_ERR(err) \
{   _err = (err); \
    _err_line = _cur_line; \
    if (!_continue_when_err) { \
        return -1; \
    } \
}

#define ASSERT(func) \
{   int32_t ret = (func); \
    if (ret && !_continue_when_err) { \
        return ret; \
    } \
}

using namespace std;

void first_meaningful(char *str,char *ptr)
{
    ptr = str;

    while (*ptr == ' ' || *ptr == '\t') {
        ptr ++;
    }
}

void first_meaningful(char *str,char **ptr,list<string> &args,list<uint32_t> &num)
{
    *ptr = str;

    list<string>::const_iterator iter_args;
    list<uint32_t>::iterator iter_num;

    for (size_t i = 0;i < args.size();++i) {
        num.push_back(0);
    }

    iter_args = args.cbegin();
    iter_num = num.begin();

    while (1) {
        for (;iter_args != args.cend();++iter_args,++iter_num) {
            if (strlen(*ptr) >= iter_args->length() && strncmp(*ptr,iter_args->c_str(),iter_args->length()) == 0) {
                (*iter_num) ++;
                *ptr += iter_args->length();
                break;
            }
        }
        if (iter_args == args.cend()) {
            break;
        }
    }
}

char last_meaningful(char *str)
{
    char *ptr = str + strlen(str) - 1;

    while (*ptr == '\n' || *ptr == ' ' || *ptr == '\t') {
        ptr --;
    }
    return *ptr;
}

bool is_empty_line(char *str)
{
    for (;*str != '\0';str ++) {
        if (*str != ' ' && *str != '\t' && *str != '\n') {
            return false;
        }
    }
    return true;
}

sequence::sequence(const std::string &path)
{
    _path = path;
    _fp = fopen(path.c_str(),"r");

    ZERO_ARRAY(_buf);

    _continue_when_err = false;
    _cur_line = 0;
    _err_line = 0;
}

sequence::~sequence()
{
    if (_fp != nullptr) {
        fclose(_fp);
        _fp = nullptr;
    }
}

string sequence::err() const
{
    return _err.size() ? fmt::format("Line {:d}:",_err_line) + _err : "";
}

int32_t sequence::compile()
{
    ASSERT(parse_vendor());
    ASSERT(parse_product());

    char *str = _buf;

    while (!read_one_line(&str,false,true)) {
        ASSERT(syntax(str));
    }

    return 0;
}

int32_t sequence::parse_vendor()
{
    char *str = _buf;
    const char header_vendor[] = "Vendor";

    fseek(_fp,0,SEEK_SET);
    ASSERT(read_one_line(&str));

    if (strncmp(str,header_vendor,strlen(header_vendor))) {
        COMPILER_ERR("Must start with \"Vendor\".");
    }

    str += strlen(header_vendor);

    trim_front(&str);

    if (*str != ':') {
        COMPILER_ERR("Missing \":\" after \"Vendor\".");
    }

    str += 1;
    trim_front(&str);
    trim_back(str);
    vendor = str;
    add_line(Vendor,&vendor);
    return 0;
}

int32_t sequence::parse_product()
{
    char *str = _buf;
    char *left = nullptr;
    char *right = nullptr;
    char each_product[512] = {0};
    const char header_product[] = "Product";

    if (read_one_line(&str,true,true,true) || strlen(str) == 0) {
        COMPILER_ERR("Missing Product info.");
    }

    if (strncmp(str,header_product,strlen(header_product))) {
        COMPILER_ERR("Must start with \"" + string(header_product) + "\".");
    }

    str += strlen(header_product);

    trim_front(&str);

    if (*str != ':') {
        COMPILER_ERR("Missing \":\" after \"" + string(header_product) + "\".");
    }

    str += 1;
    trim_front(&str);
    trim_back(str);

    left = str;
    while ((right = strchr(left,';')) != nullptr) {
        ZERO_ARRAY(each_product);
        strncpy(each_product,left,right - left);
        trim_back(each_product);
        product.push_back(each_product);
        left = right + 1;

        trim_front(&left);

        if (*left == ';') {
            COMPILER_ERR("Invalid string with continuously \";\".");
        }
    }

    product.push_back(left);
    add_line(Product,&product);
    return 0;
}

uint32_t sequence::tree_node_floor()
{
    char *str = _buf;
    uint32_t sum = 0;
    const char prefix_0[] = "    ";
    const char prefix_1[] = "\t";

    const size_t prefix_0_length = strlen(prefix_0);
    const size_t prefix_1_length = strlen(prefix_1);

    while (1) {
        if (strncmp(str,prefix_0,prefix_0_length) == 0) {
            str += prefix_0_length;
            sum ++;
            continue;
        }
        if (strncmp(str,prefix_1,prefix_1_length) == 0) {
            str += prefix_1_length;
            sum ++;
            continue;
        }
        break;
    }

    if (*str == ' ') {
        COMPILER_ERR("Invalid prefix,replace with 4 spaces or 1 tab instead.");
    }
    return sum;
}

int32_t sequence::read_one_line(char **str,const bool trim_front,const bool trim_back,const bool err_if_empty)
{
    if (_fp == nullptr) {
        COMPILER_ERR("Can not open " + _path);
    }

    ZERO_ARRAY(_buf);
    char *str_temp = _buf;
    bool err_lf = false;

    if (feof(_fp)) {
        return -1;
    }

    while (!feof(_fp)) {
        if (fgets(_buf,sizeof(_buf),_fp) == nullptr) {
            return -2;
        }

        _cur_line ++;

        if (trim_front) { this->trim_front(&str_temp); }
        if (trim_back)  { this->trim_back(str_temp);  }

        if (is_empty_line(str_temp)) {
            if (err_if_empty) {
                err_lf = true;
                add_line(Error);
                break;
            } else {
                add_line(Empty);
            }
        } else {
            break;
        }
    }

    if (str != nullptr && (*str) != nullptr) {
        (*str) = str_temp;
    }

    if (last_meaningful(str_temp) == ';') {
        COMPILER_ERR("Unknown expression with the end of \';\'.");
    }

    if (err_lf) {
        return -3;
    }

    return 0;
}

int32_t sequence::syntax(char *str)
{
    char *ptr = str;
    char keyword_include[] = "include";

    if (strchr(str,':')) {
        return syntax_config(str);
    }

    first_meaningful(str,ptr);

    if (strncmp(ptr,keyword_include,strlen(keyword_include)) == 0) {
        return syntax_include(str);
    }

    return syntax_tree(str);
}

int32_t sequence::syntax_include(char *str)
{
    const char keyword[] = "include";
    string sub_path;

    str += strlen(keyword);

    trim_front(&str);

    if (*str != '(') {
        COMPILER_ERR("Missing \'(\' after keyword " + string(keyword));
    }

    if (str[strlen(str) - 1] != ')') {
        COMPILER_ERR("Missing \')\' at the end of \"" + string(keyword) + ("\" statement."));
    }

    if (strlen(str) == 2) {
        COMPILER_ERR("Empty sub sequence path.");
    }

    sub_path.assign(str + 1,str + strlen(str) - 1);
    sub_sequence.push_back(sequence(sub_path));

    return 0;
}

int32_t sequence::syntax_tree(char *str)
{
    list<string> args = {"    ","\t"};
    list<uint32_t> num;
    uint32_t sum_args = 0;

    char *ptr = str;

    first_meaningful(str,&ptr,args,num);
    sum_args = sum(num);

    if (*str == ' ') {
        add_line(Error);
        COMPILER_ERR("Invalid " + string(sum_args ? "sub" : "root") + "tree.");
    }

    if (sum_args == 0) {
        syntax_tree_header(ptr);
    } else {
        syntax_tree_tree(ptr,sum_args);
    }

    return 0;
}

int32_t sequence::syntax_tree_header(char *str)
{
    char invalid_char[] = {',','.','`','~','!','?',';',':'};
    list<string> node;
    list<list<string>>::const_iterator iter_tree = tree.cbegin();

    trim_back(str);

    for (size_t i = 0;i < ARRAY_SIZE(invalid_char);++i) {
        if (strchr(str,invalid_char[i])) {
            COMPILER_ERR(fmt::format("Invalid char : \"{:c}\".",invalid_char[i]));
        }
    }

    for (;iter_tree != tree.cend();++iter_tree) {
        if (strcmp(str,iter_tree->front().c_str()) == 0) {
            COMPILER_ERR("Conflicted tree root node defination found.");
        }
    }

    node.push_back(str);
    tree.push_back(node);
    add_line(Tree_Node,&tree.back());
    return 0;
}

int32_t sequence::syntax_tree_tree(char *str,const uint32_t floor)
{
    list<line_t>::const_reverse_iterator iter_lines = lines.crbegin();
    list<string>::const_iterator iter_tree;

    for (;iter_lines != lines.crend();iter_lines ++) {
        if (iter_lines->syntax == Empty) { continue; }
        if (iter_lines->syntax == Tree_Node) { break; }

        COMPILER_ERR("Can not find the parent tree node.");
    }

    if (tree.back().size() - 1 < floor) {
        tree.back().push_back(str);
    } else {
        list<string> node;
        iter_tree = tree.back().cbegin();

        for (uint32_t i = 0;i < floor;++i,++iter_tree) {
            node.push_back(*iter_tree);
        }
        node.push_back(str);
        tree.push_back(node);
    }

    add_line(Tree_Node,&tree.back());
    return 0;
}

int32_t sequence::syntax_config(char *str)
{
    list<string> args = {"    ","\t"};
    list<uint32_t> num;
    uint32_t sum_args = 0;

    char *ptr = str;

    first_meaningful(str,&ptr,args,num);
    sum_args = sum(num);

    if (*str == ' ') {
        add_line(Error);
        COMPILER_ERR("Invalid " + string(sum_args ? "sub" : "root") + "config prefix.");
    }

    if (sum_args == 0) {
        syntax_config_header(ptr);
    } else {
        syntax_config_config(ptr,sum_args);
    }

    return 0;
}

int32_t sequence::syntax_config_header(char *str)
{
    config_root_node_t root_node;

    const list<string> seperator = {":"};
    list<string> config_nodes;
    list<list<string>>::const_iterator iter_tree = tree.cbegin();
    list<string>::const_iterator iter_each_tree;
    list<string>::iterator iter_config_nodes;

    uint32_t match_sum = 0;

    trim_back(str);
    split(str,seperator,config_nodes);

    trim_front(config_nodes.back());
    trim_back(config_nodes.back());

    if (config_nodes.back().empty()) {
        config_nodes.pop_back();
    }

    for (iter_config_nodes = config_nodes.begin();iter_config_nodes != config_nodes.end();++iter_config_nodes) {
        trim_front(*iter_config_nodes);
        trim_back(*iter_config_nodes);

        if (iter_config_nodes->empty()) {
            COMPILER_ERR("Error syntax with continuous \":\".");
        }
    }

    ::trim_back(str,2,' ',':');
    root_node.floor = 0;
    root_node.str = str;

    iter_config_nodes = config_nodes.begin();

//    for (;iter_tree != tree.cend();++iter_tree) {
//        for (iter_each_tree = (*iter_tree).cbegin();iter_each_tree != (*iter_tree).cend();++iter_each_tree) {
//            if (*iter_config_nodes == *iter_each_tree) {
//                iter_config_nodes ++;

//                if (iter_config_nodes == config_nodes.cend()) {
//                    match_sum ++;
//                    iter_config_nodes --;
//                }
//            }
//        }
//    }

    if (match_sum > 1) {
        COMPILER_ERR("Conflicted defination found.");
    }

    config_root_node.push_back(root_node);
    add_line(match_sum ? Config_Node_Header : Config_Other_Header,&config_root_node.back());
    return 0;
}

int32_t sequence::syntax_config_config(char *str,const uint32_t floor)
{
    const list<string> seperator = {":"};
    list<string> seperated_config;
    line_t last_line = lines.back();

    if (last_line.syntax == Empty) {
        COMPILER_ERR(fmt::format("Extra empty line {:d};",_cur_line - 1));
    }

    if (last_line.syntax < Config_Node_Header || last_line.syntax > Config_Other_Config) {
        COMPILER_ERR("Sub config must have a parent config.");
    }

    config_node_t node;
    config_node_t *last_node = nullptr;
    config_node_t *parent_node = &config_root_node.back();

    while (floor - parent_node->floor != 1) {
        parent_node = &(parent_node->sub_node.back());
    }

    last_config_node(&last_node,nullptr);

    if (last_node == nullptr) {
        COMPILER_ERR("No root config found.");
    }

    int32_t floor_jump = floor - last_node->floor;

    if (floor_jump > 1) {
        COMPILER_ERR("Too deep config.");
    }

    split(str,seperator,seperated_config);

    node.floor = floor;
    node.str = seperated_config.front();
    node.config = seperated_config.back();

    parent_node->sub_node.push_back(node);

    return 0;
}

void sequence::last_config_node(config_node_t **node, config_node_t **parent)
{
    if (node == nullptr) {
        return;
    }

    if (parent != nullptr) {
        *parent = nullptr;
    }

    if (config_root_node.empty()) {
        *node = nullptr;
        return;
    }

    *node = &config_root_node.back();

    while (!(*node)->sub_node.empty()) {
        if (parent != nullptr) {
            *parent = *node;
        }
        *node = &(*node)->sub_node.back();
    }
}

uint32_t sequence::trim_front(char **ptr)
{ return ::trim_front(ptr,2,' ','\t'); }

uint32_t sequence::trim_front(char *ptr)
{ return ::trim_front(ptr,2,' ','\t'); }

uint32_t sequence::trim_front(string &str)
{ return ::trim_front(str,2,' ','\t'); }

uint32_t sequence::trim_back(char *ptr)
{ return ::trim_back(ptr,3,'\n',' ','\t'); }

uint32_t sequence::trim_back(string &str)
{ return ::trim_back(str,3,'\n',' ','\t'); }

void sequence::print_vendor()
{
    if (!vendor.empty()) {
        cout << "Vendor : " << vendor << endl;
    }
}

void sequence::print_product()
{
    list<string>::const_iterator iter = product.cbegin();

    for (;iter != product.cend();++iter) {
        cout << "Product : " << *iter << endl;
    }
    cout << endl;
}

void sequence::print_include()
{
    list<sequence>::const_iterator iter;

    for (iter = sub_sequence.cbegin();iter != sub_sequence.cend();++iter) {
        cout << "include(" << iter->path() << ")" << endl;
    }
    cout << endl;
}

void sequence::print_tree()
{
    list<list<string>>::const_iterator iter_tree = tree.cbegin();
    list<string>::const_iterator iter_each_tree;

    for (;iter_tree != tree.cend();++iter_tree) {
        iter_each_tree = iter_tree->cbegin();
        cout << *iter_each_tree;

        iter_each_tree ++;

        for (;iter_each_tree != iter_tree->cend();++iter_each_tree) {
            cout << " << " << *iter_each_tree;
        }
        cout << endl;
    }
    cout << endl;
}

void print_sub_config(const config_node_t *node)
{
    list<config_node_t>::const_iterator iter;

    for (uint32_t i = 0;i < node->floor;++i) {
        cout << "\t";
    }

    cout << node->str << " : " << node->config << endl;;

    for (iter = node->sub_node.cbegin();iter != node->sub_node.cend();++iter) {
        print_sub_config(&(*iter));
    }
}

void sequence::print_config()
{
    list<config_root_node_t>::const_iterator iter;

    for (iter = config_root_node.cbegin();iter != config_root_node.cend();++iter) {
        print_sub_config(&(*iter));
        cout << endl;
    }
}
