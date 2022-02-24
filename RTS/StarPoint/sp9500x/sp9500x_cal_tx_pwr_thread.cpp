#include "sp9500x_cal_tx_pwr_thread.h"
#include "sp9500x_cal_tx_pwr_childwidgets.h"
#include "cal_widget.h"
#include "spec.h"
#include "test_data.hpp"
#include "algo_math.hpp"

using namespace std;
using namespace ns_sp1403;
using namespace ns_sp2406;
using namespace ns_sp9500x;
using namespace NS_SP9500X;
using namespace set_helper;

void Q_Cal_TX_Pwr_Thread::cal()
{
    CAL_THREAD_START("TX Power",(1));

//    setTempRange(2.0);

    Q_Cal_TX_Pwr_Widget *p = dynamic_cast<Q_Cal_TX_Pwr_Widget *>(parent());
    Q_TX_Pwr_Config_Delegate *delegate = p->childs->configDelegate;

    bool cal_TX0_Output = delegate->checkBoxTX0_Output->isChecked();
    bool cal_TX0_IO = delegate->checkBoxTX0_IO->isChecked();
    bool cal_TX1 = delegate->checkBoxTX1->isChecked();

    freqString = p->ui->textEditRFFreqs->toPlainText().toStdString();

    parse(freqString,freqRange);

    if ((cal_TX0_Output || cal_TX0_IO || cal_TX1) == false) {
        return;
    }

    Instr.init();
    Instr.has_pm();
    Instr.pm_reset();

    if (cal_TX0_IO) {
        THREAD_CHECK_BOX("IO <===> Power Meter");
        port = port_t::TX0;
        mode = IO;
        calIt();
    }
    if (cal_TX0_Output) {
        THREAD_CHECK_BOX("TX0 <===> Power Meter");
        port = port_t::TX0;
        mode = OUTPUT;
        calIt();
    }
    if (cal_TX1) {
        THREAD_CHECK_BOX("TX1 <===> Power Meter");
        port = port_t::TX1;
        mode = OUTPUT;
        calIt();
    }
}

void Q_Cal_TX_Pwr_Thread::calIt()
{
    uint64_t freq = 0;
    int i = 0;

    tx_pwr_table_t::data_f_t data;

    cal_table_t table = cal_table_t::TX0_Pwr_Output;

    if (port == port_t::TX0) {
        table = (mode == OUTPUT ? cal_table_t::TX0_Pwr_Output : cal_table_t::TX0_Pwr_IO);
    } else {
        table = cal_table_t::TX1_Pwr;
    }

    SP1403->set_io_mode(port,mode);
    init();

    for (set<uint64_t>::iterator iter = freqRange.freq.begin();iter != freqRange.freq.end();++iter,++i) {
        THREAD_TEST_CANCEL

        freq = *iter;

        SP1403->tx_state(freq,mode,data);

        SP1403->set_tx_freq(freq);
        Instr.pm_set_freq(freq);

        coarseTuning(freq,data);
        fineTuning(freq,data);

        data.set_key(freq);
        SP1403->cal_file()->add(table,&data);
        emit uiUpdate(i,i,table);
    }
    SP1403->cal_file()->persistence(table);
}

void Q_Cal_TX_Pwr_Thread::coarseTuning(const uint64_t freq, data_f_tx_pwr &data)
{
    const double target = SP1403->tx_base_pwr(freq,mode);
    double pwrPM = 0.0;
    double offset = 0.0;
    double att0 = data_f_tx_pwr::att_double(data.att[0]);
    int count = 0;
    uint8_t att_idx = 0;

    if (is_before(HWVer,hw_ver_t::R1B)) {
        att_idx = (port == port_t::TX0 ? rd::sp1403_r1a::att_t::TX0_ATT0 : rd::sp1403_r1b::att_t::TX1_ATT0);
    }

    SP2406->set_dl_pwr_comp(data.d_gain);
    SP1403->set_tx_state(port,data);

    Instr.pm_get_pwr(pwrPM);
    offset = target - pwrPM;

    while (abs(offset) > 0.5) {
        att0 -= offset;
        att0 = linear_quantify(0.0,0.5,att0);
        att0 = att0 < 0.0 ? 0.0 : att0;
        data.set_att(0,att0);

        SP1403->set_att(att_idx,att0);
        msleep(50);

        Instr.pm_get_pwr(pwrPM);
        offset = target - pwrPM;

        if (++count > 10) {
            break;
        }
    }
}

void Q_Cal_TX_Pwr_Thread::fineTuning(const uint64_t freq,data_f_tx_pwr &data)
{
    const double target = SP1403->tx_base_pwr(freq,mode);
    double pwrPM = 0.0;
    double offset = 0.0;
    int count = 0;

    Instr.pm_get_pwr(pwrPM);
    offset = target - pwrPM;

    while (abs(offset) > 0.02) {
        data.d_gain += float(offset);

        SP2406->set_dl_pwr_comp(data.d_gain);
        msleep(50);

        Instr.pm_get_pwr(pwrPM);
        offset = target - pwrPM;

        if (++count > 10) {
            break;
        }
    }
}

void Q_Cal_TX_Pwr_Thread::init()
{
    data_f_tx_pwr data;
    data.set_att(31.5,31.5,31.5,31.5);

    SP1403->set_tx_state(port_t::TX0,data);
    SP1403->set_tx_state(port_t::TX1,data);
    SP2406->set_dl_src(dl_src_t::Disable);
    SP2406->set_dl_src(0,dl_src_t::DDS0);
    SP2406->set_dl_src_dds0_gain(-6.0);
    SP2406->set_dl_cpri_sr(dl_cpri_sr_t::_12288);
    SP2406->set_dl_lpf_en(false);
    SP2406->set_dl_hbf0_en(true);
    SP2406->set_dl_hbf1_en(true);
    SP2406->set_dl_cf_en(false);
    SP2406->set_dl_jesd_src(dl_jesd_src_t::DUC);
    SP2406->set_dl_pwr_comp(0.0);
    msleep(10);
}

void Q_Cal_TX_Pwr_Thread::check()
{

}


void Q_Exp_TX_Pwr_Thread::run()
{
    initProgress("Exporting TX Power",100);
    addProgressPos(100);
    THREAD_ENDED
}
