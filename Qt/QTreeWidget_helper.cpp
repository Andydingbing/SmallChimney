#include "QTreeWidget_helper.h"
#include <string>
#include "../utilities/string_util.hpp"

using namespace std;

TreeChildItem::TreeChildItem(string &str,Qt::CheckState _checkState,void *_tabWidgets)
{
    list<string> seperator = {","};

    split(str.c_str(),seperator,stringList);

    checkState = _checkState;

    if (_tabWidgets != nullptr) {
        tabWidgets = static_cast<QList<Q_Widget *> *>(_tabWidgets);
    }
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

void setTree(QTreeWidget &tree, const QList<TreeChildItem *> &childItems)
{
    QList<TreeChildItem *>::const_iterator iterCI;
    QList<QTreeWidgetItem *>::iterator iterItem;
    QList<QTreeWidgetItem *> rootItems;
    list<string>::const_iterator iterStr;

    for (iterCI = childItems.constBegin();iterCI != childItems.constEnd();++iterCI) {
        for (iterItem = rootItems.begin();iterItem != rootItems.end();++iterItem) {
            if (*(*iterCI)->stringList.begin() == (*iterItem)->text(0).toStdString()) {
                break;
            }
        }
        if (iterItem == rootItems.end()) {
            QString rootItemText = QString::fromStdString(*((*iterCI)->stringList.begin()));
            QTreeWidgetItem *rootItem = new QTreeWidgetItem(&tree,QStringList(rootItemText));
            rootItem->setFlags(rootItem->flags() | Qt::ItemIsAutoTristate);
            rootItem->setCheckState(0,Qt::Unchecked);
            rootItems.append(rootItem);
        }
    }

    for (iterCI = childItems.constBegin();iterCI != childItems.constEnd();++iterCI) {
        QString rootItemText = QString::fromStdString(*((*iterCI)->stringList.begin()));
        QTreeWidgetItem *root = rootItem(rootItemText,rootItems);

        iterStr = (*iterCI)->stringList.cbegin();
        iterStr ++;
        for (;iterStr != (*iterCI)->stringList.cend();++iterStr) {
            root = addTreeItem(QString::fromStdString(*iterStr),root);
        }
    }

    tree.expandAll();
}


ChildWidgetHelper::ChildWidgetHelper() : QObject()
{
    setTreeWidget(nullptr);
    setTabWidget(nullptr);
}

ChildWidgetHelper::ChildWidgetHelper(QTreeWidget *treeWidget, QTabWidget *tabWidget) : QObject()
{
    setTreeWidget(treeWidget);
    setTabWidget(tabWidget);
}

void ChildWidgetHelper::setTreeWidget(QTreeWidget *treeWidget)
{
    tree = treeWidget;
}

void ChildWidgetHelper::setTabWidget(QTabWidget *tabWidget)
{
    tab = tabWidget;
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

Q_Widget *ChildWidgetHelper::currentWidget(quint32 tabIndex)
{
    if (treeChildItems.empty()) {
        return nullptr;
    }

    QList<TreeChildItem *>::const_iterator iterItems;

    for (iterItems = treeChildItems.constBegin();iterItems != treeChildItems.constEnd();++iterItems) {
        if (compare((*iterItems)->stringList,tree->currentItem())) {
            return (*iterItems)->tabWidgets->at(tabIndex);
        }
    }
    return nullptr;
}

Qt::CheckState ChildWidgetHelper::currentTreeItemCheckState()
{
    if (treeChildItems.empty()) {
        return Qt::Unchecked;
    }

    QList<TreeChildItem *>::const_iterator iterItems;

    for (iterItems = treeChildItems.constBegin();iterItems != treeChildItems.constEnd();++iterItems) {
        if (compare((*iterItems)->stringList,tree->currentItem())) {
            return (*iterItems)->checkState;
        }
    }
    return Qt::Unchecked;
}

TreeChildItem * ChildWidgetHelper::containTreeChildItem(const list<string> &l) const
{
    QList<TreeChildItem *>::const_iterator iterItems;

    for (iterItems = treeChildItemsBuiltIn.cbegin();iterItems != treeChildItemsBuiltIn.cend();++iterItems) {
        if ((*iterItems)->stringList == l) {
            return *iterItems;
        }
    }
    return nullptr;
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

void ChildWidgetHelper::treeItemClicked(QTreeWidgetItem *item, int column)
{
    QList<TreeChildItem *>::const_iterator iterItems;

//    setCheckStateChild(item,item->checkState(0));
//    setCheckStateParent(item);

    for (iterItems = treeChildItems.constBegin();iterItems != treeChildItems.constEnd();++iterItems) {
        if (compare((*iterItems)->stringList,item)) {
            if (item->checkState(0) != (*iterItems)->checkState) {
                break;
            }

            tab->clear();
            for (int i = 0;i < (*iterItems)->tabWidgets->size();++i) {
                tab->addTab((*iterItems)->tabWidgets->at(i),tabName(i));
            }
            break;
        }
    }
    updateCheckState();
}

void ChildWidgetHelper::initChildWidgets()
{
    QList<TreeChildItem *>::iterator iterTreeItem;
    QList<Q_Widget *> *tabWidgets;
    QList<Q_Widget *>::iterator iterWidget;

    for (iterTreeItem = treeChildItems.begin();iterTreeItem != treeChildItems.end();++iterTreeItem) {
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

void ChildWidgetHelper::updateCheckState(const QTreeWidgetItem *item, QList<TreeChildItem *>::iterator *iterTCI)
{
    if (item->childCount() == 0) {
        (*(*iterTCI))->checkState = item->checkState(0);
        (*iterTCI) ++;
        return;
    }

    QTreeWidgetItem *childItem = nullptr;

    for (int i = 0;i < item->childCount();++i) {
        childItem = item->child(i);
        updateCheckState(childItem,iterTCI);
    }
}

void ChildWidgetHelper::updateCheckState()
{
    QList<TreeChildItem *>::iterator iterTCI = treeChildItems.begin();

    for (int i = 0;i < tree->topLevelItemCount();++i) {
        updateCheckState(tree->topLevelItem(i),&iterTCI);
    }
    printItems();
}


void ChildWidgetHelper::checkList(const QTreeWidgetItem *item, QList<bool>::iterator *iter)
{
    if (item->childCount() == 0) {
        (*(*iter)) = item->checkState(0) == Qt::Checked;
        (*iter) ++;
        return;
    }

    QTreeWidgetItem *childItem = nullptr;

    for (int i = 0;i < item->childCount();++i) {
        childItem = item->child(i);
        checkList(childItem,iter);
    }
}

QList<bool> ChildWidgetHelper::checkList()
{
    QList<bool> list;

    for (int i = 0;i < treeChildItems.size();++ i) {
        list.push_back(false);
    }

    QList<bool>::iterator iter = list.begin();

    for (int i = 0;i < tree->topLevelItemCount();++i) {
        checkList(tree->topLevelItem(i),&iter);
    }
    printItems();
    return list;
}

void ChildWidgetHelper::treeWidgetItemList(QTreeWidgetItem *item, QList<QTreeWidgetItem *>::iterator *iter)
{
    if (item->childCount() == 0) {
        (*(*iter)) = item;
        (*iter) ++;
        return;
    }

    QTreeWidgetItem *childItem = nullptr;

    for (int i = 0;i < item->childCount();++i) {
        childItem = item->child(i);
        treeWidgetItemList(childItem,iter);
    }
}

QList<QTreeWidgetItem *> ChildWidgetHelper::treeWidgetItemList()
{
    QList<QTreeWidgetItem *> list;

    for (int i = 0;i < treeChildItems.size();++ i) {
        list.push_back(nullptr);
    }

    QList<QTreeWidgetItem *>::iterator iter = list.begin();

    for (int i = 0;i < tree->topLevelItemCount();++i) {
        treeWidgetItemList(tree->topLevelItem(i),&iter);
    }

    return list;
}
