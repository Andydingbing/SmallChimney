#ifndef THREAD_WIDGET_H
#define THREAD_WIDGET_H

#include <QAbstractItemModel>
#include <QTextEdit>
#include "attach_thread_widget.hpp"
#include "winthread.h"
#include "table_model.h"
#include "table_view.hpp"
#include "config_table.h"

class Q_Widget : public QWidget
{
    Q_OBJECT

public:
    Q_Widget(QWidget *parent = nullptr) : QWidget(parent) {}

public slots:
    virtual void init() {} // Init child widgets
};


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
#define KASE_THREAD_CLASS_NAME(kase) Q_##kase##_Thread
#define KASE_WIDGET_UI_CLASS_NAME(vendor,device,kase) Q_##vendor##_##device##_##kase##_Widget

#define KASE_WIDGET_PREFIX(vendor,product,kase) \
    class KASE_THREAD_CLASS_NAME(kase); \
    class API KASE_WIDGET_CLASS_NAME(kase) : public Q_Thread_Widget<KASE_THREAD_CLASS_NAME(kase)> \
    { \
    public: \
        ~KASE_WIDGET_CLASS_NAME(kase)() { delete ui; } \
        class Q_Config_Delegate; \
        class Q_Result_Delegate; \
        Q_Config_Delegate *config; \
        Q_Result_Delegate *result; \
        Ui::KASE_WIDGET_UI_CLASS_NAME(vendor,product,kase) *ui; \
        void initUi() \
        { \
            ui = new Ui::KASE_WIDGET_UI_CLASS_NAME(vendor,product,kase); \
            ui->setupUi(this); \
        } \
    \
    public slots: \
        void init(); \
        void prepare(const bool is_exp = false); \
        void uiUpdate(const int first,const int last,const int item); \
    public:

#define KASE_CONFIG(...) CONFIG_TABLE_KASE(__VA_ARGS__)

#define KASE_RESULT(...) RESULT_TABLE_KASE(__VA_ARGS__)

#define KASE_THREAD(kase_name) \
    KASE_WIDGET_CLASS_NAME(kase_name)(QWidget *parent) : \
        Q_Thread_Widget(parent) \
    { \
        initUi(); \
        configModel = new Q_Config_Model; \
        configDelegate = new Q_Config_Delegate; \
        config = static_cast<Q_Config_Delegate *>(configDelegate); \
        resultModel = new Q_Result_Model; \
        resultDelegate = new Q_Result_Delegate; \
        result = static_cast<Q_Result_Delegate *>(resultDelegate); \
        ui->config_result->config->setItemDelegate(configDelegate); \
        ui->config_result->config->setModel(configModel); \
        ui->config_result->config->setSpan(0,0,1,2); \
        ui->config_result->result->setItemDelegate(resultDelegate); \
        ui->config_result->result->setModel(resultModel); \
        ui->config_result->result->setSpan(0,0,1,2); \
        config->labelConfig->setText("Config:"); \
        result->labelResult->setText("Result:"); \
        ui->config_result->splitter->setStretchFactor(0,2); \
        ui->config_result->splitter->setStretchFactor(1,1); \
        init(); \
    } \
    }; \
    class API KASE_THREAD_CLASS_NAME(kase_name) : public Q_Thread \
    { \
    public: \
        KASE_THREAD_CLASS_NAME(kase_name)(QObject *parent) : \
            Q_Thread(parent) \
        { \
            config = static_cast<KASE_WIDGET_CLASS_NAME(kase_name)::Q_Config_Delegate *>(configDelegate); \
            result = static_cast<KASE_WIDGET_CLASS_NAME(kase_name)::Q_Result_Delegate *>(resultDelegate); \
        } \
        void kase(); \
        KASE_WIDGET_CLASS_NAME(kase_name)::Q_Config_Delegate *config; \
        KASE_WIDGET_CLASS_NAME(kase_name)::Q_Result_Delegate *result; \
    public:

#define KASE_WIDGET_SUFFIX() \
    };

#endif
