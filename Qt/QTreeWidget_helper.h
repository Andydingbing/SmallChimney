#ifndef QTreeWidget_HELPER_H
#define QTreeWidget_HELPER_H

#include "thread_widget.h"
#include <QList>
#include <QWidget>
#include <QMenu>
#include <QTreeWidget>
#include <QTreeWidgetItem>

class QTreeWidget_helper
{
public:
    static void itemList(QTreeWidgetItem *item, QList<QTreeWidgetItem *> &list)
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

    static QList<QTreeWidgetItem *> itemList(QTreeWidget &widget)
    {
        QList<QTreeWidgetItem *> list;

        for (int i = 0;i < widget.topLevelItemCount();++i) {
            itemList(widget.topLevelItem(i),list);
        }

        return list;
    }

};


struct QHELPER_EXPORT TreeChildItem {
    std::list<std::string> stringList;
    Qt::CheckState checkState;
    QList<Q_Widget *> *tabWidgets;

    TreeChildItem(std::string &str,Qt::CheckState _checkState,void *_tabWidgets = nullptr);
};

QHELPER_EXPORT void setTree(QTreeWidget &tree, const QList<TreeChildItem *> &childItems);

#define DECL_TREE(list,kase_class,n) \
{ \
    QList<kase_class *> * kase_class##s = new QList<kase_class *>; \
    std::string str = list; \
    treeChildItemsBuiltIn.append(new TreeChildItem(str,Qt::Checked,kase_class##s)); \
    for (size_t i = 0;i < n;i ++) { \
        kase_class##s->push_back(new kase_class(tab)); \
        kase_class##s->at(i)->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding); \
        kase_class##s->at(i)->setVisible(false); \
        /*MainWindow::childDlgLayout.addWidget(kase_class##s->at(i));*/ \
    } \
}

class QHELPER_EXPORT ChildWidgetHelper : public QObject
{
    Q_OBJECT

public:
    ChildWidgetHelper(QTreeWidget *treeWidget, QTabWidget *tabWidget);
    Q_Widget *currentWidget(quint32 tabIndex);
    Qt::CheckState currentTreeItemCheckState();

    TreeChildItem *containTreeChildItem(const std::list<std::string> &l) const;
//    void reSortTreeChildItem(const sequence &s);
    void treeItemClicked(QTreeWidgetItem *item, int column);
    QList<bool> checkList();
    QList<QTreeWidgetItem *> treeWidgetItemList();

    virtual void tabCurrentChanged(int index) {}
    virtual void init() {}
    virtual void initMenu(QList<QMenu *> &menus) {}
    virtual QString tabName(int idx) { return QString("RF-%1").arg(idx); }

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

    QList<TreeChildItem *> treeChildItemsBuiltIn;
    QList<TreeChildItem *> treeChildItems;

    int _selectedItems;
};

#endif
