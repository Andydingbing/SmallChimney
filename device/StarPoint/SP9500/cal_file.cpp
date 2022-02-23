#include "cal_file.h"
#include "cfv_ctrl.h"
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "sp1401.h"

using namespace std;
using namespace rd;
using namespace rd::ns_sp9500;
using namespace ns_sp1401;
using namespace rd::ns_sp2401;

boost::shared_ptr<x9119_table> cal_file::g_cal_table_x9119 = nullptr;
cal_file::item_buf_t cal_file::g_cal_item_buf = cal_file::item_buf_t();

x9119_table* cal_file::x9119()
{
     return g_cal_table_x9119.get();
}

cal_file::item_buf_t::item_buf_t()
{
    rf_idx = g_max_rf + 1;
    rfu_idx = g_max_rfu + 1;
    buf = nullptr;
    item = cal_item_t(-1);
}

cal_file::item_buf_t::~item_buf_t()
{
    if (buf != nullptr) {
        delete []((char *)buf);
        buf = nullptr;
    }
}

cal_file::cal_file(hw_ver_t ver, uint32_t rf_idx, uint32_t rfu_idx)
{
    m_hw_ver = ver;
    m_rf_idx = rf_idx;
    m_rfu_idx = rfu_idx;
    m_ordinal = 0;
    m_bw = _160M;
#ifdef RD_C_MSC
	_set_fmode(_O_BINARY);
#endif
}

cal_file::~cal_file()
{
}

uint32_t cal_file::get_fix_item_table_r1ab(item_info_t *info,uint32_t &items)
{
    item_info_t item_info[] = {
        {0xaa,TX_SB,     sizeof(tx_sb_table_r1ab::data_f_t)  ,tx_freqs_infile,0xff},
        {0xaa,TX_LOL,    sizeof(tx_lol_table_r1ab::data_f_t) ,tx_freqs_infile,0xff},
        {0xaa,TX_ATT_OP, sizeof(tx_att_table_r1ab::data_f_t) ,tx_freqs_infile,0xff},
        {0xaa,TX_PWR_OP, sizeof(tx_pwr_table_r1ab::data_f_t) ,tx_freqs_infile,0xff},
        {0xaa,TX_PWR_IO, sizeof(tx_pwr_table_r1ab::data_f_t) ,tx_freqs_infile,0xff},
        {0xaa,RX_REF,    sizeof(rx_ref_table_r1ab::data_f_t) ,rx_freqs_infile,0xff},
        {0xaa,X9119,     sizeof(x9119_table::data_t)		 ,1              ,0xff},
        {0xaa,TX_ATT_IO, sizeof(tx_att_table_r1ab::data_f_t) ,tx_freqs_infile,0xff}
	};
    items = ARRAY_SIZE(item_info);
    for (uint32_t i = 0;i < items;i ++) {
        info[i] = item_info[i];
    }

    return get_max_item_byte(info,items);
}

uint32_t cal_file::get_fix_item_table_r1cd(item_info_t *info,uint32_t &items)
{
    item_info_t item_info[] = {
        {0xaa,X9119,                  sizeof(x9119_table::data_t),              1,               0xff},
        {0xaa,TX_LOL,                 sizeof(tx_lol_table_r1cd::data_f_t),      1,               0xff},
        {0xaa,TX_SB,                  sizeof(tx_sb_table_r1cd::data_f_t),       1,               0xff},
        {0xaa,TX_PWR_OP,              sizeof(tx_pwr_op_table_r1c::data_f_t), 	tx_freqs_infile, 0xff},
        {0xaa,TX_PWR_IO,              sizeof(tx_pwr_io_table_r1c::data_f_t),    tx_freqs_infile, 0xff},
        {0xaa,TX_ATT_OP,              sizeof(tx_att_op_table_r1cd::data_f_t),   tx_freqs_infile, 0xff},
        {0xaa,TX_ATT_IO,              sizeof(tx_att_io_table_r1cd::data_f_t),   tx_freqs_infile, 0xff},
        {0xaa,RX_REF_OP,              sizeof(rx_ref_op_table_r1cd::data_f_t),   rx_freqs_infile, 0xff},
        {0xaa,RX_REF_IO,              sizeof(rx_ref_io_table_r1cd::data_f_t),   rx_freqs_infile, 0xff},
        {0xaa,RX_ATT_OP,              sizeof(rx_att_op_table_r1cd::data_f_t),   rx_freqs_infile, 0xff},
        {0xaa,RX_ATT_IO,              sizeof(rx_att_io_table_r1cd::data_f_t),   rx_freqs_infile, 0xff},
        {0xaa,TX_FILTER_80,           sizeof(tx_filter_80m_table::data_f_t),    tx_freqs_infile, 0xff},
        {0xaa,TX_FILTER_160,          sizeof(tx_filter_160m_table::data_f_t),   tx_freqs_infile, 0xff},
        {0xaa,TX_RF_FR_0,             sizeof(tx_rf_fr_table::data_f_t),         tx_filter_freqs_0,0xff},
        {0xaa,TX_RF_FR_1,             sizeof(tx_rf_fr_table::data_f_t),         tx_filter_freqs_1,0xff},
        {0xaa,TX_IF_FR,               sizeof(tx_if_fr_table::data_f_t),         dl_filter_160M_freqs, 0xff},
        {0xaa,RX_FILTER_80,           sizeof(rx_filter_80m_table::data_f_t),    rx_freqs_infile, 0xff},
        {0xaa,RX_FILTER_160,          sizeof(rx_filter_160m_table::data_f_t),   rx_freqs_infile, 0xff},
        {0xaa,RX_RF_FR,               sizeof(rx_rf_fr_table::data_f_t),         rx_filter_freqs, 0xff},
        {0xaa,RX_IF_FR,               sizeof(rx_if_fr_table::data_f_t),         ul_filter_160M_freqs, 0xff},
        {0xaa,TX_FILTER_OFFSET_OP_80, sizeof(tx_filter_offset_table::data_f_t), tx_freqs_infile, 0xff},
        {0xaa,TX_FILTER_OFFSET_IO_80, sizeof(tx_filter_offset_table::data_f_t), tx_freqs_infile, 0xff},
        {0xaa,RX_FILTER_OFFSET_OP_80, sizeof(rx_filter_offset_table::data_f_t),	rx_freqs_infile, 0xff},
        {0xaa,RX_FILTER_OFFSET_IO_80, sizeof(rx_filter_offset_table::data_f_t),	rx_freqs_infile, 0xff}
//        {0xaa,TX_TP,                  sizeof(tx_tp_table::data_f_t),            tx_freqs_infile, 0xff},
//        {0xaa,TX_TC,                  sizeof(tx_tc_table::data_f_t),            tx_freqs_infile, 0xff},
//        {0xaa,RX_TP,                  sizeof(rx_tp_table::data_f_t),            tx_freqs_infile, 0xff}, // TX here!
//        {0xaa,RX_TC,                  sizeof(rx_tc_table::data_f_t),            tx_freqs_infile, 0xff}  // TX here!
	};
    items = ARRAY_SIZE(item_info);
    for (uint32_t i = 0;i < items;i ++) {
        info[i] = item_info[i];
    }

    return get_max_item_byte(info,items);
}

uint32_t cal_file::get_max_item_byte(item_info_t *info,uint32_t items)
{
    uint32_t max_byte = 0;
    for (uint32_t i = 0;i < items;i ++) {
        if (max_byte < info[i].size * info[i].pt) {
            max_byte = info[i].size * info[i].pt;
        }
	}
    return max_byte;
}

uint32_t cal_file::get_max_item_byte()
{
    file_info_t info;
    uint32_t max_byte_r1ab = cal_file::get_fix_item_table_r1ab(info.item_info,info.items);
    uint32_t max_byte_r1cd = cal_file::get_fix_item_table_r1cd(info.item_info,info.items);
    return max_byte_r1ab >= max_byte_r1cd ? max_byte_r1ab : max_byte_r1cd;
}

int32_t cal_file::open()
{
    if (!is_file_valid()) {
        if (create() < 0) {
			return -1;
        }
        if (!is_file_valid()) {
			return -1;
        }
	}
    map2mem();
	return 0;
}

bool cal_file::is_file_valid()
{
    cal_file::file_info_t info;
    int32_t ver = get_file_ver();

    if (ver < 0) {
        return false;
    } else if (ver < CAL_FILE_VER) {
        cfv_ctrl controller;
        controller.target(this);
        controller.for_c(ver);
	}

    if (load(info)) {
		return false;
    }

    for (uint32_t i = 0;i < info.items;i ++) {
        if ((info.item_info[i]).head != 0xaa || (info.item_info[i]).tail != 0xff) {
            Log.set_last_err("invalid data format");
			return false;
		}
	}
	return true;
}

uint32_t cal_file::get_file_ver()
{
    file_ver_t ver;

    char path[64] = {0};
    sprintf(path,"C:\\CSECal\\rfu%drf%d.cal",m_rfu_idx,m_rf_idx);

    FILE *fp = fopen(path,"rb");
    CFO_ASSERT(fp,fp == nullptr);
    CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));
    CFO_ASSERT(fp,fread(&ver,sizeof(file_ver_t),1,fp) == 0);
    CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));
    fclose(fp);

    if ((0x04ABCDEF == ver.head && 0x05ABCDEF == ver.tail) ||
        (CAL_FILE_VER_HEAD == ver.head && CAL_FILE_VER_TAIL == ver.tail)) {
        return ver.ver;
    }
    return 0;
}

int32_t cal_file::w_from_pos(const char *path,uint32_t pos,uint32_t size,void *data)
{
    size_t writed_size = 0;
    FILE *fp = fopen(path,"rb+");
    CFO_ASSERT(fp,nullptr == fp);
    CFO_ASSERT(fp,fseek(fp,pos,SEEK_SET));
    CFO_ASSERT(fp,(writed_size = fwrite(data,size,1,fp)) == 0);
    CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));
	fclose(fp);
	return 0;
}

int32_t cal_file::r_from_pos(const char *path,uint32_t pos,uint32_t size,void *data)
{
//in non posix env,the mode "b" of "rb" is must for EOF = 16,and the ASCII in file maybe happen to "EOF",
//then fread can return 0 for the os think the file has reach the EndOfFile;
//in posix env such as linux,the mode "b" can be ignored
    size_t read_size = 0;
    FILE *fp = fopen(path,"rb");
    CFO_ASSERT(fp,nullptr == fp);
    CFO_ASSERT(fp,fseek(fp,pos,SEEK_SET));
    CFO_ASSERT(fp,(read_size = fread(data,size,1,fp)) == 0);
    CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));
	fclose(fp);
    return 0;
}

int32_t cal_file::w(cal_file::cal_item_t item)
{
    uint32_t pos = 0;
    uint32_t size = 0;
    string path = (item == cal_file::X9119 ?
                "C:\\CSECal\\cxu.cal" :
                (boost::format("C:\\CSECal\\rfu%drf%d.cal") % m_rfu_idx % m_rf_idx).str());

    INT_CHECK(get_item_size(item,pos,size));
    INT_CHECK(w_from_pos(path.c_str(),pos,size,g_cal_item_buf.buf));
    map2mem();
    map2buf(item);
	return 0;	
}

int32_t cal_file::r(cal_file::cal_item_t item,void *data)
{
    uint32_t pos = 0;
    uint32_t size = 0;
    string path = (item == cal_file::X9119 ?
                "C:\\CSECal\\cxu.cal" :
                (boost::format("C:\\CSECal\\rfu%drf%d.cal") % m_rfu_idx % m_rf_idx).str());

    INT_CHECK(get_item_size(item,pos,size));
    INT_CHECK(r_from_pos(path.c_str(),pos,size,data));

    for (uint32_t i = 0;i < size;i ++) {
        ((char *)(g_cal_item_buf.buf))[i] = ((char *)data)[i];
    }
	return 0;
}

void cal_file::show_file_ver(file_ver_t *ver)
{
//	Log.stdprintf("file ver : %#06x\n",ver->ver);
}

void cal_file::show_file_info(file_info_t *info)
{
// 	Log.stdprintf("sn : %s\n",info->sn);
// 	Log.stdprintf("num of item : %d\n",info->items);
// 	Log.stdprintf("first table addr : %#x\n",info->item_info);
}

void cal_file::show_item_info(item_info_t *info)
{
// 	char szItemName[][64] = {
// 		{"Tx SideBand"},
// 		{"Tx LoLeakage"},
// 		{"Tx Att Output"},
// 		{"Tx Power Output(160M)"},
// 		{"Tx Power IO(160M)"},
// 		{"Rx Reference"},
// 		{"X9119"},
// 		{"Tx Att IO"},
// 		{"Rx Reference Output"},
// 		{"Rx Reference IO"},
// 		{"Rx Att Output"},
// 		{"Rx Att IO"},
// 		{"Tx Filter 80M"},
// 		{"Tx Filter 160M"},
// 		{"Tx RF Freq Response(<=3100)"},
//		{"Tx RF Freq Response(>3100)"},
// 		{"Tx IF Freq Response"},
// 		{"Rx Filter 80M"},
// 		{"Rx Filter 160M"},
// 		{"Rx RF Freq Response"},
// 		{"Rx IF Freq Response"},
// 		{"Tx Filter Offset Output(80M)"},
// 		{"Tx Filter Offset IO(80M)"},
// 		{"Rx Filter Offset Output(80M)"},
// 		{"Rx Filter Offset IO(80M)"}
// 	};
// 	Log.stdprintf("item name : %s\n",szItemName[info->item]);
// 	Log.stdprintf("data byte : %d\n",info->size);
// 	Log.stdprintf("points    : %d\n",info->pt);
}
