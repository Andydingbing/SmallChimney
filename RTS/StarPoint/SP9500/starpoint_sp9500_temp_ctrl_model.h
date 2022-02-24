#ifndef Q_R1C_TEMP_CTRL_MODEL_H
#define Q_R1C_TEMP_CTRL_MODEL_H

#include "q_base_model.h"

class QwtTempData : public QwtCalData
{
public:
    struct Data {
        double temp;
        qint64 i;
        tm time;
    };

    explicit QwtTempData() : QwtCalData()
    { table = new QVector<Data>; }

    virtual size_t size() const
    { return table->size(); }

    virtual QPointF sample(size_t i) const
    { return QPointF(table->at(i).i,table->at(i).temp); }

    QVector<Data> *calTable() const
    { return table;}

public:
    QVector<Data> *table;
};

#endif
