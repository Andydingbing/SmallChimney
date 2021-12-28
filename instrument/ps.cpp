#include "ps.h"
#include <fmt/core.h>

using namespace std;

bool ps::set_output_en(const bool en) const
{
    return w(en ? "OUTP 1;*WAI" : "OUTP 0;*WAI");
}

bool ps::get_output_en(bool &en) const
{
    BOOL_CHECK(w("SYST:LANG TMSL;*WAI"));
    BOOL_CHECK(w("OUTP?;*WAI"));
    return r_to_bool(en);
}

bool ps::set_current(const double curr_mA) const
{
    return w(fmt::format("CURR {:f} MA;*WAI",curr_mA));
}

bool ps::get_current(double &curr_mA) const
{
    BOOL_CHECK(w("CURR?;*WAI"));
    return r_to_double(curr_mA);
}

bool ps::get_current_measured(double &curr_mA) const
{
    BOOL_CHECK(w("MEAS:CURR?;*WAI"));
    return r_to_double(curr_mA);
}

bool ps::set_current_max(const double curr_mA) const
{
    BOOL_CHECK(w("SYST:LANG COMP;*WAI"));
    BOOL_CHECK(w(fmt::format("IMAX {:f} MA;*WAI",curr_mA)));
    return w("SYST:LANG TMSL;*WAI");
}

bool ps::get_current_max(double &curr_mA) const
{
    BOOL_CHECK(w("SYST:LANG COMP;*WAI"));
    BOOL_CHECK(w("IMAX?;*WAI"));
    BOOL_CHECK(r_to_double(curr_mA));
    return w("SYST:LANG TMSL;*WAI");
}

bool ps::set_ocp_en(const bool en) const
{
    return w(en ? "CURR:PROT:STAT 1;*WAI" : "CURR:PROT:STAT 0;*WAI");
}

bool ps::get_ocp_en(bool &en) const
{
    BOOL_CHECK(w("CURR:PROT:STAT?;*WAI"));
    return r_to_bool(en);
}

bool ps::set_voltage(const double vol_mV) const
{
    return w(fmt::format("VOLT {:f} MV;*WAI",vol_mV));
}

bool ps::get_voltage(double &vol_mV) const
{
    BOOL_CHECK(w("VOLT?;*WAI"));
    return r_to_double(vol_mV);
}

bool ps::get_voltage_measured(double &vol_mV) const
{
    BOOL_CHECK(w("MEAS:VOLT?;*WAI"));
    return r_to_double(vol_mV);
}

bool ps::set_voltage_max(const double vol_mV) const
{
    BOOL_CHECK(w("SYST:LANG COMP;*WAI"));
    BOOL_CHECK(w(fmt::format("VMAX {:f} MV;*WAI",vol_mV)));
    return w("SYST:LANG TMSL;*WAI");
}

bool ps::get_voltage_max(double &vol_mV) const
{
    BOOL_CHECK(w("SYST:LANG COMP;*WAI"));
    BOOL_CHECK(w("VMAX?;*WAI"));
    BOOL_CHECK(r_to_double(vol_mV));
    return w("SYST:LANG TMSL;*WAI");
}

bool ps::trigger_set_current(const double curr_mA) const
{
    return w(fmt::format("CURR:TRIG {:f} MA;*WAI",curr_mA));
}

bool ps::trigger_get_current(double &curr_mA) const
{
    BOOL_CHECK(w("CURR:TRIG?;*WAI"));
    return r_to_double(curr_mA);
}
