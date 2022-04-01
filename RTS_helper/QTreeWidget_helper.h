#ifndef QTreeWidget_HELPER_H
#define QTreeWidget_HELPER_H

#include "global.h"
#include "../RTS/plugin.h"
#include "thread_widget.h"
#include <QList>
#include <QWidget>
#include <QMenu>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <boost/dll/alias.hpp>




QList<QTreeWidgetItem *> itemList(QTreeWidget &widget);


//struct RTS_HELPER_EXPORT TreeChildItem {
//    std::list<std::string> stringList;
//    Qt::CheckState checkState;
//    QList<Q_Widget *> *tabWidgets;

//    TreeChildItem(std::string &str,Qt::CheckState _checkState,void *_tabWidgets = nullptr);
//};

RTS_HELPER_EXPORT void setTree(QTreeWidget &tree, const QList<TreeChildItem *> &childItems);
RTS_HELPER_EXPORT Q_Widget *currentWidget(QTreeWidget *tree, QList<TreeChildItem *> *items, quint32 index);

class RTS_HELPER_EXPORT ChildWidgetHelper : public QObject
{
    Q_OBJECT

public:
    ChildWidgetHelper();
    ChildWidgetHelper(QTreeWidget *treeWidget, QTabWidget *tabWidget);
    virtual ~ChildWidgetHelper() {}
    virtual QString tabName(int idx) { return QString("RF-%1").arg(idx); }
    void setTreeWidget(QTreeWidget *treeWidget);
    void setTabWidget(QTabWidget *tabWidget);
//    Qt::CheckState currentTreeItemCheckState();

//    TreeChildItem *containTreeChildItem(const std::list<std::string> &l) const;
//    void reSortTreeChildItem(const sequence &s);
    void treeItemClicked(QTreeWidgetItem *item, int column);
    QList<bool> checkList();
    QList<QTreeWidgetItem *> treeWidgetItemList();

public slots:
    virtual void initChildWidgets();

private:
    void printItems();
    void updateCheckState(const QTreeWidgetItem *item, QList<TreeChildItem *>::iterator *iterTCI);
    void updateCheckState();
    void checkList(const QTreeWidgetItem *item, QList<bool>::iterator *iter);
    void treeWidgetItemList(QTreeWidgetItem *item, QList<QTreeWidgetItem *>::iterator *iter);

public:
    QTreeWidget *tree;
    QTabWidget *tab;
    QList<TreeChildItem *> *_treeChildItemsBuiltIn;
    QList<TreeChildItem *> *_treeChildItems;
    int _selectedItems;
};

#endif
