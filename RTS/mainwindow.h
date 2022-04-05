#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "../RTS_helper/global.h"
#include "../RTS_helper/QTreeWidget_helper.h"
#include "global.h"
#include <QMainWindow>
#include <QSplitter>
#include <QTreeWidget>
#include <QTabWidget>
#include <QHBoxLayout>
#include "main_thread.h"
#include "table_view.hpp"

#include "thread_widget.h"
#include "device_init_thread.h"
#include "../sequence/sequence.h"

namespace Ui {
    class MainWindow;
}

class QLabel;
class QTreeWidgetItem;
class QProgressBar;
class QMenu;
class QAction;

class QMsgLogModel;

struct MapProjectMenu {
    Project project;
    QStringList menu;
    MapProjectMenu(const Project p, const QString &m);
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

    friend class ChildWidgetHelper;

public:
    QLabel *labelVerRF;

public:
    QMsgLogModel *msgLogModel;

    QProgressBar *mainProgressBar;
    QLabel *labelProgressName;

    static QHBoxLayout childDlgLayout;
    QHBoxLayout *mainTabLayout;

public:
    typedef PlugIn* (pluginapi_create_t)();

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void switchProject();
    void deviceInit();
    void deviceInitSilent();
    void exit();

    void initProg(const QString name);
    void initProg(const QString name,quint32 pts);
    void setProgPos(quint32 pos);
    void addProgPos(quint32 off);
    void setProgClr(const QColor clr);

    void updateMsgTable(int row);

    void threadSPC();
    void threadStop();
    void threadDone(bool success = true);
    void threadCheckBox(const QString msg) { ::threadCheckBox(msg.toStdString().c_str()); }
    void threadErrorBox(const QString msg) { ::threadErrorBox(msg.toStdString().c_str()); }
    void threadProcess(const Q_Thread_Base::Process p);

    bool ftpRetryBox() { return ::ftpRetryBox(); }

public:
    bool verifySequence(const QString &path);
    void addMapProjectMenu(const Project project,const QString &menu);
    void addMsgListCallback() { emit addMsgList(int(Log.msgs())); }

private:
    void initMenu();
    void initToolBar();
    void initStatusBar();
    void initMainTreeWidget();
    void initMainTabWidget();
    void initMainLogTabWidget();
    void initMsgLogDlg();

    bool mainTreeSelectFirst(QTreeWidgetItem *item);
    void updateCheckState(const QTreeWidgetItem *item, QList<TreeChildItem *>::iterator *iterTCI);
    void updateCheckState();

signals:
    void addMsgList(int row);

private slots:
    void mainTree_itemClicked(QTreeWidgetItem *item, int column);
    void mainTree_selectFirst();
    void runThread(const int idx);

    void mainTab_currentChanged(int index);

public:
    QList<MapProjectMenu> mapProjectMenu;

public:
    Ui::MainWindow *ui;

    QSplitter *mainSplitter;
    QSplitter *rightSplitter;

    QTreeWidget *mainTree;
    QTabWidget  *mainTab;
    QTabWidget  *mainLogTab;

    Q_Table_View *msgTableView;

    QMenu *menuProject;

    QAction *actionSPC;
    QAction *actionStop;

    Q_Main_Thread *thread;

public:
    boost::function<pluginapi_create_t> plugInCreator;
    QList<PlugIn *> plugIns;
    PlugIn *currentPlugIn;
};

extern MainWindow *g_MainW;
extern Project project;
extern sequence mainSequence;

#endif
