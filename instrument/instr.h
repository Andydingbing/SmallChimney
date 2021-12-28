#ifndef INSTR_INSTR_H
#define INSTR_INSTR_H

#include "sa.h"
#include "sg.h"
#include "pm.h"
#include "ps.h"
#include <vector>

#define Instr instr::instance()

class API instr : noncopyable
{
protected:
    instr(void);

public:
    static instr &instance();
    void set_en_expt(bool en);

public:
    bool init();
    bool close();
    bool has_sa() const;
    bool has_sg();
    bool has_pm();
    bool has_ps() const;
    void set_sa(std::string des);
    void set_sg(std::string des);
    void set_pm(std::string des);
    void set_ps(std::string des);

public:  
    bool sa_reset();
    bool sa_set_mode(const sa_mode_t mode) const;
    bool sa_set_meas(const sa_meas_t meas) const;
    bool sa_set_radio_standard(const sa_radio_standard_t standard) const;
    bool sa_set_cf(double freq) const;
    bool sa_set_cf(uint64_t freq) const;
    bool sa_set_span(double freq);
    bool sa_set_rbw(double freq,bool is_auto);
    bool sa_set_vbw(double freq,bool is_auto);
    bool sa_set_det(det_type_t type);
    bool sa_set_peak_search(peak_search_type_t type);
    bool sa_set_marker(marker_type_t type);
    bool sa_set_cal(cal_type_t type);
    bool sa_set_avg_trace(bool en,uint32_t cnt);
    bool sa_set_ref(double ref);
    bool sa_get_ref(double &ref);
    bool sa_get_marker_pwr(double &pwr);
    bool sa_set_en_preamp(bool en);
    bool sa_get_marker_freq(double &freq);
    bool sa_sweep_once();
    bool sa_marker_move(marker_move_type_t type);
    bool sa_marker_move(double freq);
    bool sa_set_avg_trace_get_data(uint32_t avg_cnt,uint32_t pt_cnt,double *data);
    bool sa_set_mech_att(bool is_auto,int32_t att = 0);
    bool sa_set_ref_auto();
    bool sa_set_ref_auto(double pwr,double &ref);
    bool sa_set_ref_auto(double pwr,double &ref,double &offset);

    bool sa_acp_set_offset_freq_stat(const uint32_t idx,const bool en) const;
    bool sa_acp_get_offset_freq_stat(const uint32_t idx,bool &en) const;
    bool sa_acp_set_offset_freq_stat(const std::vector<bool> &stat) const;
    bool sa_acp_get_offset_freq_stat(std::vector<bool> &stat) const;
    bool sa_acp_set_offset_freq(const std::vector<uint64_t> &freq) const;
    bool sa_acp_get_offset_freq(std::vector<uint64_t> &freq) const;
    bool sa_acp_get_result(
            std::vector<double> *dbc_lower = nullptr,
            std::vector<double> *dbc_upper = nullptr,
            std::vector<double> *dbm_lower = nullptr,
            std::vector<double> *dbm_upper = nullptr) const;

public:
    bool sg_reset();
    bool sg_set_cw(double freq);
    bool sg_get_cw(double &freq);
    bool sg_set_pl(double pwr);
    bool sg_get_pl(double &pwr);
    bool sg_set_output_en(bool en);
    bool sg_set_mod_en(bool en);

public:
    bool pm_reset();
    bool pm_set_freq(double freq);
    bool pm_get_pwr(double &pwr);
    bool pm_get_pwr(double freq,double &pwr);

public:
    bool ps_reset();

    bool ps_set_output_en(const bool en) const;
    bool ps_get_output_en(bool &en) const;

    bool ps_set_current(const double curr_mA) const;
    bool ps_get_current(double &curr_mA) const;
    bool ps_get_current_measured(double &curr_mA) const;
    bool ps_set_current_max(const double curr_mA) const;
    bool ps_get_current_max(double &curr_mA) const;
    bool ps_set_ocp_en(const bool en) const;
    bool ps_get_ocp_en(bool &en) const;

    bool ps_set_voltage(const double vol_mV) const;
    bool ps_get_voltage(double &vol_mV) const;
    bool ps_get_voltage_measured(double &vol_mV) const;
    bool ps_set_voltage_max(const double vol_mV) const;
    bool ps_get_voltage_max(double &vol_mV) const;

    bool ps_trigger_set_current(const double curr_mA) const;
    bool ps_trigger_get_current(double &curr_mA) const;

private:
    bool ins_instr(const std::string &idn, ViRsrc des);

private:
    bool _en_expt;
    std::string _des_sa;
    std::string _des_sg;
    std::string _des_pm;
    std::string _des_ps;
    sa *_sa;
    sg *_sg;
    pm *_pm;
    ps *_ps;
    std::vector<sa *> _all_sa;
    std::vector<sg *> _all_sg;
    std::vector<pm *> _all_pm;
    std::vector<ps *> _all_ps;
};

#endif
