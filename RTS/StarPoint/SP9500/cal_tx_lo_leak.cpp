#include "cal_tx_lo_leak.h"

using namespace std;

void Q_Cal_TX_LO_Leak_Widget::Thread::kase()
{
    KASE_THREAD_START("TX LO Leakage",100);

    quint64 freq = freq_string_to_uint64_t(Config("Freq",QLineEdit)->text().toStdString());
    bool isCalX9119 = Config("10M Ref",QCheckBox)->isChecked();
    bool isUseSA = Config("Method",QComboBox)->currentIndex() == 0;

    qint16 dc_i_m = 0;
    qint16 dc_q_m = 0;

    tx_lol_table_t::data_f_t data;

    if (isCalX9119 || isUseSA) {
        checkPoint(QString("TX<===>Spectrum"));
    }

    initTXChain();

    if (isCalX9119) {
        calX9119();
    }

    cancelPoint();

    if (isUseSA) {
        calUseSA(dc_i_m,dc_q_m,data.pwr);

        data.freq = freq;
        data.dc_i = dc_i_m;
        data.dc_q = dc_q_m;
        data.use_sa_or_loopback = isUseSA;

        SP1401_R1C->get_temp(4,data.temp[0]);
        SP1401_R1C->get_temp(5,data.temp[1]);
        SP1401_R1C->get_temp(6,data.temp[2]);
        SP1401_R1C->get_temp(7,data.temp[3]);

        SP1401->data_base()->add(cal_table_t::TX_LOL,&data);
        SP1401->data_base()->persistence(cal_table_t::TX_LOL);
    } else {
        self_cal_tx_lol_helper caller(SP3301,SP1401->rf_idx());

        if (caller.run()) {
            Log.set_last_err("Cal TX LO Leakage Fail.Power:%f",caller.pwr_lol);
            THREAD_ERROR_BOX(Log.last_err());
            emit threadProcess(Running_Except);
        }
    }

    emit uiUpdate(0,1,cal_table_t::TX_LOL);
    setProgressPos(100);
}

void Q_Cal_TX_LO_Leak_Widget::Thread::initTXChain()
{
    SP1401_R1C->set_tx_att(10.0,10.0,10.0,0.0);
    SP1401->set_io_mode(io_mode_t::OUTPUT);
    SP2401->set_da_sw(sp2401_r1a::FROM_TO_RF);
    SP2401->set_ad_sw(sp2401_r1a::FROM_TO_RF);
    SP2401->set_dds_src(sp2401_r1a::ALL_0);
    SP2401->set_tx_pwr_comp(0.0);
    SP2401->set_tx_dc_offset(0,0);
    SP2401->set_tx_filter_sw(false);
}

void Q_Cal_TX_LO_Leak_Widget::Thread::calX9119()
{
    quint64 freq = tx_freq_stop;
    double mkrFreq = 0.0;
    double mkrPwr  = -100;
    double freqDelta = 0.0;
    double curSpan = 15000.0;
    x9119_table_t::data_f_t data;

    SP1401->set_tx_freq(freq);

    Instr.init();
    Instr.has_sa();
    Instr.sa_reset();
    Instr.sa_set_cf(double(freq));
    Instr.sa_set_span(curSpan);
    Instr.sa_set_rbw(200.0,false);
    Instr.sa_set_vbw(300.0,false);
    Instr.sa_set_ref(-10.0);

    for (quint16 value = 0;value < 1023;value ++) {
        pausePoint();

        SP3501.vol_9119(value);
        Instr.sa_sweep_once();
        Instr.sa_set_peak_search(Peak_Highest);
        Instr.sa_get_marker_freq(mkrFreq);
        Instr.sa_get_marker_pwr(mkrPwr);

        freqDelta = abs(mkrFreq - double(freq));

        if (freqDelta < 5.0 && mkrPwr >= -70.0) {
            data.value = value;
            break;
        } else if (freqDelta < 500.0) {
            if (curSpan != 1000.0) Instr.sa_set_span(curSpan = 1000.0);
        } else if (freqDelta < 2500.0) {
            if (curSpan != 5000.0) Instr.sa_set_span(curSpan = 5000.0);
        } else if (freqDelta < 5000.0) {
            if (curSpan != 10000.0) Instr.sa_set_span(curSpan = 10000.0);
        }
    }

    if (freqDelta > 5.0) {
        Log.set_last_err("Cal OCXO Fail");
        THREAD_ERROR_BOX(Log.last_err());
        emit threadProcess(Running_Except);
    }

    SP1401->data_base()->add(cal_table_t::X9119,&data);
    SP1401->data_base()->persistence(cal_table_t::X9119);
}

void Q_Cal_TX_LO_Leak_Widget::Thread::calUseSA(qint16 &dc_i_m, qint16 &dc_q_m,double &pwrLOL)
{
    Instr.init();
    Instr.has_sa();
    Instr.sa_reset();
    Instr.sa_set_span(10e3);
    Instr.sa_set_ref(0.0);

    uint64_t freq = 2400000000;
    qint16 dc_i_l = -10000;
    qint16 dc_i_r = 10000;
    qint16 dc_q_l = -10000;
    qint16 dc_q_r = 10000;
    qint16 step = 0;
    double pwrSA = 0.0;

    SP1401->set_io_mode(io_mode_t::OUTPUT);
    SP1401->set_tx_freq(freq);
    SP2401->set_tx_dc_offset(dc_i_m,dc_q_m);

    Instr.sa_set_cf(freq);
    Instr.sa_set_ref(0.0);
    Instr.sa_sweep_once();
    Instr.sa_sweep_once();
    Instr.sa_set_peak_search(Peak_Highest);

    step = 200;

    while ((dc_i_r - dc_i_l) > 4) {
        pausePoint();

        dc_i_m = (qint16)round((dc_i_l + dc_i_r) / 2.0,0);

        if (slopeI_SA(dc_i_m,dc_q_m,step,pwrLOL) > 0) {
            dc_i_r = dc_i_m;
        } else {
            dc_i_l = dc_i_m;
        }
    }

    while ((dc_q_r - dc_q_l) > 4) {
        pausePoint();

        dc_q_m = (qint16)round((dc_q_l + dc_q_r) / 2.0,0);

        if (slopeQ_SA(dc_i_m,dc_q_m,step,pwrLOL) > 0) {
            dc_q_r = dc_q_m;
        } else {
            dc_q_l = dc_q_m;
        }
    }

    step = 20;
    dc_i_l = dc_i_m - 100;
    dc_i_r = dc_i_m + 100;

    if (pwrLOL <= -40.0) {
        Instr.sa_set_ref(-40.0);
    }

    while ((dc_i_r - dc_i_l) > 4) {
        pausePoint();

        dc_i_m = (qint16)round((dc_i_l + dc_i_r) / 2.0,0);

        if (slopeI_SA(dc_i_m,dc_q_m,step,pwrLOL) > 0) {
            dc_i_r = dc_i_m;
        } else {
            dc_i_l = dc_i_m;
        }
    }

    dc_q_l = dc_q_m - 100;
    dc_q_r = dc_q_m + 100;

    while ((dc_q_r - dc_q_l) > 4) {
        pausePoint();

        dc_q_m = (qint16)round((dc_q_l + dc_q_r) / 2.0,0);

        if (slopeQ_SA(dc_i_m,dc_q_m,step,pwrLOL) > 0) {
            dc_q_r = dc_q_m;
        } else {
            dc_q_l = dc_q_m;
        }
    }

    if (pwrLOL <= -40.0) {
        Instr.sa_set_ref(-40.0);
    }

    step = 1;

    for (qint16 retry = 0;retry < 3;retry ++) {
        pausePoint();

        if (slopeI_SA(dc_i_m,dc_q_m,1,pwrLOL) > 0) {
            while (--dc_i_m) { if (slopeI_SA(dc_i_m,dc_q_m,step,pwrLOL) < 0) break; }
        } else {
            while (++dc_i_m) { if (slopeI_SA(dc_i_m,dc_q_m,step,pwrLOL) > 0) break; }
        }

        if (slopeQ_SA(dc_i_m,dc_q_m,1,pwrLOL) > 0) {
            while (--dc_q_m) { if (slopeQ_SA(dc_i_m,dc_q_m,step,pwrLOL) < 0) break; }
        } else {
            while (++dc_q_m) { if (slopeQ_SA(dc_i_m,dc_q_m,step,pwrLOL) > 0) break; }
        }
    }

    qint16 DC_I_M_Min = dc_i_m;
    qint16 DC_Q_M_Min = dc_q_m;

    for (int retry = 0;retry < 3;retry ++) {
        dc_i_l = dc_i_m - 5;
        dc_i_r = dc_i_m + 5;

        for (dc_i_m = dc_i_l; dc_i_m <= dc_i_r; dc_i_m ++) {
            pausePoint();

            SP2401->set_tx_dc_offset(dc_i_m,dc_q_m);
            Instr.sa_sweep_once();
            Instr.sa_get_marker_pwr(pwrSA);

            if (pwrSA < pwrLOL) {
                pwrLOL = pwrSA;
                DC_I_M_Min = dc_i_m;
            }
        }

        dc_i_m = DC_I_M_Min;

        for (dc_q_m = dc_q_l; dc_q_m <= dc_q_r; dc_q_m ++) {
            pausePoint();

            SP2401->set_tx_dc_offset(dc_i_m,dc_q_m);
            Instr.sa_sweep_once();
            Instr.sa_get_marker_pwr(pwrSA);

            if (pwrSA < pwrLOL) {
                pwrLOL = pwrSA;
                DC_Q_M_Min = dc_q_m;
            }
        }
        dc_q_m = DC_Q_M_Min;
    }

    if (pwrLOL > spec::cal_tx_lol_up() || pwrLOL < spec::cal_tx_lol_down()) {
        dc_i_m = 0;
        dc_q_m = 0;
        Log.set_last_err("Cal TX LO Leakage Fail.Power:%f",pwrLOL);
        THREAD_ERROR_BOX(Log.last_err());
        emit threadProcess(Running_Except);
    }
}

double Q_Cal_TX_LO_Leak_Widget::Thread::slopeI_SA(qint16 dc_i_m, qint16 dc_q_m, qint16 delta, double &pwr)
{
    double pwrSA[2] = {0.0,0.0};
    SP2401->set_tx_dc_offset(quint16(dc_i_m + delta),quint16(dc_q_m));
    Instr.sa_sweep_once();
    Instr.sa_get_marker_pwr(pwrSA[1]);

    SP2401->set_tx_dc_offset(quint16(dc_i_m),quint16(dc_q_m));
    Instr.sa_sweep_once();
    Instr.sa_get_marker_pwr(pwrSA[0]);
    pwr = pwrSA[0];

    return (pwrSA[1] - pwrSA[0]) / double(delta);
}

double Q_Cal_TX_LO_Leak_Widget::Thread::slopeQ_SA(qint16 dc_i_m, qint16 dc_q_m, qint16 delta, double &pwr)
{
    double pwrSA[2] = {0.0,0.0};
    SP2401->set_tx_dc_offset(quint16(dc_i_m),quint16(dc_q_m + delta));
    Instr.sa_sweep_once();
    Instr.sa_get_marker_pwr(pwrSA[1]);

    SP2401->set_tx_dc_offset(quint16(dc_i_m),quint16(dc_q_m));
    Instr.sa_sweep_once();
    Instr.sa_get_marker_pwr(pwrSA[0]);
    pwr = pwrSA[0];

    return (pwrSA[1] - pwrSA[0]) / double(delta);
}

void Q_Cal_TX_LO_Leak_Widget::init()
{
//    registerModelView(&model,ui->tableView);
    Config("Freq",QLineEdit)->setText("2400M");
    Config("10M Ref",QCheckBox)->setChecked(false);
    Config("Method",QComboBox)->addItems({"Spectrum","Loopback"});

    Q_Thread_Widget::init();
}

void Q_Cal_TX_LO_Leak_Widget::prepare(const bool is_exp)
{
//    Radio.prepare_case(RFIdx,RX_RF_VGA,config->textEditFreq->toPlainText().toStdString(),is_exp);
}

void Q_Cal_TX_LO_Leak_Widget::uiUpdate(const int first,const int last,const int kase)
{
    Q_UNUSED(last);

    ui->tableView->selectRow(first);
}
