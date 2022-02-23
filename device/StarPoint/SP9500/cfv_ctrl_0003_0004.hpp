#ifndef CFV_CTRL_0003_0004_HPP
#define CFV_CTRL_0003_0004_HPP

#include "cal_file_r1ab.h"
#include "cal_file_r1cd.h"
#include <string.h>
#include <errno.h>

int32_t for_0003_0004(cal_file *file)
{
    typedef struct file_ver_v0004_t {
        uint32_t head;
        uint32_t ver;
        uint32_t tail;
    }file_ver_v0004_t;

    typedef struct item_info_v0004_t {
        uint32_t head;
        cal_file_r1ab::cal_item_t item;
        uint32_t size;
        uint32_t pt;
        uint32_t tail;
    }item_info_v0004_t;

    typedef struct file_info_v0004_t {
        uint32_t head;
        char	 sn[32];
        uint32_t items;
        item_info_v0004_t *item_info;
        uint32_t tail;
    public:
        file_info_v0004_t() {
            item_info = new item_info_v0004_t[rd::cal_file::CAL_TOTAL_ITEMS];
        }
        ~file_info_v0004_t() {
            SAFE_DEL(item_info);
        }
    }file_info_v0004_t;

    typedef struct att_io_data_t {
        uint64_t freq;
        float	 att1_10;
        float	 att1_20;
        float	 att1_30;
        float	 att2_10;
        float	 att2_20;
        float	 att2_30;
        float	 att3_10;
        float	 att3_20;
        float	 att3_30;
    }att_io_data_t;

    file_ver_v0004_t ver;
    uint32_t att_out_pos= 0,att_out_size = 0;
    int32_t old_data_size = 0;
    file_info_v0004_t info;
    item_info_v0004_t item_info_att_io;

    char path[64] = {0};
    sprintf(path,"C:\\CSECal\\rfu%drf%d.cal",file->rfu_idx(),file->rf_idx());

    FILE *fp = fopen(path,"r+");
    CFO_ASSERT(fp,nullptr == fp);
    CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));
    CFO_ASSERT(fp,fread(&ver,sizeof(file_ver_v0004_t),1,fp) == 0);
    ver.ver ++;
    CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));
    CFO_ASSERT(fp,fwrite(&ver,sizeof(file_ver_v0004_t),1,fp) == 0);

    item_info_v0004_t *item_info = info.item_info;
    CFO_ASSERT(fp,fread(&info,sizeof(file_info_v0004_t),1,fp) == 0);
    info.item_info = item_info;

    for (uint32_t i = 0;i < info.items;i ++)
        CFO_ASSERT(fp,fread(&(info.item_info[i]),sizeof(item_info_v0004_t),1,fp) == 0);
    info.items ++;
    CFO_ASSERT(fp,fseek(fp,sizeof(file_ver_v0004_t),SEEK_SET));
    CFO_ASSERT(fp,fwrite(&info,sizeof(file_info_v0004_t),1,fp) == 0);
    CFO_ASSERT(fp,fseek(fp,(info.items - 1) * sizeof(item_info_v0004_t),SEEK_CUR));

    int32_t pos_table_star = ftell(fp);
    int32_t pos_table_stop = 0;
    CFO_ASSERT(fp,-1 == pos_table_star);
    CFO_ASSERT(fp,fseek(fp,0,SEEK_END));
    pos_table_stop = ftell(fp);
    CFO_ASSERT(fp,-1 == pos_table_stop);
    old_data_size = pos_table_stop - pos_table_star;
    CFO_ASSERT(fp,fseek(fp,pos_table_star,SEEK_SET));
    char *buf = new char[old_data_size / sizeof(char)];
    CFO_ASSERT(fp,fread(buf,1,old_data_size,fp) == 0);

    CFO_ASSERT(fp,fseek(fp,sizeof(item_info_v0004_t),SEEK_END));
    CFO_ASSERT(fp,fseek(fp,sizeof(file_ver_v0004_t) + sizeof(file_info_v0004_t),SEEK_SET));
    CFO_ASSERT(fp,fseek(fp,(info.items - 1) * sizeof(item_info_v0004_t),SEEK_CUR));

    item_info_att_io.head   = 0xaa;
    item_info_att_io.item   = cal_file_r1ab::TX_ATT_IO;
    item_info_att_io.size   = sizeof(att_io_data_t);
    item_info_att_io.pt     = (uint32_t)SERIE_SIZE(50000000,6000000000,5000000);
    item_info_att_io.tail   = 0xff;

    CFO_ASSERT(fp,fwrite(&item_info_att_io,sizeof(item_info_v0004_t),1,fp) == 0);
    CFO_ASSERT(fp,fwrite(buf,1,old_data_size,fp) == 0);
    delete []buf;

    att_out_pos += sizeof(file_ver_v0004_t);
    att_out_pos += sizeof(file_info_v0004_t);
    for (uint32_t i = 0;i < info.items;i ++)
        att_out_pos += sizeof(item_info_v0004_t);
    for (int32_t i = 0;i < (int32_t)(cal_file_r1ab::TX_ATT_OP);i ++)
        att_out_pos += ((info.item_info[i]).size) * ((info.item_info[i]).pt);

    att_out_size = info.item_info[(int32_t)cal_file_r1ab::TX_ATT_OP].size * info.item_info[(int32_t)cal_file_r1ab::TX_ATT_OP].pt;	//it's safe here

    CFO_ASSERT(fp,fseek(fp,att_out_size,SEEK_END));
    CFO_ASSERT(fp,fseek(fp,att_out_pos,SEEK_SET));

    buf = new char[att_out_size / sizeof(char)];
    CFO_ASSERT(fp,fread(buf,1,att_out_size,fp) == 0);
    CFO_ASSERT(fp,fseek(fp,pos_table_stop + sizeof(item_info_v0004_t),SEEK_SET));
    CFO_ASSERT(fp,fwrite(buf,1,att_out_size,fp) == 0);
    delete []buf;
    CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));
    fclose(fp);

    Log.stdprintf("file \"%s\" version 0x0003 ---> 0x0004\n",path);
    Log.add_msg(0,"file \"%s\" version 0x0003 ---> 0x0004",path);
    return 0;
}

#endif // CFV_CTRL_0003_0004_HPP
