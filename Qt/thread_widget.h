#ifndef THREAD_WIDGET_H
#define THREAD_WIDGET_H

#include <QAbstractItemModel>
#include <QTextEdit>
#include "attach_thread_widget.hpp"
#include "winthread.h"
#include "table_model.h"
#include "table_view.hpp"
#include "global.h"
#include "config_table.h"

class Q_Thread_Widget_Base : public Q_Widget, public Q_Attach_Thread_Widget
{
    Q_OBJECT

public:
    Q_Thread_Widget_Base(QWidget *parent = nullptr);

    virtual void init();

    virtual void threadSPC() = 0;
    virtual void prepare(const bool is_exp = false) { Q_UNUSED(is_exp) }

    void registerModelView(Q_Table_Model *model,Q_Table_View *view);

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


template<typename thread_t>
class Q_Thread_Widget : public Q_Thread_Widget_Base
{
public:
    Q_Thread_Widget(QWidget *parent) : Q_Thread_Widget_Base(parent) {}

    virtual void Set() {}

    void threadSPC() OVERRIDE
    {
        if (Q_Thread_Base::g_threadThread == nullptr) {
            return threadStart();
        }

        if (Q_Thread_Base::g_threadPausing) {
            return threadContinue();
        }

        return threadPause();
    }

    void threadStart()
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

        Q_Thread_Base::g_threadThread = new thread_t(this);
        Q_Thread_Base::g_threadThread->RFIdx = ::RFIdx;
        Q_Thread_Base::g_threadThread->configDelegate = configDelegate;
        Q_Thread_Base::g_threadThread->resultDelegate = resultDelegate;
        Q_Thread_Base::g_threadThread->start();
    }

    void threadPause()
    {
        Q_Thread_Base::g_threadThread->threadLock.lock();
        widgetPausing();
        Q_Thread_Base::g_threadPausing = true;
    }

    void threadContinue()
    {
        Q_Thread_Base::g_threadThread->threadLock.unlock();
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
#define KASE_THREAD_CLASS_NAME(kase) Q_##kase##_Thread
#define KASE_WIDGET_UI_CLASS_NAME(vendor,device,kase) Q_##vendor##_##device##_##kase##_Widget

#define KASE_WIDGET_PREFIX(vendor,product,kase) \
    class KASE_THREAD_CLASS_NAME(kase); \
    class kase##_ConfigModel; \
    class kase##_ConfigDelegate; \
    class kase##_ResultModel; \
    class kase##_ResultDelegate; \
    class KASE_WIDGET_CLASS_NAME(kase) : public Q_Thread_Widget<KASE_THREAD_CLASS_NAME(kase)> \
    { \
    public: \
        KASE_WIDGET_CLASS_NAME(kase)(QWidget *parent) : \
            Q_Thread_Widget(parent), \
            ui(new Ui::KASE_WIDGET_UI_CLASS_NAME(vendor,product,kase)) \
        { \
            ui->setupUi(this); \
            configModel = new kase##_ConfigModel; \
            configDelegate = new kase##_ConfigDelegate; \
            config = static_cast<kase##_ConfigDelegate *>(configDelegate); \
            resultModel = new kase##_ResultModel; \
            resultDelegate = new kase##_ResultDelegate; \
            result = static_cast<kase##_ResultDelegate *>(resultDelegate); \
            ui->config_result->ui->config->setItemDelegate(configDelegate); \
            ui->config_result->ui->config->setModel(configModel); \
            ui->config_result->ui->config->setSpan(0,0,1,2); \
            ui->config_result->ui->result->setItemDelegate(resultDelegate); \
            ui->config_result->ui->result->setModel(resultModel); \
            ui->config_result->ui->result->setSpan(0,0,1,2); \
            config->labelConfig->setText("Config:"); \
            result->labelResult->setText("Result:"); \
            ui->config_result->ui->splitter->setStretchFactor(0,2); \
            ui->config_result->ui->splitter->setStretchFactor(1,1); \
            init(); \
        } \
        ~KASE_WIDGET_CLASS_NAME(kase)() { delete ui; } \
        Ui::KASE_WIDGET_UI_CLASS_NAME(vendor,product,kase) *ui; \
        kase##_ConfigDelegate *config; \
        kase##_ResultDelegate *result; \
    \
    public slots: \
        void init(); \
        void prepare(const bool is_exp = false); \
        void uiUpdate(const int first,const int last,const int item); \
    public:

#define KASE_CONFIG(kase,...) \
    CONFIG_TABLE_KASE(kase,__VA_ARGS__)

#define KASE_RESULT(kase,...) \
    RESULT_TABLE_KASE(kase,__VA_ARGS__)

#define KASE_THREAD(kase_name) \
    }; \
    class KASE_THREAD_CLASS_NAME(kase_name) : public Q_Thread \
    { \
    public: \
        KASE_THREAD_CLASS_NAME(kase_name)(QObject *parent) : \
            Q_Thread(parent) \
        { \
            config = static_cast<kase_name##_ConfigDelegate *>(configDelegate); \
            result = static_cast<kase_name##_ResultDelegate *>(resultDelegate); \
        } \
        void kase(); \
        kase_name##_ConfigDelegate *config; \
        kase_name##_ResultDelegate *result; \
    public:

#define KASE_WIDGET_SUFFIX() \
    };

#endif // THREAD_WIDGET_H
