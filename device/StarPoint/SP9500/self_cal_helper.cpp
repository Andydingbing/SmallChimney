#include "self_cal_helper.h"
#include "sleep_common.h"
#include "liblog.h"

using namespace ns_sp1401;

self_cal_helper::self_cal_helper(sp3301 *SP3301,uint32_t rf_idx)
{
    _rf_idx = rf_idx;
    _sp3301 = SP3301;
    _sp1401 = _sp3301->get_sp1401_r1f(_rf_idx);
    _sp2401 = _sp3301->get_sp2401(_rf_idx);

    _I = nullptr;
    _Q = nullptr;
    _I_brother = nullptr;
    _Q_brother = nullptr;

    const uint32_t samples = 245760;

    _sequence._new(samples);
    _sp3301->set_iq_cap_samples(_rf_idx,samples);
    _sp3301->get_iq_cap_buffer(_rf_idx,&_I,&_Q);
    _sp3301->get_iq_cap_buffer(brother_idx(_rf_idx),&_I_brother,&_Q_brother);
    _sp3301->set_iq_cap_buffer(_rf_idx,_sequence.i(),_sequence.q());
    _sp3301->set_iq_cap_buffer(brother_idx(_rf_idx),nullptr,nullptr);
}

self_cal_helper::~self_cal_helper()
{
    _sp3301->set_iq_cap_buffer(_rf_idx,_I,_Q);
    _sp3301->set_iq_cap_buffer(brother_idx(_rf_idx),_I_brother,_Q_brother);
}

int32_t self_cal_helper::meas_once(double *pwr)
{
    INT_CHECK(_sp3301->iq_cap(_rf_idx));
    INT_CHECK(_sp3301->iq_cap_iq2buf(_rf_idx));
    INT_CHECK(_sp3301->iq_cap_abort(_rf_idx));

    _sequence.dft();
    *pwr = _sequence.pwr(FREQ_M(-1),FREQ_M(1));
    return 0;
}


self_cal_tx_lol_helper::self_cal_tx_lol_helper(sp3301 *SP3301,uint32_t rf_idx) :
    self_cal_helper(SP3301,rf_idx)
{
    init();
}

int32_t self_cal_tx_lol_helper::init()
{
    dc_i_l = -10000;
    dc_i_m = 0;
    dc_i_r = +10000;
    dc_q_l = -10000;
    dc_q_m = 0;
    dc_q_r = +10000;
    return 0;
}

int32_t self_cal_tx_lol_helper::run()
{
    if (is_rf_ver_before(_sp1401->get_hw_ver(),R1B)) {
        return 0;
    }

    const uint64_t freq = FREQ_M(2400);

    bool result = true;
    int16_t step = 0;

    _sp1401->cf()->m_tx_lol->get(freq,&data);

    _sp3301->set_io_mode(_rf_idx,io_mode_t::OUTPUT);
    _sp3301->set_rx_freq(_rf_idx,freq);
    _sp3301->set_rx_level(_rf_idx,0.0);

    _sp3301->set_tx_freq(_rf_idx,freq);
    _sp3301->set_tx_pwr(_rf_idx,-20.0);
    _sp3301->set_io_mode(_rf_idx,io_mode_t::LOOP);

    _sp2401->set_da_sw(sp2401_r1a::FROM_TO_RF);
    _sp2401->set_ad_sw(sp2401_r1a::FROM_TO_RF);
    _sp2401->set_dds_src(sp2401_r1a::ALL_0);
    _sp2401->set_ddc(-92640000.0);

    _sp3301->set_tx_state(_rf_idx,true);

    _sp2401->set_tx_dc_offset(dc_i_m,dc_q_m);
    sleep_ms(50);

    init();

    step = 100;
    get_min_i(step,1);
    get_min_q(step,1);

    step = 10;
    get_min_i(step,1);
    get_min_q(step,1);

    step = 5;
    get_min_i(step,2);
    get_min_q(step,2);

    step = 1;
    for (int32_t retry = 0;retry < 3;retry ++) {
        get_min_i(step,5);
        get_min_q(step,5);
    }

    meas_once(&pwr_lol);


    // Log
    Log.en(log_t::RD_LOG_MESSAGE_F,true);
    Log.add_msg("RFU : %d RF : %d i : %6d \t q : %6d \t lo_leakage : %8.3f",
                _sp3301->get_rfu_idx(),
                _rf_idx,
                dc_i_m,
                dc_q_m,
                pwr_lol);
    Log.en(log_t::RD_LOG_MESSAGE_F,false);

    if (abs(dc_i_m) >= 10000 || abs(dc_q_m) >= 10000) {
        result = false;
    }

    if (pwr_lol > -70.0) {
        result = false;
    }

    if(result == false) {
        _sp2401->set_tx_dc_offset(data.dc_i,data.dc_q);
        return -1;
    }

    data.freq = freq;
    data.dc_i = dc_i_m;
    data.dc_q = dc_q_m;
    data.pwr  = pwr_lol;
    data.use_sa = false;
    _sp1401->get_temp(4,data.temp[0]);
    _sp1401->get_temp(5,data.temp[1]);
    _sp1401->get_temp(6,data.temp[2]);
    _sp1401->get_temp(7,data.temp[3]);

    time_t cur_time;
    time(&cur_time);
    data.time = *localtime(&cur_time);

    _sp1401->cf()->add(cal_file::TX_LOL,&data);
    _sp1401->cf()->w(cal_file::TX_LOL);
    return 0;
}

int32_t self_cal_tx_lol_helper::get_min_i(uint16_t step,uint16_t coef)
{
    ad_min = LONG_MAX;
    int64_t ad[512] = {0};
    int16_t idx = 0;
    int16_t idx_min = 0;

    for (dc_i_m = dc_i_l;dc_i_m <= dc_i_r;dc_i_m += step) {
        _sp2401->set_tx_dc_offset(dc_i_m,dc_q_m);
        sleep_ms(10);
        _sp1401->get_ads5474(ad[idx]);

        if (ad[idx] < ad_min) {
            ad_min = ad[idx];
            idx_min = idx;
        }
        idx ++;
    }
    dc_i_m = dc_i_l + idx_min * step;
    dc_i_l = dc_i_m - step * coef;
    dc_i_r = dc_i_m + step * coef;
    return 0;
}

int32_t self_cal_tx_lol_helper::get_min_q(uint16_t step,uint16_t coef)
{
    ad_min = LONG_MAX;
    int64_t ad[512] = {0};
    int16_t idx = 0;
    int16_t idx_min = 0;

    for (dc_q_m = dc_q_l;dc_q_m <= dc_q_r;dc_q_m += step) {
        _sp2401->set_tx_dc_offset(dc_i_m,dc_q_m);
        sleep_ms(10);
        _sp1401->get_ads5474(ad[idx]);

        if (ad[idx] < ad_min) {
            ad_min = ad[idx];
            idx_min = idx;
        }
        idx ++;
    }
    dc_q_m = dc_q_l + idx_min * step;
    dc_q_l = dc_q_m - step * coef;
    dc_q_r = dc_q_m + step * coef;
    return 0;
}


self_cal_tx_sb_helper::self_cal_tx_sb_helper(sp3301 *SP3301,uint32_t rf_idx) :
    self_cal_helper(SP3301,rf_idx)
{
    init();
}

int32_t self_cal_tx_sb_helper::init()
{
    th_l = -10.0;
    th_m = 0.0;
    th_r = +10.0;

    am_i_l = 8192 - 500;
    am_i_m = 8192;
    am_i_r = 8192 + 500;

    am_q_l = 8192 - 500;
    am_q_m = 8192;
    am_q_r = 8192 + 500;
    return 0;
}

int32_t self_cal_tx_sb_helper::run(tx_sb_table_r1cd::data_f_t *data)
{
    if (is_rf_ver_before(_sp1401->get_hw_ver(),R1B)) {
        return 0;
    }

    const uint64_t freq = FREQ_M(2400);

    double stepTh = 0.0;
    uint16_t stepAm = 0;

    _sp3301->get_sp1401_r1f(_rf_idx)->cf()->m_tx_sb->get(freq,data);

    _sp3301->set_io_mode(_rf_idx,io_mode_t::OUTPUT);

    _sp3301->set_tx_freq(_rf_idx,freq);
    _sp3301->set_tx_pwr(_rf_idx,-20.0);
    _sp3301->set_tx_state(_rf_idx,true);

    _sp3301->set_rx_freq(_rf_idx,freq - FREQ_M(20));
    _sp3301->set_rx_level(_rf_idx,0.0);

    _sp3301->set_io_mode(_rf_idx,io_mode_t::LOOP);

    _sp2401->set_ddc(-92640000.0);
    _sp2401->set_da_sw(sp2401_r1a::FROM_TO_RF);
    _sp2401->set_ad_sw(sp2401_r1a::FROM_TO_RF);
    _sp2401->set_dds_src(sp2401_r1a::SINGLE_TONE);
    _sp2401->set_dds1(20000000.0);
    _sp2401->set_tx_phase_rotate_I(0.0);
    _sp2401->set_tx_amplitude_balance(8192,8192);

    init();

    stepTh = 2.0;
    get_min_th(stepTh,0.5);

    stepTh = 0.5;
    get_min_th(stepTh,0.4);

    stepTh = 0.1;
    get_min_th(stepTh,1.0);

    stepAm = 20;
    get_min_am_i(stepAm,1);
    get_min_am_q(stepAm,1);

    stepAm = 5;
    get_min_am_i(stepAm,1);
    get_min_am_q(stepAm,1);

    get_min_th(stepTh,1.0);

    stepAm = 1;
    for (int32_t retry = 0;retry < 3;retry ++) {
        get_min_am_i(stepAm,2);
        get_min_am_q(stepAm,2);
    }

    // Log
    Log.en(log_t::RD_LOG_MESSAGE_F,true);
    Log.add_msg("RFU : %d RF : %d angle : %8.3f i : %6d \t q : %6d \t sideband : %8.3f",
                _sp3301->get_rfu_idx(),
                _rf_idx,
                th_m,
                am_i_m,
                am_q_m,
                pwr_sb);
    Log.en(log_t::RD_LOG_MESSAGE_F,false);

    if (pwr_sb > -70.0) {
        _sp2401->set_tx_phase_rotate_I(data->th);
        _sp2401->set_tx_amplitude_balance(data->am_i,data->am_q);
        return -1;
    }

    data->th   = th_m;
    data->am_i = am_i_m;
    data->am_q = am_q_m;
    data->pwr  = pwr_sb;
    data->use_sa = false;
    _sp1401->get_temp(4,data->temp[0]);
    _sp1401->get_temp(5,data->temp[1]);
    _sp1401->get_temp(6,data->temp[2]);
    _sp1401->get_temp(7,data->temp[3]);

    time_t cur_time;
    time(&cur_time);
    data->time = *localtime(&cur_time);

    _sp1401->cf()->add(cal_file::TX_SB,data);
    _sp1401->cf()->w(cal_file::TX_SB);
    return 0;
}

int32_t self_cal_tx_sb_helper::get_min_th(double step,double coef)
{
    double pwr[512] = {0.0};
    pwr_sb = double(LONG_MAX);
    int16_t idx = 0;
    int16_t idx_min = 0;

    for (th_m = th_l;th_m <= th_r;th_m += step) {
        INT_CHECK(_sp2401->set_tx_phase_rotate_I(th_m));
        meas_once(&pwr[idx]);

        if (pwr[idx] < pwr_sb) {
            pwr_sb = pwr[idx];
            idx_min = idx;
        }
        idx ++;
    }
    th_m = th_l + idx_min * step;
    th_l = th_m - step * coef;
    th_r = th_m + step * coef;
    return 0;
}

int32_t self_cal_tx_sb_helper::get_min_am_i(uint16_t step,uint16_t coef)
{
    double pwr[512] = {0.0};
    pwr_sb = double(LONG_MAX);
    int16_t idx = 0;
    int16_t idx_min = 0;

    for (am_i_m = am_i_l;am_i_m <= am_i_r;am_i_m += step) {
        INT_CHECK(_sp2401->set_tx_amplitude_balance(am_i_m,am_q_m));
        meas_once(&pwr[idx]);

        if (pwr[idx] < pwr_sb) {
            pwr_sb = pwr[idx];
            idx_min = idx;
        }
        idx ++;
    }
    am_i_m = am_i_l + idx_min * step;
    am_i_l = am_i_m - step * coef;
    am_i_r = am_i_m + step * coef;
    return 0;
}

int32_t self_cal_tx_sb_helper::get_min_am_q(uint16_t step,uint16_t coef)
{
    double pwr[512] = {0.0};
    pwr_sb = double(LONG_MAX);
    int16_t idx = 0;
    int16_t idx_min = 0;

    for (am_q_m = am_q_l;am_q_m <= am_q_r;am_q_m += step) {
        INT_CHECK(_sp2401->set_tx_amplitude_balance(am_i_m,am_q_m));
        meas_once(&pwr[idx]);
        if (pwr[idx] < pwr_sb) {
            pwr_sb = pwr[idx];
            idx_min = idx;
        }
        idx ++;
    }
    am_q_m = am_q_l + idx_min * step;
    am_q_l = am_q_m - step * coef;
    am_q_r = am_q_m + step * coef;
    return 0;
}
