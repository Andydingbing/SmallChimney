#include "q_model_rx_filter_offset.h"
#include "algo_math.hpp"

using namespace rd::ns_sp9500;

QPointF QwtRXFilterOffsetData::sample(size_t i) const
{
    double freq = (*(iter + i)).freq / 1000000.0;
    double offset = ad_to_dBc(g_0dBFS - (*(iter + i)).offset[offsetIdx],g_0dBFS);
    return QPointF(freq,offset);
}

QVariant QRXFilterOffsetModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        if (index.column() == 0) {
            return QString("%1").arg(table->at(index.row()).freq / 1000000);
        } else if (index.column() == 1) {
            return QString("%1").arg(table->at(index.row()).offset[0]);
        } else if (index.column() == 2) {
            return QString("%1").arg(table->at(index.row()).offset[1]);
        } else if (index.column() == 3) {
            return QString("%1").arg(table->at(index.row()).offset[2]);
        } else if (index.column() == 4) {
            return QString("%1").arg(table->at(index.row()).temp[0]);
        } else if (index.column() == 5) {
            return QString("%1").arg(table->at(index.row()).temp[1]);
        } else if (index.column() == 6) {
            return QString("%1").arg(table->at(index.row()).temp[2]);
        } else if (index.column() == 7) {
            return QString("%1").arg(table->at(index.row()).temp[3]);
        } else if (index.column() == 8) {
            return tm2QString(table->at(index.row()).time);
        }
    } else if (role == Qt::BackgroundColorRole) {
        return CLR_PROTECT_EYE;
    }
    return QVariant();
}

QVariant QRXFilterOffsetModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        QString strHeader[9] = {
            tr("Freq(MHz)"),
            tr("Offset 0"),
            tr("Offset 1"),
            tr("Offset 2"),
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
