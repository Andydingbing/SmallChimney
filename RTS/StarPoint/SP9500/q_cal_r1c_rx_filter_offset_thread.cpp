#include "q_cal_r1c_rx_filter_offset_thread.h"
#include "q_model_rx_filter_offset.h"
#include "algo_math.hpp"

using namespace rd::ns_sp9500;
using namespace rd::ns_sp1401;
using namespace rd::ns_sp2401;

void QCalR1CRXFilterOffsetThread::run()
{
#define POST_CLEAR DYNAMIC_SP1401_R1CE_CAL->cf()->set_bw(_160M);

    RD_CAL_TRY
    CAL_THREAD_START("RX Filter Offset",freqRangeCal.freqs.size());
    THREAD_CHECK_BOX("RX<===>Z28<===>Signal Generator");

    qint64 ad[2] = {0,0};
    double pwrSG[3] = {10.0 + PM_IL,10.0 + PM_IL,-20.0 + PM_IL};
    for (quint32 i = 0;i < ARRAY_SIZE(pwrSG);i ++) {	// For preventing saturation
        pwrSG[i] -= 10.0;
    }

    quint64 freq = 0;
    qint32 secBfr = -1;
    qint32 secCur = 0;

    rx_filter_160m_table::data_m_t dataFilter_160;
    rx_filter_80m_table::data_m_t  dataFilter_80;
    double coefReal[ul_filter_tap] = {0.0};
    double coefImag[ul_filter_tap] = {0.0};

    Instr.init();
    Instr.has_pm();
    Instr.has_sg();
    Instr.pm_reset();
    Instr.sg_reset();
    Instr.sg_set_en_mod(false);
    Instr.sg_set_pl(-60.0);
    Instr.sg_set_en_output(true);

    SP1401->cf()->set_bw(_80M);
    SP1401->cf()->map2buf(cal_file::RX_FILTER_160);
    SP1401->cf()->m_rx_filter_160m->map2mem();

    SP1401->set_tx_att(30.0,30.0,30.0,30.0);
    SP1401->set_rx_if_filter_sw(sp1401::_160MHz);
    SP2401->set_tx_pwr_comp(-5.0);
    SP2401->set_rx_pwr_comp(0);
    SP2401->set_rx_filter_sw(sp2401_r1a::_2I3D);

    rx_filter_offset_table::data_f_t data;
    const qint32 offsetCnt = ARRAY_SIZE(data.offset);

    QRXFilterOffsetModel *model = (QRXFilterOffsetModel *)(calParam.model_0);
    SP1401->set_io_mode(OUTPUT);

    rx_ref_op_table_r1cd::data_m_t dataRef;

    for (quint32 i = 0;i < freqRangeCal.freqs.size();i ++) {
        THREAD_TEST_CANCEL_S(POST_CLEAR);

        freq = freqRangeCal.freqs.at(i);

        Instr.sg_set_cw(double(freq));
        SP1401->set_rx_freq(freq);
        SP1401->cf()->m_rx_ref_op->get_base(freq,&dataRef);
        SP1401->cf()->m_rx_filter_160m->get(freq,&dataFilter_160);
        SP1401->cf()->m_rx_filter_80m->get(freq,&dataFilter_80);

        for (qint32 j = 0;j < offsetCnt;j ++) {
            THREAD_TEST_CANCEL_S(POST_CLEAR);
            Instr.sg_set_pl(pwrSG[j]);
            dataFilter_160._2double(coefReal,coefImag);
            SP2401->set_rx_filter_sw(sp2401_r1a::_2I3D);
            SP2401->set_rx_filter(coefReal,coefImag);
            SP1401->set_rx_lna_att_sw(r1c::rx_lna_att_t(dataRef.state[j].lna_att));
            SP1401->set_rx_att_019_sw(r1c::rx_att_019_t(dataRef.state[j].att_019));
            SP1401->set_rx_att(double(dataRef.state[j].att1),
                               double(dataRef.state[j].att2),
                               double(dataRef.state[j].att3));
            SP2401->set_rx_pwr_comp(dataRef.state[j].ad_offset);
            msleep(50);

            getADS5474(DYNAMIC_SP1401_R1CE_CAL,ad[0]);

            dataFilter_80._2double(coefReal,coefImag);
            SP2401->set_rx_filter_sw(sp2401_r1a::_3D);
            SP2401->set_rx_filter(coefReal,coefImag);
            msleep(50);

            getADS5474(DYNAMIC_SP1401_R1CE_CAL,ad[1]);
            data.offset[j] = g_0dBFS - dBc_to_ad(g_0dBFS,ad_to_dBc(ad[1],ad[0]));
        }

        data.freq = freq;
        SP1401->get_temp(0,data.temp[0]);
        SP1401->get_temp(1,data.temp[1]);
        SP1401->get_temp(2,data.temp[2]);
        SP1401->get_temp(3,data.temp[3]);
        data.time = getCurTime();

        secCur = freq_section(freq,freqRangeCal);

        for (qint32 j = 0;j < offsetCnt;j ++) {
            if(secCur != secBfr)
                model->iterTable(j)->at(secCur)->locate2CalTable(model->calTable()->begin() + i);
            model->iterTable(j)->at(secCur)->addOneData();
        }
        secBfr = secCur;

        model->calTable()->replace(i,data);
        SP1401->cf()->add(cal_file::RX_FILTER_OFFSET_OP_80,&data);
        emit update(model->index(i,0),
                    model->index((i + 1),7),
                    cal_file::RX_FILTER_OFFSET_OP_80,
                    secCur);
        SET_PROG_POS(i + 1);
    }
    SP1401->cf()->w(cal_file::RX_FILTER_OFFSET_OP_80);
    SP1401->cf()->m_rx_filter_offset_op_80m->save_as("c:\\rx_filter_off_op_80.txt");

    THREAD_ENDED_S(POST_CLEAR);
    RD_CAL_CATCH
}


void QExpR1CRXFilterOffsetThread::run()
{
    initProgress("Exporting Rx Filter Offset",100);

    QRXFilterOffsetModel *model = (QRXFilterOffsetModel *)(calParam.model_0);

    quint64 freq = 0;
    int secBfr = -1;
    int secCur = 0;

    rx_filter_offset_table::data_f_t data;
    const quint32 offsetCnt = ARRAY_SIZE(data.offset);

    SP1401->cf()->set_bw(_80M);
    SP1401->cf()->map2buf(cal_file::RX_FILTER_OFFSET_OP_80);

    for (quint32 i = 0;i < freqRangeCal.freqs.size();i ++) {
        freq = freqRangeCal.freqs.at(i);
        SP1401->cf()->m_rx_filter_offset_op_80m->get(freq,&data);
        secCur = freq_section(freq,freqRangeCal);
        for (quint32 j = 0;j < offsetCnt;j ++) {
            if(secCur != secBfr)
                model->iterTable(j)->at(secCur)->locate2CalTable(model->calTable()->begin() + i);
            model->iterTable(j)->at(secCur)->addOneData();
        }
        secBfr = secCur;

        model->calTable()->replace(i,data);
    }

    emit update(model->index(0,0),
                model->index(freqRangeCal.freqs.size() * offsetCnt,7),
                cal_file::RX_FILTER_OFFSET_OP_80,
                secCur);

    SP1401->cf()->set_bw(_160M);

    SET_PROG_POS(100);
    THREAD_ENDED
}
