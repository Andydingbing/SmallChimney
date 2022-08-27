#include "sp2401_r1a.h"
#include "../chip/reg_def_sp9500.h"
#include "../cal_table.h"
#include "sleep_common.h"
#include "algo_math.hpp"
#include "gen_int.hpp"
#include <sstream>

#define NCO_DDS_32 0
#define NCO_DDS_64 1

#define NCO_DUC_32 0
#define NCO_DUC_64 1

#define NCO_DDC_32 0
#define NCO_DDC_64 1

using namespace std;
using namespace ns_starpoint;
using namespace ns_sp9500;
using namespace ns_sp2401;

sp2401_r1a::sp2401_r1a(uint32_t rf_idx)
{
    _k7 = nullptr;
    _s6 = nullptr;
    _rf_idx = rf_idx;
}

int32_t sp2401_r1a::open_board(pci_dev_vi *k7, pci_dev_vi *s6)
{
    _k7 = k7;
    _s6 = s6;

    INT_CHECK(set_da_sw(FROM_TO_RF));
    INT_CHECK(set_ad_sw(FROM_TO_RF));

    if (_rf_idx == 2) {
        INT_CHECK(set_da_freq_MHz(491.52));
        INT_CHECK(set_ad_freq_MHz(368.64));
    }

    bool lock = false;
    if (_rf_idx == 0 || _rf_idx == 2) {
        INT_CHECK(clock_of_da_ld(lock));
        if (lock == false) {
            Log.set_last_err("ADF4351 of DAC unlocked");
        }
        INT_CHECK(da_reset());
        INT_CHECK(clock_of_ad_ld(lock));
        if (lock == false) {
            Log.set_last_err("ADF4351 of ADC unlocked");
        }
    }

    INT_CHECK(set_dds_src(INTER_FILTER));
    INT_CHECK(set_dds1(20e6));
    INT_CHECK(set_dds2(20e6));
    INT_CHECK(set_dds_pwr_comp());
    INT_CHECK(set_duc_dds(0.0));
    INT_CHECK(set_tx_filter_truncation(12));
    INT_CHECK(set_tx_filter_sw(true));
    INT_CHECK(set_tx_pwr_comp(0.0));

    INT_CHECK(set_ddc(-92.64e6));
    INT_CHECK(set_rx_pwr_comp(0));
    INT_CHECK(set_rx_filter_truncation(12));
    INT_CHECK(set_rx_filter_sw(_2I3D));
    INT_CHECK(set_rx_filter_default());

    INT_CHECK(set_frame_trig_src(NEXT_FRAME));
    INT_CHECK(set_frame_trig_offset(0));
	return 0;
}

int32_t sp2401_r1a::open_board(pci_dev_vi *s6)
{
    _s6 = s6;
	return 0;
}

int32_t sp2401_r1a::get_s6_ver(uint32_t &ver)
{
    RFU_S6_REG_DECLARE(0x0000);
    RFU_S6_R(0x0000);
    ver = RFU_S6_REG_DATA(0x0000);
	return 0;
}

int32_t sp2401_r1a::fpga_reset()
{
    RFU_K7_REG_DECLARE(0x0000);
    RFU_K7_OP(0x0000);
	return 0;
}

pci_dev* sp2401_r1a::k7()
{
    return _k7;
}

pci_dev* sp2401_r1a::s6()
{
    return _s6;
}

int32_t sp2401_r1a::clock_of_da_ld(bool &lock)
{
    RFU_K7_REG_DECLARE(0x0002);
    RFU_K7_RE(0x0002);
    RFU_K7_R(0x0002);
    lock = RFU_K7_REG(0x0002).ld_da == 1;
    sleep_ms(2);
    RFU_K7_FE(0x0002);
    sleep_ms(50);
    return 0;
}

int32_t sp2401_r1a::clock_of_ad_ld(bool &lock)
{
    RFU_K7_REG_DECLARE(0x0002);
    RFU_K7_RE(0x0002);
    RFU_K7_R(0x0002);
    lock = RFU_K7_REG(0x0002).ld_ad == 1;
    sleep_ms(2);
    RFU_K7_FE(0x0002);
    sleep_ms(50);
    return 0;
}

int32_t sp2401_r1a::set_da_freq_MHz(double freq)
{
    RFU_K7_REG_DECLARE(0x0035);
    RFU_K7_REG_DECLARE(0x0036);
    RFU_K7_REG(0x0036).freq_int = unsigned(freq);
    RFU_K7_REG(0x0036).freq_frac = unsigned((freq - unsigned(freq)) * 100);
    RFU_K7_W(0x0036);
    RFU_K7_REG(0x0035).ad_da = 0;
    RFU_K7_OP(0x0035);
	return 0;
}

int32_t sp2401_r1a::set_da_sw(ad_da_port_t port)
{
    RFU_S6_REG_DECLARE(0x000b);
    RFU_S6_R(0x000b);
    switch (_rf_idx) {
        case 0 : {RFU_S6_REG(0x000b).rf_ch_0 = unsigned(port);break;}
        case 1 : {RFU_S6_REG(0x000b).rf_ch_1 = unsigned(port);break;}
        case 2 : {RFU_S6_REG(0x000b).rf_ch_2 = unsigned(port);break;}
        case 3 : {RFU_S6_REG(0x000b).rf_ch_3 = unsigned(port);break;}
		default:break;
	}
    RFU_S6_W(0x000b);
	return 0;
}

int32_t sp2401_r1a::da_reset()
{
    RFU_S6_REG_DECLARE(0x000b);
    RFU_S6_R(0x000b);
    RFU_S6_REG(0x000b).rst = 1;
    RFU_S6_W(0x000b);
    sleep_ms(10);
    RFU_S6_REG(0x000b).rst = 0;
    RFU_S6_W(0x000b);
	return 0;
}

int32_t sp2401_r1a::set_ad_freq_MHz(double freq)
{
    RFU_K7_REG_DECLARE(0x0035);
    RFU_K7_REG_DECLARE(0x0036);
    RFU_K7_REG(0x0036).freq_int = unsigned(freq);
    RFU_K7_REG(0x0036).freq_frac = unsigned((freq - unsigned(freq)) * 100);
    RFU_K7_W(0x0036);
    RFU_K7_REG(0x0035).ad_da = 1;
    RFU_K7_OP(0x0035);
	return 0;
}

int32_t sp2401_r1a::set_ad_sw(ad_da_port_t port)
{
    RFU_S6_REG_DECLARE(0x000a);
    RFU_S6_R(0x000a);
    switch (_rf_idx) {
        case 0 : {RFU_S6_REG(0x000a).rf_ch_0 = unsigned(port);break;}
        case 1 : {RFU_S6_REG(0x000a).rf_ch_1 = unsigned(port);break;}
        case 2 : {RFU_S6_REG(0x000a).rf_ch_2 = unsigned(port);break;}
        case 3 : {RFU_S6_REG(0x000a).rf_ch_3 = unsigned(port);break;}
		default:break;
	}
    RFU_S6_W(0x000a);
	return 0;
}

int32_t sp2401_r1a::set_tx_pwr_comp(double offset)
{
    RFU_K7_REG_DECLARE_2(0x0094,0x2094);
    RFU_K7_REG_2(0x0094,0x2094).i = uint16_t(offset * 512.0);
    RFU_K7_REG_2(0x0094,0x2094).q = uint16_t(offset * 512.0);
    RFU_K7_W_2(0x0094,0x2094);
	return 0;
}

int32_t sp2401_r1a::set_tx_pwr_comp(float offset)
{
    return set_tx_pwr_comp(double(offset));
}

int32_t sp2401_r1a::get_tx_pwr_comp(double &offset)
{
    RFU_K7_REG_DECLARE_2(0x0094,0x2094);
    RFU_K7_R_2(0x0094,0x2094);
    offset = RFU_K7_REG_2(0x0094,0x2094).i / 512.0;
    return 0;
}

int32_t sp2401_r1a::set_dds_src(da_src_t src)
{
    RFU_K7_REG_DECLARE_2(0x0072,0x2072);
    RFU_K7_REG_2(0x0072,0x2072).bb_tx_source = src;
    RFU_K7_W_2(0x0072,0x2072);
	return 0;
}

int32_t sp2401_r1a::set_dds1(double freq)
{
    RFU_K7_REG_DECLARE_2(0x0079,0x2079);
    RFU_K7_REG_DECLARE_2(0x007a,0x207a);

#if (NCO_DDS_32)
    RFU_K7_REG_2(0x007a,0x207a).freq = (unsigned)(4294967296.0 * freq / 1000000.0 / 245.76);
#elif (NCO_DDS_64)
    RFU_K7_REG_DECLARE_2(0x00e9,0x20e9);
    unsigned int dds_h = 0;
    unsigned int dds_l = 0;
    stringstream ss;

    ss << int64_t(round(freq,0));
    gen_int dds = gen_int("18446744073709551616") * gen_int(ss.str().c_str()) / 245760000;
    dds._2ui64(dds_h,dds_l);
    RFU_K7_REG_2(0x007a,0x207a).freq = dds_l;
    RFU_K7_REG_2(0x00e9,0x20e9).freq = dds_h;
    RFU_K7_W_2(0x00e9,0x20e9);
#endif

    RFU_K7_W_2(0x007a,0x207a);
    RFU_K7OP_2(0x0079,0x2079,dds1_op);
	return 0;
}

int32_t sp2401_r1a::set_dds2(double freq)
{
    RFU_K7_REG_DECLARE_2(0x0079,0x2079);

#if (NCO_DDS_32)
    RFU_K7_REG_DECLARE_2(0x007a,0x207a);
    RFU_K7_REG_2(0x007a,0x207a).freq = (unsigned)(4294967296.0 * freq / 1000000.0 / 245.76);
    RFU_K7_W_2(0x007a,0x207a);
#elif (NCO_DDS_64)
    RFU_K7_REG_DECLARE_2(0x00ea,0x20ea);
    RFU_K7_REG_DECLARE_2(0x00eb,0x20eb);
    unsigned int dds_h = 0;
    unsigned int dds_l = 0;
    stringstream ss;

    ss << int64_t(round(freq,0));
    gen_int dds = gen_int("18446744073709551616") * gen_int(ss.str().c_str()) / 245760000;
    dds._2ui64(dds_h,dds_l);
    RFU_K7_REG_2(0x00eb,0x20eb).freq = dds_h;
    RFU_K7_REG_2(0x00ea,0x20ea).freq = dds_l;
    RFU_K7_W_2(0x00eb,0x20eb);
    RFU_K7_W_2(0x00ea,0x20ea);
#endif

    RFU_K7OP_2(0x0079,0x2079,dds2_op);
	return 0;
}

int32_t sp2401_r1a::set_dds_pwr_comp(double offset)
{
    RFU_K7_REG_DECLARE_2(0x0085,0x2085);
    RFU_K7_REG_2(0x0085,0x2085).dds1_amp = unsigned(sqrt(double(g_0dBFS)) * 8192 / 32768);
    RFU_K7_REG_2(0x0085,0x2085).dds2_amp = unsigned(sqrt(double(g_0dBFS)) * 8192 / 32768);
    RFU_K7_W_2(0x0085,0x2085);
	return 0;
}

int32_t sp2401_r1a::set_duc_dds(double freq)
{
    RFU_K7_REG_DECLARE_2(0x007b,0x207b);
    RFU_K7_REG_DECLARE_2(0x007c,0x207c);

#if (NCO_DUC_32)
    RFU_K7_REG_2(0x007c,0x207c).duc = (unsigned)(4294967296.0 * freq / 1000000.0 / 245.76);
#elif (NCO_DUC_64)
    RFU_K7_REG_DECLARE_2(0x00e8,0x20e8);
    unsigned int duc_h = 0;
    unsigned int duc_l = 0;
    stringstream ss;

    ss << int64_t(round(freq,0));
    gen_int duc = gen_int("18446744073709551616") * gen_int(ss.str().c_str()) / 245760000;
    duc._2ui64(duc_h,duc_l);
    RFU_K7_REG_2(0x007c,0x207c).duc = duc_l;
    RFU_K7_REG_2(0x00e8,0x20e8).duc = duc_h;
    RFU_K7_W_2(0x00e8,0x20e8);
#endif

    RFU_K7_W_2(0x007c,0x207c);
    RFU_K7_OP_2(0x007b,0x207b);
	return 0;
}

int32_t sp2401_r1a::set_tx_filter_truncation(uint32_t digit)
{
    RFU_K7_REG_DECLARE_2(0x00ff,0x20ff);
    RFU_K7_R_2(0x00ff,0x20ff);
    RFU_K7_REG_2(0x00ff,0x20ff).digit_tx = unsigned(digit);
    RFU_K7_W_2(0x00ff,0x20ff);
	return 0;
}

int32_t sp2401_r1a::set_tx_filter_sw(bool en)
{
    RFU_K7_REG_DECLARE_2(0x00fa,0x20fa);
    RFU_K7_R_2(0x00fa,0x20fa);
    RFU_K7_REG_2(0x00fa,0x20fa).en = en ? 1 : 0;
    RFU_K7_W_2(0x00fa,0x20fa);
	return 0;
}

int32_t sp2401_r1a::set_tx_filter(double *real,double *imag)
{
    RFU_K7_REG_DECLARE_2(0x00ec,0x20ec);
    RFU_K7_REG_DECLARE_2(0x00ed,0x20ed);
    RFU_K7_REG_DECLARE_2(0x00ee,0x20ee);
    RFU_K7_REG_DECLARE_2(0x00ef,0x20ef);
    RFU_K7_REG_DECLARE_2(0x00f0,0x20f0);
    RFU_K7_REG_DECLARE_2(0x00f1,0x20f1);

    for (int32_t i = 0;i < dl_filter_tap_2i;i ++) {
        RFU_K7_REG_2(0x00ed,0x20ed).addr = unsigned(i);
        RFU_K7_REG_2(0x00ef,0x20ef).real = unsigned(real[i]);
        RFU_K7_REG_2(0x00f0,0x20f0).imag = unsigned(imag[i]);
        RFU_K7_REG_2(0x00ee,0x20ee).sum  = unsigned(real[i] + imag[i]);
        RFU_K7_W_2(0x00ed,0x20ed);
        RFU_K7_W_2(0x00ee,0x20ee);
        RFU_K7_W_2(0x00ef,0x20ef);
        RFU_K7_W_2(0x00f0,0x20f0);
        RFU_K7_OP_2(0x00ec,0x20ec);
	}
    RFU_K7_OP_2(0x00f1,0x20f1);
	return 0;
}

int32_t sp2401_r1a::set_tx_filter(data_m_tx_filter &data)
{
    RFU_K7_REG_DECLARE_2(0x00ec,0x20ec);
    RFU_K7_REG_DECLARE_2(0x00ed,0x20ed);
    RFU_K7_REG_DECLARE_2(0x00ee,0x20ee);
    RFU_K7_REG_DECLARE_2(0x00ef,0x20ef);
    RFU_K7_REG_DECLARE_2(0x00f0,0x20f0);
    RFU_K7_REG_DECLARE_2(0x00f1,0x20f1);

    for (int32_t i = 0;i < dl_filter_tap_2i;i ++) {
        RFU_K7_REG_2(0x00ed,0x20ed).addr = unsigned(i);
        RFU_K7_REG_2(0x00ef,0x20ef).real = unsigned(data.coef[i].i);
        RFU_K7_REG_2(0x00f0,0x20f0).imag = unsigned(data.coef[i].q);
        RFU_K7_REG_2(0x00ee,0x20ee).sum  = unsigned(data.coef[i].i + data.coef[i].q);
        RFU_K7_W_2(0x00ed,0x20ed);
        RFU_K7_W_2(0x00ee,0x20ee);
        RFU_K7_W_2(0x00ef,0x20ef);
        RFU_K7_W_2(0x00f0,0x20f0);
        RFU_K7_OP_2(0x00ec,0x20ec);
    }
    RFU_K7_OP_2(0x00f1,0x20f1);
    return 0;
}

int32_t sp2401_r1a::set_ddc(double freq)
{
    RFU_K7_REG_DECLARE_2(0x007d,0x207d);
    RFU_K7_REG_DECLARE_2(0x007e,0x207e);

#if (NCO_DDC_32)
    RFU_K7_REG_2(0x007e,0x207e).ddc = (unsigned)(freq * 4294967296 / (368.64 * 1e6));
#elif (NCO_DDC_64)
    RFU_K7_REG_DECLARE_2(0x00e7,0x20e7);
    unsigned int ddc_h = 0;
    unsigned int ddc_l = 0;
    stringstream ss;

    ss << int64_t(round(freq,0));
    gen_int ddc = gen_int("18446744073709551616") * gen_int(ss.str().c_str()) / 368640000;
    ddc._2ui64(ddc_h,ddc_l);
    RFU_K7_REG_2(0x007e,0x207e).ddc = ddc_l;
    RFU_K7_REG_2(0x00e7,0x20e7).ddc = ddc_h;
    RFU_K7_W_2(0x00e7,0x20e7);
#endif

    RFU_K7_W_2(0x007e,0x207e);
    RFU_K7_OP_2(0x007d,0x207d);
	return 0;
}

int32_t sp2401_r1a::set_rx_filter_truncation(uint32_t digit)
{
    RFU_K7_REG_DECLARE_2(0x00ff,0x20ff);
    RFU_K7_R_2(0x00ff,0x20ff);
    RFU_K7_REG_2(0x00ff,0x20ff).digit_rx = unsigned(digit);
    RFU_K7_W_2(0x00ff,0x20ff);
	return 0;
}

int32_t sp2401_r1a::set_rx_filter_sw(rx_filter_t filter)
{
    RFU_K7_REG_DECLARE_2(0x00f8,0x20f8);
    RFU_K7_REG_2(0x00f8,0x20f8).filter = (_2I3D == filter ? 1 : 0);
    RFU_K7_W_2(0x00f8,0x20f8);
	return 0;
}

int32_t sp2401_r1a::set_rx_filter_2I3D(double *real,double *imag)
{
    RFU_K7_REG_DECLARE_2(0x1069,0x1099);
    RFU_K7_REG_DECLARE_2(0x106c,0x109c);
    RFU_K7_REG_DECLARE_2(0x1080,0x10b0);

    RFU_K7_REG_2(0x106c,0x109c).group = 0;
    for (int32_t i = 0;i < ul_filter_tap_2i3d;i ++) {
        RFU_K7_REG_2(0x106c,0x109c).op = 0;
        RFU_K7_REG_2(0x106c,0x109c).addr = unsigned(i);
        RFU_K7_REG_2(0x106c,0x109c).real = unsigned(real[i]);
        RFU_K7_REG_2(0x1080,0x10b0).imag = unsigned(imag[i]);
        RFU_K7_W_2(0x1080,0x10b0);
        RFU_K7_OP_2(0x106c,0x109c);
	}
    RFU_K7_OP_2(0x1069,0x1099);
	return 0;
}

int32_t sp2401_r1a::set_rx_filter_2I3D_default()
{
    double real[ul_filter_tap_2i3d] = {
        -22,-35,9,21,-1,-31,-1,36,9,-46,-21,49,33,-54,
        -50,55,69,-53,-92,46,116,-33,-142,13,168,15,-191,
        -52,211,98,-224,-152,228,214,-222,-283,201,356,-165,
        -432,111,507,-40,-584,-59,650,181,-706,-329,745,504,
        -767,-718,750,958,-699,-1238,597,1565,-430,-1957,167,
        2444,241,-3099,-909,4098,2151,-6037,-5288,12703,32767,
        32767,12703,-5288,-6037,2151,4098,-909,-3099,241,2444,
        167,-1957,-430,1565,597,-1238,-699,958,750,-718,-767,
        504,745,-329,-706,181,650,-59,-584,-40,507,111,-432,
        -165,356,201,-283,-222,214,228,-152,-224,98,211,-52,
        -191,15,168,13,-142,-33,116,46,-92,-53,69,55,-50,-54,
        33,49,-21,-46,9,36,-1,-31,-1,21,9,-35,-22};

    double imag[ul_filter_tap_2i3d] = {
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0};

    return set_rx_filter_2I3D(real,imag);
}

int32_t sp2401_r1a::set_rx_filter(double *real,double *imag)
{
    RFU_K7_REG_DECLARE_2(0x00f2,0x20f2);
    RFU_K7_REG_DECLARE_2(0x00f3,0x20f3);
    RFU_K7_REG_DECLARE_2(0x00f4,0x20f4);
    RFU_K7_REG_DECLARE_2(0x00f5,0x20f5);
    RFU_K7_REG_DECLARE_2(0x00f6,0x20f6);
    RFU_K7_REG_DECLARE_2(0x00f7,0x20f7);

    for (int32_t i = 0;i < ul_filter_tap;i ++) {
        RFU_K7_REG_2(0x00f3,0x20f3).addr = unsigned(i);
        RFU_K7_REG_2(0x00f5,0x20f5).real = unsigned(real[i]);
        RFU_K7_REG_2(0x00f6,0x20f6).imag = unsigned(imag[i]);
        RFU_K7_REG_2(0x00f4,0x20f4).sum  = unsigned(real[i] + imag[i]);
        RFU_K7_W_2(0x00f3,0x20f3);
        RFU_K7_W_2(0x00f4,0x20f4);
        RFU_K7_W_2(0x00f5,0x20f5);
        RFU_K7_W_2(0x00f6,0x20f6);
        RFU_K7_OP_2(0x00f2,0x20f2);
	}
    RFU_K7_OP_2(0x00f7,0x20f7);
	return 0;
}

template <typename data_t>
int32_t sp2401_r1a::set_rx_filter(const data_t &data) const
{
    RFU_K7_REG_DECLARE_2(0x00f2,0x20f2);
    RFU_K7_REG_DECLARE_2(0x00f3,0x20f3);
    RFU_K7_REG_DECLARE_2(0x00f4,0x20f4);
    RFU_K7_REG_DECLARE_2(0x00f5,0x20f5);
    RFU_K7_REG_DECLARE_2(0x00f6,0x20f6);
    RFU_K7_REG_DECLARE_2(0x00f7,0x20f7);

    for (uint32_t i = 0;i < ARRAY_SIZE(data.coef);i ++) {
        RFU_K7_REG_2(0x00f3,0x20f3).addr = unsigned(i);
        RFU_K7_REG_2(0x00f5,0x20f5).real = unsigned(data.coef[i].i);
        RFU_K7_REG_2(0x00f6,0x20f6).imag = unsigned(data.coef[i].q);
        RFU_K7_REG_2(0x00f4,0x20f4).sum  = unsigned(data.coef[i].i + data.coef[i].q);
        RFU_K7_W_2(0x00f3,0x20f3);
        RFU_K7_W_2(0x00f4,0x20f4);
        RFU_K7_W_2(0x00f5,0x20f5);
        RFU_K7_W_2(0x00f6,0x20f6);
        RFU_K7_OP_2(0x00f2,0x20f2);
    }
    RFU_K7_OP_2(0x00f7,0x20f7);
    return 0;
}

int32_t sp2401_r1a::set_rx_filter(const data_m_rx_filter_80 &data) const
{ return set_rx_filter<data_m_rx_filter_80>(data); }

int32_t sp2401_r1a::set_rx_filter(const data_m_rx_filter_160 &data) const
{ return set_rx_filter<data_m_rx_filter_160>(data); }

int32_t sp2401_r1a::set_rx_filter_default()
{
    double real[ul_filter_tap] = {0.0};
    double imag[ul_filter_tap] = {0.0};

    sp2401_r1a::rx_filter_coef_default(real,imag,ul_filter_tap);
    return set_rx_filter(real,imag);
}

int32_t sp2401_r1a::set_rx_pwr_comp(int32_t offset)
{
    double ptp_iq = sqrt(double(g_0dBFS - uint64_t(offset)));
    int32_t ratio = int32_t(round((sqrt(double(g_0dBFS)) * (8192 / ptp_iq)),0));

    RFU_K7_REG_DECLARE_2(0x00e4,0x20e4);
    RFU_K7_REG_2(0x00e4,0x20e4).ratio = unsigned(ratio);
    RFU_K7_W_2(0x00e4,0x20e4);
	return 0;
}

int32_t sp2401_r1a::set_tx_dc_offset(uint16_t i,uint16_t q)
{
    RFU_K7_REG_DECLARE_2(0x0061,0x2061);
    RFU_K7_REG_2(0x0061,0x2061).i = i;
    RFU_K7_REG_2(0x0061,0x2061).q = q;
    RFU_K7_W_2(0x0061,0x2061);
	return 0;
}

int32_t sp2401_r1a::set_tx_phase_rotate_I(double th)
{
    RFU_K7_REG_DECLARE_2(0x006a,0x206a);
    RFU_K7_REG_2(0x006a,0x206a).cos = uint16_t(cos(angle_to_radian<double>(th)) * 16384);
    RFU_K7_REG_2(0x006a,0x206a).sin = uint16_t(sin(angle_to_radian<double>(th)) * 16384);
    RFU_K7_W_2(0x006a,0x206a);
	return 0;
}

int32_t sp2401_r1a::set_frame_trig_mode(frame_trig_mode_t mode)
{
    RFU_K7_REG_DECLARE(0x0545);
    RFU_K7_R(0x0545);
    RFU_K7_REG(0x0545).frame_trig_mode = mode;
    RFU_K7_W(0x0545);
    return 0;
}

int32_t sp2401_r1a::set_frame_trig_in_offset(bool en, double time_ms)
{
    RFU_K7_REG_DECLARE(0x0545);
    RFU_K7_REG_DECLARE(0x0548);
    RFU_K7_R(0x0545);
    if (en) {
        RFU_K7_REG(0x0545).frame_trig_in_offset = 1;
        RFU_K7_REG(0x0548).frame_trig_in_offset = unsigned(round(time_ms * 245760, 0));
        RFU_K7_W(0x0545);
        RFU_K7_W(0x0548);
    } else {
        RFU_K7_REG(0x0545).frame_trig_in_offset = 0;
        RFU_K7_W(0x0545);
    }
    return 0;
}

int32_t sp2401_r1a::set_frame_trig_out_offset(bool en, double time_ms)
{
    RFU_K7_REG_DECLARE(0x0545);
    RFU_K7_REG_DECLARE(0x054c);
    RFU_K7_R(0x0545);
    if (en) {
        RFU_K7_REG(0x0545).frame_trig_out_offset = 1;
        RFU_K7_REG(0x054c).frame_trig_out_offset = unsigned(round(time_ms * 245760, 0));
        RFU_K7_W(0x0545);
        RFU_K7_W(0x054c);
    } else {
        RFU_K7_REG(0x0545).frame_trig_out_offset = 0;
        RFU_K7_W(0x0545);
    }
    return 0;
}

int32_t sp2401_r1a::set_frame_time(double time_ms)
{
    RFU_K7_REG_DECLARE(0x0546);
    RFU_K7_REG(0x0546).frame_time = unsigned(round(time_ms * 245760, 0));
    RFU_K7_W(0x0546);
    return 0;
}

double sp2401_r1a::get_frame_time_ms()
{
    RFU_K7_REG_DECLARE(0x0546);
    return RFU_K7_REG(0x0546).frame_time / 245760.0;
}

int32_t sp2401_r1a::set_hyper_frame_trig_mode(hyper_frame_trig_mode_t mode)
{
    RFU_K7_REG_DECLARE(0x0545);
    RFU_K7_R(0x0545);
    RFU_K7_REG(0x0545).hyper_frame_trig_mode = mode;
    RFU_K7_W(0x0545);
    return 0;
}

int32_t sp2401_r1a::set_hyper_frame_trig_in_offset(bool en, double time_ms)
{
    RFU_K7_REG_DECLARE(0x0545);
    RFU_K7_REG_DECLARE(0x054a);
    RFU_K7_R(0x0545);
    if (en) {
        RFU_K7_REG(0x0545).hyper_frame_trig_in_offset = 1;
        RFU_K7_REG(0x054a).hyper_frame_trig_in_offset = unsigned(round(time_ms * 245760, 0));
        RFU_K7_W(0x0545);
        RFU_K7_W(0x054a);
    } else {
        RFU_K7_REG(0x0545).hyper_frame_trig_in_offset = 0;
        RFU_K7_W(0x0545);
    }
    return 0;
}

int32_t sp2401_r1a::set_hyper_frame_trig_out_offset(bool en, double time_ms)
{
    RFU_K7_REG_DECLARE(0x0545);
    RFU_K7_REG_DECLARE(0x054e);
    RFU_K7_R(0x0545);
    if (en) {
        RFU_K7_REG(0x0545).hyper_frame_trig_out_offset = 1;
        RFU_K7_REG(0x054e).hyper_frame_trig_out_offset = unsigned(round(time_ms * 245760, 0));
        RFU_K7_W(0x0545);
        RFU_K7_W(0x054e);
    } else {
        RFU_K7_REG(0x0545).hyper_frame_trig_out_offset = 0;
        RFU_K7_W(0x0545);
    }
    return 0;
}

int32_t sp2401_r1a::set_hyper_frame_period(uint32_t period)
{
    RFU_K7_REG_DECLARE(0x0547);
    RFU_K7_REG(0x0547).hyper_frame_period = period;
    RFU_K7_W(0x0547);
    return 0;
}

double sp2401_r1a::get_hyper_frame_time_ms()
{
    RFU_K7_REG_DECLARE(0x0547);
    RFU_K7_R(0x0547);
    double frame_time = get_frame_time_ms();
    return frame_time * RFU_K7_REG(0x0547).hyper_frame_period;
}

int32_t sp2401_r1a::set_slot_time(double time_ms)
{
    RFU_K7_REG_DECLARE(0x0550);
    RFU_K7_REG(0x0550).slot_time = unsigned(round(time_ms * 245760, 0));
    RFU_K7_W(0x0550);
    return 0;
}

int32_t sp2401_r1a::set_trig_frame(uint32_t frame)
{
    RFU_K7_REG_DECLARE(0x0551);
    RFU_K7_REG(0x0551).frame = frame;
    RFU_K7_W(0x0551);
    return 0;
}

int32_t sp2401_r1a::set_trig_slot(uint32_t slot)
{
    RFU_K7_REG_DECLARE(0x0552);
    RFU_K7_REG(0x0552).slot = slot;
    RFU_K7_W(0x0552);
    return 0;
}

int32_t sp2401_r1a::set_frame_trig_src(frame_trig_src_t src)
{
    RFU_K7_REG_DECLARE(0x0553);
    RFU_K7_R(0x0553);
    RFU_K7_REG(0x0553).trig_src = src;
    RFU_K7_W(0x0553);
    return 0;
}

int32_t sp2401_r1a::set_frame_trig_mod_x_y(uint16_t x,uint16_t y)
{
    RFU_K7_REG_DECLARE(0x0556);
    RFU_K7_REG(0x0556).x = x;
    RFU_K7_REG(0x0556).y = y;
    RFU_K7_W(0x0556);
    return 0;
}

uint32_t sp2401_r1a::get_cur_frame()
{
    RFU_K7_REG_DECLARE(0x0554);
    RFU_K7_R(0x0554);
    return RFU_K7_REG(0x0554).frame;
}

uint32_t sp2401_r1a::get_cur_slot()
{
    RFU_K7_REG_DECLARE(0x0555);
    RFU_K7_R(0x0555);
    return RFU_K7_REG(0x0555).slot;
}

int32_t sp2401_r1a::set_tx_amplitude_balance(uint16_t I,uint16_t Q)
{
    RFU_K7_REG_DECLARE_2(0x0067,0x2067);
    RFU_K7_REG_2(0x0067,0x2067).i = I;
    RFU_K7_REG_2(0x0067,0x2067).q = Q;
    RFU_K7_W_2(0x0067,0x2067);
	return 0;
}

int32_t sp2401_r1a::get_iq_cap_src(iq_cap_src_t &src,bool &posedge)
{
    RFU_K7_REG_DECLARE_2(0x1062,0x1092);
    RFU_K7_R_2(0x1062,0x1092);
    src = iq_cap_src_t(RFU_K7_REG_2(0x1062,0x1092).iqcap_src_sel);
    posedge = RFU_K7_REG_2(0x1062,0x1092).iqcap_src_edge == 0 /*posedge ? 1 : 0*/;
    return 0;
}

int32_t sp2401_r1a::rx_filter_coef_default(double *real,double *imag,int32_t order)
{
    if (order == ul_filter_tap) {
        double real_def[ul_filter_tap] = {
            12,3,-7,-1,6,6,-8,-6,7,10,-8,-12,7,16,-6,-19,3,24,0,-26,-5,31,10,-33,
            -18,36,27,-36,-38,36,50,-34,-65,29,83,-20,-104,6,130,16,-165,-52,219,
            119,-323,-287,683,1769,1769,683,-287,-323,119,219,-52,-165,16,130,6,
            -104,-20,83,29,-65,-34,50,36,-38,-36,27,36,-18,-33,10,31,-5,-26,0,24,
            3,-19,-6,16,7,-12,-8,10,7,-6,-8,6,6,-1,-7,3,12
        };
        double imag_def[ul_filter_tap] = {
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        };
        for (int32_t i = 0;i < ul_filter_tap;i ++) {
            real[i] = real_def[i];
            imag[i] = imag_def[i];
        }
    }
    return 0;
}

int32_t sp2401_r1a::set_fpga_bit(const char *path)
{
    uint32_t file_size = 0;
    uint32_t cnt = 0;
    uint32_t to_cnt = 0;
    const uint32_t fpga_idx = (_rf_idx == 0 || _rf_idx == 1 ? 1 : 0);
    const uint32_t buffer_offset = (fpga_idx == 0 ? 0x0005 : 0x0009) * 4;
    const uint32_t block_size = 500; // * 32bit

    FILE *fp = fopen(path,"rb");
    if (fp == nullptr) {
        Log.set_last_err("Can't open %s",path);
		return -1;
	}

	fseek(fp,0,SEEK_END);
    file_size = uint32_t(ftell(fp));
    Log.stdprintf("File size before align = %d\n",file_size);
    file_size = (file_size / 4 + ((file_size % 4) ? 1 : 0)) * 4; //4 bytes align
    cnt = (file_size / 4 / block_size) + 1;                      //4 bytes once
    Log.stdprintf("File size after  align = %d\n",file_size);
	fseek(fp,0,SEEK_SET);

    uint32_t *buf = new uint32_t[file_size / 4];
    memset(buf,0,file_size / 4);
    fread(buf,1,file_size,fp);

    RFU_S6_REG_DECLARE_2(0x0002,0x0006);
    RFU_S6_REG_DECLARE_2(0x0003,0x0007);
    RFU_S6_REG_DECLARE_2(0x0004,0x0008);
    RFU_S6_REG_2(0x0004,0x0008).load_mode = 1;
    RFU_S6_W_2(0x0004,0x0008);

    Log.stdprintf("Set signal PROGRAM_B ---\\___/---\n");
    RFU_S6_REG_2(0x0002,0x0006).PROGRAM_B = 0;
    RFU_S6_W_2(0x0002,0x0006);
    RFU_S6_W_2(0x0002,0x0006);
    RFU_S6_W_2(0x0002,0x0006);
    RFU_S6_W_2(0x0002,0x0006);
    RFU_S6_W_2(0x0002,0x0006);
    RFU_S6_W_2(0x0002,0x0006);
    RFU_S6_W_2(0x0002,0x0006);
    RFU_S6_W_2(0x0002,0x0006);

    RFU_S6_REG_2(0x0002,0x0006).PROGRAM_B = 1;
    RFU_S6_W_2(0x0002,0x0006);

    Log.stdprintf("Waiting signal INIT_B ___/---...\n");
    to_cnt = 0;
    RFU_S6_R_2(0x0003,0x0007);

    while(RFU_S6_REG_2(0x0003,0x0007).pc_cfg_init == 0) {
        RFU_S6_R_2(0x0003,0x0007);
        if (++to_cnt > 1000) {
            Log.stdprintf(" Waiting signal INIT_B ___/--- timeout\n");
			break;
        }
	}

    Log.stdprintf("Prepare programming %s\n",1 == fpga_idx ? "K7_1" : "K7_0" );
    Log.stdprintf("Waiting FIFO empty...\n");
    RFU_S6_WAIT_FIFO_EMPTY_2(0x0003,0x0007,0,1000);

    Log.stdprintf("Waiting signal DONE ---\\___...\n");
    to_cnt = 0;
    RFU_S6_R_2(0x0003,0x0007);
    while (RFU_S6_REG_2(0x0003,0x0007).c_cfg_done != 0) {
        RFU_S6_R_2(0x0003,0x0007);
        if (++to_cnt > 1000) {
            Log.stdprintf("Waiting signal DONE ---\\___ timeout\n");
            break;
        }
    }

    Log.stdprintf("Downloading...\n");

    for (uint32_t i = 0;i < cnt;i ++) {
        if (i == cnt - 1) {
            _s6->w32(pci_dev::AS_BAR0,buffer_offset,(file_size - i * 4 * block_size) / 4,&buf[i * block_size]);
        } else {
            _s6->w32(pci_dev::AS_BAR0,buffer_offset,block_size,&buf[i * block_size]);
        }

        RFU_S6_WAIT_FIFO_EMPTY_2(0x0003,0x0007,0,1000);

        Log.stdprintf("%-5d Done\r",i);
        sleep_ms(1);
	}

    RFU_S6_R_2(0x0003,0x0007);
    Log.stdprintf("\n");
    delete []buf;
	fclose(fp);

    if (RFU_S6_REG_2(0x0003,0x0007).c_cfg_done == 1) {
#ifdef RD_PLATFORM_WIN32
        sleep_ms(10);
        Log.stdprintf("Restarting...\n");
        system("devcon_x64.exe Restart \"PCI\\VEN_10EE&DEV_7021\"");
        sleep_ms(1000);
        Log.stdprintf("Restarted\n");
#elif defined RD_PLATFORM_LINUX
        Log.stdprintf("download success\n");
#endif
		return 0;
    } else {
        Log.set_last_err("Download complete,but device start fail");
        Log.stdprintf("Download fail\n");
		return -1;
	}
}

int32_t sp2401_r1a::set_frame_trig_offset(int32_t offset)
{
    RFU_K7_REG_DECLARE(0x10ca);
    RFU_K7_R(0x10ca);
    RFU_K7_REG(0x10ca).trigg_offset = unsigned(offset);
    RFU_K7_W(0x10ca);
    return 0;
}
