#include "thread_widget.h"

Q_Thread_Widget_Base::Q_Thread_Widget_Base(QWidget *parent) :
    Q_Widget(parent),
    Q_Attach_Thread_Widget(),
    configModel(nullptr),
    configDelegate(nullptr),
    resultModel(nullptr),
    resultDelegate(nullptr)
{

}

void Q_Thread_Widget_Base::init()
{
    widgetResume();
}

void Q_Thread_Widget_Base::registerModelView(Q_Table_Model *model,Q_Table_View *view)
{
    if (model == nullptr || view == nullptr) {
        return;
    }

    tables.push_back(QPair<Q_Table_Model *,Q_Table_View *>(model,view));
    connect(this,SIGNAL(reset()),model,SLOT(reset()));
}

void Q_Thread_Widget_Base::done(bool success)
{
    Q_Thread_Base::g_threadThread = nullptr;
    Q_Thread_Base::g_threadStop = true;
    widgetResume();
}
