#include "child_widget_helper.h"
#include "mainwindow.h"
#include <QString>
#include <QMenu>
#include <QAction>

ChildWidgetHelper::TreeChildItem::TreeChildItem(QStringList &_stringList, Qt::CheckState _checkState, void *_tabWidgets)
{
    stringList = _stringList;
    checkState = _checkState;

    if (_tabWidgets != nullptr) {
        tabWidgets = static_cast<QList<Q_Widget *> *>(_tabWidgets);
    }
}

ChildWidgetHelper::ChildWidgetHelper(MainWindow *_parent) : QObject()
{
    parent = _parent;
    ui = _parent->ui;
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

bool compare(const QStringList &stringList,const QTreeWidgetItem *item)
{
    const QTreeWidgetItem *parent = item;
    quint32 i = parents(item);

    if (i != quint32(stringList.size() - 1)) {
        return false;
    }

    for (quint32 j = 0;j <= i;j ++) {
        if (parent->text(0) != stringList.at(i - j)) {
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

Q_Widget *ChildWidgetHelper::currentWidget()
{
    if (_items.empty()) {
        return nullptr;
    }

    QList<TreeChildItem *>::const_iterator iterItems;

    for (iterItems = _items.constBegin();iterItems != _items.constEnd();++iterItems) {
        if (compare((*iterItems)->stringList,g_MainW->mainTree->currentItem())) {
            return (*iterItems)->tabWidgets->at(RFIdx);
        }
    }
    return nullptr;
}

Qt::CheckState ChildWidgetHelper::currentTreeItemCheckState()
{
    if (_items.empty()) {
        return Qt::Unchecked;
    }

    QList<TreeChildItem *>::const_iterator iterItems;

    for (iterItems = _items.constBegin();iterItems != _items.constEnd();++iterItems) {
        if (compare((*iterItems)->stringList,g_MainW->mainTree->currentItem())) {
            return (*iterItems)->checkState;
        }
    }
    return Qt::Unchecked;
}

void ChildWidgetHelper::setMainTree(const QList<TreeChildItem *> childItems)
{
    QList<TreeChildItem *>::const_iterator iterCI;
    QList<QTreeWidgetItem *>::iterator iterItem;
    QList<QTreeWidgetItem *> rootItems;
    QStringList::const_iterator iterStr;

    for (iterCI = childItems.constBegin();iterCI != childItems.constEnd();++iterCI) {
        for (iterItem = rootItems.begin();iterItem != rootItems.end();++iterItem) {
            if (*(*iterCI)->stringList.begin() == (*iterItem)->text(0)) {
                break;
            }
        }
        if (iterItem == rootItems.end()) {
            QTreeWidgetItem *rootItem = new QTreeWidgetItem(parent->mainTree,QStringList(*((*iterCI)->stringList.begin())));
            rootItem->setFlags(rootItem->flags() | Qt::ItemIsAutoTristate);
            rootItem->setCheckState(0,Qt::Unchecked);
            rootItems.append(rootItem);
        }
    }

    for (iterCI = childItems.constBegin();iterCI != childItems.constEnd();++iterCI) {
        QTreeWidgetItem *root = rootItem(*(*iterCI)->stringList.begin(),rootItems);
        for (iterStr = (*iterCI)->stringList.constBegin() + 1;iterStr != (*iterCI)->stringList.constEnd();++iterStr) {
            root = addTreeItem(*iterStr,root);
        }
    }

    parent->mainTree->expandAll();
}

void ChildWidgetHelper::mainTreeItemClicked(QTreeWidgetItem *item, int column)
{
    QList<TreeChildItem *>::const_iterator iterItems;

//    setCheckStateChild(item,item->checkState(0));
//    setCheckStateParent(item);

    for (iterItems = _items.constBegin();iterItems != _items.constEnd();++iterItems) {
        if (compare((*iterItems)->stringList,item)) {
            if (item->checkState(0) != (*iterItems)->checkState) {
                break;
            }

            parent->mainTab->clear();
            for (int i = 0;i < (*iterItems)->tabWidgets->size();++i) {
                parent->mainTab->addTab((*iterItems)->tabWidgets->at(i),tabName(i));
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

    for (iterTreeItem = _items.begin();iterTreeItem != _items.end();++iterTreeItem) {
        tabWidgets = (*iterTreeItem)->tabWidgets;
        for (iterWidget = tabWidgets->begin();iterWidget != tabWidgets->end();++iterWidget) {
            (*iterWidget)->init();
        }
    }
}

void ChildWidgetHelper::printItems()
{
    QList<TreeChildItem *>::const_iterator iterTCI = _items.constBegin();

    for (;iterTCI != _items.constEnd();++iterTCI) {
        LoggerMsg.stdprintf("%s\n",(*iterTCI)->checkState == Qt::Checked ? "checked" : "unchecked");
    }
    LoggerMsg.stdprintf("\n");
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
    QList<TreeChildItem *>::iterator iterTCI = _items.begin();

    for (int i = 0;i < parent->mainTree->topLevelItemCount();++i) {
        updateCheckState(parent->mainTree->topLevelItem(i),&iterTCI);
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

    for (int i = 0;i < _items.size();++ i) {
        list.push_back(false);
    }

    QList<bool>::iterator iter = list.begin();

    for (int i = 0;i < parent->mainTree->topLevelItemCount();++i) {
        checkList(parent->mainTree->topLevelItem(i),&iter);
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

    for (int i = 0;i < _items.size();++ i) {
        list.push_back(nullptr);
    }

    QList<QTreeWidgetItem *>::iterator iter = list.begin();

    for (int i = 0;i < parent->mainTree->topLevelItemCount();++i) {
        treeWidgetItemList(parent->mainTree->topLevelItem(i),&iter);
    }

    return list;
}
