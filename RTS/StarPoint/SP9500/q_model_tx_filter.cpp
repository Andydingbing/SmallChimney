#include "q_model_tx_filter.h"

using namespace rd::ns_sp2401;

QVariant QTXFilterModel::data(const QModelIndex &index, int role) const
{
    if (Qt::DisplayRole == role) {
        if (index.column() == 0) {
            return QString("%1").arg(table->at(index.row()).freq / 1000000);
        } else if (index.column() == 1) {
            return Coef2QString(table->at(index.row()).coef_real,dl_filter_tap_2i);
        } else if (index.column() == 2) {
            return Coef2QString(table->at(index.row()).coef_imag,dl_filter_tap_2i);
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
    }
    else if (Qt::BackgroundColorRole == role) {
        return CLR_PROTECT_EYE;
    }

    return QVariant();
}

QVariant QTXFilterModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (Qt::DisplayRole == role && Qt::Horizontal == orientation) {
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
