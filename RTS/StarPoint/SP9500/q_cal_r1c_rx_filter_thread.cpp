#include "q_cal_r1c_rx_filter_thread.h"
#include <QFile>

#ifdef RD_PLATFORM_WIN32
    #include <Windows.h>
#endif

using namespace rd::ns_sp1401;
using namespace rd::ns_sp2401;

void QCalR1CRXFilterThread::run()
{
    RD_CAL_TRY
    CAL_THREAD_START("RX Filter",(rx_filter_freqs + ul_filter_160M_freqs));

    Qwt_RX_RF_FR_Data *dataRF_FR = dynamic_cast<Qwt_RX_RF_FR_Data *>(calParam.plotData_0);
    Qwt_RX_IF_FR_Data *dataIF_FR = dynamic_cast<Qwt_RX_IF_FR_Data *>(calParam.plotData_1);

    // 80M : 100M-43M ~~~ 6G+43M,step 2M.
    // 160M: 100M-83M ~~~ 6G+83M,step 2M.

    setTempRange(2.0);

    if (calParam.justRebuildCoef != true) {
        THREAD_CHECK_BOX("RX<===>Z28<===>Signal Generator");

        Instr.init();
        Instr.has_pm();
        Instr.has_sg();
        Instr.pm_reset();
        Instr.sg_reset();
        Instr.sg_set_en_mod(false);
        Instr.sg_set_pl(-60.0);
        Instr.sg_set_en_output(true);

        initRXChain();
        THREAD_TEST_CANCEL
        sweepRF(dataRF_FR);
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
        SET_PROG_POS(rx_filter_freqs + ul_filter_160M_freqs);
    }
    THREAD_ENDED
    RD_CAL_CATCH
}

void QCalR1CRXFilterThread::initRXChain()
{
    qint32 lnaAtt = 0;
    qint32 att019 = 0;
    double att1 = 0.0;
    double att2 = 0.0;
    double att3 = 0.0;

    if (RFVer == R1C || RFVer == R1D || RFVer == R1E) {
        rx_ref_op_table_r1cd::guess_base_r1c(1,fakeRef,lnaAtt,att019,att1,att2,att3);
    } else if (RFVer == R1F) {
        rx_ref_op_table_r1cd::guess_base_r1f(1,fakeRef,lnaAtt,att019,att1,att2);
    }

    fakeRef -= 15;

    SP1401->set_io_mode(OUTPUT);
    SP1401->set_rx_lna_att_sw(r1c::rx_lna_att_t(lnaAtt));
    SP1401->set_rx_att_019_sw(r1c::rx_att_019_t(att019));
    SP1401->set_rx_att(att1,att2,att3);
    SP1401->set_pwr_meas_src(sp1401::PWR_MEAS_FREE_RUN,false);
    SP1401->set_pwr_meas_timeout(6553600);
    SP1401->set_pwr_meas_samples(102400);
    SP1401->set_tx_att(30.0,30.0,30.0,30.0);
    SP1401->set_rx_if_filter_sw(sp1401::_160MHz);
    SP2401->set_dds_src(sp2401_r1a::ALL_0);
    SP2401->set_tx_filter_sw(false);
    SP2401->set_ddc(-92640000.0);
    SP2401->set_rx_pwr_comp(0);
    SP2401->set_rx_filter_sw(sp2401_r1a::_2I3D);
    SP2401->set_rx_filter_default();
    msleep(10);
}

void QCalR1CRXFilterThread::sweepRF(Qwt_RX_RF_FR_Data *qwtData)
{
    rx_rf_fr_table::data_f_t data;
    quint64 freq = 0;

    for (quint32 i = 0;i < rx_filter_freqs;i ++) {
        THREAD_TEST_PAUSE_S
        THREAD_TEST_CANCEL

        data.freq = rx_filter_freq_star + i * rx_filter_freq_step;
        freq = data.freq < rx_freq_star ? rx_freq_star : data.freq;
        freq = freq > rx_freq_stop ? rx_freq_stop : freq;

        ajustSG(double(freq),fakeRef);
        SP1401->set_rx_freq(freq);
        msleep(50);

        data.pwr = getRXPwr();

        SP1401->get_rx_avg_temp(tempCur);
        THREAD_CHECK_TEMP;

        SP1401->cf()->add(cal_file::RX_RF_FR,&data);
        qwtData->calTable()->append(data);
        emit update(QModelIndex(),QModelIndex(),cal_file::RX_RF_FR);

        SET_PROG_POS(i + 1);
        THREAD_TEST_PAUSE_E
    }
    SP1401->cf()->w(cal_file::RX_RF_FR);
    SP1401->cf()->m_rx_rf_fr->save_as("c:\\rx_filter_rf_fr.txt");
}

void QCalR1CRXFilterThread::sweepIF(Qwt_RX_IF_FR_Data *qwtData)
{
    rx_if_fr_table::data_f_t data;

    SP1401->set_rx_freq(2000000000);
    msleep(10);

    for (quint32 i = 0;i < ul_filter_160M_freqs;i ++) {
        THREAD_TEST_PAUSE_S
        THREAD_TEST_CANCEL

        data.freq = ul_filter_160M_freq_star + i * ul_filter_freq_step;
        ajustSG(2000000000 + data.freq,fakeRef);
        msleep(50);

        data.pwr = getRXPwr();

        SP1401->get_rx_avg_temp(tempCur);
        THREAD_CHECK_TEMP;

        SP1401->cf()->add(cal_file::RX_IF_FR,&data);
        qwtData->calTable()->append(data);
        emit update(QModelIndex(),QModelIndex(),cal_file::RX_IF_FR);

        SET_PROG_POS(rx_filter_freqs + i + 1);
        THREAD_TEST_PAUSE_E
    }
    SP1401->cf()->w(cal_file::RX_IF_FR);
    SP1401->cf()->m_rx_if_fr->save_as("c:\\rx_filter_if_fr.txt");
}

void QCalR1CRXFilterThread::generateFreqResponse()
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
    FILE *fp_fr = fopen("C:\\CSECal\\rx_filter_80.txt","w");
    if (fp_fr == nullptr) {
        QString msg = QString("Could not open file");
        THREAD_ERROR_BOX(msg);
        THREAD_ENDED
    }

    // (2G-43M)~~~(2G+43M),step 2M.
    rx_rf_fr_table::data_f_t tempDataRFFr_80[ul_filter_80M_freqs];
    // -43M~~~43M,step 2M.
    rx_if_fr_table::data_f_t tempDataIFFr_80[ul_filter_80M_freqs];

    SP1401->cf()->set_bw(_80M);
    SP1401->cf()->map2buf(cal_file::RX_RF_FR);
    freqStarRF = quint64(2e9 + ul_filter_80M_freq_star);
    freqStopRF = quint64(2e9 + ul_filter_80M_freq_stop);
    freqStepRF = quint64(ul_filter_freq_step);
    for (freqRF = freqStarRF;freqRF <= freqStopRF;freqRF += freqStepRF) {
        SP1401->cf()->m_rx_rf_fr->get(freqRF,&tempDataRFFr_80[idx]);
        idx ++;
    }

    idx = 0;
    SP1401->cf()->map2buf(cal_file::RX_IF_FR);
    freqStarIF = ul_filter_80M_freq_star;
    freqStopIF = ul_filter_80M_freq_stop;
    freqStepIF = ul_filter_freq_step;
    for (freqIF = freqStarIF;freqIF <= freqStopIF;freqIF += freqStepIF) {
        SP1401->cf()->m_rx_if_fr->get(freqIF,&tempDataIFFr_80[idx]);
        idx ++;
    }

    // Real IF frequency response,-43M~~~43M.
    for (idx = 0;idx < ul_filter_80M_freqs;idx ++) {
        tempDataIFFr_80[idx].pwr -= tempDataRFFr_80[idx].pwr;
    }

    SP1401->cf()->map2buf(cal_file::RX_RF_FR);
    freqStarRF = rx_freq_star;
    freqStopRF = rx_freq_stop;
    freqStepRF = r1c::rx_freq_step_called;
    for (freqRF = freqStarRF;freqRF <= freqStopRF;freqRF += freqStepRF) {
        freqCalled = freqRF < 100000000 ? 100000000 : freqRF;
        idx = 0;
        freqStarRF_InBw = quint64(freqCalled + ul_filter_80M_freq_star);
        freqStopRF_InBw = quint64(freqCalled + ul_filter_80M_freq_stop);
        freqStepRF_InBw = quint64(ul_filter_freq_step);
        for (quint64 freq = freqStarRF_InBw;freq <= freqStopRF_InBw;freq += freqStepRF_InBw) {
            SP1401->cf()->m_rx_rf_fr->get(freq,&tempDataRFFr_80[idx]);
            fprintf(fp_fr,"%.6f ",tempDataRFFr_80[idx].pwr + tempDataIFFr_80[idx].pwr);
            idx ++;
        }
        fprintf(fp_fr,"\n");
    }
    fclose(fp_fr);
    fp_fr = nullptr;

    //160M
    fp_fr = fopen("C:\\CSECal\\rx_filter_160.txt","w");
    if (fp_fr == nullptr) {
        QString msg = QString("Could not open file");
        THREAD_ERROR_BOX(msg);
        THREAD_ENDED
    }

    // (2G-83M)~~~(2G+83M),step 2M.
    rx_rf_fr_table::data_f_t tempDataRFFr_160[ul_filter_160M_freqs];
    // -83M~~~83M,step 2M.
    rx_if_fr_table::data_f_t tempDataIFFr_160[ul_filter_160M_freqs];

    idx = 0;
    SP1401->cf()->set_bw(_160M);
    SP1401->cf()->map2buf(cal_file::RX_RF_FR);
    freqStarRF = quint64(2e9 + ul_filter_160M_freq_star);
    freqStopRF = quint64(2e9 + ul_filter_160M_freq_stop);
    freqStepRF = quint64(ul_filter_freq_step);
    for (freqRF = freqStarRF;freqRF <= freqStopRF;freqRF += freqStepRF) {
        SP1401->cf()->m_rx_rf_fr->get(freqRF,&tempDataRFFr_160[idx]);
        idx ++;
    }

    idx = 0;
    SP1401->cf()->map2buf(cal_file::RX_IF_FR);
    freqStarIF = ul_filter_160M_freq_star;
    freqStopIF = ul_filter_160M_freq_stop;
    freqStepIF = ul_filter_freq_step;
    for (freqIF = freqStarIF;freqIF <= freqStopIF;freqIF += freqStepIF) {
        SP1401->cf()->m_rx_if_fr->get(freqIF,&tempDataIFFr_160[idx]);
        idx ++;
    }

    // Real IF frequency response,-83M~~~83M.
    for (idx = 0;idx < ul_filter_160M_freqs;idx ++) {
        tempDataIFFr_160[idx].pwr -= tempDataRFFr_160[idx].pwr;
    }

    SP1401->cf()->map2buf(cal_file::RX_RF_FR);
    freqStarRF = rx_freq_star;
    freqStopRF = rx_freq_stop;
    freqStepRF = r1c::rx_freq_step_called;
    freqCalled = 0;
    for (freqRF = freqStarRF;freqRF <= freqStopRF;freqRF += freqStepRF) {
        freqCalled = freqRF < 100000000 ? 100000000 : freqRF;
        idx = 0;
        freqStarRF_InBw = quint64(freqCalled + ul_filter_160M_freq_star);
        freqStopRF_InBw = quint64(freqCalled + ul_filter_160M_freq_stop);
        freqStepRF_InBw = quint64(ul_filter_freq_step);
        for (quint64 freq = freqStarRF_InBw;freq <= freqStopRF_InBw;freq += freqStepRF_InBw) {
            SP1401->cf()->m_rx_rf_fr->get(freq,&tempDataRFFr_160[idx]);
            fprintf(fp_fr,"%.6f ",tempDataRFFr_160[idx].pwr + tempDataIFFr_160[idx].pwr);
            idx ++;
        }
        fprintf(fp_fr,"\n");
    }
    fclose(fp_fr);
    fp_fr = nullptr;
}

void QCalR1CRXFilterThread::generateCoef()
{
#ifdef RD_PLATFORM_WIN32
    char firExePath[MAX_PATH] = {0};
    ZERO_ARRAY(firExePath);

    ::GetModuleFileNameA(nullptr,firExePath,MAX_PATH);
    *strrchr(firExePath,'\\') = 0;
    strcat(firExePath,"\\SP9500\\rx_filter.exe");

    if (exeFirProcess(firExePath)) {
        QString msg = QString("%1").arg(Log.last_err());
        THREAD_ERROR_BOX(msg);
        THREAD_ENDED
    }
#else
#endif
}

void QCalR1CRXFilterThread::updateCalFile()
{
    char pathReal[MAX_PATH];
    char pathImag[MAX_PATH];

    FILE *fp_real = nullptr;
    FILE *fp_imag = nullptr;

    rx_filter_80m_table::data_f_t  tempData_80;
    rx_filter_160m_table::data_f_t tempData_160;

    QRXFilterModel_80M  *model_80  = dynamic_cast<QRXFilterModel_80M *>(calParam.model_0);
    QRXFilterModel_160M *model_160 = dynamic_cast<QRXFilterModel_160M *>(calParam.model_1);

    quint32 idx = 0;

    // 80M
    sprintf(pathReal,"C:\\CSECal\\rx_filter_80_coef_real.txt");
    sprintf(pathImag,"C:\\CSECal\\rx_filter_80_coef_imag.txt");
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

    for (quint64 freq = rx_freq_star;freq <= rx_freq_stop;freq += r1c::rx_freq_step_called) {
        ZERO_OBJ(tempData_80);
        for (quint32 i = 0;i < ul_filter_tap;i ++) {
            fscanf(fp_real,"%hd,",&(tempData_80.coef_real[i]));
        }
        fscanf(fp_real,"\n");

        for (quint32 i = 0;i < ul_filter_tap;i ++) {
            fscanf(fp_imag,"%hd,",&(tempData_80.coef_imag[i]));
        }
        fscanf(fp_imag,"\n");

        tempData_80.freq = freq;
        tempData_80.time = Log.time_current();

        model_80->calTable()->replace(idx,tempData_80);
        SP1401->cf()->add(cal_file::RX_FILTER_80,&tempData_80);
        emit update(model_80->index(idx,0),model_80->index(idx,7),cal_file::RX_FILTER_80);
        idx ++;
    }
    SP1401->cf()->w(cal_file::RX_FILTER_80);
    fclose(fp_real);
    fclose(fp_imag);
    fp_real = nullptr;
    fp_imag = nullptr;

    QFile::remove(pathReal);
    QFile::remove(pathImag);

    // 160M
    sprintf(pathReal,"C:\\CSECal\\rx_filter_160_coef_real.txt");
    sprintf(pathImag,"C:\\CSECal\\rx_filter_160_coef_imag.txt");
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
    for (quint64 freq = rx_freq_star;freq <= rx_freq_stop;freq += r1c::rx_freq_step_called) {
        ZERO_OBJ(tempData_160);
        for (quint32 i = 0;i < ul_filter_tap;i ++) {
            fscanf(fp_real,"%hd,",&(tempData_160.coef_real[i]));
        }
        fscanf(fp_real,"\n");

        for (quint32 i = 0;i < ul_filter_tap;i ++) {
            fscanf(fp_imag,"%hd,",&(tempData_160.coef_imag[i]));
        }
        fscanf(fp_imag,"\n");

        tempData_160.freq = freq;
        tempData_160.time = Log.time_current();

        model_160->calTable()->replace(idx,tempData_160);
        SP1401->cf()->add(cal_file::RX_FILTER_160,&tempData_160);
        emit update(model_160->index(idx,0),model_160->index(idx,7),cal_file::RX_FILTER_160);
        idx ++;
    }
    SP1401->cf()->w(cal_file::RX_FILTER_160);
    fclose(fp_real);
    fclose(fp_imag);
    fp_real = nullptr;
    fp_imag = nullptr;

    QFile::remove(pathReal);
    QFile::remove(pathImag);
}

void QCalR1CRXFilterThread::ajustSG(double freq, qint32 pwr)
{
    double pmPwr = 0.0;
    Instr.pm_set_freq(freq);
    Instr.sg_set_cw(freq);
    Instr.sg_set_pl(pwr + PM_IL);
    msleep(10);

    for (qint32 i = 0;i < 10;i ++) {
        Instr.pm_get_pwr(freq,pmPwr);
        if (abs(pwr - pmPwr) < 0.05)
            break;
        Instr.sg_set_pl(pwr + PM_IL + (pwr - pmPwr));
        msleep(10);
    }
    msleep(10);
}

double QCalR1CRXFilterThread::getRXPwr()
{
    double pwr = 0.0;
    sp1401::pwr_meas_state_t state = sp1401::PMS_IDLE;

    SP1401->pwr_meas_abort();
    SP1401->pwr_meas_start();

    while (state != sp1401::PMS_DONE) {
        SP1401->get_pwr_meas_state(state);
        if (state == sp1401::PMS_TT)
            return -100.0;
    }
    SP1401->get_pwr_meas_pwr(pwr);
    return pwr;
}


void QExpR1CRXFilterThread::run()
{
    initProgress("Exporting RX Filter",100);

    rx_rf_fr_table::data_f_t tempDataRF_FR;
    rx_if_fr_table::data_f_t tempDataIF_FR;
    rx_filter_80m_table::data_f_t  tempData_80;
    rx_filter_160m_table::data_f_t tempData_160;
    rx_filter_80m_table::data_f_t  coefData_80;
    rx_filter_160m_table::data_f_t coefData_160;

    Qwt_RX_RF_FR_Data *dataRF_FR = dynamic_cast<Qwt_RX_RF_FR_Data *>(calParam.plotData_0);
    Qwt_RX_IF_FR_Data *dataIF_FR = dynamic_cast<Qwt_RX_IF_FR_Data *>(calParam.plotData_1);
    QRXFilterModel_80M  *model_80  = dynamic_cast<QRXFilterModel_80M *>(calParam.model_0);
    QRXFilterModel_160M *model_160 = dynamic_cast<QRXFilterModel_160M *>(calParam.model_1);

    quint64 freqStarRF = 0;
    quint64 freqStopRF = 0;
    quint64 freqStepRF = 0;
    double freqStarIF = 0.0;
    double freqStopIF = 0.0;
    double freqStepIF = 0.0;

    quint32 idx = 0;

    SP1401->cf()->map2buf(cal_file::RX_RF_FR);
    freqStarRF = rx_filter_freq_star;
    freqStopRF = rx_filter_freq_stop;
    freqStepRF = rx_filter_freq_step;
    for (quint64 freq = freqStarRF;freq <= freqStopRF;freq += freqStepRF) {
        SP1401->cf()->m_rx_rf_fr->get(freq,&tempDataRF_FR);
        dataRF_FR->calTable()->append(tempDataRF_FR);
    }

    SP1401->cf()->map2buf(cal_file::RX_IF_FR);
    freqStarIF = ul_filter_160M_freq_star;
    freqStopIF = ul_filter_160M_freq_stop;
    freqStepIF = ul_filter_freq_step;
    for (double freq = freqStarIF;freq <= freqStopIF;freq += freqStepIF) {
        SP1401->cf()->m_rx_if_fr->get(freq,&tempDataIF_FR);
        dataIF_FR->calTable()->append(tempDataIF_FR);
    }

    SP1401->cf()->set_bw(_80M);
    SP1401->cf()->map2buf(cal_file::RX_FILTER_80);
    freqStarRF = rx_freq_star;
    freqStopRF = rx_freq_stop;
    freqStepRF = r1c::rx_freq_step_called;
    for (quint64 freq = freqStarRF;freq <= freqStopRF;freq += freqStepRF) {
        SP1401->cf()->m_rx_filter_80m->get(freq,&tempData_80);
        model_80->calTable()->replace(idx,tempData_80);
        idx ++;
    }

    idx = 0;
    SP1401->cf()->set_bw(_160M);
    SP1401->cf()->map2buf(cal_file::RX_FILTER_160);
    for (quint64 freq = freqStarRF;freq <= freqStopRF;freq += freqStepRF) {
        SP1401->cf()->m_rx_filter_160m->get(freq,&tempData_160);
        model_160->calTable()->replace(idx,tempData_160);
        idx ++;
    }

    emit update(QModelIndex(),QModelIndex(),cal_file::RX_RF_FR);
    emit update(QModelIndex(),QModelIndex(),cal_file::RX_IF_FR);
    emit update(model_80->index(0,0),model_80->index(r1c::rx_freqs_called,7),cal_file::RX_FILTER_80);
    emit update(model_160->index(0,0),model_160->index(r1c::rx_freqs_called,7),cal_file::RX_FILTER_160);

    SET_PROG_POS(100);
    THREAD_ENDED
}
