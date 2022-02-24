#ifndef SP9500_SELF_CAL_HELPER_HPP
#define SP9500_SELF_CAL_HELPER_HPP

#include "sp3301.h"

class API self_cal_helper
{
public:
    self_cal_helper(sp3301 *SP3301,uint32_t rf_idx);
    ~self_cal_helper();

    int32_t meas_once(double *pwr);

public:
    complex_sequence _sequence;
    sp3301 *_sp3301;
    sp1401_r1f *_sp1401;
    sp2401_r1a *_sp2401;
    uint32_t _rf_idx;

private:
    int16_t *_I;
    int16_t *_Q;
    int16_t *_I_brother;
    int16_t *_Q_brother;
};


class API self_cal_tx_lol_helper : public self_cal_helper
{
public:
    self_cal_tx_lol_helper(sp3301 *SP3301,uint32_t rf_idx);

    int32_t run();

private:
    int32_t init();
    int32_t get_min_i(uint16_t step,uint16_t coef);
    int32_t get_min_q(uint16_t step,uint16_t coef);

public:
    tx_lol_table_r1cd::data_f_t data;

    double pwr_lol;
    int64_t ad_min;

    int16_t dc_i_l;
    int16_t dc_i_m;
    int16_t dc_i_r;

    int16_t dc_q_l;
    int16_t dc_q_m;
    int16_t dc_q_r;
};


class API self_cal_tx_sb_helper : public self_cal_helper
{
public:
    self_cal_tx_sb_helper(sp3301 *SP3301,uint32_t rf_idx);

    int32_t run(tx_sb_table_r1cd::data_f_t *data);

private:
    int32_t init();
    int32_t get_min_th(double step,double coef);
    int32_t get_min_am_i(uint16_t step,uint16_t coef);
    int32_t get_min_am_q(uint16_t step,uint16_t coef);

public:
    double pwr_sb;

    double th_l;
    double th_m;
    double th_r;

    uint16_t am_i_l;
    uint16_t am_i_m;
    uint16_t am_i_r;

    uint16_t am_q_l;
    uint16_t am_q_m;
    uint16_t am_q_r;
};

#endif
