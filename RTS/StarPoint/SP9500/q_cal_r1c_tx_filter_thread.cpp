#include "q_cal_r1c_tx_filter_thread.h"
#include "spec.h"
#include "test_data.hpp"
#include <QFile>

#ifdef RD_PLATFORM_WIN32
    #include <Windows.h>
#endif

using namespace ns_sp1401;
using namespace ns_sp2401;

void QCalR1CTXFilterThread::cal()
{
    CAL_THREAD_START("TX Filter",(tx_filter_freqs + dl_filter_160M_freqs));

    Qwt_TX_RF_FR_Data *dataRF_FR_0 = dynamic_cast<Qwt_TX_RF_FR_Data *>(calParam.plotData_0);
    Qwt_TX_RF_FR_Data *dataRF_FR_1 = dynamic_cast<Qwt_TX_RF_FR_Data *>(calParam.plotData_1);
    Qwt_TX_IF_FR_Data *dataIF_FR   = dynamic_cast<Qwt_TX_IF_FR_Data *>(calParam.plotData_2);

    // 80M : 100M-43M ~~~ 6G+43M,step 2M.
    // 160M: 100M-83M ~~~ 6G+83M,step 2M.

    setTempRange(2.0);

    if (calParam.justRebuildCoef != true) {
        THREAD_CHECK_BOX("TX<===>Power Meter");

        Instr.init();
        Instr.has_pm();
        Instr.pm_reset();

        initTXChain();
        THREAD_TEST_CANCEL
        sweepRF_0(dataRF_FR_0);
        THREAD_TEST_CANCEL
        sweepRF_1(dataRF_FR_1);
        THREAD_TEST_CANCEL
        sweepIF(dataIF_FR);
    }
    THREAD_TEST_CANCEL
    generateFreqResponse();
    THREAD_TEST_CANCEL
    generateCoef();
    THREAD_TEST_CANCEL
    updateCalFile();

    if (calParam.justRebuildCoef == true) {
        SET_PROG_POS(tx_filter_freqs + dl_filter_160M_freqs);
    }
}

void QCalR1CTXFilterThread::initTXChain()
{
    tx_lol_table_r1cd::data_m_t dataLOL;
    tx_sb_table_r1cd::data_m_t dataSB;
    SP1401->cf()->m_tx_lol->get(2000000000,&dataLOL);
    SP1401->cf()->m_tx_sb->get(2000000000,&dataSB);
    SP2401->set_tx_dc_offset(uint16_t(dataLOL.dc_i),uint16_t(dataLOL.dc_q));
    SP2401->set_tx_phase_rotate_I(double(dataSB.th));
    SP2401->set_tx_amplitude_balance(dataSB.am_i,dataSB.am_q);
    SP2401->set_dds_src(sp2401_r1a::SINGLE_TONE);
    SP2401->set_dds1(0.0);
    SP2401->set_duc_dds(0.0);
    SP2401->set_tx_filter_sw(false);

    double att0 = 0.0;
    double att1 = 0.0;
    double att2 = 0.0;
    double att3 = 0.0;
    double d_gain = -5.0;

    if (RFVer == R1C || RFVer == R1D || RFVer == R1E) {
        tx_pwr_op_table_r1c::guess_base_r1c(att0,att1,att2,att3,d_gain);
    } else if (RFVer == R1F) {
        tx_pwr_op_table_r1c::guess_base_r1f(att0,att1,att2,d_gain);
    }

    SP2401->set_tx_pwr_comp(d_gain);
    SP1401->set_io_mode(rd::OUTPUT);
    SP1401->set_tx_att(att0,att1,att2,att3);
    SP1401->set_rx_lna_att_sw(r1c::RX_ATT);
    SP1401->set_rx_att_019_sw(r1c::RX_ATT_19);
    SP1401->set_rx_att(31.0,31.0,31.0);
    msleep(10);
}

void QCalR1CTXFilterThread::sweepRF_0(Qwt_TX_RF_FR_Data *qwtData)
{
    tx_rf_fr_table::data_f_t data;
    quint64 freq = 0;

    for (quint32 i = 0;i < tx_filter_freqs_0;i ++) {
        THREAD_TEST_PAUSE_S
        THREAD_TEST_CANCEL

        data.freq = tx_filter_freq_star_0 + i * tx_filter_freq_step;
        freq = data.freq < tx_freq_star ? tx_freq_star : data.freq;
        SP1401->set_tx_freq(freq);
        SP1401->set_tx_filter_sw(sp1401_r1c::LPF,sp1401_r1c::LPF);
        msleep(10);

        Instr.pm_get_pwr(double(freq),data.pwr);

        SP1401->get_tx_avg_temp(tempCur);
        THREAD_CHECK_TEMP;

        SP1401->cf()->add(cal_file::TX_RF_FR_0,&data);
        qwtData->calTable()->append(data);
        emit update(QModelIndex(),QModelIndex(),cal_file::TX_RF_FR_0);

        SET_PROG_POS(i + 1);
        THREAD_TEST_PAUSE_E
    }
    SP1401->cf()->w(cal_file::TX_RF_FR_0);
    SP1401->cf()->m_tx_rf_fr_0->save_as("c:\\tx_filter_rf_fr_0.txt");
}

void QCalR1CTXFilterThread::sweepRF_1(Qwt_TX_RF_FR_Data *qwtData)
{
    tx_rf_fr_table::data_f_t data;
    quint64 freq = 0;

    for (quint32 i = 0;i < tx_filter_freqs_1;i ++) {
        THREAD_TEST_PAUSE_S
        THREAD_TEST_CANCEL
        data.freq = tx_filter_freq_star_1 + i * tx_filter_freq_step;
        freq = data.freq > tx_freq_stop ? tx_freq_stop : data.freq;
        SP1401->set_tx_freq(freq);
        SP1401->set_tx_filter_sw(sp1401_r1c::BPF,sp1401_r1c::BPF);
        msleep(10);

        Instr.pm_get_pwr(double(freq),data.pwr);

        SP1401->get_tx_avg_temp(tempCur);
        THREAD_CHECK_TEMP;

        SP1401->cf()->add(cal_file::TX_RF_FR_1,&data);
        qwtData->calTable()->append(data);
        emit update(QModelIndex(),QModelIndex(),cal_file::TX_RF_FR_1);

        SET_PROG_POS(tx_filter_freqs_0 + i + 1);
        THREAD_TEST_PAUSE_E
    }
    SP1401->cf()->w(cal_file::TX_RF_FR_1);
    SP1401->cf()->m_tx_rf_fr_1->save_as("c:\\tx_filter_rf_fr_1.txt");
}

void QCalR1CTXFilterThread::sweepIF(Qwt_TX_IF_FR_Data *qwtData)
{
    tx_if_fr_table::data_f_t data;

    SP1401->set_tx_freq(2000000000);
    msleep(10);

    for (quint32 i = 0;i < dl_filter_160M_freqs;i ++) {
        THREAD_TEST_PAUSE_S
        THREAD_TEST_CANCEL
        data.freq = dl_filter_160M_freq_star + i * dl_filter_freq_step;
        SP2401->set_duc_dds(data.freq);
        msleep(10);

        Instr.pm_get_pwr(2e9 + data.freq,data.pwr);

        SP1401->get_tx_avg_temp(tempCur);
        THREAD_CHECK_TEMP;

        SP1401->cf()->add(cal_file::TX_IF_FR,&data);
        qwtData->calTable()->append(data);
        emit update(QModelIndex(),QModelIndex(),cal_file::TX_IF_FR);

        SET_PROG_POS(tx_filter_freqs + i + 1);
        THREAD_TEST_PAUSE_E
    }
    SP1401->cf()->w(cal_file::TX_IF_FR);
    SP1401->cf()->m_tx_if_fr->save_as("c:\\tx_filter_if_fr.txt");
}

void QCalR1CTXFilterThread::generateFreqResponse()
{
    quint64 freqRF = 0;
    quint64 freqStarRF = 0;
    quint64 freqStopRF = 0;
    quint64 freqStepRF = 0;
    quint64 freqCalled = 0;
    quint64 freqStarRF_InBw = 0;
    quint64 freqStopRF_InBw = 0;
    quint64 freqStepRF_InBw = 0;

    double freqIF = 0.0;
    double freqStarIF = 0.0;
    double freqStopIF = 0.0;
    double freqStepIF = 0.0;

    quint32 idx = 0;

    //80M
    FILE *fp_fr = fopen("C:\\CSECal\\tx_filter_80.txt","w");
    if (fp_fr == nullptr) {
        QString msg = QString("Could not open file");
        THREAD_ERROR_BOX(msg);
        THREAD_ENDED
    }

    // (2G-43M)~~~(2G+43M),step 2M.
    tx_rf_fr_table::data_f_t tempDataRFFr_80[dl_filter_80M_freqs];
    // -43M~~~43M,step 2M.
    tx_if_fr_table::data_f_t tempDataIFFr_80[dl_filter_80M_freqs];

    SP1401->cf()->set_bw(_80M);
    SP1401->cf()->map2buf(cal_file::TX_RF_FR_0);
    freqStarRF = quint64(2e9 + dl_filter_80M_freq_star);
    freqStopRF = quint64(2e9 + dl_filter_80M_freq_stop);
    freqStepRF = quint64(dl_filter_freq_step);
    for (freqRF = freqStarRF;freqRF <= freqStopRF;freqRF += freqStepRF) {
        SP1401->cf()->m_tx_rf_fr_0->get(freqRF,&tempDataRFFr_80[idx]);
        idx ++;
    }

    idx = 0;
    SP1401->cf()->map2buf(cal_file::TX_IF_FR);
    freqStarIF = dl_filter_80M_freq_star;
    freqStopIF = dl_filter_80M_freq_stop;
    freqStepIF = dl_filter_freq_step;
    for (freqIF = freqStarIF;freqIF <= freqStopIF;freqIF += freqStepIF) {
        SP1401->cf()->m_tx_if_fr->get(freqIF,&tempDataIFFr_80[idx]);
        idx ++;
    }

    // Real IF frequency response,-43M~~~43M.
    for (idx = 0;idx < dl_filter_80M_freqs;idx ++) {
        tempDataIFFr_80[idx].pwr -= tempDataRFFr_80[idx].pwr;
    }

    SP1401->cf()->map2buf(cal_file::TX_RF_FR_0);
    freqStarRF = tx_freq_star;
    freqStopRF = 3100000000;
    freqStepRF = tx_freq_step_called;
    for (freqRF = freqStarRF;freqRF < freqStopRF;freqRF += freqStepRF) {
        freqCalled = freqRF < 100000000 ? 100000000 : freqRF;
        idx = 0;
        freqStarRF_InBw = quint64(freqCalled + dl_filter_80M_freq_star);
        freqStopRF_InBw = quint64(freqCalled + dl_filter_80M_freq_stop);
        freqStepRF_InBw = quint64(dl_filter_freq_step);
        for (quint64 freq = freqStarRF_InBw;freq <= freqStopRF_InBw;freq += freqStepRF_InBw) {
            SP1401->cf()->m_tx_rf_fr_0->get(freq,&tempDataRFFr_80[idx]);
            fprintf(fp_fr,"%.6f ",tempDataRFFr_80[idx].pwr + tempDataIFFr_80[idx].pwr);
            idx ++;
        }
        fprintf(fp_fr,"\n");
    }

    SP1401->cf()->map2buf(cal_file::TX_RF_FR_1);
    freqStarRF = 3100000000;
    freqStopRF = tx_freq_stop;
    freqStepRF = tx_freq_step_called;
    for (freqRF = freqStarRF;freqRF <= freqStopRF;freqRF += freqStepRF) {
        idx = 0;
        freqStarRF_InBw = quint64(freqRF + dl_filter_80M_freq_star);
        freqStopRF_InBw = quint64(freqRF + dl_filter_80M_freq_stop);
        freqStepRF_InBw = quint64(dl_filter_freq_step);
        for (quint64 freq = freqStarRF_InBw;freq <= freqStopRF_InBw;freq += freqStepRF_InBw) {
            SP1401->cf()->m_tx_rf_fr_1->get(freq,&tempDataRFFr_80[idx]);
            fprintf(fp_fr,"%.6f ",tempDataRFFr_80[idx].pwr + tempDataIFFr_80[idx].pwr);
            idx ++;
        }
        fprintf(fp_fr,"\n");
    }
    fclose(fp_fr);
    fp_fr = nullptr;

    //160M
    fp_fr = fopen("C:\\CSECal\\tx_filter_160.txt","w");
    if (fp_fr == nullptr) {
        QString msg = QString("could not open file");
        THREAD_ERROR_BOX(msg);
        THREAD_ENDED
    }

    // (2G-83M)~~~(2G+83M),step 2M.
    tx_rf_fr_table::data_f_t tempDataRFFr_160[dl_filter_160M_freqs];
    // -83M~~~83M,step 2M.
    tx_if_fr_table::data_f_t tempDataIFFr_160[dl_filter_160M_freqs];

    idx = 0;
    SP1401->cf()->set_bw(_160M);
    SP1401->cf()->map2buf(cal_file::TX_RF_FR_0);
    freqStarRF = quint64(2000000000.0 + dl_filter_160M_freq_star);
    freqStopRF = quint64(2000000000.0 + dl_filter_160M_freq_stop);
    freqStepRF = quint64(dl_filter_freq_step);
    for (freqRF = freqStarRF;freqRF <= freqStopRF;freqRF += freqStepRF) {
        SP1401->cf()->m_tx_rf_fr_0->get(freqRF,&tempDataRFFr_160[idx]);
        idx ++;
    }

    idx = 0;
    SP1401->cf()->map2buf(cal_file::TX_IF_FR);
    freqStarIF = dl_filter_160M_freq_star;
    freqStopIF = dl_filter_160M_freq_stop;
    freqStepIF = dl_filter_freq_step;
    for (freqIF = freqStarIF;freqIF <= freqStopIF;freqIF += freqStepIF) {
        SP1401->cf()->m_tx_if_fr->get(freqIF,&tempDataIFFr_160[idx]);
        idx ++;
    }

    // Real IF frequency response,-83M~~~83M.
    for (idx = 0;idx < dl_filter_160M_freqs;idx ++) {
        tempDataIFFr_160[idx].pwr -= tempDataRFFr_160[idx].pwr;
    }

    SP1401->cf()->map2buf(cal_file::TX_RF_FR_0);
    freqStarRF = tx_freq_star;
    freqStopRF = 3100000000;
    freqStepRF = tx_freq_step_called;
    freqCalled = 0;
    for (freqRF = freqStarRF;freqRF < freqStopRF;freqRF += freqStepRF) {
        freqCalled = freqRF < 100000000 ? 100000000 : freqRF;
        idx = 0;
        freqStarRF_InBw = quint64(freqCalled + dl_filter_160M_freq_star);
        freqStopRF_InBw = quint64(freqCalled + dl_filter_160M_freq_stop);
        freqStepRF_InBw = quint64(dl_filter_freq_step);
        for (quint64 freq = freqStarRF_InBw;freq <= freqStopRF_InBw;freq += freqStepRF_InBw) {
            SP1401->cf()->m_tx_rf_fr_0->get(freq,&tempDataRFFr_160[idx]);
            fprintf(fp_fr,"%.6f ",tempDataRFFr_160[idx].pwr + tempDataIFFr_160[idx].pwr);
            idx ++;
        }
        fprintf(fp_fr,"\n");
    }
    SP1401->cf()->map2buf(cal_file::TX_RF_FR_1);
    freqStarRF = 3100000000;
    freqStopRF = tx_freq_stop;
    freqStepRF = tx_freq_step_called;
    for (freqRF = freqStarRF;freqRF <= freqStopRF;freqRF += freqStepRF) {
        idx = 0;
        freqStarRF_InBw = quint64(freqRF + dl_filter_160M_freq_star);
        freqStepRF_InBw = quint64(freqRF + dl_filter_160M_freq_stop);
        freqStopRF_InBw = quint64(dl_filter_freq_step);
        for (quint64 freq = freqStarRF_InBw;freq <= freqStepRF_InBw;freq += freqStopRF_InBw) {
            SP1401->cf()->m_tx_rf_fr_1->get(freq,&tempDataRFFr_160[idx]);
            fprintf(fp_fr,"%.6f ",tempDataRFFr_160[idx].pwr + tempDataIFFr_160[idx].pwr);
            idx ++;
        }
        fprintf(fp_fr,"\n");
    }
    fclose(fp_fr);
    fp_fr = nullptr;
}

void QCalR1CTXFilterThread::generateCoef()
{
#ifdef RD_PLATFORM_WIN32
    char firExePath[MAX_PATH] = {0};
    ZERO_ARRAY(firExePath);

    ::GetModuleFileNameA(nullptr,firExePath,MAX_PATH);
    *strrchr(firExePath,'\\') = 0;
    strcat(firExePath,"\\SP9500\\tx_filter.exe");

    if (exeFirProcess(firExePath)) {
        QString msg = QString("%1").arg(Log.last_err());
        THREAD_ERROR_BOX(msg);
        THREAD_ENDED
    }
#else
#endif
}

void QCalR1CTXFilterThread::updateCalFile()
{
    char pathReal[MAX_PATH];
    char pathImag[MAX_PATH];

    FILE *fp_real = nullptr;
    FILE *fp_imag = nullptr;

    tx_filter_80m_table::data_f_t  tempData_80;
    tx_filter_160m_table::data_f_t tempData_160;

    QTXFilterModel *model_80  = dynamic_cast<QTXFilterModel *>(calParam.model_0);
    QTXFilterModel *model_160 = dynamic_cast<QTXFilterModel *>(calParam.model_1);

    quint32 idx = 0;

    // 80M
    sprintf(pathReal,"C:\\CSECal\\tx_filter_80_coef_real.txt");
    sprintf(pathImag,"C:\\CSECal\\tx_filter_80_coef_imag.txt");
    fp_real = fopen(pathReal,"r");
    fp_imag = fopen(pathImag,"r");
    if (fp_real == nullptr) {
        Log.set_last_err("Could not open %s",pathReal);
        QString msg = QString("%1").arg(Log.last_err());
        THREAD_ERROR_BOX(msg);
        THREAD_ENDED
    }
    if (fp_imag == nullptr) {
        Log.set_last_err("Could not open %s",pathImag);
        QString msg = QString("%1").arg(Log.last_err());
        THREAD_ERROR_BOX(msg);
        THREAD_ENDED
    }

    for (quint64 freq = tx_freq_star;freq <= tx_freq_stop;freq += tx_freq_step_called) {
        ZERO_OBJ(tempData_80);
        for (quint32 i = 0;i < dl_filter_tap_2i;i ++) {
            fscanf(fp_real,"%hd,",&(tempData_80.coef_real[i]));
        }
        fscanf(fp_real,"\n");

        for (quint32 i = 0;i < dl_filter_tap_2i;i ++) {
            fscanf(fp_imag,"%hd,",&(tempData_80.coef_imag[i]));
        }
        fscanf(fp_imag,"\n");

        tempData_80.freq = freq;
        SP1401->get_temp(4,tempData_80.temp[0]);
        SP1401->get_temp(5,tempData_80.temp[1]);
        SP1401->get_temp(6,tempData_80.temp[2]);
        SP1401->get_temp(7,tempData_80.temp[3]);
        tempData_80.time = getCurTime();

        model_80->calTable()->replace(idx,tempData_80);
        SP1401->cf()->add(cal_file::TX_FILTER_80,&tempData_80);
        emit update(model_80->index(idx,0),model_80->index(idx,7),cal_file::TX_FILTER_80);
        idx ++;
    }
    SP1401->cf()->w(cal_file::TX_FILTER_80);
    fclose(fp_real);
    fclose(fp_imag);
    fp_real = nullptr;
    fp_imag = nullptr;

    QFile::remove(pathReal);
    QFile::remove(pathImag);

    // 160M
    sprintf(pathReal,"C:\\CSECal\\tx_filter_160_coef_real.txt");
    sprintf(pathImag,"C:\\CSECal\\tx_filter_160_coef_imag.txt");
    fp_real = fopen(pathReal,"r");
    fp_imag = fopen(pathImag,"r");
    if (fp_real == nullptr) {
        Log.set_last_err("Could not open %s",pathReal);
        QString msg = QString("%1").arg(Log.last_err());
        THREAD_ERROR_BOX(msg);
        THREAD_ENDED
    }
    if (fp_imag == nullptr) {
        Log.set_last_err("Could not open %s",pathImag);
        QString msg = QString("%1").arg(Log.last_err());
        THREAD_ERROR_BOX(msg);
        THREAD_ENDED
    }

    idx = 0;
    for (quint64 freq = tx_freq_star;freq <= tx_freq_stop;freq += tx_freq_step_called) {
        ZERO_OBJ(tempData_160);
        for (quint32 i = 0;i < dl_filter_tap_2i;i ++) {
            fscanf(fp_real,"%hd,",&(tempData_160.coef_real[i]));
        }
        fscanf(fp_real,"\n");

        for (quint32 i = 0;i < dl_filter_tap_2i;i ++) {
            fscanf(fp_imag,"%hd,",&(tempData_160.coef_imag[i]));
        }
        fscanf(fp_imag,"\n");

        tempData_160.freq = freq;
        SP1401->get_temp(4,tempData_160.temp[0]);
        SP1401->get_temp(5,tempData_160.temp[1]);
        SP1401->get_temp(6,tempData_160.temp[2]);
        SP1401->get_temp(7,tempData_160.temp[3]);
        tempData_160.time = getCurTime();

        model_160->calTable()->replace(idx,tempData_160);
        SP1401->cf()->add(cal_file::TX_FILTER_160,&tempData_160);
        emit update(model_160->index(idx,0),model_160->index(idx,7),cal_file::TX_FILTER_160);
        idx ++;
    }
    SP1401->cf()->w(cal_file::TX_FILTER_160);
    fclose(fp_real);
    fclose(fp_imag);
    fp_real = nullptr;
    fp_imag = nullptr;

    QFile::remove(pathReal);
    QFile::remove(pathImag);
}

void QCalR1CTXFilterThread::check()
{
    if (!calParam.cal) {
        THREAD_CHECK_BOX("TX<===>Power Meter");

        Instr.init();
        Instr.has_pm();
        Instr.pm_reset();

        initTXChain();
    }

    uint64_t freqRF = 0;
    double freqIF = 0.0;
    double freqStarIF = 0.0;
    double freqStopIF = 0.0;
    const double freqStepIF = 1e6;

    double flatness = spec::cal_tx_filter_flatness();
    double ripple = spec::cal_tx_filter_ripple();
    spec::cal_tx_filter_freq(freqStringCheck);
    parse_range_freq_string(freqStringCheck,freqRangeCheck);

    tx_passband_freq_res_160_cal_data data;

    SP1401->prepare_cr_tx_passband_freq_res_160_cal();
    SP2401->set_tx_filter_sw(true);
    freqStarIF = dl_filter_160M_freq_star;
    freqStopIF = dl_filter_160M_freq_stop;
    for (quint32 i = 0;i < freqRangeCheck.freqs.size();i ++) {
        freqRF = freqRangeCheck.freqs.at(i);

        SP3301->set_tx_freq(RFIdx,freqRF);

        quint32 j = 0;
        for (freqIF = freqStarIF;freqIF <= freqStopIF;freqIF += freqStepIF) {
            SP2401->set_dds1(freqIF);
            msleep(50);

            Instr.pm_get_pwr(freqRF + freqIF,data.pwr[j]);
            j ++;
        }
        SP1401->cr_tx_passband_freq_res_160_cal()->add(freqRF,data);
    }
    SP1401->cr_tx_passband_freq_res_160_cal()->update();
}


void QExpR1CTXFilterThread::run()
{
    RD_TRY
    initProgress("Exporting TX Filter",100);

    tx_rf_fr_table::data_f_t tempDataRF_FR;;
    tx_if_fr_table::data_f_t tempDataIF_FR;
    tx_filter_80m_table::data_f_t  tempData_80;
    tx_filter_160m_table::data_f_t tempData_160;
    tx_filter_80m_table::data_f_t  coefData_80;
    tx_filter_160m_table::data_f_t coefData_160;

    Qwt_TX_RF_FR_Data *dataRF_FR_0 = dynamic_cast<Qwt_TX_RF_FR_Data *>(calParam.plotData_0);
    Qwt_TX_RF_FR_Data *dataRF_FR_1 = dynamic_cast<Qwt_TX_RF_FR_Data *>(calParam.plotData_1);
    Qwt_TX_IF_FR_Data *dataIF_FR   = dynamic_cast<Qwt_TX_IF_FR_Data *>(calParam.plotData_2);
    QTXFilterModel *model_80  = dynamic_cast<QTXFilterModel *>(calParam.model_0);
    QTXFilterModel *model_160 = dynamic_cast<QTXFilterModel *>(calParam.model_1);

    quint64 freqStarRF = 0;
    quint64 freqStopRF = 0;
    quint64 freqStepRF = 0;
    double freqStarIF = 0.0;
    double freqStopIF = 0.0;
    double freqStepIF = 0.0;

    quint32 idx = 0;

    SP1401->cf()->map2buf(cal_file::TX_RF_FR_0);
    freqStarRF = tx_filter_freq_star_0;
    freqStopRF = tx_filter_freq_stop_0;
    freqStepRF = tx_filter_freq_step;
    for (quint64 freq = freqStarRF;freq <= freqStopRF;freq += freqStepRF) {
        SP1401->cf()->m_tx_rf_fr_0->get(freq,&tempDataRF_FR);
        dataRF_FR_0->calTable()->append(tempDataRF_FR);
    }

    SP1401->cf()->map2buf(cal_file::TX_RF_FR_1);
    freqStarRF = tx_filter_freq_star_1;
    freqStopRF = tx_filter_freq_stop_1;
    freqStepRF = tx_filter_freq_step;
    for (quint64 freq = freqStarRF;freq <= freqStopRF;freq += freqStepRF) {
        SP1401->cf()->m_tx_rf_fr_1->get(freq,&tempDataRF_FR);
        dataRF_FR_1->calTable()->append(tempDataRF_FR);
    }

    SP1401->cf()->map2buf(cal_file::TX_IF_FR);
    freqStarIF = dl_filter_160M_freq_star;
    freqStopIF = dl_filter_160M_freq_stop;
    freqStepIF = dl_filter_freq_step;
    for (double freq = freqStarIF;freq <= freqStopIF;freq += freqStepIF) {
        SP1401->cf()->m_tx_if_fr->get(freq,&tempDataIF_FR);
        dataIF_FR->calTable()->append(tempDataIF_FR);
    }

    SP1401->cf()->set_bw(_80M);
    SP1401->cf()->map2buf(cal_file::TX_FILTER_80);
    freqStarRF = tx_freq_star;
    freqStopRF = tx_freq_stop;
    freqStepRF = tx_freq_step_called;
    for (quint64 freq = freqStarRF;freq <= freqStopRF;freq += freqStepRF) {
        SP1401->cf()->m_tx_filter_80m->get(freq,&tempData_80);
        model_80->calTable()->replace(idx,tempData_80);
        idx ++;
    }

    idx = 0;
    SP1401->cf()->set_bw(_160M);
    SP1401->cf()->map2buf(cal_file::TX_FILTER_160);
    for (quint64 freq = freqStarRF;freq <= freqStopRF;freq += freqStepRF) {
        SP1401->cf()->m_tx_filter_160m->get(freq,&tempData_160);
        model_160->calTable()->replace(idx,tempData_160);
        idx ++;
    }

    emit update(QModelIndex(),QModelIndex(),cal_file::TX_RF_FR_0);
    emit update(QModelIndex(),QModelIndex(),cal_file::TX_RF_FR_1);
    emit update(QModelIndex(),QModelIndex(),cal_file::TX_IF_FR);
    emit update(model_80->index(0,0),model_80->index(tx_freqs_called,7),cal_file::TX_FILTER_80);
    emit update(model_160->index(0,0),model_160->index(tx_freqs_called,7),cal_file::TX_FILTER_160);

    SET_PROG_POS(100);
    THREAD_ENDED
    RD_CAL_CATCH
}
