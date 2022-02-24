#ifndef QR1CRXATTMODEL_H
#define QR1CRXATTMODEL_H

#include <q_base_model.h>
#include "global.h"

class QwtRxAttOPData : public QwtCalData
{
    DECL_QWT_CAL_DATA(QwtRxAttOPData)

    typedef QVector<rx_att_op_table_r1cd::data_f_t>::iterator start_iter;

public:
    explicit QwtRxAttOPData(int idx);
    void locate2CalTable(start_iter iter);
    void addOneData();

private:
    start_iter iter;
    int n;
    int attIdx;
};

class QwtRxAttIOData : public QwtCalData
{
    DECL_QWT_CAL_DATA(QwtRxAttIOData)

    typedef QVector<rx_att_io_table_r1cd::data_f_t>::iterator start_iter;

public:
    explicit QwtRxAttIOData(int idx);
    void locate2CalTable(start_iter iter);
    void addOneData();

private:
    start_iter iter;
    int n;
    int attIdx;
};


class QR1CRxAttOPModel : public QCalBaseModel
{
    Q_OBJECT
    DECLARE_CAL_TABLE_MODEL(QR1CRxAttOPModel)

public:
    explicit QR1CRxAttOPModel(sp1401 *sp1401, QObject *parent = 0);
    QVector<rx_att_op_table_r1cd::data_f_t> *calTable();
    QVector<QwtRxAttOPData *> *iterTable(int idx);
    void init();
    void resetData();
    void setSP1401(sp1401 *);

private:
    QVector<rx_att_op_table_r1cd::data_f_t> *table;
    QVector<QwtRxAttOPData *> *iter;
    sp1401 *_sp1401;
};

class QR1CRxAttIOModel : public QCalBaseModel
{
    Q_OBJECT
    DECLARE_CAL_TABLE_MODEL(QR1CRxAttIOModel)

public:
    explicit QR1CRxAttIOModel(sp1401 *sp1401, QObject *parent = 0);
    QVector<rx_att_io_table_r1cd::data_f_t> *calTable();
    QVector<QwtRxAttIOData *> *iterTable(int idx);
    void init();
    void resetData();
    void setSP1401(sp1401 *);

private:
    QVector<rx_att_io_table_r1cd::data_f_t> *table;
    QVector<QwtRxAttIOData *> *iter;
    sp1401 *_sp1401;
};

#endif // QR1CRXATTMODEL_H
