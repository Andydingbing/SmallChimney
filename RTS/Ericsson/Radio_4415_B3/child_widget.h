#ifndef CHILD_WIDGET_H
#define CHILD_WIDGET_H

#include "plugin.h"
#include "table_view.hpp"

class API ChildWidgets : public PlugIn
{
public:
    ChildWidgets();
    ~ChildWidgets() {}

    void treeItemClicked(QTreeWidgetItem *item, int column) {}
    void tabCurrentChanged(int index) {}
    void init();
    void initMenu(QList<QMenu *> &menus);
    void initMainLogTabWidget();
    QString tabName(int idx) { return QString("Branch-%1").arg(char('A' + idx)); }

    static void addComLoggerCallback();

private:
    QMenu *menuDevice;
    QAction *actionInit;
    QAction *actionInitSilent;
    QAction *actionExit;

    QMenu *menuFunction;
    QAction *actionLogConfig;
    QAction *actionLogViewer;

    QMenu *menuHelp;
    QAction *actionAbout;

    Q_Table_View *comLogTableView;

public:
    static ChildWidgets *create();
};

#endif
