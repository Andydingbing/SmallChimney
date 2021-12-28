#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QToolBar>
#include <QLabel>
#include <QVBoxLayout>
#include <QMetaType>
#include <QMessageBox>
#include <QTreeWidgetItem>
#include <QProgressBar>
#include <QAction>
#include <QMenu>
#include <QStyleFactory>
#include "device.h"
#include "Ericsson/Radio_4415/ericsson_radio_4415_b3_child_widget.h"
#include "Ericsson/Air_3268/ericsson_air_3268_b42_child_widget.h"
#include "log_model.hpp"
#include "device_init_thread.h"
#include "scroll_lineedit.h"

#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
    #define setSectionResizeMode setResizeMode
#endif

QHBoxLayout MainWindow::childDlgLayout;
MainWindow *g_MainW = nullptr;

double QFreqScrollLineEdit::stepDefault = 1e6;
double QFreqScrollLineEdit::stepCtrl    = 5e6;
double QFreqScrollLineEdit::stepShift   = 10e6;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    currentWidgets(nullptr),
    ui(new Ui::MainWindow),
    thread(nullptr)
{
    ui->setupUi(this);

    Q_Thread::registerMetaType();

    actionSPC = new QAction;
    actionSPC->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    connect(actionSPC,SIGNAL(triggered()),this,SLOT(threadSPC()));

    actionStop = new QAction;
    actionStop->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    connect(actionStop,SIGNAL(triggered()),this,SLOT(threadStop()));

    QToolBar *toolBar = addToolBar("");
    toolBar->addAction(actionSPC);
    toolBar->addAction(actionStop);


    menuProject = new QMenu("Project(&P)",ui->menuBar);

    QList<QStringList> menus;
    menus.push_back({"Ericsson",
                     "Radio 4415 B3",
                     "Air 3268 B47"});

    for (int i = 0;i < menus.size();++ i) {
        QMenu *menu = new QMenu(menus.at(i).at(0));
        menuProject->addMenu(menu);

        for (int j = 1;j < menus.at(i).size();++ j) {
            QAction *action = new QAction(menus.at(i).at(j),menu);
            action->setCheckable(true);
            menu->addAction(action);
            connect(action,SIGNAL(triggered()),this,SLOT(switchProject()));
        }
    }

    ui->menuBar->addMenu(menuProject);

    Log.en(log_t::RD_LOG_ALL_ON,false);

    childWidgets.push_back(new ns_ericsson::ns_radio_4415::ChildWidgets(this));
    childWidgets.push_back(new ns_ericsson::ns_air_3268::ChildWidgets(this));

    mainSplitter  = new QSplitter(Qt::Horizontal,ui->centralWidget);
    rightSplitter = new QSplitter(Qt::Vertical);

    initMsgLogDlg();
    initStatusBar();
    initMainTreeWidget();
    initMainTabWidget();
    initMainLogTabWidget();

    rightSplitter->setHandleWidth(0);
    rightSplitter->addWidget(mainTab);
    rightSplitter->addWidget(mainLogTab);
    rightSplitter->setStretchFactor(0,10);
    rightSplitter->setStretchFactor(1,3);

    mainSplitter->setHandleWidth(0);
    mainSplitter->addWidget(mainTree);
    mainSplitter->addWidget(rightSplitter);
    mainSplitter->setStretchFactor(0,0);
    mainSplitter->setStretchFactor(1,1);

    QHBoxLayout *mainLayout = new QHBoxLayout(ui->centralWidget);
    mainLayout->setMargin(0);
    mainLayout->addWidget(mainSplitter);
    ui->centralWidget->setLayout(mainLayout);

    Log.set_default();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::switchProject()
{
    QAction *action = (QAction *)(sender());
    QMenu *menu = (QMenu *)(action->parent());

    if (menu->title() == "Ericsson") {
        if (action->text() == "Radio 4415 B3") {
            project = Ericsson_Radio_4415_B3;
        } else if (action->text() == "Air 3268 B47") {
            project = Ericsson_Air_3268_B47;
        }
    }

    ui->menuBar->clear();
    ui->menuBar->addAction(menuProject->menuAction());

    mainTree->clear();
    mainTab->clear();

    mainLogTab->clear();
    mainLogTab->addTab(msgTableView,"Message");

    currentWidgets = childWidgets.at(project);
    currentWidgets->init();
}

void MainWindow::deviceInit()
{
    QDeviceInitThread *thread = new QDeviceInitThread(this);
    Q_Thread_Base::g_threadThread = thread;

    thread->silent = false;
    connect(thread,&QDeviceInitThread::done,currentWidgets,&ChildWidgetHelper::initChildWidgets,Qt::BlockingQueuedConnection);

    thread->start();
}

void MainWindow::deviceInitSilent()
{
    QDeviceInitThread *thread = new QDeviceInitThread(this);
    Q_Thread_Base::g_threadThread = thread;

    thread->silent = true;
    connect(thread,&QDeviceInitThread::done,currentWidgets,&ChildWidgetHelper::initChildWidgets,Qt::BlockingQueuedConnection);

    thread->start();
}

void MainWindow::exit()
{
    QApplication::exit();
}

void MainWindow::initStatusBar()
{
    mainProgressBar = new QProgressBar(ui->statusBar);
    mainProgressBar->setStyleSheet(mainProgressBar->styleSheet() + "QProgressBar{text-align:center;}");

    setProgClr(Qt::green);

    labelProgressName = new QLabel(QString("..."));
    labelVerRF = new QLabel(QString("RF:"));

    ui->statusBar->clearMessage();
    ui->statusBar->addWidget(labelProgressName);
    ui->statusBar->addWidget(mainProgressBar,1);
    ui->statusBar->addPermanentWidget(labelVerRF);
}

void MainWindow::initMainTreeWidget()
{
    QFont font;
    font.setPointSize(10);
    font.setBold(true);
    font.setWeight(75);

    mainTree = new QTreeWidget;
    mainTree->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);
    mainTree->setHeaderHidden(true);
    mainTree->setRootIsDecorated(true);
    mainTree->clear();
    mainTree->setColumnCount(1);
    mainTree->setFont(font);
    mainTree->setSelectionMode(QAbstractItemView::SingleSelection);
//    mainTree->setStyle(QStyleFactory::create("windows"));
    mainTree->setStyleSheet(
                "QTreeWidget {background:rgb(179,217,255)}"
                "QTreeWidget::item:selected {background:rgb(0,255,0);color:black;}");

    connect(mainTree,&QTreeWidget::itemClicked,this,&MainWindow::mainTree_itemClicked);
}

void MainWindow::initMainTabWidget()
{
    QFont font;
    font.setPointSize(10);
    font.setBold(true);
    font.setWeight(75);

    mainTab = new QTabWidget;
    mainTab->setParent(rightSplitter);
    mainTab->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    mainTab->setFont(font);
    mainTab->setStyleSheet(
                "QTabBar::tab {height:20px; width:120px;}"
                "QTabBar::tab:selected {background:rgb(0,255,0)}");

    QHBoxLayout *mainTabLayout = new QHBoxLayout;
    mainTabLayout->addLayout(&childDlgLayout);
    mainTab->setLayout(mainTabLayout);

    connect(mainTab,&QTabWidget::currentChanged,this,&MainWindow::mainTab_currentChanged);
}

void MainWindow::initMainLogTabWidget()
{
    mainLogTab = new QTabWidget;
    mainLogTab->setParent(rightSplitter);
    mainLogTab->setTabPosition(QTabWidget::South);
}

void MainWindow::initMsgLogDlg()
{
    msgLogModel = new QMsgLogModel;

    msgTableView = new Q_Table_View;
    msgTableView->setModel(msgLogModel);
    msgTableView->setColumnWidth(0,50);
    msgTableView->setColumnWidth(1,125);
    msgTableView->setColumnWidth(2,350);
    msgTableView->setColumnWidth(3,50);
    msgTableView->setColumnWidth(4,80);
    msgTableView->verticalHeader()->setDefaultSectionSize(18);
    msgTableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    msgTableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);
    msgTableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Fixed);
    msgTableView->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Stretch);
    msgTableView->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Fixed);
    msgTableView->horizontalHeader()->setSectionResizeMode(4,QHeaderView::Fixed);

    connect(this,SIGNAL(addMsgList(int)),msgLogModel,SLOT(update(int)));
    connect(this,SIGNAL(addMsgList(int)),this,SLOT(updateMsgTable(int)));
}

void MainWindow::initProg(const QString name)
{
    Q_Thread_Base::g_threadName = name;
    labelProgressName->setText(name);
}

void MainWindow::initProg(const QString name, quint32 pts)
{
    initProg(name);
    mainProgressBar->setRange(0,int(pts));
    mainProgressBar->setValue(0);
}

void MainWindow::setProgPos(quint32 pos)
{
    mainProgressBar->setValue(int(pos));
}

void MainWindow::addProgPos(quint32 off)
{
    mainProgressBar->setValue(mainProgressBar->value() + int(off));
}

void MainWindow::setProgClr(const QColor clr)
{
    QString ss = QString("QProgressBar::chunk{background-color:rgb(%1,%2,%3)}")
                        .arg(clr.red())
                        .arg(clr.green())
                        .arg(clr.blue());
    mainProgressBar->setStyleSheet(mainProgressBar->styleSheet() + ss);
}

void MainWindow::updateMsgTable(int row)
{
    msgTableView->scrollToBottom();
    msgTableView->selectRow(row - 1);
}

void MainWindow::threadSPC()
{
    if (thread == nullptr) {
        thread = new Q_Main_Thread(this);
        connect(thread,SIGNAL(done(bool)),this,SLOT(threadDone(bool)));
        thread->start(QThread::LowPriority);
        return;
    }

    QStyle::StandardPixmap sp = QStyle::SP_MediaPlay;

    if (Q_Thread_Base::g_threadThread == nullptr) {
        sp = QStyle::SP_MediaPause;
    }

    if (Q_Thread_Base::g_threadPausing) {
        sp = QStyle::SP_MediaPause;
    }

    actionSPC->setIcon(style()->standardIcon(sp));

    if (currentWidgets == nullptr) {
        return;
    }

    Q_Widget *widget = currentWidgets->currentWidget();

    if (widget == nullptr) {
        return;
    }

    Q_Thread_Widget_Base *threadWidget = dynamic_cast<Q_Thread_Widget_Base *>(widget);

    if (threadWidget == nullptr) {
        return;
    }

    if (Q_Thread_Base::g_threadThread == nullptr) {
        sp = QStyle::SP_MediaPause;
    }

    if (Q_Thread_Base::g_threadPausing) {
        sp = QStyle::SP_MediaPause;
    }

    actionSPC->setIcon(style()->standardIcon(sp));
    threadWidget->threadSPC();
    connect(Q_Thread::g_threadThread,SIGNAL(done(bool)),
            thread,SLOT(childThreadDone(bool)),Qt::BlockingQueuedConnection);
}

void MainWindow::threadStop()
{
    if (Q_Thread_Base::g_threadPausing == true) {
        Q_Thread_Base::g_threadPausing = false;
        Q_Thread_Base::g_threadThread->threadLock.unlock();
    }
    Q_Thread_Base::g_threadStop = true;
}

void MainWindow::threadDone(bool success)
{
    thread = nullptr;
}

void MainWindow::threadProcess(const Q_Thread_Base::Process p)
{
    switch (p) {
    case Q_Thread_Base::Prepare :
        return setProgClr(Qt::darkGreen);

    case Q_Thread_Base::Started :
    case Q_Thread_Base::Running_Normal :
    case Q_Thread_Base::End_Normal :
        return setProgClr(Qt::green);

    case Q_Thread_Base::End_Manual :
        actionSPC->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        return setProgClr(Qt::darkYellow);

    case Q_Thread_Base::Running_Except :
        return setProgClr(Qt::red);

    case Q_Thread_Base::End_Except :
        actionSPC->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        return setProgClr(Qt::red);

    case Q_Thread_Base::Paused :
        return setProgClr(Qt::yellow);

    default : return;
    }
}

void MainWindow::mainTree_itemClicked(QTreeWidgetItem *item, int column)
{
    disconnect(mainTab,&QTabWidget::currentChanged,this,&MainWindow::mainTab_currentChanged);
    currentWidgets->mainTreeItemClicked(item,column);
    connect(mainTab,&QTabWidget::currentChanged,this,&MainWindow::mainTab_currentChanged);
    mainTab->setCurrentIndex(int(RFIdx));
}


bool MainWindow::mainTreeSelectFirst(QTreeWidgetItem *item)
{
    if (item == nullptr) {
        return false;
    }

    if (item->childCount() == 0 && item->checkState(0) == Qt::Checked) {
        mainTree->setCurrentItem(item);

        Q_Widget *widget = currentWidgets->currentWidget();

        if (widget == nullptr) {
            return false;
        }

        Q_Thread_Widget_Base *threadWidget = dynamic_cast<Q_Thread_Widget_Base *>(widget);

        if (threadWidget == nullptr) {
            return false;
        }

        return true;
    }

    return mainTreeSelectFirst(item->child(0));
}

void MainWindow::mainTree_selectFirst()
{
    if (mainTree->topLevelItemCount() == 0) {
        return;
    }

    bool selected = false;

    for (int i = 0;i < mainTree->topLevelItemCount();++i) {
        if ((selected = mainTreeSelectFirst(mainTree->topLevelItem(i))) == true) {
            break;
        }
    }

    if (selected) {
        mainTree_itemClicked(mainTree->currentItem(),0);
    }
}

void MainWindow::runThread(const int idx)
{
    QList<bool> checkList = currentWidgets->checkList();
    QList<bool>::const_iterator iterCheckList = checkList.constBegin();

    QList<QTreeWidgetItem *> treeWidgetItemList = currentWidgets->treeWidgetItemList();
    QList<QTreeWidgetItem *>::const_iterator iterItem = treeWidgetItemList.constBegin();

    int i = 0;

    for (;iterCheckList != checkList.constEnd();++iterCheckList,++iterItem) {
        if (*iterCheckList == true) {
            if (i == idx) {
                mainTree->setCurrentItem(*iterItem);
                break;
            }
            i ++;
        }
    }

    mainTree_itemClicked(*iterItem,0);

    threadSPC();
}

void MainWindow::mainTab_currentChanged(int index)
{
    if (index == -1) {
        return;
    }

    RFIdx = quint32(index);

    currentWidgets->mainTabCurrentChanged(index);
}
