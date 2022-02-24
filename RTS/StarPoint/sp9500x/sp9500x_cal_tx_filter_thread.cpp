#include "sp9500x_cal_tx_filter_thread.h"
#include "sp9500x_cal_tx_filter_childwidgets.h"
#include "cal_widget.h"
#include "spec.h"
#include "test_data.hpp"
#include "algo_math.hpp"
#include <QFile>

using namespace std;
using namespace ns_sp1403;
using namespace ns_sp2406;
using namespace ns_sp9500x;
using namespace NS_SP9500X;
using namespace set_helper;

void Q_Cal_TXFilter_Thread::cal()
{
    CAL_THREAD_START("TX Filter",(1));

//    setTempRange(2.0);

    Q_Cal_TXFilter_Widget *p = dynamic_cast<Q_Cal_TXFilter_Widget *>(parent());
    Q_TXFilter_Config_Delegate *delegate = p->childs->configDelegate;

    cal_TX0 = delegate->checkBoxTX0->isChecked();
    cal_TX1 = delegate->checkBoxTX1->isChecked();
    justRebuildCoef = delegate->checkBoxRebuildCoef->isChecked();

    cal_RF_IF_FR_0000_3000 = delegate->checkBoxCal_RF_IF_FR_0000_3000->isChecked();
    cal_RF_FR_0000_3000    = delegate->checkBoxCal_RF_FR_0000_3000->isChecked();
    cal_RF_FR_3000_4800    = delegate->checkBoxCal_RF_FR_3000_4800->isChecked();
    cal_RF_FR_4800_6000    = delegate->checkBoxCal_RF_FR_4800_6000->isChecked();
    cal_RF_FR_6000_7500    = delegate->checkBoxCal_RF_FR_6000_7500->isChecked();
    cal_IF_FR_3000_7500    = delegate->checkBoxCal_IF_FR_3000_7500->isChecked();

    freqString = p->ui->lineEditRFFreqs->text().toStdString();

    parse(freqString,freqRange);

    if (justRebuildCoef != true) {
        THREAD_CHECK_BOX("TX<===>Power Meter");

        Instr.init();
        Instr.has_pm();
        Instr.pm_reset();

        init();

        if (cal_RF_IF_FR_0000_3000) { THREAD_TEST_CANCEL; sweepRF_IF_0000_3000(); }

        if (cal_RF_FR_0000_3000)    { THREAD_TEST_CANCEL; sweepRF_0000_3000(); }

        if (cal_RF_FR_3000_4800)    { THREAD_TEST_CANCEL; sweepRF_3000_4800(); }

        if (cal_RF_FR_4800_6000)    { THREAD_TEST_CANCEL; sweepRF_4800_6000(); }

        if (cal_RF_FR_6000_7500)    { THREAD_TEST_CANCEL; sweepRF_6000_7500(); }

        if (cal_IF_FR_3000_7500)    { THREAD_TEST_CANCEL; sweepIF_3000_7500(); }
    }

    THREAD_TEST_CANCEL; generateFreqResponse();
    THREAD_TEST_CANCEL;
//    generateCoef();
//    THREAD_TEST_CANCEL
//    updateCalFile();

//    if (calParam.justRebuildCoef == true) {
//        SET_PROG_POS(tx_filter_freqs + dl_filter_160M_freqs);
//    }
}

void Q_Cal_TXFilter_Thread::init()
{
    SP2406->set_dl_src(dl_src_t::Disable);
    SP2406->set_dl_src(0,dl_src_t::DDS0);
    SP2406->set_dl_cpri_sr(dl_cpri_sr_t::_12288);
    SP2406->set_dl_lpf_en(false);
    SP2406->set_dl_hbf0_en(true);
    SP2406->set_dl_hbf1_en(true);
    SP2406->set_dl_cf_en(false);
    SP2406->set_dl_jesd_src(dl_jesd_src_t::DUC);
    SP2406->set_dl_pwr_comp(0);

    SP1403->set_io_mode(OUTPUT);

//    double att0 = 0.0;
//    double att1 = 0.0;
//    double att2 = 0.0;
//    double att3 = 0.0;
//    double d_gain = -5.0;

//    if (RFVer == R1C || RFVer == R1D || RFVer == R1E) {
//        tx_pwr_op_table_r1c::guess_base_r1c(att0,att1,att2,att3,d_gain);
//    } else if (RFVer == R1F) {
//        tx_pwr_op_table_r1c::guess_base_r1f(att0,att1,att2,d_gain);
//    }

//    SP1401->set_tx_att(att0,att1,att2,att3);
//    SP1401->set_rx_lna_att_sw(r1c::RX_ATT);
//    SP1401->set_rx_att_019_sw(r1c::RX_ATT_19);
//    SP1401->set_rx_att(31.0,31.0,31.0);
    msleep(10);
}

void Q_Cal_TXFilter_Thread::sweepRF_IF_0000_3000()
{
    cal_table_t table = cal_table_t::TX_RF_IF_FR_0000_3000;
    tx_rf_if_fr_0000_3000_table_t::data_f_t data;
    uint64_t freq = 0;

    array<int64_t,4> freq_dds1  = {FREQ_M(-150),FREQ_M(-50),FREQ_M(50),FREQ_M(150)};

    int64_t freq_if = 0;
    int64_t freq_if_star = FREQ_M(-49);
    int64_t freq_if_stop = FREQ_M(49);

    quint32 idx_freq_if = 0;

    SP2406->set_dl_dds0(0,0);

    for (set<uint64_t>::iterator iter = freqRange.freq.begin();iter != freqRange.freq.end();++iter) {
        THREAD_TEST_CANCEL

        freq = *iter;

        data.freq = freq;
        SP1403->set_tx_freq(freq);

        idx_freq_if = 0;
        for (quint32 i = 0;i < freq_dds1.size();++i) {
            THREAD_TEST_CANCEL
            SP2406->set_dl_dds1(0,freq_dds1[i]);

            for (freq_if = freq_if_star;freq_if <= freq_if_stop;freq_if += FREQ_M(2)) {
                THREAD_TEST_PAUSE_S
                THREAD_TEST_CANCEL
                SP2406->set_dl_src_dds0_freq(freq_if);
                msleep(10);

                data.pts[idx_freq_if].x = uint64_t(int64_t(freq) + freq_dds1[i] + freq_if);
                Instr.pm_set_freq(data.pts[idx_freq_if].x);
                Instr.pm_get_pwr(data.pts[idx_freq_if].y);
                Log.stdprintf("%d freq : %lld\n",idx_freq_if,data.pts[idx_freq_if].x);

                idx_freq_if ++;

                THREAD_TEST_PAUSE_E
            }
        }

//        SP1403->get_tx_avg_temp(tempCur);
//        THREAD_CHECK_TEMP;

        SP1403->cal_file()->add(table,&data);
//        SP1403->cal_file()->persistence(table);

        emit uiUpdate(0,0,table);
//        ADD_PROG_POS(1);
    }
    SP1403->cal_file()->persistence(table);
}

void Q_Cal_TXFilter_Thread::sweepRF_0000_3000()
{
    cal_table_t table = cal_table_t::TX_RF_FR_0000_3000;
    tx_rf_fr_0000_3000_table_t::data_f_t data;
    uint64_t freq = 0;

    for (set<uint64_t>::iterator iter = freqRange.freq.begin();iter != freqRange.freq.end();++iter) {
        THREAD_TEST_PAUSE_S
        THREAD_TEST_CANCEL

        freq = *iter;
        SP1403->set_tx_freq(freq);
        SP1403_R1B->set_tx1_sw1(ns_sp9500x::sp1403_r1b::tx_sw1_t::_300_3000);
        SP1403_R1B->set_tx1_sw2(ns_sp9500x::sp1403_r1b::tx_sw2_t::_6000_8000);
        SP1403_R1B->set_tx1_sw3(ns_sp9500x::sp1403_r1b::tx_sw3_t::_6000_8000);
        SP1403_R1B->set_tx1_sw4(ns_sp9500x::sp1403_r1b::tx_sw4_t::_3000_4800);
        msleep(10);

        Instr.pm_get_pwr(freq,data.pts[0].y);

//        SP1403->get_tx_avg_temp(tempCur);
//        THREAD_CHECK_TEMP;

        data.freq = freq;
        SP1403->cal_file()->add(table,&data);

        emit uiUpdate(0,0,table);

//        ADD_PROG_POS(1);
        THREAD_TEST_PAUSE_E
    }
    SP1403->cal_file()->persistence(table);
}

void Q_Cal_TXFilter_Thread::sweepRF_3000_4800()
{
    cal_table_t table = cal_table_t::TX_RF_FR_3000_4800;
    tx_rf_fr_3000_4800_table_t::data_f_t data;
    uint64_t freq = 0;

    BW_Max = FREQ_M(800);

    for (freq = FREQ_M(3000) - BW_Max / 2;freq < FREQ_M(4800) + BW_Max / 2;freq += FREQ_M(2)) {
        THREAD_TEST_PAUSE_S
        THREAD_TEST_CANCEL

        SP1403->set_tx_freq(freq);
        SP1403_R1B->set_tx1_sw1(ns_sp9500x::sp1403_r1b::tx_sw1_t::_3000_8000);
        SP1403_R1B->set_tx1_sw2(ns_sp9500x::sp1403_r1b::tx_sw2_t::_3000_6000);
        SP1403_R1B->set_tx1_sw3(ns_sp9500x::sp1403_r1b::tx_sw3_t::_3000_6000);
        SP1403_R1B->set_tx1_sw4(ns_sp9500x::sp1403_r1b::tx_sw4_t::_3000_4800);
        msleep(10);

        Instr.pm_get_pwr(freq,data.pts[0].y);

//        SP1403->get_tx_avg_temp(tempCur);
//        THREAD_CHECK_TEMP;

        data.freq = freq;
        SP1403->cal_file()->add(table,&data);

        emit uiUpdate(0,0,table);

        ADD_PROG_POS(1);
        THREAD_TEST_PAUSE_E
    }
    SP1403->cal_file()->persistence(table);
}

void Q_Cal_TXFilter_Thread::sweepRF_4800_6000()
{
    cal_table_t table = cal_table_t::TX_RF_FR_4800_6000;
    tx_rf_fr_4800_6000_table_t::data_f_t data;
    uint64_t freq = 0;

    BW_Max = FREQ_M(800);

    for (freq = FREQ_M(4800) - BW_Max / 2;freq < FREQ_M(6000) + BW_Max / 2;freq += FREQ_M(2)) {
        THREAD_TEST_PAUSE_S
        THREAD_TEST_CANCEL

        SP1403->set_tx_freq(freq);
        SP1403_R1B->set_tx1_sw1(ns_sp9500x::sp1403_r1b::tx_sw1_t::_3000_8000);
        SP1403_R1B->set_tx1_sw2(ns_sp9500x::sp1403_r1b::tx_sw2_t::_3000_6000);
        SP1403_R1B->set_tx1_sw3(ns_sp9500x::sp1403_r1b::tx_sw3_t::_3000_6000);
        SP1403_R1B->set_tx1_sw4(ns_sp9500x::sp1403_r1b::tx_sw4_t::_4800_6000);
        msleep(10);

        Instr.pm_get_pwr(freq,data.pts[0].y);

//        SP1403->get_tx_avg_temp(tempCur);
//        THREAD_CHECK_TEMP;

        data.freq = freq;
        SP1403->cal_file()->add(table,&data);

        emit uiUpdate(0,0,table);

        ADD_PROG_POS(1);
        THREAD_TEST_PAUSE_E
    }
    SP1403->cal_file()->persistence(table);
}

void Q_Cal_TXFilter_Thread::sweepRF_6000_7500()
{
    cal_table_t table = cal_table_t::TX_RF_FR_6000_7500;
    tx_rf_fr_6000_7500_table_t::data_f_t data;
    uint64_t freq = 0;

    BW_Max = FREQ_M(800);

    for (freq = FREQ_M(6000) - BW_Max / 2;freq < FREQ_M(7500) + BW_Max / 2;freq += FREQ_M(2)) {
        THREAD_TEST_PAUSE_S
        THREAD_TEST_CANCEL

        SP1403->set_tx_freq(freq);
        SP1403_R1B->set_tx1_sw1(ns_sp9500x::sp1403_r1b::tx_sw1_t::_3000_8000);
        SP1403_R1B->set_tx1_sw2(ns_sp9500x::sp1403_r1b::tx_sw2_t::_6000_8000);
        SP1403_R1B->set_tx1_sw3(ns_sp9500x::sp1403_r1b::tx_sw3_t::_6000_8000);
        SP1403_R1B->set_tx1_sw4(ns_sp9500x::sp1403_r1b::tx_sw4_t::_4800_6000);
        msleep(10);

        Instr.pm_get_pwr(freq,data.pts[0].y);

//        SP1403->get_tx_avg_temp(tempCur);
//        THREAD_CHECK_TEMP;

        data.freq = freq;
        SP1403->cal_file()->add(table,&data);

        emit uiUpdate(0,0,table);

        ADD_PROG_POS(1);
        THREAD_TEST_PAUSE_E
    }
    SP1403->cal_file()->persistence(table);
}

void Q_Cal_TXFilter_Thread::sweepIF_3000_7500()
{
    cal_table_t table = cal_table_t::TX_IF_FR_3000_7500;
    tx_if_fr_3000_7500_table_t::data_f_t data;
    uint64_t freq_rf = FREQ_M(7500);
    int64_t  freq_dds0[4]  = {FREQ_M(-150),FREQ_M(-50),FREQ_M(50),FREQ_M(150)};
    int64_t  freq_dds0_cur = 0;
    int64_t  freq_dds1[2]  = {FREQ_M(-200),FREQ_M(200)};
    int64_t  freq_dds1_cur = 0;

    int64_t  freq_if = 0;
    int64_t  freq_if_star = FREQ_M(-49);
    int64_t  freq_if_stop = FREQ_M(49);

    quint32 idx_freq_if = 0;

    BW_Max = FREQ_M(800);

    data.freq = 0;
    SP1403->set_tx_freq(freq_rf);
    msleep(10);

    for (quint32 i = 0;i < ARRAY_SIZE(freq_dds1);++i) {
        freq_dds1_cur = freq_dds1[i];
        SP2406->set_dl_dds1(0,freq_dds1_cur);

        for (quint32 j = 0;j < ARRAY_SIZE(freq_dds0);++j) {
            THREAD_TEST_CANCEL
            freq_dds0_cur = freq_dds0[j];
            SP2406->set_dl_dds0(0,freq_dds0_cur);

            for (freq_if = freq_if_star;freq_if <= freq_if_stop;freq_if += FREQ_M(2)) {
                THREAD_TEST_PAUSE_S
                THREAD_TEST_CANCEL
                SP2406->set_dl_src_dds0_freq(freq_if);

                data.pts[idx_freq_if].x = freq_dds0_cur + freq_dds1_cur + freq_if;
                Instr.pm_set_freq(data.pts[idx_freq_if].x);
                Instr.pm_get_pwr(data.pts[idx_freq_if].y);
                Log.stdprintf("%d %lld %f\n",idx_freq_if,data.pts[idx_freq_if].x,data.pts[idx_freq_if].y);
                SP1403->cal_file()->add(table,&data);

                idx_freq_if ++;

                emit uiUpdate(0,0,table);
                ADD_PROG_POS(1);
                THREAD_TEST_PAUSE_E
            }
        }
    }
    SP1403->cal_file()->persistence(table);
    SP1403->cal_file()->tx_if_fr_3000_7500()->save_as("c:\\if_fr_3000_7500.txt");
}

void Q_Cal_TXFilter_Thread::generateFreqResponse()
{
    if (cal_RF_IF_FR_0000_3000) {
        SP1403->cal_file()->tx_rf_if_fr_0000_3000_table()->save_as("C:\\1.txt");
    }
    if (cal_RF_FR_0000_3000) {
        SP1403->cal_file()->tx_rf_fr_0000_3000()->save_as("C:\\2.txt");
    }
}

void Q_Cal_TXFilter_Thread::generateCoef()
{

}

void Q_Cal_TXFilter_Thread::updateCalFile()
{

}

void Q_Cal_TXFilter_Thread::check()
{

}


void Q_Exp_TXFilter_Thread::run()
{
    initProgress("Exporting TX Filter",100);
    addProgressPos(100);
    THREAD_ENDED
}
