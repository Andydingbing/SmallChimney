#include "parser.h"


std::string get_vendor(FILE *fp,const char *)
{
    char buffer[256] = {0};

    fseek(fp,0,SEEK_SET);

    fgets(buffer,sizeof(buffer),fp);
    return "";
}
