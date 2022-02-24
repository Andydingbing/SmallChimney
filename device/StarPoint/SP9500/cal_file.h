#ifndef CAL_FILE_H
#define CAL_FILE_H

#include "liblog.h"
#include "libdriver.h"
#include "tx_sb_table.h"
#include "tx_lol_table.h"
#include "tx_att_table.h"
#include "tx_pwr_table.h"
#include "rx_ref_table.h"
#include "rx_att_table.h"
#include "tx_filter_table.h"
#include "rx_filter_table.h"
#include "tx_filter_offset_table.h"
#include "rx_filter_offset_table.h"
#include "tx_tc_table.h"
#include <boost/smart_ptr.hpp>
#include <boost/noncopyable.hpp>

/*update calibration file version here*/
/*must add the related function pointer such as "t_GetItemSize";"t_ForwardConvert"*/
//#define CAL_FILE_VER 0x0000   // the very first version
//#define CAL_FILE_VER 0x0001   // add the version number of the file at the beginning,add head and tail before and after the CalFileInfo structure
//#define CAL_FILE_VER 0x0002   // get the probably correct value of x9119 from all calibration files,then create a new file named "cxu.cal" to save this value
//#define CAL_FILE_VER 0x0003   // change receiver's reference range : "output mode : "+30~-30dBm"--->"+30~-40dBm"
                                //                                     "in/out mode : "+30~-20dBm"--->"+30~-30dBm"
//#define CAL_FILE_VER 0x0004   // add an item:attenuation deviation in in/out mode
                                // note that the driver has changed the meaning of the data,each 9 float data represents a status of the attenuation from 0dB to 90dB,step 10dB,so it's necessary to calibrate tx att and tx power again
//#define CAL_FILE_VER 0x0005   // r1c first version
//#define CAL_FILE_VER 0x0006   // change all r1c/d's table's data member "m_EndTime" from struct "COleDateTime" to struct "tm"
#define CAL_FILE_VER 0x0006

/*define all info's head and tail,which are used to locate the related structure*/
/*GUID_1 = {44A4AE51-6470-4C83-A568-34EDDA974411}*/
/*GUID_2 = {1E8EB58C-791E-42AA-8124-C197398944FF}*/
#define CAL_FILE_VER_HEAD  0x44A4AE51
#define CAL_FILE_VER_TAIL  0x64704C83

#define CAL_FILE_INFO_HEAD 0xA56834ED
#define CAL_FILE_INFO_TAIL 0xDA974411

#define CAL_ITEM_INFO_HEAD 0x1E8EB58C	// not used actually,replace head with 0xaa and tail with 0xff
#define CAL_ITEM_INFO_TAIL 0x791E42AA

// Calibration-File-Operation assert
#define CFO_ASSERT(stream,func) \
    do { \
        if (func) { \
            Log.set_last_err("%s:%s:%d(%s)",__FILE__,__FUNCTION__,__LINE__,strerror(errno)); \
            if (stream) \
                fclose(stream); \
            return -1; \
        } \
    } while(0)

// Safe Calibration-File-Operation assert
#define CFO_ASSERT_S(stream,func,post) \
    do { \
        if (func) { \
            Log.set_last_err("%s:%s:%d(%s)",__FILE__,__FUNCTION__,__LINE__,strerror(errno)); \
            if (stream) \
                fclose(stream); \
            post; \
            return -1; \
        } \
    } while(0)

//! Map global buf(g_cal_item_buf.buf) to item(table)'s data(d) buf
#define MAP_GLOBAL_DATAF(table,d) { table->map2buf((d *)(g_cal_item_buf.buf));return 0; }

//! Map data from calibration file to memory
#define MAP_FILE_DATAM(item,table) { map2buf(item);table->map2mem(); }

//! Add data from (d) to item(table)'s data buf
#define ADD_DATAF(table,d) { table->add((d *)data);return 0; }

class API cal_file : ::noncopyable
{
public:
    //! List all calibration items,value only allowed to accumulate.
    enum cal_item_t : unsigned {
        TX_SB         = 0x00, // R1A/B/C/D/E/F
        TX_LOL        = 0x01, // R1A/B/C/D/E/F
        TX_ATT_OP     = 0x02, // R1A/B/C/D/E/F
        TX_PWR_OP     = 0x03, // R1A/B/C/D/E/F  Default BW 160M
        TX_PWR_IO     = 0x04, // R1A/B/C/D/E/F  Default BW 160M
        RX_REF        = 0x05, // R1A/B
        X9119         = 0x06, // R1A/B/C/D/E/F
        TX_ATT_IO     = 0x07, // R1A/B/C/D/E/F
        RX_REF_OP     = 0x08, // R1C/D/E/F      Default BW 160M
        RX_REF_IO     = 0x09, // R1C/D/E/F      Default BW 160M
        RX_ATT_OP     = 0x0a, // R1C/D/E/F
        RX_ATT_IO     = 0x0b, // R1C/D/E/F
        TX_FILTER_80  = 0x0c, // R1C/D/E/F
        TX_FILTER_160 = 0x0d, // R1C/D/E/F
        TX_RF_FR_0    = 0x0e, // R1C/D/E/F
        TX_RF_FR_1    = 0x0f, // R1C/D/E/F
        TX_IF_FR      = 0x10, // R1C/D/E/F
        RX_FILTER_80  = 0x11, // R1C/D/E/F
        RX_FILTER_160 = 0x12, // R1C/D/E/F
        RX_RF_FR      = 0x13, // R1C/D/E/F
        RX_IF_FR      = 0x14, // R1C/D/E/F

        // TX_FILTER_OFFSET_OP_80 & TX_FILTER_OFFSET_IO_80 still in file!
        // They are used to verify the difference between Output and IO mode.
        // And the result is that the offset are almost the same.
        // So we decide to call just in Output mode.
        // In addition,the two items are also used to generate the very first file.
        // The TX_FILTER_OFFSET_80 is the actually called item instead.
        // RX filter offset is the same as TX.
        TX_FILTER_OFFSET_80     = 0x15, // R1C/D/E/F
        TX_FILTER_OFFSET_OP_80	= 0x15,	// R1C/D/E/F  Deprecated!
        TX_FILTER_OFFSET_IO_80	= 0x16,	// R1C/D/E/F  Deprecated!

        RX_FILTER_OFFSET_80     = 0x17,
        RX_FILTER_OFFSET_OP_80	= 0x17,	// R1C/D/E/F  Deprecated!
        RX_FILTER_OFFSET_IO_80	= 0x18,	// R1C/D/E/F  Deprecated!

        TX_TP,
        TX_TC,
        RX_TP,
        RX_TC,
        CAL_TOTAL_ITEMS
	};

    struct file_ver_t {
        uint32_t head;
        uint32_t ver;
        uint32_t tail;
        file_ver_t() : head(0),ver(0xffffffff),tail(0) {}
    };

    struct item_info_t;
    struct file_info_t {
        uint32_t head;
        char sn[32];
        uint32_t items;
        item_info_t *item_info;
        uint32_t tail;
        file_info_t() {item_info = nullptr; SAFE_NEW_ARRAY(item_info,item_info_t,CAL_TOTAL_ITEMS);}
        ~file_info_t(){SAFE_DEL_ARRAY(item_info);}
    };

    STRUCT_ALIGN_S(item_info_t,4)
        uint32_t   head : 32;
        cal_item_t item : 32;
        uint32_t   size : 32; //each frequency's data type(byte)
        uint32_t   pt   : 32; //points
        uint32_t   tail : 32;
    STRUCT_ALIGN_E(item_info_t,4)

    struct API item_buf_t {
        uint32_t rf_idx;
        uint32_t rfu_idx;
        void *buf;
        cal_item_t item;
        item_buf_t();
        ~item_buf_t();
    };

public:
    cal_file(ns_sp1401::hw_ver_t ver,uint32_t rf_idx,uint32_t rfu_idx);
    virtual ~cal_file();
    friend class cfv_ctrl;
public:
    virtual int32_t add(cal_item_t item,void *data) = 0;
    virtual int32_t map2buf(cal_item_t item) = 0;
    virtual int32_t map2mem() = 0;
    virtual int32_t create() = 0;
    virtual int32_t load(file_info_t &info) = 0;
    //getting every item's size may differ from each file version
    virtual int32_t get_item_size(cal_item_t item,uint32_t &pos,uint32_t &size) = 0;
public:
    ns_sp1401::hw_ver_t hw_ver() {return m_hw_ver;}
    uint32_t rf_idx()  {return m_rf_idx; }
    uint32_t rfu_idx() {return m_rfu_idx;}
    uint32_t ordinal() {return m_ordinal;}
    ns_sp1401::bw_t bw()  {return m_bw;}
    static x9119_table *x9119();
    static uint32_t get_max_item_byte();

    virtual void set_bw(ns_sp1401::bw_t bw) {m_bw = bw;}
    void set_hw_ver(ns_sp1401::hw_ver_t ver) {m_hw_ver = ver;}

    virtual int32_t open();
    int32_t w(cal_item_t item);
    int32_t r(cal_item_t item,void *data);
protected:
    void show_file_ver(file_ver_t *ver);
    void show_file_info(file_info_t *info);
    void show_item_info(item_info_t *info);
    static uint32_t get_fix_item_table_r1ab(item_info_t *info,uint32_t &items);
    static uint32_t get_fix_item_table_r1cd(item_info_t *info,uint32_t &items);
    static uint32_t get_max_item_byte(item_info_t *info,uint32_t items);
private:
    bool is_file_valid();
    uint32_t get_file_ver();
    int32_t w_from_pos(const char *path,uint32_t pos,uint32_t size,void *data);
    int32_t r_from_pos(const char *path,uint32_t pos,uint32_t size,void *data);
protected:
    static boost::shared_ptr<x9119_table> g_cal_table_x9119;
    static item_buf_t g_cal_item_buf;
    uint32_t m_rf_idx;
    uint32_t m_rfu_idx;
    uint32_t m_ordinal;
    ns_sp1401::bw_t m_bw;
    ns_sp1401::hw_ver_t m_hw_ver;

#define DECLARE_CAL_FILE() \
public: \
    virtual int32_t add(cal_item_t item,void *data); \
    virtual int32_t map2buf(cal_item_t item); \
    virtual int32_t map2mem(); \
    virtual int32_t create(); \
    virtual int32_t load(file_info_t &info); \
    virtual int32_t get_item_size(cal_item_t item,uint32_t &pos,uint32_t &size);
};

#endif
