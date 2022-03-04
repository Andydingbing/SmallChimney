#include "cal_file_r1ab.h"
#include <string.h>

using namespace ns_starpoint;
using namespace ns_sp9500;

//void cal_file_r1ab::get_tx_att(uint64_t freq,io_mode_t mode,float *att)
//{
//    if (mode._to_integral() == io_mode_t::OUTPUT) {
//        m_tx_att_op->get(freq,att);
//    }
//    if (mode._to_integral() == io_mode_t::IO) {
//        m_tx_att_io->get(freq,att);
//    }
//}

//void cal_file_r1ab::get_tx_att(uint64_t freq,io_mode_t mode,int32_t att,double &offset)
//{
//    if (mode._to_integral() == io_mode_t::OUTPUT) {
//        m_tx_att_op->get(freq,att,offset);
//    }
//    if (mode._to_integral() == io_mode_t::IO) {
//        m_tx_att_io->get(freq,att,offset);
//    }
//}

//void cal_file_r1ab::get_tx_pwr(uint64_t freq,double pwr,io_mode_t mode,double &d_gain,int32_t &att1,int32_t &att2,tx_pa_att_t &pa_att,int32_t &att3)
//{
//    int32_t i_pa_att = 1;
//    if (mode._to_integral() == io_mode_t::OUTPUT) {
//        m_tx_pwr_op->get_op(freq,pwr,d_gain,att1,att2,i_pa_att,att3);
//    }
//    if (mode._to_integral() == io_mode_t::IO) {
//        m_tx_pwr_io->get_io(freq,pwr,d_gain,att1,att2,i_pa_att,att3);
//    }
//    pa_att = (0 == i_pa_att ? TX_PA : TX_ATT);
//}
