#include "q_iq_cap_thread.h"
#include "mainwindow.h"

void QIQCapThread::run()
{
    if (project == SP9500) {
        while (1) {
            THREAD_TEST_CANCEL;
            INT_CHECKV(SP3301->iq_cap(RFIdx));
            emit updatePlot();
        }
    } else if (project == DT3308) {
        while (1) {
            THREAD_TEST_CANCEL;
            DT3308_BB_F->ddr()->fpga_w();
            emit updatePlot();
        }
    } else if (project == SP9500X) {
        while (1) {
            THREAD_TEST_CANCEL;
            INT_CHECKV(NS_SP9500X::SP2406->set_iq_cap_abort());
            INT_CHECKV(NS_SP9500X::SP2406->set_iq_cap_start());
            emit updatePlot();
        }
    }
}
