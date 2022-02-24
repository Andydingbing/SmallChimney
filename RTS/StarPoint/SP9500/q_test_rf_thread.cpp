#include "q_test_rf_thread.h"
#include "q_model_freq_res.h"
#include "spec.h"
#include "mainwindow.h"
#include <QDialog>

using namespace std;
using namespace ns_sp1401;

void QTestR1CRFThread::run()
{
    RD_TEST_TRY

    TestRFParam *param = static_cast<TestRFParam *>(Param);

//    quint32 pts = 0;
//    pts += param->isTestTX_PhaseNoise ?  : 0;
//    pts += param->isTestRF_RX ? RF_RXFreqRange.freqs.size() : 0;
//    pts += param->isTestIF_TX ? IF_TX_FILTER_160M_PTS : 0;
//    pts += param->isTestIF_RX ? IF_RX_FILTER_160M_PTS : 0;

//    TEST_THREAD_START("RF",pts);

    if (param->isTestTX_IOSw)       { totalResult = true; TX_IOSw(); }
    if (param->isTestTX_PwrModSw)   { totalResult = true; TX_PwrModSw(); }
    if (param->isTestTX_FilterSw)   { totalResult = true; TX_FilterSw(); }
    if (param->isTestTX_LOLD)       { totalResult = true; TX_LOLD(); }
    if (param->isTestTX_PhaseNoise) { totalResult = true; TX_PhaseNoise(); }
    if (param->isTestTX_NoiseFloor) { totalResult = true; TX_NoiseFloor(); }

    THREAD_ENDED
    RD_TEST_CATCH
}

void QTestR1CRFThread::TX_IOSw()
{
    const uint64_t freq = 2000000000;
    const double dds1 = 10e6;
    double pwr[2] = { 0.0 };
    bool res = true;
    int msgBoxExec = 0;
    tx_io_sw_test_data data;

    Instr.init();
    Instr.has_sa();
    Instr.sa_reset();
    Instr.sa_set_cf(double(freq) + dds1);
    Instr.sa_set_span(10e6);
    Instr.sa_set_rbw(1e3,true);
    Instr.sa_set_vbw(1e3,true);

    SP1401->set_io_mode(OUTPUT);
    SP1401_R1F->set_tx_att(10.0,10.0,10.0,10.0);
    SP1401->set_tx_freq(quint64(freq));
    SP2401->set_dds_src(sp2401_r1a::SINGLE_TONE);
    SP2401->set_dds1(dds1);
    SP2401->set_duc_dds(0.0);
    SP2401->set_tx_phase_rotate_I(0.0);
    SP2401->set_tx_amplitude_balance(8192,8192);
    SP2401->set_tx_dc_offset(0,0);
    SP2401->set_tx_filter_sw(false);
    SP1401->prepare_tr_tx_io_sw_test();

    THREAD_TEST_CANCEL

    connect(this,SIGNAL(testTXIOSwBox(QColor,QColor,int &)),
           g_MainW,SLOT(testTXIOSwBox(QColor,QColor,int &)),Qt::BlockingQueuedConnection);

    emit testTXIOSwBox(Qt::green,Qt::green,msgBoxExec);
    if (msgBoxExec == QDialog::Rejected) { res = false; }
    updateTotalResult(res);
    emit update(QModelIndex(),QModelIndex(),TI_TX_IO_SW,int(totalResult));

    Instr.sa_sweep_once();
    Instr.sa_set_peak_search(sa::PEAK_HIGHEST);
    Instr.sa_get_marker_pwr(pwr[0]);

    SP1401->set_io_mode(IO);
    msleep(50);

    THREAD_TEST_CANCEL
    emit testTXIOSwBox(Qt::red,Qt::green,msgBoxExec);
    if (msgBoxExec == QDialog::Rejected) { res = false; }
    updateTotalResult(res);
    emit update(QModelIndex(),QModelIndex(),TI_TX_IO_SW,int(totalResult));

    Instr.sa_sweep_once();
    Instr.sa_get_marker_pwr(pwr[1]);

    if (pwr[0] - pwr[1] < 30.0) {
        data.is_ok = res = false;
    }

    SP1401->set_io_mode(LOOP);
    msleep(50);

    THREAD_TEST_CANCEL
    emit testTXIOSwBox(Qt::red,Qt::red,msgBoxExec);
    if (msgBoxExec == QDialog::Rejected) { res = false; }
    updateTotalResult(res);
    emit update(QModelIndex(),QModelIndex(),TI_TX_IO_SW,int(totalResult));

    Instr.sa_sweep_once();
    Instr.sa_get_marker_pwr(pwr[1]);

    if (pwr[0] - pwr[1] < 30.0) {
        data.is_ok = res = false;
    }
    data.set_result(res ? test_common_data::PASS : test_common_data::FAIL);
    data.set_time();
    SP1401->set_io_mode(OUTPUT);
    SP1401->tr_tx_io_sw_test()->add(freq,data);
    SP1401->tr_tx_io_sw_test()->update();
    SP1401->ftp_put_tr_tx_io_sw_test();
}

void QTestR1CRFThread::TX_PwrModSw()
{
    const uint64_t freq = 2000000000;
    const double dds1 = 10e6;
    double pwr[2] = { 0.0 };
    bool res = true;
    tx_pwr_mod_sw_test_data data;

    Instr.init();
    Instr.has_sa();
    Instr.sa_reset();
    Instr.sa_set_cf(double(freq) + dds1);
    Instr.sa_set_span(10e6);
    Instr.sa_set_rbw(1e3,true);
    Instr.sa_set_vbw(1e3,true);

    SP1401->set_io_mode(OUTPUT);
    SP1401_R1F->set_tx_att(10.0,10.0,10.0,10.0);
    SP1401->set_pwr_en(true);
    SP1401->set_tx_modulator_en(true);
    SP1401->set_tx_freq(quint64(freq));
    SP2401->set_dds_src(sp2401_r1a::SINGLE_TONE);
    SP2401->set_dds1(dds1);
    SP2401->set_duc_dds(0.0);
    SP2401->set_tx_phase_rotate_I(0.0);
    SP2401->set_tx_amplitude_balance(8192,8192);
    SP2401->set_tx_dc_offset(0,0);
    SP2401->set_tx_filter_sw(false);
    SP1401->prepare_tr_tx_pwr_mod_sw_test();

    THREAD_TEST_CANCEL
    Instr.sa_sweep_once();
    Instr.sa_set_peak_search(sa::PEAK_HIGHEST);
    Instr.sa_get_marker_pwr(pwr[0]);

    SP1401->set_pwr_en(false);
    msleep(500);

    THREAD_TEST_CANCEL
    Instr.sa_sweep_once();
    Instr.sa_get_marker_pwr(pwr[1]);

    data.is_ok[0] = (pwr[0] - pwr[1] >= 30.0);

    SP1401->set_pwr_en(true);
    msleep(500);
    SP1401->set_tx_modulator_en(false);
    msleep(500);

    THREAD_TEST_CANCEL
    Instr.sa_sweep_once();
    Instr.sa_get_marker_pwr(pwr[1]);

    SP1401->set_tx_modulator_en(true);

    data.is_ok[1] = (pwr[0] - pwr[1] >= 30.0);
    res = data.is_ok[0] && data.is_ok[1];
    data.set_result(res ? test_common_data::PASS : test_common_data::FAIL);
    data.set_time();
    SP1401->tr_tx_pwr_mod_sw_test()->add(freq,data);
    updateTotalResult(res);

    emit update(QModelIndex(),QModelIndex(),TI_TX_PWR_MOD_SW,int(totalResult));

    SP1401->tr_tx_pwr_mod_sw_test()->update();
    SP1401->ftp_put_tr_tx_pwr_mod_sw_test();
}

void QTestR1CRFThread::TX_FilterSw()
{
    if (is_rf_ver_after(RFVer,R1F)) { return TX_FilterSw_R1F(); }
}

void QTestR1CRFThread::TX_FilterSw_R1F()
{
    const double dds1 = 10e6;
    uint64_t freq = 2000000000;
    double pwr[2] = { 0.0 };
    bool res = true;
    tx_filter_sw_test_data data;

    Instr.init();
    Instr.has_sa();
    Instr.sa_reset();
    Instr.sa_set_cf(double(freq) + dds1);
    Instr.sa_set_span(10e6);
    Instr.sa_set_rbw(1e3,true);
    Instr.sa_set_vbw(1e3,true);

    SP1401->set_io_mode(OUTPUT);
    SP1401_R1F->set_tx_att(10.0,10.0,10.0,10.0);
    SP1401->set_pwr_en(true);
    SP1401->set_tx_modulator_en(true);
    SP1401->set_tx_freq(quint64(freq));
    SP2401->set_dds_src(sp2401_r1a::SINGLE_TONE);
    SP2401->set_dds1(dds1);
    SP2401->set_duc_dds(0.0);
    SP2401->set_tx_phase_rotate_I(0.0);
    SP2401->set_tx_amplitude_balance(8192,8192);
    SP2401->set_tx_dc_offset(0,0);
    SP2401->set_tx_filter_sw(false);
    SP1401->prepare_tr_tx_filter_sw_test();

    THREAD_TEST_CANCEL
    Instr.sa_sweep_once();
    Instr.sa_set_peak_search(sa::PEAK_HIGHEST);
    Instr.sa_get_marker_pwr(pwr[0]);

    SP1401_R1F->set_tx_filter_sw(sp1401_r1f::BPF,sp1401_r1f::LPF);
    msleep(50);

    THREAD_TEST_CANCEL
    Instr.sa_sweep_once();
    Instr.sa_get_marker_pwr(pwr[1]);

    data.is_ok[0] = (pwr[0] - pwr[1] >= 30.0);

    freq = 6000000000;
    Instr.sa_set_cf(double(freq) + dds1);
    SP1401->set_tx_freq(quint64(freq));

    THREAD_TEST_CANCEL
    Instr.sa_sweep_once();
    Instr.sa_set_peak_search(sa::PEAK_HIGHEST);
    Instr.sa_get_marker_pwr(pwr[0]);

    SP1401_R1F->set_tx_filter_sw(sp1401_r1f::BPF,sp1401_r1f::LPF);
    msleep(50);

    THREAD_TEST_CANCEL
    Instr.sa_sweep_once();
    Instr.sa_get_marker_pwr(pwr[1]);

    data.is_ok[1] = (pwr[0] - pwr[1] >= 10.0);
    res = data.is_ok[0] && data.is_ok[1];
    data.set_result(res ? test_common_data::PASS : test_common_data::FAIL);
    data.set_time();
    SP1401->tr_tx_filter_sw_test()->add(freq,data);
    updateTotalResult(res);

    emit update(QModelIndex(),QModelIndex(),TI_TX_FILTER_SW,int(totalResult));

    SP1401->tr_tx_filter_sw_test()->update();
    SP1401->ftp_put_tr_tx_filter_sw_test();
}

void QTestR1CRFThread::TX_LOLD()
{
    const quint32 repeat = 2;
    bool res = true;
    quint64 freq = 0;
    uint64_t freqLO1 = 0;
    uint64_t freqLO2 = 0;
    uint64_t freqLO3 = 0;
    sp1401_r1c::tx_filter_t filter1;
    sp1401_r1c::tx_filter_t filter2;
    tx_lo_ld_test_data data;

    SP1401->set_io_mode(OUTPUT);
    SP1401->set_tx_freq(2000000000);
    SP1401_R1F->set_tx_att(10.0,10.0,10.0,10.0);
    SP2401->set_dds_src(sp2401_r1a::SINGLE_TONE);
    SP2401->set_dds1(0.0);
    SP2401->set_tx_filter_sw(false);
    SP1401->prepare_tr_tx_lo_ld_test();

    DECLARE_R1C_DETLOLOCK_FUNCTION_MAP
    for (quint32 i = 0;i < repeat;i ++) {
        THREAD_TEST_CANCEL
        for (freq = tx_freq_star;freq < tx_freq_stop;freq += 10000000) {
            THREAD_TEST_CANCEL
            SP1401_R1F->tx_freq2lo(freq,freqLO1,freqLO2,freqLO3,filter1,filter2);
            SP1401_R1F->init_hmc1197();
            SP1401_R1F->set_adf5355(sp1401_r1c::TX_LO_2,freqLO2);
            SP1401_R1F->set_adf5355(sp1401_r1c::TX_LO_3,freqLO3);
            SP1401_R1F->set_tx_filter_sw(filter1,filter2);
            msleep(10);
            (SP1401_R1F->*det_r1c_lo_lock[0])(data.is_locked[0]);
            (SP1401_R1F->*det_r1c_lo_lock[1])(data.is_locked[1]);
            (SP1401_R1F->*det_r1c_lo_lock[2])(data.is_locked[2]);

            for (quint32 j = 0;j < ARRAY_SIZE(data.is_locked);j ++) {
                res = res && data.is_locked[j];
            }

            data.set_result(res ? test_common_data::PASS : test_common_data::FAIL);
            data.set_time();
            SP1401->tr_tx_lo_ld_test()->add(int64_t(freq),data);
            updateTotalResult(res);

            emit update(QModelIndex(),QModelIndex(),TI_TX_LO_LD,int(totalResult));

            if (!res) { break; }
        }
        if (!res) { break; }
    }
    SP1401->tr_tx_lo_ld_test()->update();
    SP1401->ftp_put_tr_tx_lo_ld_test();
}

void QTestR1CRFThread::TX_PhaseNoise()
{
    Instr.init();
    Instr.has_sa();
    Instr.sa_reset();
    Instr.sa_set_vbw(1000.0,true);
    Instr.sa_set_ref(r1c::tx_base_pwr_op + 20.0);

    uint64_t Freq = 0;
    string freqStrOffset;
    string freqStrSpan;
    string freqStrRBW;
    vector<string> strOffset;
    vector<string> strSpan;
    vector<string> strRBW;
    double offset;
    double span;
    double rbw;
    double pwr[2] = { 0.0 };

    double att0 = 0.0;
    double att1 = 0.0;
    double att2 = 0.0;
    double att3 = 0.0;
    double d_gain = 0.0;

    bool res = true;

    range_freq_string freqString;
    range_freq<quint64> freqRange;

    spec::test_tx_phase_noise_freq(freqString);
    spec::test_tx_phase_noise_offset(freqStrOffset);
    spec::test_tx_phase_noise_span(freqStrSpan);
    spec::test_tx_phase_noise_rbw(freqStrRBW);

    parse_range_freq_string(freqString,freqRange);
    set_helper::split(freqStrOffset,set_helper::sections(freqStrOffset),strOffset);
    set_helper::split(freqStrSpan,set_helper::sections(freqStrSpan),strSpan);
    set_helper::split(freqStrRBW,set_helper::sections(freqStrRBW),strRBW);

    if (strOffset.size() != strSpan.size() || strSpan.size() != strRBW.size()) {
        RD_THROW rd::runtime_error("Invalid SA configuration");
    }

    tx_phase_noise_test_data data;

    if (is_rf_ver_between(RFVer,R1C,R1E)) {
        tx_pwr_op_table_r1c::guess_base_r1c(att0,att1,att2,att3,d_gain);
    } else if (is_rf_ver_after(RFVer,R1F)) {
        tx_pwr_op_table_r1c::guess_base_r1f(att0,att1,att2,d_gain);
    }

    SP1401->set_io_mode(OUTPUT);
    SP1401_R1F->set_tx_att(att0,att1,att2,att3);
    SP1401_R1F->set_rx_att_019_sw(r1c::RX_ATT_19);
    SP1401_R1F->set_rx_lna_att_sw(r1c::RX_ATT);
    SP1401_R1F->set_rx_att(30.0,30.0,30.0);
    SP2401->set_dds_src(sp2401_r1a::SINGLE_TONE);
    SP2401->set_tx_pwr_comp(d_gain);
    SP2401->set_dds1(0.0);
    SP2401->set_duc_dds(0.0);
    SP2401->set_tx_phase_rotate_I(0.0);
    SP2401->set_tx_amplitude_balance(8192,8192);
    SP2401->set_tx_dc_offset(0,0);
    SP2401->set_tx_filter_sw(false);
    SP1401->prepare_tr_tx_phase_noise_test();

    for (quint32 i = 0;i < freqRange.freqs.size();i ++) {
        THREAD_TEST_CANCEL

        Freq = freqRange.freqs.at(i);
        SP1401->set_tx_freq(Freq);
        msleep(50);

        Instr.sa_set_cf(Freq);
        Instr.sa_set_span(1e6);
        Instr.sa_set_rbw(1000.0,true);
        Instr.sa_set_avg_trace(false,1);
        Instr.sa_set_ref_auto();
        Instr.sa_set_avg_trace(true,20);

        res = true;
        for (quint32 j = 0;j < strOffset.size();j ++) {
            THREAD_TEST_CANCEL
            offset = freq_string_to_double(strOffset.at(j));
            span = freq_string_to_double(strSpan.at(j));
            rbw = freq_string_to_double(strRBW.at(j));
            Instr.sa_set_cf(Freq + span / 10.0);
            Instr.sa_set_span(span);
            Instr.sa_set_rbw(rbw,false);
            Instr.sa_sweep_once();
            Instr.sa_set_peak_search(sa::PEAK_HIGHEST);
            Instr.sa_get_marker_pwr(pwr[0]);
            Instr.sa_marker_move(Freq + offset);
            Instr.sa_get_marker_pwr(pwr[1]);
            if (j < ARRAY_SIZE(data.phase_noise)) {
                data.phase_noise[j] = pwr[1] - pwr[0] - 10.0 * log10(rbw);
            }
            if (data.phase_noise[j] > spec::test_tx_phase_noise(Freq,offset)) {
                res = false;
            }
        }
        data.set_result(res ? test_common_data::PASS : test_common_data::FAIL);
        data.set_time();
        SP1401->tr_tx_phase_noise_test()->add(int64_t(Freq),data);
        updateTotalResult(res);

        emit update(QModelIndex(),QModelIndex(),TI_TX_PHASE_NOISE,int(totalResult));
    }
    SP1401->tr_tx_phase_noise_test()->update();
    SP1401->ftp_put_tr_tx_phase_noise_test();
}

void QTestR1CRFThread::TX_NoiseFloor()
{
    Instr.init();
    Instr.has_sa();
    Instr.sa_reset();
    Instr.sa_set_vbw(1000.0,true);
    Instr.sa_set_avg_trace(true,20);
    Instr.sa_set_det(sa::DET_AVERAGE);

    quint64 Freq = 0;
    qint32 TXPwr[2] = { 10,-40 };
    string freqStrSpan;
    string freqStrRBW;
    QVector<string> strSpan;
    QVector<string> strRBW;
    double span = 0.0;
    double rbw = 0.0;
    double noise = 0.0;

    double att0 = 0.0;
    double att1 = 0.0;
    double att2 = 0.0;
    double att3 = 0.0;
    double d_gain = 0.0;

    bool res = true;

    range_freq_string freqString;
    range_freq<quint64> freqRange;

    spec::test_tx_noise_floor_freq(freqString);
    spec::test_tx_noise_floor_span(freqStrSpan);
    spec::test_tx_noise_floor_rbw(freqStrRBW);

    parse_range_freq_string(freqString,freqRange);
    set_helper::split(freqStrSpan,set_helper::sections(freqStrSpan),strSpan);
    set_helper::split(freqStrRBW,set_helper::sections(freqStrRBW),strRBW);

    if (strSpan.size() != 1 || strRBW.size() != 1) {
        RD_THROW rd::runtime_error("Invalid SA configuration");
    }

    span = freq_string_to_double(strSpan.at(0));
    rbw = freq_string_to_double(strRBW.at(0));

    tx_noise_floor_test_data data;
    tx_pwr_table_r1c::data_m_t dataBase;
    tx_pwr_table_r1c::data_m_t dataPwr[2];

    if (is_rf_ver_between(RFVer,R1C,R1E)) {
        tx_pwr_op_table_r1c::guess_base_r1c(att0,att1,att2,att3,d_gain);
        dataBase.att0 = short(att0 * 2);
        dataBase.att1 = short(att1 * 2);
        dataBase.att2 = short(att2 * 2);
        dataBase.att3 = short(att3 * 2);
        dataBase.d_gain = d_gain;
        tx_pwr_op_table_r1c::get_config_table_r1c(TXPwr[0],dataBase,&dataPwr[0]);
        tx_pwr_op_table_r1c::get_config_table_r1c(TXPwr[1],dataBase,&dataPwr[1]);
    } else if (is_rf_ver_after(RFVer,R1F)) {
        tx_pwr_op_table_r1c::guess_base_r1f(att0,att1,att2,d_gain);
        dataBase.att0 = short(att0 * 2);
        dataBase.att1 = short(att1 * 2);
        dataBase.att2 = short(att2 * 2);
        dataBase.d_gain = d_gain;
        tx_pwr_op_table_r1c::get_config_table_r1f(TXPwr[0],dataBase,&dataPwr[0]);
        tx_pwr_op_table_r1c::get_config_table_r1f(TXPwr[1],dataBase,&dataPwr[1]);
    }

    SP1401->set_io_mode(OUTPUT);
    SP1401_R1F->set_tx_att(30.0,30.0,30.0,30.0);
    SP1401_R1F->set_rx_att_019_sw(r1c::RX_ATT_19);
    SP1401_R1F->set_rx_lna_att_sw(r1c::RX_ATT);
    SP1401_R1F->set_rx_att(30.0,30.0,30.0);
    SP2401->set_dds_src(sp2401_r1a::ALL_0);
    SP2401->set_tx_pwr_comp(d_gain);
    SP2401->set_dds1(0.0);
    SP2401->set_duc_dds(0.0);
    SP2401->set_tx_phase_rotate_I(0.0);
    SP2401->set_tx_amplitude_balance(8192,8192);
    SP2401->set_tx_dc_offset(0,0);
    SP2401->set_tx_filter_sw(false);
    SP1401->prepare_tr_tx_noise_floor_test();

    Instr.sa_set_ref(-90.0);
    Instr.sa_set_span(span);
    Instr.sa_set_rbw(rbw,false);
    Instr.sa_set_en_preamp(true);
    Instr.sa_set_mech_att(false,0);

    for (quint32 i = 0;i < freqRange.freqs.size();i ++) {
        THREAD_TEST_CANCEL

        Freq = freqRange.freqs.at(i);
        SP1401->set_tx_freq(Freq);
        Instr.sa_set_cf(Freq + 11e6);
        msleep(50);

        res = true;
        for (quint32 j = 0;j < ARRAY_SIZE(dataPwr);j ++) {
            THREAD_TEST_CANCEL
            SP1401_R1F->set_tx_att(dataPwr[j].att0 / 2.0,dataPwr[j].att1 / 2.0,
                               dataPwr[j].att2 / 2.0,dataPwr[j].att3 / 2.0);
            msleep(50);
            Instr.sa_sweep_once();
            Instr.sa_set_peak_search(sa::PEAK_HIGHEST);
            Instr.sa_get_marker_pwr(noise);
            if (j < ARRAY_SIZE(data.noise_floor)) {
                data.noise_floor[j] = noise - 10.0 * log10(rbw);
            }
            if (data.noise_floor[j] > spec::test_tx_noise_floor(Freq,TXPwr[j])) {
                res = false;
            }
        }
        data.set_result(res ? test_common_data::PASS : test_common_data::FAIL);
        data.set_time();
        SP1401->tr_tx_noise_floor_test()->add(int64_t(Freq),data);
        updateTotalResult(res);

        emit update(QModelIndex(),QModelIndex(),TI_TX_NOISE_FLOOR,int(totalResult));
    }
    SP1401->tr_tx_noise_floor_test()->update();
    SP1401->ftp_put_tr_tx_noise_floor_test();
}
