#include "q_cal_r1c_tx_loleak_thread.h"
#include "q_model_tx_loleak.h"
#include "algo_math.hpp"
#include "spec.h"

using namespace ns_sp9500;
using namespace ns_sp1401;

void QCalR1CTXLOLeakThread::run()
{
    RD_TRY
    CAL_THREAD_START("TX LO Leakage",100);

    QTXLOLeakModel *model = dynamic_cast<QTXLOLeakModel *>(calParam.model_0);
    bool useSA = (calParam.methodLOLeak == M_Spectrum);

    quint64 freq = 2400000000;
    qint16 dc_i_m = 0;
    qint16 dc_q_m = 0;

    tx_lol_table_r1cd::data_f_t data;

    if (calParam.calX9119 || useSA) {
        THREAD_CHECK_BOX(QString("TX<===>Spectrum"));
    }

    initTXChain();

    if (calParam.calX9119) {
        calX9119();
    }

    THREAD_TEST_CANCEL
    if (useSA) {
        calUseSA(dc_i_m,dc_q_m,data.pwr);
    } else {
        calUseLoop(dc_i_m,dc_q_m,data.pwr);
    }

    THREAD_TEST_CANCEL
    data.freq = freq;
    data.dc_i = dc_i_m;
    data.dc_q = dc_q_m;
    data.use_sa = useSA;
    SP1401->get_temp(4,data.temp[0]);
    SP1401->get_temp(5,data.temp[1]);
    SP1401->get_temp(6,data.temp[2]);
    SP1401->get_temp(7,data.temp[3]);
    data.time = getCurTime();
    SP1401->cf()->add(cal_file::TX_LOL,&data);
    SP1401->cf()->w(cal_file::TX_LOL);

    for (quint32 i = 0;i < tx_freqs_called;i ++) {
        data.freq = tx_freq_star + i * tx_freq_step_called;
        model->calTable()->replace(i,data);
        emit update(model->index(i,0),model->index(i,9));
    }

    SET_PROG_POS(100);
    THREAD_ENDED
    RD_CAL_CATCH
}

void QCalR1CTXLOLeakThread::calX9119()
{
    quint64 freq = tx_freq_stop;
    double mkrFreq = 0.0;
    double mkrPwr  = -100;
    double freqDelta = 0.0;
    double curSpan = 15000.0;
    x9119_table::data_t Xdata;

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
        THREAD_TEST_PAUSE_S
        THREAD_TEST_CANCEL
        SP3501.vol_9119(value);
        Instr.sa_sweep_once();
        Instr.sa_set_peak_search(sa::PEAK_HIGHEST);
        Instr.sa_get_marker_freq(mkrFreq);
        Instr.sa_get_marker_pwr(mkrPwr);

        freqDelta = abs(mkrFreq - double(freq));

        if (freqDelta < 5.0 && mkrPwr >= -70.0) {
            Xdata.value = value;
            threadLock.unlock();
            break;
        } else if (freqDelta < 500.0) {
            if (curSpan != 1000.0) Instr.sa_set_span(curSpan = 1000.0);
        } else if (freqDelta < 2500.0) {
            if (curSpan != 5000.0) Instr.sa_set_span(curSpan = 5000.0);
        } else if (freqDelta < 5000.0) {
            if (curSpan != 10000.0) Instr.sa_set_span(curSpan = 10000.0);
        }
        THREAD_TEST_PAUSE_E
    }

    if (freqDelta > 5.0) {
        Log.set_last_err("Cal OCXO Fail");
        THREAD_ERROR_BOX(Log.last_err());
        emit threadProcess(RUNNING_EXCEPT);
    }

    SP1401->cf()->add(cal_file::X9119,&Xdata);
    SP1401->cf()->w(cal_file::X9119);
}

void QCalR1CTXLOLeakThread::calUseSA(qint16 &dc_i_m, qint16 &dc_q_m,double &pwrLOL)
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

    SP1401->set_io_mode(OUTPUT);
    SP1401->set_tx_freq(freq);
    SP2401->set_tx_dc_offset(dc_i_m,dc_q_m);

    Instr.sa_set_cf(freq);
    Instr.sa_set_ref(0.0);
    Instr.sa_sweep_once();
    Instr.sa_sweep_once();
    Instr.sa_set_peak_search(sa::PEAK_HIGHEST);

    step = 200;
    while ((dc_i_r - dc_i_l) > 4) {
        THREAD_TEST_PAUSE_S
        THREAD_TEST_CANCEL
        dc_i_m = (qint16)round((dc_i_l + dc_i_r) / 2.0,0);
        if (slopeI_SA(dc_i_m,dc_q_m,step,pwrLOL) > 0)
            dc_i_r = dc_i_m;
        else
            dc_i_l = dc_i_m;
        THREAD_TEST_PAUSE_E
    }
    while ((dc_q_r - dc_q_l) > 4) {
        THREAD_TEST_PAUSE_S
        THREAD_TEST_CANCEL
        dc_q_m = (qint16)round((dc_q_l + dc_q_r) / 2.0,0);
        if (slopeQ_SA(dc_i_m,dc_q_m,step,pwrLOL) > 0)
            dc_q_r = dc_q_m;
        else
            dc_q_l = dc_q_m;
        THREAD_TEST_PAUSE_E
    }

    step = 20;
    dc_i_l = dc_i_m - 100;
    dc_i_r = dc_i_m + 100;

    if (pwrLOL <= -40.0) {
        Instr.sa_set_ref(-40.0);
    }

    while ((dc_i_r - dc_i_l) > 4) {
        THREAD_TEST_PAUSE_S
        THREAD_TEST_CANCEL
        dc_i_m = (qint16)round((dc_i_l + dc_i_r) / 2.0,0);
        if (slopeI_SA(dc_i_m,dc_q_m,step,pwrLOL) > 0)
            dc_i_r = dc_i_m;
        else
            dc_i_l = dc_i_m;
        THREAD_TEST_PAUSE_E
    }
    dc_q_l = dc_q_m - 100;
    dc_q_r = dc_q_m + 100;
    while ((dc_q_r - dc_q_l) > 4) {
        THREAD_TEST_PAUSE_S
        THREAD_TEST_CANCEL
        dc_q_m = (qint16)round((dc_q_l + dc_q_r) / 2.0,0);
        if (slopeQ_SA(dc_i_m,dc_q_m,step,pwrLOL) > 0)
            dc_q_r = dc_q_m;
        else
            dc_q_l = dc_q_m;
        THREAD_TEST_PAUSE_E
    }

    if (pwrLOL <= -40.0)
        Instr.sa_set_ref(-40.0);

    step = 1;
    for (qint16 retry = 0;retry < 3;retry ++) {
        THREAD_TEST_PAUSE_S
        if (slopeI_SA(dc_i_m,dc_q_m,1,pwrLOL) > 0) {
            while ( -- dc_i_m) {
                if (slopeI_SA(dc_i_m,dc_q_m,step,pwrLOL) < 0)
                    break;
            }
        } else {
            while ( ++ dc_i_m) {
                if (slopeI_SA(dc_i_m,dc_q_m,step,pwrLOL) > 0)
                    break;
            }
        }

        if (slopeQ_SA(dc_i_m,dc_q_m,1,pwrLOL) > 0) {
            while ( -- dc_q_m) {
                if (slopeQ_SA(dc_i_m,dc_q_m,step,pwrLOL) < 0)
                    break;
            }
        } else {
            while ( ++ dc_q_m) {
                if (slopeQ_SA(dc_i_m,dc_q_m,step,pwrLOL) > 0)
                    break;
            }
        }
        THREAD_TEST_PAUSE_E
    }

    qint16 DC_I_M_Min = dc_i_m;
    qint16 DC_Q_M_Min = dc_q_m;

    for (int retry = 0;retry < 3;retry ++) {
        dc_i_l = dc_i_m - 5;
        dc_i_r = dc_i_m + 5;
        for (dc_i_m = dc_i_l; dc_i_m <= dc_i_r; dc_i_m ++) {
            THREAD_TEST_PAUSE_S
            SP2401->set_tx_dc_offset(dc_i_m,dc_q_m);
            Instr.sa_sweep_once();
            Instr.sa_get_marker_pwr(pwrSA);
            if (pwrSA < pwrLOL) {
                pwrLOL = pwrSA;
                DC_I_M_Min = dc_i_m;
            }
            THREAD_TEST_PAUSE_E
        }
        dc_i_m = DC_I_M_Min;
        for (dc_q_m = dc_q_l; dc_q_m <= dc_q_r; dc_q_m ++) {
            THREAD_TEST_PAUSE_S
            SP2401->set_tx_dc_offset(dc_i_m,dc_q_m);
            Instr.sa_sweep_once();
            Instr.sa_get_marker_pwr(pwrSA);
            if (pwrSA < pwrLOL) {
                pwrLOL = pwrSA;
                DC_Q_M_Min = dc_q_m;
            }
            THREAD_TEST_PAUSE_E
        }
        dc_q_m = DC_Q_M_Min;
    }

    if (pwrLOL > spec::cal_tx_lol_up() || pwrLOL < spec::cal_tx_lol_down()) {
        dc_i_m = 0;
        dc_q_m = 0;
        Log.set_last_err("Cal TX LO Leakage Fail.Power:%f",pwrLOL);
        THREAD_ERROR_BOX(Log.last_err());
        emit threadProcess(RUNNING_EXCEPT);
    }
}

void QCalR1CTXLOLeakThread::calUseLoop(qint16 &dc_i_m, qint16 &dc_q_m,double &pwrLOL)
{
    SP1401->set_io_mode(LOOP);
    SP1401->set_rx_lna_att_sw(r1c::RX_ATT);
    SP1401->set_rx_att_019_sw(r1c::RX_ATT_0);
    SP2401->set_ddc(-92640000.0);

    uint64_t freq = 2400000000;
    qint16 dc_i_l = -10000,dc_i_r = 10000;
    qint16 dc_q_l = -10000,dc_q_r = 10000;
    qint16 step = 0;
    int64_t adLOL[512] = {0},adMin = LONG_MAX;

    SP1401->set_rx_freq(freq);
    SP1401->set_rx_att1(10.0);
    SP1401->set_rx_att2(0.0);
    SP1401->set_rx_att3(0.0);
    SP1401->set_tx_freq(freq);
    SP2401->set_tx_dc_offset(dc_i_m,dc_q_m);
    msleep(50);

    THREAD_TEST_CANCEL
    step = 100;
    memset(adLOL,0,sizeof(adLOL));
    adMin = getMinDCOffsetI_Rx(step,1,&dc_i_l,&dc_i_r,&dc_i_m,&dc_q_m,adLOL);
    memset(adLOL,0,sizeof(adLOL));
    adMin = getMinDCOffsetQ_Rx(step,1,&dc_q_l,&dc_q_r,&dc_i_m,&dc_q_m,adLOL);

    THREAD_TEST_CANCEL
    step = 10;
    memset(adLOL,0,sizeof(adLOL));
    adMin = getMinDCOffsetI_Rx(step,1,&dc_i_l,&dc_i_r,&dc_i_m,&dc_q_m,adLOL);
    memset(adLOL,0,sizeof(adLOL));
    adMin = getMinDCOffsetQ_Rx(step,1,&dc_q_l,&dc_q_r,&dc_i_m,&dc_q_m,adLOL);

    THREAD_TEST_CANCEL
    step = 5;
    memset(adLOL,0,sizeof(adLOL));
    adMin = getMinDCOffsetI_Rx(step,2,&dc_i_l,&dc_i_r,&dc_i_m,&dc_q_m,adLOL);
    memset(adLOL,0,sizeof(adLOL));
    adMin = getMinDCOffsetQ_Rx(step,2,&dc_q_l,&dc_q_r,&dc_i_m,&dc_q_m,adLOL);

    THREAD_TEST_CANCEL
    step = 1;
    for (int32_t retry = 0;retry < 3;retry ++) {
        THREAD_TEST_CANCEL
        memset(adLOL,0,sizeof(adLOL));
        adMin = getMinDCOffsetI_Rx(step,5,&dc_i_l,&dc_i_r,&dc_i_m,&dc_q_m,adLOL);
        memset(adLOL,0,sizeof(adLOL));
        adMin = getMinDCOffsetQ_Rx(step,5,&dc_q_l,&dc_q_r,&dc_i_m,&dc_q_m,adLOL);
    }
    pwrLOL = ad_to_dBc(g_0dBFS,adMin);
}

double QCalR1CTXLOLeakThread::slopeI_SA(qint16 dc_i_m, qint16 dc_q_m, qint16 delta, double &pwr)
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

void QCalR1CTXLOLeakThread::initTXChain()
{
    SP1401->set_tx_att(10.0,10.0,10.0,0.0);
    SP1401->set_io_mode(OUTPUT);
    SP2401->set_da_sw(sp2401_r1a::FROM_TO_RF);
    SP2401->set_ad_sw(sp2401_r1a::FROM_TO_RF);
    SP2401->set_dds_src(sp2401_r1a::ALL_0);
    SP2401->set_tx_pwr_comp(0.0);
    SP2401->set_tx_dc_offset(0,0);
    SP2401->set_tx_filter_sw(false);
}

double QCalR1CTXLOLeakThread::slopeQ_SA(qint16 dc_i_m, qint16 dc_q_m, qint16 delta, double &pwr)
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

int64_t QCalR1CTXLOLeakThread::getMinDCOffsetI_Rx(qint16 step, qint16 lr_coef, qint16 *dc_i_l, qint16 *dc_i_r, qint16 *dc_i_m, qint16 *dc_q_m, int64_t *ad)
{
    int64_t adMin = LONG_MAX;
    qint16 idx = 0,idxMin = 0;
    for (*dc_i_m = *dc_i_l;*dc_i_m <= *dc_i_r;*dc_i_m += step) {
        THREAD_TEST_PAUSE_S
        SP2401->set_tx_dc_offset(*dc_i_m,*dc_q_m);
        msleep(10);
        SP1401->get_ads5474(ad[idx]);
        if (ad[idx] < adMin) {
            adMin = ad[idx];
            idxMin = idx;
        }
        idx ++;
        THREAD_TEST_PAUSE_E
    }
    *dc_i_m = *dc_i_l + idxMin * step;
    *dc_i_l = *dc_i_m - step * lr_coef;
    *dc_i_r = *dc_i_m + step * lr_coef;
    return adMin;
}

int64_t QCalR1CTXLOLeakThread::getMinDCOffsetQ_Rx(qint16 step, qint16 lr_coef, qint16 *dc_q_l, qint16 *dc_q_r, qint16 *dc_i_m, qint16 *dc_q_m, int64_t *ad)
{
    int64_t adMin = LONG_MAX;
    qint16 idx = 0,idxMin = 0;
    for (*dc_q_m = *dc_q_l;*dc_q_m <= *dc_q_r;*dc_q_m += step) {
        THREAD_TEST_PAUSE_S
        SP2401->set_tx_dc_offset(*dc_i_m,*dc_q_m);
        msleep(10);
        SP1401->get_ads5474(ad[idx]);
        if (ad[idx] < adMin) {
            adMin = ad[idx];
            idxMin = idx;
        }
        idx ++;
        THREAD_TEST_PAUSE_E
    }
    *dc_q_m = *dc_q_l + idxMin * step;
    *dc_q_l = *dc_q_m - step * lr_coef;
    *dc_q_r = *dc_q_m + step * lr_coef;
    return adMin;
}

void QExpR1CTXLOLeakThread::run()
{
    initProgress("Exporting TX LO Leakage",100);

    QTXLOLeakModel *model = dynamic_cast<QTXLOLeakModel *>(calParam.model_0);

    tx_lol_table_r1cd::data_f_t data;
    int idx = 0;

    SP1401->cf()->map2buf(cal_file::TX_LOL);
    for (quint64 freq = tx_freq_star;freq <= tx_freq_stop;freq += tx_freq_step_called) {
        SP1401->cf()->m_tx_lol->get(freq,&data);
        model->calTable()->replace(idx,data);
        idx ++;
    }

    emit update(model->index(0,0),model->index(tx_freqs_called,9));

    SET_PROG_POS(100);
    THREAD_ENDED
}
