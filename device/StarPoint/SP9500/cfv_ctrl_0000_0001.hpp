#ifndef CFV_CTRL_0000_0001_HPP
#define CFV_CTRL_0000_0001_HPP

#include "cal_file_r1ab.h"
#include "cal_file_r1cd.h"
#include <string.h>
#include <errno.h>

using namespace rd;

int32_t for_0000_0001(cal_file *file)
{
    struct item_info_v0000_t {
        uint32_t head;
        cal_file_r1ab::cal_item_t item;
        uint32_t size;
        uint32_t pt;
        uint32_t tail;
    };

    struct file_info_v0000_t {
        char     sn[32];
        uint32_t items;
        item_info_v0000_t *item_info;
    };

    struct file_info_v0001_t {
        uint32_t head;
        char	 sn[32];
        uint32_t items;
        item_info_v0000_t *item_info;
        uint32_t tail;
    };

    struct file_ver_v0001_t {
        uint32_t head;
        uint32_t ver;
        uint32_t tail;
    };

    file_info_v0000_t info_v0000;
    file_info_v0001_t info_v0001;
    file_ver_v0001_t  ver_v0001;

    char path[64] = {0};
    sprintf(path,"C:\\CSECal\\rfu%drf%d.cal",file->rfu_idx(),file->rf_idx());

    char path_new[64];
    strcpy(path_new,path);
    strcat(path_new,"_v0001_temp");

    FILE *fp_old;
    FILE *fp_new;

    fp_old = fopen(path,"r+");
    CFO_ASSERT(fp_old,nullptr == fp_old);
    CFO_ASSERT(fp_old,fseek(fp_old,0,SEEK_SET));
    CFO_ASSERT(fp_old,fread(&info_v0000,sizeof(file_info_v0000_t),1,fp_old) == 0);
    info_v0000.item_info = new item_info_v0000_t[(info_v0000.items)];
    for (uint32_t i = 0;i < info_v0000.items;i ++)
        CFO_ASSERT(fp_old,fread(&(info_v0000.item_info[i]),sizeof(item_info_v0000_t),1,fp_old) == 0);

    info_v0001.head      = 0x02ABCDEF;
    strcpy(info_v0001.sn,info_v0000.sn);
    info_v0001.items	 = info_v0000.items;
    info_v0001.item_info = info_v0000.item_info;
    info_v0001.tail      = 0x03ABCDEF;

    ver_v0001.head = 0x04ABCDEF;
    ver_v0001.ver  = 0x0001;
    ver_v0001.tail = 0x05ABCDEF;

    fp_new = fopen(path_new,"w+");
    CFO_ASSERT(fp_new,nullptr == fp_new);
    CFO_ASSERT(fp_new,fseek(fp_new,0,SEEK_SET));
    CFO_ASSERT(fp_new,fwrite(&ver_v0001,sizeof(file_ver_v0001_t),1,fp_new) == 0);
    CFO_ASSERT(fp_new,fwrite(&info_v0001,sizeof(file_info_v0001_t),1,fp_new) == 0);
    for (uint32_t i = 0;i < info_v0001.items;i ++)
        CFO_ASSERT(fp_new,fwrite(&(info_v0001.item_info[i]),sizeof(item_info_v0000_t),1,fp_new) == 0);

    for (uint32_t i = 0;i < info_v0001.items;i ++) {
        uint32_t size = ((info_v0001.item_info[i]).size) * ((info_v0001.item_info[i]).pt);
        unsigned char *buf = new unsigned char[size];
        memset(buf,0,size * sizeof(unsigned char));
        CFO_ASSERT(fp_old,fread(buf,sizeof(unsigned char),size,fp_old) == 0);
        CFO_ASSERT(fp_new,fwrite(buf,sizeof(unsigned char),size,fp_new) == 0);
        delete []buf;
    }
    CFO_ASSERT(fp_old,fseek(fp_old,0,SEEK_SET));
    CFO_ASSERT(fp_new,fseek(fp_new,0,SEEK_SET));
    fclose(fp_old);
    fclose(fp_new);

    remove(path);
    rename(path_new,path);
    delete []info_v0000.item_info;
    Log.stdprintf("file \"%s\" version 0x0000 ---> 0x0001\n",path);
    Log.add_msg(0,"file \"%s\" version 0x0000 ---> 0x0001",path);
    return 0;
}

#endif // CFV_CTRL_0000_0001_HPP
