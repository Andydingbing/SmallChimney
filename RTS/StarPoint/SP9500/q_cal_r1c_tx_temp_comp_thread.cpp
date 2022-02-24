#include "q_cal_r1c_tx_temp_comp_thread.h"
#include "q_cal_dlg.h"
#include "q_cal_temp_comp_widget.hpp"
#include "q_model_temp_pwr.hpp"
#include "algo_fit.hpp"

using namespace ns_sp1401;

void QCalR1CTxTempCompThread::cal()
{
    CAL_THREAD_START("TX Temperature Compansate",totalPts);

    quint64 freq = 0;
    double pwr = 0.0;
    double temp = 0.0;
    double temp0 = 0.0;
    double temp1 = 0.0;

    Point *point = nullptr;
    QList<QVector<Point>> allPwrs;
    QList<QVector<Point>>::iterator iterAllPwrs;
    QVector<tx_tc_table::data_f_t> data;
    QVector<bool> ready;
    quint32 freqsLeft = freqRangeCal.freqs.size();

    std::vector<double> coef;

    QCalR1CTxTempCompDlg *parent = dynamic_cast<QCalR1CTxTempCompDlg *>(calParam.parent);
    QCalTxTempCompWidget *child = nullptr;

    QVector<QwtTXTempPropertyData *> dataTP;
    QVector<QwtTempCompData *> dataTC;
    for (quint32 i = 0;i < freqRangeCal.freqs.size();i ++) {
        child = dynamic_cast<QCalTxTempCompWidget *>(parent->UI()->tabWidget->widget(i));
        dataTP.append(child->dataProperty());
        dataTC.append(child->dataComp());
        data.append(tx_tc_table::data_f_t());
        allPwrs.append(QVector<Point>());
        ready.append(false);
    }

    Instr.init();
    Instr.has_pm();
    SP3301->set_io_mode(RFIdx,OUTPUT);
    SP2401->set_duc_dds(0.0);
    SP3301->set_tx_en_tc(RFIdx,false);
    SP3301->set_tx_freq(RFIdx,2000000000);
    SP3301->set_tx_pwr(RFIdx,-15.0);
    SP3301->set_tx_src(RFIdx,sp2401_r1a::SINGLE_TONE);
    SP3301->set_src_freq(RFIdx,0);
    SP3301->set_rx_freq(RFIdx,6000000000);
    SP3301->set_rx_level(RFIdx,10.0);

    while (1) {
        THREAD_TEST_CANCEL

        if (freqsLeft == 0) {
            break;
        }

        iterAllPwrs = allPwrs.begin();

        for (quint32 i = 0;i < freqRangeCal.freqs.size();i ++) {
            freq = freqRangeCal.freqs.at(i);

            if (ready.at(i) == true) {
                Log.stdprintf("Freq@%lld is Ready!\n",freq);
                ++ iterAllPwrs;
                continue;
            }

            Log.stdprintf("Still Running @%lld\n",freq);

            THREAD_TEST_CANCEL
            THREAD_TEST_PAUSE_S

            if ((*iterAllPwrs).empty()) {
                (*iterAllPwrs).fill(Point(),ARRAY_SIZE(tx_tp_table::data_f_t::pwr));
            }

            SP3301->set_tx_freq(RFIdx,freq);
            msleep(10);

            Instr.pm_get_pwr(freq,pwr);

            getTemp0(temp0);
            getTemp1(temp1);

            temp = (temp0 + temp1) / 2.0;
            temp = linear_quantify(sp1401::temp_min,sp1401::temp_granularity,temp);

            point = &((*iterAllPwrs)[sp1401::temp_idx(temp)]);
            if (point->points == 0) {
                point->avgPwr = pwr;
                point->points ++;
            } else {
                if (abs(pwr - point->avgPwr) > 0.1) {
//                    THREAD_ERROR_BOX("Wrong!!!!");
                } else {
                    point->avgPwr = (point->avgPwr * point->points + pwr) / (point->points + 1);
                    point->points ++;
                    dataTP.at(i)->set(temp,point->avgPwr);
                    dataTP.at(i)->data()->temp.temp[0] = temp0;
                    dataTP.at(i)->data()->temp.temp[1] = temp1;
                }
            }

            if (dataTP.at(i)->data()->is_ready_to_fit(g_temp_star,g_temp_stop)) {
                dataTP.at(i)->data()->freq = freq;
                SP1401->cf()->add(cal_file::TX_TP,dataTP.at(i)->data());
                SP1401->cf()->w(cal_file::TX_TP);
                ready[i] = true;
                -- freqsLeft;
            }

            emit update(QModelIndex(),QModelIndex(),cal_file::TX_TP,i);
            ++ iterAllPwrs;

            THREAD_TEST_PAUSE_E
        }

        if (freqsLeft != 0) {
            continue;
        }
    }

    for (quint32 i = 0;i < freqRangeCal.freqs.size();i ++) {
        freq = freqRangeCal.freqs.at(i);

        data[i].order = dataTP.at(i)->data()->fit(g_temp_star,g_temp_stop,5,coef);
        data[i].freq = freq;
        data[i].temp_star = g_temp_star;
        data[i].temp_stop = g_temp_stop;

        for (quint32 i_coef = 0;i_coef < coef.size();i_coef ++) {
            data[i].coef[i_coef] = coef[i_coef];
        }
        SP1401->cf()->add(cal_file::TX_TC,&data[i]);

        dataTC.at(i)->setCoef(data[i]);
        dataTC.at(i)->makeCurve();

        emit update(QModelIndex(),QModelIndex(),cal_file::TX_TC,i);
    }
    SP1401->cf()->w(cal_file::TX_TC);
}

void QCalR1CTxTempCompThread::initTXChain()
{

}

void QCalR1CTxTempCompThread::getTemp0(double &temp) const
{
    temp = 0.0;
    double temp0 = -100.0;
    for (quint32 i = 0;i < avgTime;i ++) {
        while (temp0 < 0.0 || temp0 > 100.0) {
            SP1401->get_temp(5,temp0);
        }
        temp = (temp * i + temp0) / (i + 1);
    }
}

void QCalR1CTxTempCompThread::getTemp1(double &temp) const
{
    temp = 0.0;
    double temp1 = -100.0;
    for (quint32 i = 0;i < avgTime;i ++) {
        while (temp1 < 0.0 || temp1 > 100.0) {
            SP1401->get_temp(6,temp1);
        }
        temp = (temp * i + temp1) / (i + 1);
    }
}

void QExpR1CTxTempCompThread::run()
{
    initProgress("Exporting TX Temperature Compansate",100);

    quint64 freq = 0;

    QCalR1CTxTempCompDlg *parent = dynamic_cast<QCalR1CTxTempCompDlg *>(calParam.parent);
    QCalTxTempCompWidget *child = nullptr;

    QwtTXTempPropertyData *dataTP = nullptr;
    QwtTempCompData *dataTC = nullptr;

    SP1401->cf()->map2buf(cal_file::TX_TP);
    for (quint32 i = 0;i < freqRangeCal.freqs.size();i ++) {
        freq = freqRangeCal.freqs.at(i);
        child = dynamic_cast<QCalTxTempCompWidget *>(parent->UI()->tabWidget->widget(i));
        dataTP = child->dataProperty();
        SP1401->cf()->tx_tp()->get(freq,dataTP->data());
        emit update(QModelIndex(),QModelIndex(),cal_file::TX_TP,i);
    }

    SP1401->cf()->map2buf(cal_file::TX_TC);
    for (quint32 i = 0;i < freqRangeCal.freqs.size();i ++) {
        freq = freqRangeCal.freqs.at(i);
        child = dynamic_cast<QCalTxTempCompWidget *>(parent->UI()->tabWidget->widget(i));
        dataTC = child->dataComp();
        SP1401->cf()->tx_tc()->get(freq,dataTC->coef());
        dataTC->makeCurve();
        emit update(QModelIndex(),QModelIndex(),cal_file::TX_TC,i);
    }

    SET_PROG_POS(100);
    THREAD_ENDED
}


void QCalR1CRXTempCompThread::cal()
{
    CAL_THREAD_START("RX Temperature Compansate",totalPts);

    quint64 freq = 0;
    double temp = 0.0;
    double pwr = 0.0;

    Point *point = nullptr;
    QList<QVector<Point>> allPwrs;
    QList<QVector<Point>>::iterator iterAllPwrs;
    QVector<rx_tc_table::data_f_t> data;
    QVector<bool> ready;
    quint32 freqsLeft = freqRangeCal.freqs.size();

    std::vector<double> coef;

    QCalR1CRXTempCompDlg *parent = dynamic_cast<QCalR1CRXTempCompDlg *>(calParam.parent);
    QCalRXTempCompWidget *child = nullptr;

    QVector<QwtRXTempPropertyData *> dataTP;
    QVector<QwtTempCompData *> dataTC;
    for (quint32 i = 0;i < freqRangeCal.freqs.size();i ++) {
        child = dynamic_cast<QCalRXTempCompWidget *>(parent->UI()->tabWidget->widget(i));
        dataTP.append(child->dataProperty());
        dataTC.append(child->dataComp());
        data.append(rx_tc_table::data_f_t());
        allPwrs.append(QVector<Point>());
        ready.append(false);
    }

    Instr.init();
    Instr.has_sg();
    Instr.sg_set_pl(-90.0);
    Instr.sg_set_en_mod(false);
    Instr.sg_set_en_output(true);

    SP3301->set_io_mode(RFIdx,OUTPUT);
    SP2401->set_ddc(-92.64e6);
    SP3301->set_rx_en_tc(RFIdx,false);
    SP3301->set_tx_freq(RFIdx,6000000000);
    SP3301->set_tx_pwr(RFIdx,-120.0);
    SP3301->set_rx_freq(RFIdx,2000000000);
    SP3301->set_rx_level(RFIdx,10.0);
    SP1401->set_pwr_meas_src(sp1401::PWR_MEAS_FREE_RUN,false);
    SP1401->set_pwr_meas_timeout(6553600);
    SP1401->set_pwr_meas_samples(102400);

    while (1) {
        THREAD_TEST_CANCEL

        if (freqsLeft == 0) {
            break;
        }

        iterAllPwrs = allPwrs.begin();

        for (quint32 i = 0;i < freqRangeCal.freqs.size();i ++) {
            freq = freqRangeCal.freqs.at(i);
            freq = freq < rx_freq_star ? rx_freq_star : freq;

            if (ready.at(i) == true) {
                ++ iterAllPwrs;
                continue;
            }

            THREAD_TEST_CANCEL
            THREAD_TEST_PAUSE_S

            if ((*iterAllPwrs).empty()) {
                (*iterAllPwrs).fill(Point(),ARRAY_SIZE(rx_tp_table::data_f_t::pwr));
            }

            SP3301->set_rx_freq(RFIdx,freq);
            msleep(10);

            Instr.sg_set_cw(freq);

            SP3301->set_rx_level(RFIdx,-10.0);
            Instr.sg_set_pl(-25.0);
            msleep(100);

            pwr = getRxAvgPwr();
            getTemp(temp);

            point = &((*iterAllPwrs)[sp1401::temp_idx(temp)]);
            if (point->points == 0) {
                point->avgPwr = pwr;
                point->points ++;
            } else {
                if (abs(pwr - point->avgPwr) > 0) {

                } else {
                    point->avgPwr = (point->avgPwr * point->points + pwr) / (point->points + 1);
                    point->points ++;
                }
            }

            dataTP.at(i)->data()->set(temp,point->avgPwr);
            dataTP.at(i)->data()->temp.temp[0] = temp;

            if (dataTP.at(i)->data()->is_ready_to_fit(45.0,60.0)) {
                dataTP.at(i)->data()->freq = freq;
                SP1401->cf()->add(cal_file::RX_TP,dataTP.at(i)->data());
                SP1401->cf()->w(cal_file::RX_TP);
                ready[i] = true;
                -- freqsLeft;
            }

            emit update(QModelIndex(),QModelIndex(),cal_file::RX_TP,i);
            ++ iterAllPwrs;

            THREAD_TEST_PAUSE_E
        }

        if (freqsLeft != 0) {
            continue;
        }
    }

    for (quint32 i = 0;i < freqRangeCal.freqs.size();i ++) {
        freq = freqRangeCal.freqs.at(i);
        freq = freq < rx_freq_star ? rx_freq_star : freq;

        data[i].order = dataTP.at(i)->data()->fit(45.0,60.0,5,coef);
        data[i].freq = freq;
        data[i].temp_star = 45.0;
        data[i].temp_stop = 60.0;

        for (quint32 i_coef = 0;i_coef < coef.size();i_coef ++) {
            data[i].coef[i_coef] = coef[i_coef];
        }
        SP1401->cf()->add(cal_file::RX_TC,&data[i]);

        dataTC.at(i)->setCoef(data[i]);
        dataTC.at(i)->makeCurve();

        emit update(QModelIndex(),QModelIndex(),cal_file::RX_TC,i);
    }
    SP1401->cf()->w(cal_file::RX_TC);
}

void QCalR1CRXTempCompThread::getTemp(double &temp) const
{
    temp = 0.0;
    double temp0 = -100.0;
    for (quint32 i = 0;i < avgTime;i ++) {
        while (temp0 < 0.0 || temp0 > 100.0) {
            SP1401->get_temp(0,temp0);
        }
        temp = (temp * i + temp0) / (i + 1);
    }
    temp = linear_quantify(sp1401::temp_min,sp1401::temp_granularity,temp);
}

void QExpR1CRXTempCompThread::run()
{
    initProgress("Exporting RX Temperature Compansate",100);

    quint64 freq = 0;

    QCalR1CRXTempCompDlg *parent = dynamic_cast<QCalR1CRXTempCompDlg *>(calParam.parent);
    QCalRXTempCompWidget *child = nullptr;

    QwtRXTempPropertyData *dataTP = nullptr;
    QwtTempCompData *dataTC = nullptr;

    SP1401->cf()->map2buf(cal_file::RX_TP);
    for (quint32 i = 0;i < freqRangeCal.freqs.size();i ++) {
        freq = freqRangeCal.freqs.at(i);
        child = dynamic_cast<QCalRXTempCompWidget *>(parent->UI()->tabWidget->widget(i));
        dataTP = child->dataProperty();
        SP1401->cf()->rx_tp()->get(freq,dataTP->data());
        emit update(QModelIndex(),QModelIndex(),cal_file::RX_TP,i);
    }

    SP1401->cf()->map2buf(cal_file::RX_TC);
    for (quint32 i = 0;i < freqRangeCal.freqs.size();i ++) {
        freq = freqRangeCal.freqs.at(i);
        child = dynamic_cast<QCalRXTempCompWidget *>(parent->UI()->tabWidget->widget(i));
        dataTC = child->dataComp();
        SP1401->cf()->rx_tc()->get(freq,dataTC->coef());
        dataTC->makeCurve();
        emit update(QModelIndex(),QModelIndex(),cal_file::RX_TC,i);
    }

    SET_PROG_POS(100);
    THREAD_ENDED
}
