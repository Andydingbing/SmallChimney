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
    void tabCurrentChanged(int index);
    void init();
    void initMenu(QList<QMenu *> &menus);
    void initMainLogTabWidget();

    static void addComLoggerCallback();

private:
    QMenu *menuDevice;
    QAction *actionInit;
    QAction *actionPCIE_Restart;
    QAction *actionTuneOCXO;
    QAction *actionExit;

    QMenu *menuRFU;
    QAction *actionRFU0;
    QAction *actionRFU1;
    QAction *actionRFU2;
    QAction *actionRFU3;
    QAction *actionRFU4;

    QMenu *menuFunction;
    QAction *actionArbViewer;
    QAction *actionStarAllIQ_Capture;
    QAction *actionStopAllIQ_Capture;

    QMenu *menuTest;
    QAction *actionProgramFPGAbit;
    QAction *actionAlgorithm_5355;
    QAction *actionFittingInterpolation;

    QMenu *menuHelp;
    QAction *actionAbout;

public:
    static ChildWidgets *create();
};

#endif
