#ifndef RTS_PLUGIN_H
#define RTS_PLUGIN_H

#include "thread_widget.h"
#include "../plugin/global.h"
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
    TreeChildItem *item = new TreeChildItem; \
    QList<kase_class *> * kase_class##s = new QList<kase_class *>; \
    for (size_t i = 0;i < n;i ++) { \
        kase_class##s->push_back(new kase_class(nullptr)); \
        kase_class##s->at(i)->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding); \
        kase_class##s->at(i)->setVisible(false); \
    } \
    split(str,",",item->stringList); \
    item->checkState = Qt::Checked; \
    item->tabWidgets = reinterpret_cast<QList<Q_Widget *> *>(kase_class##s); \
    _treeChildItemsBuiltIn.append(item); \
}

class PlugIn
{
public:
    PlugIn(int32_t (*hash_generator)() = plugin_hash_generator) : _hash(hash_generator()) {}
    virtual ~PlugIn() {}

    virtual std::string version() = 0;
    virtual std::string projectMenu() = 0;
    virtual std::string tabName(int idx) = 0;
    virtual void treeItemClicked(QTreeWidgetItem *item, int column) = 0;
    virtual void tabCurrentChanged(int index) = 0;
    virtual void init() = 0;
    virtual void initMenu(QList<QMenu *> &menus) = 0;

    int32_t hash() const { return _hash; }
    void set_hash(const int32_t h) { _hash = h; }

    std::string path() const { return _path; }
    void set_path(const std::string &p) { _path = p; }

    QList<TreeChildItem *> *treeChildItemsBuiltIn() { return &_treeChildItemsBuiltIn; }
    QList<TreeChildItem *> *treeChildItems() { return &_treeChildItems; }

protected:
    int32_t _hash;
    std::string _path;
    QList<TreeChildItem *> _treeChildItemsBuiltIn;
    QList<TreeChildItem *> _treeChildItems;
};

typedef PlugIn* (pluginapi_create_t)();

#endif
