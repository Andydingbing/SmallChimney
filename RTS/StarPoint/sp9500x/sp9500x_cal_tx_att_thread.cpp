#include "sp9500x_cal_tx_att_thread.h"
#include "sp9500x_cal_tx_att_childwidgets.h"
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

void Q_Cal_TX_Att_Thread::cal()
{
    CAL_THREAD_START("TX Attenuation",(1));

//    setTempRange(2.0);

    Q_Cal_TX_Att_Widget *p = dynamic_cast<Q_Cal_TX_Att_Widget *>(parent());
    Q_TX_Att_Config_Delegate *delegate = p->childs->configDelegate;

    bool cal_TX0_Output = delegate->checkBoxTX0_Output->isChecked();
    bool cal_TX0_IO = delegate->checkBoxTX0_IO->isChecked();
    bool cal_TX1 = delegate->checkBoxTX1->isChecked();

    freqString = p->ui->textEditRFFreqs->toPlainText().toStdString();

    parse(freqString,freqRange);

    if ((cal_TX0_Output || cal_TX0_IO || cal_TX1) == false) {
        return;
    }

    Instr.init();
    Instr.has_sa();
    Instr.sa_reset();
    Instr.sa_set_span(20000.0);
    Instr.sa_set_det(sa::DET_AVERAGE);
    Instr.sa_set_rbw(3000.0,false);
    Instr.sa_set_vbw(3000.0,false);

    if (cal_TX0_IO) {
        THREAD_CHECK_BOX("IO <===> Spectrum");
        port = port_t::TX0;
        mode = IO;
        calIt();
    }
    if (cal_TX0_Output) {
        THREAD_CHECK_BOX("TX0 <===> Spectrum");
        port = port_t::TX0;
        mode = OUTPUT;
        calIt();
    }
    if (cal_TX1) {
        THREAD_CHECK_BOX("TX1 <===> Spectrum");
        port = port_t::TX1;
        mode = OUTPUT;
        calIt();
    }
}

void Q_Cal_TX_Att_Thread::calIt()
{
    uint64_t freq = 0;
    uint32_t attIdx = 0;
    double att = 0.0;
    double basePwr = 0.0;
    double ref[2] = { 30.0,30.0 };
    double pwrSA[2] = { 0.0,0.0 };
    double offset_SA = 0.0;
    int i = 0;

    tx_pwr_table_t::data_f_t baseState;
    tx_pwr_table_t::data_f_t currentState;
    tx_att_table_t::data_f_t data;

    cal_table_t table = cal_table_t::TX0_Att_Output;

    if (port == port_t::TX0) {
        table = (mode == OUTPUT ? cal_table_t::TX0_Att_Output : cal_table_t::TX0_Att_IO);
    } else {
        table = cal_table_t::TX1_Att;
    }

    SP1403->set_io_mode(port,mode);
    init();

    for (set<uint64_t>::iterator iter = freqRange.freq.begin();iter != freqRange.freq.end();++iter,++i) {
        THREAD_TEST_CANCEL

        freq = *iter;

        SP1403->set_tx_freq(freq);
        SP1403->tx_state(freq,mode,baseState);
        basePwr = SP1403->tx_base_pwr(freq,mode);

        Instr.sa_set_ref(ref[0]);
        Instr.sa_set_en_preamp(false);
        Instr.sa_set_mech_att(true);
        Instr.sa_set_avg_trace(false,0);
        Instr.sa_sweep_once();
        msleep(10);

        attIdx = 0;
        for (att = -10.0;att <= data_f_tx_pwr::att_double(baseState.att[0]) + 90.0;att += 1.0) {
            SP1403->tx_state(att,baseState,currentState);
            SP1403->set_tx_state(port,currentState);

            Instr.sa_set_ref_auto(basePwr - att,ref[1],offset_SA);
            Instr.sa_sweep_once();
            Instr.sa_set_peak_search(sa::PEAK_HIGHEST);
            Instr.sa_get_marker_pwr(pwrSA[0]);

            data.offset[attIdx] = 0;

            ++attIdx;
        }

        data.set_key(freq);
        SP1403->cal_file()->add(table,&data);
        emit uiUpdate(i,i,table);
    }
    SP1403->cal_file()->persistence(table);
}

void Q_Cal_TX_Att_Thread::init()
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

void Q_Cal_TX_Att_Thread::check()
{

}

void Q_Exp_TX_Att_Thread::run()
{
    initProgress("Exporting TX Power",100);
    addProgressPos(100);
    THREAD_ENDED
}
