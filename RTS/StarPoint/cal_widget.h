#ifndef SP9500PRO_CAL_WIDGET_H
#define SP9500PRO_CAL_WIDGET_H

#include "q_cal_base_dlg.h"
#include "ui_cal_filter.h"
#include "ui_sp9500x_cal_tx_pwr.h"
#include "ui_sp9500x_cal_tx_att.h"

#include "sp9500x_cal_tx_filter_thread.h"
#include "sp9500x_cal_tx_pwr_thread.h"
#include "sp9500x_cal_tx_att_thread.h"

namespace Ui {
typedef class Q_Cal_Filter_Widget Q_SP9500X_Cal_TXFilter_Widget;
}

DECL_CAL_WIDGET(SP9500X,TXFilter)
DECL_CAL_WIDGET(SP9500X,TX_Pwr)
DECL_CAL_WIDGET(SP9500X,TX_Att)


#endif // SP9500PRO_CAL_WIDGET_H
