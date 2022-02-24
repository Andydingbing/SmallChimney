#ifndef Q_TX_ATT_MODEL_H
#define Q_TX_ATT_MODEL_H

#include <q_base_model.h>
#include "global.h"

template <typename Table_T>
class QwtTXAttData : public QwtCalData
{
public:
    typedef typename Table_T::data_f_t Data_T;
    typedef typename QVector<Data_T>::iterator start_iter;

    explicit QwtTXAttData() :
        QwtCalData() { n = 0; }

    explicit QwtTXAttData(int idx) :
        QwtCalData(),
        n(0),
        attIdx(idx) {}

    virtual size_t size() const
    { return n; }

    virtual QPointF sample(size_t i) const
    { return QPointF((*(iter + i)).freq / 1000000.0,(*(iter + i)).offset[attIdx]); }

    void locate2CalTable(start_iter iter)
    { this->iter = iter; }

    void addOneData()
    { n += 1; }

private:
    start_iter iter;
    int n;
    int attIdx;
};

typedef QwtTXAttData<tx_att_op_table_r1cd> QwtR1CTXAttOPData;
typedef QwtTXAttData<tx_att_io_table_r1cd> QwtR1CTXAttIOData;


template <typename Table_T, quint32 Att_Pts>
class QR1CTXAttModel : public QCalBaseModel
{
public:
    typedef typename Table_T::data_f_t Data_T;
    typedef QwtTXAttData<Table_T> Qwt_Data_T;

    explicit QR1CTXAttModel(QObject *parent) :
        QCalBaseModel(parent) { init(); }

    explicit QR1CTXAttModel(sp1401 *sp1401, QObject *parent) :
        QCalBaseModel(parent),
        _sp1401(sp1401) { init(); }

    QVariant data(const QModelIndex &index, int role) const = 0;

    int rowCount(const QModelIndex &parent) const
    { return table->size() * Att_Pts; }

    int columnCount(const QModelIndex &parent) const
    { return 13; }

    QVector<Data_T> *calTable()
    { return table; }

    QVector<Qwt_Data_T *> *iterTable(int idx)
    { return &iter[idx]; }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
            QString strHeader[13] = {
                tr("Freq(MHz)"),
                tr("Power(dBm)"),
                tr("Att0"),
                tr("Att1"),
                tr("Att2"),
                tr("Att3"),
                tr("DGain"),
                tr("Offset"),
                tr("Temp4_TxLO2"),
                tr("Temp5_TxLO1"),
                tr("Temp6_TxPA4"),
                tr("Temp7_TxPA3"),
                tr("End Time")
            };
            return strHeader[section];
        }
        return QVariant();
    }

public:
    void init()
    {
        table = new QVector<Data_T>;
        table->clear();
        iter = new QVector<Qwt_Data_T *>[Att_Pts];
        for (quint32 i = 0;i < Att_Pts;i ++) {
            iter[i].clear();
        }
    }

    void resetData()
    {
        typename QVector<Qwt_Data_T *>::const_iterator ptrIter;

        for (quint32 i = 0;i < Att_Pts;i ++) {
            for (ptrIter = iter[i].constBegin();ptrIter != iter[i].constEnd();ptrIter ++) {
                delete *ptrIter;
            }
            iter[i].clear();
        }
        table->clear();
    }

    void setSP1401(sp1401 *sp1401) { _sp1401 = sp1401; }

protected:
    QVector<Data_T> *table;
    QVector<Qwt_Data_T *> *iter;
    sp1401 *_sp1401;
};

class QR1CTXAttOPModel :
        public QR1CTXAttModel<tx_att_op_table_r1cd,R1C_TX_ATT_OP_POWER_PTS>
{
public:
    explicit QR1CTXAttOPModel(QObject *parent) :
        QR1CTXAttModel(parent) {}

    explicit QR1CTXAttOPModel(sp1401 *sp1401, QObject *parent) :
        QR1CTXAttModel(sp1401,parent) {}

    QVariant data(const QModelIndex &index, int role) const;
};

class QR1CTXAttIOModel :
        public QR1CTXAttModel<tx_att_io_table_r1cd,R1C_TX_ATT_IO_POWER_PTS>
{
public:
    explicit QR1CTXAttIOModel(QObject *parent) :
        QR1CTXAttModel(parent) {}

    explicit QR1CTXAttIOModel(sp1401 *sp1401, QObject *parent) :
        QR1CTXAttModel(sp1401,parent) {}

    QVariant data(const QModelIndex &index, int role) const;
};

#endif // Q_TX_ATT_MODEL_H
