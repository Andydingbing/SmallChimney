#include "q_test_freq_res_thread.h"
#include "q_model_freq_res.h"
#include "algo_math.hpp"
#include "spec.h"

using namespace ns_sp9500;
using namespace ns_sp1401;
using namespace ns_sp2401;

void Q_Test_FreqRes_Thread::run()
{
    RD_TEST_TRY

    param = static_cast<TestFreqResParam *>(Param);

    RF_TXFreqString.star = param->RF_TxFreqStar.toStdString();
    RF_TXFreqString.stop = param->RF_TxFreqStop.toStdString();
    RF_TXFreqString.step = param->RF_TxFreqStep.toStdString();

    RF_RXFreqString.star = param->RF_RxFreqStar.toStdString();
    RF_RXFreqString.stop = param->RF_RxFreqStop.toStdString();
    RF_RXFreqString.step = param->RF_RxFreqStep.toStdString();

    parse_range_freq_string(RF_TXFreqString,RF_TXFreqRange);
    parse_range_freq_string(RF_RXFreqString,RF_RXFreqRange);

    dataRF_Tx = dynamic_cast<QwtRF_TX_FreqResData *>(param->plotData_0);
    dataIF_Tx = dynamic_cast<QwtIF_TX_FreqResData *>(param->plotData_1);
    dataRF_Rx = dynamic_cast<QwtRF_RX_FreqResData *>(param->plotData_2);
    dataIF_Rx = dynamic_cast<QwtIF_RX_FreqResData *>(param->plotData_3);

    quint32 pts = 0;

    pts += param->isTestRF_Tx ? RF_TXFreqRange.freqs.size() : 0;
    pts += param->isTestRF_Rx ? RF_RXFreqRange.freqs.size() : 0;
    pts += param->isTestIF_Tx ? dl_filter_160M_freqs : 0;
    pts += param->isTestIF_Rx ? ul_filter_160M_freqs : 0;

    TEST_THREAD_START("TX FreqResponse",pts);

    if (param->isTestRF_Tx || param->isTestIF_Tx) {
        sweepTx();
    }

    THREAD_TEST_CANCEL

    if (param->isTestRF_Rx || param->isTestIF_Rx) {
        sweepRx();
    }

    THREAD_ENDED
    RD_TEST_CATCH
}

void QTestR1CFreqResThread::sweepTx()
{
    if (param->isTestRF_Tx || param->isTestIF_Tx) {
        THREAD_CHECK_BOX("TX<===>Spectrum");
    }

    Instr.init();
    Instr.has_sa();
    Instr.sa_reset();
    Instr.sa_set_span(1000000.0);
    Instr.sa_set_rbw(1000.0,true);
    Instr.sa_set_vbw(1000.0,true);
    Instr.sa_set_ref(r1c::tx_base_pwr_op + 20.0);

    uint64_t RF_Freq = 0;
    double IF_Freq = 0.0;
    double dds1 = 20e6;
    double att0 = 0.0;
    double att1 = 0.0;
    double att2 = 0.0;
    double att3 = 0.0;
    double d_gain = 0.0;

    freq_res_data data;

    if (RFVer >= R1C && RFVer <= R1E) {
        tx_pwr_op_table_r1c::guess_base_r1c(att0,att1,att2,att3,d_gain);
    } else if (RFVer >= R1F) {
        tx_pwr_op_table_r1c::guess_base_r1f(att0,att1,att2,d_gain);
    }

    SP1401->set_io_mode(OUTPUT);
    SP1401_R1F->set_tx_att(att0,att1,att2,att3);
    SP1401_R1F->set_rx_att_019_sw(r1c::RX_ATT_19);
    SP1401_R1F->set_rx_lna_att_sw(r1c::RX_ATT);
    SP1401_R1F->set_rx_att(30.0,30.0,30.0);
    SP2401->set_dds_src(sp2401_r1a::SINGLE_TONE);
    SP2401->set_tx_pwr_comp(d_gain);
    SP2401->set_dds1(dds1);
    SP2401->set_duc_dds(0.0);
    SP2401->set_tx_phase_rotate_I(0.0);
    SP2401->set_tx_amplitude_balance(8192,8192);
    SP2401->set_tx_dc_offset(0,0);
    SP2401->set_tx_filter_sw(false);

    if (param->isTestRF_Tx) {
        totalResult = true;
        dataRF_Tx->report()->set_result("Pass");

        for (quint32 i = 0;i < RF_TXFreqRange.freqs.size();i ++) {
            THREAD_TEST_CANCEL

            RF_Freq = RF_TXFreqRange.freqs.at(i);
            SP1401->set_tx_freq(RF_Freq);
            Instr.sa_set_cf(RF_Freq + dds1);
            msleep(50);

            Instr.sa_sweep_once();
            Instr.sa_set_peak_search(sa::PEAK_HIGHEST);
            Instr.sa_get_marker_pwr(data.pwr);

            if (i == 0) {
                pwrBefore = data.pwr;
                pwrMin = data.pwr;
                pwrMax = data.pwr;
            } else {
                if (pwrMin > data.pwr)
                    pwrMin = data.pwr;
                if (pwrMax < data.pwr)
                    pwrMax = data.pwr;
            }
            checkRF_Tx(data);
            dataRF_Tx->report()->add(RF_Freq,data);
            pwrBefore = data.pwr;
        }
        dataRF_Tx->report()->update();
        SP1401->ftp_put_tr_rf_tx_freq_res_test();
    }

    if (param->isTestIF_Tx) {
        totalResult = true;
        dataIF_Tx->report()->set_result("Pass");

        RF_Freq = 2000000000;
        Instr.sa_set_cf(RF_Freq);
        Instr.sa_set_span(200e6);
        SP1401->set_tx_freq(RF_Freq);

        for (quint32 i = 0;i < dl_filter_160M_freqs;i ++) {
            THREAD_TEST_CANCEL

            IF_Freq = dl_filter_160M_freq_star + i * dl_filter_freq_step;
            SP2401->set_dds1(IF_Freq);
            msleep(50);

            Instr.sa_sweep_once();
            Instr.sa_set_peak_search(sa::PEAK_HIGHEST);
            Instr.sa_get_marker_pwr(data.pwr);

            if (i == 0) {
                pwrBefore = data.pwr;
                pwrMin = data.pwr;
                pwrMax = data.pwr;
            } else {
                if (pwrMin > data.pwr)
                    pwrMin = data.pwr;
                if (pwrMax < data.pwr)
                    pwrMax = data.pwr;
            }
            checkIF_Tx(data);
            dataIF_Tx->report()->add(int64_t(IF_Freq),data);
            pwrBefore = data.pwr;
        }
        dataIF_Tx->report()->update();
        SP1401->ftp_put_tr_if_tx_freq_res_test();
    }
}

void Q_Test_FreqRes_Thread::checkRF_Tx(rf_tx_freq_res_test_data &data)
{
    bool result = true;
    if (data.pwr < spec::test_rf_tx_freq_res_min() || data.pwr > spec::test_rf_tx_freq_res_max())
        result = false;
    if (data.pwr - pwrBefore > 5.0)
        result = false;

    if (pwrMax - pwrMin > spec::test_rf_tx_freq_res()) {
        totalResult = false;
        dataRF_Tx->report()->set_result("Fail");
    }

    data.set_result(result ? test_common_data::PASS : test_common_data::FAIL);
    data.set_time();
    updateTotalResult(result);

    emit update(QModelIndex(),QModelIndex(),TI_RF_TX_FREQ_RES,int(totalResult));
}

void Q_Test_FreqRes_Thread::checkIF_Tx(if_tx_freq_res_test_data &data)
{
    bool result = true;
    if (data.pwr < spec::test_if_tx_freq_res_min() || data.pwr > spec::test_if_tx_freq_res_max())
        result = false;
    if (data.pwr - pwrBefore > 3.0)
        result = false;

    if (pwrMax - pwrMin > spec::test_if_tx_freq_res()) {
        totalResult = false;
        dataIF_Tx->report()->set_result("Fail");
    }

    data.set_result(result ? test_common_data::PASS : test_common_data::FAIL);
    data.set_time();
    updateTotalResult(result);

    emit update(QModelIndex(),QModelIndex(),TI_IF_TX_FREQ_RES,int(totalResult));
}

void QTestR1CFreqResThread::sweepRx()
{
    if (param->isTestRF_Rx || param->isTestIF_Rx) {
        THREAD_CHECK_BOX("RX<===>Signal Generator");
    }

    Instr.init();
    Instr.has_sg();
    Instr.sg_reset();
    Instr.sg_set_en_mod(false);
    Instr.sg_set_en_output(true);

    uint64_t RF_Freq = 0;
    double IF_Freq = 0.0;
    qint32 ref = 0;
    qint32 lnaAtt = 0;
    qint32 att019 = 0;
    double att1 = 0.0;
    double att2 = 0.0;
    double att3 = 0.0;
    qint64 ad = 0;

    freq_res_data data;

    if (RFVer >= R1C && RFVer <= R1E) {
        rx_ref_op_table_r1cd::guess_base_r1c(1,ref,lnaAtt,att019,att1,att2,att3);
    } else if (RFVer >= R1F) {
        rx_ref_op_table_r1cd::guess_base_r1f(1,ref,lnaAtt,att019,att1,att2);
    }

    SP1401->set_io_mode(OUTPUT);
    SP1401_R1F->set_tx_att(30.0,30.0,30.0,30.0);
    SP1401_R1F->set_rx_lna_att_sw(r1c::rx_lna_att_t(lnaAtt));
    SP1401_R1F->set_rx_att_019_sw(r1c::rx_att_019_t(att019));
    SP1401_R1F->set_rx_att(att1,att2,att3);
    SP1401->set_pwr_meas_src(sp1401::PWR_MEAS_FREE_RUN,false);
    SP1401->set_pwr_meas_timeout(6553600);
    SP1401->set_pwr_meas_samples(102400);
    SP2401->set_rx_filter_default();

    Instr.sg_set_pl(ref - 5.0);

    if (param->isTestRF_Rx) {
        totalResult = true;
        dataRF_Rx->report()->set_result("Pass");

        for (quint32 i = 0;i < RF_RXFreqRange.freqs.size();i ++) {
            THREAD_TEST_CANCEL

            RF_Freq = RF_RXFreqRange.freqs.at(i);
            SP1401->set_rx_freq(RF_Freq);
            Instr.sg_set_cw(RF_Freq);
            msleep(200);

            getADS5474(SP1401,ad,1);
            ad_to_dBc(g_0dBFS,ad,data.pwr);

            if (i == 0) {
                pwrBefore = data.pwr;
                pwrMin = data.pwr;
                pwrMax = data.pwr;
            } else {
                if (pwrMin > data.pwr)
                    pwrMin = data.pwr;
                if (pwrMax < data.pwr)
                    pwrMax = data.pwr;
            }
            checkRF_Rx(data);
            dataRF_Rx->report()->add(RF_Freq,data);
            pwrBefore = data.pwr;
        }
        dataRF_Rx->report()->update();
        SP1401->ftp_put_tr_rf_rx_freq_res_test();
    }

    if (param->isTestIF_Rx) {
        totalResult = true;
        dataIF_Rx->report()->set_result("Pass");

        RF_Freq = 2000000000;
        SP1401->set_rx_freq(RF_Freq);

        for (quint32 i = 0;i < ul_filter_160M_freqs;i ++) {
            THREAD_TEST_CANCEL

            IF_Freq = ul_filter_160M_freq_star + i * ul_filter_freq_step;

            Instr.sg_set_cw(RF_Freq + IF_Freq);
            msleep(100);
            data.pwr = getRxAvgPwr();

            if (i == 0) {
                pwrBefore = data.pwr;
                pwrMin = data.pwr;
                pwrMax = data.pwr;
            } else {
                if (pwrMin > data.pwr)
                    pwrMin = data.pwr;
                if (pwrMax < data.pwr)
                    pwrMax = data.pwr;
            }
            checkIF_Rx(data);
            dataIF_Rx->report()->add(int64_t(IF_Freq),data);
            pwrBefore = data.pwr;
        }
        dataIF_Rx->report()->update();
        SP1401->ftp_put_tr_if_rx_freq_res_test();
    }
}

void Q_Test_FreqRes_Thread::checkRF_Rx(rf_rx_freq_res_test_data &data)
{
    bool result = true;
    if (data.pwr < spec::test_rf_rx_freq_res_min() || data.pwr > spec::test_rf_rx_freq_res_max())
        result = false;
    if (data.pwr - pwrBefore > 5.0)
        result = false;

    if (pwrMax - pwrMin > spec::test_rf_rx_freq_res()) {
        totalResult = false;
        dataRF_Rx->report()->set_result("Fail");
    }

    data.set_result(result ? test_common_data::PASS : test_common_data::FAIL);
    data.set_time();
    updateTotalResult(result);

    emit update(QModelIndex(),QModelIndex(),TI_RF_RX_FREQ_RES,int(totalResult));
}

void Q_Test_FreqRes_Thread::checkIF_Rx(if_rx_freq_res_test_data &data)
{
    bool result = true;
    if (data.pwr < spec::test_if_rx_freq_res_min() || data.pwr > spec::test_if_rx_freq_res_max())
        result = false;
    if (data.pwr - pwrBefore > 3.0)
        result = false;

    if (pwrMax - pwrMin > spec::test_if_rx_freq_res()) {
        totalResult = false;
        dataIF_Rx->report()->set_result("Fail");
    }

    data.set_result(result ? test_common_data::PASS : test_common_data::FAIL);
    data.set_time();
    updateTotalResult(result);

    emit update(QModelIndex(),QModelIndex(),TI_IF_RX_FREQ_RES,int(totalResult));
}
