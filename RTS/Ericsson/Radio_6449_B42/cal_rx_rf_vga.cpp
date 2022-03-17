#include "cal_rx_rf_vga.h"

using namespace std;
using namespace ns_ericsson;
using namespace ns_radio_6449;

void Q_Cal_RX_RF_VGA_Thread::kase()
{
    KASE_THREAD_START("RX RF VGA",(32768));

    int i = 0;
    while (1) {
        pausePoint();

        data_f_rx_rf_vga data;
        set_helper::set_helper_t<uint8_t> atts;
        set<uint8_t>::iterator atts_iter;
        size_t idx = 0;

        atts.parse_from("0:1:16,23");
        atts_iter = atts.data.begin();

        for (;atts_iter != atts.data.end();++atts_iter,++idx) {
            data.att[idx] = *atts_iter;
            data.pwr[idx] = rand() * (110.0 - 70.0) / 32768.0 + 70.0;
        }
        data.freq = FREQ_K(1747500);
        Radio.data_base_add(RFIdx,RX_RF_VGA,&data);

        setProgressPos(i % 32768);
        i += 50;

        emit uiUpdate(0,17,RX_RF_VGA);

        LoggerMsg.add(0,"%d",i);
    }

    uint64_t freq = FREQ_K(1747500);

    data_f_rx_rf_vga data;

    Instr.init();
    Instr.sg_reset();
    Instr.sg_set_cw(freq);
    Instr.sg_set_pl(-60.0);
    Instr.sg_set_mod_en(false);
    Instr.sg_set_output_en(true);

    Radio.init();
    Radio.rxrfvga(RFIdx,23);
    Radio.rxtype(RFIdx,LTE,FREQ_M(5));
    Radio.rxrfsw(RFIdx,true,true);
    Radio.rx(RFIdx,freq);
    Radio.rxcpriconf(RFIdx);
    Radio.rxagc(RFIdx,false);
    Radio.rxrfvgaswp(RFIdx,config->lineEditAtt->text().toStdString(),&data);

    emit uiUpdate(0,17,RX_RF_VGA);
}

void Q_Cal_RX_RF_VGA_Widget::init()
{
    registerModelView(&model,ui->tableView);
    config->textEditFreq->setText("1747.5M");
    config->lineEditAtt->setText("0:1:16,23");
    result->checkBoxMonotonic->setChecked(false);
    result->lineEditDyRange->setText("");

    initPlot(ui->plot,0,32,70.0,110.0);
    ui->plot->setTitle("Att bit/Power");

    curve.curve.setPen(Qt::green);
    curve.curve.attach(ui->plot);

    Q_Thread_Widget::init();
}

void Q_Cal_RX_RF_VGA_Widget::prepare(const bool is_exp)
{
    Radio.prepare_kase(currentRFIdx(),RX_RF_VGA,config->textEditFreq->toPlainText().toStdString(),is_exp);
    ui->plot->replot();
}

void Q_Cal_RX_RF_VGA_Widget::uiUpdate(const int first,const int last,const int kase)
{
    ignore_unused(last);

//    ui->config_result->ui->ledResult->setColor(model.table->at(0).is_monotonic() ? Qt::green : Qt::red);
//    result->checkBoxMonotonic->setChecked(model.table->at(0).is_monotonic());
//    result->lineEditDyRange->setText(QString("%1").arg(model.table->at(0).dynamic_range()));

    ui->tableView->selectRow(first);
    ui->plot->replot();
}
