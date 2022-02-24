#include "q_cal_r1c_tx_sb_thread.h"
#include "q_model_tx_sb.h"
#include "self_cal_helper.h"
#include "algo_math.hpp"
#include "spec.h"

using namespace ns_sp1401;

void QCalR1CTXSBThread::run()
{
    RD_CAL_TRY
    CAL_THREAD_START("TX Sideband",100);

    QTXSBModel *model = dynamic_cast<QTXSBModel *>(calParam.model_0);
    bool useSA = (calParam.methodSB == M_Spectrum);

    double thM = 0.0;
    quint16 am_i_m = 8192;
    quint16 am_q_m = 8192;
    double pwrSB = 0.0;

    tx_sb_table_r1cd::data_f_t data;

    if (useSA) {
        THREAD_CHECK_BOX(QString("TX<===>Spectrum"));
    }

    initTXChain();

    if (useSA) {
        calUseSA(thM,am_i_m,am_q_m,pwrSB);

        THREAD_TEST_CANCEL
        data.th   = thM;
        data.am_i = am_i_m;
        data.am_q = am_q_m;
        data.pwr  = pwrSB;
        data.use_sa = useSA;
        SP1401->get_temp(4,data.temp[0]);
        SP1401->get_temp(5,data.temp[1]);
        SP1401->get_temp(6,data.temp[2]);
        SP1401->get_temp(7,data.temp[3]);
        data.time = getCurTime();
        SP1401->cf()->add(cal_file::TX_SB,&data);
        SP1401->cf()->w(cal_file::TX_SB);
    } else {
        self_cal_tx_sb_helper caller(this->SP3301,RFIdx);

        if (caller.run(&data)) {
            Log.set_last_err("Cal TX Sideband Fail.Power:%f",caller.pwr_sb);
            THREAD_ERROR_BOX(Log.last_err());
            emit threadProcess(RUNNING_EXCEPT);
        }
    }

    for (quint32 i = 0;i < tx_freqs_called;i ++) {
        data.freq = tx_freq_star + i * tx_freq_step_called;
        model->calTable()->replace(i,data);
        emit update(model->index(i,0),model->index(i,10));
    }

    SET_PROG_POS(100);
    THREAD_ENDED
    RD_CAL_CATCH
}

void QCalR1CTXSBThread::initTXChain()
{
    SP1401->set_tx_att(10.0,10.0,10.0,0.0);
    SP2401->set_da_sw(sp2401_r1a::FROM_TO_RF);
    SP2401->set_ad_sw(sp2401_r1a::FROM_TO_RF);
    SP2401->set_dds_src(sp2401_r1a::SINGLE_TONE);
    SP2401->set_dds1(20000000.0);
    SP2401->set_tx_pwr_comp(0.0);
    SP2401->set_tx_phase_rotate_I(0.0);
    SP2401->set_tx_amplitude_balance(8192,8192);
    SP2401->set_tx_filter_sw(false);
}

void QCalR1CTXSBThread::calUseSA(double &thM, uint16_t &am_i_m, uint16_t &am_q_m, double &pwrSb)
{
    quint64 freq = 2400000000;
    double pwrSA = 0.0;
    qint16 dc_i = 0,dc_q = 0;
    float tempLOL = 0.0;

    double thL = -10.0;
    double thR = 10.0;
    thM = 0.0;
    quint16 am_i_l = 8192 - 500,am_i_r = 8192 + 500;
    quint16 am_q_l = 8192 - 500,am_q_r = 8192 + 500;
    am_i_m = 8192;
    am_q_m = 8192;

    Instr.init();
    Instr.has_sa();
    Instr.sa_reset();
    Instr.sa_set_ref(-10.0);
    Instr.sa_set_span(1e6);
    Instr.sa_set_rbw(50e3,false);
    Instr.sa_set_vbw(10e3,false);

    SP1401->set_io_mode(rd::OUTPUT);
    SP1401->set_tx_freq(freq);
    SP2401->set_tx_phase_rotate_I(0.0);
    SP2401->set_tx_amplitude_balance(8192,8192);
    SP2401->set_duc_dds(0.0);
    SP1401->cf()->m_tx_lol->get(freq,dc_i,dc_q,tempLOL);
    SP2401->set_tx_dc_offset(dc_i,dc_q);

    Instr.sa_set_cf(double(freq - 20000000));
    Instr.sa_set_ref(-10.0);
    Instr.sa_sweep_once();
    Instr.sa_set_peak_search(sa::PEAK_HIGHEST);

    while ((thR - thL) >= 1) {
        THREAD_TEST_PAUSE_S
        THREAD_TEST_CANCEL
        thM = round((thL + thR) / 2.0,1);
        if (slopeTh_SA(thM,1,pwrSb) > 0) {
            thR = thM;
        } else {
            thL = thM;
        }
        THREAD_TEST_PAUSE_E
    }
    Instr.sa_set_ref(-40.0);

    thL = thM - 0.5;
    thR = thM + 0.5;
    while ((thR - thL) >= 1) {
        THREAD_TEST_PAUSE_S
        THREAD_TEST_CANCEL
        thM = round((thL + thR) / 2.0,1);
        if (slopeTh_SA(thM,0.1,pwrSb) > 0) {
            thR = thM;
        } else {
            thL = thM;
        }
        THREAD_TEST_PAUSE_E
    }

    double thM_min = thM;
    for (thM = thL; thM <= thR; thM += 0.1) {
        THREAD_TEST_PAUSE_S
        THREAD_TEST_CANCEL
        SP2401->set_tx_phase_rotate_I(thM);
        Instr.sa_sweep_once();
        Instr.sa_get_marker_pwr(pwrSA);
        if (pwrSA < pwrSb) {
            pwrSb = pwrSA;
            thM_min = thM;
        }
        THREAD_TEST_PAUSE_E
    }
    thM = thM_min;
    SP2401->set_tx_phase_rotate_I(thM);

    while ((am_i_r - am_i_l) >= 2 ) {
        THREAD_TEST_PAUSE_S
        THREAD_TEST_CANCEL
        am_i_m = quint16((am_i_l + am_i_r) / 2);
        if (slopeAmI_SA(am_i_m,am_q_m,10,pwrSb) > 0) {
            am_i_r = am_i_m;
        } else {
            am_i_l = am_i_m;
        }
        THREAD_TEST_PAUSE_E
    }
    while ((am_q_r - am_q_l) >= 2) {
        THREAD_TEST_PAUSE_S
        THREAD_TEST_CANCEL
        am_q_m = quint16((am_q_l + am_q_r) / 2);
        if (slopeAmQ_SA(am_i_m,am_q_m,10,pwrSb) > 0) {
            am_q_r = am_q_m;
        } else {
            am_q_l = am_q_m;
        }
        THREAD_TEST_PAUSE_E
    }

    thM_min = thM;
    thL = thM - 0.5;
    thR = thM + 0.5;
    for (thM = thL; thM <= thR; thM += 0.1) {
        THREAD_TEST_PAUSE_S
        THREAD_TEST_CANCEL
        SP2401->set_tx_phase_rotate_I(thM);
        Instr.sa_sweep_once();
        Instr.sa_get_marker_pwr(pwrSA);
        if (pwrSA < pwrSb) {
            pwrSb = pwrSA;
            thM_min = thM;
        }
        THREAD_TEST_PAUSE_E
    }
    thM = thM_min;
    SP2401->set_tx_phase_rotate_I(thM);

    for (int16_t retry = 0;retry < 2;retry ++) {
        THREAD_TEST_PAUSE_S
        thM_min = thM;
        thL = thM - 0.2;
        thR = thM + 0.2;
        for (thM = thL; thM <= thR; thM += 0.1) {
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
            while (-- am_i_m) {
                if (slopeAmI_SA(am_i_m,am_q_m,1,pwrSb) < 0)
                    break;
            }
        } else {
            while (++ am_i_m) {
                if (slopeAmI_SA(am_i_m,am_q_m,1,pwrSb) > 0)
                    break;
            }
        }

        if (slopeAmQ_SA(am_i_m,am_q_m,1,pwrSb) > 0) {
            while (-- am_q_m) {
                if (slopeAmQ_SA(am_i_m,am_q_m,1,pwrSb) < 0)
                    break;
            }
        } else {
            while (++ am_q_m) {
                if (slopeAmQ_SA(am_i_m,am_q_m,1,pwrSb) > 0)
                    break;
            }
        }
        THREAD_TEST_PAUSE_E
    }

    if (pwrSb > spec::cal_tx_sb_up() || pwrSb < spec::cal_tx_sb_down()) {
        thM = 0.0;
        am_i_m = 8192;
        am_q_m = 8192;
        Log.set_last_err("Cal TX Sideband Fail.Power:%f",pwrSb);
        THREAD_ERROR_BOX(Log.last_err());
        emit threadProcess(RUNNING_EXCEPT);
    }
}

double QCalR1CTXSBThread::slopeTh_SA(double th, double delta, double &pwr)
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

double QCalR1CTXSBThread::slopeAmI_SA(uint16_t am_i, uint16_t am_q, uint16_t delta, double &pwr)
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

double QCalR1CTXSBThread::slopeAmQ_SA(uint16_t am_i, uint16_t am_q, uint16_t delta, double &pwr)
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


void QExpR1CTXSBThread::run()
{
    initProgress("Exporting Tx Sideband",100);

    QTXSBModel *model = dynamic_cast<QTXSBModel *>(calParam.model_0);

    tx_sb_table_r1cd::data_f_t data;
    int idx = 0;

    SP1401->cf()->map2buf(cal_file::TX_SB);
    for (quint64 freq = tx_freq_star;freq <= tx_freq_stop;freq += tx_freq_step_called) {
        SP1401->cf()->m_tx_sb->get(freq,&data);
        model->calTable()->replace(idx,data);
        idx ++;
    }

    emit update(model->index(0,0),model->index(tx_freqs_called,10));

    SET_PROG_POS(100);
    THREAD_ENDED
}
