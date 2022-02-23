#ifndef CFV_CTRL_0002_0003_HPP
#define CFV_CTRL_0002_0003_HPP

#include "cal_file_r1ab.h"
#include "cal_file_r1cd.h"
#include <string.h>
#include <errno.h>

int32_t for_0002_0003(cal_file *file)
{
    struct file_ver_v0003_t {
        uint32_t head;
        uint32_t ver;
        uint32_t tail;
    };

    struct item_info_v0003_t {
        uint32_t head;
        cal_file_r1ab::cal_item_t item;
        uint32_t size;
        uint32_t pt;
        uint32_t tail;
    };

    struct file_info_v0003_t {
        uint32_t head;
        char	 sn[32];
        uint32_t items;
        item_info_v0003_t *item_info;
        uint32_t tail;
    };

    struct rx_state_t {
        int64_t  ad_offset;
        int32_t  lna_att;
        double   att1;
        int32_t  att2;
    };

    struct rx_ref_data_v0002_t {
        uint64_t freq;
        rx_state_t state_op[SERIE_SIZE(30,-30,-1)];
        rx_state_t state_io[SERIE_SIZE(30,-20,-1)];
    };

    struct rx_ref_data_v0003_t {
        uint64_t freq;
        rx_state_t state_op[SERIE_SIZE(30,-40,-1)];
        rx_state_t state_io[SERIE_SIZE(30,-30,-1)];
    public:
        rx_ref_data_v0003_t()
        {
            for (uint32_t i = 0;i < ARRAY_SIZE(state_op);i ++) {
                state_op[i].lna_att = 0;
                state_op[i].att1 = 0.0;
                state_op[i].att2 = 0;
                state_op[i].ad_offset = 0;
            }
            for (uint32_t i = 0;i < ARRAY_SIZE(state_io);i ++) {
                state_io[i].lna_att = 0;
                state_io[i].att1 = 0.0;
                state_io[i].att2 = 0;
                state_io[i].ad_offset = 0;
            }
        }
    };

    char path_new[64] = {0};
    char path[64] = {0};
    sprintf(path,"C:\\CSECal\\rfu%drf%d.cal",file->rfu_idx(),file->rf_idx());
    strcpy(path_new,path);
    strcat(path_new,"_v0001_temp");

    uint32_t max_size = 0;
    char *buf = nullptr;

    FILE *fp_old = fopen(path,"r+");
    CFO_ASSERT(fp_old,nullptr == fp_old);
    CFO_ASSERT(fp_old,fseek(fp_old,0,SEEK_SET));
    FILE *fp_new = fopen(path_new,"w+");
    CFO_ASSERT(fp_new,nullptr == fp_new);
    CFO_ASSERT(fp_new,fseek(fp_new,0,SEEK_SET));

    file_ver_v0003_t ver;
    CFO_ASSERT(fp_old,fread(&ver,sizeof(file_ver_v0003_t),1,fp_old) == 0);
    ver.ver += 1;
    CFO_ASSERT(fp_new,fwrite(&ver,sizeof(file_ver_v0003_t),1,fp_new) == 0);

    file_info_v0003_t info;
    CFO_ASSERT(fp_old,fread(&info,sizeof(file_info_v0003_t),1,fp_old) == 0);
    CFO_ASSERT(fp_new,fwrite(&info,sizeof(file_info_v0003_t),1,fp_new) == 0);

    info.item_info = new item_info_v0003_t[info.items];
    for (uint32_t i = 0;i < info.items;i ++) {
        CFO_ASSERT(fp_old,fread(&(info.item_info[i]),sizeof(item_info_v0003_t),1,fp_old) == 0);
        if (i == cal_file_r1ab::RX_REF)
            info.item_info[i].size = sizeof(rx_ref_data_v0003_t);
        if (info.item_info[i].size * info.item_info[i].pt > max_size)
            max_size = info.item_info[i].size * info.item_info[i].pt;
        CFO_ASSERT(fp_new,fwrite(&(info.item_info[i]),sizeof(item_info_v0003_t),1,fp_new) == 0);
    }
    buf = new char[max_size / sizeof(char)];
    for (uint32_t i = 0;i < cal_file_r1ab::RX_REF;i ++) {
        memset(buf,0,max_size / sizeof(char));
        CFO_ASSERT(fp_old,fread(buf,1,info.item_info[i].size * info.item_info[i].pt,fp_old) == 0);
        CFO_ASSERT(fp_new,fwrite(buf,1,info.item_info[i].size * info.item_info[i].pt,fp_new) == 0);
    }
    rx_ref_data_v0002_t data_v0002;
    rx_ref_data_v0003_t data_v0003;
    for (uint32_t i = 0;i < SERIE_SIZE(100000000,6000000000,5000000);i ++) {
        CFO_ASSERT(fp_old,fread(&data_v0002,sizeof(rx_ref_data_v0002_t),1,fp_old) == 0);
        data_v0003.freq = data_v0002.freq;
        for (uint32_t j = 0;j < ARRAY_SIZE(data_v0002.state_op);j ++) {
            data_v0003.state_op[j].lna_att = data_v0002.state_op[j].lna_att;
            data_v0003.state_op[j].att1 = data_v0002.state_op[j].att1;
            data_v0003.state_op[j].att2 = data_v0002.state_op[j].att2;
            data_v0003.state_op[j].ad_offset = data_v0002.state_op[j].ad_offset;
        }
        for (uint32_t j = 0;j < ARRAY_SIZE(data_v0002.state_io);j ++) {
            data_v0003.state_io[j].lna_att = data_v0002.state_io[j].lna_att;
            data_v0003.state_io[j].att1 = data_v0002.state_io[j].att1;
            data_v0003.state_io[j].att2 = data_v0002.state_io[j].att2;
            data_v0003.state_io[j].ad_offset = data_v0002.state_io[j].ad_offset;
        }
        CFO_ASSERT(fp_new,fwrite(&data_v0003,sizeof(rx_ref_data_v0003_t),1,fp_new) == 0);
    }
    fclose(fp_new);
    fclose(fp_old);

    remove(path);
    rename(path_new,path);

    delete []buf;
    delete []info.item_info;
    Log.stdprintf("file \"%s\" version 0x0002 ---> 0x0003\n",path);
    Log.add_msg(0,"file \"%s\" version 0x0002 ---> 0x0003",path);
    return 0;
}

#endif // CFV_CTRL_0002_0003_HPP
