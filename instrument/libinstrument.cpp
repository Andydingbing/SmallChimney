#include "libinstrument.h"
#include "instr.h"

bool instr_init()
{ return Instr.init(); }

bool instr_close()
{ return Instr.close(); }

bool instr_has_sa()
{ return Instr.has_sa(); }

bool instr_has_sg()
{ return Instr.has_sg(); }

bool instr_has_pm()
{ return Instr.has_pm(); }

bool instr_has_ps()
{ return Instr.has_ps(); }

bool instr_sa_reset()
{ return Instr.sa_reset(); }

bool instr_sa_set_cf(double freq)
{ return Instr.sa_set_cf(freq); }

bool instr_sa_set_span(double freq)
{ return Instr.sa_set_span(freq); }

bool instr_sa_set_rbw(double freq,bool is_auto)
{ return Instr.sa_set_rbw(freq,is_auto); }

bool instr_sa_set_vbw(double freq,bool is_auto)
{ return Instr.sa_set_vbw(freq,is_auto); }

bool instr_sa_set_det(det_type_t type)
{ return Instr.sa_set_det(type); }

bool instr_sa_set_peak_search(peak_search_type_t type)
{ return Instr.sa_set_peak_search(type); }

bool instr_sa_set_marker(marker_type_t type)
{ return Instr.sa_set_marker(type); }

bool instr_sa_set_cal(cal_type_t type)
{ return Instr.sa_set_cal(type); }

bool instr_sa_set_avg_trace(bool en,uint32_t cnt)
{ return Instr.sa_set_avg_trace(en,cnt); }

bool instr_sa_set_ref(double ref)
{ return Instr.sa_set_ref(ref); }

bool instr_sa_get_ref(double &ref)
{ return Instr.sa_get_ref(ref); }

bool instr_sa_get_marker_pwr(double &pwr)
{ return Instr.sa_get_marker_pwr(pwr); }

bool instr_sa_set_en_preamp(bool en)
{ return Instr.sa_set_en_preamp(en); }

bool instr_sa_get_marker_freq(double &freq)
{ return Instr.sa_get_marker_freq(freq); }

bool instr_sa_sweep_once()
{ return Instr.sa_sweep_once(); }

bool instr_sa_marker_move(marker_move_type_t type)
{ return Instr.sa_marker_move(type); }

bool instr_sa_marker_move(double freq)
{ return Instr.sa_marker_move(freq); }

bool instr_sa_set_avg_trace_get_data(uint32_t avg_cnt,uint32_t pt_cnt,double *data)
{ return Instr.sa_set_avg_trace_get_data(avg_cnt,pt_cnt,data); }

bool instr_sa_set_mech_att(bool is_auto,int32_t att)
{ return Instr.sa_set_mech_att(is_auto,att); }

bool instr_sa_set_ref_auto()
{ return Instr.sa_set_ref_auto(); }

bool instr_sa_set_ref_auto(double pwr,double &ref)
{ return Instr.sa_set_ref_auto(pwr,ref); }

bool instr_sa_set_ref_auto(double pwr,double &ref,double &offset)
{ return Instr.sa_set_ref_auto(pwr,ref,offset); }

bool instr_sg_reset()
{ return Instr.sg_reset(); }

bool instr_sg_set_cw(double freq)
{ return Instr.sg_set_cw(freq); }

bool instr_sg_get_cw(double &freq)
{ return Instr.sg_get_cw(freq); }

bool instr_sg_set_pl(double pwr)
{ return Instr.sg_set_pl(pwr); }

bool instr_sg_get_pl(double &pwr)
{ return Instr.sg_get_pl(pwr); }

bool instr_sg_set_output_en(bool en)
{ return Instr.sg_set_output_en(en); }

bool instr_sg_set_mod_en(bool en)
{ return Instr.sg_set_mod_en(en); }

bool instr_pm_reset()
{ return Instr.pm_reset(); }

bool instr_pm_set_freq(double freq)
{ return Instr.pm_set_freq(freq); }

bool instr_pm_get_pwr(double &pwr)
{ return Instr.pm_get_pwr(pwr); }

bool instr_pm_get_pwr(double freq,double &pwr)
{ return Instr.pm_get_pwr(freq,pwr); }

bool instr_ps_reset()
{ return Instr.ps_reset(); }

bool instr_ps_set_output_en(const bool en)
{ return Instr.ps_set_output_en(en); }

bool instr_ps_get_output_en(bool &en)
{ return Instr.ps_get_output_en(en); }

bool instr_ps_set_current(const double curr_mA)
{ return Instr.ps_set_current(curr_mA); }

bool instr_ps_get_current(double &curr_mA)
{ return Instr.ps_get_current(curr_mA); }

bool instr_ps_get_current_measured(double &curr_mA)
{ return Instr.ps_get_current_measured(curr_mA); }

bool instr_ps_set_current_max(const double curr_mA)
{ return Instr.ps_set_current_max(curr_mA); }

bool instr_ps_get_current_max(double &curr_mA)
{ return Instr.ps_get_current_max(curr_mA); }

bool instr_ps_set_ocp_en(const bool en)
{ return Instr.ps_set_ocp_en(en); }

bool instr_ps_get_ocp_en(bool &en)
{ return Instr.ps_get_ocp_en(en); }

bool instr_ps_set_voltage(const double vol_mV)
{ return Instr.ps_set_voltage(vol_mV); }

bool instr_ps_get_voltage(double &vol_mV)
{ return Instr.ps_get_voltage(vol_mV); }

bool instr_ps_get_voltage_measured(double &vol_mV)
{ return Instr.ps_get_voltage_measured(vol_mV); }

bool instr_ps_set_voltage_max(const double vol_mV)
{ return Instr.ps_set_voltage_max(vol_mV); }

bool instr_ps_get_voltage_max(double &vol_mV)
{ return Instr.ps_get_voltage_max(vol_mV); }

bool instr_ps_trigger_set_current(const double curr_mA)
{ return Instr.ps_trigger_set_current(curr_mA); }

bool instr_ps_trigger_get_current(double &curr_mA)
{ return Instr.ps_trigger_get_current(curr_mA); }
