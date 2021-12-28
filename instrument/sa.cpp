#include "sa.h"
#include "sleep_common.h"
#include "algo_math.hpp"
#include "utilities.hpp"
#include "floating_point_numbers.hpp"
#include <fmt/core.h>

using namespace std;

bool sa::set_mode(const sa_mode_t mode) const
{
    switch (mode) {
    case SA_Mode_SA :         return w("INST SA;*WAI");
    case SA_Mode_RTSA :       return w("INST RTSA;*WAI");
    case SA_Mode_SEQAN :      return w("INST SEQAN;*WAI");
    case SA_Mode_EMI :        return w("INST EMI;*WAI");
    case SA_Mode_BASIC :      return w("INST BASIC;*WAI");
    case SA_Mode_WCDMA :      return w("INST WCDMA;*WAI");
    case SA_Mode_EDGEGSM :    return w("INST EDGEGSM;*WAI");
    case SA_Mode_WIMAXOFDMA : return w("INST WIMAXOFDMA;*WAI");
    case SA_Mode_VSA :        return w("INST VSA;*WAI");
    case SA_Mode_PNOISE :     return w("INST PNOISE;*WAI");
    case SA_Mode_NFIG :       return w("INST NFIG;*WAI");
    case SA_Mode_ADEMOD :     return w("INST ADEMOD;*WAI");
    case SA_Mode_BT :         return w("INST BT;*WAI");
    case SA_Mode_TDSCDMA :    return w("INST TDSCDMA;*WAI");
    case SA_Mode_CDMA2K :     return w("INST CDMA2K;*WAI");
    case SA_Mode_CDMA1XEV :   return w("INST CDMA1XEV;*WAI");
    case SA_Mode_LTE :        return w("INST LTE;*WAI");
    case SA_Mode_LTETDD :     return w("INST LTETDD;*WAI");
    case SA_Mode_LTEAFDD :    return w("INST LTEAFDD;*WAI");
    case SA_Mode_LTEATDD :    return w("INST LTEATDD;*WAI");
    case SA_Mode_MSR :        return w("INST MSR;*WAI");
    case SA_Mode_DVB :        return w("INST DVB;*WAI");
    case SA_Mode_DTMB :       return w("INST DTMB;*WAI");
    case SA_Mode_DCTV :       return w("INST DCTV;*WAI");
    case SA_Mode_ISDBT :      return w("INST ISDBT;*WAI");
    case SA_Mode_CMMB :       return w("INST CMMB;*WAI");
    case SA_Mode_WLAN :       return w("INST WLAN;*WAI");
    case SA_Mode_CWLAN :      return w("INST CWLAN;*WAI");
    case SA_Mode_CWIMAXOFDM : return w("INST CWIMAXOFDM;*WAI");
    case SA_Mode_WIMAXFIXED : return w("INST WIMAXFIXED;*WAI");
    case SA_Mode_IDEN :       return w("INST IDEN;*WAI");
    case SA_Mode_RLC :        return w("INST RLC;*WAI");
    case SA_Mode_SCPILC :     return w("INST SCPILC;*WAI");
    case SA_Mode_VSA89601 :   return w("INST VSA89601;*WAI");
    }
    return false;
}

bool sa::set_meas(const sa_meas_t meas) const
{
    switch (meas) {
    case SA_Meas_ACP :       return w("CONF:ACP;*WAI");
    case SA_Meas_CHP :       return w("CONF:CHP;*WAI");
    case SA_Meas_Harmonics : return w("CONF:HARM;*WAI");
    case SA_Meas_List :      return w("CONF:LIST;*WAI");
    case SA_Meas_OBW :       return w("CONF:OBW;*WAI");
    case SA_Meas_CCDF :      return w("CONF:PST;*WAI");
    case SA_Meas_SEM :       return w("CONF:SEM;*WAI");
    case SA_Meas_Spurious :  return w("CONF:SPUR;*WAI");
    case SA_Meas_TOI :       return w("CONF:TOI;*WAI");
    case SA_Meas_TXP :       return w("CONF:TXP;*WAI");
    }
    return false;
}

bool sa::set_radio_standard(const sa_radio_standard_t standard) const
{
    switch (standard) {
    case SA_Radio_Standard_NONE :        return w("RAD:STAN NONE");
    case SA_Radio_Standard_JSTD :        return w("RAD:STAN JSTD");
    case SA_Radio_Standard_IS95a :       return w("RAD:STAN IS95a");
    case SA_Radio_Standard_IS97D :       return w("RAD:STAN IS97D");
    case SA_Radio_Standard_IS98D :       return w("RAD:STAN IS98D");
    case SA_Radio_Standard_GSM :         return w("RAD:STAN GSM");
    case SA_Radio_Standard_W3GPP :       return w("RAD:STAN W3GPP");
    case SA_Radio_Standard_C2000MC1 :    return w("RAD:STAN C2000MC1");
    case SA_Radio_Standard_C20001X :     return w("RAD:STAN C20001X");
    case SA_Radio_Standard_NADC :        return w("RAD:STAN NADC");
    case SA_Radio_Standard_PDC :         return w("RAD:STAN PDC");
    case SA_Radio_Standard_BLUEtooth :   return w("RAD:STAN BLUE");
    case SA_Radio_Standard_TETRa :       return w("RAD:STAN TETR");
    case SA_Radio_Standard_WL802DOT11A : return w("RAD:STAN WL802DOT11A");
    case SA_Radio_Standard_WL802DOT11B : return w("RAD:STAN WL802DOT11B");
    case SA_Radio_Standard_WL802DOT11G : return w("RAD:STAN WL802DOT11G");
    case SA_Radio_Standard_HIPERLAN2 :   return w("RAD:STAN HIPERLAN2");
    case SA_Radio_Standard_DVBTLSN :     return w("RAD:STAN DVBTLSN");
    case SA_Radio_Standard_DVBTGPN :     return w("RAD:STAN DVBTGPN");
    case SA_Radio_Standard_DVBTIPN :     return w("RAD:STAN DVBTIPN");
    case SA_Radio_Standard_FCC15 :       return w("RAD:STAN FCC15");
    case SA_Radio_Standard_SDMBSE :      return w("RAD:STAN SDMBSE");
    case SA_Radio_Standard_UWBINDOOR :   return w("RAD:STAN UWBINDOOR");
    case SA_Radio_Standard_LTEB1M4 :     return w("RAD:STAN LTEB1M4");
    case SA_Radio_Standard_LTEB3M :      return w("RAD:STAN LTEB3M");
    case SA_Radio_Standard_LTEB5M :      return w("RAD:STAN LTEB5M");
    case SA_Radio_Standard_LTEB10M :     return w("RAD:STAN LTEB10M");
    case SA_Radio_Standard_LTEB15M :     return w("RAD:STAN LTEB15M");
    case SA_Radio_Standard_LTEB20M :     return w("RAD:STAN LTEB20M");
    case SA_Radio_Standard_WL11N20M :    return w("RAD:STAN WL11N20M");
    case SA_Radio_Standard_WL11N40M :    return w("RAD:STAN WL11N40M");
    case SA_Radio_Standard_WL11AC20M :   return w("RAD:STAN WL11AC20M");
    case SA_Radio_Standard_WL11AC40M :   return w("RAD:STAN WL11AC40M");
    case SA_Radio_Standard_WL11AC80M :   return w("RAD:STAN WL11AC80M");
    case SA_Radio_Standard_WL11AC160M :  return w("RAD:STAN WL11AC160M");
    case SA_Radio_Standard_WL11AD2G :    return w("RAD:STAN WL11AD2G");
    }
    return false;
}

bool sa::set_window_zoom(const bool is_zoomed) const
{
    return is_zoomed ? w("DISP:WIND:FORM:ZOOM;*WAI") : w("DISP:WIND:FORM:TILE;*WAI");
}

bool sa::set_window_next() const
{
    uint8_t n = 0;

    BOOL_CHECK(get_window_number(n));
    n += 1;
    return set_window_number(n);
}

bool sa::set_window_number(const uint8_t n) const
{
    return w(fmt::format("DISP:WIND {:d};*WAI",n));
}

bool sa::get_window_number(uint8_t &n) const
{
    BOOL_CHECK(w("DISP:WIND?;*WAI"));
    return r_to_uint8(n);
}

bool sa::set_window_fullscreen(const bool is_full) const
{
    return is_full ? w("DISP:FSCR 1;*WAI") : w("DISP:FSCR 0;*WAI");
}

bool sa::get_window_fullscreen(bool &is_full) const
{
    BOOL_CHECK(w("DISP:FSCR?;*WAI"));
    return r_to_bool(is_full);
}

bool sa::set_window_display(const bool is_on) const
{
    return is_on ? w("DISP:ENAB 1;*WAI") : w("DISP:ENAB 0;*WAI");
}

bool sa::get_window_display(bool &is_on) const
{
    BOOL_CHECK(w("DISP:ENAB?;*WAI"));
    return r_to_bool(is_on);
}

bool sa::set_io_reset() const
{
    return w("SYST:DEF INP;*WAI");
}

bool sa::set_io_mixer(const io_mixer_t mixer) const
{
    return w(mixer == IO_Mixer_Internal ? "INP:MIX INT;*WAI" : "INP:MIX EXT;*WAI");
}

bool sa::get_io_mixer(io_mixer_t &mixer) const
{
    uint32_t n_mixer = 0;

    BOOL_CHECK(w("INP:MIX?;*WAI"));
    BOOL_CHECK(r_to_uint32(n_mixer));
    mixer = io_mixer_t(n_mixer);
    return true;
}

bool sa::set_io_impedance(const io_impedance_t ohm) const
{
    return w(ohm == IO_Impedance_50 ? "CORR:IMP 50;*WAI" : "CORR:IMP 75;*WAI");
}

bool sa::get_io_impedance(io_impedance_t &ohm) const
{
    uint32_t n_ohm = 0;

    BOOL_CHECK(w("CORR:IMP?;*WAI"));
    BOOL_CHECK(r_to_uint32(n_ohm));
    ohm = io_impedance_t(n_ohm);
    return true;
}

bool sa::set_io_coupling(const io_coupling_t coupling) const
{
    return w(coupling == IO_Coupling_AC ? "INP:COUP AC;*WAI": "INP:COUP DC;*WAI");
}

bool sa::get_io_coupling(io_coupling_t &coupling) const
{
    string buf;

    BOOL_CHECK(w("INP:COUP?;*WAI"));
    BOOL_CHECK(r(buf,32));
    coupling = (buf == "AC" ? IO_Coupling_AC : IO_Coupling_DC);
    return true;
}

bool sa::set_io_ref_in(const io_ref_in_t ref) const
{
    switch (ref) {
    case IO_Ref_In_Sense :    return w("ROSC:SOUR:TYPE SENS;*WAI");
    case IO_Ref_In_Internal : return w("ROSC:SOUR:TYPE INT;*WAI");
    case IO_Ref_In_External : return w("ROSC:SOUR:TYPE EXT;*WAI");
    case IO_Ref_In_Pulse :    return w("ROSC:SOUR:TYPE PULS;*WAI");
    }
    return false;
}

bool sa::get_io_ref_in(io_ref_in_t &ref) const
{
    uint32_t n_ref = 0;

    BOOL_CHECK(w("ROSC:SOUR:TYPE?;*WAI"));
    BOOL_CHECK(r_to_uint32(n_ref));

    if (n_ref == 7 || n_ref == 11) {
        ref = IO_Ref_In_Internal;
    } else if (n_ref == 8 || n_ref == 10) {
        ref = IO_Ref_In_External;
    } else {
        ref = IO_Ref_In_Pulse;
    }
    return true;
}

bool sa::set_io_ref_ext_freq(const double freq) const
{
    return w(fmt::format("ROSC:EXT:FREQ {:f};*WAI",freq));
}

bool sa::get_io_ref_ext_freq(double &freq) const
{
    BOOL_CHECK(w("ROSC:EXT:FREQ?;*WAI"));
    return r_to_double(freq);
}

bool sa::set_io_ref_ext_freq_default() const
{
    return w("ROSC:EXT:FREQ:DEF;*WAI");
}

bool sa::set_io_ref_ext_bw(const io_ref_ext_bw_t bw) const
{
    return w(fmt::format("ROSC:BAND {:s};*WAI",bw == IO_Ref_Ext_BW_Wide ? "WIDE" : "NARR"));
}

bool sa::get_io_ref_ext_bw(io_ref_ext_bw_t &bw) const
{
    string buf;

    BOOL_CHECK(w("ROSC:BAND?;*WAI"));
    BOOL_CHECK(r(buf,32));
    bw = (buf == "WIDE" ? IO_Ref_Ext_BW_Wide : IO_Ref_Ext_BW_Narrow);
    return true;
}

bool sa::set_cf(double freq) const
{
    return w(fmt::format("FREQ:CENT {:.0f};*WAI",freq));
}

bool sa::set_span(double freq) const
{
    return w(fmt::format("FREQ:SPAN {:.0f};*WAI",freq));
}

bool sa::set_rbw(double freq,bool is_auto) const
{
    if (is_auto) {
        return w("BAND:AUTO ON;*WAI");
    }

    return w(fmt::format("BAND {:.0f} Hz;*WAI",freq));
}

bool sa::set_vbw(double freq,bool is_auto) const
{
    if (is_auto) {
        return w("BAND:VID:AUTO ON;*WAI");
    }

    return w(fmt::format("BAND:VID {:.0f} Hz;*WAI",freq));
}

bool sa::set_det(det_type_t type) const
{
    switch (type) {
    case Det_Average :  return w("DET RMS;*WAI");
    case Det_Normal:    return w("DET NORM;*WAI");
    case Det_Peak :     return w("DET POS;*WAI");
    case Det_Auto :     return w("DET:AUTO ON;*WAI");
    case Det_Sample :   return w("DET SAMP;*WAI");
    case Det_Neg_Peak : return w("DET NEG;*WAI");
    }
    return false;
}

bool sa::set_peak_search(peak_search_type_t type) const
{
    switch (type) {
    case Peak_Highest :    return w("CALC:MARK1:MAX;*WAI");
    case Peak_Next :       return w("CALC:MARK1:MAX:NEXT;*WAI");
    case Peak_Next_Right : return w("CALC:MARK1:MAX:RIGH;*WAI");
    case Peak_Next_Left :  return w("CALC:MARK1:MAX:LEFT;*WAI");
    }
    return false;
}

bool sa::set_ref(double ref) const
{
    return w(fmt::format("DISP:WIND:TRAC:Y:RLEV {:f} dbm;*WAI",ref));
}

bool sa::get_ref(double &ref) const
{
    BOOL_CHECK(w("DISP:WIND:TRAC:Y:RLEV?;*WAI"));
    return r_to_double(ref);
}

bool sa::set_marker(marker_type_t type) const
{
    switch (type) {
    case Marker_Off :    return w("CALC:MARK1:STAT OFF;*WAI");
    case Marker_Normal : return w("CALC:MARK1:MODE POS;*WAI");
    case Marker_Delta :  return w("CALC:MARK1:MODE DELT;*WAI");
    }
    return false;
}

bool sa::set_en_preamp(bool en) const
{
    if (en) {
        BOOL_CHECK(w("POW:GAIN:STAT ON;POW:GAIN:BAND FULL;*WAI"));
    } else {
        BOOL_CHECK(w("POW:GAIN:STAT OFF;*WAI"));
    }
    return true;
}

bool sa::is_en_preamp(bool &en) const
{
    BOOL_CHECK(w("POW:GAIN:STAT?;*WAI"));
    return r_to_bool(en);
}

bool sa::get_marker_pwr(double &pwr) const
{
    BOOL_CHECK(w("CALC:MARK1:Y?;*WAI"));
    return r_to_double(pwr);
}

bool sa::get_marker_freq(double &freq) const
{
    BOOL_CHECK(w("CALC:MARK1:X?;*WAI"));
    return r_to_double(freq);
}

bool sa::wait_for_continue() const
{
    return w("*WAI");
}

bool sa::sweep_once() const
{
    string buf;

    BOOL_CHECK(w("INIT:IMM;*WAI"));
    BOOL_CHECK(w("*OPC?"));
    BOOL_CHECK(r(buf,256,60000));
    return true;
}

bool sa::marker_move(marker_move_type_t type) const
{
    return w(type == Marker_To_CF ? "CALC:MARK1:CENT;*WAI" : "CALC:MARK1:RLEV;*WAI");
}

bool sa::marker_move(double freq) const
{
    return w(fmt::format("CALC:MARK1:X {:f} HZ;*WAI",freq));
}

bool sa::set_cal(cal_type_t type) const
{
    switch (type) {
    case Cal_Auto_Off : return w("CAL:AUTO OFF;*WAI");
    case Cal_Auto_On :  return w("CAL:AUTO ON;*WAI");
    case CAL_Now :      return w("CAL:ALL;*WAI");
    }
    return false;
}

bool sa::set_avg_trace(bool en, uint32_t cnt) const
{
    if (en) {
        BOOL_CHECK(w(fmt::format("AVER:COUN {:d};*WAI",cnt)));
        BOOL_CHECK(w("AVER ON;*WAI"));
    } else {
        BOOL_CHECK(w("AVER OFF;*WAI"));
    }
    return true;
}

bool sa::get_trace_avg_cnt(uint32_t &cnt) const
{
    BOOL_CHECK(w("AVER:COUN?;*WAI"));
    return r_to_uint32(cnt);
}

bool sa::is_en_avg_trace(bool &en) const
{
    BOOL_CHECK(w("AVER?;*WAI"));
    return r_to_bool(en);
}

bool sa::set_avg_trace_get_data(uint32_t avg_cnt, uint32_t pt_cnt, double *data) const
{
    uint32_t buf_size = 18 * pt_cnt;
    string buf;

    BOOL_CHECK(w(fmt::format("SWE:POIN {:d};*WAI",pt_cnt)));
    BOOL_CHECK(set_avg_trace(true,avg_cnt));
    BOOL_CHECK(sweep_once());
    BOOL_CHECK(w("FORM ASCii;*WAI"));
    BOOL_CHECK(w("TRAC? TRACE1;*WAI"));
    BOOL_CHECK(r(buf,buf_size));

    uint32_t data_idx = 0;
    uint32_t last_idx = 0;

    for (uint32_t i = 0;i < buf_size;i ++) {
        if (buf[i] == ',' || i == buf_size - 1) {
            buf[i] = 0;
            data[data_idx ++] = atof(&buf[last_idx]);
            last_idx = i + 1;
            i++;
        }
    }

    return true;
}

bool sa::set_mech_att(bool is_auto,int32_t att) const
{
    if (is_auto) {
        BOOL_CHECK(w("POW:ATT:AUTO ON;*WAI"));
    } else {
        BOOL_CHECK(w("POW:ATT:AUTO OFF;*WAI"));
        BOOL_CHECK(w(fmt::format("POW:ATT {:d};*WAI",att)));
    }
    return true;
}

bool sa::acp_set_offset_freq_stat(const uint32_t idx,const bool en) const
{
    vector<bool> stat;

    BOOL_CHECK(acp_get_offset_freq_stat(stat));

    if (idx >= stat.size()) {
        return false;
    }

    stat[idx] = en;
    return acp_set_offset_freq_stat(stat);
}

bool sa::acp_get_offset_freq_stat(const uint32_t idx,bool &en) const
{
    vector<bool> stat;

    BOOL_CHECK(acp_get_offset_freq_stat(stat));

    if (idx >= stat.size()) {
        return false;
    }

    en = stat.at(idx);
    return true;
}

bool sa::acp_set_offset_freq_stat(const vector<bool> &stat) const
{
    string write = "ACP:OFFS1:LIST:STAT ";

    write += string_of<string>(stat,",");
    write.pop_back();
    write += ";*WAI";
    return w(write);
}

bool sa::acp_get_offset_freq_stat(vector<bool> &stat) const
{
    string read;

    BOOL_CHECK(w("ACP:OFFS1:LIST:STAT?;*WAI"));
    BOOL_CHECK(r(read,64));

    string_to_container<bool>(read,',',stat);
    return true;
}

bool sa::acp_set_offset_freq(const std::vector<uint64_t> &freq) const
{
    string write = "ACP:OFFS1:LIST ";

    write += string_of<string>(freq,",");
    write.pop_back();
    write += ";*WAI";
    return w(write);
}

bool sa::acp_get_offset_freq(std::vector<uint64_t> &freq) const
{
    string read;

    BOOL_CHECK(w("ACP:OFFS1:LIST?;*WAI"));
    BOOL_CHECK(r(read,256));

    return 0;
}

bool sa::acp_get_result(
        std::vector<double> *dbc_lower,
        std::vector<double> *dbc_upper,
        std::vector<double> *dbm_lower,
        std::vector<double> *dbm_upper) const
{
    string read;
    size_t pos[2] = {0,0};
    vector<double> read_datas;

    BOOL_CHECK(w("CALC:DATA1?"));
    BOOL_CHECK(r(read,1024));

    double each = 0.0;

    while ((pos[1] = read.find_first_of(",",pos[0])) != string::npos) {
        floating_point_numbers::normal_notation(read.substr(pos[0],pos[1]),each);
        read_datas.push_back(each);
        pos[0] = pos[1] + 1;
    }

    for (size_t i = 4;i < read_datas.size();++i) {
        if (dbc_lower != nullptr) {
            dbc_lower->push_back(read_datas.at(i));
        }

        ++i;
        if (dbm_lower != nullptr) {
            dbm_lower->push_back(read_datas.at(i));
        }

        ++i;
        if (dbc_upper != nullptr) {
            dbc_upper->push_back(read_datas.at(i));
        }

        ++i;
        if (dbm_upper != nullptr) {
            dbm_upper->push_back(read_datas.at(i));
        }
    }
    return true;
}

bool sa::set_ref_auto() const
{
    double ref = 0.0;
    double peak_pwr = 0.0;
    double delta = 0.0;

    BOOL_CHECK(sweep_once());
    BOOL_CHECK(set_peak_search(Peak_Highest));
    BOOL_CHECK(get_marker_pwr(peak_pwr));
    BOOL_CHECK(get_ref(ref));

    delta = ref - peak_pwr;

    if (delta > 0.0 && delta <= 40.0) {
        return true;
    }

    do {
        BOOL_CHECK(set_ref(int(peak_pwr) + 5));
        BOOL_CHECK(sweep_once());
        BOOL_CHECK(get_marker_pwr(peak_pwr));
        BOOL_CHECK(get_ref(ref));
    } while(peak_pwr > ref);
    sleep_ms(200);
    return true;
}

bool sa::set_ref_auto(double pwr,double &ref) const
{
    pwr += 10.0;
    double ref_cur = 0.0;

    BOOL_CHECK(get_ref(ref_cur));

    ref = linear_quantify(20.0,-20.0,pwr);

    if (ref_cur - ref != 0.0) {
        BOOL_CHECK(set_ref(ref));
    } else {
        return true;
    }

    if (pwr < -40.0) {
        BOOL_CHECK(set_en_preamp(true));
        BOOL_CHECK(set_mech_att(false,0));

        if (pwr > -60.0) {
            BOOL_CHECK(set_avg_trace(true,3));
        } else {
            BOOL_CHECK(set_avg_trace(true,6));
        }
        sleep_ms(100);
    }
    return true;

}

bool sa::set_ref_auto(double pwr,double &ref,double &offset) const
{
    pwr += 10.0;

    double pwr_before = 0.0;
    double pwr_after  = 0.0;
    double ref_cur = 0.0;
    bool change_setting = false;

    BOOL_CHECK(get_ref(ref_cur));

    ref = linear_quantify(20.0,-20.0,pwr);

    if (ref_cur - ref != 0.0) {
        change_setting = true;

        BOOL_CHECK(sweep_once());
        BOOL_CHECK(set_peak_search(Peak_Highest));
        BOOL_CHECK(get_marker_pwr(pwr_before));
        BOOL_CHECK(set_ref(ref));
    } else {
        return true;
    }

    if (pwr < -40.0) {
        BOOL_CHECK(set_en_preamp(true));
        BOOL_CHECK(set_mech_att(false,0));

        if (pwr > -60.0) {
            BOOL_CHECK(set_avg_trace(true,3));
        } else {
            BOOL_CHECK(set_avg_trace(true,6));
        }
        sleep_ms(100);
    }

    BOOL_CHECK(sweep_once());
    BOOL_CHECK(set_peak_search(Peak_Highest));
    BOOL_CHECK(get_marker_pwr(pwr_after));

    offset += pwr_after - pwr_before;
    return true;

}
