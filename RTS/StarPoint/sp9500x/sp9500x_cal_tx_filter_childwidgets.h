#ifndef SP9500X_CAL_TX_FILTER_CHILDWIDGETS_H
#define SP9500X_CAL_TX_FILTER_CHILDWIDGETS_H

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

CONFIG_MODEL(Q_TXFilter_Config_Model,
             "TX-0",
             "TX-1",
             "RebuildCoef",
             "RF&IF(0~3G)",
             "RF(0~3G)",
             "RF(3G~4.8G)",
             "RF(4.8G~6G)",
             "RF(6G~7.5G)",
             "IF(3G~7.5G)")

class Q_TXFilter_Config_Delegate : public Q_Config_Table_Delegate
{
public:
    Q_TXFilter_Config_Delegate(QObject *parent = nullptr) :
        Q_Config_Table_Delegate(parent)
    {
        checkBoxTX0 = new QCheckBox;
        checkBoxTX1 = new QCheckBox;
        checkBoxRebuildCoef = new QCheckBox;
        checkBoxCal_RF_IF_FR_0000_3000 = new QCheckBox;
        checkBoxCal_RF_IF_FR_0000_3000->setChecked(true);

        checkBoxCal_RF_FR_0000_3000 = new QCheckBox;
        checkBoxCal_RF_FR_0000_3000->setChecked(true);

        checkBoxCal_RF_FR_3000_4800 = new QCheckBox;
        checkBoxCal_RF_FR_3000_4800->setChecked(true);

        checkBoxCal_RF_FR_4800_6000 = new QCheckBox;
        checkBoxCal_RF_FR_4800_6000->setChecked(true);

        checkBoxCal_RF_FR_6000_7500 = new QCheckBox;
        checkBoxCal_RF_FR_6000_7500->setChecked(true);

        checkBoxCal_IF_FR_3000_7500 = new QCheckBox;
        checkBoxCal_IF_FR_3000_7500->setChecked(true);
    }

public:
    FIRST_CONFIG_WIDGET(QCheckBox,checkBoxTX0)
    QCheckBox *checkBoxTX1;
    QCheckBox *checkBoxRebuildCoef;
    QCheckBox *checkBoxCal_RF_IF_FR_0000_3000;
    QCheckBox *checkBoxCal_RF_FR_0000_3000;
    QCheckBox *checkBoxCal_RF_FR_3000_4800;
    QCheckBox *checkBoxCal_RF_FR_4800_6000;
    QCheckBox *checkBoxCal_RF_FR_6000_7500;
    QCheckBox *checkBoxCal_IF_FR_3000_7500;
};


class Q_TXFilter_Model : public QCalBaseModel
{
public:
    Q_TXFilter_Model(QObject *parent = nullptr) :
        QCalBaseModel(parent) { table = nullptr; }

    int rowCount(const QModelIndex &/*parent*/) const
    { return table == nullptr ? 0 : int(table->size()); }

    int columnCount(const QModelIndex &/*parent*/) const
    { return 8; }

    QVariant data(const QModelIndex &index, int role) const
    {
        if (Qt::DisplayRole == role) {
            if (index.column() == 0) {
                return QString("%1").arg(table->at(index.row()).freq / 1000000);
            } else if (index.column() == 1) {
                return string_of<QString>(table->at(index.row()).coef_real,ns_sp2406::dl_cf_tap,",");
            } else if (index.column() == 2) {
                return string_of<QString>(table->at(index.row()).coef_imag,ns_sp2406::dl_cf_tap,",");
            } else if (index.column() == 3) {
                return QString("%1").arg(table->at(index.row()).temp[0]);
            } else if (index.column() == 4) {
                return QString("%1").arg(table->at(index.row()).temp[1]);
            } else if (index.column() == 5) {
                return QString("%1").arg(table->at(index.row()).temp[2]);
            } else if (index.column() == 6) {
                return QString("%1").arg(table->at(index.row()).temp[3]);
            } else if (index.column() == 7) {
                return string_of<QString>(table->at(index.row()).time);
            }
        } else if (Qt::BackgroundColorRole == role) {
            return CLR_PROTECT_EYE;
        }

        return QVariant();
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (Qt::DisplayRole == role && Qt::Horizontal == orientation) {
            QString header[8] = { "Freq(MHz)","Real","Imag","Temp4","Temp5","Temp6","Temp7","Time" };
            return header[section];
        }
        return QVariant();
    }

public:
    std::vector<ns_sp9500x::tx_filter_table_t::data_f_t> *table;
};


class Cal_TXFilter_ChildWidgets
{
public:
    Cal_TXFilter_ChildWidgets(QWidget *parent = nullptr)
    {
        _parent = parent;

        QWidget *plotWidget = new QWidget;

        plotRF = new Q_RDDT_CalPlot(plotWidget);
        plotRF->init(ns_sp1403::tx_freq_star/1e6,ns_sp1403::tx_freq_sec3/1e6,-70.0,0.0);
        plotRF->setTitle("RF Freq Response(dBm/MHz)");
        plotRF->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

        plotIF = new Q_RDDT_CalPlot(plotWidget);
        plotIF->init(-491.52,491.52,-70.0,4.0);
        plotIF->setTitle("IF Freq Response(@RF 2GHz)(dBm/MHz)");
        plotIF->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

        QVBoxLayout *plotLayout = new QVBoxLayout(plotWidget);
        plotLayout->addWidget(plotRF);
        plotLayout->addWidget(plotIF);
        plotLayout->setStretch(0,1);
        plotLayout->setStretch(1,1);

        dataRF_FR_0000_3000 = new Qwt_FR_CW_Data<data_f_fr_tx_rf_0000_3000_t>;
        dataRF_FR_3000_4800 = new Qwt_FR_CW_Data<data_f_fr_tx_rf_3000_4800_t>;
        dataRF_FR_4800_6000 = new Qwt_FR_CW_Data<data_f_fr_tx_rf_4800_6000_t>;
        dataRF_FR_6000_7500 = new Qwt_FR_CW_Data<data_f_fr_tx_rf_6000_7500_t>;
        dataIF_FR_3000_7500 = new Qwt_TX_IF_FR_0000_7500_Data;

        curveRF_FR_0000_3000 = new QwtPlotCurve("RF Freq Response 0000M~3000M");
        curveRF_FR_0000_3000->setPen(QColor(Qt::red));
        curveRF_FR_0000_3000->setVisible(true);
        curveRF_FR_0000_3000->attach(plotRF);
        curveRF_FR_0000_3000->setSamples(dataRF_FR_0000_3000);

        curveRF_FR_3000_4800 = new QwtPlotCurve("RF Freq Response 3000M~4800M");
        curveRF_FR_3000_4800->setPen(QColor(Qt::red));
        curveRF_FR_3000_4800->setVisible(true);
        curveRF_FR_3000_4800->attach(plotRF);
        curveRF_FR_3000_4800->setSamples(dataRF_FR_3000_4800);

        curveRF_FR_4800_6000 = new QwtPlotCurve("RF Freq Response 4800M~6000M");
        curveRF_FR_4800_6000->setPen(QColor(Qt::green));
        curveRF_FR_4800_6000->setVisible(true);
        curveRF_FR_4800_6000->attach(plotRF);
        curveRF_FR_4800_6000->setSamples(dataRF_FR_4800_6000);

        curveRF_FR_6000_7500 = new QwtPlotCurve("RF Freq Response 6000M~7500M");
        curveRF_FR_6000_7500->setPen(QColor(Qt::blue));
        curveRF_FR_6000_7500->setVisible(true);
        curveRF_FR_6000_7500->attach(plotRF);
        curveRF_FR_6000_7500->setSamples(dataRF_FR_6000_7500);

        curveIF_FR_3000_7500 = new QwtPlotCurve("IF Freq Response 3000M~7500M");
        curveIF_FR_3000_7500->setVisible(true);
        curveIF_FR_3000_7500->attach(plotIF);
        curveIF_FR_3000_7500->setSamples(dataIF_FR_3000_7500);

        Q_Cal_TXFilter_Widget *p = dynamic_cast<Q_Cal_TXFilter_Widget *>(_parent);

        set_helper::range_freq<uint64_t> freqs;
        set_helper::parse(p->ui->lineEditRFFreqs->text().toStdString(),freqs);

        for (uint32_t i = 0;i < freqs.pts_before(ns_sp1403::tx_freq_sec0);++i) {
            dataRF_IF_FR_0000_3000.append(new Qwt_TX_RF_IF_FR_0000_3000_Data());
            curveRF_IF_FR_0000_3000.append(new QwtPlotCurve());
            curveRF_IF_FR_0000_3000[i]->setVisible(true);
            curveRF_IF_FR_0000_3000[i]->attach(plotRF);
            curveRF_IF_FR_0000_3000[i]->setSamples(dataRF_IF_FR_0000_3000[i]);
        }


        tableView_100 = new Q_RDDT_TableView(p->ui->tabWidget);
        tableView_200 = new Q_RDDT_TableView(p->ui->tabWidget);
        tableView_400 = new Q_RDDT_TableView(p->ui->tabWidget);
        tableView_800 = new Q_RDDT_TableView(p->ui->tabWidget);

        tableView_100->setModel(model_100 = new Q_TXFilter_Model);
        tableView_200->setModel(model_200 = new Q_TXFilter_Model);
        tableView_400->setModel(model_400 = new Q_TXFilter_Model);
        tableView_800->setModel(model_800 = new Q_TXFilter_Model);

        p->ui->tabWidget->clear();
        p->ui->tabWidget->addTab(plotWidget,QString("Freq Response"));
        p->ui->tabWidget->addTab(tableView_100,QString("Coef_100M"));
        p->ui->tabWidget->addTab(tableView_200,QString("Coef_200M"));
        p->ui->tabWidget->addTab(tableView_400,QString("Coef_400M"));
        p->ui->tabWidget->addTab(tableView_800,QString("Coef_800M"));

        configModel = new Q_TXFilter_Config_Model;
        configDelegate = new Q_TXFilter_Config_Delegate;

        p->ui->tableViewConfig->setItemDelegate(configDelegate);
        p->ui->tableViewConfig->setModel(configModel);
    }

    void prepare(const bool exp)
    {
        Q_Cal_TXFilter_Widget *p = dynamic_cast<Q_Cal_TXFilter_Widget *>(_parent);
        set_helper::range_freq<uint64_t> freqs;

        quint32 pts = set_helper::parse(p->ui->lineEditRFFreqs->text().toStdString(),freqs);

        if (configDelegate->checkBoxCal_RF_IF_FR_0000_3000->isChecked()) {
            SP1403->prepare_cal(cal_table_t::TX_RF_IF_FR_0000_3000,freqs.freq,exp);

            for (int i = 0;i < dataRF_IF_FR_0000_3000.size();++i) {
                dataRF_IF_FR_0000_3000[i]->table = &(SP1403->cal_file()->tx_rf_if_fr_0000_3000_table()->data_calibrating()->at(i));
            }
        }

        if (configDelegate->checkBoxCal_RF_FR_0000_3000->isChecked()) {
            SP1403->prepare_cal(cal_table_t::TX_RF_FR_0000_3000,freqs.freq,exp);
            dataRF_FR_0000_3000->table = SP1403->cal_file()->tx_rf_fr_0000_3000()->data_calibrating();
        }

        if (configDelegate->checkBoxCal_RF_FR_3000_4800->isChecked()) {
            SP1403->prepare_cal(cal_table_t::TX_RF_FR_3000_4800,freqs.freq,exp);
            dataRF_FR_3000_4800->table = SP1403->cal_file()->tx_rf_fr_3000_4800()->data_calibrating();
        }

        if (configDelegate->checkBoxCal_RF_FR_4800_6000->isChecked()) {
            SP1403->prepare_cal(cal_table_t::TX_RF_FR_4800_6000,freqs.freq,exp);
            dataRF_FR_4800_6000->table = SP1403->cal_file()->tx_rf_fr_4800_6000()->data_calibrating();
        }

        if (configDelegate->checkBoxCal_RF_FR_6000_7500->isChecked()) {
            SP1403->prepare_cal(cal_table_t::TX_RF_FR_6000_7500,freqs.freq,exp);
            dataRF_FR_6000_7500->table = SP1403->cal_file()->tx_rf_fr_6000_7500()->data_calibrating();
        }

        if (configDelegate->checkBoxCal_IF_FR_3000_7500->isChecked()) {
            freqs.freq.clear();
            freqs.freq.insert(0);
            SP1403->prepare_cal(cal_table_t::TX_IF_FR_3000_7500,freqs.freq,exp);
            dataIF_FR_3000_7500->table = &SP1403->cal_file()->tx_if_fr_3000_7500()->data_calibrating()->at(0);
        }
    }

    QWidget *_parent;

    Q_RDDT_CalPlot *plotRF;
    Q_RDDT_CalPlot *plotIF;

    QList<QwtPlotCurve *> curveRF_IF_FR_0000_3000;
    QwtPlotCurve *curveRF_FR_0000_3000;
    QwtPlotCurve *curveRF_FR_3000_4800;
    QwtPlotCurve *curveRF_FR_4800_6000;
    QwtPlotCurve *curveRF_FR_6000_7500;
    QwtPlotCurve *curveIF_FR_3000_7500;

    QList<Qwt_TX_RF_IF_FR_0000_3000_Data *> dataRF_IF_FR_0000_3000;
    Qwt_FR_CW_Data<data_f_fr_tx_rf_0000_3000_t> *dataRF_FR_0000_3000;
    Qwt_FR_CW_Data<data_f_fr_tx_rf_3000_4800_t> *dataRF_FR_3000_4800;
    Qwt_FR_CW_Data<data_f_fr_tx_rf_4800_6000_t> *dataRF_FR_4800_6000;
    Qwt_FR_CW_Data<data_f_fr_tx_rf_6000_7500_t> *dataRF_FR_6000_7500;
    Qwt_TX_IF_FR_0000_7500_Data *dataIF_FR_3000_7500;

    // The filter coef View/Model.
    Q_RDDT_TableView *tableView_100;
    Q_RDDT_TableView *tableView_200;
    Q_RDDT_TableView *tableView_400;
    Q_RDDT_TableView *tableView_800;

    Q_TXFilter_Model *model_100;
    Q_TXFilter_Model *model_200;
    Q_TXFilter_Model *model_400;
    Q_TXFilter_Model *model_800;

    Q_TXFilter_Config_Model *configModel;
    Q_TXFilter_Config_Delegate *configDelegate;
};

} // namespace NS_SP9500X

#endif // SP9500X_CAL_TX_FILTER_CHILDWIDGETS_H
