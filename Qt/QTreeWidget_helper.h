#ifndef QTreeWidget_HELPER_H
#define QTreeWidget_HELPER_H

#include <QtGui/QList>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QTreeWidgetItem>

class QTreeWidget_helper
{
public:
    static void itemList(QTreeWidgetItem *item, QList<QTreeWidgetItem *>::iterator *iter)
    {
        if (item->childCount() == 0) {
            (*(*iter)) = item;
            (*iter) ++;
            return;
        }

        QTreeWidgetItem *childItem = nullptr;

        for (int i = 0;i < item->childCount();++i) {
            childItem = item->child(i);
            itemList(childItem,iter);
        }
    }

    static QList<QTreeWidgetItem *> itemList(QTreeWidget &widget)
    {
        QList<QTreeWidgetItem *> list;

        for (int i = 0;i < treeChildItems.size();++ i) {
            list.push_back(nullptr);
        }

        QList<QTreeWidgetItem *>::iterator iter = list.begin();

        for (int i = 0;i < widget.topLevelItemCount();++i) {
            itemList(widget.topLevelItem(i),&iter);
        }

        return list;
    }

};

#endif
