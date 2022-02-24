#ifndef Q_MODEL_FILTER_OFFSET_H
#define Q_MODEL_FILTER_OFFSET_H

#include <q_base_model.h>
#include "global.h"

template <typename Data_T>
class QwtFilterOffsetData : public QwtCalData
{
public:
    typedef typename QVector<Data_T>::iterator start_iter;

    explicit QwtFilterOffsetData() :
        QwtCalData() { n = 0; }

    virtual size_t size() const
    { return n; }

    virtual QPointF sample(size_t i) const
    { return QPointF((*(iter + i)).freq / 1000000.0,(*(iter + i)).offset); }

    void locate2CalTable(start_iter iter)
    { this->iter = iter; }

    void addOneData()
    { n += 1; }

private:
    start_iter iter;
    int n;
};


template <typename Data_T>
class QFilterOffsetModel : public QCalBaseModel
{
public:
    typedef QwtFilterOffsetData<Data_T> Qwt_Data_T;

    explicit QFilterOffsetModel(QObject *parent) :
        QCalBaseModel(parent)
    {
        table = new QVector<Data_T>;
        table->clear();
        iter = new QVector<Qwt_Data_T *>;
        iter->clear();
    }

    int rowCount(const QModelIndex &parent) const
    { return table->size(); }

    int columnCount(const QModelIndex &parent) const
    { return 7; }

    QVector<Data_T> *calTable()
    { return table; }

    QVector<Qwt_Data_T *> *iterTable()
    { return iter; }

    void resetData()
    {
        typename QVector<Qwt_Data_T *>::const_iterator ptrIter;
        for (ptrIter = iter->constBegin();ptrIter != iter->constEnd();ptrIter ++) {
            delete *ptrIter;
        }
        iter->clear();
        table->clear();
    }

    QVariant data(const QModelIndex &index, int role) const
    {
        if (role == Qt::DisplayRole) {
            if (index.column() == 0) {
                return QString("%1").arg(table->at(index.row()).freq / 1000000);
            } else if (index.column() == 1) {
                return QString("%1").arg(table->at(index.row()).offset,0,'f',3);
            } else if (index.column() == 2) {
                return QString("%1").arg(table->at(index.row()).temp[0]);
            } else if (index.column() == 3) {
                return QString("%1").arg(table->at(index.row()).temp[1]);
            } else if (index.column() == 4) {
                return QString("%1").arg(table->at(index.row()).temp[2]);
            } else if (index.column() == 5) {
                return QString("%1").arg(table->at(index.row()).temp[3]);
            } else if (index.column() == 6) {
                return tm2QString(table->at(index.row()).time);
            }
        } else if (role == Qt::BackgroundColorRole) {
            return CLR_PROTECT_EYE;
        }
        return QVariant();
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
            QString strHeader[7] = {
                tr("Freq(MHz)"),
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

private:
    QVector<Data_T> *table;
    QVector<Qwt_Data_T *> *iter;
};

typedef QwtFilterOffsetData<tx_filter_offset_table::data_f_t> QwtTXFilterOffsetData;
typedef QFilterOffsetModel<tx_filter_offset_table::data_f_t> QTXFilterOffsetModel;

#endif // Q_MODEL_FILTER_OFFSET_H
