#include "QTreeWidget_helper.h"
#include <string>
//#include "../utilities/string_util.hpp"

using namespace std;

void itemList(QTreeWidgetItem *item, QList<QTreeWidgetItem *> &list)
{
    if (item->childCount() == 0) {
        list.push_back(item);
        return;
    }

    QTreeWidgetItem *childItem = nullptr;

    for (int i = 0;i < item->childCount();++i) {
        childItem = item->child(i);
        itemList(childItem,list);
    }
}

QList<QTreeWidgetItem *> itemList(QTreeWidget *tree)
{
    QList<QTreeWidgetItem *> list;

    for (int i = 0;i < tree->topLevelItemCount();++i) {
        itemList(tree->topLevelItem(i),list);
    }

    return list;
}

QTreeWidgetItem *rootItem(const QString item,const QList<QTreeWidgetItem *> &rootItems)
{
    QList<QTreeWidgetItem *>::const_iterator iter;

    for (iter = rootItems.constBegin();iter != rootItems.constEnd();++iter) {
        if (item == (*iter)->text(0)) {
            return *iter;
        }
    }
    return nullptr;
}

QTreeWidgetItem *addTreeItem(const QString item,QTreeWidgetItem *parent)
{
    QList<QTreeWidgetItem *>::const_iterator iter;

    for (int i = 0;i < parent->childCount();++i) {
        if (item == parent->child(i)->text(0)) {
            return parent->child(i);
        }
    }
    QTreeWidgetItem *treeItem = new QTreeWidgetItem(parent,QStringList(item));
    treeItem->setFlags(treeItem->flags() | Qt::ItemIsAutoTristate);
    treeItem->setCheckState(0,Qt::Unchecked);
    return parent->child(parent->childCount() - 1);
}

quint32 parents(const QTreeWidgetItem *item)
{
    QTreeWidgetItem *parent = item->parent();
    quint32 i = 0;

    while (parent != nullptr) {
        parent = parent->parent();
        ++ i;
    }
    return i;
}

bool compare(const list<string> &stringList,const QTreeWidgetItem *item)
{
    list<string>::const_reverse_iterator iter = stringList.crbegin();
    const QTreeWidgetItem *parent = item;
    quint32 i = parents(item);

    if (i != quint32(stringList.size() - 1)) {
        return false;
    }

    for (quint32 j = 0;j <= i;++ j,++ iter) {
        if (parent->text(0).toStdString() != *iter) {
            return false;
        }
        parent = parent->parent();
    }
    return true;
}

void setTree(QTreeWidget *tree, const QList<TreeChildItem *> *childItems)
{
    QList<TreeChildItem *>::const_iterator iterCI;
    QList<QTreeWidgetItem *>::iterator iterItem;
    QList<QTreeWidgetItem *> rootItems;
    list<string>::const_iterator iterStr;

    for (iterCI = childItems->constBegin();iterCI != childItems->constEnd();++iterCI) {
        for (iterItem = rootItems.begin();iterItem != rootItems.end();++iterItem) {
            if (*(*iterCI)->stringList.begin() == (*iterItem)->text(0).toStdString()) {
                break;
            }
        }
        if (iterItem == rootItems.end()) {
            QString rootItemText = QString::fromStdString(*((*iterCI)->stringList.begin()));
            QTreeWidgetItem *rootItem = new QTreeWidgetItem(tree,QStringList(rootItemText));
            rootItem->setFlags(rootItem->flags() | Qt::ItemIsAutoTristate);
            rootItem->setCheckState(0,Qt::Unchecked);
            rootItems.append(rootItem);
        }
    }

    for (iterCI = childItems->constBegin();iterCI != childItems->constEnd();++iterCI) {
        QString rootItemText = QString::fromStdString(*((*iterCI)->stringList.begin()));
        QTreeWidgetItem *root = rootItem(rootItemText,rootItems);

        iterStr = (*iterCI)->stringList.cbegin();
        iterStr ++;
        for (;iterStr != (*iterCI)->stringList.cend();++iterStr) {
            root = addTreeItem(QString::fromStdString(*iterStr),root);
        }
    }

    tree->expandAll();
}

Q_Widget *currentWidget(QTreeWidget *tree, QList<TreeChildItem *> *items, quint32 index)
{
    if (items->empty()) {
        return nullptr;
    }

    QList<TreeChildItem *>::const_iterator iterItems;

    for (iterItems = items->constBegin();iterItems != items->constEnd();++iterItems) {
        if (compare((*iterItems)->stringList,tree->currentItem())) {
            return (*iterItems)->tabWidgets->at(index);
        }
    }
    return nullptr;
}

void checkList(const QTreeWidgetItem *item, QList<bool> &list)
{
    if (item->childCount() == 0) {
        list.push_back(item->checkState(0) == Qt::Checked);
        return;
    }

    QTreeWidgetItem *childItem = nullptr;

    for (int i = 0;i < item->childCount();++i) {
        childItem = item->child(i);
        checkList(childItem,list);
    }
}

QList<bool> checkList(QTreeWidget *tree)
{
    QList<bool> list;

    for (int i = 0;i < tree->topLevelItemCount();++i) {
        checkList(tree->topLevelItem(i),list);
    }
    return list;
}


ChildWidgetHelper::ChildWidgetHelper() : QObject()
{

}

void setCheckStateChild(QTreeWidgetItem *item, const Qt::CheckState state)
{
    if (item->childCount() == 0) {
        item->setCheckState(0,state);
        return;
    }

    QTreeWidgetItem *childItem = nullptr;

    for (int i = 0;i < item->childCount();++i) {
        childItem = item->child(i);
        setCheckStateChild(childItem,state);
    }
}

void setCheckStateParent(QTreeWidgetItem *item)
{
    int checkCount = 0;
    int unCheckCount = 0;
    Qt::CheckState state = Qt::Unchecked;
    QTreeWidgetItem *parentItem = item->parent();

    if (parentItem == nullptr) {
        return;
    }

    for (int i = 0;i < parentItem->childCount();++i) {
        if (parentItem->child(i)->checkState(0) == Qt::Checked) {
            checkCount ++;
        } else {
            unCheckCount ++;
        }
    }

    if (checkCount == 0) {
        state = Qt::Unchecked;
    } else if (unCheckCount == 0) {
        state = Qt::Checked;
    } else {
        state = Qt::PartiallyChecked;
    }

    parentItem->setCheckState(0,state);
    setCheckStateParent(parentItem);
}

//void ChildWidgetHelper::reSortTreeChildItem(const sequence &s)
//{
//    list<list<string>>::const_iterator iterLL = s.tree.cbegin();
//    TreeChildItem *item;

//    for (;iterLL != s.tree.cend();++iterLL) {
//        if ((item = containTreeChildItem((*iterLL))) != nullptr) {
//            treeChildItems.push_back(item);
//        }
//    }
//}


void ChildWidgetHelper::initChildWidgets()
{
    QList<TreeChildItem *>::iterator iterTreeItem;
    QList<Q_Widget *> *tabWidgets;
    QList<Q_Widget *>::iterator iterWidget;

    for (iterTreeItem = _treeChildItems->begin();iterTreeItem != _treeChildItems->end();++iterTreeItem) {
        tabWidgets = (*iterTreeItem)->tabWidgets;
        for (iterWidget = tabWidgets->begin();iterWidget != tabWidgets->end();++iterWidget) {
            (*iterWidget)->init();
        }
    }
}

void ChildWidgetHelper::printItems()
{
//    QList<TreeChildItem *>::const_iterator iterTCI = treeChildItems.constBegin();

//    for (;iterTCI != treeChildItems.constEnd();++iterTCI) {
//        LoggerMsg.stdprintf("%s\n",(*iterTCI)->checkState == Qt::Checked ? "checked" : "unchecked");
//    }
//    LoggerMsg.stdprintf("\n");
}
