#ifndef Q_WINTHREAD_H
#define Q_WINTHREAD_H

#include "global.h"
#include <QThread>
#include <QMutex>
#include "freq_string.hpp"
#include "set_helper.hpp"

#define checkPoint(msg) \
{   emit threadCheckBox(msg); \
    if (Q_Thread_Base::g_threadStop) { \
        emit threadProcess(End_Manual); \
        emit done(); \
        return; \
    } \
}

#define THREAD_ERROR_BOX(msg) { emit threadErrorBox(msg); }

#define cancelPoint() \
{   if (Q_Thread_Base::g_threadStop) { \
        emit threadProcess(End_Manual); \
        return; \
    } \
}

#define pausePoint() \
{   if (Q_Thread_Base::g_threadPausing) { \
        emit threadProcess(Paused); \
        while (Q_Thread_Base::g_threadPausing) { msleep(2); } \
        emit threadProcess(Running_Normal); \
    } \
    cancelPoint() \
}


#define KASE_THREAD_START(name,pts) \
    cancelPoint(); \
    emit threadProcess(Prepare); \
    checkPoint(QString("Prepare Case : %1").arg(name)); \
    emit threadProcess(Started); \
    initProgress(QString("Case : %1").arg(name),pts);


class Q_Thread_Base : public QThread
{
    Q_OBJECT

public:
    enum Process {
        Prepare = 0,     // The thread is prepared,but not started yet.
        Started,         // The thread has been started and running.
        Running_Normal,  // Running normally.
        Running_Except,  // Some exceptions happened or something failed,but still running.
        Paused,          // The thread has been paused.
        End_Normal,      // The thread is normally ended.
        End_Manual,      // The thread is manually ended.
        End_Except       // Some exceptions are throwed and the thread is ended.
    };

public:
    Q_Thread_Base(QObject *parent = nullptr);
    static int registerMetaType() { return qRegisterMetaType<Process>("Process"); }
    void initProgress(const QString name) { emit initProg(name); }
    void initProgress(const QString name, quint32 pts) { emit initProg(name,pts); }
    void setProgressPos(quint32 pos) { emit setProgPos(pos); }
    void addProgressPos(quint32 off) { emit addProgPos(off); }

signals:
    void initProg(const QString name);
    void initProg(const QString name, quint32 pts);
    void setProgPos(quint32 pos);
    void addProgPos(quint32 off);
    void done(bool success = true);
    void threadCheckBox(QString msg);
    void threadErrorBox(QString msg);
    void threadProcess(const Process p);

    void uiInsert(const int first,const int last,const int arg);
    void uiUpdate(const int first,const int last,const int arg);

public:
    quint32 RFIdx;

    QMutex threadLock;
    static bool g_threadStop;
    static bool g_threadPausing;
    static QString  g_threadName;
    static Q_Thread_Base *g_threadThread;

    Q_Config_Table_Delegate_Kase *configDelegate;
    Q_Result_Table_Delegate_Kase *resultDelegate;
};


class Q_Thread : public Q_Thread_Base
{
    Q_OBJECT

public:
    Q_Thread(QObject *parent = nullptr);

    virtual void kase() {}

    void run();
    void updateTotalResult(bool singleResult)
    { totalResult = totalResult ? singleResult : totalResult; }

protected:
    int32_t HWVer;

    bool totalResult;
    quint32 totalPts;

    std::string freqString;
    set_helper::range_freq<uint64_t> freqRange;
};

void threadCheckBox(const QString &msg);
void threadErrorBox(const QString &msg);

bool ftpRetryBox();

#endif // Q_WINTHREAD_H
