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


RTS_HELPER_EXPORT void setTree(QTreeWidget *tree, const QList<TreeChildItem *> *childItems);
RTS_HELPER_EXPORT Q_Widget *currentWidget(QTreeWidget *tree, QList<TreeChildItem *> *items, quint32 index);
RTS_HELPER_EXPORT QList<bool> checkList(QTreeWidget *tree);
RTS_HELPER_EXPORT QList<QTreeWidgetItem *> itemList(QTreeWidget *tree);

class RTS_HELPER_EXPORT ChildWidgetHelper : public QObject
{
    Q_OBJECT

public:
    ChildWidgetHelper();
    virtual ~ChildWidgetHelper() {}

//    void reSortTreeChildItem(const sequence &s);
//    QList<QTreeWidgetItem *> treeWidgetItemList();

public slots:
    virtual void initChildWidgets();

private:
    void printItems();
    void updateCheckState();

public:
    QList<TreeChildItem *> *_treeChildItemsBuiltIn;
    QList<TreeChildItem *> *_treeChildItems;
};

#endif
