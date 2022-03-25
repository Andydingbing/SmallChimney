#include "cal_tx_vga.h"

using namespace ns_ericsson;
using namespace ns_radio_6449;

void Q_Cal_TX_VGA_Thread::kase()
{
    KASE_THREAD_START("TX VGA",(1));

    uint64_t freq = FREQ_K(1842500);
    double refPwr = 0.0;
    double pwr = 0.0;
    double attCur = 0.0;
    double dyRange = 0.0;
    uint32_t dac = 0;
    uint32_t idx = 0;

    tx_vga_table_t::data_f_t data;
    data.freq = freq;

    Instr.init();
    Instr.sa_reset();
    Instr.sa_set_cf(freq);
    Instr.sa_set_mech_att(false,30);
    Instr.sa_set_ref(50.0);
    Instr.sa_set_rbw(FREQ_K(30),false);
    Instr.sa_set_vbw(FREQ_K(300),false);

    Radio.init();
    Radio.ccctrl(RFIdx,false);
    Radio.biasctrl(RFIdx,false);
    Radio.ccctrl(RFIdx,true);
    Radio.mpa(RFIdx,true);
    Radio.txoff(RFIdx);
    Radio.pabias(RFIdx);

    Radio.serial_write((QString("dpagain1 -b %1 0").arg(char('a' + RFIdx))).toStdString());
    Radio.serial_write((QString("dpagain3 -b %1 0").arg(char('a' + RFIdx))).toStdString());
    Radio.serial_write((QString("mpagain1 -b %1 0").arg(char('a' + RFIdx))).toStdString());
    Radio.serial_write((QString("mpagain2 -b %1 0").arg(char('a' + RFIdx))).toStdString());

    Radio.txstepattmain(RFIdx,12);
    Radio.iqcomp3(RFIdx);
    Radio.pacm(RFIdx);
    Radio.set_tx_frequency(RFIdx,freq);
    Radio.txattmain(RFIdx,dac);
    Radio.iqcomp3(RFIdx);
    Radio.intdldcw(RFIdx,dyRange,true);

    Instr.sa_sweep_once();
    Instr.sa_set_peak_search(Peak_Highest);
    Instr.sa_get_marker_pwr(pwr);
    refPwr = pwr;

    for (dac = 1;dac <= 4000;dac += 100,++idx) {
        Radio.txattmain(RFIdx,dac);
        Radio.iqcomp3(RFIdx);
        Radio.intdldcw(RFIdx,-dyRange,true);

        Instr.sa_sweep_once();
        Instr.sa_set_peak_search(Peak_Highest);
        Instr.sa_get_marker_pwr(pwr);

        attCur = pwr - refPwr;
        dyRange += attCur;

        data.dac[idx] = dac;
        data.att[idx] = float(dyRange);

        Radio.data_base_add(RFIdx,TX_VGA,&data);

        emit uiInsert(idx,idx,TX_VGA);
        emit uiUpdate(idx,idx,TX_VGA);
    }
}

void Q_Cal_TX_VGA_Widget::init()
{
    registerModelView(&model,ui->tableView);
    config->textEditFreq->setText("1842.5M");
    result->lineEditDyRange->setText("");

    initPlot(ui->plot,0,4000,-5,35.0);
    ui->plot->setTitle("DAC/Range");

    curve.curve.setPen(Qt::green,3);
    curve.curve.attach(ui->plot);

    Q_Thread_Widget::init();
}

void Q_Cal_TX_VGA_Widget::prepare(const bool is_exp)
{
    Radio.prepare_kase(currentRFIdx(),TX_VGA,config->textEditFreq->toPlainText().toStdString(),is_exp);

    ui->plot->replot();
}

void Q_Cal_TX_VGA_Widget::uiUpdate(const int first,const int last,const int kase)
{
    Q_UNUSED(first);

//    result->lineEditDyRange->setText(QString("%1").arg(model.table->at(0).dynamic_range()));
    ui->tableView->selectRow(last);
    ui->plot->replot();
}
