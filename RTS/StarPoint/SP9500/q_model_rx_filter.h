#ifndef Q_RX_FILTER_MODEL_H
#define Q_RX_FILTER_MODEL_H

#include "q_base_model.h"
#include "global.h"

typedef QwtFreqResponseData<rx_rf_fr_table::data_f_t> Qwt_RX_RF_FR_Data;
typedef QwtFreqResponseData<rx_if_fr_table::data_f_t> Qwt_RX_IF_FR_Data;

template <typename Data_T,int Order>
class QRXFilterModel : public QCalBaseModel
{
public:
    explicit QRXFilterModel(QObject *parent) :
        QCalBaseModel(parent),
        table(new QVector<Data_T>)
    { table->clear(); }

    ~QRXFilterModel()
    { table->clear(); delete table; }

    int rowCount(const QModelIndex &parent) const
    { Q_UNUSED(parent); return table->size(); }

    int columnCount(const QModelIndex &parent) const
    { Q_UNUSED(parent); return 8; }

    QVector<Data_T> *calTable() const
    { return table; }

    QVariant data(const QModelIndex &index, int role) const
    {
        if (role == Qt::DisplayRole) {
            if (index.column() == 0) {
                return QString("%1").arg(table->at(index.row()).freq / 1000000);
            } else if (index.column() == 1) {
                return Coef2QString(table->at(index.row()).coef_real,Order);
            } else if (index.column() == 2) {
                return Coef2QString(table->at(index.row()).coef_imag,Order);
            } else if (index.column() == 3) {
                return QString("%1").arg(table->at(index.row()).temp[0]);
            } else if (index.column() == 4) {
                return QString("%1").arg(table->at(index.row()).temp[1]);
            } else if (index.column() == 5) {
                return QString("%1").arg(table->at(index.row()).temp[2]);
            } else if (index.column() == 6) {
                return QString("%1").arg(table->at(index.row()).temp[3]);
            } else if (index.column() == 7) {
                return tm2QString(table->at(index.row()).time);
            }
        } else if (Qt::BackgroundColorRole == role) {
            return CLR_PROTECT_EYE;
        }
        return QVariant();
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
            QString strHeader[8] = {
                tr("Freq(MHz)"),
                tr("Real"),
                tr("Imag"),
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
    QVector<Data_T> *table;
};

typedef QRXFilterModel<rx_filter_80m_table::data_f_t, ns_sp2401::ul_filter_tap>      QRXFilterModel_80M;
typedef QRXFilterModel<rx_filter_160m_table::data_f_t,ns_sp2401::ul_filter_tap_2i3d> QRXFilterModel_160M;

#endif // Q_RX_FILTER_MODEL_H
