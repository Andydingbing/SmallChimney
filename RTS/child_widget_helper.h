#ifndef CHILD_WIDGET_HELPER_HPP
#define CHILD_WIDGET_HELPER_HPP

#include "mainwindow.h"
#include <QObject>
#include <QSizePolicy>

namespace Ui { class MainWindow; }

#define DECL_TREE(list,kase_class,n) \
    { \
        QList<kase_class *> * kase_class##s = new QList<kase_class *>; \
        QStringList str; \
        _items.append(new TreeChildItem(str << list,Qt::Checked,kase_class##s)); \
        for (size_t i = 0;i < n;i ++) { \
            kase_class##s->push_back(new kase_class(parent->mainTab)); \
            kase_class##s->at(i)->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding); \
            kase_class##s->at(i)->setVisible(false); \
            MainWindow::childDlgLayout.addWidget(kase_class##s->at(i)); \
        } \
    }


class QMenu;
class QAction;
class QTreeWidgetItem;

class ChildWidgetHelper : public QObject
{
    Q_OBJECT

public:
    struct TreeChildItem {
        QStringList stringList;
        Qt::CheckState checkState;
        QList<Q_Widget *> *tabWidgets;

        TreeChildItem(QStringList &_stringList,Qt::CheckState _checkState,void *_tabWidgets = nullptr);
    };

    ChildWidgetHelper(MainWindow *_parent);
    Q_Widget *currentWidget();
    Qt::CheckState currentTreeItemCheckState();

    void setMainTree(const QList<TreeChildItem *> childItems);
    void mainTreeItemClicked(QTreeWidgetItem *item, int column);
    QList<bool> checkList();
    QList<QTreeWidgetItem *> treeWidgetItemList();

    virtual void mainTabCurrentChanged(int index) {}
    virtual void init() {}
    virtual void initMenu() {}
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

    QList<TreeChildItem *> _items;

    int _selectedItems;
};

#endif // CHILD_WIDGET_HELPER_HPP
