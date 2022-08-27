#include "cal_tx_sideband.h"

using namespace std;

void Q_Cal_TX_Sideband_Widget::Thread::kase()
{
    KASE_THREAD_START("TX Sideband",100);

    quint64 freq = freq_string_to_uint64_t(Config("Freq",QLineEdit)->text().toStdString());
    bool isUseSA = Config("Method",QComboBox)->currentIndex() == 0;

    double thM = 0.0;
    quint16 am_i_m = 8192;
    quint16 am_q_m = 8192;
    double pwrSB = 0.0;

    tx_sideband_table_t::data_f_t data;

    if (isUseSA) {
        checkPoint(QString("TX<===>Spectrum"));
    }

    initTXChain();

    if (isUseSA) {
        calUseSA(thM,am_i_m,am_q_m,pwrSB);

        cancelPoint();

        data.th   = thM;
        data.am_i = am_i_m;
        data.am_q = am_q_m;
        data.pwr  = pwrSB;
        data.use_sa_or_loopback = isUseSA;

        SP1401_R1C->get_temp(4,data.temp[0]);
        SP1401_R1C->get_temp(5,data.temp[1]);
        SP1401_R1C->get_temp(6,data.temp[2]);
        SP1401_R1C->get_temp(7,data.temp[3]);

        SP1401->data_base()->add(cal_table_t::TX_SB,&data);
        SP1401->data_base()->persistence(cal_table_t::TX_SB);
    } else {
        self_cal_tx_sb_helper caller(SP3301,SP1401->rf_idx());

        if (caller.run(&data)) {
            Log.set_last_err("Cal TX Sideband Fail.Power:%f",caller.pwr_sb);
            THREAD_ERROR_BOX(Log.last_err());
            emit threadProcess(Running_Except);
        }
    }

    emit uiUpdate(0,1,cal_table_t::TX_SB);

    setProgressPos(100);
}

void Q_Cal_TX_Sideband_Widget::Thread::initTXChain()
{
    SP1401_R1C->set_tx_att(10.0,10.0,10.0,0.0);
    SP2401->set_da_sw(sp2401_r1a::FROM_TO_RF);
    SP2401->set_ad_sw(sp2401_r1a::FROM_TO_RF);
    SP2401->set_dds_src(sp2401_r1a::SINGLE_TONE);
    SP2401->set_dds1(20000000.0);
    SP2401->set_tx_pwr_comp(0.0);
    SP2401->set_tx_phase_rotate_I(0.0);
    SP2401->set_tx_amplitude_balance(8192,8192);
    SP2401->set_tx_filter_sw(false);
}

void Q_Cal_TX_Sideband_Widget::Thread::calUseSA(double &thM, uint16_t &am_i_m, uint16_t &am_q_m, double &pwrSb)
{
    quint64 freq = 2400000000;
    double pwrSA = 0.0;

    double thL = -10.0;
    double thR = 10.0;
    thM = 0.0;
    quint16 am_i_l = 8192 - 500;
    quint16 am_i_r = 8192 + 500;
    quint16 am_q_l = 8192 - 500;
    quint16 am_q_r = 8192 + 500;
    am_i_m = 8192;
    am_q_m = 8192;

    DECLARE_DB_ITEM_TABLE(lol_table,tx_lol_table_t,SP1401->data_base()->db(cal_table_t::TX_LOL));
    tx_lol_table_t::data_f_t data_lol;

    lol_table->get(freq,data_lol);

    Instr.init();
    Instr.has_sa();
    Instr.sa_reset();
    Instr.sa_set_ref(-10.0);
    Instr.sa_set_span(1e6);
    Instr.sa_set_rbw(50e3,false);
    Instr.sa_set_vbw(10e3,false);

    SP1401->set_io_mode(io_mode_t::OUTPUT);
    SP1401->set_tx_freq(freq);
    SP2401->set_tx_phase_rotate_I(0.0);
    SP2401->set_tx_amplitude_balance(8192,8192);
    SP2401->set_duc_dds(0.0);
    SP2401->set_tx_dc_offset(data_lol.dc_i,data_lol.dc_q);

    Instr.sa_set_cf(double(freq - 20000000));
    Instr.sa_set_ref(-10.0);
    Instr.sa_sweep_once();
    Instr.sa_set_peak_search(Peak_Highest);

    while ((thR - thL) >= 1) {
        pausePoint();

        thM = round((thL + thR) / 2.0,1);

        if (slopeTh_SA(thM,1,pwrSb) > 0) {
            thR = thM;
        } else {
            thL = thM;
        }
    }

    Instr.sa_set_ref(-40.0);
    thL = thM - 0.5;
    thR = thM + 0.5;

    while ((thR - thL) >= 1) {
        pausePoint();

        thM = round((thL + thR) / 2.0,1);

        if (slopeTh_SA(thM,0.1,pwrSb) > 0) {
            thR = thM;
        } else {
            thL = thM;
        }
    }

    double thM_min = thM;

    for (thM = thL;thM - thR <= 0.0;thM += 0.1) {
        pausePoint();

        SP2401->set_tx_phase_rotate_I(thM);
        Instr.sa_sweep_once();
        Instr.sa_get_marker_pwr(pwrSA);

        if (pwrSA < pwrSb) {
            pwrSb = pwrSA;
            thM_min = thM;
        }
    }
    thM = thM_min;
    SP2401->set_tx_phase_rotate_I(thM);

    while ((am_i_r - am_i_l) >= 2 ) {
        pausePoint();

        am_i_m = quint16((am_i_l + am_i_r) / 2);

        if (slopeAmI_SA(am_i_m,am_q_m,10,pwrSb) > 0) {
            am_i_r = am_i_m;
        } else {
            am_i_l = am_i_m;
        }
    }

    while ((am_q_r - am_q_l) >= 2) {
        pausePoint();

        am_q_m = quint16((am_q_l + am_q_r) / 2);

        if (slopeAmQ_SA(am_i_m,am_q_m,10,pwrSb) > 0) {
            am_q_r = am_q_m;
        } else {
            am_q_l = am_q_m;
        }
    }

    thM_min = thM;
    thL = thM - 0.5;
    thR = thM + 0.5;

    for (thM = thL;thM - thR <= 0.0;thM += 0.1) {
        pausePoint();

        SP2401->set_tx_phase_rotate_I(thM);
        Instr.sa_sweep_once();
        Instr.sa_get_marker_pwr(pwrSA);

        if (pwrSA < pwrSb) {
            pwrSb = pwrSA;
            thM_min = thM;
        }
    }
    thM = thM_min;
    SP2401->set_tx_phase_rotate_I(thM);

    for (int16_t retry = 0;retry < 2;retry ++) {
        pausePoint();

        thM_min = thM;
        thL = thM - 0.2;
        thR = thM + 0.2;

        for (thM = thL;thM - thR <= 0.0;thM += 0.1) {
            SP2401->set_tx_phase_rotate_I(thM);
            Instr.sa_sweep_once();
            Instr.sa_get_marker_pwr(pwrSA);

            if (pwrSA < pwrSb) {
                pwrSb = pwrSA;
                thM_min = thM;
            }
        }
        thM = thM_min;
        SP2401->set_tx_phase_rotate_I(thM);

        if (slopeAmI_SA(am_i_m,am_q_m,1,pwrSb) > 0) {
            while (--am_i_m) { if (slopeAmI_SA(am_i_m,am_q_m,1,pwrSb) < 0) break; }
        } else {
            while (++am_i_m) { if (slopeAmI_SA(am_i_m,am_q_m,1,pwrSb) > 0) break; }
        }

        if (slopeAmQ_SA(am_i_m,am_q_m,1,pwrSb) > 0) {
            while (--am_q_m) { if (slopeAmQ_SA(am_i_m,am_q_m,1,pwrSb) < 0) break; }
        } else {
            while (++am_q_m) { if (slopeAmQ_SA(am_i_m,am_q_m,1,pwrSb) > 0) break; }
        }
    }

    if (pwrSb > spec::cal_tx_sb_up() || pwrSb < spec::cal_tx_sb_down()) {
        thM = 0.0;
        am_i_m = 8192;
        am_q_m = 8192;
        Log.set_last_err("Cal TX Sideband Fail.Power:%f",pwrSb);
        THREAD_ERROR_BOX(Log.last_err());
        emit threadProcess(Running_Except);
    }
}

double Q_Cal_TX_Sideband_Widget::Thread::slopeTh_SA(double th, double delta, double &pwr)
{
    double pwrSA[2] = {0.0,0.0};
    SP2401->set_tx_phase_rotate_I(th - delta);
    Instr.sa_sweep_once();
    Instr.sa_get_marker_pwr(pwrSA[0]);

    SP2401->set_tx_phase_rotate_I(th);
    Instr.sa_sweep_once();
    Instr.sa_get_marker_pwr(pwrSA[1]);
    pwr = pwrSA[1];

    return ((pwrSA[1] - pwrSA[0]) / delta);
}

double Q_Cal_TX_Sideband_Widget::Thread::slopeAmI_SA(uint16_t am_i, uint16_t am_q, uint16_t delta, double &pwr)
{
    double pwrSA[2] = {0.0,0.0};

    SP2401->set_tx_amplitude_balance(am_i - delta,am_q);
    Instr.sa_sweep_once();
    Instr.sa_get_marker_pwr(pwrSA[0]);

    SP2401->set_tx_amplitude_balance(am_i,am_q);
    Instr.sa_sweep_once();
    Instr.sa_get_marker_pwr(pwrSA[1]);
    pwr = pwrSA[1];

    return ((pwrSA[1] - pwrSA[0]) / delta);
}

double Q_Cal_TX_Sideband_Widget::Thread::slopeAmQ_SA(uint16_t am_i, uint16_t am_q, uint16_t delta, double &pwr)
{
    double pwrSA[2] = {0,0};

    SP2401->set_tx_amplitude_balance(am_i,am_q - delta);
    Instr.sa_sweep_once();
    Instr.sa_get_marker_pwr(pwrSA[0]);

    SP2401->set_tx_amplitude_balance(am_i,am_q);
    Instr.sa_sweep_once();
    Instr.sa_get_marker_pwr(pwrSA[1]);
    pwr = pwrSA[1];

    return ((pwrSA[1] - pwrSA[0]) / delta);
}

void Q_Cal_TX_Sideband_Widget::init()
{
//    registerModelView(&model,ui->tableView);
    Config("Freq",QLineEdit)->setText("2400M");
    Config("Method",QComboBox)->addItems({"Spectrum","Loopback"});

    Q_Thread_Widget::init();
}

void Q_Cal_TX_Sideband_Widget::prepare(const bool is_exp)
{
//    Radio.prepare_case(RFIdx,RX_RF_VGA,config->textEditFreq->toPlainText().toStdString(),is_exp);
}

void Q_Cal_TX_Sideband_Widget::uiUpdate(const int first,const int last,const int kase)
{
    Q_UNUSED(last);

//    ui->config_result->ui->ledResult->setColor(model.table->at(0).is_monotonic() ? Qt::green : Qt::red);
//    result->checkBoxMonotonic->setChecked(model.table->at(0).is_monotonic());
//    result->lineEditDyRange->setText(QString("%1").arg(model.table->at(0).dynamic_range()));

    ui->tableView->selectRow(first);
}
