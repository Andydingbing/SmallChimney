#ifndef CFV_CTRL_0001_0002_HPP
#define CFV_CTRL_0001_0002_HPP

#include "cal_file_r1ab.h"
#include "cal_file_r1cd.h"
#include <string.h>
#include <errno.h>

int32_t for_0001_0002(cal_file *file)
{
    struct file_ver_v0001_t {
        uint32_t head;
        uint32_t ver;
        uint32_t tail;
    };

    struct item_info_v0000_t {
        uint32_t head;
        cal_file_r1ab::cal_item_t item;
        uint32_t size;
        uint32_t pt;
        uint32_t tail;
    };

    struct file_info_v0001_t {
        uint32_t head;
        char	 sn[32];
        uint32_t items;
        item_info_v0000_t *item_info;
        uint32_t tail;
    };

    struct x9119_data_t {
        int16_t value;
    };

    file_info_v0001_t info_v0001;
    file_ver_v0001_t ver_v0001;
    x9119_data_t data_x9119;
    data_x9119.value = 0;

    char path[64] = {0};
    char path_new[64] = {0};
    sprintf(path,"C:\\CSECal\\rfu%drf%d.cal",file->rfu_idx(),file->rf_idx());
    strcpy(path_new,path);

    FILE *fp_old;
    FILE *fp_new;

    fp_old = fopen(path,"r+");
    CFO_ASSERT(fp_old,nullptr == fp_old);
    CFO_ASSERT(fp_old,fseek(fp_old,sizeof(file_ver_v0001_t),SEEK_SET));
    CFO_ASSERT(fp_old,fread(&info_v0001,sizeof(file_info_v0001_t),1,fp_old) == 0);
    info_v0001.item_info = new item_info_v0000_t[(info_v0001.items)];
    for (uint32_t i = 0;i < info_v0001.items;i ++) {
        CFO_ASSERT(fp_old,fread(&info_v0001.item_info[i],sizeof(item_info_v0000_t),1,fp_old) == 0);
    }

    for (uint32_t i = 0;i < cal_file_r1ab::X9119;i ++) {//for the list of calibration item is only allowed to accumulate,so it is safe to use the enum member here
        CFO_ASSERT(fp_old,fseek(fp_old,info_v0001.item_info[i].size * info_v0001.item_info[i].pt,SEEK_CUR));
    }
    CFO_ASSERT(fp_old,fread(&data_x9119,sizeof(x9119_data_t),1,fp_old) == 0);

    CFO_ASSERT(fp_old,fseek(fp_old,0,SEEK_SET));
    CFO_ASSERT(fp_old,fread(&ver_v0001,sizeof(file_ver_v0001_t),1,fp_old) == 0);
    ver_v0001.ver += 1;
    CFO_ASSERT(fp_old,fseek(fp_old,0,SEEK_SET));
    CFO_ASSERT(fp_old,fwrite(&ver_v0001,sizeof(file_ver_v0001_t),1,fp_old) == 0);

    info_v0001.items -= 1;
    CFO_ASSERT(fp_old,fwrite(&info_v0001,sizeof(file_info_v0001_t),1,fp_old) == 0);

    int32_t data_size = 0;
    for (uint32_t i = 0;i < info_v0001.items;i ++)
        data_size += info_v0001.item_info[i].size * info_v0001.item_info[i].pt;
    char *buf = new char[data_size / sizeof(char)];
    CFO_ASSERT(fp_old,fseek(fp_old,(info_v0001.items + 1) * sizeof(item_info_v0000_t),SEEK_CUR));
    CFO_ASSERT(fp_old,fread(buf,1,data_size,fp_old) == 0);
    CFO_ASSERT(fp_old,fseek(fp_old,sizeof(file_ver_v0001_t) + sizeof(file_info_v0001_t) + info_v0001.items * sizeof(item_info_v0000_t),SEEK_SET));
    CFO_ASSERT(fp_old,fwrite(buf,1,data_size,fp_old) == 0);
    delete []buf;

    long new_file_size = ftell(fp_old);
    buf = new char[new_file_size / sizeof(char)];
    CFO_ASSERT(fp_old,fseek(fp_old,0,SEEK_SET));
    CFO_ASSERT(fp_old,fread(buf,1,new_file_size,fp_old) == 0);
    fclose(fp_old);
    fp_old = fopen(path,"w");
    CFO_ASSERT(fp_old,nullptr == fp_old);
    CFO_ASSERT(fp_old,fseek(fp_old,0,SEEK_SET));
    CFO_ASSERT(fp_old,fwrite(buf,1,new_file_size,fp_old) == 0);
    CFO_ASSERT(fp_old,fseek(fp_old,0,SEEK_SET));
    fclose(fp_old);
    delete []buf;

    *strrchr(path_new,'\\') = 0;
    strcat(path_new,"\\cxu.cal");

    fp_new = fopen(path_new,"r+");
    if (nullptr == fp_new) {
        fp_new = fopen(path_new,"w");
        CFO_ASSERT(fp_new,nullptr == fp_new);
        x9119_data_t data;
        data.value = 0;
        CFO_ASSERT(fp_new,fseek(fp_new,0,SEEK_SET));
        CFO_ASSERT(fp_new,fwrite(&data,sizeof(x9119_table::data_t),1,fp_new) == 0);
        CFO_ASSERT(fp_new,fseek(fp_new,0,SEEK_SET));
        fclose(fp_new);
    }
    if (data_x9119.value) {
        fp_new = fopen(path_new,"w");
        CFO_ASSERT(fp_new,nullptr == fp_new);
        CFO_ASSERT(fp_new,fseek(fp_new,0,SEEK_SET));
        CFO_ASSERT(fp_new,fwrite(&data_x9119,sizeof(x9119_table::data_t),1,fp_new) == 0);
        CFO_ASSERT(fp_new,fseek(fp_new,0,SEEK_SET));
        fclose(fp_new);
    }
    delete []info_v0001.item_info;
    Log.stdprintf("file \"%s\" version 0x0001 ---> 0x0002\n",path);
    Log.add_msg(0,"file \"%s\" version 0x0001 ---> 0x0002",path);
    return 0;
}

#endif // CFV_CTRL_0001_0002_HPP
