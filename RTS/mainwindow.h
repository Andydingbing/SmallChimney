#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QTreeWidget>
#include <QTabWidget>
#include <QHBoxLayout>
#include "main_thread.h"
#include "ui_mainwindow.h"
#include "table_view.hpp"

#include "thread_widget.h"
#include "device_init_thread.h"

class ChildWidgetHelper;

class QLabel;
class QTreeWidgetItem;
class QProgressBar;
class QMenu;
class QAction;

class QMsgLogModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    friend class ChildWidgetHelper;

public:
    QLabel *labelVerRF;

public:
    QList<ChildWidgetHelper *> childWidgets;
    ChildWidgetHelper *currentWidgets;

public:
    QMsgLogModel *msgLogModel;

    QProgressBar *mainProgressBar;
    QLabel *labelProgressName;

    static QHBoxLayout childDlgLayout;
    QHBoxLayout *mainTabLayout;

public:
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
    void addMsgListCallback() { emit addMsgList(int(Log.msgs())); }

private:
    void initStatusBar();
    void initMainTreeWidget();
    void initMainTabWidget();
    void initMainLogTabWidget();
    void initMsgLogDlg();

    bool mainTreeSelectFirst(QTreeWidgetItem *item);

signals:
    void addMsgList(int row);

private slots:
    void mainTree_itemClicked(QTreeWidgetItem *item, int column);
    void mainTree_selectFirst();
    void runThread(const int idx);

    void mainTab_currentChanged(int index);

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
};

extern MainWindow *g_MainW;
extern Project project;

#endif // MAIN_WINDOW_H
