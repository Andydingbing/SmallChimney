#include "q_cal_r1c_tx_filter_offset_thread.h"
#include "q_model_filter_offset.h"

using namespace ns_sp1401;
using namespace ns_sp1401::r1c;
using namespace ns_sp2401;

void QCalR1CTXFilterOffsetThread::run()
{
#define POST_CLEAR DYNAMIC_SP1401_R1CE_CAL->cf()->set_bw(_160M);

    RD_CAL_TRY
    CAL_THREAD_START("TX Filter Offset",freqRangeCal.freqs.size());
    THREAD_CHECK_BOX("TX<===>Spectrum");

    QTXFilterOffsetModel *model = dynamic_cast<QTXFilterOffsetModel *>(calParam.model_0);

    Instr.init();
    Instr.has_sa();
    Instr.sa_reset();
    Instr.sa_set_span(20000.0);
    Instr.sa_set_rbw(1000.0,true);
    Instr.sa_set_vbw(1000.0,true);
    Instr.sa_set_ref(tx_base_pwr_op + 20.0);

    quint64 freq = 0;
    double pwr[2] = {0.0,0.0};
    int secBfr = -1;
    int secCur = 0;

    tx_filter_offset_table::data_f_t data;
    tx_pwr_op_table_r1c::data_m_t  dataPwr;
    tx_filter_160m_table::data_m_t dataFilter_160;
    tx_filter_80m_table::data_m_t  dataFilter_80;
    double coefReal[dl_filter_tap_2i] = {0.0};
    double coefImag[dl_filter_tap_2i] = {0.0};

    SP1401->cf()->set_bw(_80M);
    SP1401->cf()->map2buf(cal_file::TX_FILTER_160);
    SP1401->cf()->m_tx_filter_160m->map2mem();

    init();

    for (quint32 i = 0;i < freqRangeCal.freqs.size();i ++) {
        THREAD_TEST_CANCEL_S(POST_CLEAR);

        freq = freqRangeCal.freqs.at(i);
        SP1401->set_tx_freq(freq);
        Instr.sa_set_cf(freq + dds1);

        SP1401->cf()->m_tx_pwr_op->get(RFVer,freq,tx_base_pwr_op,&dataPwr);
        SP1401->cf()->m_tx_filter_160m->get(freq,&dataFilter_160);
        SP1401->cf()->m_tx_filter_80m->get(freq,&dataFilter_80);
        dataFilter_160._2double(coefReal,coefImag);

        SP1401->set_tx_att0(dataPwr.att0 / 2.0);
        SP1401->set_tx_att1(dataPwr.att1 / 2.0);
        SP1401->set_tx_att2(dataPwr.att2 / 2.0);
        SP1401->set_tx_att3(dataPwr.att3 / 2.0);
        SP2401->set_tx_pwr_comp(double(dataPwr.d_gain));
        SP2401->set_tx_filter(coefReal,coefImag);
        msleep(50);

        Instr.sa_sweep_once();
        Instr.sa_set_peak_search(sa::PEAK_HIGHEST);
        Instr.sa_get_marker_pwr(pwr[0]);

        dataFilter_80._2double(coefReal,coefImag);
        SP2401->set_tx_filter(coefReal,coefImag);
        msleep(50);

        Instr.sa_sweep_once();
        Instr.sa_get_marker_pwr(pwr[1]);

        data.freq = freq;
        data.offset = pwr[0] - pwr[1];
        SP1401->get_temp(4,data.temp[0]);
        SP1401->get_temp(5,data.temp[1]);
        SP1401->get_temp(6,data.temp[2]);
        SP1401->get_temp(7,data.temp[3]);
        data.time = getCurTime();

        secCur = freq_section(freq,freqRangeCal);
        if (secCur != secBfr) {
            model->iterTable()->at(secCur)->locate2CalTable(model->calTable()->begin() + i);
            secBfr = secCur;
        }
        model->calTable()->replace(i,data);
        model->iterTable()->at(secCur)->addOneData();
        SP1401->cf()->add(cal_file::TX_FILTER_OFFSET_80,&data);
        emit update(model->index(i,0),
                    model->index(i,7),
                    cal_file::TX_FILTER_OFFSET_80,
                    secCur);
        SET_PROG_POS(i + 1);
    }
    SP1401->cf()->w(cal_file::TX_FILTER_OFFSET_80);
    SP1401->cf()->m_tx_filter_offset_op_80m->save_as("c:\\tx_filter_off_op_80.txt");

    THREAD_ENDED_S(POST_CLEAR);
    RD_CAL_CATCH
}

void QCalR1CTXFilterOffsetThread::init()
{
    tx_lol_table_r1cd::data_m_t	dataLOL;
    tx_sb_table_r1cd::data_m_t	dataSB;

    SP1401->set_io_mode(OUTPUT);
    SP1401->set_rx_att_019_sw(r1c::RX_ATT_19);
    SP1401->set_rx_lna_att_sw(r1c::RX_ATT);
    SP1401->set_rx_att(30.0,30.0,30.0);
    SP2401->set_tx_filter_sw(true);
    SP2401->set_dds_src(sp2401_r1a::SINGLE_TONE);
    SP2401->set_dds1(dds1);
    SP2401->set_duc_dds(0.0);

    SP1401->cf()->m_tx_sb->get(2000000000,&dataSB);
    SP1401->cf()->m_tx_lol->get(2000000000,&dataLOL);
    SP2401->set_tx_phase_rotate_I(double(dataSB.th));
    SP2401->set_tx_amplitude_balance(dataSB.am_i,dataSB.am_q);
    SP2401->set_tx_dc_offset(quint16(dataLOL.dc_i),quint16(dataLOL.dc_q));
}


void QExpR1CTXFilterOffsetThread::run()
{
    initProgress("Exporting TX Filter Offset",100);

    QTXFilterOffsetModel *model = dynamic_cast<QTXFilterOffsetModel *>(calParam.model_0);

    quint64 freq = 0;
    int secBfr = -1;
    int secCur = 0;

    tx_filter_offset_table::data_f_t data;

    SP1401->cf()->set_bw(_80M);
    SP1401->cf()->map2buf(cal_file::TX_FILTER_OFFSET_OP_80);

    for (quint32 i = 0;i < freqRangeCal.freqs.size();i ++) {
        freq = freqRangeCal.freqs.at(i);
        SP1401->cf()->m_tx_filter_offset_op_80m->get(freq,&data);
        secCur = freq_section(freq,freqRangeCal);
        if (secCur != secBfr) {
            model->iterTable()->at(secCur)->locate2CalTable(model->calTable()->begin() + i);
            secBfr = secCur;
        }
        model->calTable()->replace(i,data);
        model->iterTable()->at(secCur)->addOneData();
    }

    emit update(model->index(0,0),
                model->index(freqRangeCal.freqs.size(),7),
                cal_file::TX_FILTER_OFFSET_OP_80,
                secCur);

    SP1401->cf()->set_bw(_160M);

    SET_PROG_POS(100);
    THREAD_ENDED
}
