#include "sg.h"
#include <fmt/core.h>

using namespace std;

sg::sg()
{
    _pwr = 0.0;
}

bool sg::set_cw(double freq)
{
    BOOL_CHECK(w(fmt::format("FREQ:CW {:.0f} Hz;*WAI",freq)));
    BOOL_CHECK(set_pl(_pwr));
    return true;
}

bool sg::get_cw(double &freq)
{
    string buf;

    BOOL_CHECK(w("FREQ:CW?;*WAI"));
    BOOL_CHECK(r(buf,256));
    freq = atof(buf.c_str());
    return true;
}

bool sg::set_pl(double pwr)
{
    BOOL_CHECK(w(fmt::format("POW {:f} dbm;*WAI",pwr)));
    _pwr = pwr;
    return true;
}

bool sg::get_pl(double &pwr)
{
    string buf;

    BOOL_CHECK(w("POW?;*WAI"));
    BOOL_CHECK(r(buf,256));
    pwr = atof(buf.c_str());
    return true;
}

bool sg::set_output_en(bool en)
{
    return en ? w("OUTP ON;*WAI") : w("OUTP OFF;*WAI");
}

bool sg::set_mod_en(bool en)
{
    return en ? w("OUTP:MOD ON;*WAI") : w("OUTP:MOD OFF;*WAI");
}
