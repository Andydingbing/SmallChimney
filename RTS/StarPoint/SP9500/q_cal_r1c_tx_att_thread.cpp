#include "q_cal_r1c_tx_att_thread.h"
#include "q_model_tx_att.h"
#include "algo_math.hpp"
#include "spec.h"

using namespace ns_sp1401;
using namespace ns_sp1401::r1c;
using namespace ns_sp2401;

void QCalR1CTXAttThread::run()
{
    RD_CAL_TRY

    if (calOP(calParam.mode) && calParam.cal) {
        totalPts += freqRangeCal.freqs.size() * 5;
    }
    if (calIO(calParam.mode) && calParam.cal) {
        totalPts += freqRangeCal.freqs.size() * 4;
    }

    CAL_THREAD_START("TX Att",totalPts);

    CalIOMode calMode = calParam.mode;

    if (calOP(calMode)) {
        THREAD_CHECK_BOX("TX<===>Spectrum");
    }
    if (calIO(calMode) && !calOP(calMode)) {
        THREAD_CHECK_BOX("IO<===>Spectrum");
    }

    tx_lol_table_r1cd::data_m_t dataLOL;
    tx_sb_table_r1cd::data_m_t dataSB;

    Instr.init();
    Instr.has_sa();
    Instr.sa_reset();
    Instr.sa_set_span(20000.0);
    Instr.sa_set_det(sa::DET_AVERAGE);
    Instr.sa_set_rbw(3000.0,false);
    Instr.sa_set_vbw(3000.0,false);

    SP1401->cf()->set_bw(_160M);
    SP1401->set_rx_att_019_sw(r1c::RX_ATT_19);
    SP1401->set_rx_lna_att_sw(r1c::RX_ATT);
    SP1401->set_rx_att(30.0,30.0,30.0);
    SP2401->set_dds_src(sp2401_r1a::SINGLE_TONE);
    SP2401->set_dds1(0.0);
    SP2401->set_duc_dds(0.0);
    SP2401->set_tx_filter_sw(true);

    SP1401->cf()->m_tx_sb->get(2000000000,&dataSB);
    SP1401->cf()->m_tx_lol->get(2000000000,&dataLOL);
    SP2401->set_tx_phase_rotate_I(double(dataSB.th));
    SP2401->set_tx_amplitude_balance(dataSB.am_i,dataSB.am_q);
    SP2401->set_tx_dc_offset(dataLOL.dc_i,dataLOL.dc_q);

    if (calOP(calMode)) {
        THREAD_TEST_CANCEL
        if (calParam.cal) {
            initProgress("Calibrating TX Att");
            SP1401->set_io_mode(OUTPUT);

            THREAD_TEST_CANCEL
            Instr.sa_set_ref(tx_base_pwr_op + 25.0);
            Instr.sa_set_en_preamp(false);
            Instr.sa_set_mech_att(true);
            Instr.sa_set_avg_trace(false,0);
            Instr.sa_sweep_once();
            msleep(10);
            calOneSec(OUTPUT,R1C_TX_ATT_OP_POWER_STAR - R1C_TX_ATT_STEP,tx_base_pwr_op - R1C_TX_ATT_STEP);

            THREAD_TEST_CANCEL
            Instr.sa_set_ref(tx_base_pwr_op + 20.0);
            Instr.sa_sweep_once();
            msleep(10);
            calOneSec(OUTPUT,tx_base_pwr_op,tx_base_pwr_op - 20.0);

            THREAD_TEST_CANCEL
            Instr.sa_set_ref(tx_base_pwr_op + 20.0 - 20.0);
            Instr.sa_sweep_once();
            msleep(10);
            calOneSec(OUTPUT,tx_base_pwr_op - 20.0 * 1,tx_base_pwr_op - 20.0 * 2);

            THREAD_TEST_CANCEL
            Instr.sa_set_ref(tx_base_pwr_op + 20.0 - 20.0 * 2);
            Instr.sa_sweep_once();
            msleep(10);
            calOneSec(OUTPUT,tx_base_pwr_op - 20.0 * 2,tx_base_pwr_op - 20.0 * 3);

            THREAD_TEST_CANCEL
            Instr.sa_set_ref(tx_base_pwr_op + 20.0 - 20.0 * 3);
            Instr.sa_set_en_preamp(true);
            Instr.sa_set_mech_att(false,0);
            Instr.sa_set_avg_trace(true,3);
            Instr.sa_sweep_once();
            msleep(10);
            calOneSec(OUTPUT,tx_base_pwr_op - 20.0 * 3,tx_base_pwr_op - 20.0 * 4);

            THREAD_TEST_CANCEL
            Instr.sa_set_ref(tx_base_pwr_op + 20.0 - 20.0 * 4);
            Instr.sa_set_avg_trace(true,6);
            Instr.sa_sweep_once();
            msleep(10);
            calOneSec(OUTPUT,tx_base_pwr_op - 20.0 * 4,R1C_TX_ATT_OP_POWER_STOP);
        }
        THREAD_TEST_CANCEL
        if (calParam.check) {
            initProgress("Checking TX Power");
            checkIt(OUTPUT);
        }
    }

    if (calOP(calMode) && calIO(calMode)) {
        THREAD_CHECK_BOX("Output Complete,IO<===>Sepctrum");
    }

    if (calIO(calMode)) {
        THREAD_TEST_CANCEL
        if (calParam.cal) {
            initProgress("Calibrating TX Att");
            SP1401->set_io_mode(IO);

            idxOffsetStar = 0;

            THREAD_TEST_CANCEL
            Instr.sa_reset();
            Instr.sa_set_span(20000.0);
            Instr.sa_set_det(sa::DET_AVERAGE);
            Instr.sa_set_rbw(3000.0,false);
            Instr.sa_set_vbw(3000.0,false);
            Instr.sa_set_en_preamp(false);
            Instr.sa_set_mech_att(true);
            Instr.sa_set_avg_trace(false,0);
            Instr.sa_set_ref(tx_base_pwr_io + 25.0);
            Instr.sa_sweep_once();
            msleep(10);
            calOneSec(IO,R1C_TX_ATT_IO_POWER_STAR - R1C_TX_ATT_STEP,tx_base_pwr_io - R1C_TX_ATT_STEP);

            THREAD_TEST_CANCEL
            Instr.sa_set_ref(tx_base_pwr_io + 20.0);
            Instr.sa_sweep_once();
            msleep(10);
            calOneSec(IO,tx_base_pwr_io,tx_base_pwr_io - 20.0);

            THREAD_TEST_CANCEL
            Instr.sa_set_ref(tx_base_pwr_io + 20.0 - 20.0);
            Instr.sa_sweep_once();
            msleep(10);
            calOneSec(IO,tx_base_pwr_io - 20.0 * 1,tx_base_pwr_io - 20.0 * 2);

            THREAD_TEST_CANCEL
            Instr.sa_set_ref(tx_base_pwr_io + 20.0 - 20.0 * 2);
            Instr.sa_set_en_preamp(true);
            Instr.sa_set_mech_att(false,0);
            Instr.sa_set_avg_trace(true,3);
            Instr.sa_sweep_once();
            msleep(10);
            calOneSec(IO,tx_base_pwr_io - 20.0 * 2,tx_base_pwr_io - 20.0 * 3);

            THREAD_TEST_CANCEL
            Instr.sa_set_ref(tx_base_pwr_io + 20.0 - 20.0 * 3);
            Instr.sa_set_avg_trace(true,6);
            Instr.sa_sweep_once();
            msleep(10);
            calOneSec(IO,tx_base_pwr_io - 20.0 * 3,R1C_TX_ATT_IO_POWER_STOP);
        }
        THREAD_TEST_CANCEL
        if (calParam.check) {
            initProgress("Checking TX Power");
            checkIt(IO);
        }
    }
    THREAD_ENDED
    RD_CAL_CATCH
}

void QCalR1CTXAttThread::checkIt(io_mode_t mode)
{
    quint64 freq = 0;
    double lineLoss = 0.0;
    double targetPwr = 0.0;
    double SAPeakPwr[2] = { 0.0,0.0 };
    double actualPwr = 0.0;
    double refOffset = 0.0;
    double refCur = 10.0;
    double refBfr = refCur;
    tx_pwr_cal_data data;
    bool res = true;

    range_pwr_string pwrString;
    range_pwr<double> pwrRange;

    spec::cal_tx_pwr_pwr(mode,pwrString);
    parse_range_pwr_string(pwrString,pwrRange);

    THREAD_TEST_CANCEL

    if (mode == OUTPUT) {
        SP1401->prepare_cr_tx_pwr_op_cal();
    } else if (mode == IO) {
        SP1401->prepare_cr_tx_pwr_io_cal();
    }

    SP3301->set_tx_en_tc(RFIdx,true);
    SP3301->set_io_mode(RFIdx,mode);

    for (quint32 i = 0;i < freqRangeCheck.freqs.size();i ++) {
        THREAD_TEST_CANCEL
        refBfr = refCur = 10.0;
        freq = freqRangeCheck.freqs.at(i);

        Instr.sa_set_cf(double(freq));
        Instr.sa_set_en_preamp(false);
        Instr.sa_set_mech_att(true);
        Instr.sa_set_avg_trace(false,0);
        Instr.sa_set_ref(refCur);
        Instr.sa_sweep_once();
        SP3301->set_tx_freq(RFIdx,freq);
        SP3301->set_tx_pwr(RFIdx,-12.0);
        msleep(500);
        Instr.sa_sweep_once();
        Instr.sa_set_peak_search(sa::PEAK_HIGHEST);
        Instr.sa_get_marker_pwr(SAPeakPwr[0]);
        lineLoss = -12.0 - SAPeakPwr[0];

        SP3301->set_tx_pwr(RFIdx,-15.0);
        msleep(500);
        Instr.sa_sweep_once();
        Instr.sa_get_marker_pwr(SAPeakPwr[0]);

        if (abs(-15.0 - SAPeakPwr[0] - lineLoss) > 0.1) {
            RD_THROW rd::runtime_error("Lineloss test failed.Not called yet?");
        }

        refBfr = refCur = ajustSA(pwrRange.pwrs.at(0));
        refOffset = 0.0;
        for (quint32 j = 0;j < pwrRange.pwrs.size();j ++) {
            THREAD_TEST_PAUSE_S
            THREAD_TEST_CANCEL
            targetPwr = pwrRange.pwrs.at(j);
            refCur = ajustSA(targetPwr);
            if (refCur - refBfr != 0.0) {
                Instr.sa_get_marker_pwr(SAPeakPwr[1]);
                refOffset += SAPeakPwr[0] - SAPeakPwr[1];
                refBfr = refCur;
            }
            SP3301->set_tx_pwr(RFIdx,float(targetPwr));
            Instr.sa_sweep_once();
            Instr.sa_get_marker_pwr(SAPeakPwr[0]);
            actualPwr = SAPeakPwr[0] + lineLoss + refOffset;
            data.pwr[j] = actualPwr;

            res = abs(actualPwr - targetPwr) <= spec::cal_tx_pwr_accuracy();
            if (!res) {
                Log.set_last_err("%s Fail.Freq:%s,Power:%f(%f)(%s)",
                                 g_threadName.toStdString().c_str(),
                                 freq_string_from(freq).c_str(),
                                 actualPwr,
                                 targetPwr,
                                 string_of(mode).c_str());
                emit threadProcess(RUNNING_EXCEPT);
            }
            data.set_result(res);
            data.set_time();

            if (mode == OUTPUT) {
                SP1401->cr_tx_pwr_op_cal()->add(int64_t(freq),data);
            } else if (mode == IO) {
                SP1401->cr_tx_pwr_io_cal()->add(int64_t(freq),data);
            }
            updateTotalResult(res);
            THREAD_TEST_PAUSE_E
        }
        addProgressPos(1);
    }

    if (mode == OUTPUT) {
        SP1401->cr_tx_pwr_op_cal()->update();
        SP1401->ftp_put_cr_tx_pwr_op_cal();
    } else if (mode == IO) {
        SP1401->cr_tx_pwr_io_cal()->update();
        SP1401->ftp_put_cr_tx_pwr_io_cal();
    }
    SP3301->set_tx_en_tc(RFIdx,false);
}

void QCalR1CTXAttThread::calOneSec(io_mode_t mode, double pwrStar, double pwrStop)
{
    QR1CTXAttOPModel *model_op = dynamic_cast<QR1CTXAttOPModel *>(calParam.model_0);
    QR1CTXAttIOModel *model_io = dynamic_cast<QR1CTXAttIOModel *>(calParam.model_1);

    tx_att_op_table_r1cd::data_f_t dataOP;
    tx_att_io_table_r1cd::data_f_t dataIO;
    tx_pwr_op_table_r1c::data_m_t  dataPwrOP;
    tx_pwr_io_table_r1c::data_m_t  dataPwrIO;
    tx_filter_160m_table::data_m_t dataFilter;
    double coefReal[dl_filter_tap_2i] = {0.0};
    double coefImag[dl_filter_tap_2i] = {0.0};

    quint64 freq = 0;
    double pwrStep = R1C_TX_ATT_STEP;
    double pwrSA[2] = {0.0,0.0};
    double pwr = 0.0;
    double basePwr = 0.0;
    quint32 idxOffset = 0;
    qint32 secBfr = -1;
    qint32 secCur = 0;
    float offsetBase = 0.0;
    float offset = 0.0;

    if (mode == OUTPUT) {
        basePwr = (pwrStar <= tx_base_pwr_op ? pwrStar : tx_base_pwr_op);
    } else if (mode == IO) {
        basePwr = (pwrStar <= tx_base_pwr_io ? pwrStar : tx_base_pwr_io);
    }

    for (quint32 i = 0;i < freqRangeCal.freqs.size();i ++) {
        THREAD_TEST_CANCEL

        freq = freqRangeCal.freqs.at(i);

        Instr.sa_set_cf(double(freq));
        Instr.sa_sweep_once();
        SP1401->set_tx_freq(freq);

        if (mode == OUTPUT) {
            SP1401->cf()->m_tx_att_op->get(freq,&dataOP);
            SP1401->cf()->m_tx_pwr_op->get(RFVer,freq,basePwr,&dataPwrOP);
            SP1401->set_tx_att0(dataPwrOP.att0 / 2.0);
            SP1401->set_tx_att1(dataPwrOP.att1 / 2.0);
            SP1401->set_tx_att2(dataPwrOP.att2 / 2.0);
            SP1401->set_tx_att3(dataPwrOP.att3 / 2.0);
            SP2401->set_tx_pwr_comp(dataPwrOP.d_gain);

            if (pwrStar == tx_base_pwr_op || pwrStop == tx_base_pwr_op - pwrStep) {
                offsetBase = 0.0;
            } else if (idxOffsetStar >= 1) {
                offsetBase = dataOP.offset[idxOffsetStar - 1];
            }
        } else if (mode == IO) {
            SP1401->cf()->m_tx_att_io->get(freq,&dataIO);
            SP1401->cf()->m_tx_pwr_io->get(RFVer,freq,basePwr,&dataPwrIO);
            SP1401->set_tx_att0(dataPwrIO.att0 / 2.0);
            SP1401->set_tx_att1(dataPwrIO.att1 / 2.0);
            SP1401->set_tx_att2(dataPwrIO.att2 / 2.0);
            SP1401->set_tx_att3(dataPwrIO.att3 / 2.0);
            SP2401->set_tx_pwr_comp(dataPwrIO.d_gain);

            if (pwrStar == tx_base_pwr_io || pwrStop == tx_base_pwr_io - pwrStep) {
                offsetBase = 0.0;
            } else if (idxOffsetStar >= 1) {
                offsetBase = dataIO.offset[idxOffsetStar - 1];
            }
        }

        SP1401->cf()->m_tx_filter_160m->get(freq,&dataFilter);
        dataFilter._2double(coefReal,coefImag);
        SP2401->set_tx_filter(coefReal,coefImag);

        msleep(500);

        Instr.sa_sweep_once();
        Instr.sa_set_peak_search(sa::PEAK_HIGHEST);
        Instr.sa_get_marker_pwr(pwrSA[0]);

        idxOffset = idxOffsetStar;
        secCur = freq_section(freq,freqRangeCal);
        for (pwr = pwrStar + pwrStep;pwr >= pwrStop;pwr += pwrStep) {
            THREAD_TEST_PAUSE_S
            THREAD_TEST_CANCEL

            if (mode == OUTPUT) {
                SP1401->cf()->m_tx_pwr_op->get(RFVer,freq,pwr,&dataPwrOP);
                SP1401->set_tx_att0(dataPwrOP.att0 / 2.0);
                SP1401->set_tx_att1(dataPwrOP.att1 / 2.0);
                SP1401->set_tx_att2(dataPwrOP.att2 / 2.0);
                SP1401->set_tx_att3(dataPwrOP.att3 / 2.0);
                SP2401->set_tx_pwr_comp(dataPwrOP.d_gain);

                if (pwr >= tx_base_pwr_op) {
                    msleep(100); // Digital Power Compansate is slower
                } else {
                    msleep(50);
                }
            } else if (mode == IO) {
                SP1401->cf()->m_tx_pwr_io->get(RFVer,freq,pwr,&dataPwrIO);
                SP1401->set_tx_att0(dataPwrIO.att0 / 2.0);
                SP1401->set_tx_att1(dataPwrIO.att1 / 2.0);
                SP1401->set_tx_att2(dataPwrIO.att2 / 2.0);
                SP1401->set_tx_att3(dataPwrIO.att3 / 2.0);
                SP2401->set_tx_pwr_comp(dataPwrIO.d_gain);

                if (pwr >= tx_base_pwr_io) {
                    msleep(100);
                } else {
                    msleep(50);
                }
            }

            Instr.sa_sweep_once();
            Instr.sa_get_marker_pwr(pwrSA[1]);

            offset = float((pwr - basePwr) - (pwrSA[1] - pwrSA[0]));

            if (mode == OUTPUT) {
                dataOP.offset[idxOffset] = offset;
                dataOP.offset[idxOffset] += offsetBase;

                if (secCur != secBfr) {
                    model_op->iterTable(idxOffset)->at(secCur)->locate2CalTable(model_op->calTable()->begin() + i);
                }
                model_op->calTable()->replace(i, dataOP);
                model_op->iterTable(idxOffset)->at(secCur)->addOneData();
                emit update(model_op->index(i * R1C_TX_ATT_OP_POWER_PTS, 0),
                            model_op->index((i + 1) * R1C_TX_ATT_OP_POWER_PTS, 13),
                            cal_file::TX_ATT_OP,
                            secCur);
            } else if (mode == IO) {
                dataIO.offset[idxOffset] = offset;
                dataIO.offset[idxOffset] += offsetBase;

                if (secCur != secBfr) {
                    model_io->iterTable(idxOffset)->at(secCur)->locate2CalTable(model_io->calTable()->begin() + i);
                }
                model_io->calTable()->replace(i, dataIO);
                model_io->iterTable(idxOffset)->at(secCur)->addOneData();
                emit update(model_io->index(i * R1C_TX_ATT_IO_POWER_PTS, 0),
                            model_io->index((i + 1) * R1C_TX_ATT_IO_POWER_PTS, 13),
                            cal_file::TX_ATT_IO,
                            secCur);
            }

            idxOffset ++;
            THREAD_TEST_PAUSE_E
        }
        secBfr = secCur;

        if (mode == OUTPUT) {
            dataOP.freq = freq;
            SP1401->get_temp(4,dataOP.temp[0]);
            SP1401->get_temp(5,dataOP.temp[1]);
            SP1401->get_temp(6,dataOP.temp[2]);
            SP1401->get_temp(7,dataOP.temp[3]);
            dataOP.time = getCurTime();
            SP1401->cf()->add(cal_file::TX_ATT_OP,&dataOP);
        } else if (mode == IO) {
            dataIO.freq = freq;
            SP1401->get_temp(4,dataIO.temp[0]);
            SP1401->get_temp(5,dataIO.temp[1]);
            SP1401->get_temp(6,dataIO.temp[2]);
            SP1401->get_temp(7,dataIO.temp[3]);
            dataIO.time = getCurTime();
            SP1401->cf()->add(cal_file::TX_ATT_IO,&dataIO);
        }
        addProgressPos(1);
    }
    idxOffsetStar = idxOffset;

    if (mode == OUTPUT) {
        SP1401->cf()->w(cal_file::TX_ATT_OP);
    } else if (mode == IO) {
        SP1401->cf()->w(cal_file::TX_ATT_IO);
    }
}

double QCalR1CTXAttThread::ajustSA(double pwr) const
{
    pwr += 10.0;
    double ref = linear_quantify<double>(20.0,-20.0,pwr);
    double refCur = 0.0;

    Instr.sa_get_ref(refCur);
    if (refCur - ref != 0.0) {
        Instr.sa_set_ref(ref);
    } else {
        return ref;
    }

    if (pwr < -40.0) {
        Instr.sa_set_en_preamp(true);
        Instr.sa_set_mech_att(false,0);
        if (pwr > -60.0) {
            Instr.sa_set_avg_trace(true,3);
        } else {
            Instr.sa_set_avg_trace(true,6);
        }
        msleep(100);
    }
    msleep(100);
    Instr.sa_sweep_once();
    return ref;
}


void QExpR1CTXAttThread::run()
{
    initProgress("Exporting TX Att",100);

    CalIOMode calMode = calParam.mode;
    quint64 freq = 0;
    int secBfr = -1;
    int secCur = 0;

    if (calOP(calMode)) {
        tx_att_op_table_r1cd::data_f_t dataOP;
        QR1CTXAttOPModel *modelOP = (QR1CTXAttOPModel *)(calParam.model_0);

        SP1401->cf()->map2buf(cal_file::TX_ATT_OP);

        for (quint32 i = 0;i < freqRangeCal.freqs.size();i ++) {
            freq = freqRangeCal.freqs.at(i);
            SP1401->cf()->m_tx_att_op->get(freq, &dataOP);

            secCur = freq_section(freq,freqRangeCal);
            for (quint32 j = 0;j < R1C_TX_ATT_OP_POWER_PTS;j ++) {
                if (secCur != secBfr)
                    modelOP->iterTable(j)->at(secCur)->locate2CalTable(modelOP->calTable()->begin() + i);
                modelOP->iterTable(j)->at(secCur)->addOneData();
            }

            modelOP->calTable()->replace(i, dataOP);
            secBfr = secCur;
        }

        emit update(modelOP->index(0, 0),
                    modelOP->index(freqRangeCal.freqs.size() * R1C_TX_ATT_OP_POWER_PTS, 13),
                    cal_file::TX_ATT_OP,
                    secCur);
    }

    secBfr = -1;
    secCur = 0;

    if (calIO(calMode)) {
        tx_att_io_table_r1cd::data_f_t dataIO;
        QR1CTXAttIOModel *modelIO = (QR1CTXAttIOModel *)(calParam.model_1);

        SP1401->cf()->map2buf(cal_file::TX_ATT_IO);

        for (quint32 i = 0;i < freqRangeCal.freqs.size();i ++) {
            freq = freqRangeCal.freqs.at(i);
            SP1401->cf()->m_tx_att_io->get(freq, &dataIO);

            secCur = freq_section(freq,freqRangeCal);
            for (quint32 j = 0;j < R1C_TX_ATT_IO_POWER_PTS;j ++) {
                if (secCur != secBfr)
                    modelIO->iterTable(j)->at(secCur)->locate2CalTable(modelIO->calTable()->begin() + i);
                modelIO->iterTable(j)->at(secCur)->addOneData();
            }

            modelIO->calTable()->replace(i, dataIO);
            secBfr = secCur;
        }

        emit update(modelIO->index(0, 0),
                    modelIO->index(freqRangeCal.freqs.size() * R1C_TX_ATT_IO_POWER_PTS, 13),
                    cal_file::TX_ATT_IO,
                    secCur);
    }

    SET_PROG_POS(100);
    THREAD_ENDED
}
