#ifndef QTROUBLESHOOTMODEL_H
#define QTROUBLESHOOTMODEL_H

#include <QAbstractTableModel>
#include <QMetaEnum>
#include "global.h"

class QTroubleShootTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    DECLARE_TX_TS_ENUM(txTroubles)
    DECLARE_RX_TS_ENUM(rxTroubles)
    Q_ENUM(txTroubles)
    Q_ENUM(rxTroubles)

public:
    explicit QTroubleShootTableModel(QObject *parent = 0) : QAbstractTableModel(parent) {}

    int columnCount(const QModelIndex &parent) const {return 2;}

    QVariant headerData(int section, Qt::Orientation orientation, int role) const {
        if (Qt::DisplayRole == role && Qt::Horizontal == orientation) {
            QString strHeader[2] = {
                tr(""),
                tr("Trouble")
            };
            return strHeader[section];
        }
        return QVariant();
    }

    Qt::ItemFlags flags(const QModelIndex &index) const {
        if (index.column() == 0) {
            return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
        }
        return  Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    }
};

// Q_OBJECT does not support template,use macro instead.

#define DECLARE_TROUBLE_SHOOT_MODEL(name, ts_t, ts_item_t, ts_begin, ts_max)                                    \
class name : public QTroubleShootTableModel {                                                                   \
    Q_OBJECT                                                                                                    \
public:                                                                                                         \
    explicit name(QObject *parent = 0) : QTroubleShootTableModel(parent) {                                      \
        _items = new QVector<ts_item_t>;                                                                        \
        for (quint32 i = 1;i < ts_max - ts_begin;i ++)                                                          \
            _items->push_back(ts_item_t(i));                                                                    \
    }                                                                                                           \
    QVector<ts_item_t> *items() const    {return _items;}                                                       \
    int rowCount(const QModelIndex &parent) const    {return ts_max - ts_begin - 1;}                            \
    QVariant data(const QModelIndex &index, int role) const {                                                   \
        if (role == Qt::CheckStateRole) {                                                                       \
            if (index.column() == 0)                                                                            \
               return _items->at(index.row()).is_attached ? Qt::Checked : Qt::Unchecked;                        \
        }                                                                                                       \
        if (role == Qt::DisplayRole) {                                                                          \
            if (index.column() == 1)                                                                            \
                return QString(QMetaEnum::fromType<ts_t>().valueToKey(_items->at(index.row()).t + ts_begin));   \
        }                                                                                                       \
        return QVariant();                                                                                      \
    }                                                                                                           \
    bool setData(const QModelIndex &index, const QVariant &value, int role) {                                   \
        if (role == Qt::CheckStateRole && index.column() == 0) {                                                \
            ts_item_t txTS = _items->at(index.row());                                                           \
            txTS.is_attached = value.toBool();                                                                  \
            _items->replace(index.row(),txTS);                                                                  \
            emit dataChanged(index,index);                                                                      \
        }                                                                                                       \
        return true;                                                                                            \
    }                                                                                                           \
private:                                                                                                        \
    QVector<ts_item_t> *_items;                                                                                 \
};

DECLARE_TROUBLE_SHOOT_MODEL(QTxTroubleShootModel, txTroubles, ns_sp1401::tx_trouble_shoot_item, TX_TS_BEGIN, TX_TS_MAX)
DECLARE_TROUBLE_SHOOT_MODEL(QRxTroubleShootModel, rxTroubles, ns_sp1401::rx_trouble_shoot_item, RX_TS_BEGIN, RX_TS_MAX)

#endif // QTROUBLESHOOTMODEL_H
