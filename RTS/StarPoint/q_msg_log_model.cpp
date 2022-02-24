#include "q_msg_log_model.h"
#include "global.h"

QMsgLogModel::QMsgLogModel(QObject *parent) : QAbstractTableModel(parent)
{

}

int QMsgLogModel::rowCount(const QModelIndex &parent) const
{
    return int(Log.msgs()->size());
}

int QMsgLogModel::columnCount(const QModelIndex &parent) const
{
    return 4;
}

QVariant QMsgLogModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        if (0 == index.column())
            return QString::fromStdString(Log.msgs()->at(index.row()).time);
        else if (1 == index.column())
            return QString::fromStdString(Log.msgs()->at(index.row()).msg);
        else if (2 == index.column())
            return Log.msgs()->at(index.row()).result ? "x" : "-";
        else if (3 == index.column())
            return QString("0x") +
                   QString("%1").arg(Log.msgs()->at(index.row()).result,16,16,QLatin1Char('0'))
                                .toUpper()
                                .right(8);
    }
    if (role == Qt::BackgroundColorRole) {
        if (Log.msgs()->at(index.row()).result)
            return CLR_ERROR;
        else
            return CLR_PROTECT_EYE;
    }
    return QVariant();
}

QVariant QMsgLogModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && Qt::Horizontal == orientation) {
        QString strHeader[4] = {tr("Time"),
                                tr("Message"),
                                tr("Result"),
                                tr("Errno")};
        return strHeader[section];
    }
    return QVariant();
}

void QMsgLogModel::update(int iRow)
{
    beginInsertRows(QModelIndex(),iRow,iRow);
    insertRow(iRow);
    endInsertRows();
    emit dataChanged(this->index(iRow,0),this->index(iRow,3));
}
