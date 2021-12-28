#include "algo_chip_pll.hpp"
#include "algo_math.hpp"
#include <boost/rational.hpp>

namespace ns_adf5355 {

freq_formula_in::freq_formula_in()
{
    freq_ref = 100000000;
    freq_chsp = 6250;
    ref_doubler = false;
    ref_divider = false;
    r_counter = 1;
}

/*
 *                           FRAC2
 *                  FRAC1 + -------
 *                           MOD2
 * RF(out) = INT + ----------------- * f(PD) / Divider
 *                       MOD1
 * RF(out) : the RF output frequency
 * INT     : the integer division factor
 * FRAC1   : the fractionality
 * FRAC2   : thr auxiliary fractionality
 * MOD1    : the fixed 24-bit modulus
 * MOD2    : the auxiliary modulus
 * Divider : the output divider that divides down the VCO frequency
 *
 * f(PD) = Ref(in) * ((1 + D) / (R * (1 + T)))
 * Ref(in) : the reference frequency input
 * D       : the REF(in) doubler bit
 * R       : the REF(in) division factor
 * T       : the REF(in) divide by 2 bit(0 or 1)
 */
void freq_formula(const freq_formula_in &in,freq_formula_out &out)
{
    double freq_pd = double(in.freq_ref);
    freq_pd *= (1.0 + (in.ref_doubler ? 1.0 : 0.0));
    freq_pd /= (1.0 + (in.ref_divider ? 1.0 : 0.0));
    freq_pd /= in.r_counter;

    out._int = uint32_t(in.freq_vco / uint32_t(freq_pd));
    out._mod1 = 16777216;
    out._frac1 = uint32_t((in.freq_vco % uint32_t(freq_pd)) / freq_pd * out._mod1);

    double _mod2_bfr_reduction = freq_pd / boost::gcd(uint32_t(freq_pd),in.freq_chsp);

    double _frac2_bfr_reduction = double(in.freq_vco % uint32_t(freq_pd));
    _frac2_bfr_reduction = _frac2_bfr_reduction / freq_pd * out._mod1 - out._frac1;
    _frac2_bfr_reduction = _frac2_bfr_reduction * _mod2_bfr_reduction;
    _frac2_bfr_reduction = round(_frac2_bfr_reduction,2);

    uint32_t gcd_mod2_frac2 = boost::gcd(uint32_t(_mod2_bfr_reduction),uint32_t(_frac2_bfr_reduction));
    out._mod2  = uint32_t(_mod2_bfr_reduction)  / gcd_mod2_frac2;
    out._frac2 = uint32_t(_frac2_bfr_reduction) / gcd_mod2_frac2;
}

void freq_formula(const freq_formula_out &param,uint32_t freq_ref,double &freq_vco)
{
    freq_vco = double(param._frac2) / double(param._mod2);
    freq_vco += double(param._frac1);
    freq_vco /= double(param._mod1);
    freq_vco += double(param._int);
    freq_vco *= double(freq_ref);
    freq_vco = round(freq_vco,5);
}

} // namespace ns_adf5355


namespace ns_hmc1197 {

void freq_formula_intmode(const freq_formula_in &in,freq_formula_out &out)
{
    double f_pd = double(in.freq_ref) / double(in.r_counter);

    out._int = uint32_t(round((double(in.freq_vco) / f_pd),1));
    out.freq_err = double(in.freq_vco) - double(out._int) * f_pd;
}

void freq_formula_fracmode(const freq_formula_in &in,freq_formula_out &out)
{
    double f_pd = double(in.freq_ref) / double(in.r_counter);
    double _int_all_part = double(in.freq_vco) / f_pd;

    out._int = uint32_t(round(_int_all_part,1));
    out._frac = uint32_t(round((_int_all_part - double(out._int)) * 16777216.0,0));
    out.freq_err = double(in.freq_vco) - (double(out._int) + double(out._frac) / 16777216.0) * f_pd;
}

bool freq_formula_exactmode(const freq_formula_in &in,freq_formula_out &out)
{
    out._frac = out._chs = 0;

    double f_pd = double(in.freq_ref) / double(in.r_counter);
    double _int_all_part = double(in.freq_vco) / f_pd;

    out._int = uint32_t(round(_int_all_part,1));
    uint64_t f_gcd = boost::gcd(in.freq_vco,uint64_t(in.freq_ref / in.r_counter));   // freq_ref = r_counter * integer default

    if (double(f_gcd) > (f_pd / 16777216.0)) {
        out._chs  = uint32_t(in.freq_ref / in.r_counter / f_gcd);
        out._frac = uint32_t(round((_int_all_part - double(out._int)) * 16777216.0,0));
        return true;
    }
    return false;
}

} // namespace ns_hmc1197


namespace ns_hmc83x {

freq_formula_in::freq_formula_in()
{
    freq_ref = 100000000;
    r_div = 2;
}

/*
 *                   FRAC
 * RF(out) = (INT + -------) * f(PD) / Divider
 *                   2<<24
 * RF(out) : the RF output frequency
 * INT     : the integer division factor
 * FRAC    : the fractionality
 * Divider : the output divider that divides down the VCO frequency
 *
 * f(PD) = Ref(in) / R
 * Ref(in) : the reference frequency input
 * R       : the REF(in) division factor
 */
void freq_formula(const freq_formula_in &in,freq_formula_out &out)
{
    double freq_pd = double(in.freq_ref) / in.r_div;

    out._int = uint32_t(double(in.freq_vco) / freq_pd);
    out._frac = uint32_t(round((double(in.freq_vco) / freq_pd - out._int) * 16777216.0));
}

void freq_formula(const freq_formula_out &param,uint32_t freq_pd,double &freq_vco)
{
    freq_vco = double(param._frac) / 16777216.0;
    freq_vco += double(param._int);
    freq_vco *= double(freq_pd);
    freq_vco = round(freq_vco,5);
}

} // namespace ns_hmc832


namespace ns_lmx2594 {

freq_formula_in::freq_formula_in()
{
    freq_pd = FREQ_M(200);
}

void freq_formula(const freq_formula_in &in,freq_formula_out &out)
{
    const uint16_t div[18] = { 2,4,6,8,12,16,24,32,48,64,72,96,128,192,256,384,512,768 };
    const uint64_t step = FREQ_K(1000);
    uint64_t freq_vco = in.freq_rf;
    uint16_t div_idx = 0;

    if (in.freq_rf < FREQ_M(7500)) {
        for (;div_idx < ARRAY_SIZE(div);div_idx ++) {
            freq_vco = in.freq_rf * div[div_idx];
            if (freq_vco >= FREQ_M(7500)) {
                break;
            }
        }
    }

    out.DIV = div_idx;
    out.N = uint16_t(double(freq_vco) / in.freq_pd);
    out.DEN = uint32_t(in.freq_pd / step);
    out.NUM = uint32_t(round((double(freq_vco) / in.freq_pd - out.N) * out.DEN));
}

} // namespace ns_lmx2594
