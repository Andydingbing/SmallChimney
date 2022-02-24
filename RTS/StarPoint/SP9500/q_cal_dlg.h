#ifndef Q_CAL_Dlg_H
#define Q_CAL_Dlg_H

#include "q_cal_base_dlg.h"
#include "ui_q_cal_tx_loleak_dlg.h"
#include "ui_q_cal_tx_sb_dlg.h"
#include "ui_q_cal_tx_filter_dlg.h"
#include "ui_q_cal_tx_pwr_dlg.h"
#include "ui_q_cal_tx_att_dlg.h"
#include "ui_q_cal_tx_filter_offset_dlg.h"
#include "ui_q_cal_rx_filter_dlg.h"
#include "ui_q_cal_rx_ref_dlg.h"
#include "ui_q_cal_rx_att_dlg.h"
#include "ui_q_cal_rx_filter_offset_dlg.h"
#include "ui_q_cal_tx_temp_comp_dlg.h"
#include "ui_q_cal_rx_temp_comp_dlg.h"
#include "q_cal_r1c_tx_loleak_thread.h"
#include "q_cal_r1c_tx_sb_thread.h"
#include "q_cal_r1c_tx_filter_thread.h"
#include "q_cal_r1c_tx_pwr_thread.h"
#include "q_cal_r1c_tx_att_thread.h"
#include "q_cal_r1c_tx_filter_offset_thread.h"
#include "q_cal_r1c_rx_filter_thread.h"
#include "q_cal_r1c_rx_ref_thread.h"
#include "q_cal_r1c_rx_att_thread.h"
#include "q_cal_r1c_rx_filter_offset_thread.h"
#include "q_cal_r1c_tx_temp_comp_thread.h"

DECL_CAL_R1C_DLG(TXLOLeak)
DECL_CAL_R1C_DLG(TXSB)
DECL_CAL_R1C_DLG(TXPwr)
DECL_CAL_R1C_DLG(TXFilter)
DECL_CAL_R1C_DLG(TXAtt)
DECL_CAL_R1C_DLG(TXFilterOffset)
DECL_CAL_R1C_DLG(RXFilter)
DECL_CAL_R1C_DLG(RXRef)
DECL_CAL_R1C_DLG(RXAtt)
DECL_CAL_R1C_DLG(RXFilterOffset)
DECL_CAL_R1C_DLG(TxTempComp)
DECL_CAL_R1C_DLG(RXTempComp)

#endif // Q_CAL_Dlg_H
