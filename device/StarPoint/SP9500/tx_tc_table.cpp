#include "tx_tc_table.h"
#include "algo_fit.hpp"
#include "sp1401.h"

using namespace std;
using namespace ns_sp1401;

using namespace descriptive_statistics;

tx_tp_table::data_f_t::data_f_t()
{
    freq = 0;
    for(uint32_t i = 0;i < ARRAY_SIZE(pwr);i ++) {
        pwr[i] = -100.0;
    }
}

void tx_tp_table::data_f_t::set(const double temp,const double new_pwr)
{
    pwr[sp1401::temp_idx(temp)] = new_pwr;
}

bool tx_tp_table::data_f_t::is_ready_to_fit(const double temp_star,const double temp_stop) const
{
    uint32_t idx_star = sp1401::temp_idx(temp_star);
    uint32_t idx_stop = sp1401::temp_idx(temp_stop);
    uint32_t idx_curr = idx_star + 1;

    if (pwr[idx_star] == -100.0 || pwr[idx_stop] == -100.0) {
        return false;
    }
    if (idx_stop - idx_star < 3) {
        return false;
    }

    double pwr_tmp[2] = { pwr[idx_star],pwr[idx_star] };

    while ((idx_curr + 1) < idx_stop) {
        pwr_tmp[0] = pwr[idx_curr];
        pwr_tmp[1] = pwr[idx_curr + 1];

        if (pwr_tmp[0] == -100.0 && pwr_tmp[1] == -100.0) {
            if (pwr[idx_curr - 1] == -100.0) {
                return false;
            }
            if (idx_curr + 1 < idx_stop && pwr[idx_curr + 1] == -100.0) {
                return false;
            }
        }
        idx_curr += 2;
    }
    return true;
}

uint32_t tx_tp_table::data_f_t::fit(const double temp_star,const double temp_stop,
                                    const uint32_t n_max,std::vector<double> &coef) const
{
    uint32_t idx_star = sp1401::temp_idx(temp_star);
    uint32_t idx_stop = sp1401::temp_idx(temp_stop);
    uint32_t points = 0;
    uint32_t n = 0;
    vector<double> x;
    vector<double> y;
    vector<double> y_fit;
    vector<double> each_coef;
    vector<double> variance;
    vector<vector<double>> coefs;

    for (uint32_t i = idx_star;i < idx_stop;++i) {
        if (pwr[i] + 100.0 == 0.0) {
            continue;
        }
        x.push_back(SERIE_VALUE(sp1401::temperature_min(),sp1401::temperature_granularity(),i));
        y.push_back(pwr[i]);
        ++ points;
    }

    for (uint32_t n = 1;n <= n_max;n ++) {
        polyfit<double>(x,y,n,each_coef);
        coefs.push_back(each_coef);

        y_fit.clear();
        for (uint32_t i = 0;i < points;i ++) {
            y_fit.push_back(polynomial<double>(each_coef,x[i]));
        }
        variance.push_back(var(y_fit,y));
    }

    n = uint32_t(min_idx(variance));
    coef.swap(coefs[n]);
    return n + 1;
}

void tx_tp_table::add(const data_f_t *data)
{
    uint32_t idx = SERIE_INDEX(data->freq,tx_freq_star,tx_freq_step_infile);
    m_data_f[idx] = *data;
}

void tx_tp_table::get(uint64_t freq,data_f_t *data)
{
    *data = m_data_f[SERIE_INDEX(freq,tx_freq_star,tx_freq_step_infile)];
}


IMPLEMENT_TX_CAL_ITEM_TABLE(tx_tc_table)

tx_tc_table::data_f_t::data_f_t()
{
    freq = 0;
    ZERO_ARRAY(coef);
    temp_star = 0.0;
    temp_stop = 0.0;
    order = 0;
}

void tx_tc_table::map2mem()
{
    if (m_data_f == nullptr) {
        return;
    }

    uint32_t idx_f = 0;
    uint32_t idx_m = 0;
    uint64_t freq = 0;

    for (idx_m = 0;idx_m < r1c::tx_temp_comp_freqs;idx_m ++) {
        freq = tx_freq_star + idx_m * r1c::tx_temp_comp_freq_step;
        idx_f = SERIE_INDEX(freq,tx_freq_star,tx_freq_step_infile);
        m_data_m[idx_m].order = m_data_f[idx_f].order;
        if (m_data_m[idx_m].order > 0) {
            SAFE_NEW_ARRAY(m_data_m[idx_m].coef,double,m_data_m[idx_m].order + 1);
            for (uint32_t i = 0;i <= m_data_m[idx_m].order;i ++) {
                m_data_m[idx_m].coef[i] = m_data_f[idx_f].coef[i];
            }
            m_data_m[idx_m].temp_star = m_data_f[idx_f].temp_star;
            m_data_m[idx_m].temp_stop = m_data_f[idx_f].temp_stop;
        }
    }
}

void tx_tc_table::get(const uint64_t freq,data_f_t *data)
{
    uint64_t freq_l = 0;
    uint64_t freq_r = 0;

    discretept(freq,tx_freq_step_called,freq_l,freq_r);
    *data = m_data_f[SERIE_INDEX(freq_l,tx_freq_star,tx_freq_step_infile)];
}

void tx_tc_table::get(const uint64_t freq,data_m_t &data)
{
    uint64_t freq_l = linear_quantify(tx_freq_star,r1c::tx_temp_comp_freq_step,freq);
    uint32_t idx = SERIE_INDEX(freq_l,tx_freq_star,r1c::tx_temp_comp_freq_step);

    data.order = m_data_m[idx].order;
    if (data.order > 0) {
        SAFE_NEW_ARRAY(data.coef,double,data.order + 1);
        memcpy(data.coef,m_data_m[idx].coef,(data.order + 1) * sizeof(double));

        data.temp_star = m_data_m[idx].temp_star;
        data.temp_stop = m_data_m[idx].temp_stop;
    }
}

void tx_tc_table::save_as(const char *path)
{

}
