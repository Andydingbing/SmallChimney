#ifndef Q_LOG_MODEL_H
#define Q_LOG_MODEL_H

#include "global.h"
#include <QAbstractTableModel>

class QMsgLogModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit QMsgLogModel(QObject *parent = nullptr) :
        QAbstractTableModel(parent) {}

    int rowCount(const QModelIndex &/*parent*/) const
    { return int(Log.msgs()); }

    int columnCount(const QModelIndex &/*parent*/) const
    { return 5; }

    QVariant data(const QModelIndex &index, int role) const
    {
        int row = index.row();

        if (role == Qt::DisplayRole) {
            if (index.column() == 0) {
                return QString("%1").arg(row);
            } else if (index.column() == 1) {
                return QString::fromStdString(Log.msg(row)->time);
            } else if (index.column() == 2) {
                return QString::fromStdString(Log.msg(row)->msg);
            } else if (index.column() == 3) {
                return Log.msg(row)->result ? "x" : "-";
            } else if (index.column() == 4) {
                return QString("0x%1").arg(Log.msg(row)->result,16,16,QLatin1Char('0')).toUpper().right(8);
            }
        } else if (role == Qt::BackgroundColorRole) {
            return Log.msg(row)->result ? QColor(255,190,190) : CLR_PROTECT_EYE;
        }
        return QVariant();
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
            QString strHeader[5] = {"",tr("Time"), tr("Message"), tr("Result"), tr("Errno")};
            return strHeader[section];
        }
        return QVariant();
    }

public slots:
    void update(int row)
    {
        beginInsertRows(QModelIndex(),row,row);
        insertRow(row);
        endInsertRows();
        emit dataChanged(this->index(row,0),this->index(row,4));
    }
};

class QRegLogModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit QRegLogModel(QObject *parent = nullptr) :
        QAbstractTableModel(parent) {}

    int rowCount(const QModelIndex &parent) const
    { Q_UNUSED(parent); return int(Log.regs()); }

    int columnCount(const QModelIndex &parent) const
    { Q_UNUSED(parent); return 6; }

    QVariant data(const QModelIndex &index, int role) const
    {
        int row = index.row();
        if (role == Qt::DisplayRole) {
            if (index.column() == 0) {
                return QString::fromStdString(Log.reg(row)->time);
            } else if (index.column() == 1) {
                return QString::fromStdString(Log.reg(row)->fpga);
            } else if (index.column() == 2) {
                return QString("0x") + QString("%1").arg(Log.reg(row)->addr,4,16,QLatin1Char('0')).toUpper();
            } else if (index.column() == 3 && Log.reg(row)->w != 0xffffffff) {
                return QString("0x") + QString("%1").arg(Log.reg(row)->w,8,16,QLatin1Char('0')).toUpper();
            } else if (index.column() == 4 && Log.reg(row)->r != 0xffffffff) {
                return QString("0x") + QString("%1").arg(Log.reg(row)->r,8,16,QLatin1Char('0')).toUpper();
            } else if (index.column() == 5) {
                return Log.reg(row)->result ? QString("x") : QString("-");
            }
        } else if (role == Qt::BackgroundColorRole) {
            return Log.reg(row)->result ? QColor(255,190,190) : CLR_PROTECT_EYE;
        }
        return QVariant();
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (role == Qt::DisplayRole && Qt::Horizontal == orientation) {
            QString strHeader[6] = {tr("Time"), tr("FPGA"), tr("Addr"), tr("Write"), tr("Read"), tr("Result")};
            return strHeader[section];
        }
        return QVariant();
    }

public slots:
    void update(int row)
    {
        beginInsertRows(QModelIndex(),row,row);
        insertRow(row);
        endInsertRows();
    }
};

#endif // Q_LOG_MODEL_H
