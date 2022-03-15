#include "winthread.h"
#include <QMessageBox>
#include <QDateTime>
#include "thread_widget.h"
#include "../instrument/instr.h"

// Exception support.
#ifdef RD_EXCEPTION
    #define KASE_TRY \
        try {

    #define KASE_CATCH \
        } catch (std::exception &e) { \
            THREAD_ERROR_BOX(QString("%1").arg(e.what())); \
            /*threadLock.lock();*/ \
            /*threadLock.unlock();*/ \
            Instr.close(); \
            emit threadProcess(End_Except); \
            emit done(); \
        }
#else
    #define KASE_TRY
    #define KASE_CATCH
#endif

bool Q_Thread_Base::g_threadStop = true;
bool Q_Thread_Base::g_threadPausing = false;
QString Q_Thread_Base::g_threadName = QString("");
Q_Thread_Base *Q_Thread_Base::g_threadThread = nullptr;

Q_Thread_Base::Q_Thread_Base(QObject *parent) :
    QThread(parent),
    RFIdx(0)
{
    g_threadStop = false;
    g_threadPausing = false;

    connect(this,SIGNAL(finished()),this,SLOT(deleteLater()));
}

void Q_Thread_Base::setMainWindow(QWidget *window)
{
    mainWindow = window;

    connect(this,SIGNAL(initProg(QString)),mainWindow,SLOT(initProg(QString)));
    connect(this,SIGNAL(initProg(QString,quint32)),mainWindow,SLOT(initProg(QString,quint32)));
    connect(this,SIGNAL(setProgPos(quint32)),mainWindow,SLOT(setProgPos(quint32)));
    connect(this,SIGNAL(addProgPos(quint32)),mainWindow,SLOT(addProgPos(quint32)));

    connect(this,SIGNAL(threadCheckBox(QString)),
        mainWindow,SLOT(threadCheckBox(QString)),Qt::BlockingQueuedConnection);

    connect(this,SIGNAL(threadErrorBox(QString)),
        mainWindow,SLOT(threadErrorBox(QString)),Qt::BlockingQueuedConnection);
}

Q_Thread::Q_Thread(QObject *parent) :
    Q_Thread_Base(parent)
{
    if (parent != nullptr) {
        connect(this,SIGNAL(uiInsert(const int,const int,const int)),
        parent,SLOT(uiInsert(const int,const int,const int)));

        connect(this,SIGNAL(uiUpdate(const int,const int,const int)),
        parent,SLOT(uiUpdate(const int,const int,const int)));

        connect(this,SIGNAL(done(bool)),parent,SLOT(done(bool)),Qt::BlockingQueuedConnection);
    }

    connect(this,SIGNAL(threadProcess(const Process p)),mainWindow,SLOT(threadProcess(const Process p)));

    Q_Thread_Widget_Base *p = dynamic_cast<Q_Thread_Widget_Base *>(parent);

    if (p == nullptr) {
        return;
    }

    for (int i = 0;i < p->tables.size();++i) {
        connect(this,&Q_Thread::uiInsert,p->tables.at(i).first,&Q_Table_Model::uiInsert);
        connect(this,&Q_Thread::uiUpdate,p->tables.at(i).first,&Q_Table_Model::uiUpdate);
    }
}

void Q_Thread::run()
{
    KASE_TRY

    kase();
    Instr.close();
    emit done();
    return;

    KASE_CATCH
}


void fmtMsg(QString &msg,const int totalLength = 50)
{
    int length = msg.length();

    if (length < totalLength) {
        int spaceL = (totalLength - length) / 2;
        int spaceR = (totalLength - length - spaceL);

        for (int i = 0;i < spaceL;i ++) {
            msg.prepend(" ");
        }

        for (int i = 0;i < spaceR;i ++) {
            msg.append(" ");
        }
    }
}

void threadCheckBox(const QString &msg)
{
    QString msgCopy = msg;
    fmtMsg(msgCopy);

    QMessageBox msgBox;
    msgBox.setText(msgCopy);
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    msgBox.setDefaultButton(QMessageBox::Yes);
    msgBox.setModal(true);

    if (msgBox.exec() == QMessageBox::No) {
        Q_Thread_Base::g_threadStop = true;
    }
}

void threadErrorBox(const QString &msg)
{
    QMessageBox msgBox;

    if (msg.length() < 1) {
        msgBox.setText(QString("Error When Creating Thread"));
        msgBox.setInformativeText(QString("Another thread may running.Try to stop it!"));
    } else {
        msgBox.setText(QString("        %1        ").arg(msg));
    }

    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setModal(true);
    msgBox.exec();
}

bool ftpRetryBox()
{
    QMessageBox msgBox;
    msgBox.setText("FTP server disconnected!Retry?");
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    msgBox.setDefaultButton(QMessageBox::Yes);
    msgBox.setModal(true);

    return msgBox.exec() == QMessageBox::Yes;
}
