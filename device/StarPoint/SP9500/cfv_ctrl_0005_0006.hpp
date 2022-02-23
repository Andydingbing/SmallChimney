#ifndef CFV_CTRL_0005_0006_HPP
#define CFV_CTRL_0005_0006_HPP

#include "cal_file_r1ab.h"
#include "cal_file_r1cd.h"
#include <string.h>
#include <errno.h>

int32_t for_0005_0006(cal_file *file)
{
    struct file_ver_v0005_t {
        uint32_t head;
        uint32_t ver;
        uint32_t tail;
    };

    STRUCT_ALIGN_S(item_info_v0005_t,4)
        uint32_t head   : 32;
        cal_file::cal_item_t  item : 32;
        uint32_t size   : 32;
        uint32_t pt     : 32;
        uint32_t tail   : 32;
    STRUCT_ALIGN_E(item_info_v0005_t,4)

    typedef struct file_info_v0005_t {
        uint32_t  head;
        char	  sn[32];
        uint32_t  items;
        item_info_v0005_t *item_info;
        uint32_t  tail;
    public:
        file_info_v0005_t() {
            item_info = new item_info_v0005_t[rd::cal_file::CAL_TOTAL_ITEMS];
        }
        ~file_info_v0005_t() {
            SAFE_DEL(item_info);
        }
    }file_info_v0005_t;

    STRUCT_ALIGN_S(ud_OleDateTime_t, 8)   //from windows <ATLComTime.h>
    public:
        enum ud_DateTimeStatus_t        //from windows COleDateTime::DateTimeStatus
        {
            UDDTS_ERROR = -1,
            UDDTS_VALID = 0,
            UDDTS_INVALID = 1,          // Invalid date (out of range, etc.)
            UDDTS_NULL = 2,             // Literally has no value
        };
    public:
        tm _2tm() {
            double diff_day = t - 25569;
            time_t t_t = time_t((diff_day > 0.0 ? diff_day : 0.0) * 24 * 60 * 60);
            tm *t = localtime(&t_t);
            return *t;
        }
    public:
        double t;
        ud_DateTimeStatus_t status;
    STRUCT_ALIGN_E(ud_OleDateTime_t, 8)

// tx lo leakage
    struct data_f_tx_lol_t {
        uint64_t freq;
        int16_t  dc_i;
        int16_t  dc_q;
    };
    struct data_f_v0005_tx_lol_t : data_f_tx_lol_t {
        bool   use_sa;
        double pwr;
        double temp[4];
        ud_OleDateTime_t time;
    };
    struct data_f_v0006_tx_lol_t : data_f_tx_lol_t {
        bool   use_sa;
        double pwr;
        double temp[4];
        tm	   time;
    };
// tx sideband
    struct data_f_tx_sb_t {
        uint64_t freq;
        double   th;
        uint16_t am_i;
        uint16_t am_q;
    };
    struct data_f_v0005_tx_sb_t : data_f_tx_sb_t {
        bool   use_sa;
        double pwr;
        double temp[4];
        ud_OleDateTime_t time;
    };
    struct data_f_v0006_tx_sb_t : data_f_tx_sb_t {
        bool   use_sa;
        double pwr;
        double temp[4];
        tm	   time;
    };
// tx power
    struct data_f_v0005_tx_pwr_t {
        uint64_t freq;
        double   d_gain;
        double   att0;
        double   att1;
        double   att2;
        double   att3;
        double   temp[4];
        ud_OleDateTime_t time;
    };
    struct data_f_v0006_tx_pwr_t {
        uint64_t freq;
        double   d_gain;
        double   att0;
        double   att1;
        double   att2;
        double   att3;
        double   temp[4];
        tm		 time;
    };
// tx att
    struct data_f_v0005_tx_att_op_t {
        uint64_t freq;
        float    offset[R1C_TX_ATT_OP_POWER_PTS];
        double   temp[4];
        ud_OleDateTime_t time;
    };
    struct data_f_v0006_tx_att_op_t {
        uint64_t freq;
        float    offset[R1C_TX_ATT_OP_POWER_PTS];
        double   temp[4];
        tm time;
    };
    struct data_f_v0005_tx_att_io_t {
        uint64_t freq;
        float    offset[R1C_TX_ATT_IO_POWER_PTS];
        double   temp[4];
        ud_OleDateTime_t time;
    };
    struct data_f_v0006_tx_att_io_t {
        uint64_t freq;
        float    offset[R1C_TX_ATT_IO_POWER_PTS];
        double   temp[4];
        tm		 time;
    };
// rx ref
    struct rx_state_f_t {
        int16_t lna_att;
        int16_t att_019;
        float	att1;
        float	att2;
        float	att3;
        double	temp[4];
        int64_t ad_offset;
    };
    struct data_f_v0005_rx_ref_t {
        uint64_t freq;
        rx_state_f_t state[3];
        ud_OleDateTime_t time;
    };
    struct data_f_v0006_rx_ref_t {
        uint64_t freq;
        rx_state_f_t state[3];
        tm		 time;
    };
// rx att
    struct data_f_v0005_rx_att_op_t {
        uint64_t freq;
        int64_t  offset[R1C_RX_REF_OP_PTS - 3 + 1];
        double   temp[4];
        ud_OleDateTime_t time;
    };
    struct data_f_v0006_rx_att_op_t {
        uint64_t freq;
        int64_t  offset[R1C_RX_REF_OP_PTS - 3 + 1];
        double   temp[4];
        tm		 time;
    };
    struct data_f_v0005_rx_att_io_t {
        uint64_t freq;
        int64_t  offset[R1C_RX_REF_IO_PTS - 3 + 1];
        double   temp[4];
        ud_OleDateTime_t time;
    };
    struct data_f_v0006_rx_att_io_t {
        uint64_t freq;
        int64_t  offset[R1C_RX_REF_IO_PTS - 3 + 1];
        double   temp[4];
        tm		 time;
    };
// tx filter
    struct data_f_v0005_tx_filter_t {
        uint64_t freq;
        int16_t  coef_real[41];
        int16_t  coef_imag[41];
        double   temp[4];
        ud_OleDateTime_t time;
    };
    struct data_f_v0006_tx_filter_t {
        uint64_t freq;
        int16_t  coef_real[41];
        int16_t  coef_imag[41];
        double   temp[4];
        tm		 time;
    };
// rx filter
    struct data_f_v0005_rx_filter_80 {
        uint64_t freq;
        int16_t  coef_real[96];
        int16_t  coef_imag[96];
        double   temp[4];
        ud_OleDateTime_t time;
    };
    struct data_f_v0006_rx_filter_80 {
        uint64_t freq;
        int16_t  coef_real[96];
        int16_t  coef_imag[96];
        double   temp[4];
        tm		 time;
    };
    struct data_f_v0005_rx_filter_160 {
        uint64_t freq;
        int16_t  coef_real[144];
        int16_t  coef_imag[144];
        double   temp[4];
        ud_OleDateTime_t time;
    };
    struct data_f_v0006_rx_filter_160 {
        uint64_t freq;
        int16_t  coef_real[144];
        int16_t  coef_imag[144];
        double   temp[4];
        tm		 time;
    };
// filter offset
    struct data_f_v0005_tx_filter_offset_t {
        uint64_t freq;
        double   offset;
        double   temp[4];
        ud_OleDateTime_t time;
    };
    struct data_f_v0006_tx_filter_offset_t {
        uint64_t freq;
        double   offset;
        double   temp[4];
        tm		 time;
    };
    struct data_f_v0005_rx_filter_offset_t {
        uint64_t freq;
        int64_t  offset[3];
        double   temp[4];
        ud_OleDateTime_t time;
    };
    struct data_f_v0006_rx_filter_offset_t {
        uint64_t freq;
        int64_t  offset[3];
        double   temp[4];
        tm		 time;
    };

    char path_new[64] = {0};
    char path_old[64] = {0};
    sprintf(path_old,"C:\\CSECal\\rfu%drf%d.cal",file->rfu_idx(),file->rf_idx());
    strcpy(path_new,path_old);
    strcat(path_new,"_v0006_temp");

    FILE *fp_old = fopen(path_old,"r+");
    CFO_ASSERT(fp_old,nullptr == fp_old);
    CFO_ASSERT(fp_old,fseek(fp_old,0,SEEK_SET));
    FILE *fp_new = fopen(path_new,"w+");
    CFO_ASSERT(fp_new,nullptr == fp_new);
    CFO_ASSERT(fp_new,fseek(fp_new,0,SEEK_SET));

    int32_t size_r1ab = 5541296;
    char *buf_r1ab = new char[size_r1ab/sizeof(char)];
    CFO_ASSERT_S(fp_old,(fread(buf_r1ab,size_r1ab,1,fp_old) == 0),delete []buf_r1ab;);
    CFO_ASSERT_S(fp_new,(fwrite(buf_r1ab,size_r1ab,1,fp_new) == 0),delete []buf_r1ab;);
    CFO_ASSERT_S(fp_new,fflush(fp_new) == EOF,delete []buf_r1ab;);
    CFO_ASSERT_S(fp_old,fseek(fp_old,0,SEEK_SET),delete []buf_r1ab;);
    CFO_ASSERT_S(fp_new,fseek(fp_new,0,SEEK_SET),delete []buf_r1ab;);
    delete []buf_r1ab;

    file_ver_v0005_t ver;
    CFO_ASSERT(fp_old,fread(&ver,sizeof(ver),1,fp_old) == 0);
    ver.ver ++;
    CFO_ASSERT(fp_new,fwrite(&ver,sizeof(ver),1,fp_new) == 0);
    CFO_ASSERT(fp_new,fflush(fp_new) == EOF);

    CFO_ASSERT(fp_old,fseek(fp_old,size_r1ab,SEEK_SET));
    CFO_ASSERT(fp_new,fseek(fp_new,size_r1ab,SEEK_SET));

    item_info_v0005_t actual_item_info_r1cd[23] = {
        {0xaa,cal_file::TX_LOL,         104,	1,	 0xff},
        {0xaa,cal_file::TX_SB,          112,	1,	 0xff},
        {0xaa,cal_file::TX_PWR_OP,		120,	1191,0xff},
        {0xaa,cal_file::TX_PWR_IO,		120,	1191,0xff},
        {0xaa,cal_file::TX_ATT_OP,		480,	1191,0xff},
        {0xaa,cal_file::TX_ATT_IO,		472,	1191,0xff},
        {0xaa,cal_file::RX_REF_OP,		216,	1181,0xff},
        {0xaa,cal_file::RX_REF_IO,		216,	1181,0xff},
        {0xaa,cal_file::RX_ATT_OP,		632,	1181,0xff},
        {0xaa,cal_file::RX_ATT_IO,		552,	1181,0xff},
        {0xaa,cal_file::TX_FILTER_80,   248,	1191,0xff},
        {0xaa,cal_file::TX_FILTER_160,  248,	1191,0xff},
        {0xaa,cal_file::TX_RF_FR_0,		16,		1584,0xff},
        {0xaa,cal_file::TX_RF_FR_1,		16,		1534,0xff},
        {0xaa,cal_file::TX_IF_FR,       16,		84,	 0xff},
        {0xaa,cal_file::RX_FILTER_80,	464,	1181,0xff},
        {0xaa,cal_file::RX_FILTER_160,	656,	1181,0xff},
        {0xaa,cal_file::RX_RF_FR,		16,		3034,0xff},
        {0xaa,cal_file::RX_IF_FR,		16,		84,	 0xff},
        {0xaa,cal_file::TX_FILTER_OFFSET_OP_80,	88,		1191,0xff},
        {0xaa,cal_file::TX_FILTER_OFFSET_IO_80,	88,		1191,0xff},
        {0xaa,cal_file::RX_FILTER_OFFSET_OP_80,	104,	1181,0xff},
        {0xaa,cal_file::RX_FILTER_OFFSET_IO_80,	104,	1181,0xff}
    };
    file_info_v0005_t info;
    item_info_v0005_t *item_info = info.item_info;
    CFO_ASSERT(fp_old,fread(&info,sizeof(info),1,fp_old) == 0);
    info.item_info = item_info;
    CFO_ASSERT(fp_new,fwrite(&info,sizeof(info),1,fp_new) == 0);
    CFO_ASSERT(fp_new,fflush(fp_new) == EOF);

    for (uint32_t i = 0;i < info.items;i ++) {
        CFO_ASSERT(fp_old,fread(&(info.item_info[i]),sizeof(item_info_v0005_t),1,fp_old) == 0);
        CFO_ASSERT(fp_new,fwrite(&(actual_item_info_r1cd[i]),sizeof(item_info_v0005_t),1,fp_new) == 0);
        CFO_ASSERT(fp_new,fflush(fp_new) == EOF);
    }

    uint32_t max_byte = cal_file::get_max_item_byte();
    char *buf = new char[max_byte / sizeof(char)];

    for (uint32_t cur_item_idx = 0;cur_item_idx < info.items;cur_item_idx ++) {
        CFO_ASSERT(fp_old,fread(buf,info.item_info[cur_item_idx].size * info.item_info[cur_item_idx].pt,1,fp_old) == 0);
        switch (info.item_info[cur_item_idx].item) {
            case cal_file::TX_LOL : {
                data_f_v0005_tx_lol_t *data_tx_lol_v0005 = (data_f_v0005_tx_lol_t *)buf;
                data_f_v0006_tx_lol_t data_tx_lol_v0006;
                for (uint32_t i = 0;i < info.item_info[cur_item_idx].pt;i ++) {
                    data_tx_lol_v0006.freq	 = data_tx_lol_v0005[i].freq;
                    data_tx_lol_v0006.dc_i	 = data_tx_lol_v0005[i].dc_i;
                    data_tx_lol_v0006.dc_q   = data_tx_lol_v0005[i].dc_q;
                    data_tx_lol_v0006.use_sa = data_tx_lol_v0005[i].use_sa;
                    data_tx_lol_v0006.pwr	 = data_tx_lol_v0005[i].pwr;
                    data_tx_lol_v0006.time	 = data_tx_lol_v0005[i].time._2tm();
                    for (int j = 0;j < 4;j ++)
                        data_tx_lol_v0006.temp[j] = data_tx_lol_v0005[i].temp[j];
                    CFO_ASSERT(fp_new,fwrite(&data_tx_lol_v0006,sizeof(data_tx_lol_v0006),1,fp_new) == 0);
                    CFO_ASSERT(fp_new,fflush(fp_new) == EOF);
                }
                break;
                                         }
            case cal_file::TX_SB : {
                data_f_v0005_tx_sb_t *data_tx_sb_v0005 = (data_f_v0005_tx_sb_t *)buf;
                data_f_v0006_tx_sb_t data_tx_sb_v0006;
                for (uint32_t i = 0;i < info.item_info[cur_item_idx].pt;i ++) {
                    data_tx_sb_v0006.freq   = data_tx_sb_v0005[i].freq;
                    data_tx_sb_v0006.th     = data_tx_sb_v0005[i].th;
                    data_tx_sb_v0006.am_i   = data_tx_sb_v0005[i].am_i;
                    data_tx_sb_v0006.am_q   = data_tx_sb_v0005[i].am_q;
                    data_tx_sb_v0006.use_sa = data_tx_sb_v0005[i].use_sa;
                    data_tx_sb_v0006.pwr    = data_tx_sb_v0005[i].pwr;
                    data_tx_sb_v0006.time   = data_tx_sb_v0005[i].time._2tm();
                    for (int j = 0;j < 4;j ++)
                        data_tx_sb_v0006.temp[j] = data_tx_sb_v0005[i].temp[j];
                    CFO_ASSERT(fp_new,fwrite(&data_tx_sb_v0006,sizeof(data_tx_sb_v0006),1,fp_new) == 0);
                    CFO_ASSERT(fp_new,fflush(fp_new) == EOF);
                }
                break;
                                        }
            case cal_file::TX_PWR_OP :
            case cal_file::TX_PWR_IO : {
                data_f_v0005_tx_pwr_t *data_tx_pwr_v0005 = (data_f_v0005_tx_pwr_t *)buf;
                data_f_v0006_tx_pwr_t data_tx_pwr_v0006;
                for (uint32_t i = 0;i < info.item_info[cur_item_idx].pt;i ++) {
                    data_tx_pwr_v0006.freq   = data_tx_pwr_v0005[i].freq;
                    data_tx_pwr_v0006.d_gain = data_tx_pwr_v0005[i].d_gain;
                    data_tx_pwr_v0006.att0   = data_tx_pwr_v0005[i].att0;
                    data_tx_pwr_v0006.att1   = data_tx_pwr_v0005[i].att1;
                    data_tx_pwr_v0006.att2   = data_tx_pwr_v0005[i].att2;
                    data_tx_pwr_v0006.att3   = data_tx_pwr_v0005[i].att3;
                    data_tx_pwr_v0006.time   = data_tx_pwr_v0005[i].time._2tm();
                    for (int j = 0;j < 4;j ++)
                        data_tx_pwr_v0006.temp[j] = data_tx_pwr_v0005[i].temp[j];
                    CFO_ASSERT(fp_new,fwrite(&data_tx_pwr_v0006,sizeof(data_tx_pwr_v0006),1,fp_new) == 0);
                    CFO_ASSERT(fp_new,fflush(fp_new) == EOF);
                }
                break;
                                       }
            case cal_file::TX_ATT_OP : {
                data_f_v0005_tx_att_op_t *data_tx_att_op_v0005 = (data_f_v0005_tx_att_op_t *)buf;
                data_f_v0006_tx_att_op_t data_tx_att_op_v0006;
                for (uint32_t i = 0;i < info.item_info[cur_item_idx].pt;i ++) {
                    data_tx_att_op_v0006.freq = data_tx_att_op_v0005[i].freq;
                    data_tx_att_op_v0006.time = data_tx_att_op_v0005[i].time._2tm();
                    for (uint32_t j = 0;j < R1C_TX_ATT_OP_POWER_PTS;j ++) {
                        data_tx_att_op_v0006.offset[j] = data_tx_att_op_v0005[i].offset[j];
                    }
                    for (uint32_t j = 0;j < 4;j ++) {
                        data_tx_att_op_v0006.temp[j] = data_tx_att_op_v0005[i].temp[j];
                    }
                    CFO_ASSERT(fp_new,fwrite(&data_tx_att_op_v0006,sizeof(data_tx_att_op_v0006),1,fp_new) == 0);
                    CFO_ASSERT(fp_new,fflush(fp_new) == EOF);
                }
                break;
                                     }
            case cal_file::TX_ATT_IO : {
                data_f_v0005_tx_att_io_t *data_tx_att_io_v0005 = (data_f_v0005_tx_att_io_t *)buf;
                data_f_v0006_tx_att_io_t data_tx_att_io_v0006;
                for (uint32_t i = 0;i < info.item_info[cur_item_idx].pt;i ++) {
                    data_tx_att_io_v0006.freq = data_tx_att_io_v0005[i].freq;
                    data_tx_att_io_v0006.time = data_tx_att_io_v0005[i].time._2tm();
                    for (uint32_t j = 0;j < R1C_TX_ATT_IO_POWER_PTS;j ++) {
                        data_tx_att_io_v0006.offset[j] = data_tx_att_io_v0005[i].offset[j];
                    }
                    for (uint32_t j = 0;j < 4;j ++) {
                        data_tx_att_io_v0006.temp[j] = data_tx_att_io_v0005[i].temp[j];
                    }
                    CFO_ASSERT(fp_new,fwrite(&data_tx_att_io_v0006,sizeof(data_tx_att_io_v0006),1,fp_new) == 0);
                    CFO_ASSERT(fp_new,fflush(fp_new) == EOF);
                }
                break;
                                     }
            case cal_file::RX_REF_OP :
            case cal_file::RX_REF_IO : {
                data_f_v0005_rx_ref_t *data_rx_ref_v0005 = (data_f_v0005_rx_ref_t *)buf;
                data_f_v0006_rx_ref_t data_rx_ref_v0006;
                for (uint32_t i = 0;i < info.item_info[cur_item_idx].pt;i ++) {
                    data_rx_ref_v0006.freq  = data_rx_ref_v0005[i].freq;
                    data_rx_ref_v0006.time = data_rx_ref_v0005[i].time._2tm();
                    for (int j = 0;j < 3;j ++) {
                        data_rx_ref_v0006.state[j].lna_att	 = data_rx_ref_v0005[i].state[j].lna_att;
                        data_rx_ref_v0006.state[j].att_019	 = data_rx_ref_v0005[i].state[j].att_019;
                        data_rx_ref_v0006.state[j].att1      = data_rx_ref_v0005[i].state[j].att1;
                        data_rx_ref_v0006.state[j].att2      = data_rx_ref_v0005[i].state[j].att2;
                        data_rx_ref_v0006.state[j].att3      = data_rx_ref_v0005[i].state[j].att3;
                        data_rx_ref_v0006.state[j].ad_offset = data_rx_ref_v0005[i].state[j].ad_offset;
                        data_rx_ref_v0006.state[j].temp[0]   = data_rx_ref_v0005[i].state[j].temp[0];
                        data_rx_ref_v0006.state[j].temp[1]	 = data_rx_ref_v0005[i].state[j].temp[1];
                        data_rx_ref_v0006.state[j].temp[2]	 = data_rx_ref_v0005[i].state[j].temp[2];
                        data_rx_ref_v0006.state[j].temp[3]	 = data_rx_ref_v0005[i].state[j].temp[3];
                    }
                    CFO_ASSERT(fp_new,fwrite(&data_rx_ref_v0006,sizeof(data_rx_ref_v0006),1,fp_new) == 0);
                    CFO_ASSERT(fp_new,fflush(fp_new) == EOF);
                }
                break;
                                     }
            case cal_file::RX_ATT_OP : {
                data_f_v0005_rx_att_op_t *data_rx_att_op_v0005 = (data_f_v0005_rx_att_op_t *)buf;
                data_f_v0006_rx_att_op_t data_rx_att_op_v0006;
                for (uint32_t i = 0;i < info.item_info[cur_item_idx].pt;i ++) {
                    data_rx_att_op_v0006.freq = data_rx_att_op_v0005[i].freq;
                    data_rx_att_op_v0006.time = data_rx_att_op_v0005[i].time._2tm();
                    for (uint32_t j = 0;j < R1C_RX_REF_OP_PTS - 3 + 1;j ++) {
                        data_rx_att_op_v0006.offset[j] = data_rx_att_op_v0005[i].offset[j];
                    }
                    for (uint32_t j = 0;j < 4;j ++) {
                        data_rx_att_op_v0006.temp[j] = data_rx_att_op_v0005[i].temp[j];
                    }
                    CFO_ASSERT(fp_new,fwrite(&data_rx_att_op_v0006,sizeof(data_rx_att_op_v0006),1,fp_new) == 0);
                    CFO_ASSERT(fp_new,fflush(fp_new) == EOF);
                }
                break;
                                     }
            case cal_file::RX_ATT_IO : {
                data_f_v0005_rx_att_io_t *data_rx_att_io_v0005 = (data_f_v0005_rx_att_io_t *)buf;
                data_f_v0006_rx_att_io_t data_rx_att_io_v0006;
                for (uint32_t i = 0;i < info.item_info[cur_item_idx].pt;i ++) {
                    data_rx_att_io_v0006.freq = data_rx_att_io_v0005[i].freq;
                    data_rx_att_io_v0006.time = data_rx_att_io_v0005[i].time._2tm();
                    for (uint32_t j = 0;j < R1C_RX_REF_IO_PTS - 3 + 1;j ++) {
                        data_rx_att_io_v0006.offset[j] = data_rx_att_io_v0005[i].offset[j];
                    }
                    for (uint32_t j = 0;j < 4;j ++) {
                        data_rx_att_io_v0006.temp[j] = data_rx_att_io_v0005[i].temp[j];
                    }
                    CFO_ASSERT(fp_new,fwrite(&data_rx_att_io_v0006,sizeof(data_rx_att_io_v0006),1,fp_new) == 0);
                    CFO_ASSERT(fp_new,fflush(fp_new) == EOF);
                }
                break;
                                     }
            case cal_file::TX_FILTER_80 :
            case cal_file::TX_FILTER_160 : {
                data_f_v0005_tx_filter_t *data_tx_filter_v0005 = (data_f_v0005_tx_filter_t *)buf;
                data_f_v0006_tx_filter_t data_tx_filter_v0006;
                for (uint32_t i = 0;i < info.item_info[cur_item_idx].pt;i ++) {
                    data_tx_filter_v0006.freq = data_tx_filter_v0005[i].freq;
                    data_tx_filter_v0006.time = data_tx_filter_v0005[i].time._2tm();
                    for (int j = 0;j < 41;j ++) {
                        data_tx_filter_v0006.coef_real[j] = data_tx_filter_v0005[i].coef_real[j];
                        data_tx_filter_v0006.coef_imag[j] = data_tx_filter_v0005[i].coef_imag[j];
                    }
                    for (int j = 0;j < 4;j ++)
                        data_tx_filter_v0006.temp[j] = data_tx_filter_v0005[i].temp[j];
                    CFO_ASSERT(fp_new,fwrite(&data_tx_filter_v0006,sizeof(data_tx_filter_v0006),1,fp_new) == 0);
                    CFO_ASSERT(fp_new,fflush(fp_new) == EOF);
                }
                break;
                                       }
            case cal_file::RX_FILTER_80 : {
                data_f_v0005_rx_filter_80 *data_rx_filter_80_v0005 = (data_f_v0005_rx_filter_80 *)buf;
                data_f_v0006_rx_filter_80 data_rx_filter_80_v0006;
                for (uint32_t i = 0;i < info.item_info[cur_item_idx].pt;i ++) {
                    data_rx_filter_80_v0006.freq = data_rx_filter_80_v0005[i].freq;
                    data_rx_filter_80_v0006.time = data_rx_filter_80_v0005[i].time._2tm();
                    for (int j = 0;j < 96;j ++) {
                        data_rx_filter_80_v0006.coef_real[j] = data_rx_filter_80_v0005[i].coef_real[j];
                        data_rx_filter_80_v0006.coef_imag[j] = data_rx_filter_80_v0005[i].coef_imag[j];
                    }
                    for (int j = 0;j < 4;j ++)
                        data_rx_filter_80_v0006.temp[j] = data_rx_filter_80_v0005[i].temp[j];
                    CFO_ASSERT(fp_new,fwrite(&data_rx_filter_80_v0006,sizeof(data_rx_filter_80_v0006),1,fp_new) == 0);
                    CFO_ASSERT(fp_new,fflush(fp_new) == EOF);
                }
                break;
                                      }
            case cal_file::RX_FILTER_160 : {
                data_f_v0005_rx_filter_160 *data_rx_filter_160_v0005 = (data_f_v0005_rx_filter_160 *)buf;
                data_f_v0006_rx_filter_160 data_rx_filter_160_v0006;
                for (uint32_t i = 0;i < info.item_info[cur_item_idx].pt;i ++) {
                    data_rx_filter_160_v0006.freq = data_rx_filter_160_v0005[i].freq;
                    data_rx_filter_160_v0006.time = data_rx_filter_160_v0005[i].time._2tm();
                    for (int j = 0;j < 144;j ++) {
                        data_rx_filter_160_v0006.coef_real[j] = data_rx_filter_160_v0005[i].coef_real[j];
                        data_rx_filter_160_v0006.coef_imag[j] = data_rx_filter_160_v0005[i].coef_imag[j];
                    }
                    for (int j = 0;j < 4;j ++)
                        data_rx_filter_160_v0006.temp[j] = data_rx_filter_160_v0005[i].temp[j];
                    CFO_ASSERT(fp_new,fwrite(&data_rx_filter_160_v0006,sizeof(data_rx_filter_160_v0006),1,fp_new) == 0);
                    CFO_ASSERT(fp_new,fflush(fp_new) == EOF);
                }
                break;
                                       }
            case cal_file::TX_FILTER_OFFSET_OP_80 :
            case cal_file::TX_FILTER_OFFSET_IO_80 : {
                data_f_v0005_tx_filter_offset_t *data_tx_filter_offset_v0005 = (data_f_v0005_tx_filter_offset_t *)buf;
                data_f_v0006_tx_filter_offset_t data_tx_filter_offset_v0006;
                for (uint32_t i = 0;i < info.item_info[cur_item_idx].pt;i ++) {
                    data_tx_filter_offset_v0006.freq   = data_tx_filter_offset_v0005[i].freq;
                    data_tx_filter_offset_v0006.offset = data_tx_filter_offset_v0005[i].offset;
                    data_tx_filter_offset_v0006.time   = data_tx_filter_offset_v0005[i].time._2tm();
                    for (int j = 0;j < 4;j ++)
                        data_tx_filter_offset_v0006.temp[j] = data_tx_filter_offset_v0005[i].temp[j];
                    CFO_ASSERT(fp_new,fwrite(&data_tx_filter_offset_v0006,sizeof(data_tx_filter_offset_v0006),1,fp_new) == 0);
                    CFO_ASSERT(fp_new,fflush(fp_new) == EOF);
                }
                break;
                                           }
            case cal_file::RX_FILTER_OFFSET_OP_80 :
            case cal_file::RX_FILTER_OFFSET_IO_80 : {
                data_f_v0005_rx_filter_offset_t *data_f_rx_filter_offset_v0005 = (data_f_v0005_rx_filter_offset_t *)buf;
                data_f_v0006_rx_filter_offset_t data_f_rx_filter_offset_v0006;
                for (uint32_t i = 0;i < info.item_info[cur_item_idx].pt;i ++) {
                    data_f_rx_filter_offset_v0006.freq = data_f_rx_filter_offset_v0005[i].freq;
                    data_f_rx_filter_offset_v0006.time = data_f_rx_filter_offset_v0005[i].time._2tm();
                    for (int j = 0;j < 3;j ++)
                        data_f_rx_filter_offset_v0006.offset[j] = data_f_rx_filter_offset_v0005[i].offset[j];
                    for (int j = 0;j < 4;j ++)
                        data_f_rx_filter_offset_v0006.temp[j] = data_f_rx_filter_offset_v0005[i].temp[j];
                    CFO_ASSERT(fp_new,fwrite(&data_f_rx_filter_offset_v0006,sizeof(data_f_rx_filter_offset_v0006),1,fp_new) == 0);
                    CFO_ASSERT(fp_new,fflush(fp_new) == EOF);
                }
                break;
                                           }
            case cal_file::TX_RF_FR_0 :
            case cal_file::TX_RF_FR_1 :
            case cal_file::TX_IF_FR	:
            case cal_file::RX_RF_FR	:
            case cal_file::RX_IF_FR	: {
                CFO_ASSERT(fp_new,fwrite(buf,info.item_info[cur_item_idx].size * info.item_info[cur_item_idx].pt,1,fp_new) == 0);
                CFO_ASSERT(fp_new,fflush(fp_new) == EOF);
                break;
                                    }
            case cal_file::RX_REF :
            case cal_file::X9119 : { break; }
            default: break;
        }
    }
    CFO_ASSERT(fp_old,fseek(fp_old,0,SEEK_SET));
    fclose(fp_old);
    CFO_ASSERT(fp_new,fseek(fp_new,0,SEEK_SET));
    fclose(fp_new);

    remove(path_old);
    rename(path_new,path_old);

    delete []buf;
    Log.stdprintf("file \"%s\" version 0x0005 ---> 0x0006\n",path_old);
    Log.add_msg(0,"file \"%s\" version 0x0005 ---> 0x0006",path_old);
    return 0;
}

#endif // CFV_CTRL_0005_0006_HPP
