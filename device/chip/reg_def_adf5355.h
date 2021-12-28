#ifndef INCLUDE_REG_DEF_ADF5355_H
#define INCLUDE_REG_DEF_ADF5355_H

#include "reg_def.h"

namespace ns_adf5355_reg {

REG_DEF_CPT_PREFIX_32(0x00)
    unsigned ctrl      : 4;
    unsigned _int      : 16;
    unsigned prescaler : 1;
    unsigned autocal   : 1;
    unsigned rsv       : 10;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x01)
    unsigned ctrl   : 4;
    unsigned _frac1 : 24;
    unsigned rsv    : 4;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x02)
    unsigned ctrl   : 4;
    unsigned _mod2  : 14;
    unsigned _frac2 : 14;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x03)
    unsigned ctrl          : 4;
    unsigned phase_value   : 24;
    unsigned phase_adjust  : 1;
    unsigned phase_resync  : 1;
    unsigned sd_load_reset : 1;
    unsigned rsv           : 1;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x04)
    unsigned ctrl           : 4;
    unsigned counter_reset  : 1;
    unsigned cp_three_state : 1;
    unsigned pd             : 1;
    unsigned pd_polarity    : 1;
    unsigned mux_logic      : 1;
    unsigned ref_mode       : 1;
    unsigned current_set    : 4;
    unsigned double_buff    : 1;
    unsigned r_counter      : 10;
    unsigned rdiv2          : 1;
    unsigned ref_doubler    : 1;
    unsigned mux_out        : 3;
    unsigned rsv            : 2;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x05)
    unsigned rsv : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x06)
    unsigned ctrl           : 4;
    unsigned rf_output_pwr  : 2;
    unsigned rf_out_a       : 1;
    unsigned rsv0           : 3;
    unsigned rf_out_b       : 1;
    unsigned mtld           : 1;
    unsigned rsv1           : 1;
    unsigned charge_pump_bc : 8;
    unsigned rf_divider_sel : 3;
    unsigned feedback_sel   : 1;
    unsigned rsv2           : 4;
    unsigned negative_bleed : 1;
    unsigned gated_bleed    : 1;
    unsigned rsv3           : 1;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x07)
    unsigned ctrl                : 4;
    unsigned ld_mode             : 1;
    unsigned frac_n_ld_precision : 2;
    unsigned lol_mode            : 1;
    unsigned ld_cycle_count      : 2;
    unsigned rsv0                : 15;
    unsigned le_sync             : 1;
    unsigned rsv1                : 6;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x08)
    unsigned rsv : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x09)
    unsigned ctrl          : 4;
    unsigned synth_lock_to : 5;
    unsigned auto_level_to : 5;
    unsigned timeout       : 10;
    unsigned vco_band_div  : 8;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0a)
    unsigned ctrl           : 4;
    unsigned adc_enable     : 1;
    unsigned adc_conversion : 1;
    unsigned adc_clock_div  : 8;
    unsigned rsv            : 18;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0b)
    unsigned rsv : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0c)
    unsigned ctrl         : 4;
    unsigned rsv          : 12;
    unsigned resync_clock : 16;
REG_DEF_CPT_SUFFIX_32

} // ns_adf5355_reg

#endif // INCLUDE_REG_DEF_ADF5355_H
