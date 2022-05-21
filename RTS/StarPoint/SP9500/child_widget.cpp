#include "child_widget.h"
#include "fmt/core.h"

#include "sp1401_container_widget.h"
#include "sp1401_r1a_widget.h"
#include "sp1401_r1c_widget.h"
#include "sp1401_r1f_widget.h"
#include "sp1401_r1a_adv_widget.h"
#include "sp1401_r1c_adv_widget.h"
#include "sp1401_r1f_adv_widget.h"

#include "cal_tx_lo_leak.h"
#include "cal_tx_sideband.h"

typedef Q_SP1401_Container_Widget<Q_SP1401_R1A_Widget> Q_SP1401_R1A_Container_Widget;
typedef Q_SP1401_Container_Widget<Q_SP1401_R1C_Widget> Q_SP1401_R1C_Container_Widget;
typedef Q_SP1401_Container_Widget<Q_SP1401_R1F_Widget> Q_SP1401_R1F_Container_Widget;

using namespace std;

BOOST_DLL_ALIAS(ChildWidgets::create,create_plugin)

ChildWidgets *ChildWidgets::create()
{
    ChildWidgets *cw = new ChildWidgets();
    return cw;
}

ChildWidgets::ChildWidgets() : Plugin()
{
}

string ChildWidgets::version()
{
    return "1.0";
}

string ChildWidgets::projectMenu()
{
    return "StarPoint,SP9500";
}

string ChildWidgets::tabName(int idx)
{
    return fmt::format("RF-{:d}",idx);
}

void ChildWidgets::tabCurrentChanged(int index)
{

}

void ChildWidgets::init()
{
    DECL_TREE("RF-R1F,Overview",Q_SP1401_R1F_Container_Widget,g_max_rf);
    DECL_TREE("RF-R1F,Advance",Q_SP1401_R1F_Adv_Widget,g_max_rf);
//    DECL_TREE("RF-R1F,Test",widget_Test_R1C);
//    DECL_TREE("RF-R1F,Calibration",widget_Cal_R1C);
    DECL_TREE("RF-R1F,Calibration,TX-LOLeakage",Q_Cal_TX_LO_Leak_Widget,g_max_rf);
    DECL_TREE("RF-R1F,Calibration,TX-Sideband",Q_Cal_TX_Sideband_Widget,g_max_rf);
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

//    DECL_TREE("BB-Debug,OverView",Q_StarPoint_SP9500_SP2401_R1A_Widget,g_max_rf);
//    DECL_TREE("BB-Debug,Arb",Q_ARB_Widget,g_max_rf);
//    DECL_TREE("BB-Debug,IQ-Capture",Q_SA_Widget,g_max_rf);
//    DECL_TREE("BB-Debug,FPGA",Q_StarPoint_SP9500_FPGA_Widget,1);

    DECL_TREE("RF-R1C/D/E,Overview",Q_SP1401_R1C_Container_Widget,g_max_rf);
    DECL_TREE("RF-R1C/D/E,Advance",Q_SP1401_R1C_Adv_Widget,g_max_rf);
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
    DECL_TREE("RF-R1A/B,Advance",Q_SP1401_R1A_Adv_Widget,g_max_rf);

    _treeChildItems = _treeChildItemsBuiltIn;
}

void ChildWidgets::initMenu(QList<QMenu *> &menus)
{
    // Device
    actionInit = new QAction("Init(&I)");
    actionInit->setStatusTip("Init Device");
//    connect(actionInit,SIGNAL(triggered(bool)),parent,SLOT(deviceInit()));

    actionPCIE_Restart = new QAction("PCIE Restart(&P)");
    actionPCIE_Restart->setStatusTip("Restart PCIE Devices");

    actionTuneOCXO = new QAction("Tune OCXO(&T)");
    actionTuneOCXO->setStatusTip("Tune OCXO");

    actionExit = new QAction("Exit(&X)");
    actionExit->setStatusTip("Exit");
//    connect(actionExit,SIGNAL(triggered(bool)),parent,SLOT(exit()));

    menuDevice = new QMenu("Device(&D)");
    menuDevice->addAction(actionInit);
    menuDevice->addAction(actionPCIE_Restart);
    menuDevice->addAction(actionTuneOCXO);
    menuDevice->addAction(actionExit);

    // RFU
    actionRFU0 = new QAction("RFU0(&0)");
    actionRFU0->setCheckable(true);

    actionRFU1 = new QAction("RFU1(&1)");
    actionRFU1->setCheckable(true);

    actionRFU2 = new QAction("RFU2(&2)");
    actionRFU2->setCheckable(true);

    actionRFU3 = new QAction("RFU3(&3)");
    actionRFU3->setCheckable(true);

    actionRFU4 = new QAction("RFU4(&4)");
    actionRFU4->setCheckable(true);

//    connect(actionRFU0,SIGNAL(triggered(bool)),this,SLOT(selSP3301_0()));
//    connect(actionRFU1,SIGNAL(triggered(bool)),this,SLOT(selSP3301_1()));
//    connect(actionRFU2,SIGNAL(triggered(bool)),this,SLOT(selSP3301_2()));
//    connect(actionRFU3,SIGNAL(triggered(bool)),this,SLOT(selSP3301_3()));
//    connect(actionRFU4,SIGNAL(triggered(bool)),this,SLOT(selSP3301_4()));

    actionRFU2->setChecked(true);

    menuRFU = new QMenu("RFU(&R)");
    menuRFU->addAction(actionRFU0);
    menuRFU->addAction(actionRFU1);
    menuRFU->addAction(actionRFU2);
    menuRFU->addAction(actionRFU3);
    menuRFU->addAction(actionRFU4);

    // Function
    actionArbViewer = new QAction("Arb Viewer(&A)");

    actionStarAllIQ_Capture = new QAction("Start All IQ Capture(&R)");
//    connect(actionStarAllIQ_Capture,SIGNAL(triggered(bool)),parent,SLOT(starAllIQCapture()));

    actionStopAllIQ_Capture = new QAction("Stop All IQ Capture(&O)");
//    connect(actionStopAllIQ_Capture,SIGNAL(triggered(bool)),parent,SLOT(stopAllIQCapture()));

    menuFunction = new QMenu("Function(&F)");
    menuFunction->addAction(actionArbViewer);
    menuFunction->addAction(actionStarAllIQ_Capture);
    menuFunction->addAction(actionStopAllIQ_Capture);

    // Test
    actionProgramFPGAbit = new QAction("Program FPGA bit(&P)");
    actionAlgorithm_5355 = new QAction("Algorithm 5355(&A)");
    actionFittingInterpolation = new QAction("Fitting & Interpolation(&F)");

    menuTest = new QMenu("Test(&T)");
    menuTest->addAction(actionProgramFPGAbit);
    menuTest->addAction(actionAlgorithm_5355);
    menuTest->addAction(actionFittingInterpolation);

    // About
    actionAbout = new QAction("About(&A)");

    menuHelp = new QMenu("Help(&H)");
    menuHelp->addAction(actionAbout);

    menus.push_back(menuDevice);
    menus.push_back(menuRFU);
    menus.push_back(menuFunction);
    menus.push_back(menuTest);
    menus.push_back(menuHelp);
}
