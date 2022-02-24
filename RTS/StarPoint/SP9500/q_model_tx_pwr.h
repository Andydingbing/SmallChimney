#ifndef Q_R1C_TX_PWR_MODEL_H
#define Q_R1C_TX_PWR_MODEL_H

#include "q_base_model.h"
#include "global.h"

class QwtTXPwrData : public QwtCalData
{
public:
    typedef QVector<tx_pwr_table_r1c::data_f_t>::iterator start_iter;

    explicit QwtTXPwrData() :
        QwtCalData() { n = 0; }

    virtual size_t size() const { return n; }

    virtual QPointF sample(size_t i) const
    { return QPointF((*(iter + i)).freq / 1000000.0,(*(iter + i)).d_gain); }

    void locate2CalTable(start_iter iter) { this->iter = iter; }
    void addOneData() { n += 1; }

private:
    start_iter iter;
    int n;
};


class QTXPwrModel : public QCalBaseModel
{
    Q_OBJECT

public:
    explicit QTXPwrModel(QObject *parent = nullptr) :
        QCalBaseModel(parent)
    {
        table = new QVector<tx_pwr_table_r1c::data_f_t>;
        table->clear();
        iter = new QVector<QwtTXPwrData *>;
        iter->clear();
    }

    ~QTXPwrModel()
    {
        table->clear(); delete table;
        iter->clear();  delete iter;
    }

    int rowCount(const QModelIndex &parent) const
    { return table->size(); }

    int columnCount(const QModelIndex &parent) const
    { return 11; }

    QVector<tx_pwr_table_r1c::data_f_t> *calTable() const
    { return table; }

    QVector<QwtTXPwrData *> *iterTable() const
    { return iter; }

    QVariant data(const QModelIndex &index, int role) const
    {
        if (role == Qt::DisplayRole) {
            if (index.column() == 0) {
                return QString("%1").arg(table->at(index.row()).freq / 1000000);
            } else if (index.column() == 1) {
                return QString("%1").arg(table->at(index.row()).d_gain,0,'f',3);
            } else if (index.column() == 2) {
                return QString("%1").arg(table->at(index.row()).att0);
            } else if (index.column() == 3) {
                return QString("%1").arg(table->at(index.row()).att1);
            } else if (index.column() == 4) {
                return QString("%1").arg(table->at(index.row()).att2);
            } else if (index.column() == 5) {
                return QString("%1").arg(table->at(index.row()).att3);
            } else if (index.column() == 6) {
                return QString("%1").arg(table->at(index.row()).temp[0]);
            } else if (index.column() == 7) {
                return QString("%1").arg(table->at(index.row()).temp[1]);
            } else if (index.column() == 8) {
                return QString("%1").arg(table->at(index.row()).temp[2]);
            } else if (index.column() == 9) {
                return QString("%1").arg(table->at(index.row()).temp[3]);
            } else if (index.column() == 10) {
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
            QString strHeader[11] = {
                tr("Freq(MHz)"),
                tr("DGain(dBm)"),
                tr("Att0"),
                tr("Att1"),
                tr("Att2"),
                tr("Att3"),
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

    void resetData()
    {
        QVector<QwtTXPwrData *>::const_iterator ptrIter;

        for (ptrIter = iter->constBegin();ptrIter != iter->constEnd();ptrIter ++) {
            delete *ptrIter;
        }

        iter->clear();
        table->clear();
    }

private:
    QVector<tx_pwr_table_r1c::data_f_t> *table;
    QVector<QwtTXPwrData *> *iter;
};


#endif // Q_R1C_TX_PWR_MODEL_H
