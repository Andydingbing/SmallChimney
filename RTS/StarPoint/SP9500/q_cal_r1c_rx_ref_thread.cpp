#include "q_cal_r1c_rx_ref_thread.h"
#include "q_model_rx_ref.h"
#include "algo_math.hpp"
#include "algo_chip_att.hpp"
#include "spec.h"

using namespace rd::ns_sp9500;
using namespace rd::ns_sp1401;
using namespace rd::ns_sp2401;

void QCalR1CRXRefThread::run()
{
    RD_CAL_TRY
    CAL_THREAD_START("RX Reference",totalPts);
    THREAD_CHECK_BOX("RX<===>Z28<===>Signal Generator");

    Instr.init();
    Instr.has_pm();
    Instr.has_sg();
    Instr.sg_reset();
    Instr.sg_set_en_mod(false);
    Instr.sg_set_pl(-60.0);
    Instr.sg_set_en_output(true);

    if (calOP(calParam.mode)) {
        THREAD_TEST_CANCEL
        if (calParam.cal) {
            initProgress("Calibrating RX Reference");
            cal(OUTPUT);
        }
        THREAD_TEST_CANCEL
        if (calParam.check) {
            initProgress("Checking RX Reference");
            checkIt(OUTPUT);
        }
    }
    if (calIO(calParam.mode)) {
        THREAD_TEST_CANCEL
        if (calParam.cal) {
            initProgress("Calibrating RX Reference");
            cal(IO);
        }
        THREAD_TEST_CANCEL
        if (calParam.check) {
            initProgress("Checking RX Reference");
            checkIt(IO);
        }
    }
    THREAD_ENDED
    RD_CAL_CATCH
}

void QCalR1CRXRefThread::cal(io_mode_t mode)
{
    QR1CRXRefModel *modelOP = dynamic_cast<QR1CRXRefModel *>(calParam.model_0);
    QR1CRXRefModel *modelIO = dynamic_cast<QR1CRXRefModel *>(calParam.model_1);

    rx_ref_table_r1cd::data_f_t data;
    rx_filter_160m_table::data_m_t dataFilter;
    double coefReal[ul_filter_tap] = {0.0};
    double coefImag[ul_filter_tap] = {0.0};

    qint32 ref = 30;
    qint32 lnaAtt = 0;
    qint32 att019 = 0;
    double att1 = 0.0;
    double att2 = 0.0;
    double att3 = 0.0;
    quint64 freq = 0;

    qint32 secBfr = -1;
    qint32 secCur = 0;

    SP1401->set_tx_att(30.0,30.0,30.0,30.0);
    SP1401->set_rx_if_filter_sw(sp1401::_160MHz);
    SP2401->set_dds_src(sp2401_r1a::ALL_0);
    SP2401->set_tx_filter_sw(false);
    SP2401->set_tx_pwr_comp(-5.0);
    SP2401->set_rx_pwr_comp(0);
    SP2401->set_rx_filter_sw(sp2401_r1a::_2I3D);
    SP1401->cf()->set_bw(_160M);
    SP1401->set_io_mode(mode);

    Instr.pm_reset();

    for (quint32 i = 0; i < freqRangeCal.freqs.size() ;i ++) {
        freq = freqRangeCal.freqs.at(i);

        SP1401->cf()->m_rx_filter_160m->get(freq,&dataFilter);
        dataFilter._2double(coefReal,coefImag);
        SP2401->set_rx_filter(coefReal,coefImag);
        SP1401->set_rx_freq(freq);

        for (quint32 j = 0;j < ARRAY_SIZE(data.state);j ++) {
            THREAD_TEST_PAUSE_S
            THREAD_TEST_CANCEL
            if (mode == OUTPUT) {
                if (RFVer == R1C || RFVer == R1D || RFVer == R1E) {
                    rx_ref_op_table_r1cd::guess_base_r1c(j,ref,lnaAtt,att019,att1,att2,att3);
                } else if (RFVer == R1F) {
                    rx_ref_op_table_r1cd::guess_base_r1f(j,ref,lnaAtt,att019,att1,att2);
                }
            } else if (mode == IO) {
                if (RFVer == R1C || RFVer == R1D || RFVer == R1E) {
                    rx_ref_io_table_r1cd::guess_base_r1c(j,ref,lnaAtt,att019,att1,att2,att3);
                } else if (RFVer == R1F) {
                    rx_ref_io_table_r1cd::guess_base_r1f(j,ref,lnaAtt,att019,att1,att2);
                }
            }

            SP1401->set_rx_lna_att_sw(r1c::rx_lna_att_t(lnaAtt));
            SP1401->set_rx_att_019_sw(r1c::rx_att_019_t(att019));
            SP1401->set_rx_att(att1,att2,att3);
            msleep(10);

            ajustSG(freq,ref - rx_ref_table_r1cd::cal_rollback(RFVer));
            tuning(att1,att2,att3);

            data.freq = freq;
            data.state[j].lna_att = int16_t(lnaAtt);
            data.state[j].att_019 = int16_t(att019);
            data.state[j].att1 = float(att1);
            data.state[j].att2 = float(att2);
            data.state[j].att3 = float(att3);
            data.state[j].ad_offset = g_0dBFS - ad;
            SP1401->get_temp(0,data.state[j].temp[0]);
            SP1401->get_temp(1,data.state[j].temp[1]);
            SP1401->get_temp(2,data.state[j].temp[2]);
            SP1401->get_temp(3,data.state[j].temp[3]);
            data.time = getCurTime();

            secCur = freq_section(freq,freqRangeCal);

            if (mode == OUTPUT) {
                if (secCur != secBfr) {
                    modelOP->iterTable(j)->at(secCur)->locate2CalTable(modelOP->calTable()->begin() + i);
                }
                modelOP->iterTable(j)->at(secCur)->addOneData();
            } else if (mode == IO) {
                if (secCur != secBfr) {
                    modelIO->iterTable(j)->at(secCur)->locate2CalTable(modelIO->calTable()->begin() + i);
                }
                modelIO->iterTable(j)->at(secCur)->addOneData();
            }
            THREAD_TEST_PAUSE_E
        }
        secBfr = secCur;

        if (mode == OUTPUT) {
            modelOP->calTable()->replace(i,data);
            emit update(modelOP->index(i * 3,0),
                        modelOP->index(i * 3,13),
                        cal_file::RX_REF_OP,
                        secCur);
            SP1401->cf()->add(cal_file::RX_REF_OP,&data);
        } else if (mode == IO) {
            modelIO->calTable()->replace(i,data);
            emit update(modelIO->index(i * 3,0),
                        modelIO->index(i * 3,13),
                        cal_file::RX_REF_IO,
                        secCur);
            SP1401->cf()->add(cal_file::RX_REF_IO,&data);
        }
        addProgressPos(1);
    }

    if (mode == OUTPUT) {
        SP1401->cf()->w(cal_file::RX_REF_OP);
    } else if (mode == IO) {
        SP1401->cf()->w(cal_file::RX_REF_IO);
    }
}

void QCalR1CRXRefThread::checkIt(io_mode_t mode)
{
    uint64_t freq = 0;
    qint64 ad = 0;
    qint32 ref = 0;
    double dBc = 0.0;
    rx_filter_160m_table::data_m_t dataFilter;
    rx_ref_table_r1cd::data_m_t dataRef;
    rx_ref_cal_data data;
    bool res = true;

    SP3301->set_rx_en_tc(RFIdx,true);
    SP3301->set_io_mode(RFIdx,mode);
    SP1401->prepare_cr_rx_ref_cal();

    for (quint32 i = 0;i < freqRangeCheck.freqs.size();i ++) {
        THREAD_TEST_CANCEL
        freq = freqRangeCheck.freqs.at(i);
        data = SP1401->cr_rx_ref_cal()->data(freq);
        res = data.result();;

        SP3301->set_rx_freq(RFIdx,freq);

        for (quint32 j = 0;j < ARRAY_SIZE(dataRef.state);j ++) {
            THREAD_TEST_PAUSE_S
            THREAD_TEST_CANCEL

            if (mode == OUTPUT) {
                SP1401->cf()->m_rx_ref_op->guess_base(RFVer,j,ref);
            } else if (mode == IO) {
                SP1401->cf()->m_rx_ref_io->guess_base(RFVer,j,ref);
            }

            SP3301->set_rx_level(RFIdx,ref);
            msleep(10);

            ajustSG(freq,ref - rx_ref_table_r1cd::cal_rollback(RFVer));

            getADS5474(SP1401,ad);
            ad = dBc_to_ad(ad,-1.0 * rx_ref_table_r1cd::cal_rollback(RFVer));
            dBc = ad_to_dBc(g_0dBFS,ad);

            if (mode == OUTPUT) {
                data.pwr_op[j] = ref + dBc;
            } else if (mode == IO) {
                data.pwr_io[j] = ref + dBc;
            }

            res = abs(dBc) <= spec::cal_rx_ref_accuracy();

            if (!res) {
                Log.set_last_err("%s Fail.Freq:%s,Power:%f(%f)(%s)",
                                 g_threadName.toStdString().c_str(),
                                 freq_string_from(freq).c_str(),
                                 ref + dBc,
                                 double(ref),
                                 string_of(mode).c_str());
                emit threadProcess(RUNNING_EXCEPT);
            }

            if (mode == OUTPUT) {
                data.res_op[j] = res;
            } else if (mode == IO) {
                data.res_io[j] = res;
            }

            data.set_result(data.res_op[0] && data.res_op[1] && data.res_op[2] &&
                            data.res_io[0] && data.res_io[1] && data.res_io[2]);
            data.set_time();
            SP1401->cr_rx_ref_cal()->add(int64_t(freq),data);
            THREAD_TEST_PAUSE_E
        }
        addProgressPos(1);
    }
    SP1401->cr_rx_ref_cal()->update();
    SP1401->ftp_put_cr_rx_ref_cal();
    SP3301->set_rx_en_tc(RFIdx,false);
}

void QCalR1CRXRefThread::tuning(double &att1, double &att2, double &att3)
{
    double *att__1 = &att2; // First  tuning att
    double *att__2 = &att3; // Second tuning att

    if (RFVer == R1F) {
        att__1 = &att1;
        att__2 = &att2;
    }

    double dBc = 0.0;
    quint32 count = 0;

    getADS5474(SP1401,ad,AVERAGE_TIMES);
    ad = dBc_to_ad(ad,-1.0 * rx_ref_table_r1cd::cal_rollback(RFVer));
    dBc = ad_to_dBc(g_0dBFS,ad);

    while (abs(dBc) > 0.3) {
        if (-dBc > *att__2) {
            *att__1 += *att__2 + dBc;
            *att__2 = 0;
            *att__1 = *att__1 < 0.0 ? 0.0 : *att__1;
        } else {
            *att__2 += dBc;
        }
        *att__2 = *att__2 < 0.0  ? 0.0  : *att__2;
        *att__2 = *att__2 > 31.5 ? 31.5 : *att__2;
        ns_hmc624::att(att__1);
        ns_hmc624::att(att__2);

        if (RFVer == R1C || RFVer == R1D || RFVer == R1E) {
            SP1401->set_rx_att2(att2);
            SP1401->set_rx_att3(att3);
        } else if (RFVer == R1F) {
            SP1401->set_rx_att1(att1);
            SP1401->set_rx_att2(att2);
        }

        msleep(10);

        getADS5474(SP1401,ad,AVERAGE_TIMES);
        ad = dBc_to_ad(ad,-1.0 * rx_ref_table_r1cd::cal_rollback(RFVer));
        dBc = ad_to_dBc(g_0dBFS,ad);

        if (++count > 10) {
            break;
        }
    }
}

void QCalR1CRXRefThread::ajustSG(quint64 freq, double pwr)
{
    double pmPwr = 0.0;
    Instr.pm_set_freq(double(freq));
    Instr.sg_set_cw(double(freq));
    Instr.sg_set_pl(pwr + pmIL);
    msleep(10);

    for (quint32 i = 0;i < 10;i ++) {
        Instr.pm_get_pwr(pmPwr);
        if (abs(pwr - pmPwr) < 0.05) {
            break;
        }
        Instr.sg_set_pl(pwr + pmIL + (pwr - pmPwr));
        msleep(10);
        pmIL += (pwr - pmPwr);
    }
    msleep(10);
}


void QExpR1CRXRefThread::run()
{
    initProgress("Exporting RX Reference",100);

    QR1CRXRefModel *modelOP = dynamic_cast<QR1CRXRefModel *>(calParam.model_0);
    QR1CRXRefModel *modelIO = dynamic_cast<QR1CRXRefModel *>(calParam.model_1);

    quint64 freq = 0;
    int secBfr = -1;
    int secCur = 0;

    rx_ref_table_r1cd::data_f_t data;

    if (calOP(calParam.mode)) {
        SP1401->cf()->map2buf(cal_file::RX_REF_OP);

        for (quint32 i = 0;i < freqRangeCal.freqs.size();i ++) {
            freq = freqRangeCal.freqs.at(i);
            SP1401->cf()->m_rx_ref_op->get_base(freq,&data);
            secCur = freq_section(freq,freqRangeCal);
            for (quint32 j = 0;j < ARRAY_SIZE(data.state);j ++) {
                if (secCur != secBfr) {
                    modelOP->iterTable(j)->at(secCur)->locate2CalTable(modelOP->calTable()->begin() + i);
                }
                modelOP->iterTable(j)->at(secCur)->addOneData();
            }
            secBfr = secCur;
            modelOP->calTable()->replace(i,data);
        }
        emit update(modelOP->index(0,0),
                    modelOP->index(freqRangeCal.freqs.size(),13),
                    cal_file::RX_REF_OP,
                    secCur);
    }

    secBfr = -1;
    secCur = 0;

    if (calIO(calParam.mode)) {
        SP1401->cf()->map2buf(cal_file::RX_REF_IO);

        for (quint32 i = 0;i < freqRangeCal.freqs.size();i ++) {
            freq = freqRangeCal.freqs.at(i);
            SP1401->cf()->m_rx_ref_io->get_base(freq,&data);
            secCur = freq_section(freq,freqRangeCal);
            for (quint32 j = 0;j < ARRAY_SIZE(data.state);j ++) {
                if (secCur != secBfr) {
                    modelIO->iterTable(j)->at(secCur)->locate2CalTable(modelIO->calTable()->begin() + i);
                }
                modelIO->iterTable(j)->at(secCur)->addOneData();
            }
            secBfr = secCur;
            modelIO->calTable()->replace(i,data);
        }
        emit update(modelIO->index(0,0),
                    modelIO->index(freqRangeCal.freqs.size(),13),
                    cal_file::RX_REF_IO,
                    secCur);
    }

    SET_PROG_POS(100);
    THREAD_ENDED
}
