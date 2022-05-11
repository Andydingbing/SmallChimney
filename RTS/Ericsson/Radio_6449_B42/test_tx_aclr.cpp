#include "test_tx_aclr.h"

using namespace std;

void Q_Test_TX_ACLR_Widget::Thread::kase()
{
    KASE_THREAD_START("TX ACLR",(1));

    uint64_t freq = FREQ_K(1842500);

    tx_aclr_table_t::data_f_t data;
    data.freq = freq;

    Instr.init();
    Instr.sa_reset();
    Instr.sa_set_mode(SA_Mode_SA);
    Instr.sa_set_meas(SA_Meas_ACP);
    Instr.sa_set_cf(freq);
    Instr.sa_set_mech_att(false,30);
    Instr.sa_set_ref(50.0);
    Instr.sa_set_radio_standard(SA_Radio_Standard_LTEB5M);

    vector<uint64_t> freq_offset;
    vector<double> acp_lower;
    vector<double> acp_upper;

    freq_offset.push_back(FREQ_M(5));
    freq_offset.push_back(FREQ_M(10));
    freq_offset.push_back(FREQ_M(0));
    freq_offset.push_back(FREQ_M(0));
    freq_offset.push_back(FREQ_M(0));
    freq_offset.push_back(FREQ_M(0));

    Instr.sa_acp_set_offset_freq(freq_offset);
    Instr.sa_acp_set_offset_freq_stat(0,true);
    Instr.sa_acp_set_offset_freq_stat(1,true);

    Radio.init();
    Radio.ccctrl(RFIdx,false);
    Radio.biasctrl(RFIdx,false);
    Radio.ccctrl(RFIdx,true);
    Radio.pabias(RFIdx);
    Radio.mpa(RFIdx,true);
    Radio.txtype(RFIdx,LTE,FREQ_M(5));

    Radio.serial_write("tx -b a 1842500 -30 460 1");
    Radio.pabias();

    Instr.sa_sweep_once();
    Instr.sa_acp_get_result(&acp_lower,&acp_upper);

    data.acp[0] = acp_lower.at(1);
    data.acp[1] = acp_lower.at(0);
    data.acp[2] = acp_upper.at(0);
    data.acp[3] = acp_upper.at(1);

    Radio.data_base_add(RFIdx,TX_ACLR,&data);

    emit uiUpdate(0,0,TX_ACLR);
}

void Q_Test_TX_ACLR_Widget::init()
{
    registerModelView(&model,ui->tableView);
    Config("Freq",QTextEdit)->setText("1842.5M");

    Q_Thread_Widget::init();
}

void Q_Test_TX_ACLR_Widget::prepare(const bool is_exp)
{
    Radio.prepare_kase(currentRFIdx(),TX_ACLR,Config("Freq",QTextEdit)->toPlainText().toStdString(),is_exp);
}

void Q_Test_TX_ACLR_Widget::uiUpdate(const int first,const int last,const int kase)
{
    Q_UNUSED(last);

    ui->tableView->selectRow(first);
}
