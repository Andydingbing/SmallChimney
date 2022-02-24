#ifndef Q_TX_LOLEAK_MODEL_H
#define Q_TX_LOLEAK_MODEL_H

#include "q_base_model.h"
#include "global.h"

class QTXLOLeakModel : public QCalBaseModel
{
    Q_OBJECT

public:
    explicit QTXLOLeakModel(QObject *parent) :
        QCalBaseModel(parent),
        table(new QVector<tx_lol_table_r1cd::data_f_t>)
    { table->clear(); }

    ~QTXLOLeakModel()
    { table->clear(); delete table; }

    int rowCount(const QModelIndex &parent) const
    { Q_UNUSED(parent); return table->size(); }

    int columnCount(const QModelIndex &parent) const
    { Q_UNUSED(parent); return 10; }

    QVector<tx_lol_table_r1cd::data_f_t> *calTable() const
    { return table; }

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    QVector<tx_lol_table_r1cd::data_f_t> *table;
};

#endif // Q_TX_LOLEAK_MODEL_H
