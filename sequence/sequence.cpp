#include "sequence.h"
#include "preprocessor/prefix.h"
#include <vector>
#include <cstdarg>

#define COMPILER_ERR(err) \
{   _err = (err); \
    _err_line = _cur_line; \
    if (!_continue_when_err) { \
        return -1; \
    }; \
}

using namespace std;

template<typename T,
template<typename _Tp = T, typename allocator_t = std::allocator<_Tp>> class container_t>
uint32_t trim_front(char **str,container_t<T> &args)
{
    typename container_t<T>::const_iterator iter;
    uint32_t sum = 0;
    bool reach_other = false;

    while (!reach_other) {
        for (iter = args.cbegin();iter != args.cend();++iter) {
            if (**str == *iter) {
                sum ++;
                (*str) ++;
                break;
            }
        }
        if (iter == args.cend()) {
            break;
        }
    }
    return sum;
}

uint32_t trim_front(char **str,const uint32_t n,...)
{
    vector<char> args;
    uint32_t sum = 0;
    size_t arg_idx = 0;
    va_list ap;

    va_start(ap,n);

    for (uint32_t i = 0;i < n;++i) {
        args.push_back(char(va_arg(ap,int)));
    }

    return trim_front<char>(str,args);
}

SYM_INLINE uint32_t trim_front(char **str)
{
    return trim_front(str,2,' ','\t');
}

void trim_back(char **str)
{
    char *ptr = (*str) + strlen(*str) - 1;

    while (*ptr == '\n' || *ptr == ' ' || *ptr == '\t') {
        *ptr = '\0';
        ptr --;
    }
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

int32_t sequence::parse()
{
    char *str = _buf;

    read_one_line(&str,false,true);

    if (strlen(str) == 0 || *str == ' ') {
        COMPILER_ERR("The tree must begin with a root node.");
    }

    list<string> node;


    while (read_one_line(&str,false,false) == 0) {

    }
}

int32_t sequence::parse_vendor()
{
    char *str = _buf;
    const char header_vendor[] = "Vendor";

    fseek(_fp,0,SEEK_SET);
    read_one_line(&str);

    if (strncmp(str,header_vendor,strlen(header_vendor))) {
        COMPILER_ERR("Must start with \"Vendor\".");
    }

    str += strlen(header_vendor);

    trim_front(&str,2,' ','\t');

    if (*str != ':') {
        COMPILER_ERR("Missing \":\" after \"Vendor\".");
    }

    str += 1;
    trim_front(&str);
    trim_back(&str);
    vendor = str;

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
    trim_back(&str);

    left = str;
    while ((right = strchr(left,';')) != nullptr) {
        ZERO_ARRAY(each_product);
        strncpy(each_product,left,right - left);
        product.push_back(each_product);
        left = right + 1;

        trim_front(&left);

        if (*left == ';') {
            COMPILER_ERR("Invalid string with continuously \";\".");
        }
    }

    product.push_back(left);
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

int32_t sequence::parse_tree_node(char **str)
{
    char *ptr = *str;

//    if (ptr[strlen(ptr) - 1] == ':') {
//        _err
//    }


    return 0;
}

int32_t sequence::read_one_line(char **str,const bool trim_front,const bool trim_back,const bool err_if_empty)
{
    ZERO_ARRAY(_buf);
    char *str_temp = _buf;
    bool err_lf = false;

    while (!feof(_fp)) {
        if (fgets(_buf,sizeof(_buf),_fp) == nullptr) {
            return -1;
        }

        _cur_line ++;

        if (trim_front) { ::trim_front(&str_temp); }
        if (trim_back)  { ::trim_back(&str_temp);  }

        if (is_empty_line(str_temp)) {
            if (err_if_empty) {
                err_lf = true;
                break;
            }
        } else {
            break;
        }
    }

    if (str != nullptr && (*str) != nullptr) {
        (*str) = str_temp;
    }
    return err_lf ? -2 : 0;
}
