#include "q_model_tx_sb.h"

QVariant QTXSBModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        if (index.column() == 0) {
            return QString("%1").arg(table->at(index.row()).freq / 1000000);
        } else if (index.column() == 1) {
            return QString("%1").arg(table->at(index.row()).th);
        } else if (index.column() == 2) {
            return QString("%1").arg(table->at(index.row()).am_i);
        } else if (index.column() == 3) {
            return QString("%1").arg(table->at(index.row()).am_q);
        } else if (index.column() == 4) {
            return table->at(index.row()).use_sa ? tr("Spectrum") : tr("Loopback");
        } else if (index.column() == 5) {
            return QString("%1").arg(table->at(index.row()).pwr);
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

QVariant QTXSBModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        QString strHeader[11] = {
            tr("Freq(MHz)"),
            tr("Angle(°)"),
            tr("Am Offset I"),
            tr("Am Offset Q"),
            tr("Method"),
            tr("Sideband(dBm)"),
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
