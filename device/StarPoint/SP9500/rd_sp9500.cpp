#include "rd_sp9500.h"
#include "liblog.h"
#include "sp3301.h"
#include "sp3501.h"
#include "reg_def.h"
#include "complex_sequence.h"
#include "gen_ini_file.hpp"
#include <vector>

using namespace std;
using namespace ns_sp9500;
using namespace ns_sp1401;

typedef struct available_rf_board_t {
    sp3301  *m_sp3301; // The RFU
    uint32_t m_rf_idx; // The RF index in particular RFU
} spec_rf_board_t;

static const uint32_t max_mimo_ports = ARRAY_SIZE(IQ_Capture_Index::Idx);
static vector<available_rf_board_t> g_rf_board;

#define DECL_DYNAMIC_SP3301 \
    if (RFIndex > g_rf_board.size() - 1) \
        return -1; \
    sp3301 *SP3301 = g_rf_board[RFIndex].m_sp3301; \
    uint32_t rf_idx = g_rf_board[RFIndex].m_rf_idx;

IQ_Capture_Param::IQ_Capture_Param()
{
    TriggerType = 0;
    RadioFrameCondition_X = 0;
    RadioFrameCondition_Y = 0;
    TriggerOffset = 0;
    MeasLength = 0;
}

IQ_Capture_Index::IQ_Capture_Index()
{
    for (uint32_t i = 0;i < max_mimo_ports;i ++) {
        Idx[i] = false;
    }
}

int32_t RF_SetBitPath(char *Path)
{
    INT_CHECK(SP3301_2.set_program_bit(Path));
    INT_CHECK(SP3301_3.set_program_bit(Path));
    return 0;
}

static uint32_t g_samples[4]={0};
static double g_RxLevel[4]={0};

int32_t RF_Boot()
{
    Log.en(log_t::RD_LOG_ALL_OFF, true);
    Log.en(log_t::RD_LOG_PROMPT, true);

    g_rf_board.clear();
    available_rf_board_t rf_board;
    sp3301::active_t RfuActiveInfo[g_max_rfu];

    SP3301_2.boot();
    SP3301_3.boot();
    SP3301_2.program_bit();
    SP3301_3.program_bit();
    SP3301_2.boot();
    SP3301_3.boot();

    uint16_t ocxo_value = 0;
    SP3301_2.get_ocxo(ocxo_value);
    Log.stdprintf("ocxo : %d\n",ocxo_value);
    SP3501.open_board();
    SP3501.vol_9119(ocxo_value);

    RfuActiveInfo[2] = SP3301_2.is_actived();
    RfuActiveInfo[3] = SP3301_3.is_actived();

    for (int i = g_max_rf - 1;i >= 0;i --) {
        if (RfuActiveInfo[2].sp1401[i]) {
            rf_board.m_sp3301 = &SP3301_2;
            rf_board.m_rf_idx = i;
            g_rf_board.push_back(rf_board);
        }
    }
    for (int i = g_max_rf - 1;i >= 0;i --) {
        if (RfuActiveInfo[3].sp1401[i]) {
            rf_board.m_sp3301 = &SP3301_3;
            rf_board.m_rf_idx = i;
            g_rf_board.push_back(rf_board);
        }
    }
    return 0;
}

int32_t RF_GetRFUNumber(uint32_t &uiRFUNumber)
{
    uiRFUNumber = 0;
    sp3301::rfu_info_t RfuInfo[g_max_rf];
    vector<string> strRsrcRfu;
    vector<string>::iterator iterRsrcRfu;
    pci_dev_vi viDev;

    char szProductForm[64];
    char szRsrcSection[64];
    char szDevKey[64];
    memset(szProductForm,0,ARRAY_SIZE(szProductForm));
    memset(szRsrcSection,0,ARRAY_SIZE(szRsrcSection));
    memset(szDevKey,0,ARRAY_SIZE(szDevKey));

    gen_ini_file IniFile(CONFIG_FILE_PATH);

    bool bK7_0_Active[g_max_rfu],bK7_1_Active[g_max_rfu],bS6_Active[g_max_rfu];
    for (int32_t i = 0;i < g_max_rfu;i ++) {
        bK7_0_Active[i] = false;
        bK7_1_Active[i] = false;
        bS6_Active[i] = false;
    }

    IniFile.get_config_str_value("Product Form","Form",szProductForm);
    strcpy(szRsrcSection,"RESOURCE");
    strcat(szRsrcSection,szProductForm);
    for (int32_t i = 0;i < g_max_rfu;i ++) {
        sprintf(szDevKey,"RFU%d_K7_0",i);
        IniFile.get_config_str_value(szRsrcSection,szDevKey,RfuInfo[i].k7_0);
        sprintf(szDevKey,"RFU%d_K7_1",i);
        IniFile.get_config_str_value(szRsrcSection,szDevKey,RfuInfo[i].k7_1);
        sprintf(szDevKey,"RFU%d_S6",i);
        IniFile.get_config_str_value(szRsrcSection,szDevKey,RfuInfo[i].s6);
    }

    viDev.get_devs(strRsrcRfu);
    for (int32_t i = 0;i < g_max_rfu;i ++) {
        for (iterRsrcRfu = strRsrcRfu.begin();iterRsrcRfu != strRsrcRfu.end();iterRsrcRfu ++ ) {
            if (0 == strcmp(RfuInfo[i].k7_0,iterRsrcRfu->c_str()))
                bK7_0_Active[i] = true;
            if (0 == strcmp(RfuInfo[i].k7_1,iterRsrcRfu->c_str()))
                bK7_1_Active[i] = true;
            if (0 == strcmp(RfuInfo[i].s6,iterRsrcRfu->c_str()))
                bS6_Active[i] = true;
        }
        if (bK7_0_Active[i] == true || bK7_1_Active[i] == true || bS6_Active[i] == true) {
            uiRFUNumber ++;
        }
    }
    return 0;
}

int32_t RF_GetRFPortNumber(uint32_t &uiRFPortNumber)
{
    uiRFPortNumber = SP3301_2.get_rf_port() + SP3301_3.get_rf_port();
    return 0;
}

int32_t RF_GetRFSerialNumber(uint32_t RFIndex, char *SerialNumber)
{
    DECL_DYNAMIC_SP3301
    INT_CHECK(SP3301->get_rf_sn(rf_idx,SerialNumber));
    return 0;
}

int32_t RF_GetRFUSerialNumber(uint32_t RFUIndex,char *SerialNumber)
{
    switch (RFUIndex) {
// 		case 0 : {SP3301_0.get_sn(SerialNumber);break;}
// 		case 1 : {SP3301_1.get_sn(SerialNumber);break;}
        case 2 : {SP3301_2.get_sn(SerialNumber);break;}
        case 3 : {SP3301_3.get_sn(SerialNumber);break;}
//		case 4 : {SP3301_4.get_sn(SerialNumber);break;}
        default:break;
    }
    return 0;
}

int32_t RF_GetRFDriverVersion(const char **version)
{
    *version = SP3301_2.get_driver_ver();
    return 0;
}

int32_t RF_GetRFUVersion(char *version)
{
    SP3301_2.get_ver(version);
    return 0;
}

int32_t RF_SetTxState(uint32_t RFIndex,bool State)
{
    DECL_DYNAMIC_SP3301;
    INT_CHECK(SP3301->set_tx_state(rf_idx, State));
    return 0;
}

int32_t RF_SetTxPower(uint32_t RFIndex,float Power)
{
    DECL_DYNAMIC_SP3301;
    INT_CHECK(SP3301->set_tx_pwr(rf_idx,Power));
    return 0;
}

int32_t RF_SetTxFrequency(uint32_t RFIndex,uint64_t Freq)
{
    DECL_DYNAMIC_SP3301;
    INT_CHECK(SP3301->set_tx_freq(rf_idx,Freq));
    return 0;
}

int32_t RF_GetTxFrequency(uint32_t RFIndex,uint64_t &Freq)
{
    DECL_DYNAMIC_SP3301;
    INT_CHECK(SP3301->get_tx_freq(rf_idx,Freq));
    return 0;
}

int32_t RF_SetTxSource(uint32_t RFIndex,SOURCE Source)
{
    DECL_DYNAMIC_SP3301;
    sp2401_r1a::da_src_t TxSrc = sp2401_r1a::INTER_FILTER;
    sp1401::iq_cap_src_t ddr_src = sp1401::PWR_MEAS_FREE_RUN;

    switch (Source) {
        case ARB  : {
            TxSrc = sp2401_r1a::DRIVER_ARB;
            ddr_src = sp1401::PWR_MEAS_FREE_RUN;
            INT_CHECK(SP3301->set_iq_cap_trig_src(rf_idx,ddr_src));
            INT_CHECK(SP3301->set_iq_cap_trig_src(brother_idx(rf_idx),ddr_src));
            break;
        }
        case FPGA : {TxSrc = sp2401_r1a::INTER_FILTER;break;}
        case CW   : {TxSrc = sp2401_r1a::SINGLE_TONE;break;}
    }
    INT_CHECK(SP3301->set_tx_src(rf_idx,TxSrc));
    return 0;
}

int32_t RF_SetSourceFrequency(uint32_t RFIndex,uint64_t Freq)
{
    DECL_DYNAMIC_SP3301;
    INT_CHECK(SP3301->set_src_freq(rf_idx,Freq));
    return 0;
}

int32_t RF_LoadARBSource(uint32_t RFIndex,char *filename)
{
    DECL_DYNAMIC_SP3301;
    INT_CHECK(SP3301->arb_load(rf_idx,filename));
    return 0;
}

int32_t RF_SetARBEnable(uint32_t RFIndex,bool bState)
{
    DECL_DYNAMIC_SP3301;
    INT_CHECK(SP3301->set_arb_en(rf_idx,bState));
    return 0;
}

int32_t RF_SetARBTriggerSource(uint32_t RFIndex,ARB_TRIGGERMODE Source)
{
    return 0;
}

int32_t RF_ExcuteARBTrigger(uint32_t RFIndex)
{
    return 0;
}

int32_t RF_SetARBCount(uint32_t RFIndex,int iCnt)
{
    return 0;
}

int32_t RF_SetRxLevel(uint32_t RFIndex,double Level)
{
    DECL_DYNAMIC_SP3301;
    INT_CHECK(SP3301->set_rx_level(rf_idx,Level));
    g_RxLevel[RFIndex]=Level;
    return 0;
}

int32_t RF_GetRxLevel(uint32_t RFIndex,double &Level)
{
    Level = g_RxLevel[RFIndex];
    return 0;
}

int32_t RF_SetRxFrequency(uint32_t RFIndex,uint64_t Freq)
{
    DECL_DYNAMIC_SP3301;
    INT_CHECK(SP3301->set_rx_freq(rf_idx,Freq));
    return 0;
}

int32_t RF_GetRxFrequency(uint32_t RFIndex,uint64_t &Freq)
{
    DECL_DYNAMIC_SP3301;
    INT_CHECK(SP3301->get_rx_freq(rf_idx,Freq));
    return 0;
}

int32_t RF_SetConnector(uint32_t RFIndex,CONNECTOR Connector)
{
    DECL_DYNAMIC_SP3301;
    io_mode_t Mode = io_mode_t::OUTPUT;
    switch (Connector) {
        case ::IO     : {Mode = io_mode_t::IO;break;}
        case ::OUTPUT : {Mode = io_mode_t::OUTPUT;break;}
        case ::LOOP   : {Mode = io_mode_t::LOOP;break;}
        default:break;
    }
    INT_CHECK(SP3301->set_io_mode(rf_idx,Mode));
    return 0;
}

int32_t RF_SetClockSource(uint32_t RFIndex,RFU_CLOCKSOURCE ClockSource)
{
    return 0;
}

int32_t RF_SetTriggerSource(uint32_t RFIndex,RFU_TRIGGERSOURCE TriggerSource)
{
    return 0;
}

int32_t RF_SetTriggerMode(uint32_t RFIndex,TRIGGERMODE TriggerMode)
{
    DECL_DYNAMIC_SP3301;
    sp1401::iq_cap_src_t MeasSrc = sp1401::PWR_MEAS_FREE_RUN;
    switch (TriggerMode) {
        case IF		 : {MeasSrc = sp1401::PWR_MEAS_IF_PWR;break;}
        case FREERUN : {MeasSrc = sp1401::PWR_MEAS_FREE_RUN;break;}
        case MARKER  : {break;}
        default:break;
    }
    INT_CHECK(SP3301->set_iq_cap_trig_src(rf_idx,MeasSrc));
    return 0;
}

int32_t RF_SetTriggerLevel(uint32_t RFIndex,float TriggerLevel)
{
    DECL_DYNAMIC_SP3301;
    INT_CHECK(SP3301->set_iq_cap_trig_level(rf_idx,TriggerLevel));
    return 0;
}

int32_t RF_SetTriggerOffset(uint32_t RFIndex,uint32_t Offset)
{
    return 0;
}


int32_t RF_InitPowerMeasure(uint32_t RFIndex)
{
    DECL_DYNAMIC_SP3301;
    INT_CHECK(SP3301->init_pwr_meas(rf_idx));
    return 0;
}

int32_t RF_AbortPowerMeasure(uint32_t RFIndex)
{
    DECL_DYNAMIC_SP3301;
    INT_CHECK(SP3301->abort_pwr_meas(rf_idx));
    return 0;
}

int32_t RF_GetMeasProcess(uint32_t RFIndex,PROCESS &Process)
{
    DECL_DYNAMIC_SP3301;
    Process = IDLE_Driver;
    sp1401::pwr_meas_state_t MeasState = sp1401::PMS_IDLE;
    INT_CHECK(SP3301->get_pwr_meas_proc(rf_idx,MeasState));
    switch (MeasState) {
        case sp1401::PMS_IDLE     : {Process = IDLE_Driver;break;}
        case sp1401::PMS_WFT      : {Process = WFTrigger_Driver;break;}
        case sp1401::PMS_TT       : {Process = Timeout_Driver;break;}
        case sp1401::PMS_RUNNING  : {Process = RUNNING_Driver;break;}
        case sp1401::PMS_DONE     : {Process = DONE_Driver;break;}
        default:break;
    }
    return 0;
}

int32_t RF_GetMeasResult(uint32_t RFIndex,float &Power,float &Crest)
{
    DECL_DYNAMIC_SP3301
    INT_CHECK(SP3301->get_pwr_meas_result(rf_idx,Power,Crest));
    return 0;
}

int32_t RF_SetIQCaptureBuffer(uint32_t RFIndex,int16_t *I,int16_t *Q)
{
    DECL_DYNAMIC_SP3301
    INT_CHECK(SP3301->set_iq_cap_buffer(rf_idx,I,Q));
    return 0;
}

int32_t RF_SetIQCaptureParams(uint32_t RFIndex,IQ_Capture_Param Param)
{
    DECL_DYNAMIC_SP3301

    INT_CHECK(SP3301->set_iq_cap_frame_trig_src(rf_idx,sp2401_r1a::frame_trig_src_t(Param.TriggerType)));
    INT_CHECK(SP3301->set_iq_cap_frame_trig_mod_x_y(rf_idx,Param.RadioFrameCondition_X,Param.RadioFrameCondition_Y));
    INT_CHECK(SP3301->set_iq_cap_frame_trig_offset(rf_idx,Param.TriggerOffset));
    INT_CHECK(SP3301->set_iq_cap_samples(rf_idx,Param.MeasLength));
    return 0;
}

int32_t RF_SetIQCaptureStart(uint32_t RFIndex)
{
    DECL_DYNAMIC_SP3301

    INT_CHECK(SP3301->iq_cap(rf_idx));
    INT_CHECK(SP3301->iq_cap_iq2buf(rf_idx));
    return 0;
}

int32_t RF_SetIQCaptureStarts(IQ_Capture_Index &RFIndex)
{
    vector<spec_rf_board_t *> SP1401;
    vector<spec_rf_board_t *>::iterator iter_SP1401;
    vector<dma_mgr *> ddr;
    vector<dma_mgr *>::iterator iter_ddr;
    uint32_t i = 0;

    // Find the first specified SP1401.
    for (i = 0;i < max_mimo_ports;i ++) {
        if (RFIndex.Idx[i]) {
            SP1401.push_back(&(g_rf_board.at(i)));
            ddr.push_back(SP1401.back()->m_sp3301->ddr(SP1401.back()->m_rf_idx));
            i ++;
            break;
        }
    }

    // Find the other SP1401s orderly.
    // If the next and the last specified SP1401s are connected to the same FPGA,
    // the next specified SP1401 will be ignored.
    for (;i < max_mimo_ports;i ++) {
        if (RFIndex.Idx[i]) {
            if (!is_brother_l_r(g_rf_board.at(i).m_rf_idx,SP1401.back()->m_rf_idx)) {
                SP1401.push_back(&(g_rf_board.at(i)));
                ddr.push_back(SP1401.back()->m_sp3301->ddr(SP1401.back()->m_rf_idx));
            }
        }
    }

    // Start.
    // We cannot make sure this will finish in 10ms right now!!!
    // All ports may not be synced!!!
    for (iter_ddr = ddr.begin();iter_ddr != ddr.end();iter_ddr ++) {
        INT_CHECK((*iter_ddr)->fpga_w_start());
    }

    for (iter_ddr = ddr.begin();iter_ddr != ddr.end();iter_ddr ++) {
        (*iter_ddr)->fpga_w_trans();
        (*iter_ddr)->fpga_w_abort();
    }

    for (iter_SP1401 = SP1401.begin();iter_SP1401 != SP1401.end();iter_SP1401 ++) {
        (*iter_SP1401)->m_sp3301->iq_cap_iq2buf((*iter_SP1401)->m_rf_idx);
    }

    return 0;
}

int32_t RF_SetIQCaptureAbort(uint32_t RFIndex)
{
    DECL_DYNAMIC_SP3301
    INT_CHECK(SP3301->iq_cap_abort(rf_idx));
    return 0;
}

int32_t RF_WarningInfo(uint32_t RFIndex,uint32_t &State)
{
    return 0;
}

int32_t RF_GetTemperature(uint32_t RFIndex,double &TxTemperature,double &RxTemperature)
{
    DECL_DYNAMIC_SP3301;
    INT_CHECK(SP3301->get_temp(rf_idx,TxTemperature,RxTemperature));
    return 0;
}

int32_t RF_GetCalTemperature(uint32_t RFIndex,double &Temperature)
{
    DECL_DYNAMIC_SP3301

    SP3301->get_cal_temp(rf_idx,Temperature);
    return 0;
}

int32_t RF_SetFans(uint32_t Speed)
{
    boost::ignore_unused(Speed);

    double tx_temp = 0.0;
    double rx_temp = 0.0;

    INT_CHECK(RF_GetTemperature(0,tx_temp,rx_temp));

    if (tx_temp >= 50.0) {
        INT_CHECK(SP3501.set_fan_group(1,180));
        INT_CHECK(SP3501.set_fan_group(3,140));
    } else {
        INT_CHECK(SP3501.set_fan_group(1,120));
        INT_CHECK(SP3501.set_fan_group(3,80));
    }

    return 0;
}

int32_t RF_SelfCalTxLOLeakage(uint32_t RFIndex)
{
    DECL_DYNAMIC_SP3301;

    return SP3301->self_cal_tx_lol(rf_idx);
}

int32_t RF_SelfCalTxSideband(uint32_t RFIndex)
{
    DECL_DYNAMIC_SP3301;

    return SP3301->self_cal_tx_sb(rf_idx);
}
