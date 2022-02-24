#ifndef DT3308_CHILD_WIDGET_HELPER_HPP
#define DT3308_CHILD_WIDGET_HELPER_HPP

#include "child_widget_helper.h"
#include "dt3308_rf_f_widget.h"
#include "dt3308_bb_f_widget.h"
#include "signal_analyzer_widget.h"

namespace NS_DT3308 {

class ChildWidgets : public ChildWidgetHelper
{
    Q_OBJECT

public:
    explicit ChildWidgets(MainWindow *parent) :
        ChildWidgetHelper(parent) {}

    void initMenu()
    {
        // Device
        actionInit = new QAction("Init(&I)",parent);
        actionInit->setStatusTip("Init Device");
        connect(actionInit,SIGNAL(triggered(bool)),parent,SLOT(deviceInit()));

        actionExit = new QAction("Exit(&X)",parent);
        actionExit->setStatusTip("Exit");
        connect(actionExit,SIGNAL(triggered(bool)),parent,SLOT(exit()));

        menuDevice = new QMenu("Device(&D)",ui->menuBar);
        menuDevice->addAction(actionInit);
        menuDevice->addAction(actionExit);

        // Function
        actionStarAllIQ_Capture = new QAction("Start All IQ Capture(&R)",parent);
        connect(actionStarAllIQ_Capture,SIGNAL(triggered(bool)),parent,SLOT(starAllIQCapture()));

        actionStopAllIQ_Capture = new QAction("Stop All IQ Capture(&O)",parent);
        connect(actionStopAllIQ_Capture,SIGNAL(triggered(bool)),parent,SLOT(stopAllIQCapture()));

        actionLogConfig = new QAction("Log Config(&L)",parent);
        actionLogViewer = new QAction("Log Viewer(&V)",parent);

        menuFunction = new QMenu("Function(&F)",ui->menuBar);
        menuFunction->addAction(actionStarAllIQ_Capture);
        menuFunction->addAction(actionStopAllIQ_Capture);
        menuFunction->addAction(actionLogConfig);
        menuFunction->addAction(actionLogViewer);

        // About
        actionAbout = new QAction("About(&A)",parent);

        menuHelp = new QMenu("Help(&H)",ui->menuBar);
        menuHelp->addAction(actionAbout);

        // Menu
        ui->menuBar->addAction(menuDevice->menuAction());
        ui->menuBar->addAction(menuFunction->menuAction());
        ui->menuBar->addAction(menuHelp->menuAction());
    }

    void initMainTreeWidget()
    {
        QStringList str;

        DECL_TREE_ITEM(str,"RF-F" << "Overview",widget_RF_F);
        DECL_TREE_ITEM(str,"BB-Debug" << "OverView",widget_BB_F);
        DECL_TREE_ITEM(str,"BB-Debug" << "IQ-Capture",widget_IQ_Cap);

        setMainTree(_items);
    }

    void addChildWidgets()
    {
        ADD_CHILD_WIDGET(widget_RF_F,Q_DT3308_RF_F_Widget,1);
        ADD_CHILD_WIDGET(widget_BB_F,Q_DT3308_BB_F_Widget,1);
        ADD_CHILD_WIDGET(widget_IQ_Cap,Q_SA_Widget,1);
    }

public:
    QList<Q_DT3308_RF_F_Widget *> widget_RF_F;
    QList<Q_DT3308_BB_F_Widget *> widget_BB_F;
    QList<Q_SA_Widget *>          widget_IQ_Cap;

private:
    QMenu *menuDevice;
    QAction *actionInit;
    QAction *actionExit;

    QMenu *menuFunction;
    QAction *actionStarAllIQ_Capture;
    QAction *actionStopAllIQ_Capture;
    QAction *actionLogConfig;
    QAction *actionLogViewer;

    QMenu *menuHelp;
    QAction *actionAbout;
};

} // namespace NS_DT3308

#endif // DT3308_CHILD_WIDGET_HELPER_HPP
