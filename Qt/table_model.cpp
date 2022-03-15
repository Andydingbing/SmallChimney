#include "table_model.h"

QVariant Q_Table_Model::data(const QModelIndex &index, int role) const
{
#if (QT_VERSION < QT_VERSION_CHECK(5,13,0))
    if (role == Qt::BackgroundColorRole) {
#else
    if (role == Qt::BackgroundRole) {
#endif
        return QColor(199,237,204);
    }

    return QVariant();
}

void Q_Table_Model::reset()
{
    beginResetModel();
    endResetModel();
}

void Q_Table_Model::uiInsert(const int first,const int last,const int kase)
{
    beginInsertRows(QModelIndex(),first,last);

    for (int i = first;i <= last;++i) {
        insertRow(i);
    }

    endInsertRows();

    emit dataChanged(index(first,0),index(last,columnCount() - 1));
}

void Q_Table_Model::uiUpdate(const int first,const int last,const int kase)
{
//    if (first < 0 || last < 0 || last < first) {
//        return;
//    }

//    int rowIdxMax = rowCount() > 0 ? rowCount() - 1 : 0;

//    if (last > rowIdxMax) {
//        beginInsertRows(QModelIndex(),rowIdxMax,last);
//        insertRows(rowIdxMax,last - rowIdxMax);
//        endInsertRows();
//    }

    emit dataChanged(index(first,0),index(last,columnCount() - 1),QVector<int>(Qt::DisplayRole));
}


Qt::ItemFlags Q_Checkable_Table_Model::flags(const QModelIndex &index) const
{
    if (index.column() == 0) {
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
    }
    return  Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant Q_Checkable_Table_Model::data(const QModelIndex &index, int role) const
{
    if (role == Qt::CheckStateRole) {
        if (index.column() == 0) {
           return checked.at(index.row()) ? Qt::Checked : Qt::Unchecked;
        }
    }
    return QVariant();
}

bool Q_Checkable_Table_Model::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::CheckStateRole && index.column() == 0) {                                                                                                          \
        checked[index.row()] = value.toBool();
        emit dataChanged(index,index);
    }
    return true;
}
