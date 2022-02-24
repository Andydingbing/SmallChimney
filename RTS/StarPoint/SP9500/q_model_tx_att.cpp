#include "q_model_tx_att.h"
#include <QPointF>

using namespace ns_sp1401;

QVariant QR1CTXAttOPModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        int freqIdx = index.row() / R1C_TX_ATT_OP_POWER_PTS;
        int attIdx  = index.row() % R1C_TX_ATT_OP_POWER_PTS;

        if (index.column() == 0) {
            return QString("%1").arg(table->at(freqIdx).freq / 1000000);
        } else if (index.column() == 7) {
            return QString("%1").arg(table->at(freqIdx).offset[attIdx],0,'f',3);
        } else if (index.column() == 8) {
            return QString("%1").arg(table->at(freqIdx).temp[0]);
        } else if (index.column() == 9) {
            return QString("%1").arg(table->at(freqIdx).temp[1]);
        } else if (index.column() == 10) {
            return QString("%1").arg(table->at(freqIdx).temp[2]);
        } else if (index.column() == 11) {
            return QString("%1").arg(table->at(freqIdx).temp[3]);
        } else if (index.column() == 12) {
            return tm2QString(table->at(freqIdx).time);
        } else {
            int pwr = attIdx < (R1C_TX_ATT_OP_POWER_STAR - r1c::tx_base_pwr_op) ?
                        (R1C_TX_ATT_OP_POWER_STAR - attIdx) :
                        (R1C_TX_ATT_OP_POWER_STAR - attIdx - 1);

            tx_pwr_op_table_r1c::data_m_t data;

            dynamic_cast<sp1401_r1c *>(_sp1401)
                    ->cf()
                    ->m_tx_pwr_op
                    ->get(_sp1401->get_hw_ver(),table->at(freqIdx).freq,double(pwr),&data);

            if (index.column() == 1) {
                return QString("%1").arg(pwr);
            } else if (index.column() == 2) {
                return QString("%1").arg(data.att0 / 2.0);
            } else if (index.column() == 3) {
                return QString("%1").arg(data.att1 / 2.0);
            } else if (index.column() == 4) {
                return QString("%1").arg(data.att2 / 2.0);
            } else if (index.column() == 5) {
                return QString("%1").arg(data.att3 / 2.0);
            } else if (index.column() == 6) {
                return QString("%1").arg(data.d_gain);
            }
        }
    } else if (role == Qt::BackgroundColorRole) {
        return CLR_PROTECT_EYE;
    }
    return QVariant();
}

QVariant QR1CTXAttIOModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        int freqIdx = index.row() / R1C_TX_ATT_IO_POWER_PTS;
        int attIdx  = index.row() % R1C_TX_ATT_IO_POWER_PTS;

        if (index.column() == 0) {
            return QString("%1").arg(table->at(freqIdx).freq / 1000000);
        } else if (index.column() == 7) {
            return QString("%1").arg(table->at(freqIdx).offset[attIdx],0,'f',3);
        } else if (index.column() == 8) {
            return QString("%1").arg(table->at(freqIdx).temp[0]);
        } else if (index.column() == 9) {
            return QString("%1").arg(table->at(freqIdx).temp[1]);
        } else if (index.column() == 10) {
            return QString("%1").arg(table->at(freqIdx).temp[2]);
        } else if (index.column() == 11) {
            return QString("%1").arg(table->at(freqIdx).temp[3]);
        } else if (index.column() == 12) {
            return tm2QString(table->at(freqIdx).time);
        } else {
            int pwr = attIdx < (R1C_TX_ATT_IO_POWER_STAR - r1c::tx_base_pwr_io) ?
                        (R1C_TX_ATT_IO_POWER_STAR - attIdx) :
                        (R1C_TX_ATT_IO_POWER_STAR - attIdx - 1);

            tx_pwr_io_table_r1c::data_m_t data;

            dynamic_cast<sp1401_r1c *>(_sp1401)
                    ->cf()
                    ->m_tx_pwr_io
                    ->get(_sp1401->get_hw_ver(),table->at(freqIdx).freq,double(pwr),&data);

            if (index.column() == 1) {
                return QString("%1").arg(pwr);
            } else if (index.column() == 2) {
                return QString("%1").arg(data.att0 / 2.0);
            } else if (index.column() == 3) {
                return QString("%1").arg(data.att1 / 2.0);
            } else if (index.column() == 4) {
                return QString("%1").arg(data.att2 / 2.0);
            } else if (index.column() == 5) {
                return QString("%1").arg(data.att3 / 2.0);
            } else if (index.column() == 6) {
                return QString("%1").arg(data.d_gain);
            }
        }
    } else if (role == Qt::BackgroundColorRole) {
        return CLR_PROTECT_EYE;
    }
    return QVariant();
}
