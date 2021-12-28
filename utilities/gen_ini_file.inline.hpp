#include "gen_ini_file.hpp"
#include <stdio.h>
#include <string>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

bool compare(const char *str1,const char *str2);
bool get_key_value(FILE *fp,const char *key,char *value);

using namespace rd;
using namespace boost;

gen_ini_file::gen_ini_file(const char *file_name)
{
    ZERO_ARRAY(_path);
    strcpy(_path,filesystem::initial_path<filesystem::path>().string().c_str());
    strcat(_path,"\\");
    strcat(_path,file_name);
}

bool gen_ini_file::get_config_str_value(const char *section,const char *key,char *value)
{
    FILE *fp = nullptr;
    char buffer[160] = {0};
    char *str1 = nullptr;
    char *str2 = nullptr;

    memset(buffer,0,sizeof(buffer));

    if ((fp = fopen(_path,"r")) == nullptr) {
        return false;
    }

    while (!feof(fp)) {
        if (fgets(buffer,150,fp) == nullptr) {
            break;
        }
        str1 = buffer;
        while ((*str1 == ' ') || (*str1 == '\t')) {
            str1 ++;
        }
        if ('[' == *str1) {
            str1 ++;
            while ((*str1 == ' ') || (*str1 == '\t')) {
                str1 ++;
            }

            // first meanful character
            str2 = str1;
            while ((*str1 != ']') && (*str1 != '\0')) {
                str1 ++;
            }
            if (*str1 == '\0') { // keep reading next line
                continue;
            }
            while ((*(str1 - 1) == ' ') || (*(str1 - 1) == '\t')) {
                str1 --;
            }
            *str1 = '\0';

            if (compare(str2,section)) {
                if (get_key_value(fp,key,value)) {
                    fclose(fp);
                    return true;
                }
            }
        }
    }
    fclose(fp);
    return false;
}

bool gen_ini_file::get_config_int_value(const char *section,const char *key,int *value)
{
    char str_key_value[64] = {0};
    char *str = nullptr;

    if (get_config_str_value(section,key,str_key_value) == false) {
        return false;
    }
    str = str_key_value;
    while ((*str == ' ') || (*str == '\t')) {
        str ++;
    }
    if ((*str == '0') && (('x' == *(str + 1)) || (*(str + 1) == 'X'))) {
        sscanf(str + 2,"%x",value);
    } else {
        sscanf(str,"%d",value);
    }

    return true;
}

bool compare(const char *str1,const char *str2)
{
    if (strlen(str1) != strlen(str2)) {
        return false;
    }

    while (*str1 == *str2) {
        if (*str1 == '\0') {
            break;
        }
        str1 ++;
        str2 ++;
    }

    if (*str1 == '\0') {
        return true;
    }
    return false;
}

bool get_key_value(FILE *fp, const char *key, char *value)
{
    char buffer[160] = {0};
    char *str1 = nullptr;
    char *str2 = nullptr;
    char *str3 = nullptr;

    memset(buffer,0,sizeof(buffer));
    while (!feof(fp)) {
        if (fgets(buffer,150,fp) == nullptr) {
            break;
        }
        str1 = buffer;
        while ((*str1 == ' ') || (*str1 == '\t')) {
            str1 ++;
        }

        // '#' or "//" as the annotate
        if (*str1 == '#') {
            continue;
        }
        if ((*str1 == '/') && (*(str1 + 1) == '/')) {
            continue;
        }

        // if end of the line or meeting '\r' or '\n',read next line
        if ((*str1 == '\0') || (*str1 == 0x0d) || (*str1 == 0x0a)) {
            continue;
        }

        // avoid when another section was inserted between the right section and it's keys
        if (*str1 == '[') {
            str2 = str1;
            while ((*str1 != ']') && (*str1 != '\0')) {
                str1 ++;
            }
            if (*str1 == ']') {
                break;
            }
            str1 = str2;
        }

        // the first meanful character
        str2 = str1;
        // the last character
        while ((*str1 != '=') && (*str1 != '\0')) {
            str1 ++;
        }
        if (*str1 == '\0') {
            continue;
        }
        str3 = str1 + 1;

        // empty key name
        if (str2 == str1) {
            continue;
        }

        *str1 = '\0';
        str1 --;
        while ((*str1 == ' ') || (*str1 == '\t')) {
            *str1 = '\0';
            str1 --;
        }

        if (compare(str2,key)) {
            str1 = str3; // jump over '='
            while ((*str1 == ' ') || (*str1 == '\t')) {
                str1 ++;
            }
            str3 = str1;
            while ((*str1 != '\0') && (*str1 != 0x0d) && (*str1 != 0x0a)) {
                // "//" as the annotate
                if ((*str1 == '/') && (*(str1 + 1) == '/')) {
                    break;
                }
                str1 ++;
            }
            *str1 = '\0';
            memcpy(value,str3,strlen(str3));
            *(value + strlen(str3)) = '\0';
            return true;
        }
    }
    return false;
}
