#include "test_rx_gain_accuracy.h"

void Q_Test_RX_Gain_Accuracy_Widget::Thread::kase()
{
    KASE_THREAD_START("RX Gain Accuracy",(1));

    uint64_t freq = FREQ_K(1747500);
    double pwr[4] = {0.0,0.0,0.0,0.0};

    rx_gain_accu_table_t::data_f_t data;
    data.freq = freq;

    Instr.init();
    Instr.sg_reset();
    Instr.sg_set_cw(freq);
    Instr.sg_set_pl(-60.0);
    Instr.sg_set_mod_en(false);
    Instr.sg_set_output_en(true);

    Radio.init();

    for (uint32_t i = 0;i < 4;++i) {
        Radio.ccctrl(i,false);
        Radio.biasctrl(i,false);
        Radio.ccctrl(i,true);
        Radio.txoff(i);
        Radio.rxtype(i,LTE,FREQ_M(5));
        Radio.rxulg(i,0);

        Radio.set_rx_frequency(i,freq);
        Radio.rxcpriconf(i);
    }

    Radio.serial_write("dbautoget fullbandSupport");
    Radio.ulils(&pwr[0],&pwr[1],&pwr[2],&pwr[3]);

    data.pwr[0] = float(pwr[0]);
    data.pwr[1] = float(pwr[1]);
    data.pwr[2] = float(pwr[2]);
    data.pwr[3] = float(pwr[3]);

    Radio.data_base_add(RFIdx,RX_Gain_Accu,&data);
    emit uiUpdate(0,3,RX_Gain_Accu);
}

void Q_Test_RX_Gain_Accuracy_Widget::init()
{
    registerModelView(&model,ui->tableView);

    Config("Freq",QTextEdit)->setText("1747.5M");

    Result("Accuracy",QLineEdit)->setText("");

    Q_Thread_Widget::init();
}

void Q_Test_RX_Gain_Accuracy_Widget::prepare(const bool is_exp)
{
    Radio.prepare_kase(currentRFIdx(),RX_Gain_Accu,Config("Freq",QTextEdit)->toPlainText().toStdString(),is_exp);
}

void Q_Test_RX_Gain_Accuracy_Widget::uiUpdate(const int first,const int last,const int kase)
{
    Q_UNUSED(first);
//    result->lineEditAccu->setText(QString("%1").arg(data.table->at(0).accu()));
    ui->tableView->selectRow(last);
}
