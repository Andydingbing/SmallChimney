#ifndef Q_TX_FILTER_MODEL_H
#define Q_TX_FILTER_MODEL_H

#include "q_base_model.h"
#include "global.h"

typedef QwtFreqResponseData<tx_rf_fr_table::data_f_t> Qwt_TX_RF_FR_Data;
typedef QwtFreqResponseData<tx_if_fr_table::data_f_t> Qwt_TX_IF_FR_Data;

class QTXFilterModel : public QCalBaseModel
{
    Q_OBJECT

public:
    QTXFilterModel(QObject *parent) :
        QCalBaseModel(parent),
        table(new QVector<tx_filter_80m_table::data_f_t>)
    { table->clear(); }

    ~QTXFilterModel()
    { table->clear(); delete table; }

    int rowCount(const QModelIndex &parent) const
    { Q_UNUSED(parent); return table->size(); }

    int columnCount(const QModelIndex &parent) const
    { Q_UNUSED(parent); return 8; }

    QVector<tx_filter_table::data_f_t> *calTable() const
    { return table; }

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    QVector<tx_filter_table::data_f_t> *table;
};

#endif // Q_TX_FILTER_MODEL_H
