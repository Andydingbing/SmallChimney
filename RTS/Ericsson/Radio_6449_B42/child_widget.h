#ifndef CHILD_WIDGET_H
#define CHILD_WIDGET_H

#include "plugin.h"
#include "table_view.hpp"

class API ChildWidgets : public Plugin
{
public:
    ChildWidgets();
    ~ChildWidgets() {}

    std::string version();
    std::string projectMenu();
    std::string tabName(int idx);
    void treeItemClicked(QTreeWidgetItem *item, int column) {}
    void tabCurrentChanged(int index) {}
    void init();
    void initMenu(QList<QMenu *> &menus);
    void initMainLogTabWidget();

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
