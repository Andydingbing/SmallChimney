#ifndef GEN_INI_FILE_H
#define GEN_INI_FILE_H

#include "global_header.h"

/*
 * A general ini file operation class.
 *
 * 1. '#' or '//' to annotate one section
 * 2. "//"        to annotate one key
 * 3. not support '\r' or '\n' in both section and key name
 * 4. the file should be placed directly in the module path
 */

namespace rd {

class gen_ini_file
{
public:
    gen_ini_file(const char *file_name);

    bool get_config_str_value(const char *section,const char *key,char *value);
    bool get_config_int_value(const char *section,const char *key,int *value);

private:
    char _path[256];
};

} //namespace rd

#endif // GEN_INI_FILE_H
