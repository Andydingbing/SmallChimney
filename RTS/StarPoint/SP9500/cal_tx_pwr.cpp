#include "cal_tx_pwr.h"
#include "algo_math.hpp"
#include "algo_chip_att.hpp"

using namespace std;
using namespace set_helper;

void Q_Cal_TX_Pwr_Widget::Thread::kase()
{
    KASE_THREAD_START("TX Power",totalPts);

    bool isCal       = Config("Cal", QCheckBox)->isChecked();
    bool isCheck     = Config("Check", QCheckBox)->isChecked();
    bool isCalOutput = Config("Mode(Output)", QCheckBox)->isChecked();
    bool isCalIO     = Config("Mode(IO)", QCheckBox)->isChecked();

    _freq_range = parse<uint64_t>(Config("Freq", QTextEdit)->toPlainText().toStdString());

    if (isCalOutput) {
        checkPoint("TX<===>Power Meter");
    }

    if (isCalIO && !isCalOutput) {
        checkPoint("IO<===>Power Meter");
    }


    Instr.init();
    Instr.has_pm();

    init();

    if (isCalOutput) {
        cancelPoint();

        if (isCal) {
            initProgress("Calibrating TX Power");
            calIt(io_mode_t::OUTPUT);
        }

        cancelPoint();

        if (isCheck) {
            initProgress("Checking TX Power");
            checkIt(io_mode_t::OUTPUT);
        }
    }

    if (isCalOutput && isCalIO) {
        checkPoint("Output Complete,IO<===>Power Meter");
    }

    if (isCalIO) {
        cancelPoint();

        if (isCal) {
            initProgress("Calibrating TX Power");
            calIt(io_mode_t::IO);
        }

        cancelPoint();

        if (isCheck) {
            initProgress("Checking TX Power");
            checkIt(io_mode_t::IO);
        }
    }
}

void Q_Cal_TX_Pwr_Widget::Thread::calIt(io_mode_t mode)
{
    quint64 freq = 0;

    cal_table_t table = cal_table_t::TX_PWR_OP;
    tx_pwr_table_t::data_f_t data;
    data_m_tx_filter data_filter;

    double att0 = 0.0;
    double att1 = 0.0;
    double att2 = 0.0;
    double att3 = 0.0;
    double d_gain = -5.0;
    double target = 0.0;

    set<uint64_t>::const_iterator iter_freq = _freq_range.freq.cbegin();

    if (mode._to_integral() == io_mode_t::OUTPUT) {
        target = r1c::tx_base_pwr_op;
        table = cal_table_t::TX_PWR_OP;
    } else {
        target = r1c::tx_base_pwr_io;
        table = cal_table_t::TX_PWR_IO;
    }

    DECLARE_DB_ITEM_TABLE(tx_filter_table,tx_filter_table_t,SP1401->data_base()->db(cal_table_t::TX_FILTER_160));

    SP1401->set_io_mode(mode);

    Instr.pm_reset();

    for (uint32_t i = 0;iter_freq != _freq_range.freq.cend();++iter_freq,++i) {
        cancelPoint();

        freq = *iter_freq;

        tx_filter_table->get(freq,data_filter);

        SP2401->set_tx_filter(data_filter);
        SP1401->set_tx_freq(freq);

        Instr.pm_set_freq(double(freq));

        if (mode._to_integral() == io_mode_t::OUTPUT) {
            initTXBaseStateOP(att0,att1,att2,att3,d_gain);
        } else if (mode._to_integral() == io_mode_t::IO) {
            initTXBaseStateIO(att0,att1,att2,att3,d_gain);
        }

        if (is_between(HWVer,R1C,R1E)) {
            coarseTuning(target,att0,att1);
        } else if (is_after(HWVer,R1F)) {
            coarseTuning(target,att1,att0);
        }

        fineTuning(target,d_gain);

        data.freq = freq;
        data.d_gain = d_gain;
        data.att0 = att0;
        data.att1 = att1;
        data.att2 = att2;
        data.att3 = att3;
        SP1401_R1C->get_temp(4,data.temp[0]);
        SP1401_R1C->get_temp(5,data.temp[1]);
        SP1401_R1C->get_temp(6,data.temp[2]);
        SP1401_R1C->get_temp(7,data.temp[3]);

        SP1401->data_base()->add(table,&data);
        emit uiUpdate(i,i,table);

        addProgressPos(1);
    }

    SP1401->data_base()->persistence(table);
}

void Q_Cal_TX_Pwr_Widget::Thread::checkIt(io_mode_t mode)
{
    cal_table_t table = cal_table_t::TX_PWR_OP;
    double pwrBase = 0.0;
    uint64_t freq = 0;
    double pwr = 0.0;

    set<uint64_t>::const_iterator iter_freq = _freq_range.freq.cbegin();

    if (mode._to_integral() == io_mode_t::OUTPUT) {
        pwrBase = r1c::tx_base_pwr_op;
        table = cal_table_t::TX_PWR_OP;
    } else {
        pwrBase = r1c::tx_base_pwr_io;
        table = cal_table_t::TX_PWR_IO;
    }

    tx_pwr_table_t::data_f_t data;
    bool res = true;

    SP3301->set_tx_en_tc(RFIdx,true);
    SP3301->set_io_mode(RFIdx,mode);

    for (uint32_t i = 0;iter_freq != _freq_range.freq.cend();++iter_freq,++i) {
        cancelPoint();

        freq = *iter_freq;

        SP3301->set_tx_freq(RFIdx,freq);
        SP3301->set_tx_pwr(RFIdx,pwrBase);
        msleep(20);

        Instr.pm_get_pwr(double(freq),pwr);

        res = abs(pwr - pwrBase) <= spec::cal_tx_base_pwr_accuracy();

        if (!res) {
            Log.set_last_err("%s Fail.Freq:%s,Power:%f(%f)(%s)",
                             g_threadName.toStdString().c_str(),
                             freq_string_from(freq).c_str(),
                             pwr,
                             pwrBase,
                             mode._to_string());
            emit threadProcess(Running_Except);
        }

        updateTotalResult(res);
        addProgressPos(1);
    }

    SP3301->set_tx_en_tc(RFIdx,false);
}

void Q_Cal_TX_Pwr_Widget::Thread::init()
{
    tx_lol_table_t::data_m_t dataLOL;
    tx_sideband_table_t::data_m_t dataSB;

    SP1401_R1C->set_rx_att_019_sw(r1c::RX_ATT_19);
    SP1401_R1C->set_rx_lna_att_sw(r1c::RX_ATT);
    SP1401_R1C->set_rx_att(30.0,30.0,30.0);

    SP2401->set_tx_filter_sw(true);
    SP2401->set_dds_src(sp2401_r1a::SINGLE_TONE);
    SP2401->set_dds1(0.0);
    SP2401->set_duc_dds(0.0);

    SP1401->data_base()->_tx_lol.get(2000000000,dataLOL);
    SP1401->data_base()->_tx_sideband.get(2000000000,dataSB);

    SP2401->set_tx_phase_rotate_I(double(dataSB.th));
    SP2401->set_tx_amplitude_balance(dataSB.am_i,dataSB.am_q);
    SP2401->set_tx_dc_offset(quint16(dataLOL.dc_i),quint16(dataLOL.dc_q));
}

void Q_Cal_TX_Pwr_Widget::Thread::initTXBaseStateOP(double &att0, double &att1, double &att2, double &att3, double &dGain)
{
    att0 = 0.0;
    att1 = 0.0;
    att2 = 0.0;
    att3 = 0.0;
    dGain = 0.0;

    data_f_tx_pwr data;

    if (HWVer == R1C || HWVer == R1D || HWVer == R1E) {
        SP1401->tx_guess_base_state(io_mode_t::OUTPUT,data);
//        SP1401->tx_guess_base_state( guess_base_r1c(att0,att1,att2,att3,dGain);
    } else if (HWVer == R1F) {
        tx_pwr_op_table_r1c::guess_base_r1f(att0,att1,att2,dGain);
    }

    SP2401->set_tx_pwr_comp(dGain);
    SP1401_R1C->set_tx_att(att0,att1,att2,att3);
    msleep(50);
}

void Q_Cal_TX_Pwr_Widget::Thread::initTXBaseStateIO(double &att0, double &att1, double &att2, double &att3, double &dGain)
{
    att0 = 0.0;
    att1 = 0.0;
    att2 = 0.0;
    att3 = 0.0;
    dGain = 0.0;

    if (HWVer == R1C || HWVer == R1D || HWVer == R1E) {
        tx_pwr_io_table_r1c::guess_base_r1c(att0,att1,att2,att3,dGain);
    } else if (HWVer == R1F) {
        tx_pwr_io_table_r1c::guess_base_r1f(att0,att1,att2,dGain);
    }

    SP2401->set_tx_pwr_comp(dGain);
    SP1401_R1C->set_tx_att(att0,att1,att2,att3);
    msleep(50);
}

void Q_Cal_TX_Pwr_Widget::Thread::coarseTuning(double target, double &att0, double &att1)
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

        if (is_rf_ver_between(HWVer,R1C,R1E)) {
            SP1401->set_tx_att0(att0);
            SP1401->set_tx_att1(att1);
        } else if (is_rf_ver_after(HWVer,R1F)) {
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

void Q_Cal_TX_Pwr_Widget::Thread::fineTuning(double target, double &dGain)
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
