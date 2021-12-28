#ifndef RD_UTILITIES_ALGO_CHIP_PLL_H
#define RD_UTILITIES_ALGO_CHIP_PLL_H

#include "global_header.h"

namespace ns_adf5355 {

struct freq_formula_in {
    uint64_t freq_vco;
    uint32_t freq_ref;
    uint32_t freq_chsp;
    uint32_t r_counter;
    bool ref_doubler;
    bool ref_divider;
    freq_formula_in();
};

struct freq_formula_out {
    uint32_t _int;
    uint32_t _frac1;
    uint32_t _frac2;
    uint32_t _mod1;
    uint32_t _mod2;
};

void freq_formula(const freq_formula_in &in,freq_formula_out &out);
void freq_formula(const freq_formula_out &param,uint32_t freq_ref,double &freq_vco);

SYM_INLINE uint32_t reg0(
    uint32_t _int,
    uint32_t prescaler,
    uint32_t autocal)
{ return (0 | (_int << 4) | (prescaler << 20) | (autocal << 21)); }

SYM_INLINE uint32_t reg1(uint32_t _frac1)
{ return (1 | (_frac1 << 4)); }

SYM_INLINE uint32_t reg2(
    uint32_t _mod2,
    uint32_t _frac2)
{ return (2 | (_mod2 << 4) | (_frac2 << 18)); }

SYM_INLINE uint32_t reg3(
    uint32_t phase_value,
    uint32_t phase_adjust,
    uint32_t phase_resync,
    uint32_t sd_load_reset)
{ return (3 | (phase_value << 4) | (phase_adjust << 28) | (phase_resync << 29) | (sd_load_reset << 30)); }

SYM_INLINE uint32_t reg4(
    uint32_t counter_reset,
    uint32_t cp_three_state,
    uint32_t power_down,
    uint32_t pd_polarity,
    uint32_t mux_logic,
    uint32_t ref_mode,
    uint32_t current_set,
    uint32_t double_buff,
    uint32_t r_counter,
    uint32_t rdiv2,
    uint32_t ref_doubler,
    uint32_t muxout)
{ return 
    (4 | 
    (counter_reset << 4) | 
    (cp_three_state << 5) | 
    (power_down<< 6) | 
    (pd_polarity << 7) | 
    (mux_logic << 8) | 
    (ref_mode << 9) | 
    (current_set << 10) | 
    (double_buff << 14) | 
    (r_counter << 15) | 
    (rdiv2 << 25) | 
    (ref_doubler << 26) | 
    (muxout << 27)); 
}

SYM_INLINE uint32_t reg5()
{ return 0x00800025; }

SYM_INLINE uint32_t reg6(
    uint32_t rf_output_power,
    uint32_t rf_out_a,
    uint32_t rf_out_b,
    uint32_t mtld,
    uint32_t charge_pump_bc,
    uint32_t rf_divider_sel,
    uint32_t feedback_sel,
    uint32_t negative_bleed,
    uint32_t gated_bleed)
{ return 
    (6 | 
    (rf_output_power << 4) | 
    (rf_out_a << 6) | 
    (rf_out_b << 10) | 
    (mtld << 11) | 
    (charge_pump_bc << 13) | 
    (rf_divider_sel << 21) | 
    (feedback_sel << 24) | 
    (0xa << 25) | 
    (negative_bleed << 29) | 
    (gated_bleed << 30));
}

SYM_INLINE uint32_t reg7(
    uint32_t ld_mode,
    uint32_t frac_n_ld_precison,
    uint32_t lol_mode,
    uint32_t ld_cycle_count,
    uint32_t lesync)
{ return 
    (7 |
    (ld_mode << 4) | 
    (frac_n_ld_precison << 5) | 
    (lol_mode << 7) | 
    (ld_cycle_count << 8) | 
    (lesync << 25) | 
    (0x4 << 26));
}

SYM_INLINE uint32_t reg8()
{ return 0x102d0428; }

SYM_INLINE uint32_t reg9(
    uint32_t synth_lock_to,
    uint32_t auto_level_to,
    uint32_t timeout,
    uint32_t vco_band_div)
{ return 
    (9 | 
    (synth_lock_to << 4) | 
    (auto_level_to << 9) | 
    (timeout << 14) | 
    (vco_band_div << 24)); 
}

SYM_INLINE uint32_t reg10(
    uint32_t adc_enable,
    uint32_t adc_conversion,
    uint32_t adc_clock_div)
{
    uint32_t reserved_10 = strtol("000000001100000000",nullptr,2) << 14;
    return (10 | adc_enable << 4 | adc_conversion << 5 | adc_clock_div << 6 | reserved_10);
}

SYM_INLINE uint32_t reg11()
{ return 0x0061300b; }

SYM_INLINE uint32_t reg12(uint32_t resync_clock)
{
    uint32_t reserved_12 = strtol("000001000001",nullptr,2) << 4;
    return (12 | reserved_12 | (resync_clock << 16));
}

} // namespace ns_adf5355


namespace ns_hmc1197 {

struct freq_formula_in {
    uint64_t freq_vco;
    uint32_t freq_ref;
    uint32_t r_counter;
};

struct freq_formula_out {
    uint32_t _int;
    uint32_t _frac;
    uint32_t _chs;
    double   freq_err;
};

void freq_formula_intmode(const freq_formula_in &in,freq_formula_out &out);
void freq_formula_fracmode(const freq_formula_in &in,freq_formula_out &out);
bool freq_formula_exactmode(const freq_formula_in &in,freq_formula_out &out);

} // namespace ns_hmc1197


namespace ns_hmc83x {

struct freq_formula_in {
    uint64_t freq_vco;
    uint32_t freq_ref;
    uint32_t r_div;
    freq_formula_in();
};

struct freq_formula_out {
    uint32_t _int;
    uint32_t _frac;
};

void freq_formula(const freq_formula_in &in,freq_formula_out &out);
void freq_formula(const freq_formula_out &param,uint32_t freq_pd,double &freq_vco);

} // namespace ns_hmc83x


namespace ns_lmx2594 {

struct freq_formula_in {
    uint64_t freq_pd;
    uint64_t freq_rf;
    freq_formula_in();
};

struct freq_formula_out {
    uint16_t DIV; // CHDIV[4:0]
    uint16_t N;
    uint32_t NUM;
    uint32_t DEN;
};

void freq_formula(const freq_formula_in &in,freq_formula_out &out);

SYM_INLINE uint32_t mash_seed(uint16_t reg0x29,uint16_t reg0x28)
{ return uint32_t(reg0x29) | (uint32_t(reg0x28) << 16); }

} // namespace ns_lmx2594

#endif // RD_UTILITIES_ALGO_CHIP_PLL_H
