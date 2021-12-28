#ifndef INCLUDE_REG_DEF_HMC83X_H
#define INCLUDE_REG_DEF_HMC83X_H

#include "reg_def.h"

enum hmc83x_t {
    HMC832,
    HMC833
};

#define HMC83X_PLL_REG_DECL(addr,init) REG_DECL_INIT(ns_reg_hmc83x_pll,addr,init)
#define HMC83X_PLL_REG_DATA(addr) REG_DATA(ns_reg_hmc83x_pll,addr)
#define HMC83X_PLL_REG(addr) REG(ns_reg_hmc83x_pll,addr)

#define HMC83X_VCO_REG_DECL(addr,init) REG_DECL_INIT(ns_reg_hmc83x_vco,addr,init)
#define HMC83X_VCO_REG_DATA(addr) REG_DATA(ns_reg_hmc83x_vco,addr)
#define HMC83X_VCO_REG(addr) REG(ns_reg_hmc83x_vco,addr)
#define HMC832_VCO_REG(addr) REG_OF(ns_reg_hmc83x_vco,addr,832)
#define HMC833_VCO_REG(addr) REG_OF(ns_reg_hmc83x_vco,addr,833)

#define HMC83X_PLL_REG_DECL_0x00 HMC83X_PLL_REG_DECL(0x00,0x000000)
#define HMC83X_PLL_REG_DECL_0x01 HMC83X_PLL_REG_DECL(0x01,0x000002)
#define HMC83X_PLL_REG_DECL_0x02 HMC83X_PLL_REG_DECL(0x02,0x000002)
#define HMC83X_PLL_REG_DECL_0x03 HMC83X_PLL_REG_DECL(0x03,0x000024)
#define HMC83X_PLL_REG_DECL_0x04 HMC83X_PLL_REG_DECL(0x04,0x000000)
#define HMC83X_PLL_REG_DECL_0x05 HMC83X_PLL_REG_DECL(0x05,0x000000)
#define HMC83X_PLL_REG_DECL_0x06 HMC83X_PLL_REG_DECL(0x06,0x200b4a)
#define HMC83X_PLL_REG_DECL_0x07 HMC83X_PLL_REG_DECL(0x07,0x00010d)
#define HMC83X_PLL_REG_DECL_0x08 HMC83X_PLL_REG_DECL(0x08,0xc1beff)
#define HMC83X_PLL_REG_DECL_0x09 HMC83X_PLL_REG_DECL(0x09,0x30ed5a)
#define HMC83X_PLL_REG_DECL_0x0a HMC83X_PLL_REG_DECL(0x0a,0x002006)
#define HMC83X_PLL_REG_DECL_0x0b HMC83X_PLL_REG_DECL(0x0b,0x0f8061)
#define HMC83X_PLL_REG_DECL_0x0c HMC83X_PLL_REG_DECL(0x0c,0x000000)
#define HMC83X_PLL_REG_DECL_0x0f HMC83X_PLL_REG_DECL(0x0f,0x000081)
#define HMC83X_PLL_REG_DECL_0x10 HMC83X_PLL_REG_DECL(0x10,0x000020)
#define HMC83X_PLL_REG_DECL_0x11 HMC83X_PLL_REG_DECL(0x11,0x07ffff)
#define HMC83X_PLL_REG_DECL_0x12 HMC83X_PLL_REG_DECL(0x12,0x000000)
#define HMC83X_PLL_REG_DECL_0x13 HMC83X_PLL_REG_DECL(0x13,0x001259)

#define HMC83X_VCO_REG_DECL_0x00 HMC83X_VCO_REG_DECL(0x00,0x020)
#define HMC83X_VCO_REG_DECL_0x01 HMC83X_VCO_REG_DECL(0x01,0x1ff)
#define HMC83X_VCO_REG_DECL_0x02 HMC83X_VCO_REG_DECL(0x02,0x001)
#define HMC83X_VCO_REG_DECL_0x03 HMC83X_VCO_REG_DECL(0x03,0x093)
#define HMC83X_VCO_REG_DECL_0x04 HMC83X_VCO_REG_DECL(0x04,0x0c1)
#define HMC83X_VCO_REG_DECL_0x05 HMC83X_VCO_REG_DECL(0x05,0x0aa)
#define HMC83X_VCO_REG_DECL_0x06 HMC83X_VCO_REG_DECL(0x06,0x0ff)
#define HMC83X_VCO_REG_DECL_0x07 HMC83X_VCO_REG_DECL(0x07,0x09b)

#define HMC83X_ALL_REG_DECL   \
    HMC83X_PLL_REG_DECL_0x00; \
    HMC83X_PLL_REG_DECL_0x01; \
    HMC83X_PLL_REG_DECL_0x02; \
    HMC83X_PLL_REG_DECL_0x03; \
    HMC83X_PLL_REG_DECL_0x04; \
    HMC83X_PLL_REG_DECL_0x05; \
    HMC83X_PLL_REG_DECL_0x06; \
    HMC83X_PLL_REG_DECL_0x07; \
    HMC83X_PLL_REG_DECL_0x08; \
    HMC83X_PLL_REG_DECL_0x09; \
    HMC83X_PLL_REG_DECL_0x0a; \
    HMC83X_PLL_REG_DECL_0x0b; \
    HMC83X_PLL_REG_DECL_0x0c; \
    HMC83X_PLL_REG_DECL_0x0f; \
    HMC83X_PLL_REG_DECL_0x10; \
    HMC83X_PLL_REG_DECL_0x11; \
    HMC83X_PLL_REG_DECL_0x12; \
    HMC83X_PLL_REG_DECL_0x13; \
    HMC83X_VCO_REG_DECL_0x00; \
    HMC83X_VCO_REG_DECL_0x01; \
    HMC83X_VCO_REG_DECL_0x02; \
    HMC83X_VCO_REG_DECL_0x03; \
    HMC83X_VCO_REG_DECL_0x04; \
    HMC83X_VCO_REG_DECL_0x05; \
    HMC83X_VCO_REG_DECL_0x06; \
    HMC83X_VCO_REG_DECL_0x07

#define HMC832_REG_INIT

#define HMC833_REG_INIT \
    HMC83X_PLL_REG_DATA(0x09) = 0x547fff; \
    HMC83X_PLL_REG_DATA(0x0a) = 0x002046; \
    HMC83X_PLL_REG_DATA(0x0b) = 0x0fc021; \
    HMC83X_VCO_REG_DATA(0x01) = 0x001f; \
    HMC83X_VCO_REG_DATA(0x02) = 0x00c1; \
    HMC83X_VCO_REG_DATA(0x03) = 0x0051; \
    HMC83X_VCO_REG_DATA(0x05) = 0x002c;

// PLL subsystem,24 bits data
namespace ns_reg_hmc83x_pll {

REG_DEF_CPT_PREFIX_32(0x00)
    unsigned u32 : 32;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x01)
    unsigned rst_pin_sel : 1;
    unsigned rst_from_spi : 1;
field(833)
    unsigned keep_bias_on : 1;
    unsigned keep_pd_on : 1;
    unsigned keep_cp_on : 1;
    unsigned keep_ref_buff_on : 1;
    unsigned keep_vco_on : 1;
    unsigned keep_gpo_on : 1;
    unsigned rsv0 : 1;
    unsigned rsv1 : 1;

    unsigned rsv2 : 22;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x02)
    unsigned r_div : 14;
    unsigned rsv0 : 18;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x03)
    unsigned _int : 19;
    unsigned rsv0 : 13;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x04)
    unsigned _frac : 24;
    unsigned rsv0 : 8;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x05)
    unsigned vco_id : 3;
    unsigned vco_reg_addr : 4;
    unsigned vco_reg_data : 9;
    unsigned rsv0 : 16;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x06)
    unsigned seed : 2;
field(833)
    unsigned order : 2;
    unsigned rsv0 : 3;

    unsigned frac_bypass : 1;
field(833)
    unsigned auto_seed : 1;
    unsigned mod_clk_src : 1;
    unsigned sd_mod_clk : 1;

    unsigned en_sd : 1;
    unsigned rsv1 : 6;
field(833)
    unsigned en_bist : 1;
    unsigned rdiv_bist_cycle : 2;

    unsigned auto_clk : 1;
    unsigned rsv2 : 1;
    unsigned rsv3 : 9;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x07)
    unsigned ld_window : 3;
    unsigned en_inter_ld : 1;
    unsigned rsv0 : 2;
    unsigned ld_window_type : 1;
    unsigned ld_window_duration : 3;
    unsigned ld_timer_freq : 2;
field(833)
    unsigned ld_timer_test_mode : 1;

    unsigned auto_relock : 1;
    unsigned rsv1 : 18;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x08)
field(833)
    unsigned en_bias : 1;
    unsigned en_cp : 1;
    unsigned en_pd : 1;
    unsigned en_ref_buf : 1;
    unsigned en_vco_buf : 1;

    unsigned en_gpo_pad : 1;
field(833)
    unsigned rsv0 : 1;
    unsigned en_vco_div_clk_to_dig : 1;
    unsigned rsv1 : 1;
    unsigned en_prescaler_clk : 1;

    unsigned en_vco_buf_psb : 1;
field(833)
    unsigned en_cp_inter_opamp : 1;
    unsigned rsv2 : 3;
    unsigned rsv3 : 3;
    unsigned spare : 1;
    unsigned rsv4 : 2;

    unsigned high_ref : 1;
    unsigned rsv5 : 2;
    unsigned rsv6 : 8;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x09)
    unsigned cp_dn_gain : 7;
    unsigned cp_up_gain : 7;
    unsigned cp_offset : 7;
    unsigned en_offset_up : 1;
    unsigned en_offset_dn : 1;
field(833)
    unsigned high_cp : 1;

    unsigned rsv0 : 8;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0a)
    unsigned vtune_res : 3;
field(833)
    unsigned vco_core_adj : 3;
    unsigned wait_state_setup : 2;
    unsigned sar_bits : 2;

    unsigned force_curve : 1;
    unsigned dis_auto_cal : 1;
    unsigned vspi_trig : 1;
    unsigned fsm_vspi_clk : 2;
field(833)
    unsigned xtal_edge_fsm : 1;
    unsigned force_rdiv_bypass : 1;

    unsigned rsv0 : 15;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0b)
    unsigned pd_del_sel : 3;
field(833)
    unsigned short_pd_in : 1;
    unsigned pd_phase : 1;

    unsigned en_pd_up : 1;
    unsigned en_pd_dn : 1;
    unsigned csp_mode : 2;
    unsigned force_cp_up : 1;
    unsigned force_cp_dn : 1;
field(833)
    unsigned force_cp_mid_rail : 1;
    unsigned rsv0 : 3;
    unsigned cp_inter_opamp_bias : 2;
    unsigned mcnter_clk_gate : 2;
    unsigned spare : 1;
    unsigned rsv1 : 4;

    unsigned rsv2 : 8;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0c)
    unsigned chs_per_pd : 14;
    unsigned rsv0 : 18;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x0f)
    unsigned gpo_sel : 5;
    unsigned gpo_test : 1;
    unsigned sdo_automux : 1;
    unsigned ldo_on : 1;
    unsigned dis_pfet : 1;
    unsigned dis_nfet : 1;
    unsigned rsv0 : 22;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x10)
    unsigned vco_sw : 8;
    unsigned auto_cal_busy : 1;
    unsigned rsv0 : 23;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x11)
    unsigned sar_err_cnt : 19;
    unsigned sar_err_sign : 1;
    unsigned rsv0 : 12;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x12)
    unsigned gpo : 1;
    unsigned ld : 1;
    unsigned rsv0 : 30;
REG_DEF_CPT_SUFFIX_32

REG_DEF_CPT_PREFIX_32(0x13)
    unsigned rsv0 : 17;
    unsigned rsv1 : 15;
REG_DEF_CPT_SUFFIX_32

} // ns_reg_hmc83x_pll

// VCO subsystem,9 bits data
namespace ns_reg_hmc83x_vco {

REG_DEF_CPT_PREFIX_16(0x00)
    unsigned cal : 1;
    unsigned caps : 8;
    unsigned rsv0 : 7;
REG_DEF_CPT_SUFFIX_16

REG_DEF_CPT_PREFIX_16(0x01)
    unsigned en_vco_subsys : 1;
    unsigned en_vco : 1;
    unsigned en_pll_buf : 1;
    unsigned en_output : 1;
    unsigned rsv0 : 1;
    unsigned en_output_stage : 1;
    unsigned rsv1 : 2;
    unsigned rsv2 : 1;
    unsigned rsv3 : 7;
REG_DEF_CPT_SUFFIX_16

REG_DEF_CPT_PREFIX_16(0x02)
    unsigned div : 6;
field(833)
    unsigned rf_gain : 2;
    unsigned div_stage_gain : 1;

    unsigned rsv0 : 7;
REG_DEF_CPT_SUFFIX_16

REG_DEF_PREFIX_16(0x03)
REG_1_PREFIX(832)
    unsigned perf_mode : 2;
    unsigned en_rf_n : 1;
    unsigned en_rf_p : 1;
    unsigned rsv0 : 1;
    unsigned return_loss : 1;
    unsigned rsv1 : 1;
    unsigned mute_mode : 2;
    unsigned rsv2 : 7;
REG_1_SUFFIX(832)
REG_2_PREFIX(833)
    unsigned out_doubler : 1;
    unsigned rsv0 : 1;
    unsigned rfo_mode : 1;
    unsigned rf_buff_bias : 2;
    unsigned spare : 4;
    unsigned rsv1 : 7;
REG_2_SUFFIX(833)
REG_DEF_SUFFIX_16

REG_DEF_CPT_PREFIX_16(0x04)
field(833)
    unsigned vco_bias : 3;
    unsigned pll_buff_bias : 2;
    unsigned fndlmtr_bias : 2;
    unsigned preset_cal : 2;
    unsigned rsv0 : 7;

    unsigned rsv1 : 7;
REG_DEF_CPT_SUFFIX_16

REG_DEF_CPT_PREFIX_16(0x05)
field(833)
    unsigned cf_l : 2;
    unsigned cf_ml : 2;
    unsigned cf_mh : 2;
    unsigned cf_h : 2;
    unsigned spare : 1;
    unsigned rsv0 : 7;
REG_DEF_CPT_SUFFIX_16

REG_DEF_CPT_PREFIX_16(0x06)
field(833)
    unsigned msb_l : 2;
    unsigned msb_ml : 2;
    unsigned msb_mh : 2;
    unsigned msb_h : 2;
    unsigned spare : 1;
    unsigned rsv0 : 7;
REG_DEF_CPT_SUFFIX_16

REG_DEF_CPT_PREFIX_16(0x07)
field(833)
    unsigned gain_ctrl : 4;
    unsigned init : 1;
    unsigned rsv0 : 4;
    unsigned rsv1 : 7;
REG_DEF_CPT_SUFFIX_16

} // ns_reg_hmc83x_vco

#endif // INCLUDE_REG_DEF_HMC83X_H
