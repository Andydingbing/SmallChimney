#include "ericsson_air_3268_b42_child_widget.h"
#include "ericsson_air_3268_b42_widget.h"

using namespace ns_ericsson;
using namespace ns_air_3268;

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
    QThread::msleep(40000);
    return 0;
}

ChildWidgets::ChildWidgets(MainWindow *parent) :
    ChildWidgetHelper(parent)
{

}

void ChildWidgets::init()
{
    Radio.set_sn("0001");
    Radio.set_init_callback(initCallback);

    DECL_TREE("Radio,Overview",Q_3268_Widget,Radio.channels());

//    DECL_TREE("Test" << "TX-ACLR",Q_Test_TX_ACLR_Widget,Radio.channels());
//    DECL_TREE("Test" << "RX-Gain Accuracy",Q_Test_RX_Gain_Accuracy_Widget,Radio.channels());

//    DECL_TREE("Calibration" << "TX-VGA",Q_Cal_TX_VGA_Widget,Radio.channels());
//    DECL_TREE("Calibration" << "RX-RF VGA",Q_Cal_RX_RF_VGA_Widget,Radio.channels());

    setMainTree(treeChildItemsBuiltIn);

    initMenu();
    initMainLogTabWidget();
}

void ChildWidgets::initMenu()
{
    QAction *separator = nullptr;

    // Device
    actionInit = new QAction("Init(&I)",parent);
    actionInit->setStatusTip("Init Device");

    actionInitSilent = new QAction("Init Silent(&S)",parent);
    actionInitSilent->setStatusTip("Init Silent");

    connect(actionInit,SIGNAL(triggered(bool)),parent,SLOT(deviceInit()));
    connect(actionInitSilent,SIGNAL(triggered(bool)),parent,SLOT(deviceInitSilent()));

    actionExit = new QAction("Exit(&X)",parent);
    actionExit->setStatusTip("Exit");
    connect(actionExit,SIGNAL(triggered(bool)),parent,SLOT(exit()));

    menuDevice = new QMenu("Device(&D)",ui->menuBar);
    menuDevice->addAction(actionInit);
    menuDevice->addAction(actionInitSilent);

    separator = new QAction(parent);
    separator->setSeparator(true);
    menuDevice->addAction(separator);
    menuDevice->addAction(actionExit);

    // Function
    actionLogConfig = new QAction("Log Config(&L)",parent);
    actionLogViewer = new QAction("Log Viewer(&V)",parent);

    menuFunction = new QMenu("Function(&F)",ui->menuBar);
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

    parent->mainLogTab->addTab(comLogTableView,"Com");

    connect(this,SIGNAL(addComLogger(int)),comLogModel,SLOT(update(int)));
    connect(this,SIGNAL(addComLogger(int)),this,SLOT(addComLoggerTableView(int)));

    Radio.set_log_callback(ChildWidgets::addComLoggerCallback);
}

void ChildWidgets::mainTabCurrentChanged(int index)
{

}

void ChildWidgets::addComLoggerCallback()
{
    ChildWidgets *childs = dynamic_cast<ChildWidgets *>(g_MainW->childWidgets.at(Ericsson_Radio_4415_B3));
    emit childs->addComLogger(Radio.com_loggers());
}

void ChildWidgets::addComLoggerTableView(int row)
{
    comLogTableView->scrollToBottom();
    comLogTableView->selectRow(row - 1);
    comLogTableView->resizeRowToContents(row - 1);
}
