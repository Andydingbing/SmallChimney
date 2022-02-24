#include "sp9500x_test_thread.h"
#include "model_test_baseband.hpp"
#include "spec.h"
#include "q_test_dlg.h"

using namespace NS_SP9500X;

void Q_Test_Baseband_Thread::run()
{
    RD_TEST_TRY

    TEST_THREAD_START("Baseband",1);

    Q_Test_Baseband_Widget     *w = dynamic_cast<Q_Test_Baseband_Widget *>(parent());
    Q_Test_Baseband_TableModel *m = dynamic_cast<Q_Test_Baseband_TableModel *>(w->model->at(0));

    if (m->item[Test_Baseband_Item::DMA]) { runDMA(); }

    THREAD_ENDED
    RD_TEST_CATCH
}

void Q_Test_Baseband_Thread::runDMA()
{
    const quint32 cnt = 10000;

    for (quint32 i = 0;i < cnt;++i) {
        if (SP2406->ddr()->test_case(1 * 1024 * 1024)) {
            emit threadProcess(RUNNING_EXCEPT);
        }
    }
}
