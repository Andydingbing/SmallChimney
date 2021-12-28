#ifndef Q_LOG_MODEL_H
#define Q_LOG_MODEL_H

#include "global.h"
#include "../Qt/table_model.h"

class QMsgLogModel : public Q_Table_Model
{
    Q_OBJECT

public:
    QMsgLogModel(QObject *parent = nullptr) :
        Q_Table_Model(parent) {}

    int rowCount(const QModelIndex &) const
    { return int(LoggerMsg.size()); }

    MODEL_DISPLAY_PREFIX("","Time","Message","Result","Errno");
    MODEL_COLUMN(0,QString("%1").arg(row));
    MODEL_COLUMN(1,QString::fromStdString(LoggerMsg.at(row)->time));
    MODEL_COLUMN(2,QString::fromStdString(LoggerMsg.at(row)->msg));
    MODEL_COLUMN(3,LoggerMsg.at(row)->result ? "x" : "-");
    MODEL_COLUMN(4,QString("0x%1").arg(LoggerMsg.at(row)->result,16,16,QLatin1Char('0')).toUpper().right(8),end);

public slots:
    void update(int row)
    {
        beginInsertRows(QModelIndex(),row,row);
        insertRow(row);
        endInsertRows();
        emit dataChanged(this->index(row,0),this->index(row,4));
    }
};

class QRegLogModel : public Q_Table_Model
{
    Q_OBJECT

public:
    QRegLogModel(QObject *parent = nullptr) :
        Q_Table_Model(parent) {}

    int rowCount(const QModelIndex &) const
    { return int(Log.regs()); }

    MODEL_DISPLAY_PREFIX("Time","FPGA","Addr","Write","Read","Result");
    MODEL_COLUMN(0,QString::fromStdString(Log.reg(row)->time));
    MODEL_COLUMN(1,QString::fromStdString(Log.reg(row)->fpga));
    MODEL_COLUMN(2,QString("0x") + QString("%1").arg(Log.reg(row)->addr,4,16,QLatin1Char('0')).toUpper());
    MODEL_COLUMN(3,QString("0x") + QString("%1").arg(Log.reg(row)->w,8,16,QLatin1Char('0')).toUpper());
    MODEL_COLUMN(4,QString("0x") + QString("%1").arg(Log.reg(row)->r,8,16,QLatin1Char('0')).toUpper());
    MODEL_COLUMN(5,Log.reg(row)->result ? QString("x") : QString("-"),end);

public slots:
    void update(int row)
    {
        beginInsertRows(QModelIndex(),row,row);
        insertRow(row);
        endInsertRows();
    }
};

#endif // Q_LOG_MODEL_H
