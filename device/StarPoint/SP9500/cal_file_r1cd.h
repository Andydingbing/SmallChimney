#ifndef CAL_FILE_R1CD_H
#define CAL_FILE_R1CD_H

#include "cal_file.h"

namespace rd {

class API cal_file_r1cd : public cal_file
{
    DECLARE_CAL_FILE()

public:
    cal_file_r1cd(ns_sp1401::hw_ver_t ver,uint32_t rf_idx,uint32_t rfu_idx);
    virtual ~cal_file_r1cd();

public:
    virtual int32_t open();
    void set_bw(ns_sp1401::bw_t bw);

    int32_t gen_tx_filter_coef();
    int32_t gen_rx_freq_res();
    int32_t gen_rx_filter_coef();
    int32_t update_rx_filter_table();

    tx_lol_table_r1cd *tx_lol() { return m_tx_lol.get(); }
//    tx_sb_table_r1cd* tx_sb { return m_tx_sb.get(); }
    tx_tc_table *tx_tc() { return m_tx_tc.get(); }
    tx_tp_table *tx_tp() { return _tx_tp.get(); }

    rx_tc_table *rx_tc() { return m_rx_tc.get(); }
    rx_tp_table *rx_tp() { return _rx_tp.get(); }

public:
    boost::shared_ptr<tx_lol_table_r1cd>        m_tx_lol;
    boost::shared_ptr<tx_sb_table_r1cd>         m_tx_sb;
    boost::shared_ptr<tx_pwr_op_table_r1c>      m_tx_pwr_op;
    boost::shared_ptr<tx_pwr_io_table_r1c>      m_tx_pwr_io;
    boost::shared_ptr<tx_att_op_table_r1cd>     m_tx_att_op;
    boost::shared_ptr<tx_att_io_table_r1cd>     m_tx_att_io;
    boost::shared_ptr<rx_ref_op_table_r1cd>     m_rx_ref_op;
    boost::shared_ptr<rx_ref_io_table_r1cd>     m_rx_ref_io;
    boost::shared_ptr<rx_att_op_table_r1cd>     m_rx_att_op;
    boost::shared_ptr<rx_att_io_table_r1cd>     m_rx_att_io;
    boost::shared_ptr<tx_filter_80m_table>      m_tx_filter_80m;
    boost::shared_ptr<tx_filter_160m_table>     m_tx_filter_160m;
    boost::shared_ptr<rx_filter_80m_table>      m_rx_filter_80m;
    boost::shared_ptr<rx_filter_160m_table>     m_rx_filter_160m;
    boost::shared_ptr<tx_filter_offset_table>   m_tx_filter_offset_op_80m;
    boost::shared_ptr<tx_filter_offset_table>   m_tx_filter_offset_io_80m;
    boost::shared_ptr<rx_filter_offset_table>   m_rx_filter_offset_op_80m;
    boost::shared_ptr<rx_filter_offset_table>   m_rx_filter_offset_io_80m;
    boost::shared_ptr<tx_tc_table>              m_tx_tc; // Coefs in memory directly(see sp3301.h)
    boost::shared_ptr<rx_tc_table>              m_rx_tc;

public:
    boost::shared_ptr<tx_rf_fr_0_table_r1cd>    m_tx_rf_fr_0;	//<=3100
    boost::shared_ptr<tx_rf_fr_1_table_r1cd>    m_tx_rf_fr_1;	//>3100
    boost::shared_ptr<tx_if_fr_table>           m_tx_if_fr;
    boost::shared_ptr<rx_rf_fr_table>           m_rx_rf_fr;
    boost::shared_ptr<rx_if_fr_table>           m_rx_if_fr;
    boost::shared_ptr<tx_tp_table>              _tx_tp;
    boost::shared_ptr<rx_tp_table>              _rx_tp;
};

} //namespace rd

#endif // CAL_FILE_R1CD_H
