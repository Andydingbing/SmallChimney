#include "main_thread.h"
#include "mainwindow.h"

#if (QT_VERSION < QT_VERSION_CHECK(4,7,0))
    #include <QTime>
#else
    #include <QElapsedTimer>
#endif

Q_Main_Thread::Q_Main_Thread(QObject *parent) :
    Q_Thread_Base(parent),
    childThreadRunning(false)
{
    connect(this,SIGNAL(prepare(const int)),parent,SLOT(runThread(const int)),Qt::BlockingQueuedConnection);
    connect(this,SIGNAL(done(bool)),parent,SLOT(threadDone(bool)));
}

void Q_Main_Thread::run()
{
//    QList<bool> checkList = g_MainW->currentWidgets->checkList();
//    QList<bool>::const_iterator iterCheckList = checkList.constBegin();
////    QList<ChildWidgetHelper::TreeChildItem *> *items = &(g_MainW->currentWidgets->_items);
////    QList<ChildWidgetHelper::TreeChildItem *>::const_iterator iterChildItem = items->constBegin();

//    int i = 0;

//    for (;iterCheckList != checkList.constEnd();++iterCheckList/*,++iterChildItem*/) {
//        if (*iterCheckList) {
//            emit prepare(i);
//            waitChildThread(0,i);
//            i ++;
//        }
//    }

//    emit done();
}

void Q_Main_Thread::childThreadDone(bool success)
{
    childThreadRunning = false;
}

void Q_Main_Thread::waitChildThread(int timeout, int idx)
{
    timeout = timeout ? timeout : int(0x7fffffff);

    childThreadRunning = true;

    for (int t = 0;t < timeout;++t) {
        if (childThreadRunning == false) {
            return;
        }
        msleep(2);
    }
}
