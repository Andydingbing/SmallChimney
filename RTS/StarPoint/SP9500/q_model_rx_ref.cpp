#include "q_model_rx_ref.h"
#include <QPointF>
#include "algo_math.hpp"

using namespace rd::ns_sp9500;

QPointF QwtRXRefData::sample(size_t i) const
{
    double freq = (*(iter + i)).freq / 1000000.0;
    double offset = ad_to_dBc(g_0dBFS,g_0dBFS + (*(iter + i)).state[refIdx].ad_offset);
    return QPointF(freq,offset);
}


QR1CRXRefModel::QR1CRXRefModel(QObject *parent) :
    QCalBaseModel(parent)
{
    table = new QVector<rx_ref_table_r1cd::data_f_t>;
    table->clear();
    iter = new QVector<QwtRXRefData *>[3];
    iter->clear();
}

int QR1CRXRefModel::rowCount(const QModelIndex &parent) const
{
    return table->size() * 3;
}

int QR1CRXRefModel::columnCount(const QModelIndex &parent) const
{
    return 13;
}

QVariant QR1CRXRefModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (Qt::DisplayRole == role && Qt::Horizontal == orientation) {
        QString strHeader[13] = {
            tr("Freq(MHz)"),
            tr("Ref(dBm)"),
            tr("LNA/Att"),
            tr("Att0/10"),
            tr("Att1"),
            tr("Att2"),
            tr("Att3"),
            tr("Offset"),
            tr("Temp0_RxLO2"),
            tr("Temp1_RxLNA"),
            tr("Temp2_RxLO1"),
            tr("Temp3_RxPA1"),
            tr("End Time")
        };
        return strHeader[section];
    }
    return QVariant();
}

QVariant QR1CRXRefModel::data(const QModelIndex &index, int role) const
{
    if (Qt::DisplayRole == role) {
        int freqIdx = index.row() / 3;
        int refIdx  = index.row() % 3;

        if (0 == index.column())
            return QString("%1").arg(table->at(freqIdx).freq / 1000000);
        else if (1 == index.column()) {
            if (0 == refIdx)
                return QString("10");
            else if (1 == refIdx)
                return QString("10");
            else
                return QString("-20");
        }
        else if (2 == index.column())
            return QString("%1").arg(table->at(freqIdx).state[refIdx].lna_att ? "LNA" : "Att");
        else if (3 == index.column())
            return QString("%1").arg(table->at(freqIdx).state[refIdx].att_019 ? "Att 0" : "Att 19");
        else if (4 == index.column())
            return QString("%1").arg(table->at(freqIdx).state[refIdx].att1);
        else if (5 == index.column())
            return QString("%1").arg(table->at(freqIdx).state[refIdx].att2);
        else if (6 == index.column())
            return QString("%1").arg(table->at(freqIdx).state[refIdx].att3);
        else if (7 == index.column())
            return QString("%1").arg(table->at(freqIdx).state[refIdx].ad_offset);
        else if (8 == index.column())
            return QString("%1").arg(table->at(freqIdx).state[refIdx].temp[0]);
        else if (9 == index.column())
            return QString("%1").arg(table->at(freqIdx).state[refIdx].temp[1]);
        else if (10 == index.column())
            return QString("%1").arg(table->at(freqIdx).state[refIdx].temp[2]);
        else if (11 == index.column())
            return QString("%1").arg(table->at(freqIdx).state[refIdx].temp[3]);
        else if (12 == index.column())
            return tm2QString(table->at(freqIdx).time);
    }
    else if (Qt::BackgroundColorRole == role)
        return CLR_PROTECT_EYE;
    return QVariant();
}

QVector<rx_ref_table_r1cd::data_f_t> *QR1CRXRefModel::calTable()
{
    return table;
}

QVector<QwtRXRefData *> *QR1CRXRefModel::iterTable(int idx)
{
    return &iter[idx];
}

void QR1CRXRefModel::resetData()
{
    QVector<QwtRXRefData *>::const_iterator ptrIter;
    for (qint8 i = 0;i < 3;i ++) {
        for (ptrIter = iter[i].constBegin();ptrIter != iter[i].constEnd();ptrIter ++)
            delete *ptrIter;
        iter[i].clear();
    }
    table->clear();
}
