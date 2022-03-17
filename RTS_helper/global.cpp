#include "global.h"
#include <QTreeWidget>
#include <QTreeWidgetItem>

quint32 RFIdx = 0;
quint32 RFUIdx = 0;

complex_sequence *complexSequence;

quint32 currentRFIdx()
{
    return RFIdx;
}

void setCurrentRFIdx(const quint32 idx)
{
    RFIdx = idx;
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
