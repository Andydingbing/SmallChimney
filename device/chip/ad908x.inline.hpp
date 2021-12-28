#ifndef RD_CHIP_AD908X_INLINE_HPP
#define RD_CHIP_AD908X_INLINE_HPP

#define AD908X_REG_W(addr) INT_CHECK(set_reg(addr,AD908X_REG_DATA(addr)))
#define AD908X_REG_R(addr) INT_CHECK(get_reg(addr,AD908X_REG_DATA(addr)))

#include "ad908x.hpp"
#include "reg_def_ad908x.h"
#include "algo_chip.hpp"

AD908X_FUNC::set_adc_cddc_page(const uint32_t cddc) const
{
    AD908X_REG_DECL(0x0018,0x00);

    AD908X_REG_R(0x0018);
    AD908X_REG(0x0018).cddc_page = (1 << cddc);
    AD908X_REG_W(0x0018);
    return 0;
}

AD908X_FUNC::set_adc_ddc_coarse_nco_ftw(const uint32_t cddc,const double freq) const
{
    AD908X_REG_DECL(0x0a05,0x00);
    AD908X_REG_DECL(0x0a06,0x00);
    AD908X_REG_DECL(0x0a07,0x00);
    AD908X_REG_DECL(0x0a08,0x00);
    AD908X_REG_DECL(0x0a09,0x00);
    AD908X_REG_DECL(0x0a0a,0x00);

    INT_CHECK(set_adc_cddc_page(cddc));

    uint64_t piw = ns_ad908x::ftw_integer_mode(freq,5898240000.0);
    AD908X_REG(0x0a05).piw = unsigned(piw);
    AD908X_REG(0x0a06).piw = unsigned(piw >> 8);
    AD908X_REG(0x0a07).piw = unsigned(piw >> 16);
    AD908X_REG(0x0a08).piw = unsigned(piw >> 24);
    AD908X_REG(0x0a09).piw = unsigned(piw >> 32);
    AD908X_REG(0x0a0a).piw = unsigned(piw >> 40);
    AD908X_REG_W(0x0a05);
    AD908X_REG_W(0x0a06);
    AD908X_REG_W(0x0a07);
    AD908X_REG_W(0x0a08);
    AD908X_REG_W(0x0a09);
    AD908X_REG_W(0x0a0a);
    return 0;
}

AD908X_FUNC::get_adc_ddc_coarse_nco_ftw(const uint32_t cddc,double &freq) const
{
    AD908X_REG_DECL(0x0a26,0x00);
    AD908X_REG_DECL(0x0a27,0x00);
    AD908X_REG_DECL(0x0a28,0x00);
    AD908X_REG_DECL(0x0a29,0x00);
    AD908X_REG_DECL(0x0a2a,0x00);
    AD908X_REG_DECL(0x0a2b,0x00);

    INT_CHECK(set_adc_cddc_page(cddc));

    AD908X_REG_R(0x0a26);
    AD908X_REG_R(0x0a27);
    AD908X_REG_R(0x0a28);
    AD908X_REG_R(0x0a29);
    AD908X_REG_R(0x0a2a);
    AD908X_REG_R(0x0a2b);

    uint64_t piw =
            uint64_t(AD908X_REG(0x0a26).active_piw) |
            uint64_t(AD908X_REG(0x0a27).active_piw) << 8  |
            uint64_t(AD908X_REG(0x0a28).active_piw) << 16 |
            uint64_t(AD908X_REG(0x0a29).active_piw) << 24 |
            uint64_t(AD908X_REG(0x0a2a).active_piw) << 32 |
            uint64_t(AD908X_REG(0x0a2b).active_piw) << 40;

    freq = ns_ad908x::nco_integer_mode(piw,5898240000.0);
    return 0;
}

AD908X_FUNC::set_dac_duc_nco_ftw(const uint32_t dac,const double freq) const
{
    AD908X_REG_DECL(0x001b,0x00);
    AD908X_REG_DECL(0x01c9,0x00);
    AD908X_REG_DECL(0x01ca,0x00);
    AD908X_REG_DECL(0x01cb,0x00);
    AD908X_REG_DECL(0x01cc,0x00);
    AD908X_REG_DECL(0x01cd,0x00);
    AD908X_REG_DECL(0x01ce,0x00);
    AD908X_REG_DECL(0x01cf,0x00);
    AD908X_REG_DECL(0x01d0,0x00);

    AD908X_REG(0x001b).dac_page_msk = 1 << dac;
    AD908X_REG_W(0x001b);
    AD908X_REG(0x01c9).en_ddsm_nco = 1;
    AD908X_REG_W(0x01c9);
    AD908X_REG_W(0x001b);
    AD908X_REG(0x01ca).ddsm_ftw_load_req = 1;
    AD908X_REG(0x01ca).rsv0 = 1;
    AD908X_REG_W(0x01ca);
    AD908X_REG_W(0x01c9);
    AD908X_REG_W(0x001b);

    uint64_t ddsm_ftw = ns_ad908x::ftw_integer_mode(freq,11796480000.0);
    AD908X_REG(0x01cb).ddsm_ftw = unsigned(ddsm_ftw);
    AD908X_REG(0x01cc).ddsm_ftw = unsigned(ddsm_ftw >> 8);
    AD908X_REG(0x01cd).ddsm_ftw = unsigned(ddsm_ftw >> 16);
    AD908X_REG(0x01ce).ddsm_ftw = unsigned(ddsm_ftw >> 24);
    AD908X_REG(0x01cf).ddsm_ftw = unsigned(ddsm_ftw >> 32);
    AD908X_REG(0x01d0).ddsm_ftw = unsigned(ddsm_ftw >> 40);
    AD908X_REG_W(0x01cb);
    AD908X_REG_W(0x01cc);
    AD908X_REG_W(0x01cd);
    AD908X_REG_W(0x01ce);
    AD908X_REG_W(0x01cf);
    AD908X_REG_W(0x01d0);

    AD908X_REG(0x01ca).ddsm_ftw_load_req = 0;
    AD908X_REG_W(0x01ca);
    AD908X_REG(0x01ca).ddsm_ftw_load_req = 1;
    AD908X_REG(0x01ca).rsv0 = 0;
    AD908X_REG_W(0x01ca);
    return 0;
}

#endif // RD_CHIP_AD908X_INLINE_HPP
