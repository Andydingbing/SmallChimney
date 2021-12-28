#include "libdriver.h"
#include "instr.h"

#include "sa_keysight_psa.h"
#include "sa_keysight_x.h"
#include "sa_rs_fsup.h"

#include "sg_keysight_esg.h"
#include "sg_keysight_psg.h"
#include "sg_keysight_x.h"
#include "sg_rs_smf100a.h"

#include "pm_rs_nrp.h"
#include "rsnrpz.h"

#include "ps_keysight_603xa.h"
#include "exception.hpp"
#include <string>

using namespace std;

#define INSTR_CHECK(func) \
{   if (!(func)) { \
        if (_en_expt) { throw ::runtime_error(__FUNCTION__); } \
        else { return false; } \
    } \
}


instr::instr(void) :
    _des_sa("\0"),
    _des_sg("\0"),
    _des_pm("\0"),
    _des_ps("\0")
{
    _all_sa.push_back(new keysight_e4440a());
    _all_sa.push_back(new keysight_e4443a());
    _all_sa.push_back(new keysight_e4445a());
    _all_sa.push_back(new keysight_e4446a());
    _all_sa.push_back(new keysight_e4447a());
    _all_sa.push_back(new keysight_e4448a());
    _all_sa.push_back(new keysight_n9000a());
    _all_sa.push_back(new keysight_n9010a());
    _all_sa.push_back(new keysight_n9020a());
    _all_sa.push_back(new keysight_n9020b());
    _all_sa.push_back(new keysight_n9030a());
    _all_sa.push_back(new rs_fsup());

    _all_sg.push_back(new keysight_n5166b());
    _all_sg.push_back(new keysight_n5171b());
    _all_sg.push_back(new keysight_n5172b());
    _all_sg.push_back(new keysight_n5173b());
    _all_sg.push_back(new keysight_n5182a());
    _all_sg.push_back(new keysight_n5181b());
    _all_sg.push_back(new keysight_n5182b());
    _all_sg.push_back(new keysight_n5183b());
    _all_sg.push_back(new keysight_e4428c());
    _all_sg.push_back(new keysight_e4438c());
    _all_sg.push_back(new keysight_e8257d());
    _all_sg.push_back(new keysight_e8267d());
    _all_sg.push_back(new keysight_e8663d());
    _all_sg.push_back(new rs_smf100a());
    _all_sg.push_back(new rs_sma100a());

    _all_pm.push_back(new rs_nrp());

    _all_ps.push_back(new keysight_6010a());
    _all_ps.push_back(new keysight_6011a());
    _all_ps.push_back(new keysight_6012b());
    _all_ps.push_back(new keysight_6015a());
    _all_ps.push_back(new keysight_6030a());
    _all_ps.push_back(new keysight_6031a());
    _all_ps.push_back(new keysight_6032a());
    _all_ps.push_back(new keysight_6033a());
    _all_ps.push_back(new keysight_6035a());
    _all_ps.push_back(new keysight_6038a());

    _en_expt = true;
    _sa = nullptr;
    _sg = nullptr;
    _pm = nullptr;

    pci_dev_vi::open_default_rm();
}

instr& instr::instance()
{
    static instr g_instr;
    return g_instr;
}

void instr::set_en_expt(bool en)
{
    _en_expt = en;
}

bool instr::init()
{
    ViFindList find_list = 0;
    ViUInt32 ret_cnt = 0;
    ViUInt32 ret = 0;
    ViChar des[256] = {0};
    ViChar idn[256] = {0};
    ViSession session = 0;
    ViChar expr[32] = "GPIB?*INSTR";

    _sa = nullptr;
    _sg = nullptr;
    _pm = nullptr;

    if (viFindRsrc(pci_dev_vi::get_default_rm(),expr,&find_list,&ret_cnt,des) < VI_SUCCESS) {
        if (_en_expt) {
            throw ::runtime_error("vi find rsrc GPIB?*INSTR");
        } else {
            return false;
        }
	}

    while (ret_cnt --) {
        if (viOpen(pci_dev_vi::get_default_rm(),des,VI_NULL,VI_NULL,&session) < VI_SUCCESS) {
            viFindNext(find_list,des);
			continue;
		}
        if (viWrite(session,ViBuf("*IDN?"),5,&ret) < VI_SUCCESS) {
            viClose(session);
            viFindNext(find_list,des);
			continue;
		}
        if (viRead(session,ViPBuf(idn),256,&ret) < VI_SUCCESS) {
            viClose(session);
            viFindNext(find_list,des);
			continue;
		}
        viClose(session);
        ins_instr(string(idn),des);
        viFindNext(find_list,des);
	}
    viClose(find_list);

    ViStatus status = VI_SUCCESS;
    ViInt32 num_sensors  = 0;
    char sensor_name[256] = {0};
    char sensor_type[256] = {0};
    char sensor_sn[256] = {0};

    for (uint32_t retry = 0;retry < 2;retry ++) {
        if ((status = rsnrpz_GetSensorCount(0,&num_sensors)) != VI_SUCCESS) {
            if (_en_expt) {
                ViChar msg[256] = {0};
                rsnrpz_error_message(VI_NULL,status,msg);
                throw ::runtime_error(msg);
            } else {
                return false;
            }
		}
        if (num_sensors > 0) {
			break;
        }
	}

    if (num_sensors < 1) {
		return true;
    }

    for (ViInt32 i = 1;i <= num_sensors;i ++) {
        if ((status = rsnrpz_GetSensorInfo(0,i,sensor_name,sensor_type,sensor_sn)) != VI_SUCCESS) {
            if (_en_expt) {
                ViChar msg[256] = {0};
                rsnrpz_error_message(VI_NULL,status,msg);
                throw ::runtime_error(msg);
            } else {
                return false;
            }
		}
        ins_instr(string("ROHDE&SCHWARZ,NRP"),sensor_name);
	}

	return true;
}

bool instr::ins_instr(const std::string &idn, ViRsrc des)
{
    bool specified_sa = !_des_sa.empty();
    bool specified_sg = !_des_sg.empty();
    bool specified_pm = !_des_pm.empty();
    bool specified_ps = !_des_ps.empty();
    string descriptor;

    for (size_t i = 0;i < _all_sa.size();i ++) {
        descriptor = (specified_sa ? _des_sa : _all_sa.at(i)->descriptor());
        if (idn.find(descriptor) != string::npos) {
            if (!_all_sa.at(i)->init(des)) {
				continue;
            } else {
                _sa = _all_sa.at(i);
				return true;
			}
		}
	}

    for (size_t i = 0;i < _all_sg.size();i ++) {
        descriptor = (specified_sg ? _des_sg : _all_sg.at(i)->descriptor());
        if (idn.find(descriptor) != string::npos) {
            if (!_all_sg.at(i)->init(des)) {
				continue;
            } else {
                _sg = _all_sg.at(i);
				return true;
			}
		}
	}

    for (size_t i = 0;i < _all_pm.size();i ++) {
        descriptor = (specified_pm ? _des_pm : _all_pm.at(i)->descriptor());
        if (idn.find(descriptor) != string::npos) {
            if (!_all_pm.at(i)->init(des)) {
				continue;
            } else {
                _pm = _all_pm.at(i);
				return true;
			}
		}
	}

    for (size_t i = 0;i < _all_ps.size();i ++) {
        descriptor = (specified_ps ? _des_ps : _all_ps.at(i)->descriptor());
        if (idn.find(descriptor) != string::npos) {
            if (!_all_ps.at(i)->init(des)) {
                continue;
            } else {
                _ps = _all_ps.at(i);
                return true;
            }
        }
    }

	return true;
}

bool instr::close()
{
    if (_pm) {
        INSTR_CHECK(_pm->close());
    }

	return true;
}

bool instr::has_sa() const
{
    if (_sa == nullptr) {
        if (_en_expt) {
            throw ::runtime_error((_des_sa.empty() ? "sa" : _des_sa) + " disconnect");
        } else {
			return false;
		}
	}
	return true;
}

bool instr::has_sg()
{
    if (_sg == nullptr) {
        if (_en_expt) {
            throw ::runtime_error((_des_sg.empty() ? "sg" : _des_sg) + " disconnect");
        } else {
			return false;
		}
	}
	return true;
}

bool instr::has_pm()
{
    if (_pm == nullptr) {
        if (_en_expt) {
            throw ::runtime_error((_des_pm.empty() ? "pm" : _des_pm) + " disconnect");
        } else {
			return false;
		}
	}
	return true;
}

bool instr::has_ps() const
{
    if (_ps == nullptr) {
        if (_en_expt) {
            throw ::runtime_error((_des_ps.empty() ? "ps" : _des_ps) + " disconnect");
        } else {
            return false;
        }
    }
    return true;
}

void instr::set_sa(std::string des)
{
    _des_sa = des;
}

void instr::set_sg(std::string des)
{
    _des_sg = des;
}

void instr::set_pm(std::string des)
{
    _des_pm = des;
}

void instr::set_ps(std::string des)
{
    _des_ps = des;
}

bool instr::sa_reset()
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->reset());
    INSTR_CHECK(_sa->set_cal(Cal_Auto_Off));
	return true;
}

bool instr::sa_set_mode(const sa_mode_t mode) const
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->set_mode(mode));
    return true;
}

bool instr::sa_set_meas(const sa_meas_t meas) const
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->set_meas(meas));
    return true;
}


bool instr::sa_set_radio_standard(const sa_radio_standard_t standard) const
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->set_radio_standard(standard));
    return true;
}

bool instr::sa_set_cf(double freq) const
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->set_cf(freq));
	return true;
}

bool instr::sa_set_cf(uint64_t freq) const
{
    return sa_set_cf(double(freq));
}

bool instr::sa_set_span(double freq)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->set_span(freq));
	return true;
}

bool instr::sa_set_rbw(double freq,bool is_auto)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->set_rbw(freq,is_auto));
	return true;
}

bool instr::sa_set_vbw(double freq,bool is_auto)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->set_vbw(freq,is_auto));
	return true;
}

bool instr::sa_set_det(det_type_t type)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->set_det(type));
	return true;
}

bool instr::sa_set_peak_search(peak_search_type_t type)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->set_peak_search(type));
	return true;
}

bool instr::sa_set_ref(double ref)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->set_ref(ref));
	return true;
}

bool instr::sa_get_ref(double &ref)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->get_ref(ref));
	return true;
}

bool instr::sa_set_ref_auto()
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->set_ref_auto());
	return true;
}

bool instr::sa_set_ref_auto(double pwr,double &ref)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->set_ref_auto(pwr,ref));
    return true;
}

bool instr::sa_set_ref_auto(double pwr,double &ref,double &offset)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->set_ref_auto(pwr,ref,offset));
    return true;
}

bool instr::sa_acp_set_offset_freq_stat(const uint32_t idx,const bool en) const
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->acp_set_offset_freq_stat(idx,en));
    return true;
}

bool instr::sa_acp_get_offset_freq_stat(const uint32_t idx,bool &en) const
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->acp_get_offset_freq_stat(idx,en));
    return true;
}

bool instr::sa_acp_set_offset_freq_stat(const std::vector<bool> &stat) const
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->acp_set_offset_freq_stat(stat));
    return true;
}

bool instr::sa_acp_get_offset_freq_stat(std::vector<bool> &stat) const
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->acp_get_offset_freq_stat(stat));
    return true;
}

bool instr::sa_acp_set_offset_freq(const std::vector<uint64_t> &freq) const
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->acp_set_offset_freq(freq));
    return true;
}

bool instr::sa_acp_get_offset_freq(std::vector<uint64_t> &freq) const
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->acp_get_offset_freq(freq));
    return true;
}

bool instr::sa_acp_get_result(
        std::vector<double> *dbc_lower,
        std::vector<double> *dbc_upper,
        std::vector<double> *dbm_lower,
        std::vector<double> *dbm_upper) const
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->acp_get_result(dbc_lower,dbc_upper,dbm_lower,dbm_upper));
    return true;
}

bool instr::sa_set_marker(marker_type_t type)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->set_marker(type));
	return true;
}

bool instr::sa_get_marker_pwr(double &pwr)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->get_marker_pwr(pwr));
	return true;
}

bool instr::sa_get_marker_freq(double &freq)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->get_marker_freq(freq));
	return true;
}

bool instr::sa_set_en_preamp(bool en)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->set_en_preamp(en));
	return true;
}

bool instr::sa_marker_move(marker_move_type_t type)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->marker_move(type));
	return true;
}

bool instr::sa_marker_move(double freq)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->marker_move(freq));
	return true;
}

bool instr::sa_sweep_once()
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->sweep_once());
	return true;
}

bool instr::sa_set_cal(cal_type_t type)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->set_cal(type));
	return true;
}

bool instr::sa_set_avg_trace(bool en, uint32_t cnt)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->set_avg_trace(en,cnt));
	return true;
}

bool instr::sa_set_avg_trace_get_data(uint32_t avg_cnt, uint32_t pt_cnt, double *data)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->set_avg_trace_get_data(avg_cnt,pt_cnt,data));
	return true;
}

bool instr::sa_set_mech_att(bool is_auto,int32_t att)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->set_mech_att(is_auto,att));
	return true;
}

bool instr::sg_reset()
{
    BOOL_CHECK(has_sg());
    INSTR_CHECK(_sg->reset());
	return true;
}

bool instr::sg_set_cw(double freq)
{
    BOOL_CHECK(has_sg());
    INSTR_CHECK(_sg->set_cw(freq));
	return true;
}

bool instr::sg_get_cw(double &freq)
{
    BOOL_CHECK(has_sg());
    INSTR_CHECK(_sg->get_cw(freq));
	return true;
}

bool instr::sg_set_pl(double pwr)
{
    BOOL_CHECK(has_sg());
    INSTR_CHECK(_sg->set_pl(pwr));
	return true;
}

bool instr::sg_get_pl(double &pwr)
{
    BOOL_CHECK(has_sg());
    INSTR_CHECK(_sg->get_pl(pwr));
	return true;
}

bool instr::sg_set_output_en(bool en)
{
    BOOL_CHECK(has_sg());
    INSTR_CHECK(_sg->set_output_en(en));
	return true;
}

bool instr::sg_set_mod_en(bool en)
{
    BOOL_CHECK(has_sg());
    INSTR_CHECK(_sg->set_mod_en(en));
	return true;
}

bool instr::pm_reset()
{
    BOOL_CHECK(has_pm());
    INSTR_CHECK(_pm->reset());
	return true;
}

bool instr::pm_set_freq(double freq)
{
    BOOL_CHECK(has_pm());
    INSTR_CHECK(_pm->set_freq(freq));
	return true;
}

bool instr::pm_get_pwr(double &pwr)
{
    BOOL_CHECK(has_pm());
    INSTR_CHECK(_pm->get_pwr(pwr));
	return true;
}

bool instr::pm_get_pwr(double freq,double &pwr)
{
    BOOL_CHECK(has_pm());
    INSTR_CHECK(_pm->get_pwr(freq,pwr));
	return true;
}

bool instr::ps_reset()
{
    BOOL_CHECK(has_ps());
    INSTR_CHECK(_ps->reset());
    return true;
}

bool instr::ps_set_output_en(const bool en) const
{
    BOOL_CHECK(has_ps());
    return _ps->set_output_en(en);
}

bool instr::ps_get_output_en(bool &en) const
{
    BOOL_CHECK(has_ps());
    return _ps->get_output_en(en);
}

bool instr::ps_set_current(const double curr_mA) const
{
    BOOL_CHECK(has_ps());
    return _ps->set_current(curr_mA);
}

bool instr::ps_get_current(double &curr_mA) const
{
    BOOL_CHECK(has_ps());
    return _ps->get_current(curr_mA);
}

bool instr::ps_get_current_measured(double &curr_mA) const
{
    BOOL_CHECK(has_ps());
    return _ps->get_current_measured(curr_mA);
}

bool instr::ps_set_current_max(const double curr_mA) const
{
    BOOL_CHECK(has_ps());
    return _ps->set_current_max(curr_mA);
}

bool instr::ps_get_current_max(double &curr_mA) const
{
    BOOL_CHECK(has_ps());
    return _ps->get_current_max(curr_mA);
}

bool instr::ps_set_ocp_en(const bool en) const
{
    BOOL_CHECK(has_ps());
    return _ps->set_ocp_en(en);
}

bool instr::ps_get_ocp_en(bool &en) const
{
    BOOL_CHECK(has_ps());
    return _ps->get_ocp_en(en);
}

bool instr::ps_set_voltage(const double vol_mV) const
{
    BOOL_CHECK(has_ps());
    return _ps->set_voltage(vol_mV);
}

bool instr::ps_get_voltage(double &vol_mV) const
{
    BOOL_CHECK(has_ps());
    return _ps->get_voltage(vol_mV);
}

bool instr::ps_get_voltage_measured(double &vol_mV) const
{
    BOOL_CHECK(has_ps());
    return _ps->get_voltage_measured(vol_mV);
}

bool instr::ps_set_voltage_max(const double vol_mV) const
{
    BOOL_CHECK(has_ps());
    return _ps->set_voltage_max(vol_mV);
}

bool instr::ps_get_voltage_max(double &vol_mV) const
{
    BOOL_CHECK(has_ps());
    return _ps->get_voltage_max(vol_mV);
}

bool instr::ps_trigger_set_current(const double curr_mA) const
{
    BOOL_CHECK(has_ps());
    return _ps->trigger_set_current(curr_mA);
}

bool instr::ps_trigger_get_current(double &curr_mA) const
{
    BOOL_CHECK(has_ps());
    return _ps->trigger_get_current(curr_mA);
}
