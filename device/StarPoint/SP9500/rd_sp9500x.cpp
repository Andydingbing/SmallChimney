#include "rd_sp9500x.h"
#include "liblog.h"
#include "global.hpp"
#include "reg_def_sp9500x.h"
#include "sp9500x_sp3103.h"
#include "sleep_common.h"
#include <vector>

#define SAFE_RF_INDEX (RFIndex < 2 ? RFIndex : 1)
#define DECL_DYNAMIC_SP3103 uint32_t rf_idx = (RFIndex < 2 ? 0 : 1);
#define DECL_DYNAMIC_SP1403 rd::ns_sp9500x::sp1403 *SP1403 = SP3103_0.working_sp1403(rf_idx);
#define DECL_DYNAMIC_SP2406 rd::ns_sp9500x::sp2406 *SP2406 = SP3103_0.working_sp2406(rf_idx);

using namespace std;
using namespace rd;
using namespace rd::ns_sp2406;
using namespace rd::ns_sp9500x;

static double g_rx_ref[2] = { 0.0,0.0 };

RD_SP9500X_IQ_Capture_Param::RD_SP9500X_IQ_Capture_Param()
{
    TriggerType = 0;
    RadioFrameCondition_X = 0;
    RadioFrameCondition_Y = 0;
    TriggerOffset = 0;
    MeasLength = 0;
    Channel = 0;
    SampleRate = SP9500X_UL_SR_98304;
}

RD_SP9500X_CA_Map::RD_SP9500X_CA_Map()
{
    memset(this,0,sizeof(*this));
}

int32_t SP9500X_RF_SetBitPath(const char *Path)
{
    SP3103_0.set_program_rfu_v9(string(Path));
    SP3103_0.set_program_bbu_v9(string(Path));
    return 0;
}

int32_t SP9500X_RF_Boot()
{
    Log.en(log_t::RD_LOG_ALL_OFF, true);
    Log.en(log_t::RD_LOG_PROMPT, true);

    SP3103_0.boot(true);
    SP3103_0.program_bbu_v9();
    SP3103_0.program_rfu_v9();
    SP3103_0.boot(false);

    SP9500X_SetFans(120);
    return 0;
}

int32_t SP9500X_RF_GetSerialNumber(const uint32_t RFIndex,char *SerialNumber)
{
    return 0;
}

int32_t SP9500X_RF_IsRRHConnected(const uint32_t RFIndex,bool &Status)
{
    DECL_DYNAMIC_SP3103;
    Status = SP3103_0.working_rrh(rf_idx)->is_connected();
    return 0;
}

int32_t SP9500X_RF_SetConnector(const uint32_t RFIndex,const RD_IO_Mode Connector)
{
    DECL_DYNAMIC_SP3103;
    return SP3103_0.set_io_mode(rf_idx,io_mode_t(Connector));
}

int32_t SP9500X_RF_SetTxState(const uint32_t RFIndex,const bool State)
{
    return 0;
}

int32_t SP9500X_RF_SetTxPower(const uint32_t RFIndex,const float Power)
{
    DECL_DYNAMIC_SP3103;
    return SP3103_0.set_tx_pwr(rf_idx,double(Power));
}

int32_t SP9500X_RF_SetTxFrequency(const uint32_t RFIndex,const uint64_t Freq)
{
    DECL_DYNAMIC_SP3103;
    return SP3103_0.set_tx_freq(rf_idx,Freq);
}

int32_t SP9500X_RF_GetTxFrequency(const uint32_t RFIndex,uint64_t &Freq)
{
    DECL_DYNAMIC_SP3103;
    return SP3103_0.get_tx_freq(rf_idx,Freq);
}

int32_t SP9500X_RF_SetRxLevel(const uint32_t RFIndex,const double Level)
{
    DECL_DYNAMIC_SP3103;
    INT_CHECK(SP3103_0.set_rx_ref(rf_idx,Level));
    g_rx_ref[SAFE_RF_INDEX] = Level;
    return 0;
}

int32_t SP9500X_RF_GetRxLevel(const uint32_t RFIndex,double &Level)
{
    Level = g_rx_ref[SAFE_RF_INDEX];
    return 0;
}

int32_t SP9500X_RF_SetRxFrequency(const uint32_t RFIndex,const uint64_t Freq)
{
    DECL_DYNAMIC_SP3103;
    return SP3103_0.set_rx_freq(rf_idx,Freq);
}

int32_t SP9500X_RF_GetRxFrequency(const uint32_t RFIndex,uint64_t &Freq)
{
    DECL_DYNAMIC_SP3103;
    return SP3103_0.get_rx_freq(rf_idx,Freq);
}

int32_t SP9500X_RF_SetRxFrequencyShift(const uint32_t RFIndex,const int64_t Freq)
{
    DECL_DYNAMIC_SP3103;
    DECL_DYNAMIC_SP1403;

    pci_dev_vi *_v9 = SP3103_0.v9();

    RFU_V9_REG_DECL(0x0462);
    RFU_V9_R(0x0462);
    INT_CHECK(SP1403->set_rx_freq(uint64_t(int64_t(FREQ_M(7500)) - Freq)));
    RFU_V9_W(0x0462);
    return 0;
}

RD_INLINE bool is_ca_carrier_valid(const RD_SP9500X_CA_Carrier &carrier)
{
    if (carrier.Channel % 2 == 1) {
        if (carrier.Bandwidth > SP9500X_CA_100M) {
            return false;
        }
    }

    if (carrier.Channel == 2 || carrier.Channel == 6) {
        if (carrier.Bandwidth > SP9500X_CA_200M) {
            return false;
        }
    }
    return true;
}

bool is_ca_map_valid(const RD_SP9500X_CA_Map &map)
{
    for (uint8_t i = 0;i < ARRAY_SIZE(map.Carrier);++i) {
        if (map.Carrier[i].Bandwidth == SP9500X_CA_Invalid) {
            continue;
        }

        if (is_ca_carrier_valid(map.Carrier[i]) == false) {
            return false;
        }
    }
    return true;
}

int32_t SP9500X_RF_SetTxCAMap(const uint32_t RFIndex,const RD_SP9500X_CA_Map &Map)
{
    DECL_DYNAMIC_SP3103;
    DECL_DYNAMIC_SP2406;

    uint8_t ch = 0;
    int64_t freq = 0;
    double freq_min_ch_0_3 =  400.0 + 1;
    double freq_max_ch_0_3 = -400.0 - 1;
    double freq_min_ch_4_7 =  400.0 + 1;
    double freq_max_ch_4_7 = -400.0 - 1;
    double freq_dds1 = 0.0;

    vector<const RD_SP9500X_CA_Carrier *> carrier_ch_0_3;
    vector<const RD_SP9500X_CA_Carrier *> carrier_ch_4_7;
    vector<const RD_SP9500X_CA_Carrier *>::iterator iter;

    for (uint8_t i = 0;i < ARRAY_SIZE(Map.Carrier);++i) {
        if (Map.Carrier[i].Bandwidth == SP9500X_CA_Invalid) {
            continue;
        }

        INT_CHECK(SP2406->set_dl_src(Map.Carrier[i].Channel,dl_src_t::CPRI));

        freq = Map.Carrier[i].Freq;
        if (Map.Carrier[i].Channel < 4) {
            freq_min_ch_0_3 = freq < freq_min_ch_0_3 ? freq : freq_min_ch_0_3;
            freq_max_ch_0_3 = freq > freq_max_ch_0_3 ? freq : freq_max_ch_0_3;
            carrier_ch_0_3.push_back(&Map.Carrier[i]);
        } else {
            freq_min_ch_4_7 = freq < freq_min_ch_4_7 ? freq : freq_min_ch_4_7;
            freq_max_ch_4_7 = freq > freq_max_ch_4_7 ? freq : freq_max_ch_4_7;
            carrier_ch_4_7.push_back(&Map.Carrier[i]);
        }
    }

    if (carrier_ch_0_3.size() > 0) {
        freq_dds1 = (freq_min_ch_0_3 + freq_max_ch_0_3) / 2.0;
        INT_CHECK(SP2406->set_dl_dds1_ch0_3(freq_dds1));

        for (iter = carrier_ch_0_3.begin();iter != carrier_ch_0_3.end();++iter) {
            ch = (*iter)->Channel;

            INT_CHECK(SP2406->set_dl_cpri_sr(ch,dl_cpri_sr_t::_from_index((*iter)->Bandwidth - 1)));
            INT_CHECK(SP2406->set_dl_hbf0_en(ch,(*iter)->Bandwidth == SP9500X_CA_100M));
            INT_CHECK(SP2406->set_dl_hbf1_en(ch,(*iter)->Bandwidth <= SP9500X_CA_200M));
            INT_CHECK(SP2406->set_dl_dds0(ch,(*iter)->Freq - freq_dds1));
        }
    }

    if (carrier_ch_4_7.size() > 0) {
        freq_dds1 = (freq_min_ch_4_7 + freq_max_ch_4_7) / 2.0;
        INT_CHECK(SP2406->set_dl_dds1_ch4_7(freq_dds1));

        for (iter = carrier_ch_4_7.begin();iter != carrier_ch_4_7.end();++iter) {
            ch = (*iter)->Channel;

            INT_CHECK(SP2406->set_dl_cpri_sr(ch,dl_cpri_sr_t::_from_index((*iter)->Bandwidth - 1)));
            INT_CHECK(SP2406->set_dl_hbf0_en(ch,(*iter)->Bandwidth == SP9500X_CA_100M));
            INT_CHECK(SP2406->set_dl_hbf1_en(ch,(*iter)->Bandwidth <= SP9500X_CA_200M));
            INT_CHECK(SP2406->set_dl_dds0(ch,(*iter)->Freq - freq_dds1));
        }
    }

    return 0;
}

int32_t SP9500X_RF_SetRxCAMap(const uint32_t RFIndex,const RD_SP9500X_CA_Map &Map)
{
    DECL_DYNAMIC_SP3103;
    DECL_DYNAMIC_SP2406;

    uint8_t ch = 0;
    int64_t freq = 0;
    double freq_min_ch_0_3 =  400.0 + 1;
    double freq_max_ch_0_3 = -400.0 - 1;
    double freq_min_ch_4_7 =  400.0 + 1;
    double freq_max_ch_4_7 = -400.0 - 1;
    double freq_ddc1 = 0.0;

    vector<const RD_SP9500X_CA_Carrier *> carrier_ch_0_3;
    vector<const RD_SP9500X_CA_Carrier *> carrier_ch_4_7;
    vector<const RD_SP9500X_CA_Carrier *>::iterator iter;

    for (uint8_t i = 0;i < ARRAY_SIZE(Map.Carrier);++i) {
        if (Map.Carrier[i].Bandwidth == SP9500X_CA_Invalid) {
            continue;
        }

        freq = Map.Carrier[i].Freq;
        if (Map.Carrier[i].Channel < 4) {
            freq_min_ch_0_3 = freq < freq_min_ch_0_3 ? freq : freq_min_ch_0_3;
            freq_max_ch_0_3 = freq > freq_max_ch_0_3 ? freq : freq_max_ch_0_3;
            carrier_ch_0_3.push_back(&Map.Carrier[i]);
        } else {
            freq_min_ch_4_7 = freq < freq_min_ch_4_7 ? freq : freq_min_ch_4_7;
            freq_max_ch_4_7 = freq > freq_max_ch_4_7 ? freq : freq_max_ch_4_7;
            carrier_ch_4_7.push_back(&Map.Carrier[i]);
        }
    }

    if (carrier_ch_0_3.size() > 0) {
        freq_ddc1 = (freq_min_ch_0_3 + freq_max_ch_0_3) / 2.0;
        INT_CHECK(SP2406->set_ul_ddc0_ch0_3(freq_ddc1));

        for (iter = carrier_ch_0_3.begin();iter != carrier_ch_0_3.end();++iter) {
            ch = (*iter)->Channel;

            INT_CHECK(SP2406->set_ul_hbf1_en(ch,(*iter)->Bandwidth <= SP9500X_CA_200M));
            INT_CHECK(SP2406->set_ul_hbf2_en(ch,(*iter)->Bandwidth == SP9500X_CA_100M));
            INT_CHECK(SP2406->set_ul_ddc1(ch,(*iter)->Freq - freq_ddc1));
        }
    }

    if (carrier_ch_4_7.size() > 0) {
        freq_ddc1 = (freq_min_ch_4_7 + freq_max_ch_4_7) / 2.0;
        INT_CHECK(SP2406->set_ul_ddc0_ch4_7(freq_ddc1));

        for (iter = carrier_ch_4_7.begin();iter != carrier_ch_4_7.end();++iter) {
            ch = (*iter)->Channel;

            INT_CHECK(SP2406->set_ul_hbf1_en(ch,(*iter)->Bandwidth <= SP9500X_CA_200M));
            INT_CHECK(SP2406->set_ul_hbf2_en(ch,(*iter)->Bandwidth == SP9500X_CA_100M));
            INT_CHECK(SP2406->set_ul_ddc1(ch,(*iter)->Freq - freq_ddc1));
        }
    }

    return 0;
}

int32_t SP9500X_RF_LoadARBSource(const uint32_t RFIndex,const char *Path)
{
    DECL_DYNAMIC_SP3103;
    return SP3103_0.arb_load(rf_idx,ns_arb::src_t::File,Path);
}

int32_t SP9500X_RF_SetARBEnable(const uint32_t RFIndex,const bool State)
{
    DECL_DYNAMIC_SP3103;
    return SP3103_0.set_arb_en(rf_idx,State);
}

int32_t SP9500X_RF_SetARBCount(const uint32_t RFIndex,const int Count)
{
    return 0;
}

int32_t SP9500X_RF_SetIQCaptureBuffer(const uint32_t RFIndex,int16_t **I,int16_t **Q)
{
    DECL_DYNAMIC_SP3103;

    *I = SP3103_0.working_sp2406(rf_idx)->ul_sequence()->i();
    *Q = SP3103_0.working_sp2406(rf_idx)->ul_sequence()->q();
    return 0;
}

int32_t SP9500X_RF_SetIQCaptureParams(const uint32_t RFIndex,const RD_SP9500X_IQ_Capture_Param &Param)
{
    DECL_DYNAMIC_SP3103;
    DECL_DYNAMIC_SP2406;

    iq_cap_src_ddc_t cap_src = iq_cap_src_ddc_t::CF;
    iq_cap_trig_src_t trig_src = iq_cap_trig_src_t::Frame;
    int32_t trig_offset = Param.TriggerOffset;

    if (Param.SampleRate == SP9500X_UL_SR_98304) {
        cap_src = iq_cap_src_ddc_t::CF;
        trig_offset /= 2;
    } else if (Param.SampleRate == SP9500X_UL_SR_49152) {
        cap_src = iq_cap_src_ddc_t::HBF0;
    } else if (Param.SampleRate == SP9500X_UL_SR_24576) {
        cap_src = iq_cap_src_ddc_t::HBF1;
    } else {
        cap_src = iq_cap_src_ddc_t::LPF;
    }

    if (Param.TriggerType == 0) {
        trig_src = ns_sp2406::iq_cap_trig_src_t::NextFrame;
    } else if (Param.TriggerType == 1) {
        trig_src = ns_sp2406::iq_cap_trig_src_t::Frame;
    } else {
        trig_src = ns_sp2406::iq_cap_trig_src_t::ModXY;
        INT_CHECK(SP2406->set_iq_cap_trig_frame_x(uint16_t(Param.RadioFrameCondition_X)));
        INT_CHECK(SP2406->set_iq_cap_trig_frame_y(uint16_t(Param.RadioFrameCondition_Y)));
    }

    INT_CHECK(SP2406->set_iq_cap_sr(iq_cap_sr_t::_from_index(Param.SampleRate)));
    INT_CHECK(SP2406->set_iq_cap_src_ddc_ch(uint8_t(Param.Channel)));
    INT_CHECK(SP2406->set_iq_cap_src_ddc(cap_src));

    INT_CHECK(SP2406->set_iq_cap_samples(uint32_t(Param.MeasLength)));
    INT_CHECK(SP2406->set_iq_cap_trig_src(trig_src));
    INT_CHECK(SP2406->set_iq_cap_trig_offset(trig_offset));
    return 0;
}

int32_t SP9500X_RF_SetIQCaptureStart(const uint32_t RFIndex)
{
    DECL_DYNAMIC_SP3103;
    DECL_DYNAMIC_SP2406;

    SP2406->set_iq_cap_start();
    return 0;
}

int32_t SP9500X_RF_SetIQCaptureAbort(const uint32_t RFIndex)
{
    DECL_DYNAMIC_SP3103;
    DECL_DYNAMIC_SP2406;

    return SP2406->set_iq_cap_abort();
}

int32_t SP9500X_HardResetChips()
{
    RFU_S6_REG_DECL(0x0002);

    pci_dev_vi *_s6 = SP3103_0.s6();

    RFU_S6_OP(0x0002);

    RFU_S6_REG(0x0002).op = 0;
    RFU_S6_W(0x0002);
    RFU_S6_REG(0x0002).op = 1;
    RFU_S6_W(0x0002);
    sleep_ms(2);
    RFU_S6_REG(0x0002).op = 0;
    RFU_S6_W(0x0002);
    return 0;
}

int32_t SP9500X_SoftResetChips()
{
    RFU_S6_REG_DECL(0x0004);

    pci_dev_vi *_s6 = SP3103_0.s6();

    RFU_S6_OP(0x0004);
    RFU_S6_REG(0x0004).op = 0;
    RFU_S6_W(0x0004);
    return 0;
}

int32_t SP9500X_ZU21_ConfigStatus(bool &done)
{
    RFU_S6_REG_DECL(0x0120);

    pci_dev_vi *_s6 = SP3103_0.s6();

    RFU_S6_R(0x0120);
    done = RFU_S6_REG(0x0120).cfg_done == 1;
    return 0;
}

int32_t SP9500X_JESD_Reset(const RD_SP9500X_JESD jesd)
{
    RFU_V9_REG_DECL(0x0440);

    pci_dev_vi *_v9 = SP3103_0.v9();

    RFU_V9_REG_DATA(0x0440) = jesd;
    RFU_V9_W(0x0440);
    RFU_V9_REG_DATA(0x0440) = 0;
    RFU_V9_W(0x0440);
    return 0;
}

int32_t SP9500X_AD998X_Status(bool &status)
{
    return SP3103_0.working_sp2406(0)->jesd_status(status);
}


int32_t SP9500X_SetFan(const uint32_t FanIndex,const uint32_t Speed)
{
    RFU_V9_REG_DECL(0x0480);
    RFU_V9_REG_DECL(0x0481);
    RFU_V9_REG_DECL(0x0482);
    RFU_V9_REG_DECL(0x0485);
    RFU_V9_REG_DECL(0x0486);
    RFU_V9_REG_DECL(0x0487);

    pci_dev_vi *_v9 = SP3103_0.v9();

    RFU_V9_REG(0x0480).mode = 1;
    RFU_V9_REG(0x0485).mode = 1;
    RFU_V9_W(0x0480);
    RFU_V9_W(0x0485);

    if (FanIndex < 12) {
        RFU_V9_REG(0x0482).reg = 0x32 + FanIndex % 4;

        if (FanIndex < 4) {
            RFU_V9_REG(0x0482).id = 0x2f;
        } else if (FanIndex < 8) {
            RFU_V9_REG(0x0482).id = 0x2e;
        } else {
            RFU_V9_REG(0x0482).id = 0x2c;
        }

        RFU_V9_REG(0x0482).speed = Speed;
        RFU_V9_W(0x0482);
        RFU_V9_OP(0x0481);
    } else {
        RFU_V9_REG(0x0487).reg = 0x32 + FanIndex % 4;
        RFU_V9_REG(0x0487).id  = 0x2f;

        RFU_V9_REG(0x0487).speed = Speed;
        RFU_V9_W(0x0487);
        RFU_V9_OP(0x0486);
    }
    sleep_ms(10);
    return 0;
}

int32_t SP9500X_SetFans(const uint32_t Speed)
{
    for (uint32_t i = 0;i < g_max_fan;++i) {
        INT_CHECK(SP9500X_SetFan(i,Speed));
    }
    return 0;
}
