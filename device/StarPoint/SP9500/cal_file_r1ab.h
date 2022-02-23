#ifndef CAL_FILE_R1AB_H
#define CAL_FILE_R1AB_H

#include "cal_file.h"

namespace rd {

class API cal_file_r1ab : public rd::cal_file
{
    DECLARE_CAL_FILE()

public:
    cal_file_r1ab(ns_sp1401::hw_ver_t ver,uint32_t rf_idx,uint32_t rfu_idx);
    virtual ~cal_file_r1ab();

public:
    virtual int32_t open();
    int32_t get_item_size_v0000(cal_file::cal_item_t item,uint32_t &pos,uint32_t &size);
    int32_t get_item_size_v0001(cal_file::cal_item_t item,uint32_t &pos,uint32_t &size);
    int32_t get_item_size_v0002(cal_file::cal_item_t item,uint32_t &pos,uint32_t &size);
    int32_t get_item_size_v0004(cal_file::cal_item_t item,uint32_t &pos,uint32_t &size);
public:
    void get_tx_att(uint64_t freq,io_mode_t mode,float *att);
    void get_tx_att(uint64_t freq,io_mode_t mode,int32_t att,double &offset);
    void get_tx_pwr(uint64_t freq,double pwr,io_mode_t mode,
                    double &d_gain,int32_t &att1,int32_t &att2,ns_sp1401::r1a::tx_pa_att_t &pa_att,int32_t &att3);
public:
    boost::shared_ptr<tx_sb_table_r1ab>  m_tx_sb;
    boost::shared_ptr<tx_lol_table_r1ab> m_tx_lol;
    boost::shared_ptr<tx_att_table_r1ab> m_tx_att_op;
    boost::shared_ptr<tx_pwr_table_r1ab> m_tx_pwr_op;
    boost::shared_ptr<tx_pwr_table_r1ab> m_tx_pwr_io;
    boost::shared_ptr<rx_ref_table_r1ab> m_rx_ref;
    boost::shared_ptr<tx_att_table_r1ab> m_tx_att_io;
};

} //namespace rd

#endif
