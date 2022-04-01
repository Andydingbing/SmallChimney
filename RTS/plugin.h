#ifndef RTS_PLUGIN_H
#define RTS_PLUGIN_H

#include "global.h"
#include "thread_widget.h"
#include "../utilities/string_util.hpp"
#include <string>
#include <QList>
#include <QString>
#include <QWidget>
#include <QMenu>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <boost/dll/alias.hpp>


struct TreeChildItem
{
    std::list<std::string> stringList;
    Qt::CheckState checkState;
    QList<Q_Widget *> *tabWidgets;
};

#define DECL_TREE(item_string_list,kase_class,n) \
{ \
    std::string str = item_string_list; \
    std::list<std::string> seperator; \
    seperator.push_back(","); \
    TreeChildItem *item = new TreeChildItem; \
    QList<kase_class *> * kase_class##s = new QList<kase_class *>; \
    for (size_t i = 0;i < n;i ++) { \
        kase_class##s->push_back(new kase_class(nullptr)); \
        kase_class##s->at(i)->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding); \
        kase_class##s->at(i)->setVisible(false); \
    } \
    split(str.c_str(),seperator,item->stringList); \
    item->checkState = Qt::Checked; \
    item->tabWidgets = reinterpret_cast<QList<Q_Widget *> *>(kase_class##s); \
    _treeChildItemsBuiltIn.append(item); \
    /*MainWindow::childDlgLayout.addWidget(kase_class##s->at(i));*/ \
}

class PlugIn
{
public:
    virtual ~PlugIn() {}
    virtual void treeItemClicked(QTreeWidgetItem *item, int column) = 0;
    virtual void tabCurrentChanged(int index) = 0;
    virtual void init() = 0;
    virtual void initMenu(QList<QMenu *> &menus) = 0;

    QList<TreeChildItem *> *treeChildItemsBuiltIn() { return &_treeChildItemsBuiltIn; }
    QList<TreeChildItem *> *treeChildItems() { return &_treeChildItems; }

    QList<TreeChildItem *> _treeChildItemsBuiltIn;
    QList<TreeChildItem *> _treeChildItems;
};

#endif
