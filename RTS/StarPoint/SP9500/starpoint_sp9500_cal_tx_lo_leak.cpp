#include "starpoint_sp9500_cal_tx_lo_leak.h"

using namespace std;
//using namespace ns_starpoint;
using namespace ns_sp9500;

void Q_Cal_TX_LO_Leak_Thread::kase()
{
    KASE_THREAD_START("TX LO Leakage",100);

//    QTXLOLeakModel *model = dynamic_cast<QTXLOLeakModel *>(calParam.model_0);
//    bool useSA = (calParam.methodLOLeak == M_Spectrum);

//    quint64 freq = 2400000000;
//    qint16 dc_i_m = 0;
//    qint16 dc_q_m = 0;

//    tx_lol_table_r1cd::data_f_t data;

//    if (calParam.calX9119 || useSA) {
//        THREAD_CHECK_BOX(QString("TX<===>Spectrum"));
//    }

//    initTXChain();

//    if (calParam.calX9119) {
//        calX9119();
//    }

//    cancelPoint();

//    if (useSA) {
//        calUseSA(dc_i_m,dc_q_m,data.pwr);
//    } else {
//        calUseLoop(dc_i_m,dc_q_m,data.pwr);
//    }

//    THREAD_TEST_CANCEL
//    data.freq = freq;
//    data.dc_i = dc_i_m;
//    data.dc_q = dc_q_m;
//    data.use_sa = useSA;
//    SP1401->get_temp(4,data.temp[0]);
//    SP1401->get_temp(5,data.temp[1]);
//    SP1401->get_temp(6,data.temp[2]);
//    SP1401->get_temp(7,data.temp[3]);
//    data.time = getCurTime();
//    SP1401->cf()->add(cal_file::TX_LOL,&data);
//    SP1401->cf()->w(cal_file::TX_LOL);

//    for (quint32 i = 0;i < tx_freqs_called;i ++) {
//        data.freq = tx_freq_star + i * tx_freq_step_called;
//        model->calTable()->replace(i,data);
//        emit update(model->index(i,0),model->index(i,9));
//    }

    setProgressPos(100);
}

void Q_Cal_TX_LO_Leak_Widget::init()
{
//    registerModelView(&model,ui->tableView);
//    config->textEditFreq->setText("1747.5M");
//    config->lineEditAtt->setText("0:1:16,23");
//    result->checkBoxMonotonic->setChecked(false);
//    result->lineEditDyRange->setText("");

    Q_Thread_Widget::init();
}

void Q_Cal_TX_LO_Leak_Widget::prepare(const bool is_exp)
{
//    Radio.prepare_case(RFIdx,RX_RF_VGA,config->textEditFreq->toPlainText().toStdString(),is_exp);
}

void Q_Cal_TX_LO_Leak_Widget::uiUpdate(const int first,const int last,const int kase)
{
    ignore_unused(last);

//    ui->config_result->ui->ledResult->setColor(model.table->at(0).is_monotonic() ? Qt::green : Qt::red);
//    result->checkBoxMonotonic->setChecked(model.table->at(0).is_monotonic());
//    result->lineEditDyRange->setText(QString("%1").arg(model.table->at(0).dynamic_range()));

    ui->tableView->selectRow(first);
}
