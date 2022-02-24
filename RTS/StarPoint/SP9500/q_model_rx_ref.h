#ifndef QR1CRXREFMODEL_H
#define QR1CRXREFMODEL_H

#include "q_base_model.h"
#include "global.h"

class QwtRXRefData : public QwtCalData
{
    typedef QVector<rx_ref_table_r1cd::data_f_t>::iterator start_iter;

public:
    explicit QwtRXRefData() :
        QwtCalData() { n = 0; }

    explicit QwtRXRefData(int idx) :
        QwtCalData() { refIdx = idx;n = 0; }

    size_t size() const { return n; }

    QPointF sample(size_t i) const;

    void locate2CalTable(start_iter iter)
    { this->iter = iter; }

    void addOneData() { n += 1; }

private:
    start_iter iter;
    int n;
    int refIdx;
};


class QR1CRXRefModel : public QCalBaseModel
{
    Q_OBJECT
    DECLARE_CAL_TABLE_MODEL(QR1CRXRefModel)

public:
    QVector<rx_ref_table_r1cd::data_f_t> *calTable();
    QVector<QwtRXRefData *> *iterTable(int idx);
    void resetData();

private:
    QVector<rx_ref_table_r1cd::data_f_t> *table;
    QVector<QwtRXRefData *> *iter;
};

#endif // QR1CRXREFMODEL_H
