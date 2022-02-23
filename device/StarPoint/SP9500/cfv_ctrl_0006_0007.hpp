#ifndef CFV_CTRL_0006_0007_HPP
#define CFV_CTRL_0006_0007_HPP

#include "cal_file_r1ab.h"
#include "cal_file_r1cd.h"
#include <string.h>
#include <errno.h>

using namespace rd;

int32_t for_0006_0007(cal_file *file)
{
    struct file_ver_v0006_t {
        uint32_t head;
        uint32_t ver;
        uint32_t tail;
    };

    STRUCT_ALIGN_S(item_info_v0006_t,4)
        uint32_t head   : 32;
        cal_file::cal_item_t  item : 32;
        uint32_t size   : 32;
        uint32_t pt     : 32;
        uint32_t tail   : 32;
    STRUCT_ALIGN_E(item_info_v0006_t,4)

    struct file_info_v0006_t {
        uint32_t  head;
        char	  sn[32];
        uint32_t  items;
        item_info_v0006_t *item_info;
        uint32_t  tail;
    public:
        file_info_v0006_t() {
            item_info = new item_info_v0006_t[rd::cal_file::CAL_TOTAL_ITEMS];
        }
        ~file_info_v0006_t() {
            SAFE_DEL(item_info);
        }
    };

    cal_file_r1cd *file_r1c = dynamic_cast<cal_file_r1cd *>(file);
    file_r1c->gen_rx_freq_res();
    file_r1c->gen_rx_filter_coef();
    file_r1c->update_rx_filter_table();

    char path[64] = {0};
    sprintf(path,"C:\\CSECal\\rfu%drf%d.cal",file->rfu_idx(),file->rf_idx());
    FILE *fp = fopen(path,"r+");
    CFO_ASSERT(fp,fp == nullptr);
    CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));

    file_ver_v0006_t ver;
    CFO_ASSERT(fp,fread(&ver,sizeof(ver),1,fp) == 0);
    ver.ver ++;
    CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));
    CFO_ASSERT(fp,fwrite(&ver,sizeof(ver),1,fp) == 0);
    CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));
    fclose(fp);

    Log.stdprintf("file \"%s\" version 0x0006 ---> 0x0007\n",path);
    Log.add_msg(0,"file \"%s\" version 0x0006 ---> 0x0007",path);
    return 0;
}

#endif // CFV_CTRL_0006_0007_HPP
