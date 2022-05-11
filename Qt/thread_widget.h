#ifndef THREAD_WIDGET_H
#define THREAD_WIDGET_H

#include <QAbstractItemModel>
#include <QTextEdit>
#include <QMainWindow>
#include "attach_thread_widget.hpp"
#include "winthread.h"
#include "table_model.h"
#include "table_view.hpp"
#include "config_table.h"

class QHELPER_EXPORT Q_Widget : public QWidget
{
    Q_OBJECT

public:
    Q_Widget(QWidget *parent = nullptr) : QWidget(parent) {}

public slots:
    virtual void init() {} // Init child widgets
};


class QHELPER_EXPORT Q_Thread_Widget_Base : public Q_Widget, public Q_Attach_Thread_Widget
{
    Q_OBJECT

public:
    Q_Thread_Widget_Base(QWidget *parent = nullptr);

    virtual void init();

    virtual Q_Thread_Base *instanceThread() = 0;
    virtual void threadSPC(const QMainWindow *mainWindow = nullptr) = 0;
    virtual void prepare(const bool is_exp = false) { Q_UNUSED(is_exp) }

    void registerModelView(Q_Table_Model *model,Q_Table_View *view);

    QWidget* config(const QString &item) const;
    QWidget* result(const QString &item) const;

public slots:
    virtual void uiInsert(const int /*first*/,const int /*last*/,const int /*kase*/) {}
    virtual void uiUpdate(const int /*first*/,const int /*last*/,const int /*kase*/) {}

    void on_pushButtonExport_clicked() { prepare(true); }

    void done(bool success = true);

signals:
    void reset();

public:
    Q_Config_Table_Model *configModel;
    Q_Config_Table_Delegate_Kase *configDelegate;

    Q_Result_Table_Model *resultModel;
    Q_Result_Table_Delegate_Kase *resultDelegate;

    QList<QPair<Q_Table_Model *,Q_Table_View *>> tables;
};


class Q_Thread_Widget : public Q_Thread_Widget_Base
{
public:
    Q_Thread_Widget(QWidget *parent) : Q_Thread_Widget_Base(parent) {}

    virtual void Set() {}

    void threadSPC(const QMainWindow *mainWindow = nullptr) OVERRIDE
    {
        if (Q_Thread_Base::g_threadThread == nullptr) {
            return threadStart(mainWindow);
        }

        if (Q_Thread_Base::g_threadPausing) {
            return threadContinue();
        }

        return threadPause();
    }

    void threadStart(const QMainWindow *mainWindow = nullptr)
    {
        if (Q_Thread_Base::g_threadThread != nullptr) {
            threadErrorBox(Q_Thread_Base::g_threadName);
            return;
        }

        Q_Thread_Base::g_threadStop = false;

        widgetPrepare();
        prepare(false);

        QList<QPair<Q_Table_Model *,Q_Table_View *>>::iterator iter;

        for (iter = tables.begin();iter != tables.end();++iter) {
            iter->second->setModel(iter->first);
        }

        emit reset();

        for (iter = tables.begin();iter != tables.end();++iter) {
            iter->second->selectRow(0);
        }

        Q_Thread_Base::g_threadThread = instanceThread();
//        Q_Thread_Base::g_threadThread->RFIdx = ::g_tabIdx;
        Q_Thread_Base::g_threadThread->configModel = configModel;
        Q_Thread_Base::g_threadThread->configDelegate = configDelegate;
        Q_Thread_Base::g_threadThread->resultModel = resultModel;
        Q_Thread_Base::g_threadThread->resultDelegate = resultDelegate;
        Q_Thread_Base::g_threadThread->setMainWindow(mainWindow);
        Q_Thread_Base::g_threadThread->start();
    }

    void threadPause()
    {
        widgetPausing();
        Q_Thread_Base::g_threadPausing = true;
    }

    void threadContinue()
    {
        widgetRunning();
        Q_Thread_Base::g_threadPausing = false;
    }

    void on_pushButtonGetSet_clicked()
    {
        Set();
    }

    void threadDone()
    {
        Q_Thread_Base::g_threadThread = nullptr;
        Q_Thread_Base::g_threadStop = true;
        widgetResume();
    }
};

#define KASE_WIDGET_CLASS_NAME(kase) Q_##kase##_Widget
#define KASE_WIDGET_UI_CLASS_NAME(kase) Q_##kase##_Widget

#define KASE_PREFIX(kase) \
    class API KASE_WIDGET_CLASS_NAME(kase) : public Q_Thread_Widget \
    { \
    public: \
        KASE_WIDGET_CLASS_NAME(kase)(QWidget *parent) : \
            Q_Thread_Widget(parent) \
        { \
            configModel = new Q_Config_Table_Model; \
            configDelegate = new Q_Config_Table_Delegate_Kase; \
            resultModel = new Q_Result_Table_Model; \
            resultDelegate = new Q_Result_Table_Delegate_Kase; \
            initUi(); \
            initConfigResultTable(); \
            configModel->setRowCount(configModel->rowCount(QModelIndex())); \
            configModel->setColumnCount(configModel->columnCount(QModelIndex())); \
            resultModel->setRowCount(resultModel->rowCount(QModelIndex())); \
            resultModel->setColumnCount(resultModel->columnCount(QModelIndex())); \
            ui->config_result->config->setItemDelegate(configDelegate); \
            ui->config_result->config->setModel(configModel); \
            ui->config_result->config->setSpan(0,0,1,2); \
            ui->config_result->result->setItemDelegate(resultDelegate); \
            ui->config_result->result->setModel(resultModel); \
            ui->config_result->result->setSpan(0,0,1,2); \
            configDelegate->labelConfig->setText("Config:"); \
            resultDelegate->labelResult->setText("Result:"); \
            ui->config_result->splitter->setStretchFactor(0,2); \
            ui->config_result->splitter->setStretchFactor(1,1); \
            init(); \
        } \
        ~KASE_WIDGET_CLASS_NAME(kase)() { delete ui; } \
        Ui::KASE_WIDGET_UI_CLASS_NAME(kase) *ui; \
    public slots: \
        void init(); \
        void prepare(const bool is_exp = false); \
        void uiUpdate(const int first,const int last,const int item); \
    public: \
        void initUi() \
        { \
            ui = new Ui::KASE_WIDGET_UI_CLASS_NAME(kase); \
            ui->setupUi(this); \
        } \
        void initConfigResultTable() \
        { \
            configModel->addItemTitle(""); \
            resultModel->addItemTitle("");


#define KASE_WIDGET \
        } \
        public


#define KASE_THREAD \
    class Thread : public Q_Thread \
    { \
    public: \
        Thread(QObject *parent) : Q_Thread(parent) \
        { \
        } \
        void kase(); \
    public

#define KASE_SUFFIX() \
    }; \
    Q_Thread_Base *instanceThread() \
    { \
        Thread *thread = new Thread(this); \
        return (Q_Thread_Base *)thread; \
    } \
    };

#endif
