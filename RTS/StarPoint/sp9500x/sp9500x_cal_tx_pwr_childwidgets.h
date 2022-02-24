#ifndef SP9500X_CAL_TX_PWR_CHILDWIDGETS_H
#define SP9500X_CAL_TX_PWR_CHILDWIDGETS_H

#include "cal_widget.h"
#include "rddt_plot.hpp"
#include "rddt_tableview.hpp"
#include "q_model_tx_filter.h"
#include "utilities.hpp"
#include <QCheckBox>
#include <QVBoxLayout>

namespace NS_SP9500X {

using namespace ns_sp9500x;

typedef Qwt_FR_BW_Data<200> Qwt_TX_RF_IF_FR_0000_3000_Data;
typedef Qwt_FR_BW_Data<400> Qwt_TX_IF_FR_0000_7500_Data;

CONFIG_MODEL(Q_TX_Pwr_Config_Model,
             "TX-0-Output",
             "TX-0-IO",
             "TX-1")

class Q_TX_Pwr_Config_Delegate : public Q_Config_Table_Delegate
{
public:
    Q_TX_Pwr_Config_Delegate(QObject *parent = nullptr) :
        Q_Config_Table_Delegate(parent)
    {
        checkBoxTX0_Output = new QCheckBox;
        checkBoxTX0_Output->setChecked(true);

        checkBoxTX0_IO = new QCheckBox;
        checkBoxTX0_IO->setChecked(true);

        checkBoxTX1 = new QCheckBox;
        checkBoxTX1->setChecked(true);
    }

public:
    FIRST_CONFIG_WIDGET(QCheckBox,checkBoxTX0_Output)
    QCheckBox *checkBoxTX0_IO;
    QCheckBox *checkBoxTX1;
};


class Q_TX_Pwr_Model : public QCalBaseModel
{
public:
    Q_TX_Pwr_Model(QObject *parent = nullptr) :
        QCalBaseModel(parent) { table = nullptr; }

    int rowCount(const QModelIndex &parent) const
    { ignore_unused(parent); return table == nullptr ? 0 : int(table->size()); }

    int columnCount(const QModelIndex &parent) const
    { ignore_unused(parent); return 12; }

    QVariant data(const QModelIndex &index, int role) const
    {
        size_t row = size_t(index.row());

        if (Qt::DisplayRole == role) {
            if (index.column() == 0) {
                return QString("%1").arg(table->at(row).freq / 1000000);
            } else if (index.column() == 1) {
                return QString("%1").arg(table->at(row).d_gain,0,'f',3);
            } else if (index.column() == 2) {
                return QString("%1").arg(data_f_tx_pwr::att_double(table->at(row).att[0]));
            } else if (index.column() == 3) {
                return QString("%1").arg(data_f_tx_pwr::att_double(table->at(row).att[1]));
            } else if (index.column() == 4) {
                return QString("%1").arg(data_f_tx_pwr::att_double(table->at(row).att[2]));
            } else if (index.column() == 5) {
                return QString("%1").arg(data_f_tx_pwr::att_double(table->at(row).att[3]));
            } else if (index.column() == 6) {
                return QString("%1").arg(table->at(row).temp[0]);
            } else if (index.column() == 7) {
                return QString("%1").arg(table->at(row).temp[1]);
            } else if (index.column() == 8) {
                return QString("%1").arg(table->at(row).temp[2]);
            } else if (index.column() == 9) {
                return QString("%1").arg(table->at(row).temp[3]);
            } else if (index.column() == 10) {
                return QString("%1").arg(table->at(row).temp[4]);
            } else if (index.column() == 11) {
                return string_of<QString>(table->at(row).time);
            }
        } else if (Qt::BackgroundColorRole == role) {
            return CLR_PROTECT_EYE;
        }

        return QVariant();
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (Qt::DisplayRole == role && Qt::Horizontal == orientation) {
            QString header[12] = {
                "Freq(MHz)",
                "D_Gain(dBm)",
                "Att0",
                "Att1",
                "Att2",
                "Att3",
                "Temp0",
                "Temp1",
                "Temp2",
                "Temp3",
                "Temp4",
                "Time" };
            return header[section];
        }
        return QVariant();
    }

public:
    std::vector<ns_sp9500x::tx_pwr_table_t::data_f_t> *table;
};


class Qwt_TX_Pwr_Data : public QwtCalData
{
public:
    Qwt_TX_Pwr_Data() { table = nullptr; }

    size_t size() const
    { return table == nullptr ? 0 : table->size(); }

    QPointF sample(size_t i) const
    { return QPointF(table->at(i).freq / 1e6,table->at(i).d_gain); }

    std::vector<ns_sp9500x::tx_pwr_table_t::data_f_t> *table;
};

class Cal_TX_Pwr_ChildWidgets
{
public:
    Cal_TX_Pwr_ChildWidgets(QWidget *parent = nullptr)
    {
        _parent = parent;

        Q_Cal_TX_Pwr_Widget *p = dynamic_cast<Q_Cal_TX_Pwr_Widget *>(_parent);

        init(p->ui->plot,ns_sp1403::tx_freq_star/1e6,ns_sp1403::tx_freq_sec3/1e6,-1.0,1.0);

        p->ui->plot->setTitle("RF Freq Response(dBm/MHz)");

        curveTX0_Output.setPen(Qt::red);
        curveTX0_Output.setVisible(true);
        curveTX0_Output.attach(p->ui->plot);
        curveTX0_Output.setSamples(&dataTX0_Output);

        curveTX0_IO.setPen(Qt::green);
        curveTX0_IO.setVisible(true);
        curveTX0_IO.attach(p->ui->plot);
        curveTX0_IO.setSamples(&dataTX0_IO);

        curveTX1.setPen(Qt::blue);
        curveTX1.setVisible(true);
        curveTX1.attach(p->ui->plot);
        curveTX1.setSamples(&dataTX1);

        p->ui->tableViewTX0_Output->setModel(&modelTX0_Output);
        p->ui->tableViewTX0_IO->setModel(&modelTX0_IO);
        p->ui->tableViewTX1->setModel(&modelTX1);

        configDelegate = new Q_TX_Pwr_Config_Delegate;
        configModel = new Q_TX_Pwr_Config_Model;

        p->ui->tableViewConfig->setItemDelegate(configDelegate);
        p->ui->tableViewConfig->setModel(configModel);
    }

    void prepare(const bool exp)
    {
        Q_Cal_TX_Pwr_Widget *p = dynamic_cast<Q_Cal_TX_Pwr_Widget *>(_parent);

        set_helper::range_freq<uint64_t> freqs;
        set_helper::parse(p->ui->textEditRFFreqs->toPlainText().toStdString(),freqs);

        if (configDelegate->checkBoxTX0_Output->isChecked()) {
            SP1403->prepare_cal(cal_table_t::TX0_Pwr_Output,freqs.freq,exp);
            dataTX0_Output.table = SP1403->cal_file()->tx0_pwr_output()->data_calibrating();
            modelTX0_Output.table = dataTX0_Output.table;
            p->ui->tableViewTX0_Output->selectRow(0);
        }

        if (configDelegate->checkBoxTX0_IO->isChecked()) {
            SP1403->prepare_cal(cal_table_t::TX0_Pwr_IO,freqs.freq,exp);
            dataTX0_IO.table = SP1403->cal_file()->tx0_pwr_io()->data_calibrating();
            modelTX0_IO.table = dataTX0_IO.table;
            p->ui->tableViewTX0_IO->selectRow(0);
        }

        if (configDelegate->checkBoxTX1->isChecked()) {
            SP1403->prepare_cal(cal_table_t::TX1_Pwr,freqs.freq,exp);
            dataTX1.table = SP1403->cal_file()->tx1_pwr()->data_calibrating();
            modelTX1.table = dataTX1.table;
            p->ui->tableViewTX1->selectRow(0);
        }

    }

    QWidget *_parent;

    QwtPlotCurve curveTX0_Output;
    QwtPlotCurve curveTX0_IO;
    QwtPlotCurve curveTX1;

    Qwt_TX_Pwr_Data dataTX0_Output;
    Qwt_TX_Pwr_Data dataTX0_IO;
    Qwt_TX_Pwr_Data dataTX1;

    Q_TX_Pwr_Model modelTX0_Output;
    Q_TX_Pwr_Model modelTX0_IO;
    Q_TX_Pwr_Model modelTX1;

    Q_TX_Pwr_Config_Model *configModel;
    Q_TX_Pwr_Config_Delegate *configDelegate;
};

} // namespace NS_SP9500X

#endif // SP9500X_CAL_TX_PWR_CHILDWIDGETS_H
