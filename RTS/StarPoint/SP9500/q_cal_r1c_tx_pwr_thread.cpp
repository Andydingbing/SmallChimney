#include "q_cal_r1c_tx_pwr_thread.h"
#include "q_model_tx_pwr.h"
#include "algo_math.hpp"
#include "algo_chip_att.hpp"
#include "spec.h"
#include "test_data.hpp"

using namespace rd::ns_sp1401;
using namespace rd::ns_sp1401::r1c;
using namespace rd::ns_sp2401;

void QCalR1CTXPwrThread::run()
{
    RD_CAL_TRY
    CAL_THREAD_START("TX Power",totalPts);

    if (calOP(calParam.mode)) {
        THREAD_CHECK_BOX("TX<===>Power Meter");
    }
    if (calIO(calParam.mode) && !calOP(calParam.mode)) {
        THREAD_CHECK_BOX("IO<===>Power Meter");
    }

    Instr.init();
    Instr.has_pm();

    init();

    if (calOP(calParam.mode)) {
        THREAD_TEST_CANCEL
        if (calParam.cal) {
            initProgress("Calibrating TX Power");
            calIt(OUTPUT);
        }
        THREAD_TEST_CANCEL
        if (calParam.check) {
            initProgress("Checking TX Power");
            checkIt(OUTPUT);
        }
    }

    if (calOP(calParam.mode) && calIO(calParam.mode)) {
        THREAD_CHECK_BOX("Output Complete,IO<===>Power Meter");
    }

    if (calIO(calParam.mode)) {
        THREAD_TEST_CANCEL
        if (calParam.cal) {
            initProgress("Calibrating TX Power");
            calIt(IO);
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

void QCalR1CTXPwrThread::calIt(io_mode_t mode)
{
    QTXPwrModel *model_op = dynamic_cast<QTXPwrModel *>(calParam.model_0);
    QTXPwrModel *model_io = dynamic_cast<QTXPwrModel *>(calParam.model_1);

    quint64 freq = 0;

    tx_pwr_table_r1c::data_f_t data;
    tx_filter_160m_table::data_m_t dataFilter;
    double coefReal[dl_filter_tap_2i] = {0.0};
    double coefImag[dl_filter_tap_2i] = {0.0};

    double att0 = 0.0;
    double att1 = 0.0;
    double att2 = 0.0;
    double att3 = 0.0;
    double d_gain = -5.0;
    double target = 0.0;

    qint32 secBfr = -1;
    qint32 secCur = 0;

    target = (mode == OUTPUT ? tx_base_pwr_op : tx_base_pwr_io);

    SP1401->set_io_mode(mode);

    Instr.pm_reset();

    for (quint32 i = 0;i < freqRangeCal.freqs.size();i ++) {
        THREAD_TEST_PAUSE_S
        THREAD_TEST_CANCEL

        freq = freqRangeCal.freqs.at(i);
        SP1401->cf()->m_tx_filter_160m->get(freq,&dataFilter);
        dataFilter._2double(coefReal,coefImag);

        SP2401->set_tx_filter(coefReal,coefImag);
        SP1401->set_tx_freq(freq);

        Instr.pm_set_freq(double(freq));

        if (mode == OUTPUT) {
            initTXBaseStateOP(att0,att1,att2,att3,d_gain);
        } else if (mode == IO) {
            initTXBaseStateIO(att0,att1,att2,att3,d_gain);
        }

        if (is_between(RFVer,R1C,R1E)) {
            coarseTuning(target,att0,att1);
        } else if (is_after(RFVer,R1F)) {
            coarseTuning(target,att1,att0);
        }

        fineTuning(target,d_gain);

        data.freq = freq;
        data.d_gain = d_gain;
        data.att0 = att0;
        data.att1 = att1;
        data.att2 = att2;
        data.att3 = att3;
        SP1401->get_temp(4,data.temp[0]);
        SP1401->get_temp(5,data.temp[1]);
        SP1401->get_temp(6,data.temp[2]);
        SP1401->get_temp(7,data.temp[3]);
        data.time = getCurTime();

        secCur = freq_section(freq,freqRangeCal);

        if (mode == OUTPUT) {
            if (secCur != secBfr) {
                model_op->iterTable()->at(secCur)->locate2CalTable(model_op->calTable()->begin() + i);
                secBfr = secCur;
            }
            model_op->calTable()->replace(i,data);
            model_op->iterTable()->at(secCur)->addOneData();
            SP1401->cf()->add(cal_file::TX_PWR_OP,&data);
            emit update(model_op->index(i,0),model_op->index(i,11),cal_file::TX_PWR_OP,secCur);
        } else if (mode == IO) {
            if (secCur != secBfr) {
                model_io->iterTable()->at(secCur)->locate2CalTable(model_io->calTable()->begin() + i);
                secBfr = secCur;
            }
            model_io->calTable()->replace(i,data);
            model_io->iterTable()->at(secCur)->addOneData();
            SP1401->cf()->add(cal_file::TX_PWR_IO,&data);
            emit update(model_io->index(i,0),model_io->index(i,11),cal_file::TX_PWR_IO,secCur);
        }
        addProgressPos(1);
        THREAD_TEST_PAUSE_E
    }

    if (mode == OUTPUT) {
        SP1401->cf()->w(cal_file::TX_PWR_OP);
        SP1401->cf()->m_tx_pwr_op->save_as("c:\\tx_power_op.txt");
    }else if (mode == IO) {
        SP1401->cf()->w(cal_file::TX_PWR_IO);
        SP1401->cf()->m_tx_pwr_io->save_as("c:\\tx_power_io.txt");
    }
}

void QCalR1CTXPwrThread::checkIt(io_mode_t mode)
{
    const double pwrBase = (mode == OUTPUT ? tx_base_pwr_op : tx_base_pwr_io);
    uint64_t freq = 0;
    double pwr = 0.0;

    tx_base_pwr_cal_data data;
    bool res = true;

    SP3301->set_tx_en_tc(RFIdx,true);
    SP3301->set_io_mode(RFIdx,mode);
    SP1401->prepare_cr_tx_base_pwr_cal();

    for (quint32 i = 0;i < freqRangeCheck.freqs.size();i ++) {
        THREAD_TEST_PAUSE_S
        THREAD_TEST_CANCEL
        freq = freqRangeCheck.freqs.at(i);
        data = SP1401->cr_tx_base_pwr_cal()->data(freq);
        res = data.result();

        SP3301->set_tx_freq(RFIdx,freq);
        SP3301->set_tx_pwr(RFIdx,pwrBase);
        msleep(20);

        Instr.pm_get_pwr(double(freq),pwr);

        if (mode == OUTPUT) {
            data.pwr_op = pwr;
        } else if (mode == IO) {
            data.pwr_io = pwr;
        }

        res = abs(pwr - pwrBase) <= spec::cal_tx_base_pwr_accuracy();

        if (!res) {
            Log.set_last_err("%s Fail.Freq:%s,Power:%f(%f)(%s)",
                             g_threadName.toStdString().c_str(),
                             freq_string_from(freq).c_str(),
                             pwr,
                             pwrBase,
                             string_of(mode).c_str());
            emit threadProcess(RUNNING_EXCEPT);
        }

        if (mode == OUTPUT) {
            data.res_op = res;
        } else if (mode == IO) {
            data.res_io = res;
        }

        data.set_result(data.res_op && data.res_io);
        data.set_time();
        SP1401->cr_tx_base_pwr_cal()->add(freq,data);
        updateTotalResult(res);
        addProgressPos(1);
        THREAD_TEST_PAUSE_E
    }
    SP1401->cr_tx_base_pwr_cal()->update();
    SP1401->ftp_put_cr_tx_base_pwr_cal();
    SP3301->set_tx_en_tc(RFIdx,false);
}

void QCalR1CTXPwrThread::init()
{
    tx_lol_table_r1cd::data_m_t	dataLOL;
    tx_sb_table_r1cd::data_m_t	dataSB;

    SP1401->cf()->set_bw(_160M);
    SP1401->set_rx_att_019_sw(r1c::RX_ATT_19);
    SP1401->set_rx_lna_att_sw(r1c::RX_ATT);
    SP1401->set_rx_att(30.0,30.0,30.0);

    SP2401->set_tx_filter_sw(true);
    SP2401->set_dds_src(sp2401_r1a::SINGLE_TONE);
    SP2401->set_dds1(0.0);
    SP2401->set_duc_dds(0.0);

    SP1401->cf()->m_tx_sb->get(2000000000,&dataSB);
    SP1401->cf()->m_tx_lol->get(2000000000,&dataLOL);
    SP2401->set_tx_phase_rotate_I(double(dataSB.th));
    SP2401->set_tx_amplitude_balance(dataSB.am_i,dataSB.am_q);
    SP2401->set_tx_dc_offset(quint16(dataLOL.dc_i),quint16(dataLOL.dc_q));
}

void QCalR1CTXPwrThread::initTXBaseStateOP(double &att0, double &att1, double &att2, double &att3, double &dGain)
{
    att0 = 0.0;
    att1 = 0.0;
    att2 = 0.0;
    att3 = 0.0;
    dGain = 0.0;

    if (RFVer == R1C || RFVer == R1D || RFVer == R1E) {
        tx_pwr_op_table_r1c::guess_base_r1c(att0,att1,att2,att3,dGain);
    } else if (RFVer == R1F) {
        tx_pwr_op_table_r1c::guess_base_r1f(att0,att1,att2,dGain);
    }

    SP2401->set_tx_pwr_comp(dGain);
    SP1401->set_tx_att(att0,att1,att2,att3);
    msleep(50);
}

void QCalR1CTXPwrThread::initTXBaseStateIO(double &att0, double &att1, double &att2, double &att3, double &dGain)
{
    att0 = 0.0;
    att1 = 0.0;
    att2 = 0.0;
    att3 = 0.0;
    dGain = 0.0;

    if (RFVer == R1C || RFVer == R1D || RFVer == R1E) {
        tx_pwr_io_table_r1c::guess_base_r1c(att0,att1,att2,att3,dGain);
    } else if (RFVer == R1F) {
        tx_pwr_io_table_r1c::guess_base_r1f(att0,att1,att2,dGain);
    }

    SP2401->set_tx_pwr_comp(dGain);
    SP1401->set_tx_att(att0,att1,att2,att3);
    msleep(50);
}

void QCalR1CTXPwrThread::coarseTuning(double target, double &att0, double &att1)
{
    double pwrPM = -100.0;
    double offset = -100.0;
    int count = 0;

    Instr.pm_get_pwr(pwrPM);
    offset = target - pwrPM;

    while (abs(offset) > 0.5) {
        if (offset > att0) { // Even if att0 == 0.0,this is still correct.
            att1 -= (offset - att0);
            att0 = 0.0;
            att1 = att1 < 0.0 ? 0.0 : att1;
        } else {
            att0 -= offset;
        }
        att0 = att0 > 20.0 ? 20.0 : att0;
        ns_hmc624::att(&att0);
        ns_hmc624::att(&att1);

        if (is_rf_ver_between(RFVer,R1C,R1E)) {
            SP1401->set_tx_att0(att0);
            SP1401->set_tx_att1(att1);
        } else if (is_rf_ver_after(RFVer,R1F)) {
            SP1401->set_tx_att0(att1);
            SP1401->set_tx_att1(att0);
        }
        msleep(50);

        Instr.pm_get_pwr(pwrPM);
        offset = target - pwrPM;

        if (++count > 10) {
            break;
        }
    };
}

void QCalR1CTXPwrThread::fineTuning(double target, double &dGain)
{
    double pwrPM = -100.0;
    double offset = -100.0;
    int count = 0;

    Instr.pm_get_pwr(pwrPM);
    offset = target - pwrPM;

    while (abs(offset) > 0.02) {
        dGain += offset;
        SP2401->set_tx_pwr_comp(dGain);
        msleep(50);

        Instr.pm_get_pwr(pwrPM);
        offset = target - pwrPM;

        if (++count > 10) {
            Log.stdprintf(" > 10 !!!\n");
            break;
        }
    }
}


void QExpR1CTXPwrThread::run()
{
    initProgress("Exporting Tx Power",100);

    QTXPwrModel *model_op = dynamic_cast<QTXPwrModel *>(calParam.model_0);
    QTXPwrModel *model_io = dynamic_cast<QTXPwrModel *>(calParam.model_1);

    CalIOMode calMode = calParam.mode;
    quint64 freq = 0;
    int secBfr = -1;
    int secCur = 0;

    tx_pwr_table_r1c::data_f_t data;

    if (calOP(calMode)) {
        SP1401->cf()->map2buf(cal_file::TX_PWR_OP);

        for (quint32 i = 0;i < freqRangeCal.freqs.size();i ++) {
            freq = freqRangeCal.freqs.at(i);
            SP1401->cf()->m_tx_pwr_op->get_base(freq,&data);
            secCur = freq_section(freq,freqRangeCal);
            if (secCur != secBfr) {
                model_op->iterTable()->at(secCur)->locate2CalTable(model_op->calTable()->begin() + i);
                secBfr = secCur;
            }
            model_op->calTable()->replace(i,data);
            model_op->iterTable()->at(secCur)->addOneData();
        }

        emit update(model_op->index(0,0),
                    model_op->index(freqRangeCal.freqs.size(),11),
                    cal_file::TX_PWR_OP,
                    secCur);
    }

    secBfr = -1;
    secCur = 0;

    if (calIO(calMode)) {
        SP1401->cf()->map2buf(cal_file::TX_PWR_IO);

        for (quint32 i = 0;i < freqRangeCal.freqs.size();i ++) {
            freq = freqRangeCal.freqs.at(i);
            SP1401->cf()->m_tx_pwr_io->get_base(freq,&data);
            secCur = freq_section(freq,freqRangeCal);
            if (secCur != secBfr) {
                model_io->iterTable()->at(secCur)->locate2CalTable(model_io->calTable()->begin() + i);
                secBfr = secCur;
            }
            model_io->calTable()->replace(i,data);
            model_io->iterTable()->at(secCur)->addOneData();
        }

        emit update(model_io->index(0,0),
                    model_io->index(freqRangeCal.freqs.size(),11),
                    cal_file::TX_PWR_OP,
                    secCur);
    }

    SET_PROG_POS(100);
    THREAD_ENDED
}
