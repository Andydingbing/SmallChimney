#include "sa_rs_fsup.h"
#include <stdlib.h>
#include <string>
#include <fmt/core.h>
#include <boost/core/ignore_unused.hpp>

using namespace std;
using namespace boost;

bool rs_fsup::reset()
{
    return w("SYSTem:PRESet;*WAI");
}

bool rs_fsup::set_cf(double freq) const
{
    return w(fmt::format("FREQ:CENT {:.0f};*WAI",freq));
}

bool rs_fsup::set_span(double freq) const
{
    return w(fmt::format("FREQ:SPAN {:.0f};*WAI",freq));
}

bool rs_fsup::set_rbw(double freq, bool is_auto) const
{
    if (is_auto) {
        return w("BAND:AUTO ON;*WAI");
    }

    return w(fmt::format("BAND {:.0f} Hz;*WAI",freq));
}

bool rs_fsup::set_vbw(double freq, bool is_auto) const
{
    if (is_auto) {
        return w("BAND:VID:AUTO ON;*WAI");
    }

    return w(fmt::format("BAND:VID {:.0f} Hz;*WAI",freq));
}

bool rs_fsup::set_det(det_type_t type) const
{
    switch (type) {
    case Det_Average  : return w("DET RMS;*WAI");
    case Det_Normal   : return w("DET APE;*WAI");
    case Det_Peak     : return w("DET POS;*WAI");
    case Det_Auto     : return w("DET:AUTO ON;*WAI");
    case Det_Sample   : return w("DET SAMP;*WAI");
    case Det_Neg_Peak : return w("DET NEG;*WAI");
	}
	return false;
}

bool rs_fsup::set_peak_search(peak_search_type_t type) const
{
    switch (type) {
    case Peak_Highest    : return w("CALC:MARK1:MAX;*WAI");
    case Peak_Next       : return w("CALC:MARK1:MAX:NEXT;*WAI");
    case Peak_Next_Right : return w("CALC:MARK1:MAX:RIGH;*WAI");
    case Peak_Next_Left  : return w("CALC:MARK1:MAX:LEFT;*WAI");
	}
	return false;
}

bool rs_fsup::set_ref(double ref) const
{
    return w(fmt::format("DISP:WIND:TRAC:Y:RLEV {:f} dbm;*WAI",ref));
}

bool rs_fsup::get_ref(double &ref) const
{
    string buf;

    BOOL_CHECK(w("DISP:WIND:TRAC:Y:RLEV?;*WAI"));
    BOOL_CHECK(r(buf,256));
    ref = atof(buf.c_str());
	return true;
}

bool rs_fsup::set_marker(marker_type_t type) const
{
    switch (type) {
    case Marker_Off    : return w("CALC:MARK1:STAT OFF;*WAI");
    case Marker_Normal : return w("CALC:MARK1:MODE POS;*WAI");
    case Marker_Delta  : return w("CALC:MARK1:MODE DELT;*WAI");
	}
	return false;
}

bool rs_fsup::set_en_preamp(bool en) const
{
    return w(fmt::format("INP:GAIN:STAT {:s};*WAI",en ? "ON" : "OFF"));
}

bool rs_fsup::is_en_preamp(bool &en) const
{
    string buf;

    BOOL_CHECK(w("INP:GAIN:STAT?;*WAI"));
    BOOL_CHECK(r(buf,256));

    en = atoi(buf.c_str()) == 1;
    return true;
}

bool rs_fsup::get_marker_pwr(double &pwr) const
{
    string buf;
    double freq = 0.0;

    BOOL_CHECK(w("CALC:MARK1:Y?;*WAI"));
    BOOL_CHECK(r(buf,256));
    BOOL_CHECK(get_marker_freq(freq));

    pwr = atof(buf.c_str());
	return true;
}

bool rs_fsup::get_marker_freq(double &freq) const
{
    string buf;

    BOOL_CHECK(w("CALC:MARK1:X?;*WAI"));
    BOOL_CHECK(r(buf, 200));
    freq = atof(buf.c_str());
	return true;
}

bool rs_fsup::wait_for_continue() const
{
    return w("*WAI");
}

bool rs_fsup::sweep_once() const
{
    BOOL_CHECK(w("INIT1:CONT OFF;*WAI;"));
    BOOL_CHECK(w("INIT1:IMM;*WAI;"));
	return true;
}

bool rs_fsup::marker_move(marker_move_type_t type) const
{
    switch (type) {
    case Marker_To_CF        : return w("CALC:MARK1:FUNC:CENT;*WAI;");
    case Marker_To_Ref_Level : return w("CALC:MARK1:FUNC:REF;*WAI;");
	}
	return false;
}

bool rs_fsup::marker_move(double freq) const
{
    return w(fmt::format("CALC:MARK1:X {:f} HZ;*WAI",freq));
}

bool rs_fsup::set_cal(cal_type_t type) const
{
    ignore_unused(type);
    return w("CAL:ALL;*WAI");
}

bool rs_fsup::set_avg_trace(bool en,uint32_t cnt) const
{
    if (en) {
        BOOL_CHECK(w(fmt::format("AVER:COUN {:d};*WAI",cnt)));
        BOOL_CHECK(w("AVER:STAT ON;*WAI"));
	} else {
        BOOL_CHECK(w("AVER:STAT OFF;*WAI"));
	}
	return true;
}

bool rs_fsup::get_trace_avg_cnt(uint32_t &cnt) const
{
    string buf;

    BOOL_CHECK(w("AVER:COUN?;*WAI"));
    BOOL_CHECK(r(buf,256));

    cnt = uint32_t(atol(buf.c_str()));
    return true;
}

bool rs_fsup::is_en_avg_trace(bool &en) const
{
    string buf;

    BOOL_CHECK(w("AVER:STAT?;*WAI"));
    BOOL_CHECK(r(buf,256));

    en = atoi(buf.c_str()) == 1;
    return true;
}

bool rs_fsup::set_avg_trace_get_data(uint32_t avg_cnt,uint32_t pt_cnt,double *data) const
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

bool rs_fsup::set_mech_att(bool is_auto,int32_t att) const
{
    ignore_unused(is_auto,att);
	return false;
}
