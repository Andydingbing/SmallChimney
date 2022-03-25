#include "child_widget.h"
#include <QAction>
#include "ericsson_radio_6449_b42_widget.h"
#include "cal_tx_vga.h"
#include "cal_rx_rf_vga.h"
#include "test_tx_aclr.h"
#include "test_rx_gain_accuracy.h"

using namespace ns_ericsson;
using namespace ns_radio_6449;

MODEL_PREFIX(Com_Log_Model,"","Time","Write","Read","Result");
MODEL_ROWCNT(Radio.com_loggers());
MODEL_COLUMN(0,QString("%1").arg(index.row()));
MODEL_COLUMN(1,Radio.com_logger_time(index.row()));
MODEL_COLUMN(2,Radio.com_logger_write(index.row()));
MODEL_COLUMN(3,Radio.com_logger_read(index.row()));
MODEL_COLUMN(4,Radio.com_logger_result(index.row()),end);
MODEL_SUFFIX

static int32_t initCallback()
{
    const double voltageExpect = 48.0;
    const double currentExpect = 15.0;
    double voltage = 0.0;
    double current = 0.0;
    bool en = false;

    Instr.init();
    Instr.has_ps();

    Instr.ps_get_voltage(voltage);
    Instr.ps_get_current(current);
    Instr.ps_get_output_en(en);

    if (en && voltage == voltageExpect && current == currentExpect) {
        return 0;
    }

    Instr.ps_reset();
    Instr.ps_set_voltage(48000);
    Instr.ps_set_current(15000);

    QThread::msleep(1000);
    Instr.ps_set_output_en(true);
    QThread::msleep(1000);

    return 0;
}

ChildWidgets::ChildWidgets(QTreeWidget *treeWidget, QTabWidget *tabWidget) :
    ChildWidgetHelper(treeWidget,tabWidget)
{

}

void ChildWidgets::init()
{
    Radio.set_sn(0,"0001");
    Radio.set_init_callback(initCallback);

    DECL_TREE("Radio,Overview",Q_6449_Widget,Radio.channels());

    DECL_TREE("Test,TX-ACLR",Q_Test_TX_ACLR_Widget,Radio.channels());
    DECL_TREE("Test,RX-Gain Accuracy",Q_Test_RX_Gain_Accuracy_Widget,Radio.channels());

    DECL_TREE("Calibration,TX-VGA",Q_Cal_TX_VGA_Widget,Radio.channels());
    DECL_TREE("Calibration,RX-RF VGA",Q_Cal_RX_RF_VGA_Widget,Radio.channels());

    treeChildItems = treeChildItemsBuiltIn;
    setTree(*tree,treeChildItemsBuiltIn);

//    initMenu();
    initMainLogTabWidget();
}

void ChildWidgets::initMenu(QList<QMenu *> &menus)
{
    // Device
    actionInit = new QAction("Init(&I)");
    actionInit->setStatusTip("Init Device");

    actionInitSilent = new QAction("Init Silent(&S)");
    actionInitSilent->setStatusTip("Init Silent");

//    connect(actionInit,SIGNAL(triggered(bool)),parent,SLOT(deviceInit()));
//    connect(actionInitSilent,SIGNAL(triggered(bool)),parent,SLOT(deviceInitSilent()));

    QAction *separator = new QAction();
    separator->setSeparator(true);

    actionExit = new QAction("Exit(&X)");
    actionExit->setStatusTip("Exit");
//    connect(actionExit,SIGNAL(triggered(bool)),parent,SLOT(exit()));

    menuDevice = new QMenu("Device(&D)");
    menuDevice->addAction(actionInit);
    menuDevice->addAction(actionInitSilent);
    menuDevice->addAction(separator);
    menuDevice->addAction(actionExit);

    // Function
    actionLogConfig = new QAction("Log Config(&L)");
    actionLogViewer = new QAction("Log Viewer(&V)");

    menuFunction = new QMenu("Function(&F)");
    menuFunction->addAction(actionLogConfig);
    menuFunction->addAction(actionLogViewer);

    // About
    actionAbout = new QAction("About(&A)");

    menuHelp = new QMenu("Help(&H)");
    menuHelp->addAction(actionAbout);

    menus.push_back(menuDevice);
    menus.push_back(menuFunction);
    menus.push_back(menuHelp);
}

void ChildWidgets::initMainLogTabWidget()
{
    Com_Log_Model *comLogModel = new Com_Log_Model;
    comLogTableView = new Q_Table_View;
    comLogTableView->setModel(comLogModel);
    comLogTableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    comLogTableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::ResizeToContents);
    comLogTableView->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Stretch);
    comLogTableView->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Stretch);
    comLogTableView->horizontalHeader()->setSectionResizeMode(4,QHeaderView::ResizeToContents);

//    parent->mainLogTab->addTab(comLogTableView,"Com");

    connect(this,SIGNAL(addComLogger(int)),comLogModel,SLOT(update(int)));
    connect(this,SIGNAL(addComLogger(int)),this,SLOT(addComLoggerTableView(int)));

    Radio.set_log_callback(ChildWidgets::addComLoggerCallback);
}

void ChildWidgets::mainTabCurrentChanged(int index)
{

}

void ChildWidgets::addComLoggerCallback()
{
//    emit addComLogger(Radio.com_loggers());
}

void ChildWidgets::addComLoggerTableView(int row)
{
    comLogTableView->scrollToBottom();
    comLogTableView->selectRow(row - 1);
    comLogTableView->resizeRowToContents(row - 1);
}
