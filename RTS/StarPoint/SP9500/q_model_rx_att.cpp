#include "q_model_rx_att.h"
#include <QPointF>
#include "algo_math.hpp"

using namespace ns_sp9500;
using namespace ns_sp1401;

// QwtRxAttOPData
QwtRxAttOPData::QwtRxAttOPData() :
    QwtCalData()
{
    n = 0;
}

QwtRxAttOPData::QwtRxAttOPData(int idx) :
    QwtCalData()
{
    attIdx = idx;
    n = 0;
}

size_t QwtRxAttOPData::size() const
{
    return n;
}

QPointF QwtRxAttOPData::sample(size_t i) const
{
    double freq = (*(iter + i)).freq / 1000000.0;
    double offset = ad_to_dBc(g_0dBFS,g_0dBFS - (*(iter + i)).offset[attIdx]);
    return QPointF(freq,offset);
}

void QwtRxAttOPData::locate2CalTable(start_iter iter)
{
    this->iter = iter;
}

void QwtRxAttOPData::addOneData()
{
    n += 1;
}

// QwtRxAttIOData
QwtRxAttIOData::QwtRxAttIOData() :
    QwtCalData()
{
    n = 0;
}

QwtRxAttIOData::QwtRxAttIOData(int idx) :
    QwtCalData()
{
    attIdx = idx;
    n = 0;
}

size_t QwtRxAttIOData::size() const
{
    return n;
}

QPointF QwtRxAttIOData::sample(size_t i) const
{
    double freq = (*(iter + i)).freq / 1000000.0;
    double offset = ad_to_dBc(g_0dBFS,g_0dBFS - (*(iter + i)).offset[attIdx]);
    return QPointF(freq,offset);
}

void QwtRxAttIOData::locate2CalTable(start_iter iter)
{
    this->iter = iter;
}

void QwtRxAttIOData::addOneData()
{
    n += 1;
}

// QR1CRxAttOPModel
QR1CRxAttOPModel::QR1CRxAttOPModel(QObject *parent) :
    QCalBaseModel(parent)
{
    init();
}

QR1CRxAttOPModel::QR1CRxAttOPModel(sp1401 *sp1401, QObject *parent)
    : QCalBaseModel(parent)
    , _sp1401(sp1401)
{
    init ();
}

void QR1CRxAttOPModel::init()
{
    table = new QVector<rx_att_op_table_r1cd::data_f_t>;
    table->clear();
    iter = new QVector<QwtRxAttOPData *>[R1C_RX_REF_OP_PTS - 3 + 1];
    for (quint32 i = 0;i < (R1C_RX_REF_OP_PTS - 3 + 1);i ++) {
        iter[i].clear();
    }
}

int QR1CRxAttOPModel::rowCount(const QModelIndex &parent) const
{
    return table->size() * (R1C_RX_REF_OP_PTS - 3 + 1);
}

int QR1CRxAttOPModel::columnCount(const QModelIndex &parent) const
{
    return 14;
}

QVariant QR1CRxAttOPModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (Qt::DisplayRole == role && Qt::Horizontal == orientation) {
        QString strHeader[14] = {
            tr("Freq(MHz)"),
            tr("Ref(dBm)"),
            tr("LNA/Att"),
            tr("Att0/19"),
            tr("Att1"),
            tr("Att2"),
            tr("Att3"),
            tr("DGain"),
            tr("Offset"),
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

QVariant QR1CRxAttOPModel::data(const QModelIndex &index, int role) const
{
    if (Qt::DisplayRole == role) {
        int freqIdx = index.row() / (R1C_RX_REF_OP_PTS - 3 + 1);
        int attIdx  = index.row() % (R1C_RX_REF_OP_PTS - 3 + 1);

        if (0 == index.column())
            return QString("%1").arg(table->at(freqIdx).freq / 1000000);
        else if (8 == index.column())
            return QString("%1").arg(table->at(freqIdx).offset[attIdx]);
        else if (9 == index.column())
            return QString("%1").arg(table->at(freqIdx).temp[0]);
        else if (10 == index.column())
            return QString("%1").arg(table->at(freqIdx).temp[1]);
        else if (11 == index.column())
            return QString("%1").arg(table->at(freqIdx).temp[2]);
        else if (12 == index.column())
            return QString("%1").arg(table->at(freqIdx).temp[3]);
        else if (13 == index.column())
            return tm2QString(table->at(freqIdx).time);
        else {
            qint32 ref = 0;
            rx_ref_op_table_r1cd::rx_state_m_t dataRefOP;

            if (attIdx < 20)
                ref = 30 - attIdx;
            else if (attIdx < 49)
                ref = 29 - attIdx;
            else
                ref = 28 - attIdx;

            DYNAMIC_SP1401_R1CE_SP1401(_sp1401)
                    ->cf()
                    ->m_rx_ref_op
                    ->get(_sp1401->get_hw_ver(),table->at(freqIdx).freq,double(ref),&dataRefOP);

            if (1 == index.column())
                return QString("%1").arg(ref);
            else if (2 == index.column())
                return QString("%1").arg(dataRefOP.lna_att ? "LNA" : "Att");
            else if (3 == index.column())
                return QString("%1").arg(dataRefOP.att_019 ? "Att 0" : "Att 19");
            else if (4 == index.column())
                return QString("%1").arg(dataRefOP.att1);
            else if (5 == index.column())
                return QString("%1").arg(dataRefOP.att2);
            else if (6 == index.column())
                return QString("%1").arg(dataRefOP.att3);
            else if (7 == index.column())
                return QString("%1").arg(dataRefOP.ad_offset);
        }
    }
    else if (Qt::BackgroundColorRole == role)
        return CLR_PROTECT_EYE;
    return QVariant();
}

QVector<rx_att_op_table_r1cd::data_f_t> *QR1CRxAttOPModel::calTable()
{
    return table;
}

QVector<QwtRxAttOPData *> *QR1CRxAttOPModel::iterTable(int idx)
{
    return &iter[idx];
}

void QR1CRxAttOPModel::resetData()
{
    QVector<QwtRxAttOPData *>::const_iterator ptrIter;
    for (quint32 i = 0;i < (R1C_RX_REF_OP_PTS - 3 + 1);i ++) {
        for (ptrIter = iter[i].constBegin();ptrIter != iter[i].constEnd();ptrIter ++) {
            delete *ptrIter;
        }
        iter[i].clear();
    }
    table->clear();
}

void QR1CRxAttOPModel::setSP1401(sp1401 *sp1401)
{
    _sp1401 = sp1401;
}

// QR1CRxAttIOModel
QR1CRxAttIOModel::QR1CRxAttIOModel(QObject *parent) :
    QCalBaseModel(parent)
{
    init();
}

QR1CRxAttIOModel::QR1CRxAttIOModel(sp1401 *sp1401, QObject *parent)
    : QCalBaseModel(parent)
    , _sp1401(sp1401)
{
    init();
}

void QR1CRxAttIOModel::init()
{
    table = new QVector<rx_att_io_table_r1cd::data_f_t>;
    table->clear();
    iter = new QVector<QwtRxAttIOData *>[R1C_RX_REF_IO_PTS - 3 + 1];
    for (quint32 i = 0;i < (R1C_RX_REF_IO_PTS - 3 + 1);i ++) {
        iter[i].clear();
    }
}

int QR1CRxAttIOModel::rowCount(const QModelIndex &parent) const
{
    return table->size() * (R1C_RX_REF_IO_PTS - 3 + 1);
}

int QR1CRxAttIOModel::columnCount(const QModelIndex &parent) const
{
    return 14;
}

QVariant QR1CRxAttIOModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (Qt::DisplayRole == role && Qt::Horizontal == orientation) {
        QString strHeader[14] = {
            tr("Freq(MHz)"),
            tr("Ref(dBm)"),
            tr("LNA/Att"),
            tr("Att0/19"),
            tr("Att1"),
            tr("Att2"),
            tr("Att3"),
            tr("DGain"),
            tr("Offset"),
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

QVariant QR1CRxAttIOModel::data(const QModelIndex &index, int role) const
{
    if (Qt::DisplayRole == role) {
        int freqIdx = index.row() / (R1C_RX_REF_IO_PTS - 3 + 1);
        int attIdx  = index.row() % (R1C_RX_REF_IO_PTS - 3 + 1);

        if (0 == index.column())
            return QString("%1").arg(table->at(freqIdx).freq / 1000000);
        else if (8 == index.column())
            return QString("%1").arg(table->at(freqIdx).offset[attIdx]);
        else if (9 == index.column())
            return QString("%1").arg(table->at(freqIdx).temp[0]);
        else if (10 == index.column())
            return QString("%1").arg(table->at(freqIdx).temp[1]);
        else if (11 == index.column())
            return QString("%1").arg(table->at(freqIdx).temp[2]);
        else if (12 == index.column())
            return QString("%1").arg(table->at(freqIdx).temp[3]);
        else if (13 == index.column())
            return tm2QString(table->at(freqIdx).time);
        else {
            qint32 ref = 0;
            rx_ref_io_table_r1cd::rx_state_m_t dataRefIO;

            if (attIdx < 20)
                ref = 30 - attIdx;
            else if (attIdx < 49)
                ref = 29 - attIdx;
            else
                ref = 28 - attIdx;

            DYNAMIC_SP1401_R1CE_SP1401(_sp1401)
                    ->cf()
                    ->m_rx_ref_io
                    ->get(_sp1401->get_hw_ver(),table->at(freqIdx).freq,double(ref),&dataRefIO);

            if (1 == index.column())
                return QString("%1").arg(ref);
            else if (2 == index.column())
                return QString("%1").arg(dataRefIO.lna_att ? "LNA" : "Att");
            else if (3 == index.column())
                return QString("%1").arg(dataRefIO.att_019 ? "Att 0" : "Att 19");
            else if (4 == index.column())
                return QString("%1").arg(dataRefIO.att1 / 2.0);
            else if (5 == index.column())
                return QString("%1").arg(dataRefIO.att2 / 2.0);
            else if (6 == index.column())
                return QString("%1").arg(dataRefIO.att3 / 2.0);
            else if (7 == index.column())
                return QString("%1").arg(dataRefIO.ad_offset);
        }
    }
    else if (Qt::BackgroundColorRole == role)
        return CLR_PROTECT_EYE;
    return QVariant();
}

QVector<rx_att_io_table_r1cd::data_f_t> *QR1CRxAttIOModel::calTable()
{
    return table;
}

QVector<QwtRxAttIOData *> *QR1CRxAttIOModel::iterTable(int idx)
{
    return &iter[idx];
}

void QR1CRxAttIOModel::resetData()
{
    QVector<QwtRxAttIOData *>::const_iterator ptrIter;
    for (quint32 i = 0;i < (R1C_RX_REF_IO_PTS - 3 + 1);i ++) {
        for (ptrIter = iter[i].constBegin();ptrIter != iter[i].constEnd();ptrIter ++) {
            delete *ptrIter;
        }
        iter[i].clear();
    }
    table->clear();
}

void QR1CRxAttIOModel::setSP1401(sp1401 *sp1401)
{
    _sp1401 = sp1401;
}
