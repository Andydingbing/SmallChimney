#ifndef Q_TX_SB_MODEL_H
#define Q_TX_SB_MODEL_H

#include "q_base_model.h"
#include "global.h"

class QTXSBModel : public QCalBaseModel
{
    Q_OBJECT

public:
    explicit QTXSBModel(QObject *parent) :
        QCalBaseModel(parent),
        table(new QVector<tx_sb_table_r1cd::data_f_t>)
    { table->clear(); }

    ~QTXSBModel()
    { table->clear(); delete table; }

    int rowCount(const QModelIndex &parent) const
    { Q_UNUSED(parent); return table->size(); }

    int columnCount(const QModelIndex &parent) const
    { Q_UNUSED(parent); return 11; }

    QVector<tx_sb_table_r1cd::data_f_t> *calTable() const
    { return table; }

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    QVector<tx_sb_table_r1cd::data_f_t> *table;
};

#endif // Q_TX_SB_MODEL_H
