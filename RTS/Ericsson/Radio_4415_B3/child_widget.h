#ifndef ERICSSON_RADIO_4415_B3_CHILD_WIDGET_H
#define ERICSSON_RADIO_4415_B3_CHILD_WIDGET_H

#include "QTreeWidget_helper.h"
#include "table_view.hpp"

namespace ns_ericsson {
namespace ns_radio_4415 {

class API ChildWidgets : public ChildWidgetHelper
{
    Q_OBJECT

public:
    ChildWidgets(QTreeWidget *treeWidget, QTabWidget *tabWidget);

    void init();
    void initMenu(QList<QMenu *> &menus);
    void initMainLogTabWidget();
    void mainTabCurrentChanged(int index);
    QString tabName(int idx) { return QString("Branch-%1").arg(char('A' + idx)); }

    static void addComLoggerCallback();

signals:
    void addComLogger(int row);

public slots:
    void addComLoggerTableView(int row);

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
};

} // namespace ns_radio_4415
} // namespace ns_ericsson

#endif
