#ifndef Q_MODEL_RX_FILTER_OFFSET_H
#define Q_MODEL_RX_FILTER_OFFSET_H

#include <q_base_model.h>
#include "global.h"

class QwtRXFilterOffsetData : public QwtCalData
{
public:
    typedef QVector<rx_filter_offset_table::data_f_t>::iterator start_iter;

    explicit QwtRXFilterOffsetData() :
        QwtCalData() { n = 0; }

    explicit QwtRXFilterOffsetData(int idx) :
        QwtCalData() { n = 0; offsetIdx = 0; }

    virtual size_t size() const
    { return n; }

    virtual QPointF sample(size_t i) const;

    void locate2CalTable(start_iter iter)
    { this->iter = iter; }

    void addOneData()
    { n += 1; }

private:
    start_iter iter;
    int n;
    int offsetIdx;
};


class QRXFilterOffsetModel : public QCalBaseModel
{
    Q_OBJECT

public:
    explicit QRXFilterOffsetModel(QObject *parent) :
        QCalBaseModel(parent)
    {
        table = new QVector<rx_filter_offset_table::data_f_t>;
        table->clear();
        iter = new QVector<QwtRXFilterOffsetData *>[3];
        iter->clear();
    }

    int rowCount(const QModelIndex &parent) const
    { return table->size(); }

    int columnCount(const QModelIndex &parent) const
    { return 9; }

    QVector<rx_filter_offset_table::data_f_t> *calTable()
    { return table; }

    QVector<QwtRXFilterOffsetData *> *iterTable(int idx)
    { return &iter[idx]; }

    void resetData()
    {
        QVector<QwtRXFilterOffsetData *>::const_iterator ptrIter;
        for (quint8 i = 0;i < 3;i ++) {
            for (ptrIter = iter[i].constBegin();ptrIter != iter[i].constEnd();ptrIter ++) {
                delete *ptrIter;
            }
            iter[i].clear();
        }
        table->clear();
    }

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    QVector<rx_filter_offset_table::data_f_t> *table;
    QVector<QwtRXFilterOffsetData *> *iter;
};

#endif // Q_MODEL_RX_FILTER_OFFSET_H
