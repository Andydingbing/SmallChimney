#ifndef CHILD_WIDGET_HELPER_HPP
#define CHILD_WIDGET_HELPER_HPP

#include "mainwindow.h"
#include <QObject>
#include <QSizePolicy>

namespace Ui { class MainWindow; }

#define DECL_TREE(list,kase_class,n) \
{ \
    QList<kase_class *> * kase_class##s = new QList<kase_class *>; \
    std::string str = list; \
    treeChildItemsBuiltIn.append(new TreeChildItem(str,Qt::Checked,kase_class##s)); \
    for (size_t i = 0;i < n;i ++) { \
        kase_class##s->push_back(new kase_class(parent->mainTab)); \
        kase_class##s->at(i)->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding); \
        kase_class##s->at(i)->setVisible(false); \
        MainWindow::childDlgLayout.addWidget(kase_class##s->at(i)); \
    } \
}

#include <QMenu>
#include <QAction>

class QTreeWidgetItem;

class ChildWidgetHelper : public QObject
{
    Q_OBJECT

public:
    struct TreeChildItem {
        std::list<std::string> stringList;
        Qt::CheckState checkState;
        QList<Q_Widget *> *tabWidgets;

        TreeChildItem(std::string &str,Qt::CheckState _checkState,void *_tabWidgets = nullptr);
    };

    ChildWidgetHelper(MainWindow *_parent);
    Q_Widget *currentWidget();
    Qt::CheckState currentTreeItemCheckState();

    TreeChildItem *containTreeChildItem(const std::list<std::string> &l) const;
    void reSortTreeChildItem(const sequence &s);
    void setMainTree(const QList<TreeChildItem *> childItems);
    void mainTreeItemClicked(QTreeWidgetItem *item, int column);
    QList<bool> checkList();
    QList<QTreeWidgetItem *> treeWidgetItemList();

    virtual void mainTabCurrentChanged(int index) {}
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
    MainWindow *parent;
    Ui::MainWindow *ui;

    QList<TreeChildItem *> treeChildItemsBuiltIn;
    QList<TreeChildItem *> treeChildItems;

    int _selectedItems;
};

#endif
