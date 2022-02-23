#include "cal_file_r1ab.h"
#include <string.h>
#include <errno.h>

#ifdef PLATFORM_WIN32
    #include <io.h>
    #include <direct.h>
#elif defined PLATFORM_LINUX
    #include <sys/stat.h>
    #include <unistd.h>
#endif

using namespace rd;
using namespace ns_sp1401;
using namespace ns_sp1401::r1a;

typedef int32_t (cal_file_r1ab::*get_item_size_vxxxx)(cal_file::cal_item_t,uint32_t &,uint32_t &);

cal_file_r1ab::cal_file_r1ab(hw_ver_t ver, uint32_t rf_idx, uint32_t rfu_idx) :
    cal_file(ver,rf_idx,rfu_idx),
    m_tx_sb(nullptr),
    m_tx_lol(nullptr),
    m_tx_att_op(nullptr),
    m_tx_pwr_op(nullptr),
    m_tx_pwr_io(nullptr),
    m_rx_ref(nullptr),
    m_tx_att_io(nullptr)
{
}

cal_file_r1ab::~cal_file_r1ab()
{
}

int32_t cal_file_r1ab::open()
{
    BOOST_SPTR_SAFE_MAKE(x9119_table,g_cal_table_x9119);
    BOOST_SPTR_SAFE_MAKE(tx_sb_table_r1ab,m_tx_sb);
    BOOST_SPTR_SAFE_MAKE(tx_lol_table_r1ab,m_tx_lol);
    BOOST_SPTR_SAFE_MAKE(tx_att_table_r1ab,m_tx_att_op);
    BOOST_SPTR_SAFE_MAKE(tx_pwr_table_r1ab,m_tx_pwr_op);
    BOOST_SPTR_SAFE_MAKE(tx_pwr_table_r1ab,m_tx_pwr_io);
    BOOST_SPTR_SAFE_MAKE(rx_ref_table_r1ab,m_rx_ref);
    BOOST_SPTR_SAFE_MAKE(tx_att_table_r1ab,m_tx_att_io);
    return cal_file::open();
}

int32_t cal_file_r1ab::map2buf(cal_item_t item)
{
    if (nullptr == g_cal_item_buf.buf) {
        cal_file::file_info_t info;
        uint32_t max_byte = cal_file::get_max_item_byte();

        g_cal_item_buf.buf = new char[max_byte / sizeof(char)];
        if (nullptr == g_cal_item_buf.buf) {
            Log.set_last_err("%s:%s:%d",__FILE__,__FUNCTION__,__LINE__);
			return -1;
		}
        memset(g_cal_item_buf.buf,0,max_byte);
	}

    if (item != g_cal_item_buf.item || this->m_rf_idx != g_cal_item_buf.rf_idx || this->m_rfu_idx != g_cal_item_buf.rfu_idx) {
        INT_CHECK(r(item,g_cal_item_buf.buf));
        g_cal_item_buf.item = item;
        g_cal_item_buf.rf_idx = this->m_rf_idx;
        g_cal_item_buf.rfu_idx = this->m_rfu_idx;
        switch (item) {
            case X9119     : MAP_GLOBAL_DATAF(g_cal_table_x9119,x9119_table::data_t);
            case TX_SB     : MAP_GLOBAL_DATAF(m_tx_sb,tx_sb_table_r1ab::data_f_t);
            case TX_LOL    : MAP_GLOBAL_DATAF(m_tx_lol,tx_lol_table_r1ab::data_f_t);
            case TX_ATT_OP : MAP_GLOBAL_DATAF(m_tx_att_op,tx_att_table_r1ab::data_f_t);
            case TX_PWR_OP : MAP_GLOBAL_DATAF(m_tx_pwr_op,tx_pwr_table_r1ab::data_f_t);
            case TX_PWR_IO : MAP_GLOBAL_DATAF(m_tx_pwr_io,tx_pwr_table_r1ab::data_f_t);
            case RX_REF    : MAP_GLOBAL_DATAF(m_rx_ref,rx_ref_table_r1ab::data_f_t);
            case TX_ATT_IO : MAP_GLOBAL_DATAF(m_tx_att_io,tx_att_table_r1ab::data_f_t);
			default:return 0;
		}
	}
	return 0;
}

int32_t cal_file_r1ab::map2mem()
{
    MAP_FILE_DATAM(X9119,g_cal_table_x9119);
    MAP_FILE_DATAM(TX_SB,m_tx_sb);
    MAP_FILE_DATAM(TX_LOL,m_tx_lol);
    MAP_FILE_DATAM(TX_ATT_OP,m_tx_att_op);
    MAP_FILE_DATAM(TX_PWR_OP,m_tx_pwr_op);
    MAP_FILE_DATAM(TX_PWR_IO,m_tx_pwr_io);
    MAP_FILE_DATAM(RX_REF,m_rx_ref);
    MAP_FILE_DATAM(TX_ATT_IO,m_tx_att_io);
    return 0;
}

int32_t cal_file_r1ab::add(cal_item_t item,void *data)
{
    map2buf(item);
    switch (item) {
        case X9119     : ADD_DATAF(g_cal_table_x9119,x9119_table::data_t);
        case TX_SB     : ADD_DATAF(m_tx_sb,tx_sb_table_r1ab::data_f_t);
        case TX_LOL    : ADD_DATAF(m_tx_lol,tx_lol_table_r1ab::data_f_t);
        case TX_ATT_OP : ADD_DATAF(m_tx_att_op,tx_att_table_r1ab::data_f_t);
        case TX_PWR_OP : ADD_DATAF(m_tx_pwr_op,tx_pwr_table_r1ab::data_f_t);
        case TX_PWR_IO : ADD_DATAF(m_tx_pwr_io,tx_pwr_table_r1ab::data_f_t);
        case RX_REF    : ADD_DATAF(m_rx_ref,rx_ref_table_r1ab::data_f_t);
        case TX_ATT_IO : ADD_DATAF(m_tx_att_io,tx_att_table_r1ab::data_f_t);
		default:return 0;
	}
	return 0;
}

int32_t cal_file_r1ab::create()
{
    cal_file::file_ver_t ver;
    cal_file::file_info_t info;
    uint32_t item_idx = 0;
    uint32_t max_byte = 0;
    char path[64] = {0};
    sprintf(path,"C:\\CSECal\\rfu%drf%d.cal",m_rfu_idx,m_rf_idx);

    ver.head = CAL_FILE_VER_HEAD;
    ver.ver  = CAL_FILE_VER > 4 ? 4 : CAL_FILE_VER;	//r1a:ver0---4,r1c:ver5---
    ver.tail = CAL_FILE_VER_TAIL;

    info.head = CAL_FILE_INFO_HEAD;
    info.tail = CAL_FILE_INFO_TAIL;
    memset(info.sn,0,sizeof(info.sn));
    sprintf(info.sn,"SP1401R1B%08dRFU%uRF%u",m_ordinal,m_rfu_idx,m_rf_idx);

    max_byte = get_fix_item_table_r1ab(info.item_info,info.items);
    info.items -= 1;
    for (uint32_t i = 0;i < info.items;i ++) {
        if (cal_file::X9119 == i) {
            item_idx ++;
        }
        info.item_info[i] = info.item_info[item_idx];
        item_idx ++;
	}

    FILE *fp;
    if (access("C:\\CSECal",0) == -1) {
#ifdef PLATFORM_WIN32
        if (mkdir("C:\\CSECal") == -1) {
#elif defined PLATFORM_LINUX
        if (mkdir("/var/lib/CSECal",0777) == -1) {
#endif
            Log.set_last_err("%s:%s:%d(%s)",__FILE__,__FUNCTION__,__LINE__,strerror(errno));
            return -1;
        }
    }
    if (access("C:\\CSECal\\cxu.cal",0) == -1) {
        x9119_table::data_t data;
        data.value = 0;

        fp = fopen("C:\\CSECal\\cxu.cal","w+");
        CFO_ASSERT(fp,nullptr == fp);
        CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));
        CFO_ASSERT(fp,fwrite(&data,sizeof(x9119_table::data_t),1,fp) == 0);
        CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));
        fclose(fp);
	}
    fp = fopen(path,"w+");
    CFO_ASSERT(fp,nullptr == fp);
    CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));
    CFO_ASSERT(fp,fwrite(&ver,sizeof(cal_file::file_ver_t),1,fp) == 0);
    CFO_ASSERT(fp,fwrite(&info,sizeof(cal_file::file_info_t),1,fp) == 0);
    for (uint32_t i = 0;i < info.items;i ++) {
        CFO_ASSERT(fp,fwrite(&(info.item_info[i]),sizeof(cal_file::item_info_t),1,fp) == 0);
    }

    unsigned char *buf = new unsigned char[max_byte];
    for (uint32_t i = 0;i < info.items;i ++) {
        uint32_t size = ((info.item_info[i]).size) * ((info.item_info[i]).pt);
        memset(buf,0,max_byte * sizeof(unsigned char));
        CFO_ASSERT(fp,fwrite(buf,1,size,fp) == 0);
	}
    delete []buf;
    CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));
    fclose(fp);
	return 0;
}

int32_t cal_file_r1ab::load(file_info_t &info)
{
    file_ver_t ver;
    char path[64] = {0};
    sprintf(path,"C:\\CSECal\\rfu%drf%d.cal",m_rfu_idx,m_rf_idx);

    FILE *fp = fopen(path,"rb");
    CFO_ASSERT(fp,nullptr == fp);
    CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));
    CFO_ASSERT(fp,fread(&ver,sizeof(file_ver_t),1,fp) == 0);
    show_file_ver(&ver);

    item_info_t *item_info = info.item_info;
    CFO_ASSERT(fp,fread(&info,sizeof(file_info_t),1,fp) == 0);
    info.item_info = item_info;
    show_file_info(&info);

    for (uint32_t i = 0;i < info.items;i ++) {
        CFO_ASSERT(fp,fread(&(info.item_info[i]),sizeof(item_info_t),1,fp) == 0);
        show_item_info(&(info.item_info[i]));
	}
    CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));
    fclose(fp);
	return 0;
}

int32_t cal_file_r1ab::get_item_size(cal_item_t item,uint32_t &pos,uint32_t &size)
{
    get_item_size_vxxxx t_get_item_size[] = {
        &cal_file_r1ab::get_item_size_v0000,
        &cal_file_r1ab::get_item_size_v0001,
        &cal_file_r1ab::get_item_size_v0002,
        &cal_file_r1ab::get_item_size_v0002,
        &cal_file_r1ab::get_item_size_v0004,
        &cal_file_r1ab::get_item_size_v0004,
        &cal_file_r1ab::get_item_size_v0004
    };

    return (this->*t_get_item_size[CAL_FILE_VER])(item,pos,size);
}

int32_t cal_file_r1ab::get_item_size_v0000(cal_file::cal_item_t item,uint32_t &pos,uint32_t &size)
{
    cal_file::file_info_t info;
    INT_CHECK(load(info));

    int32_t i_item = int32_t(item);

    size = ((info.item_info[i_item]).size) * ((info.item_info[i_item]).pt);

    pos = 0;
    pos += sizeof(cal_file::file_info_t);
    for (uint32_t i = 0;i < info.items;i ++) {
        pos += sizeof(cal_file::item_info_t);
    }
    for (int32_t i = 0;i < i_item;i ++) {
        pos += ((info.item_info[i]).size) * ((info.item_info[i]).pt);
    }
	return 0;
}

int32_t cal_file_r1ab::get_item_size_v0001(cal_file::cal_item_t item,uint32_t &pos,uint32_t &size)
{
    cal_file::file_info_t info;
    INT_CHECK(load(info));

    int32_t i_item = int32_t(item);
    size = ((info.item_info[i_item]).size) * ((info.item_info[i_item]).pt);

    pos = 0;
    pos += sizeof(cal_file::file_ver_t);
    pos += sizeof(cal_file::file_info_t);
    for (uint32_t i = 0;i < info.items;i ++) {
        pos += sizeof(cal_file::item_info_t);
    }
    for (int32_t i = 0;i < i_item;i ++) {
        pos += ((info.item_info[i]).size) * ((info.item_info[i]).pt);
    }
	return 0;
}

int32_t cal_file_r1ab::get_item_size_v0002(cal_file::cal_item_t item,uint32_t &pos,uint32_t &size)
{
    cal_file::file_info_t info;
    INT_CHECK(load(info));

    int32_t i_item = int32_t(item);

    if (cal_file::X9119 == item) {
        size = sizeof(x9119_table::data_t) * 1;
        pos = 0;
		return 0;
	}

    size = ((info.item_info[i_item]).size) * ((info.item_info[i_item]).pt);

    pos = 0;
    pos += sizeof(cal_file::file_ver_t);
    pos += sizeof(cal_file::file_info_t);
    for (uint32_t i = 0;i < info.items;i ++) {
        pos += sizeof(cal_file::item_info_t);
    }
    for (int32_t i = 0;i < i_item;i ++) {
        pos += ((info.item_info[i]).size) * ((info.item_info[i]).pt);
    }
	return 0;
}

int32_t cal_file_r1ab::get_item_size_v0004(cal_file::cal_item_t item,uint32_t &pos,uint32_t &size)
{
    cal_file::file_info_t info;
    INT_CHECK(load(info));

    if (cal_file::X9119 == item) {
        size = sizeof(x9119_table::data_t) * 1;
        pos = 0;
		return 0;
	}

    int32_t item_idx = int32_t(item > cal_file::X9119 ? (item - 1) : item);
    size = ((info.item_info[item_idx]).size) * ((info.item_info[item_idx]).pt);
    pos = 0;
    pos += sizeof(cal_file::file_ver_t);
    pos += sizeof(cal_file::file_info_t);
    for (uint32_t i = 0;i < info.items;i ++) {
        pos += sizeof(cal_file::item_info_t);
    }
    for (int32_t i = 0;i < item_idx;i ++) {
        pos += ((info.item_info[i]).size) * ((info.item_info[i]).pt);
    }
	return 0;
}

void cal_file_r1ab::get_tx_att(uint64_t freq,io_mode_t mode,float *att)
{
    if (mode._to_integral() == io_mode_t::OUTPUT) {
        m_tx_att_op->get(freq,att);
    }
    if (mode._to_integral() == io_mode_t::IO) {
        m_tx_att_io->get(freq,att);
    }
}

void cal_file_r1ab::get_tx_att(uint64_t freq,io_mode_t mode,int32_t att,double &offset)
{
    if (mode._to_integral() == io_mode_t::OUTPUT) {
        m_tx_att_op->get(freq,att,offset);
    }
    if (mode._to_integral() == io_mode_t::IO) {
        m_tx_att_io->get(freq,att,offset);
    }
}

void cal_file_r1ab::get_tx_pwr(uint64_t freq,double pwr,io_mode_t mode,double &d_gain,int32_t &att1,int32_t &att2,tx_pa_att_t &pa_att,int32_t &att3)
{
    int32_t i_pa_att = 1;
    if (mode._to_integral() == io_mode_t::OUTPUT) {
        m_tx_pwr_op->get_op(freq,pwr,d_gain,att1,att2,i_pa_att,att3);
    }
    if (mode._to_integral() == io_mode_t::IO) {
        m_tx_pwr_io->get_io(freq,pwr,d_gain,att1,att2,i_pa_att,att3);
    }
    pa_att = (0 == i_pa_att ? TX_PA : TX_ATT);
}
