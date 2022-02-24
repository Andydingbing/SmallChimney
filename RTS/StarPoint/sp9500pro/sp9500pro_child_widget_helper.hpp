#ifndef SP9500PRO_CHILD_WIDGET_HELPER_HPP
#define SP9500PRO_CHILD_WIDGET_HELPER_HPP

#include "global.h"
#include "child_widget_helper.h"
#include "sp9500x_rrh_widget.h"
#include "sp9500x_sp2406_widget.h"
#include "arb_widget.h"
#include "sp9500x_iq_cap_config_widget.h"
#include "signal_analyzer_widget.h"
#include "sp9500x_fpga_widget.h"
#include "sp9500x_fan_dlg.h"
#include "cal_widget.h"
#include "q_test_dlg.h"

namespace NS_SP9500Pro {

class ChildWidgets : public ChildWidgetHelper
{
    Q_OBJECT

public:
    explicit ChildWidgets(MainWindow *parent) :
        ChildWidgetHelper(parent) {}

    void initMenu()
    {
        QAction *separator = nullptr;
        dlg_Fan = nullptr;

        // Device
        actionInit = new QAction("Init(&I)",parent);
        actionInit->setStatusTip("Init Device");

        actionInitSilent = new QAction("Init Silent(&S)",parent);
        actionInitSilent->setStatusTip("Init Silent");

        connect(actionInit,SIGNAL(triggered(bool)),parent,SLOT(deviceInit()));
        connect(actionInitSilent,SIGNAL(triggered(bool)),parent,SLOT(deviceInitSilent()));

        actionFan = new QAction("Fan Control(&F)",parent);
        actionFan->setStatusTip("Fan Control");
        connect(actionFan,SIGNAL(triggered(bool)),this,SLOT(fanControl()));

        actionPCIE_Restart = new QAction("PCIE Restart(&P)",parent);
        actionPCIE_Restart->setStatusTip("Restart PCIE Devices");

        actionTuneOCXO = new QAction("Tune OCXO(&T)",parent);
        actionTuneOCXO->setStatusTip("Tune OCXO");

        actionExit = new QAction("Exit(&X)",parent);
        actionExit->setStatusTip("Exit");
        connect(actionExit,SIGNAL(triggered(bool)),parent,SLOT(exit()));

        menuDevice = new QMenu("Device(&D)",ui->menuBar);
        menuDevice->addAction(actionInit);
        menuDevice->addAction(actionInitSilent);

        separator = new QAction(parent);
        separator->setSeparator(true);
        menuDevice->addAction(separator);

        menuDevice->addAction(actionFan);
        menuDevice->addAction(actionPCIE_Restart);
        menuDevice->addAction(actionTuneOCXO);
        menuDevice->addAction(actionExit);

        // Function
        actionArbViewer = new QAction("Arb Viewer(&A)",parent);

        actionStarAllIQ_Capture = new QAction("Start All IQ Capture(&R)",parent);
        connect(actionStarAllIQ_Capture,SIGNAL(triggered(bool)),parent,SLOT(starAllIQCapture()));

        actionStopAllIQ_Capture = new QAction("Stop All IQ Capture(&O)",parent);
        connect(actionStopAllIQ_Capture,SIGNAL(triggered(bool)),parent,SLOT(stopAllIQCapture()));

        actionLogConfig = new QAction("Log Config(&L)",parent);
        actionLogViewer = new QAction("Log Viewer(&V)",parent);

        menuFunction = new QMenu("Function(&F)",ui->menuBar);
        menuFunction->addAction(actionArbViewer);
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

//        DECL_TREE_ITEM(str,"RF-R1B" << "Overview",widget_SP1403_R1B);
        DECL_TREE_ITEM(str,"RF-R1B" << "Test",widget_Test_R1A);
//        DECL_TREE_ITEM(str,"RF-R1B" << "Calibration" << "TX-Filter",widget_Cal_TXFilter);
        DECL_TREE_ITEM(str,"RRH" << "Overview",widget_SP9500X_RRH);
//        DECL_TREE_ITEM(str,"BB-Debug" << "Overview",widget_SP2406);
        DECL_TREE_ITEM(str,"BB-Debug" << "ARB",widget_ARB);
        DECL_TREE_ITEM(str,"BB-Debug" << "Signal Analyzer",widget_SA);
        DECL_TREE_ITEM(str,"BB-Debug" << "FPGA",widget_FPGA);
//        DECL_TREE_ITEM(str,"RF-R1F" << "Test",widget_Test_R1C);
//        DECL_TREE_ITEM(str,"RF-R1F" << "Calibration",widget_Cal_R1C);
//        DECL_TREE_ITEM(str,"RF-R1F" << "Calibration" << "TX-LOLeakage",widget_Cal_R1C_TX_LOLeak);
//        DECL_TREE_ITEM(str,"RF-R1F" << "Calibration" << "TX-Sideband",widget_Cal_R1C_TX_SB);
//        DECL_TREE_ITEM(str,"RF-R1F" << "Calibration" << "TX-Filter",widget_Cal_R1C_TX_Filter);

//        DECL_TREE_ITEM(str,"BB-Debug" << "OverView",widget_SP2401_R1A);

        setMainTree(_items);
    }

    void addChildWidgets()
    {
//        ADD_CHILD_WIDGET(widget_SP1403_R1B,Q_SP9500PRO_SP1403_R1B_Widget,ns_sp3103::g_max_rf);
//        ADD_CHILD_WIDGET(widget_Cal_TXFilter,Q_Cal_TXFilter_Widget,ns_sp3103::g_max_rf);
        ADD_CHILD_WIDGET(widget_Test_R1A,Q_Test_SP9501_TabWidget,ns_sp3103::g_max_rf);
        ADD_CHILD_WIDGET(widget_SP9500X_RRH,Q_SP9500X_RRH_Widget,ns_sp3103::g_max_rf);
//        ADD_CHILD_WIDGET(widget_SP2406,Q_SP2406_Widget,ns_sp3103::g_max_rf);
        ADD_CHILD_WIDGET(widget_ARB,Q_ARB_Widget,ns_sp3103::g_max_rf);
        ADD_CHILD_WIDGET(widget_SA,Q_SA_Widget,ns_sp3103::g_max_rf);
        ADD_CHILD_WIDGET(widget_FPGA,NS_SP9500X::Q_FPGA_Widget,1);
    }

    void updatePtr()
    {
//        if (SP3103 != nullptr) {
//            RFUIdx = SP3103->rfu_idx();
//            SP1403 = SP3103->working_sp1403(RFIdx);
//            SP1403_R1B = SP3103->working_sp1403_r1b(RFIdx);
//            SP9500X_RRH = SP3103->working_rrh(RFIdx);
//            SP2406 = SP3103->working_sp2406(RFIdx);
//            complexSequence = SP2406->ul_sequence();
//            widget_SA[RFIdx]->_data_I._sequence = complexSequence;
//            widget_SA[RFIdx]->_data_Q._sequence = complexSequence;
//            widget_SA[RFIdx]->_data_DFT._sequence = complexSequence;
//        }
    }

    void mainTabCurrentChanged(int index)
    {
        complexSequence->_new(widget_SA[index]->widget_Config[project]->samples());
    }

public slots:
    void fanControl()
    {
        if (dlg_Fan == nullptr) {
            dlg_Fan = new NS_SP9500X::Q_Fan_Dlg;
            dlg_Fan->setWindowTitle("SP9500X Fan Control");
        }
        dlg_Fan->setVisible(true);
        dlg_Fan->showNormal();
        dlg_Fan->activateWindow();
    }

    void initChildWidgets()
    {
        updatePtr();
        for (int i = 0;i < ns_sp3103::g_max_rf;i ++) {
//            widget_SP1403_R1B[i]->init();
            widget_SP9500X_RRH[i]->init();
//            widget_SP2406[i]->init();
            widget_ARB[i]->init();
            widget_SA[i]->init();
        }
        updatePtr();
    }

public:
//    QList<Q_SP9500PRO_SP1403_R1B_Widget *> widget_SP1403_R1B;
    QList<Q_SP9500X_RRH_Widget *> widget_SP9500X_RRH;

//    QList<Q_SP2406_Widget *> widget_SP2406;
    QList<Q_ARB_Widget *> widget_ARB;
    QList<Q_SA_Widget *> widget_SA;
    QList<NS_SP9500X::Q_FPGA_Widget *> widget_FPGA;

    QList<Q_Test_SP9501_TabWidget *> widget_Test_R1A;

    NS_SP9500X::Q_Fan_Dlg *dlg_Fan;

private:
    QMenu *menuDevice;
    QAction *actionInit;
    QAction *actionInitSilent;
    QAction *actionFan;
    QAction *actionPCIE_Restart;
    QAction *actionTuneOCXO;
    QAction *actionExit;

    QMenu *menuFunction;
    QAction *actionArbViewer;
    QAction *actionStarAllIQ_Capture;
    QAction *actionStopAllIQ_Capture;
    QAction *actionLogConfig;
    QAction *actionLogViewer;

    QMenu *menuHelp;
    QAction *actionAbout;
};

} // namespace NS_SP9500Pro

#endif // SP9500PRO_CHILD_WIDGET_HELPER_HPP
