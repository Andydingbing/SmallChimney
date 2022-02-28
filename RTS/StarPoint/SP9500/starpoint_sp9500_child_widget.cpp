#include "starpoint_sp9500_global.h"
#include "starpoint_sp9500_child_widget.h"
#include "starpoint_sp9500_sp1401_container_widget.h"
#include "starpoint_sp9500_sp1401_r1a_widget.h"
#include "starpoint_sp9500_sp1401_r1c_widget.h"
#include "starpoint_sp9500_sp1401_r1f_widget.h"
#include "starpoint_sp9500_sp1401_r1a_adv_widget.h"
#include "starpoint_sp9500_sp1401_r1c_adv_widget.h"
#include "starpoint_sp9500_sp1401_r1f_adv_widget.h"
#include "starpoint_sp9500_sp2401_r1a_widget.h"
#include "../starpoint_arb_widget.h"
#include "../starpoint_signal_analyzer_widget.h"
#include "starpoint_sp9500_fpga_widget.h"
#include "starpoint_sp9500_cal_tx_lo_leak.h"

typedef Q_SP1401_Container_Widget<Q_StarPoint_SP9500_SP1401_R1A_Widget> Q_SP1401_R1A_Container_Widget;
typedef Q_SP1401_Container_Widget<Q_StarPoint_SP9500_SP1401_R1C_Widget> Q_SP1401_R1C_Container_Widget;
typedef Q_SP1401_Container_Widget<Q_StarPoint_SP9500_SP1401_R1F_Widget> Q_SP1401_R1F_Container_Widget;

ChildWidgets::ChildWidgets(MainWindow *parent) :
    ChildWidgetHelper(parent)
{

}

void ChildWidgets::init()
{
    DECL_TREE("RF-R1F,Overview",Q_SP1401_R1F_Container_Widget,g_max_rf);
    DECL_TREE("RF-R1F,Advance",Q_StarPoint_SP9500_SP1401_R1F_Adv_Widget,g_max_rf);
//    DECL_TREE("RF-R1F,Test",widget_Test_R1C);
//    DECL_TREE("RF-R1F,Calibration",widget_Cal_R1C);
    DECL_TREE("RF-R1F,Calibration,TX-LOLeakage",Q_Cal_TX_LO_Leak_Widget,g_max_rf);
//    DECL_TREE("RF-R1F,Calibration,TX-Sideband",widget_Cal_R1C_TX_SB);
//    DECL_TREE("RF-R1F,Calibration,TX-Filter",widget_Cal_R1C_TX_Filter);
//    DECL_TREE("RF-R1F,Calibration,TX-BasePower",widget_Cal_R1C_TX_Pwr);
//    DECL_TREE("RF-R1F,Calibration,TX-Attenuation",widget_Cal_R1C_TX_Att);
//    DECL_TREE("RF-R1F,Calibration,TX-FilterOffset",widget_Cal_R1C_TX_FilterOffset);
//    DECL_TREE("RF-R1F,Calibration,RX-Filter",widget_Cal_R1C_RX_Filter);
//    DECL_TREE("RF-R1F,Calibration,RX-Reference",widget_Cal_R1C_RX_Ref);
//    DECL_TREE("RF-R1F,Calibration,RX-Attenuation",widget_Cal_R1C_RX_Att);
//    DECL_TREE("RF-R1F,Calibration,RX-FilterOffset",widget_Cal_R1C_RX_FilterOffset);
//    DECL_TREE("RF-R1F,Calibration,TX-TempComp",widget_Cal_R1C_TX_TempComp);
//    DECL_TREE("RF-R1F,Calibration,RX-TempComp",widget_Cal_R1C_RX_TempComp);

    DECL_TREE("BB-Debug,OverView",Q_StarPoint_SP9500_SP2401_R1A_Widget,g_max_rf);
    DECL_TREE("BB-Debug,Arb",Q_ARB_Widget,g_max_rf);
    DECL_TREE("BB-Debug,IQ-Capture",Q_SA_Widget,g_max_rf);
    DECL_TREE("BB-Debug,FPGA",Q_StarPoint_SP9500_FPGA_Widget,1);

    DECL_TREE("RF-R1C/D/E,Overview",Q_SP1401_R1C_Container_Widget,g_max_rf);
    DECL_TREE("RF-R1C/D/E,Advance",Q_StarPoint_SP9500_SP1401_R1C_Adv_Widget,g_max_rf);
//    DECL_TREE("RF-R1C/D/E,Test",widget_Test_R1C);
//    DECL_TREE("RF-R1C/D/E,Calibration,TX-LOLeakage",widget_Cal_R1C_TX_LOLeak);
//    DECL_TREE("RF-R1C/D/E,Calibration,TX-Sideband",widget_Cal_R1C_TX_SB);
//    DECL_TREE("RF-R1C/D/E,Calibration,TX-Filter",widget_Cal_R1C_TX_Filter);
//    DECL_TREE("RF-R1C/D/E,Calibration,TX-BasePower",widget_Cal_R1C_TX_Pwr);
//    DECL_TREE("RF-R1C/D/E,Calibration,TX-Attenuation",widget_Cal_R1C_TX_Att);
//    DECL_TREE("RF-R1C/D/E,Calibration,TX-FilterOffset",widget_Cal_R1C_TX_FilterOffset);
//    DECL_TREE("RF-R1C/D/E,Calibration,RX-Filter",widget_Cal_R1C_RX_Filter);
//    DECL_TREE("RF-R1C/D/E,Calibration,RX-Reference",widget_Cal_R1C_RX_Ref);
//    DECL_TREE("RF-R1C/D/E,Calibration,RX-Attenuation",widget_Cal_R1C_RX_Att);
//    DECL_TREE("RF-R1C/D/E,Calibration,RX-FilterOffset",widget_Cal_R1C_RX_FilterOffset);
//    DECL_TREE("RF-R1C/D/E,Calibration,TX-TempComp",widget_Cal_R1C_TX_TempComp);
//    DECL_TREE("RF-R1C/D/E,Calibration,RX-TempComp",widget_Cal_R1C_RX_TempComp);

    DECL_TREE("RF-R1A/B,Overview",Q_SP1401_R1A_Container_Widget,g_max_rf);
    DECL_TREE("RF-R1A/B,Advance",Q_StarPoint_SP9500_SP1401_R1A_Adv_Widget,g_max_rf);

    treeChildItems = treeChildItemsBuiltIn;
    setMainTree(treeChildItemsBuiltIn);

    initMenu();
}

void ChildWidgets::initMenu()
{
    // Device
    actionInit = new QAction("Init(&I)",parent);
    actionInit->setStatusTip("Init Device");
    connect(actionInit,SIGNAL(triggered(bool)),parent,SLOT(deviceInit()));

    actionPCIE_Restart = new QAction("PCIE Restart(&P)",parent);
    actionPCIE_Restart->setStatusTip("Restart PCIE Devices");

    actionTuneOCXO = new QAction("Tune OCXO(&T)",parent);
    actionTuneOCXO->setStatusTip("Tune OCXO");

    actionExit = new QAction("Exit(&X)",parent);
    actionExit->setStatusTip("Exit");
    connect(actionExit,SIGNAL(triggered(bool)),parent,SLOT(exit()));

    menuDevice = new QMenu("Device(&D)",ui->menuBar);
    menuDevice->addAction(actionInit);
    menuDevice->addAction(actionPCIE_Restart);
    menuDevice->addAction(actionTuneOCXO);
    menuDevice->addAction(actionExit);

    // RFU
    actionRFU0 = new QAction("RFU0(&0)",parent);
    actionRFU0->setCheckable(true);

    actionRFU1 = new QAction("RFU1(&1)",parent);
    actionRFU1->setCheckable(true);

    actionRFU2 = new QAction("RFU2(&2)",parent);
    actionRFU2->setCheckable(true);

    actionRFU3 = new QAction("RFU3(&3)",parent);
    actionRFU3->setCheckable(true);

    actionRFU4 = new QAction("RFU4(&4)",parent);
    actionRFU4->setCheckable(true);

    connect(actionRFU0,SIGNAL(triggered(bool)),this,SLOT(selSP3301_0()));
    connect(actionRFU1,SIGNAL(triggered(bool)),this,SLOT(selSP3301_1()));
    connect(actionRFU2,SIGNAL(triggered(bool)),this,SLOT(selSP3301_2()));
    connect(actionRFU3,SIGNAL(triggered(bool)),this,SLOT(selSP3301_3()));
    connect(actionRFU4,SIGNAL(triggered(bool)),this,SLOT(selSP3301_4()));

    actionRFU2->setChecked(true);

    menuRFU = new QMenu("RFU(&R)",ui->menuBar);
    menuRFU->addAction(actionRFU0);
    menuRFU->addAction(actionRFU1);
    menuRFU->addAction(actionRFU2);
    menuRFU->addAction(actionRFU3);
    menuRFU->addAction(actionRFU4);

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

    // Test
    actionProgramFPGAbit = new QAction("Program FPGA bit(&P)",parent);
    actionAlgorithm_5355 = new QAction("Algorithm 5355(&A)",parent);
    actionFittingInterpolation = new QAction("Fitting & Interpolation(&F)",parent);

    menuTest = new QMenu("Test(&T)",ui->menuBar);
    menuTest->addAction(actionProgramFPGAbit);
    menuTest->addAction(actionAlgorithm_5355);
    menuTest->addAction(actionFittingInterpolation);

    // About
    actionAbout = new QAction("About(&A)",parent);

    menuHelp = new QMenu("Help(&H)",ui->menuBar);
    menuHelp->addAction(actionAbout);

    // Menu
    ui->menuBar->addAction(menuDevice->menuAction());
    ui->menuBar->addAction(menuRFU->menuAction());
    ui->menuBar->addAction(menuFunction->menuAction());
    ui->menuBar->addAction(menuTest->menuAction());
    ui->menuBar->addAction(menuHelp->menuAction());
}

void ChildWidgets::mainTabCurrentChanged(int index)
{
//    if (complexSequence == nullptr) {
//        return;
//    }

//    complexSequence->_new(widget_IQ_Cap[index]->widget_Config[project]->samples());

//    for (quint8 i = 0;i < ns_sp9500::g_max_rf;++i) {
//        if (i != index) {
//            SP3301->set_iq_cap_buffer(i,nullptr,nullptr);
//        } else {
//            SP3301->set_iq_cap_buffer(i,complexSequence->i(),complexSequence->q());
//        }
//    }
}



//        ADD_CHILD_WIDGET(widget_Cal_R1C,QCalR1CDlg,ns_sp9500::g_max_rf)
//        ADD_CHILD_WIDGET(widget_Cal_R1C_TX_LOLeak,QCalR1CTXLOLeakDlg,ns_sp9500::g_max_rf)
//        ADD_CHILD_WIDGET(widget_Cal_R1C_TX_SB,QCalR1CTXSBDlg,ns_sp9500::g_max_rf)
//        ADD_CHILD_WIDGET(widget_Cal_R1C_TX_Filter,QCalR1CTXFilterDlg,ns_sp9500::g_max_rf)
//        ADD_CHILD_WIDGET(widget_Cal_R1C_TX_Pwr,QCalR1CTXPwrDlg,ns_sp9500::g_max_rf)
//        ADD_CHILD_WIDGET(widget_Cal_R1C_TX_Att,QCalR1CTXAttDlg,ns_sp9500::g_max_rf)
//        ADD_CHILD_WIDGET(widget_Cal_R1C_TX_FilterOffset,QCalR1CTXFilterOffsetDlg,ns_sp9500::g_max_rf)
//        ADD_CHILD_WIDGET(widget_Cal_R1C_RX_Filter,QCalR1CRXFilterDlg,ns_sp9500::g_max_rf)
//        ADD_CHILD_WIDGET(widget_Cal_R1C_RX_Ref,QCalR1CRXRefDlg,ns_sp9500::g_max_rf)
//        ADD_CHILD_WIDGET(widget_Cal_R1C_RX_Att,QCalR1CRXAttDlg,ns_sp9500::g_max_rf)
//        ADD_CHILD_WIDGET(widget_Cal_R1C_RX_FilterOffset,QCalR1CRXFilterOffsetDlg,ns_sp9500::g_max_rf)
//        ADD_CHILD_WIDGET(widget_Cal_R1C_TX_TempComp,QCalR1CTxTempCompDlg,ns_sp9500::g_max_rf)
//        ADD_CHILD_WIDGET(widget_Cal_R1C_RX_TempComp,QCalR1CRXTempCompDlg,ns_sp9500::g_max_rf)
//        ADD_CHILD_WIDGET(widget_Test_R1C,QTestR1CTabWidget,ns_sp9500::g_max_rf)


    void ChildWidgets::updatePtr()
    {
//        if (SP3301 != nullptr) {
//            RFUIdx = SP3301->get_rfu_idx();
//            SP1401 = SP3301->working_sp1401(RFIdx);
//            SP1401_R1A = SP3301->get_sp1401_r1a(RFIdx);
//            SP1401_R1B = SP3301->get_sp1401_r1b(RFIdx);
//            SP1401_R1C = SP3301->get_sp1401_r1c(RFIdx);
//            SP1401_R1D = SP3301->get_sp1401_r1d(RFIdx);
//            SP1401_R1E = SP3301->get_sp1401_r1e(RFIdx);
//            SP1401_R1F = SP3301->get_sp1401_r1f(RFIdx);
//            SP2401 = SP3301->get_sp2401(RFIdx);
//            widget_IQ_Cap[RFIdx]->_data_I._sequence = complexSequence;
//            widget_IQ_Cap[RFIdx]->_data_Q._sequence = complexSequence;
//            widget_IQ_Cap[RFIdx]->_data_DFT._sequence = complexSequence;
//        }
    }

    void ChildWidgets::initChildWidgets()
    {
//        updatePtr();
//        for (quint8 i = 0;i < ns_sp9500::g_max_rf;++i) {
//            widget_SP1401_R1A_Container[i]->widget->init();
//            widget_SP1401_R1C_Container[i]->widget->init();
//            widget_SP1401_R1F_Container[i]->widget->init();
//            widget_ARB[i]->init();
//            widget_IQ_Cap[i]->init();
//        }
//        updatePtr();
//        mainTabCurrentChanged(0);
    }

    void ChildWidgets::selSP3301_0()
    {
        SP3301 = &SP3301_0;
        updatePtr();
        actionRFU0->setChecked(true);
        actionRFU1->setChecked(false);
        actionRFU2->setChecked(false);
        actionRFU3->setChecked(false);
        actionRFU4->setChecked(false);
        Log.add_msg(0,"Working on RFU0");
    }

    void ChildWidgets::selSP3301_1()
    {
        SP3301 = &SP3301_1;
        updatePtr();
        actionRFU0->setChecked(false);
        actionRFU1->setChecked(true);
        actionRFU2->setChecked(false);
        actionRFU3->setChecked(false);
        actionRFU4->setChecked(false);
        Log.add_msg(0,"Working on RFU1");
    }

    void ChildWidgets::selSP3301_2()
    {
        SP3301 = &SP3301_2;
        updatePtr();
        actionRFU0->setChecked(false);
        actionRFU1->setChecked(false);
        actionRFU2->setChecked(true);
        actionRFU3->setChecked(false);
        actionRFU4->setChecked(false);
        Log.add_msg(0,"Working on RFU2");
    }

    void ChildWidgets::selSP3301_3()
    {
        SP3301 = &SP3301_3;
        updatePtr();
        actionRFU0->setChecked(false);
        actionRFU1->setChecked(false);
        actionRFU2->setChecked(false);
        actionRFU3->setChecked(true);
        actionRFU4->setChecked(false);
        Log.add_msg(0,"Working on RFU3");
    }

    void ChildWidgets::selSP3301_4()
    {
        SP3301 = &SP3301_4;
        updatePtr();
        actionRFU0->setChecked(false);
        actionRFU1->setChecked(false);
        actionRFU2->setChecked(false);
        actionRFU3->setChecked(false);
        actionRFU4->setChecked(true);
        Log.add_msg(0,"Working on RFU4");
    }

