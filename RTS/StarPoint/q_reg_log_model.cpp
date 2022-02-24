#include "q_reg_log_model.h"
#include "global.h"

QRegLogModel::QRegLogModel(QObject *parent) : QAbstractTableModel(parent)
{

}

int QRegLogModel::rowCount(const QModelIndex &parent) const
{
    return int(Log.regs()->size());
}

int QRegLogModel::columnCount(const QModelIndex &parent) const
{
    return 6;
}

QVariant QRegLogModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        if (0 == index.column())
            return QString::fromStdString(Log.regs()->at(index.row()).time);
        else if (1 == index.column())
            return QString::fromStdString(Log.regs()->at(index.row()).fpga);
        else if (2 == index.column())
            return QString("0x") + QString("%1").arg(Log.regs()->at(index.row()).addr,4,16,QLatin1Char('0')).toUpper();
        else if (3 == index.column() && 0xffffffff != Log.regs()->at(index.row()).w)
            return QString("0x") + QString("%1").arg(Log.regs()->at(index.row()).w,8,16,QLatin1Char('0')).toUpper();
        else if (4 == index.column() && 0xffffffff != Log.regs()->at(index.row()).r)
            return QString("0x") + QString("%1").arg(Log.regs()->at(index.row()).r,8,16,QLatin1Char('0')).toUpper();
        else if (5 == index.column())
            return Log.regs()->at(index.row()).result ? QString("x") : QString("-");
    }
    if (role == Qt::BackgroundColorRole) {
        if (Log.regs()->at(index.row()).result)
            return CLR_ERROR;
        else
            return CLR_PROTECT_EYE;
    }
    return QVariant();
}

QVariant QRegLogModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && Qt::Horizontal == orientation) {
        QString strHeader[6] = {tr("Time"),
                                tr("FPGA"),
                                tr("Addr"),
                                tr("Write"),
                                tr("Read"),
                                tr("Result")};
        return strHeader[section];
    }
    return QVariant();
}

void QRegLogModel::update(int iRow)
{
    beginInsertRows(QModelIndex(),iRow,iRow);
    insertRow(iRow);
    endInsertRows();
}

