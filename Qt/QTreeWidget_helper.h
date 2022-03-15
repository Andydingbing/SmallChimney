#ifndef QTreeWidget_HELPER_H
#define QTreeWidget_HELPER_H

#include <QtGui/QList>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QTreeWidgetItem>

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

#endif
