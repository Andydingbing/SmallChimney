#ifndef INCLUDE_REG_DEF_AD908X_H
#define INCLUDE_REG_DEF_AD908X_H

#include "reg_def.h"

#define AD908X_REG_DECL(addr,init) REG_DECL_INIT(ns_reg_ad908x,addr,init)
#define AD908X_REG_DATA(addr) REG_DATA(ns_reg_ad908x,addr)
#define AD908X_REG(addr) REG(ns_reg_ad908x,addr)
#define AD908X_REG_W(addr) INT_CHECK(set_reg(addr,AD908X_REG_DATA(addr)))

namespace ns_reg_ad908x {

/*
 * cddc : coarse digital down conversion
 * pfilt : programmable FIR filter
 * piw : phase_inc_word
 *
 */

REG_DEF_CPT_PREFIX_8(0x0000)
    unsigned soft_reset : 1;
    unsigned lsb_first : 1;
    unsigned addr_inc : 1;
    unsigned sdo_active : 1;
    unsigned sdo_active_m : 1;
    unsigned addr_inc_m : 1;
    unsigned lsb_first_m : 1;
    unsigned soft_reset_m : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0001)
    unsigned rsv0 : 7;
    unsigned single_ins : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0002)
    unsigned rsv0 : 2;
    unsigned cust_op_mode : 2;
    unsigned rsv1 : 4;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0003)
    unsigned chip_type : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0006)
    unsigned chip_grade : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x000b)
    unsigned spi_revision : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x000c)
    unsigned vendor_id_lsb : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x000d)
    unsigned vendor_id_msb : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0010)
    unsigned chip_id_l : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0011)
    unsigned chip_id_m1 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0012)
    unsigned chip_id_m2 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0013)
    unsigned chip_id_h : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0018)
    unsigned adc_page : 4;
    unsigned cddc_page : 4;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0019)
    unsigned fine_ddc0_page : 1;
    unsigned fine_ddc1_page : 1;
    unsigned fine_ddc2_page : 1;
    unsigned fine_ddc3_page : 1;
    unsigned fine_ddc4_page : 1;
    unsigned fine_ddc5_page : 1;
    unsigned fine_ddc6_page : 1;
    unsigned fine_ddc7_page : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x001a)
    unsigned jtx_link0_page : 1;
    unsigned jtx_link1_page : 1;
    unsigned rsv0 : 6;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x001b)
    unsigned dac_page_msk : 4;
    unsigned rsv0 : 4;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x001c)
    unsigned dac_chan_msk : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x001d)
    unsigned jesd204c_rcv_link_msk : 2;
    unsigned mods_msk : 2;
    unsigned rsv0 : 4;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x001e)
    unsigned pfilt_adc_pair0_page : 1;
    unsigned pfilt_adc_pair1_page : 1;
    unsigned rsv0 : 6;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x001f)
    unsigned pfilt_coef_page0 : 1;
    unsigned pfilt_coef_page1 : 1;
    unsigned pfilt_coef_page2 : 1;
    unsigned pfilt_coef_page3 : 1;
    unsigned rsv0 : 4;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0020)
    unsigned rsv0 : 2;
    unsigned en_sysref_irq : 1;
    unsigned rsv1 : 3;
    unsigned en_data_ready : 1;
    unsigned rsv2 : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0023)
    unsigned en_pll_lock_fast_irq : 1;
    unsigned en_pll_lock_slow_irq : 1;
    unsigned en_pll_lost_fast_irq : 1;
    unsigned en_pll_lost_slow_irq : 1;
    unsigned en_dll_lock01 : 1;
    unsigned en_dll_lost01 : 1;
    unsigned en_dll_lock23 : 1;
    unsigned en_dll_lost23 : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0026)
    unsigned rsv0 : 2;
    unsigned irq_sysref_jitter : 1;
    unsigned rsv1 : 3;
    unsigned irq_data_ready : 1;
    unsigned rsv2 : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0027)
    unsigned rsv0 : 3;
    unsigned irq_pa_err0 : 1;
    unsigned rsv1 : 3;
    unsigned irq_pa_err1 : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0028)
    unsigned rsv0 : 3;
    unsigned irq_pa_err2 : 1;
    unsigned rsv1 : 3;
    unsigned irq_pa_err3 : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0029)
    unsigned irq_pll_lock_fast : 1;
    unsigned irq_pll_lock_slow : 1;
    unsigned rsv0 : 6;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x002c)
    unsigned rsv0 : 2;
    unsigned mux_sysref_jitter : 1;
    unsigned rsv1 : 2;
    unsigned mux_lane_fifo : 1;
    unsigned mux_data_ready : 1;
    unsigned mux_jesd_irq : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x002d)
    unsigned rsv0 : 3;
    unsigned mux_pa_err0 : 1;
    unsigned rsv1 : 3;
    unsigned mux_pa_err1 : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x002e)
    unsigned rsv0 : 3;
    unsigned mux_pa_err2 : 1;
    unsigned rsv1 : 3;
    unsigned mux_pa_err3 : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x002f)
    unsigned mux_pll_lock_fast : 1;
    unsigned mux_pll_lock_slow : 1;
    unsigned rsv0 : 6;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0032)
    unsigned irq_status_all : 1;
    unsigned rsv0 : 7;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0038)
    unsigned sync1_outbp_cfg : 4;
    unsigned sync1_outbn_cfg : 4;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0061)
    unsigned en_dll : 1;
    unsigned rsv1 : 7;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0063)
    unsigned dll_locked : 1;
    unsigned dll_lost : 1;
    unsigned rsv0 : 6;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0090)
    unsigned dac_pd0 : 1;
    unsigned dac_pd1 : 1;
    unsigned dac_pd2 : 1;
    unsigned dac_pd3 : 1;
    unsigned rsv0 : 4;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0091)
    unsigned aclk_pd : 1;
    unsigned rsv0 : 7;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0093)
    unsigned pll_div : 2;
    unsigned rsv0 : 6;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0094)
    unsigned rsv0 : 7;
    unsigned bypass_pll : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x00b4)
    unsigned sysref_cnt : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x00b5)
    unsigned sysref_phase : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x00b6)
    unsigned sysref_phase : 5;
    unsigned rsv0 : 3;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x00b8)
    unsigned sysref_mode_continuous : 1;
    unsigned sysref_mode_oneshot : 1;
    unsigned rsv0 : 2;
    unsigned oneshot_sync_done : 1;
    unsigned rsv1 : 3;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x00b9)
    unsigned rotation_mode : 2;
    unsigned rsv0 : 2;
    unsigned en_nco_rst_after_rot : 1;
    unsigned rsv1 : 3;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x00ba)
    unsigned sysref_average : 3;
    unsigned rsv0 : 5;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x00bc)
    unsigned nco_sync_ms_trig : 1;
    unsigned rsv0 : 7;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x00bf)
    unsigned oneshot_sync_hw : 1;
    unsigned rsv0 : 7;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x00c0)
    unsigned rsv0 : 1;
    unsigned en_tx_clk : 1;
    unsigned rsv1 : 2;
    unsigned direct_loopback_mode : 2;
    unsigned rsv2 : 2;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x00c2)
    unsigned loopback_cb_ctrl : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x00ca)
    unsigned sds_pll_refclk_div_spi : 6;
    unsigned rsv0 : 1;
    unsigned en_sds_pll_refclk_div_spi : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x00cb)
    unsigned nco_sync_sysref_mode : 2;
    unsigned nco_sync_sysref_mode_rx : 2;
    unsigned rsv0 : 4;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x00cc)
    unsigned nco_sync_ms_mode : 2;
    unsigned nco_sync_ms_trig_src : 2;
    unsigned rsv0 : 4;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x00d0)
    unsigned en_spi_d2a0 : 1;
    unsigned en_spi_d2a1 : 1;
    unsigned en_spi_d2a_center : 1;
    unsigned en_spi_ana_center : 1;
    unsigned en_spi_dac_ana : 1;
    unsigned rsv0 : 3;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x00d1)
    unsigned en_spi_reg8_adc0 : 1;
    unsigned en_spi_reg8_adc1 : 1;
    unsigned rsv0 : 2;
    unsigned en_spi_reg32_adc0 : 1;
    unsigned en_spi_reg32_adc1 : 1;
    unsigned rsv1 : 2;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x00e2)
    unsigned d_cal_reset : 1;
    unsigned rsv0 : 7;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x00e3)
    unsigned d_refin_div : 2;
    unsigned rsv0 : 6;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x00e4)
    unsigned d_cp_current : 6;
    unsigned rsv0 : 2;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x00e9)
    unsigned d_div_ctrl : 6;
    unsigned rsv0 : 2;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x00ec)
    unsigned rsv0 : 1;
    unsigned d_pll_lock_ctrl : 2;
    unsigned rsv1 : 1;
    unsigned d_ctrl_hs_fb_div : 2;
    unsigned rsv2 : 2;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x00f7)
    unsigned d_cp_bleed : 6;
    unsigned rsv0 : 2;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0140)
    unsigned rsv0 : 4;
    unsigned msb_mode : 2;
    unsigned rsv1 : 2;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0143)
    unsigned rsv0 : 5;
    unsigned en_msb_rotation : 1;
    unsigned rsv1 : 2;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0180)
    unsigned adc_clk_div_spi : 2;
    unsigned rsv0 : 2;
    unsigned adc_clk_div_pd : 1;
    unsigned rsv1 : 3;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0196)
    unsigned adc_driver_data_ctrl : 5;
    unsigned rsv0 : 3;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0198)
    unsigned rsv0 : 1;
    unsigned sel_adc_clk_driver : 1;
    unsigned rsv1 : 6;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x019a)
    unsigned sysref_pd : 1;
    unsigned rsv0 : 6;
    unsigned sysref_bias_pd : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01a0)
    unsigned en_test_tone : 1;
    unsigned ddsc_sel_sideband : 1;
    unsigned en_ddsc_modulus : 1;
    unsigned rsv0 : 3;
    unsigned en_ddsc_nco : 1;
    unsigned rsv1 : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01a1)
    unsigned ddsc_ftw_load_req : 1;
    unsigned rsv0 : 1;
    unsigned ddsc_ftw_load_sysref : 1;
    unsigned rsv1 : 5;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01a2)
    unsigned ddsc_ftw : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01a3)
    unsigned ddsc_ftw : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01a4)
    unsigned ddsc_ftw : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01a5)
    unsigned ddsc_ftw : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01a6)
    unsigned ddsc_ftw : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01a7)
    unsigned ddsc_ftw : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01a8)
    unsigned ddsc_nco_phase_offset : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01a9)
    unsigned ddsc_nco_phase_offset : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01aa)
    unsigned ddsc_acc_modulus : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01ab)
    unsigned ddsc_acc_modulus : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01ac)
    unsigned ddsc_acc_modulus : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01ad)
    unsigned ddsc_acc_modulus : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01ae)
    unsigned ddsc_acc_modulus : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01af)
    unsigned ddsc_acc_modulus : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01b0)
    unsigned ddsc_acc_delta : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01b1)
    unsigned ddsc_acc_delta : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01b2)
    unsigned ddsc_acc_delta : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01b3)
    unsigned ddsc_acc_delta : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01b4)
    unsigned ddsc_acc_delta : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01b5)
    unsigned ddsc_acc_delta : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01b6)
    unsigned dc_offset : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01b7)
    unsigned dc_offset : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01b8)
    unsigned gain : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01b9)
    unsigned gain : 4;
    unsigned rsv0 : 4;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01ba)
    unsigned en_ch : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01c6)
    unsigned skew_adj : 4;
    unsigned rsv0 : 4;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01c7)
    unsigned nco_sync_ch : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01c8)
    unsigned en_main_dp_dac : 4;
    unsigned rsv0 : 3;
    unsigned en_main_dp_dac_spi : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01c9)
    unsigned rsv0 : 1;
    unsigned ddsm_sel_sideband : 1;
    unsigned en_ddsm_modulus : 1;
    unsigned en_ddsm_nco : 1;
    unsigned ddsm_mode : 2;
    unsigned en_cmplx_mod : 1;
    unsigned rsv1 : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01ca)
    unsigned ddsm_ftw_load_req : 1;
    unsigned rsv0 : 1;
    unsigned ddsm_ftw_load_sysref : 1;
    unsigned rsv1 : 5;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01cb)
    unsigned ddsm_ftw : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01cc)
    unsigned ddsm_ftw : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01cd)
    unsigned ddsm_ftw : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01ce)
    unsigned ddsm_ftw : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01cf)
    unsigned ddsm_ftw : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01d0)
    unsigned ddsm_ftw : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01d1)
    unsigned ddsm_nco_phase_offset : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01d2)
    unsigned ddsm_nco_phase_offset : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01d3)
    unsigned ddsm_acc_modulus : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01d4)
    unsigned ddsm_acc_modulus : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01d5)
    unsigned ddsm_acc_modulus : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01d6)
    unsigned ddsm_acc_modulus : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01d7)
    unsigned ddsm_acc_modulus : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01d8)
    unsigned ddsm_acc_modulus : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01d9)
    unsigned ddsm_acc_delta : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01da)
    unsigned ddsm_acc_delta : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01db)
    unsigned ddsm_acc_delta : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01dc)
    unsigned ddsm_acc_delta : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01dd)
    unsigned ddsm_acc_delta : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01de)
    unsigned ddsm_acc_delta : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01E5)
    unsigned ddsm_cal_ftw : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01e6)
    unsigned ddsm_cal_ftw : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01e7)
    unsigned ddsm_cal_ftw : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01e8)
    unsigned ddsm_cal_ftw : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01e9)
    unsigned en_ddsm_cal_freq_tune : 1;
    unsigned en_ddsm_cal_dc_input : 1;
    unsigned en_ddsm_cal_acc : 1;
    unsigned rsv0 : 5;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01ef)
    unsigned rsv0 : 4;
    unsigned en_nco_1x1x : 4;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01f0)
    unsigned rsv0 : 4;
    unsigned en_dac_maindp : 4;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01f1)
    unsigned txen_route_ctrl : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01f6)
    unsigned nco_sync_main_sel : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01fb)
    unsigned start_nco_sync : 1;
    unsigned rsv0 : 7;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01fe)
    unsigned rsv0 : 6;
    unsigned com_sync : 1;
    unsigned mode_not_in_table : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x01ff)
    unsigned fine_interp_mode : 4;
    unsigned coarse_interp_mode : 4;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0200)
    unsigned ch_interp_mode_dec : 3;
    unsigned dp_interp_mode_dec : 3;
    unsigned rsv0 : 2;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0203)
    unsigned en_spi_main_nco_rst : 4;
    unsigned rsv0 : 4;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0204)
    unsigned en_spi_ch_nco_rst : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0280)
    unsigned adc_coarse_cb : 4;
    unsigned c_mxr_iq_sfl : 4;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0281)
    unsigned coarse_fine_cb1 : 1;
    unsigned coarse_fine_cb2 : 1;
    unsigned coarse_fine_cb3 : 1;
    unsigned coarse_fine_cb4 : 1;
    unsigned coarse_fine_cb5 : 1;
    unsigned coarse_fine_cb6 : 1;
    unsigned coarse_fine_cb7 : 1;
    unsigned coarse_fine_cb8 : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0282)
    unsigned coarse_dec_sel : 4;
    unsigned coarse_c2r_en : 1;
    unsigned coarse_gain : 1;
    unsigned coarse_mxr_if : 2;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0283)
    unsigned fine_dec_sel : 3;
    unsigned rsv0 : 1;
    unsigned fine_c2r_en : 1;
    unsigned fine_gain : 1;
    unsigned fine_mxr_if : 2;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0284)
    unsigned ddc_overall_dec : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0285)
    unsigned en_coarse_ddc : 4;
    unsigned rsv0 : 4;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0286)
    unsigned en_fine_ddc1 : 1;
    unsigned en_fine_ddc2 : 1;
    unsigned en_fine_ddc3 : 1;
    unsigned en_fine_ddc4 : 1;
    unsigned en_fine_ddc5 : 1;
    unsigned en_fine_ddc6 : 1;
    unsigned en_fine_ddc7 : 1;
    unsigned en_fine_ddc8 : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0287)
    unsigned bypass_fine1 : 1;
    unsigned bypass_fine2 : 1;
    unsigned bypass_fine3 : 1;
    unsigned bypass_fine4 : 1;
    unsigned bypass_fine5 : 1;
    unsigned bypass_fine6 : 1;
    unsigned bypass_fine7 : 1;
    unsigned bypass_fine8 : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0289)
    unsigned chip_dec_ratio : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x028a)
    unsigned en_common_hop : 1;
    unsigned rsv0 : 7;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02a1)
    unsigned dformat_ctrl_bit0_sel : 4;
    unsigned dformat_ctrl_bit1_sel : 4;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02a2)
    unsigned dformat_ctrl_bit2_sel : 4;
    unsigned rsv0 : 4;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02a3)
    unsigned dformat_sel : 2;
    unsigned dformat_inv : 1;
    unsigned rsv0 : 5;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02a4)
    unsigned dformat_ovr_clr : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02a5)
    unsigned dformat_ovr_clr : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02a6)
    unsigned dformat_ovr_status : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02a7)
    unsigned dformat_ovr_status : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02a8)
    unsigned dformat_res : 4;
    unsigned en_dformat_fbw_dither : 1;
    unsigned en_dformat_ddc_dither : 1;
    unsigned rsv0 : 2;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02a9)
    unsigned dformat_fd_sel : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02aa)
    unsigned dformat_fd_sel : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02ab)
    unsigned dformat_fbw_sel : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02ac)
    unsigned dformat_fbw_sel : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02ad)
    unsigned dformat_tmode_sel : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02ae)
    unsigned dformat_tmode_sel : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02b0)
    unsigned tmode_i_pn_sel : 4;
    unsigned tmode_i_type_sel : 4;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02b1)
    unsigned rsv0 : 6;
    unsigned tmode_i_pn_force_rst : 1;
    unsigned tmode_i_flush : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02b2)
    unsigned tmode_i_usr_pat0 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02b3)
    unsigned tmode_i_usr_pat0 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02b4)
    unsigned tmode_i_usr_pat1 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02b5)
    unsigned tmode_i_usr_pat1 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02b6)
    unsigned tmode_i_usr_pat2 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02b7)
    unsigned tmode_i_usr_pat2 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02b8)
    unsigned tmode_i_usr_pat3 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02b9)
    unsigned tmode_i_usr_pat3 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02ba)
    unsigned rsv0 : 3;
    unsigned nco_rst_all_sysref : 1;
    unsigned allow_sysref_mask : 1;
    unsigned rsv1 : 3;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02bd)
    unsigned en_gpio_trig : 1;
    unsigned rsv0 : 1;
    unsigned loopback_master_trig : 1;
    unsigned rsv1 : 1;
    unsigned en_gpio_trig_sync : 1;
    unsigned gpio_trig_deglitch : 1;
    unsigned rsv2 : 2;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02c6)
    unsigned rxen0_fddc_sel : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02c7)
    unsigned rxen0_cddc_sel : 4;
    unsigned rxen0_jtxl_sel : 2;
    unsigned rxen0_adc_sel : 2;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02c8)
    unsigned rxen0_jtx_phy_sel : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02c9)
    unsigned rxen1_fddc_sel : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02ca)
    unsigned rxen1_cddc_sel : 4;
    unsigned rxen1_jtxl_sel : 2;
    unsigned rxen1_adc_sel : 2;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02cb)
    unsigned rxen1_jtx_phy_sel : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02cc)
    unsigned fine_ddc_i_status_sel : 2;
    unsigned fine_ddc_q_status_sel : 2;
    unsigned rsv0 : 4;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02cd)
    unsigned fd_eq_i_status_sel : 2;
    unsigned fd_eq_q_status_sel : 2;
    unsigned rsv0 : 4;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02ce)
    unsigned rxengp0_fddc_sel : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02cf)
    unsigned rxengp0_cddc_sel : 4;
    unsigned rxengp0_jtxl_sel : 2;
    unsigned rxengp0_adc_sel : 2;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02d0)
    unsigned rxengp0_jtx_phy_sel : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02d1)
    unsigned rxengp1_fddc_sel : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02d2)
    unsigned rxengp1_cddc_sel : 4;
    unsigned rxengp1_jtxl_sel : 2;
    unsigned rxengp1_adc_sel : 2;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02d3)
    unsigned rxengp1_jtx_phy_sel : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02d4)
    unsigned tmode_q_pn_sel : 4;
    unsigned tmode_q_type_sel : 4;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02d5)
    unsigned rsv0 : 6;
    unsigned tmode_q_pn_force_rst : 1;
    unsigned tmode_q_flush : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02d6)
    unsigned tmode_q_usr_pat0 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02d7)
    unsigned tmode_q_usr_pat0 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02d8)
    unsigned tmode_q_usr_pat1 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02d9)
    unsigned tmode_q_usr_pat1 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02da)
    unsigned tmode_q_usr_pat2 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02db)
    unsigned tmode_q_usr_pat2 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02dc)
    unsigned tmode_q_usr_pat3 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02dd)
    unsigned tmode_q_usr_pat3 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02e9)
    unsigned en_coarse_ddc1 : 1;
    unsigned en_coarse_ddc2 : 1;
    unsigned en_coarse_ddc3 : 1;
    unsigned en_coarse_ddc4 : 1;
    unsigned rsv0 : 4;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02ea)
    unsigned tmode_i_usr_pat4 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02eb)
    unsigned tmode_i_usr_pat4 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02ec)
    unsigned tmode_i_usr_pat5 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02ed)
    unsigned tmode_i_usr_pat5 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02ee)
    unsigned tmode_i_usr_pat6 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02ef)
    unsigned tmode_i_usr_pat6 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02f0)
    unsigned tmode_i_usr_pat7 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02f1)
    unsigned tmode_i_usr_pat7 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02f2)
    unsigned tmode_q_usr_pat4 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02f3)
    unsigned tmode_q_usr_pat4 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02f4)
    unsigned tmode_q_usr_pat5 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02f5)
    unsigned tmode_q_usr_pat5 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02f6)
    unsigned tmode_q_usr_pat6 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02f7)
    unsigned tmode_q_usr_pat6 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02f8)
    unsigned tmode_q_usr_pat7 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02f9)
    unsigned tmode_q_usr_pat7 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02fa)
    unsigned rxen0_use_txen : 1;
    unsigned rxen1_use_txen : 1;
    unsigned rxen0_pol : 1;
    unsigned rxen1_pol : 1;
    unsigned rxengp0_pol : 1;
    unsigned rxengp1_pol : 1;
    unsigned rsv0 : 2;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02fb)
    unsigned en_rxen0_spi : 1;
    unsigned en_rxen1_spi : 1;
    unsigned en_rxengp0_spi : 1;
    unsigned en_rxengp1_spi : 1;
    unsigned rxen0_spi : 1;
    unsigned rxen1_spi : 1;
    unsigned rxengp0_spi : 1;
    unsigned rxengp1_spi : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02fc)
    unsigned rxen0_0s_ctrl : 1;
    unsigned rxengp0_0s_ctrl : 1;
    unsigned rxen1_1s_ctrl : 1;
    unsigned rxengp1_1s_ctrl : 1;
    unsigned rxen0_0f_ctrl : 1;
    unsigned rxengp0_0f_ctrl : 1;
    unsigned rxen1_1f_ctrl : 1;
    unsigned rxengp1_1f_ctrl : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x02fd)
    unsigned rxen0_2s_ctrl : 1;
    unsigned rxengp0_2s_ctrl : 1;
    unsigned rxen1_3s_ctrl : 1;
    unsigned rxengp1_3s_ctrl : 1;
    unsigned rxen0_2f_ctrl : 1;
    unsigned rxengp0_2f_ctrl : 1;
    unsigned rxen1_3f_ctrl : 1;
    unsigned rxengp1_3f_ctrl : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0300)
    unsigned rsv0 : 7;
    unsigned en_soft_off_gain : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0301)
    unsigned en_spi_soft_off : 1;
    unsigned en_tx_soft_off : 1;
    unsigned en_rotate_soft_off : 1;
    unsigned en_jesd_err_soft_off : 1;
    unsigned rsv0 : 2;
    unsigned en_long_pa_err_soft_off : 1;
    unsigned en_short_pa_err_soft_off : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0302)
    unsigned en_dll_unlock_soft_off : 1;
    unsigned en_204c_crc_err_soft_off : 1;
    unsigned rsv0 : 2;
    unsigned en_slew_rate_err_soft_off : 1;
    unsigned rsv1 : 3;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0303)
    unsigned rsv0 : 6;
    unsigned en_long_level_soft_on : 1;
    unsigned en_spi_soft_on : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0304)
    unsigned en_all_short_pa : 1;
    unsigned en_all_long_pa : 1;
    unsigned en_all_soft_off_gain : 1;
    unsigned rsv0 : 5;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0305)
    unsigned long_pa_threshold : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0306)
    unsigned long_pa_threshold : 5;
    unsigned rsv0 : 3;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0307)
    unsigned long_pa_avg_time : 4;
    unsigned rsv0 : 3;
    unsigned en_long_pa : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0308)
    unsigned long_pa_pwr : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0309)
    unsigned long_pa_pwr : 5;
    unsigned rsv0 : 3;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x030a)
    unsigned short_pa_threshold : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x030b)
    unsigned short_pa_threshold : 5;
    unsigned rsv0 : 3;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x030c)
    unsigned short_pa_avg_time : 2;
    unsigned rsv0 : 5;
    unsigned en_short_pa : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x030d)
    unsigned short_pa_pwr : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x030e)
    unsigned short_pa_pwr : 5;
    unsigned rsv0 : 3;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x030f)
    unsigned en_txen_sm : 1;
    unsigned rsv0 : 7;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x031f)
    unsigned spi_pa_ctrl : 1;
    unsigned en_pa_prot_pin_txen_sm : 1;
    unsigned en_pa_prot_pin_bl_sm : 1;
    unsigned en_pa_prot_pin_jesd_err : 1;
    unsigned en_pa_prot_pin_jesd204c_rcv_204c_crc_err : 1;
    unsigned en_long_pa_prot_pin_avg_pwr : 1;
    unsigned en_short_pa_prot_pin_avg_pwr : 1;
    unsigned rsv0 : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0320)
    unsigned rsv0 : 2;
    unsigned en_pa_prot_pin_sr_err : 1;
    unsigned rsv1 : 5;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0321)
    unsigned rsv0 : 2;
    unsigned en_spi_txen : 1;
    unsigned spi_txen : 1;
    unsigned rsv1 : 4;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0322)
    unsigned en_spi_flush : 1;
    unsigned rsv0 : 7;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0342)
    unsigned smon_status_fcnt : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0343)
    unsigned smon_period : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0344)
    unsigned smon_period : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0345)
    unsigned smon_period : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0346)
    unsigned smon_period : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0347)
    unsigned smon_status_update : 1;
    unsigned smon_status_rdsel : 3;
    unsigned rsv0 : 4;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0348)
    unsigned en_smon_sframer : 1;
    unsigned smon_sframer_mode : 1;
    unsigned smon_sframer_insel : 6;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0349)
    unsigned en_smon_sync : 1;
    unsigned smon_sync_next : 1;
    unsigned rsv0 : 6;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x034a)
    unsigned smon_status : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x034b)
    unsigned smon_status : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x034c)
    unsigned smon_status : 4;
    unsigned rsv0 : 4;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x034d)
    unsigned smon_threshold_low : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x034e)
    unsigned smon_threshold_low : 3;
    unsigned rsv0 : 5;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x034f)
    unsigned smon_threshold_high : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0350)
    unsigned smon_threshold_high : 3;
    unsigned rsv0 : 5;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0380)
    unsigned rx_dig_rst : 1;
    unsigned rsv0 : 3;
    unsigned lcpll_reg_rst : 1;
    unsigned rsv1 : 3;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0381)
    unsigned des_reg_rst : 1;
    unsigned rsv0 : 7;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0429)
    unsigned sel_synca_mode_rc : 1;
    unsigned rsv0 : 7;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x042a)
    unsigned sel_syncb_mode_rc : 1;
    unsigned rsv0 : 7;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x05bb)
    unsigned en_jesd204c_rcv_204c_crc_irq : 1;
    unsigned en_jesd204c_rcv_204c_mb_irq : 1;
    unsigned en_jesd204c_rcv_204c_sh_irq : 1;
    unsigned jesd204c_rcv_204c_crc_irq : 1;
    unsigned jesd204c_rcv_204c_mb_irq : 1;
    unsigned jesd204c_rcv_204c_sh_irq : 1;
    unsigned rsv0 : 2;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0721)
    unsigned pwr_up_lcpll : 1;
    unsigned rsv0 : 2;
    unsigned clr_lolb : 1;
    unsigned rsv1 : 1;
    unsigned bypass_pll_lock_input : 1;
    unsigned rsv2 : 2;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0743)
    unsigned rsv0 : 1;
    unsigned sel_synca_refclk : 1;
    unsigned rsv1 : 6;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0800)
    unsigned ddsm_hopf_sel : 5;
    unsigned rsv0 : 1;
    unsigned ddsm_hopf_mode : 2;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0806)
    unsigned ddsm_hopf_ftw : 8;
REG_DEF_CPT_SUFFIX_8

typedef reg_0x0806
reg_0x0807,reg_0x0808,reg_0x0809,reg_0x080a,
reg_0x080b,reg_0x080c,reg_0x080d,reg_0x080e,
reg_0x080f,reg_0x0810,reg_0x0811,reg_0x0812,
reg_0x0813,reg_0x0814,reg_0x0815,reg_0x0816,
reg_0x0817,reg_0x0818,reg_0x0819,reg_0x081a,
reg_0x081b,reg_0x081c,reg_0x081d,reg_0x081e,
reg_0x081f,reg_0x0820,reg_0x0821,reg_0x0822,
reg_0x0823,reg_0x0824,reg_0x0825,reg_0x0826,
reg_0x0827,reg_0x0828,reg_0x0829,reg_0x082a,
reg_0x082b,reg_0x082c,reg_0x082d,reg_0x082e,
reg_0x082f,reg_0x0830,reg_0x0831,reg_0x0832,
reg_0x0833,reg_0x0834,reg_0x0835,reg_0x0836,
reg_0x0837,reg_0x0838,reg_0x0839,reg_0x083a,
reg_0x083b,reg_0x083c,reg_0x083d,reg_0x083e,
reg_0x083f,reg_0x0840,reg_0x0841,reg_0x0842,
reg_0x0843,reg_0x0844,reg_0x0845,reg_0x0846,
reg_0x0847,reg_0x0848,reg_0x0849,reg_0x084a,
reg_0x084b,reg_0x084c,reg_0x084d,reg_0x084e,
reg_0x084f,reg_0x0850,reg_0x0851,reg_0x0852,
reg_0x0853,reg_0x0854,reg_0x0855,reg_0x0856,
reg_0x0857,reg_0x0858,reg_0x0859,reg_0x085a,
reg_0x085b,reg_0x085c,reg_0x085d,reg_0x085e,
reg_0x085f,reg_0x0860,reg_0x0861,reg_0x0862,
reg_0x0863,reg_0x0864,reg_0x0865,reg_0x0866,
reg_0x0867,reg_0x0868,reg_0x0869,reg_0x086a,
reg_0x086b,reg_0x086c,reg_0x086d,reg_0x086e,
reg_0x086f,reg_0x0870,reg_0x0871,reg_0x0872,
reg_0x0873,reg_0x0874,reg_0x0875,reg_0x0876,
reg_0x0877,reg_0x0878,reg_0x0879,reg_0x087a,
reg_0x087b,reg_0x087c,reg_0x087d,reg_0x087e,
reg_0x087f,reg_0x0880,reg_0x0881;

REG_DEF_CPT_PREFIX_8(0x0a00)
    unsigned en_ddc_sync : 1;
    unsigned ddc_sync_next : 1;
    unsigned rsv0 : 2;
    unsigned ddc_soft_rst : 1;
    unsigned rsv1 : 2;
    unsigned en_ddc_trig_nco_rst : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a02)
    unsigned en_ddc_trig_hop : 1;
    unsigned rsv0 : 7;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a03)
    unsigned ddc0_nco_regmap_ch : 4;
    unsigned ddc0_nco_ch_sel_mode : 4;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a04)
    unsigned ddco_profile_update_idx : 4;
    unsigned rsv0 : 2;
    unsigned ddc0_gpio_chip_trans_mode : 1;
    unsigned ddc0_profile_update_mode : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a05)
    unsigned piw : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a06)
    unsigned piw : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a07)
    unsigned piw : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a08)
    unsigned piw : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a09)
    unsigned piw : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a0a)
    unsigned piw : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a0b)
    unsigned ddc0_phase_offset0 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a0c)
    unsigned ddc0_phase_offset1 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a0d)
    unsigned ddc0_phase_offset2 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a0e)
    unsigned ddc0_phase_offset3 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a0f)
    unsigned ddc0_phase_offset4 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a10)
    unsigned ddc0_phase_offset5 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a11)
    unsigned ddc0_phase_inc_frac_a0 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a12)
    unsigned ddc0_phase_inc_frac_a1 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a13)
    unsigned ddc0_phase_inc_frac_a2 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a14)
    unsigned ddc0_phase_inc_frac_a3 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a15)
    unsigned ddc0_phase_inc_frac_a4 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a16)
    unsigned ddc0_phase_inc_frac_a5 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a17)
    unsigned ddc0_phase_inc_frac_b0 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a18)
    unsigned ddc0_phase_inc_frac_b1 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a19)
    unsigned ddc0_phase_inc_frac_b2 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a1a)
    unsigned ddc0_phase_inc_frac_b3 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a1b)
    unsigned ddc0_phase_inc_frac_b4 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a1c)
    unsigned ddc0_phase_inc_frac_b5 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a1d)
    unsigned ddc0_chip_trans_status : 1;
    unsigned rsv0 : 7;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a1e)
    unsigned rsv0 : 1;
    unsigned en_ddc0_phase_dither : 1;
    unsigned rsv1 : 6;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a1f)
    unsigned ddc0_chip_trans : 1;
    unsigned rsv0 : 7;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a20)
    unsigned ddc0_psw0 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a21)
    unsigned ddc0_psw1 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a22)
    unsigned ddc0_psw2 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a23)
    unsigned ddc0_psw3 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a24)
    unsigned ddc0_psw4 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a25)
    unsigned ddc0_psw5 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a26)
    unsigned active_piw : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a27)
    unsigned active_piw : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a28)
    unsigned active_piw : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a29)
    unsigned active_piw : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a2a)
    unsigned active_piw : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a2b)
    unsigned active_piw : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a2c)
    unsigned ddc0_active_phase_offset0 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a2d)
    unsigned ddc0_active_phase_offset1 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a2e)
    unsigned ddc0_active_phase_offset2 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a2f)
    unsigned ddc0_active_phase_offset3 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a30)
    unsigned ddc0_active_phase_offset4 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a31)
    unsigned ddc0_active_phase_offset5 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a80)
    unsigned en_ddc_sync : 1;
    unsigned ddc_sync_next : 1;
    unsigned rsv0 : 2;
    unsigned ddc_soft_rst : 1;
    unsigned rsv1 : 2;
    unsigned en_ddc_trig_nco_rst : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a81)
    unsigned ddc_sync_en_clr : 1;
    unsigned rsv0 : 7;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a82)
    unsigned en_ddc_trig_hop : 1;
    unsigned rsv0 : 7;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a83)
    unsigned ddc0_nco_regmap_ch : 4;
    unsigned ddc0_nco_ch_sel_mode : 4;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a84)
    unsigned ddc0_profile_update_idx : 4;
    unsigned rsv0 : 2;
    unsigned ddc0_gpio_chip_trans_mode : 1;
    unsigned ddc0_profile_update_mode : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a85)
    unsigned ddc0_phase_inc0 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a86)
    unsigned ddc0_phase_inc1 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a87)
    unsigned ddc0_phase_inc2 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a88)
    unsigned ddc0_phase_inc3 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a89)
    unsigned ddc0_phase_inc4 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a8a)
    unsigned ddc0_phase_inc5 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a8b)
    unsigned ddc0_phase_offset0 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a8c)
    unsigned ddc0_phase_offset1 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a8d)
    unsigned ddc0_phase_offset2 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a8e)
    unsigned ddc0_phase_offset3 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a8f)
    unsigned ddc0_phase_offset4 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a90)
    unsigned ddc0_phase_offset5 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a91)
    unsigned ddc0_phase_inc_frac_a0 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a92)
    unsigned ddc0_phase_inc_frac_a1 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a93)
    unsigned ddc0_phase_inc_frac_a2 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a94)
    unsigned ddc0_phase_inc_frac_a3 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a95)
    unsigned ddc0_phase_inc_frac_a4 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a96)
    unsigned ddc0_phase_inc_frac_a5 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a97)
    unsigned ddc0_phase_inc_frac_b0 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a98)
    unsigned ddc0_phase_inc_frac_b1 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a99)
    unsigned ddc0_phase_inc_frac_b2 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a9a)
    unsigned ddc0_phase_inc_frac_b3 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a9b)
    unsigned ddc0_phase_inc_frac_b4 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a9c)
    unsigned ddc0_phase_inc_frac_b5 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a9d)
    unsigned ddc0_chip_trans_status : 1;
    unsigned rsv0 : 7;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a9e)
    unsigned rsv0 : 1;
    unsigned en_ddc0_phase_dither : 1;
    unsigned rsv1 : 6;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0a9f)
    unsigned ddc0_chip_trans : 1;
    unsigned rsv0 : 7;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0aa0)
    unsigned ddc0_psw0 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0aa1)
    unsigned ddc0_psw1 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0aa2)
    unsigned ddc0_psw2 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0aa3)
    unsigned ddc0_psw3 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0aa4)
    unsigned ddc0_psw4 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0aa5)
    unsigned ddc0_psw5 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0aa6)
    unsigned ddc0_active_phase_inc0 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0aa7)
    unsigned ddc0_active_phase_inc1 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0aa8)
    unsigned ddc0_active_phase_inc2 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0aa9)
    unsigned ddc0_active_phase_inc3 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0aaa)
    unsigned ddc0_active_phase_inc4 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0aab)
    unsigned ddc0_active_phase_inc5 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0aac)
    unsigned ddc0_active_phase_offset0 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0aad)
    unsigned ddc0_active_phase_offset1 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0aae)
    unsigned ddc0_active_phase_offset2 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0aaf)
    unsigned ddc0_active_phase_offset3 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0ab0)
    unsigned ddc0_active_phase_offset4 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0ab1)
    unsigned ddc0_active_phase_offset5 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0b00)
    unsigned rsv0 : 2;
    unsigned en_eq_gpio : 1;
    unsigned rsv1 : 5;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0b01)
    unsigned cshift0 : 4;
    unsigned rsv0 : 4;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0b02)
    unsigned cshift1 : 4;
    unsigned rsv0 : 4;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0b03)
    unsigned cshift2 : 4;
    unsigned rsv0 : 4;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0b04)
    unsigned cshift3 : 4;
    unsigned rsv0 : 4;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0b05)
    unsigned cdsel : 2;
    unsigned en_cd_gpio : 1;
    unsigned rsv0 : 5;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0b06)
    unsigned fdsel : 2;
    unsigned en_fd_gpio : 1;
    unsigned en_fd : 1;
    unsigned rsv0 : 1;
    unsigned eq_upsamp_clk_sel : 1;
    unsigned rsv1 : 2;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0b07)
    unsigned fshift0 : 4;
    unsigned rsv0 : 4;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0b08)
    unsigned fshift1 : 4;
    unsigned rsv0 : 4;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0b09)
    unsigned fshift2 : 4;
    unsigned rsv0 : 4;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0b0a)
    unsigned fshift3 : 4;
    unsigned rsv0 : 4;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0b12)
    unsigned pfilt_din_i_sel : 2;
    unsigned pfilt_din_q_sel : 2;
    unsigned rsv0 : 4;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0b13)
    unsigned pfilt_out_sel : 2;
    unsigned rsv0 : 6;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0b19)
    unsigned en_orx_gpio : 1;
    unsigned en_orx_reg : 1;
    unsigned orx_reg : 1;
    unsigned orx_fd_ctrl : 1;
    unsigned rsv0 : 4;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0b1a)
    unsigned orx_cd_ctrl : 4;
    unsigned orx_pfir_ctrl : 4;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0c0c)
    unsigned pfir_i_mode : 4;
    unsigned pfir_q_mode : 4;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0c0d)
    unsigned pfir_ix_gain : 3;
    unsigned pfir_iy_gain : 3;
    unsigned rsv0 : 2;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0c0f)
    unsigned pfir_qx_gain : 3;
    unsigned pfir_qy_gain : 3;
    unsigned rsv0 : 2;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0c11)
    unsigned delay : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0c17)
    unsigned pfir_coef_trans : 1;
    unsigned rsv0 : 7;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0c1c)
    unsigned quad_mode : 1;
    unsigned rsv0 : 7;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0c1d)
    unsigned real_i_load : 1;
    unsigned real_q_load : 1;
    unsigned real_cross_i_load : 1;
    unsigned real_cross_q_load : 1;
    unsigned complex_load : 1;
    unsigned rsv0 : 2;
    unsigned gpio_config : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0c1e)
    unsigned rd_coef_page : 2;
    unsigned rsv0 : 6;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0fb0)
    unsigned rsv0 : 3;
    unsigned en_spi_sysref : 1;
    unsigned rsv1 : 4;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x0fb8)
    unsigned sysref_setup : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x2008)
    unsigned pll_lock_slow : 1;
    unsigned pll_lock_fast : 1;
    unsigned rsv0 : 6;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x20d0)
    unsigned dsa_ctrl : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x20d1)
    unsigned dsa_cutover : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x20d2)
    unsigned dsa_boost : 5;
    unsigned rsv0 : 3;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x20d3)
    unsigned dp_gain : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x20d4)
    unsigned dp_gain : 4;
    unsigned rsv0 : 3;
    unsigned gain_load_strobe : 1;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x2c58)
    unsigned dsa0 : 2;
    unsigned dsa1 : 2;
    unsigned dsa2 : 2;
    unsigned dsa3 : 2;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x37cc)
    unsigned peri_i_12 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x37cd)
    unsigned peri_i_13 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x37ce)
    unsigned peri_i_14 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x37cf)
    unsigned peri_i_15 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x37d0)
    unsigned peri_i_16 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x37d1)
    unsigned peri_i_17 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x37d2)
    unsigned peri_i_18 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x37d3)
    unsigned peri_i_19 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x37d4)
    unsigned peri_i_20 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x37d5)
    unsigned peri_i_21 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x37d6)
    unsigned peri_i_22 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x37d7)
    unsigned peri_i_23 : 8;
REG_DEF_CPT_SUFFIX_8

REG_DEF_CPT_PREFIX_8(0x37d8)
    unsigned peri_i_24 : 8;
REG_DEF_CPT_SUFFIX_8

} // ns_reg_ad908x

#endif
