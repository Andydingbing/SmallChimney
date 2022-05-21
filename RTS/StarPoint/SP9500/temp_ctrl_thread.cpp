#include "q_r1c_temp_ctrl_thread.h"
#include "q_temp_ctrl_dlg.h"

QR1CTempCtrlThread::QR1CTempCtrlThread(Param *param, QObject *parent) :
    QWinThread(parent)
{
    this->param = *param;
}

void QR1CTempCtrlThread::run()
{
    QwtTempData::Data data[8];

    while (1) {
       THREAD_TEST_CANCEL

       for (quint8 i = 0;i < ns_sp9500::g_max_rf;i ++) {
           sp1401_r1c *SP1401 = (sp1401_r1c *)(param.SP3301->get_sp1401_r1c(i));
           SP1401->get_temp(0,data[0].temp);
           SP1401->get_temp(1,data[1].temp);
           SP1401->get_temp(2,data[2].temp);
           SP1401->get_temp(3,data[3].temp);
           SP1401->get_temp(4,data[4].temp);
           SP1401->get_temp(5,data[5].temp);
           SP1401->get_temp(6,data[6].temp);
           SP1401->get_temp(7,data[7].temp);
       }
   }
   THREAD_ENDED
}
