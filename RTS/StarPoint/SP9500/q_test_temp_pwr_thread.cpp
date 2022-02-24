#include "q_test_temp_pwr_thread.h"
#include "q_model_temp_pwr.hpp"
#include "algo_math.hpp"
#include "spec.h"
#include "q_test_freq_res_widget.h"

void QTestR1CTempPwrThread::run()
{
    RD_TEST_TRY

    param = static_cast<TestTempPwrParam *>(Param);

    if (param->ch == sp1401::CH_TX) {
        Tx();
    } else {
        Rx();
    }

    THREAD_ENDED
    RD_TEST_CATCH
}

void QTestR1CTempPwrThread::Tx()
{
    Instr.init();
    Instr.has_sa();
    Instr.sa_reset();
    Instr.sa_set_span(20000.0);
    Instr.sa_set_rbw(1000.0,true);
    Instr.sa_set_vbw(1000.0,true);
    Instr.sa_set_ref(20.0);
    Instr.sa_sweep_once();

    uint64_t freq = 0;
    double pwr = 0.0;
    double temp0 = 0.0;
    double temp1 = 0.0;
    int statesCnt = 0;

    vector<common_atts_t> states;
    vector<common_atts_t>::iterator iter;

    FreqString.star = param->FreqStar.toStdString();
    FreqString.stop = param->FreqStop.toStdString();
    FreqString.step = param->FreqStep.toStdString();
    parse_range_freq_string(FreqString,FreqRange);

    tx_lol_table_r1cd::data_m_t	dataLOL;
    tx_sb_table_r1cd::data_m_t	dataSB;

    SP1401->tx_att_states(states);
    SP2401->set_dds_src(sp2401_r1a::SINGLE_TONE);
    SP2401->set_duc_dds(0.0);
    SP2401->set_dds1(0.0);
    SP2401->set_tx_pwr_comp(-10.0);
    SP2401->set_tx_filter_sw(false);

    SP1401_R1F->cf()->m_tx_sb->get(2000000000,&dataSB);
    SP1401_R1F->cf()->m_tx_lol->get(2000000000,&dataLOL);
    SP2401->set_tx_phase_rotate_I(double(dataSB.th));
    SP2401->set_tx_amplitude_balance(dataSB.am_i,dataSB.am_q);
    SP2401->set_tx_dc_offset(quint16(dataLOL.dc_i),quint16(dataLOL.dc_q));

    QwtTempPwrData *data;
    QwtTempPwrData::Point point;

    while (1) {
        THREAD_TEST_CANCEL
        for (quint32 i = 0;i < FreqRange.freqs.size();i ++) {
            freq = FreqRange.freqs.at(i);

            SP1401->set_tx_freq(freq);
            Instr.sa_set_cf(freq);

            statesCnt = 0;
            for (iter = states.begin();iter != states.end();iter ++) {
                THREAD_TEST_CANCEL

                data = param->data.at(i)->at(statesCnt);
                SP1401_R1F->set_tx_att(iter->att[0],iter->att[1],iter->att[2],iter->att[3]);
                msleep(10);

                Instr.sa_set_ref_auto();
                Instr.sa_set_peak_search(sa::PEAK_HIGHEST);
                Instr.sa_get_marker_pwr(pwr);

                SP1401_R1F->get_temp(5,temp0);
                SP1401_R1F->get_temp(6,temp1);

                point.pwr = pwr;
                point.temp.temp[0] = linear_quantify(g_temp_star,g_temp_step,float(temp0));
                point.temp.temp[1] = linear_quantify(g_temp_star,g_temp_step,float(temp1));
                point.avg_temp = (temp0 + temp1) / 2.0;

                data->add(point.temp.temp[1],point);

                emit update(QModelIndex(),QModelIndex(),test_item_t(i),statesCnt);
                statesCnt ++;
            }
        }
    }
}

void QTestR1CTempPwrThread::Rx()
{
    quint64 freq = 0;
    const double dds1 = 20e6;
    double pwr = 0.0;
    double temp = 0.0;
    int statesCnt = 0;

    vector<common_atts_t> states;
    vector<common_atts_t>::iterator iter;

    FreqString.star = param->FreqStar.toStdString();
    FreqString.stop = param->FreqStop.toStdString();
    FreqString.step = param->FreqStep.toStdString();
    parse_range_freq_string(FreqString,FreqRange);

    SP1401->rx_att_states(states);
    SP1401->set_pwr_meas_src(sp1401::PWR_MEAS_FREE_RUN,false);
    SP1401->set_pwr_meas_timeout(6553600);
    SP1401->set_pwr_meas_samples(102400);
    SP2401->set_ddc(-92.64e6);
    SP3301->set_rx_level(RFIdx,-5.0);

    QwtTempPwrData *data;
    QwtTempPwrData::Point point;

    Instr.init();
    Instr.has_sg();
    Instr.sg_reset();
    Instr.sg_set_pl(-40.0);
    Instr.sg_set_en_mod(false);
    Instr.sg_set_en_output(true);

    while (1) {
        THREAD_TEST_CANCEL
        for (quint32 i = 0;i < FreqRange.freqs.size();i ++) {
            freq = FreqRange.freqs.at(i);

            SP1401->set_rx_freq(freq);
            Instr.sg_set_cw(freq);

            statesCnt = 0;
            for (iter = states.begin();iter != states.end();iter ++) {
                THREAD_TEST_CANCEL

                data = param->data.at(i)->at(statesCnt);
                SP1401_R1F->set_rx_lna_att_sw(iter->rx_sp1401_r1f.lna_att);
                SP1401_R1F->set_rx_att_019_sw(iter->rx_sp1401_r1f.att_019);
                SP1401_R1F->set_rx_att(iter->att[0],iter->att[1],iter->att[2]);
                msleep(10);

                pwr = getRxAvgPwr();

                SP1401_R1F->get_temp(0,temp);

                point.pwr = pwr;
                point.temp.temp[0] = linear_quantify(g_temp_star,g_temp_step,float(temp));
                point.avg_temp = temp;

                data->add(point.temp.temp[0],point);

                emit update(QModelIndex(),QModelIndex(),test_item_t(i),statesCnt);
                statesCnt ++;
            }
        }
    }
}
