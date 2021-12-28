#ifndef INCLUDE_REG_DEF_AD9122_H
#define INCLUDE_REG_DEF_AD9122_H

#include "reg_def.h"

#define AD9122_REG_DECL(addr,init) REG_DECL_INIT(ns_reg_ad9122,addr,init)
#define AD9122_REG_DATA(addr) REG_DATA(ns_reg_ad9122,addr)
#define AD9122_REG(addr) REG(ns_reg_ad9122,addr)

#define AD9122_REG_DECL_0x00 AD9122_REG_DECL(0x00,0x00);
#define AD9122_REG_DECL_0x01 AD9122_REG_DECL(0x01,0x10);
#define AD9122_REG_DECL_0x03 AD9122_REG_DECL(0x03,0x00);
#define AD9122_REG_DECL_0x04 AD9122_REG_DECL(0x04,0x00);
#define AD9122_REG_DECL_0x05 AD9122_REG_DECL(0x05,0x00);
#define AD9122_REG_DECL_0x08 AD9122_REG_DECL(0x08,0x3f);
#define AD9122_REG_DECL_0x0a AD9122_REG_DECL(0x0a,0x40);
#define AD9122_REG_DECL_0x0c AD9122_REG_DECL(0x0c,0xd1);
#define AD9122_REG_DECL_0x0d AD9122_REG_DECL(0x0d,0xd9);
#define AD9122_REG_DECL_0x10 AD9122_REG_DECL(0x10,0x48);
#define AD9122_REG_DECL_0x11 AD9122_REG_DECL(0x11,0x00);
#define AD9122_REG_DECL_0x16 AD9122_REG_DECL(0x16,0x00);
#define AD9122_REG_DECL_0x17 AD9122_REG_DECL(0x17,0x04);
#define AD9122_REG_DECL_0x1b AD9122_REG_DECL(0x1b,0xe4);
#define AD9122_REG_DECL_0x1c AD9122_REG_DECL(0x1c,0x00);
#define AD9122_REG_DECL_0x1d AD9122_REG_DECL(0x1d,0x00);
#define AD9122_REG_DECL_0x1e AD9122_REG_DECL(0x1e,0x00);
#define AD9122_REG_DECL_0x1f AD9122_REG_DECL(0x1f,0x08);
#define AD9122_REG_DECL_0x30 AD9122_REG_DECL(0x30,0x00);
#define AD9122_REG_DECL_0x31 AD9122_REG_DECL(0x31,0x00);
#define AD9122_REG_DECL_0x32 AD9122_REG_DECL(0x32,0x00);
#define AD9122_REG_DECL_0x33 AD9122_REG_DECL(0x33,0x00);
#define AD9122_REG_DECL_0x34 AD9122_REG_DECL(0x34,0x00);
#define AD9122_REG_DECL_0x35 AD9122_REG_DECL(0x35,0x00);
#define AD9122_REG_DECL_0x36 AD9122_REG_DECL(0x36,0x00);
#define AD9122_REG_DECL_0x38 AD9122_REG_DECL(0x38,0x00);
#define AD9122_REG_DECL_0x39 AD9122_REG_DECL(0x39,0x00);
#define AD9122_REG_DECL_0x3a AD9122_REG_DECL(0x3a,0x00);
#define AD9122_REG_DECL_0x3b AD9122_REG_DECL(0x3b,0x00);
#define AD9122_REG_DECL_0x3c AD9122_REG_DECL(0x3c,0x00);
#define AD9122_REG_DECL_0x3d AD9122_REG_DECL(0x3d,0x00);
#define AD9122_REG_DECL_0x3e AD9122_REG_DECL(0x3e,0x00);
#define AD9122_REG_DECL_0x3f AD9122_REG_DECL(0x3f,0x00);
#define AD9122_REG_DECL_0x40 AD9122_REG_DECL(0x40,0xf9);
#define AD9122_REG_DECL_0x41 AD9122_REG_DECL(0x41,0x01);
#define AD9122_REG_DECL_0x42 AD9122_REG_DECL(0x42,0x00);
#define AD9122_REG_DECL_0x43 AD9122_REG_DECL(0x43,0x00);
#define AD9122_REG_DECL_0x44 AD9122_REG_DECL(0x44,0xf9);
#define AD9122_REG_DECL_0x45 AD9122_REG_DECL(0x45,0x01);
#define AD9122_REG_DECL_0x46 AD9122_REG_DECL(0x46,0x00);
#define AD9122_REG_DECL_0x47 AD9122_REG_DECL(0x47,0x00);
#define AD9122_REG_DECL_0x48 AD9122_REG_DECL(0x48,0x02);
#define AD9122_REG_DECL_0x67 AD9122_REG_DECL(0x67,0x00);
#define AD9122_REG_DECL_0x68 AD9122_REG_DECL(0x68,0xb6);
#define AD9122_REG_DECL_0x69 AD9122_REG_DECL(0x69,0x7a);
#define AD9122_REG_DECL_0x6a AD9122_REG_DECL(0x6a,0x45);
#define AD9122_REG_DECL_0x6b AD9122_REG_DECL(0x6b,0xea);
#define AD9122_REG_DECL_0x6c AD9122_REG_DECL(0x6c,0x16);
#define AD9122_REG_DECL_0x6d AD9122_REG_DECL(0x6d,0x1a);
#define AD9122_REG_DECL_0x6e AD9122_REG_DECL(0x6e,0xc6);
#define AD9122_REG_DECL_0x6f AD9122_REG_DECL(0x6f,0xaa);
#define AD9122_REG_DECL_0x70 AD9122_REG_DECL(0x70,0x00);
#define AD9122_REG_DECL_0x71 AD9122_REG_DECL(0x71,0x00);
#define AD9122_REG_DECL_0x72 AD9122_REG_DECL(0x72,0x00);
#define AD9122_REG_DECL_0x73 AD9122_REG_DECL(0x73,0x00);

namespace ns_reg_ad9122 {

REG_DEF_CPT_PREFIX_8(0x00)
    unsigned rsv0 : 5;
    unsigned rst : 1;
    unsigned lsb_first : 1;
    unsigned sdio : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01)
    unsigned rsv0 : 4;
    unsigned pd_aux_adc : 1;
    unsigned pd_data_recv : 1;
    unsigned pd_q_dac : 1;
    unsigned pd_i_dac : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x03)
    unsigned data_bus_width : 2;
    unsigned rsv0 : 3;
    unsigned msb_swap : 1;
    unsigned q_data_first : 1;
    unsigned bin_data_fmt : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x04)
    unsigned en_fifo_warn_2 : 1;
    unsigned en_fifo_warn_1 : 1;
    unsigned rsv0 : 2;
    unsigned en_sync_signal_locked : 1;
    unsigned en_sync_signal_lost : 1;
    unsigned en_pll_locked : 1;
    unsigned en_pll_lock_lost : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x05)
    unsigned rsv0 : 2;
    unsigned en_sed_cmp_fail : 1;
    unsigned en_aed_cmp_fail : 1;
    unsigned en_aed_cmp_pass : 1;
    unsigned rsv1 : 3;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x06)
    unsigned fifo_warn_2 : 1;
    unsigned fifo_warn_1 : 1;
    unsigned rsv0 : 2;
    unsigned sync_signal_locked : 1;
    unsigned sync_signal_lost : 1;
    unsigned pll_locked : 1;
    unsigned pll_lock_lost : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x07)
    unsigned rsv0 : 2;
    unsigned sed_cmp_fail : 1;
    unsigned aed_cmp_fail : 1;
    unsigned aed_cmp_pass : 1;
    unsigned rsv1 : 3;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x08)
    unsigned rsv0 : 4;
    unsigned refclk_cross_corr : 1;
    unsigned dacclk_cross_corr : 1;
    unsigned refclk_duty_corr : 1;
    unsigned dacclk_duty_corr : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a)
    unsigned vco_band : 6;
    unsigned en_pll_manual : 1;
    unsigned en_pll : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0c)
    unsigned pll_cp_curr : 5;
    unsigned rsv0 : 1;
    unsigned pll_loop_bw : 2;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0d)
    unsigned n1 : 2;
    unsigned no : 2;
    unsigned en_pll_cross_ctrl : 1;
    unsigned rsv0 : 1;
    unsigned n2 : 2;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0e)
    unsigned vco_ctrl_vol : 4;
    unsigned rsv0 : 3;
    unsigned pll_locked : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0f)
    unsigned vco_band : 6;
    unsigned rsv0 : 2;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x10)
    unsigned sync_averaging : 3;
    unsigned rising_edge_sync : 1;
    unsigned rsv0 : 2;
    unsigned data_fifo_rate_toggle : 1;
    unsigned en_sync : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x11)
    unsigned sync_phase_req : 6;
    unsigned rsv0 : 2;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x12)
    unsigned rsv0 : 6;
    unsigned sync_locked : 1;
    unsigned sync_lost : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x13)
    unsigned sync_phase : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x15)
    unsigned lvds_data_level_l : 1;
    unsigned lvds_data_level_h : 1;
    unsigned lvds_dci_level_l : 1;
    unsigned lvds_dci_level_h : 1;
    unsigned lvds_frame_level_l : 1;
    unsigned lvds_frame_level_h : 1;
    unsigned rsv0 : 2;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x16)
    unsigned dci_delay : 2;
    unsigned rsv0 : 6;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x17)
    unsigned fifo_phase_offset : 3;
    unsigned rsv0 : 5;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x18)
    unsigned rsv0 : 1;
    unsigned fifo_soft_align_req : 1;
    unsigned fifo_soft_align_ack : 1;
    unsigned rsv1 : 3;
    unsigned fifo_warning_2 : 1;
    unsigned fifo_warning_1 : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x19)
    unsigned fifo_level : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x1b)
    unsigned send_i_to_q : 1;
    unsigned sel_sideband : 1;
    unsigned bypass_phase_comp_dc_offset : 1;
    unsigned nco_gain : 1;
    unsigned rsv0 : 1;
    unsigned bypass_nco : 1;
    unsigned bypass_sinc : 1;
    unsigned bypass_premod : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x1c)
    unsigned bypass_hb1 : 1;
    unsigned hb1 : 2;
    unsigned rsv0 : 5;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x1d)
    unsigned bypass_hb2 : 1;
    unsigned hb2 : 6;
    unsigned rsv0 : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x1e)
    unsigned bypass_hb3 : 1;
    unsigned hb3 : 6;
    unsigned rsv0 : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x1f)
    unsigned id : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x30)
    unsigned ftw : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x31)
    unsigned ftw : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x32)
    unsigned ftw : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x33)
    unsigned ftw : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x34)
    unsigned nco_phase_offset : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x35)
    unsigned nco_phase_offset : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x36)
    unsigned update_ftw_req : 1;
    unsigned update_ftw_ack : 1;
    unsigned rsv0 : 2;
    unsigned frame_ftw_req : 1;
    unsigned frame_ftw_ack : 1;
    unsigned rsv1 : 2;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x38)
    unsigned i_phase_adj : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x39)
    unsigned i_phase_adj : 1;
    unsigned rsv0 : 7;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x3a)
    unsigned q_phase_adj : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x3b)
    unsigned q_phase_adj : 1;
    unsigned rsv0 : 7;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x3c)
    unsigned i_dac_offset : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x3d)
    unsigned i_dac_offset : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x3e)
    unsigned q_dac_offset : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x3f)
    unsigned q_dac_offset : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x40)
    unsigned i_dac_fs_adj : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x41)
    unsigned i_dac_fs_adj : 2;
    unsigned rsv0 : 5;
    unsigned i_dac_sleep : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x42)
    unsigned i_aux_dac : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x43)
    unsigned i_aux_dac : 2;
    unsigned rsv0 : 3;
    unsigned i_aux_dac_sleep : 1;
    unsigned i_aux_dac_curr_direction : 1;
    unsigned i_aux_dac_sign : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x44)
    unsigned q_dac_fs_adj : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x45)
    unsigned q_dac_fs_adj : 2;
    unsigned rsv0 : 5;
    unsigned q_dac_sleep : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x46)
    unsigned q_aux_dac : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x47)
    unsigned q_aux_dac : 2;
    unsigned rsv0 : 3;
    unsigned q_aux_dac_sleep : 1;
    unsigned q_aux_dac_curr_direction : 1;
    unsigned q_aux_dac_sign : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x48)
    unsigned cap_value : 1;
    unsigned ref_curr : 3;
    unsigned fs_curr : 3;
    unsigned rsv0 : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x49)
    unsigned die_temp : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x4a)
    unsigned die_temp : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x67)
    unsigned cmp_pass : 1;
    unsigned cmp_fail : 1;
    unsigned rsv0 : 1;
    unsigned en_auto_clr : 1;
    unsigned rsv1 : 1;
    unsigned sample_err_det : 1;
    unsigned rsv2 : 1;
    unsigned en_sed_cmp : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x68)
    unsigned cmp_value_i_0 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x69)
    unsigned cmp_value_i_0 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x6a)
    unsigned cmp_value_q_0 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x6b)
    unsigned cmp_value_q_0 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x6c)
    unsigned cmp_value_i_1 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x6d)
    unsigned cmp_value_i_1 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x6e)
    unsigned cmp_value_q_1 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x6f)
    unsigned cmp_value_q_1 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x70)
    unsigned err_det_i : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x71)
    unsigned err_det_i : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x72)
    unsigned err_det_q : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x73)
    unsigned err_det_q : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x7f)
    unsigned rsv0 : 2;
    unsigned rev : 4;
    unsigned rsv1 : 2;
REG_DEF_CPT_SUFFIX_8

}

#endif // INCLUDE_REG_DEF_AD9122_H
