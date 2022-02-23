#ifndef CFV_CTRL_0004_0005_HPP
#define CFV_CTRL_0004_0005_HPP

#include "cal_file_r1ab.h"
#include "cal_file_r1cd.h"
#include <string.h>
#include <errno.h>

int32_t for_0004_0005(cal_file *file)
{
    typedef struct file_ver_v0005_t {
        uint32_t head;
        uint32_t ver;
        uint32_t tail;
    }file_ver_v0005_t;

    STRUCT_ALIGN_S(item_info_v0005_t,4)
        uint32_t head : 32;
        cal_file_r1ab::cal_item_t item : 32;
        uint32_t size : 32;
        uint32_t pt   : 32;
        uint32_t tail : 32;
    STRUCT_ALIGN_E(item_info_v0005_t,4)

    typedef struct file_info_v0005_t {
        uint32_t head;
        char	 sn[32];
        uint32_t items;
        item_info_v0005_t *item_info;
        uint32_t tail;
    public:
        file_info_v0005_t() {
            item_info = new item_info_v0005_t[rd::cal_file::CAL_TOTAL_ITEMS];
        }
        ~file_info_v0005_t() {
            SAFE_DEL(item_info);
        }
    }file_info_v0005_t;

    file_ver_v0005_t ver;
    uint32_t max_byte = 0;
    file_info_v0005_t info;

    char path[64] = {0};
    sprintf(path,"C:\\CSECal\\rfu%drf%d.cal",file->rfu_idx(),file->rf_idx());

    FILE *fp = fopen(path,"r+");
    CFO_ASSERT(fp,nullptr == fp);
    CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));
    CFO_ASSERT(fp,fread(&ver,sizeof(file_ver_v0005_t),1,fp) == 0);
    ver.head = 0x44A4AE51;
    ver.ver ++;
    ver.tail = 0x64704C83;
    CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));
    CFO_ASSERT(fp,fwrite(&ver,sizeof(file_ver_v0005_t),1,fp) == 0);

    item_info_v0005_t *item_info = info.item_info;
    CFO_ASSERT(fp,fread(&info,sizeof(file_info_v0005_t),1,fp) == 0);
    info.item_info = item_info;
    info.head = 0xA56834ED;
    info.tail = 0xDA974411;
    CFO_ASSERT(fp,fseek(fp,sizeof(file_ver_v0005_t),SEEK_SET));
    CFO_ASSERT(fp,fwrite(&info,sizeof(file_info_v0005_t),1,fp) == 0);

    for (uint32_t i = 0;i < info.items;i ++) {
        CFO_ASSERT(fp,fread(&(info.item_info[i]),sizeof(item_info_v0005_t),1,fp) == 0);
        info.item_info[i].head = 0xaa;
        info.item_info[i].tail = 0xff;
    }
    CFO_ASSERT(fp,fseek(fp,sizeof(file_ver_v0005_t) + sizeof(file_info_v0005_t),SEEK_SET));
    for (uint32_t i = 0;i < info.items;i ++)
        CFO_ASSERT(fp,fwrite(&(info.item_info[i]),sizeof(item_info_v0005_t),1,fp) == 0);
    CFO_ASSERT(fp,fseek(fp,0,SEEK_END));

    item_info_v0005_t item_info_r1cd[23] = {
        {0xaa,cal_file::TX_LOL,         80,	1,	 0xff},
        {0xaa,cal_file::TX_SB,          88,	1,	 0xff},
        {0xaa,cal_file::TX_PWR_OP,		96,	1191,0xff},
        {0xaa,cal_file::TX_PWR_IO,		96,	1191,0xff},
        {0xaa,cal_file::TX_ATT_OP,		456,1191,0xff},
        {0xaa,cal_file::TX_ATT_IO,		448,1191,0xff},
        {0xaa,cal_file::RX_REF_OP,		192,1181,0xff},
        {0xaa,cal_file::RX_REF_IO,		192,1181,0xff},
        {0xaa,cal_file::RX_ATT_OP,		608,1181,0xff},
        {0xaa,cal_file::RX_ATT_IO,		528,1181,0xff},
        {0xaa,cal_file::TX_FILTER_80,   224,1191,0xff},
        {0xaa,cal_file::TX_FILTER_160,  224,1191,0xff},
        {0xaa,cal_file::TX_RF_FR_0,     16,	1584,0xff},
        {0xaa,cal_file::TX_RF_FR_1,     16,	1534,0xff},
        {0xaa,cal_file::TX_IF_FR,       16,	84,	 0xff},
        {0xaa,cal_file::RX_FILTER_80,   440,1181,0xff},
        {0xaa,cal_file::RX_FILTER_160,  632,1181,0xff},
        {0xaa,cal_file::RX_RF_FR,       16,	3034,0xff},
        {0xaa,cal_file::RX_IF_FR,       16,	84,	 0xff},
        {0xaa,cal_file::TX_FILTER_OFFSET_OP_80,	64,	1191,0xff},
        {0xaa,cal_file::TX_FILTER_OFFSET_IO_80,	64,	1191,0xff},
        {0xaa,cal_file::RX_FILTER_OFFSET_OP_80,	80,	1181,0xff},
        {0xaa,cal_file::RX_FILTER_OFFSET_IO_80,	80,	1181,0xff}
    };
    info.items = 23;
    info.sn[8] = 'C';
    for (uint32_t i = 0;i < info.items;i ++)
        info.item_info[i] = item_info_r1cd[i];

    CFO_ASSERT(fp,fwrite(&info,sizeof(file_info_v0005_t),1,fp) == 0);
    for (uint32_t i = 0;i < info.items;i ++)
        CFO_ASSERT(fp,fwrite(&(info.item_info[i]),sizeof(item_info_v0005_t),1,fp) == 0);

    max_byte = cal_file::get_max_item_byte();
    unsigned char *buf = new unsigned char[max_byte];
    for (uint32_t i = 0;i < info.items;i ++) {
        uint32_t size = ((info.item_info[i]).size) * ((info.item_info[i]).pt);
        memset(buf,0,max_byte * sizeof(unsigned char));
        CFO_ASSERT(fp,fwrite(buf,1,size,fp) == 0);
    }
    delete []buf;
    CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));
    fclose(fp);

    Log.stdprintf("file \"%s\" version 0x0004 ---> 0x0005\n",path);
    Log.add_msg(0,"file \"%s\" version 0x0004 ---> 0x0005",path);
    return 0;
}

#endif // CFV_CTRL_0004_0005_HPP
