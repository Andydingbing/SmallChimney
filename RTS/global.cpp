#include "global.h"
#include <QTreeWidget>
#include <QTreeWidgetItem>

Project project = Ericsson_Radio_4415_B3;
Mode mode = Sequence;

quint32 RFIdx = 0;
quint32 RFUIdx = 0;

int checkedItems(const QTreeWidget *tree)
{
    return 0;
}

void selectFirst(QTreeWidget *tree, QTreeWidgetItem *item)
{
    if (item->childCount() == 0) {
        tree->setCurrentItem(item);
        return;
    }

    selectFirst(tree,item->child(0));
}

QTreeWidgetItem *selectFirst(QTreeWidget *tree)
{
    if (tree->topLevelItemCount() == 0) {
        return nullptr;
    }
    selectFirst(tree,tree->topLevelItem(0));
    return tree->currentItem();
}
