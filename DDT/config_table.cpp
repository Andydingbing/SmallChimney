#include "config_table.h"
#include <QHeaderView>

Q_Config_Table_View::Q_Config_Table_View(QWidget *parent) :
    QTableView(parent)
{
    setSelectionMode(QAbstractItemView::NoSelection);
    setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    horizontalHeader()->setHidden(true);
    verticalHeader()->setHidden(true);
    verticalHeader()->setDefaultSectionSize(30);
}

void Q_Config_Table_View::setModel(QAbstractItemModel *model)
{
    QTableView::setModel(model);

    for (int i = 0;i < model->rowCount();++i) {
        openPersistentEditor(model->index(i,1));
    }

    resizeColumnsToContents();
    horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);
    horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
}

QVariant Q_Config_Table_Model::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole && index.column() == 0) {
        return _item.at(index.row());
    }
    return QVariant();
}


QWidget* Q_Config_Table_Delegate::createEditor(
        QWidget *parent,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{
    Q_UNUSED(option);

    if (index.column() != 1) {
        return nullptr;
    }

    QWidget *widget = static_cast<QWidget *>(*(first() + index.row()));
    widget->setParent(parent);

    return widget;
}

void Q_Config_Table_Delegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    Q_UNUSED(editor);
    Q_UNUSED(index);
}

void Q_Config_Table_Delegate::setModelData(
        QWidget *editor,
        QAbstractItemModel *model,
        const QModelIndex &index) const
{
    Q_UNUSED(editor);
    Q_UNUSED(model);
    Q_UNUSED(index);
}

void Q_Config_Table_Delegate::updateEditorGeometry(
        QWidget *editor,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{
    Q_UNUSED(index);

    editor->setGeometry(option.rect);
}
