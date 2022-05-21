#ifndef LIB_INSTR_H
#define LIB_INSTR_H

#include "preprocessor/prefix.h"

// Common
enum io_mixer_t {
    IO_Mixer_Internal,
    IO_Mixer_External
};

enum io_impedance_t {
    IO_Impedance_50,
    IO_Impedance_75
};

enum io_coupling_t {
    IO_Coupling_AC,
    IO_Coupling_DC
};

enum io_ref_in_t {
    IO_Ref_In_Sense,
    IO_Ref_In_Internal,
    IO_Ref_In_External,
    IO_Ref_In_Pulse
};

enum io_ref_ext_bw_t {
    IO_Ref_Ext_BW_Wide,
    IO_Ref_Ext_BW_Narrow
};

// SA
enum sa_mode_t {
    SA_Mode_SA,
    SA_Mode_RTSA,
    SA_Mode_SEQAN,
    SA_Mode_EMI,
    SA_Mode_BASIC,
    SA_Mode_WCDMA,
    SA_Mode_EDGEGSM,
    SA_Mode_GSM = SA_Mode_EDGEGSM,
    SA_Mode_WIMAXOFDMA,
    SA_Mode_VSA,
    SA_Mode_PNOISE,
    SA_Mode_NFIG,
    SA_Mode_ADEMOD,
    SA_Mode_BT,
    SA_Mode_TDSCDMA,
    SA_Mode_CDMA2K,
    SA_Mode_CDMA1XEV,
    SA_Mode_LTE,
    SA_Mode_LTETDD,
    SA_Mode_LTEAFDD,
    SA_Mode_LTEATDD,
    SA_Mode_MSR,
    SA_Mode_DVB,
    SA_Mode_DTMB,
    SA_Mode_DCTV,
    SA_Mode_ISDBT,
    SA_Mode_CMMB,
    SA_Mode_WLAN,
    SA_Mode_CWLAN,
    SA_Mode_CWIMAXOFDM,
    SA_Mode_WIMAXFIXED,
    SA_Mode_IDEN,
    SA_Mode_RLC,
    SA_Mode_SCPILC,
    SA_Mode_VSA89601
};

enum sa_meas_t {
    SA_Meas_ACP,
    SA_Meas_CHP,
    SA_Meas_Harmonics,
    SA_Meas_List,
    SA_Meas_OBW,
    SA_Meas_CCDF,
    SA_Meas_SEM,
    SA_Meas_Spurious,
    SA_Meas_TOI,
    SA_Meas_TXP,
};

enum sa_radio_standard_t {
    SA_Radio_Standard_NONE,
    SA_Radio_Standard_JSTD,
    SA_Radio_Standard_IS95a,
    SA_Radio_Standard_IS97D,
    SA_Radio_Standard_IS98D,
    SA_Radio_Standard_GSM,
    SA_Radio_Standard_W3GPP,
    SA_Radio_Standard_C2000MC1,
    SA_Radio_Standard_C20001X,
    SA_Radio_Standard_NADC,
    SA_Radio_Standard_PDC,
    SA_Radio_Standard_BLUEtooth,
    SA_Radio_Standard_TETRa,
    SA_Radio_Standard_WL802DOT11A,
    SA_Radio_Standard_WL802DOT11B,
    SA_Radio_Standard_WL802DOT11G,
    SA_Radio_Standard_HIPERLAN2,
    SA_Radio_Standard_DVBTLSN,
    SA_Radio_Standard_DVBTGPN,
    SA_Radio_Standard_DVBTIPN,
    SA_Radio_Standard_FCC15,
    SA_Radio_Standard_SDMBSE,
    SA_Radio_Standard_UWBINDOOR,
    SA_Radio_Standard_LTEB1M4,
    SA_Radio_Standard_LTEB3M,
    SA_Radio_Standard_LTEB5M,
    SA_Radio_Standard_LTEB10M,
    SA_Radio_Standard_LTEB15M,
    SA_Radio_Standard_LTEB20M,
    SA_Radio_Standard_WL11N20M,
    SA_Radio_Standard_WL11N40M,
    SA_Radio_Standard_WL11AC20M,
    SA_Radio_Standard_WL11AC40M,
    SA_Radio_Standard_WL11AC80M,
    SA_Radio_Standard_WL11AC160M,
    SA_Radio_Standard_WL11AD2G
};

enum det_type_t {
    Det_Auto,
    Det_Normal,
    Det_Average,
    Det_Peak,
    Det_Sample,
    Det_Neg_Peak
};

enum peak_search_type_t {
    Peak_Highest,
    Peak_Next,
    Peak_Next_Right,
    Peak_Next_Left
};

enum marker_type_t {
    Marker_Normal,
    Marker_Delta,
    Marker_Off
};

enum marker_move_type_t {
    Marker_To_CF,
    Marker_To_Ref_Level
};

enum cal_type_t {
    Cal_Auto_On,
    Cal_Auto_Off,
    CAL_Now
};

bool API instr_init();
bool API instr_close();
bool API instr_has_sa();
bool API instr_has_sg();
bool API instr_has_pm();
bool API instr_has_ps();

bool API instr_sa_reset();
bool API instr_sa_set_cf(double freq);
bool API instr_sa_set_span(double freq);
bool API instr_sa_set_rbw(double freq,bool is_auto);
bool API instr_sa_set_vbw(double freq,bool is_auto);
bool API instr_sa_set_det(det_type_t type);
bool API instr_sa_set_peak_search(peak_search_type_t type);
bool API instr_sa_set_marker(marker_type_t type);
bool API instr_sa_set_cal(cal_type_t type);
bool API instr_sa_set_avg_trace(bool en,uint32_t cnt);
bool API instr_sa_set_ref(double ref);
bool API instr_sa_get_ref(double &ref);
bool API instr_sa_get_marker_pwr(double &pwr);
bool API instr_sa_set_en_preamp(bool en);
bool API instr_sa_get_marker_freq(double &freq);
bool API instr_sa_sweep_once();
bool API instr_sa_marker_move(marker_move_type_t type);
bool API instr_sa_marker_move(double freq);
bool API instr_sa_set_avg_trace_get_data(uint32_t avg_cnt,uint32_t pt_cnt,double *data);
bool API instr_sa_set_mech_att(bool is_auto,int32_t att);
bool API instr_sa_set_ref_auto();
bool API instr_sa_set_ref_auto(double pwr,double &ref);
bool API instr_sa_set_ref_auto(double pwr,double &ref,double &offset);

bool API instr_sg_reset();
bool API instr_sg_set_cw(double freq);
bool API instr_sg_get_cw(double &freq);
bool API instr_sg_set_pl(double pwr);
bool API instr_sg_get_pl(double &pwr);
bool API instr_sg_set_output_en(bool en);
bool API instr_sg_set_mod_en(bool en);

bool API instr_pm_reset();
bool API instr_pm_set_freq(double freq);
bool API instr_pm_get_pwr(double &pwr);
bool API instr_pm_get_pwr(double freq,double &pwr);

bool API instr_ps_reset();
bool API instr_ps_set_output_en(const bool en);
bool API instr_ps_get_output_en(bool &en);
bool API instr_ps_set_current(const double curr_mA);
bool API instr_ps_get_current(double &curr_mA);
bool API instr_ps_get_current_measured(double &curr_mA);
bool API instr_ps_set_current_max(const double curr_mA);
bool API instr_ps_get_current_max(double &curr_mA);
bool API instr_ps_set_ocp_en(const bool en);
bool API instr_ps_get_ocp_en(bool &en);
bool API instr_ps_set_voltage(const double vol_mV);
bool API instr_ps_get_voltage(double &vol_mV);
bool API instr_ps_get_voltage_measured(double &vol_mV);
bool API instr_ps_set_voltage_max(const double vol_mV);
bool API instr_ps_get_voltage_max(double &vol_mV);
bool API instr_ps_trigger_set_current(const double curr_mA);
bool API instr_ps_trigger_get_current(double &curr_mA);

#endif


