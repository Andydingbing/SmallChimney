#ifndef RD_SP9500_H
#define RD_SP9500_H

#include "stdint_common.h"
#include "preprocessor/prefix.h"

#ifndef __cplusplus
#define __cplusplus
#endif

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

enum SOURCE
{
    ARB,
    FPGA,
    CW
};

enum CONNECTOR
{
    IO,
    OUTPUT,
    LOOP
};

enum RFU_CLOCKSOURCE
{
    RFU_CLOCK_INTERNAL,
    RFU_CLOCK_EXTERNAL
};

enum RFU_TRIGGERSOURCE
{
    RFU_TRIGGER_INTERNAL,
    RFU_TRIGGER_EXTERNAL
};

enum TRIGGERMODE
{
    IF,
    FREERUN,
    MARKER
};

enum PROCESS
{
    IDLE_Driver,
    WFTrigger_Driver,
    Timeout_Driver,
    RUNNING_Driver,
    DONE_Driver
};

enum ARB_TRIGGERMODE
{
    ARB_MANUAL,
    ARB_CXUTRIGGER,
    ARB_FREERUN
};

typedef struct API IQ_Capture_Param {
    int TriggerType;
    int RadioFrameCondition_X;
    int RadioFrameCondition_Y;
    int TriggerOffset;
    int MeasLength;
    IQ_Capture_Param();
} tagGetRfRawDataInput;

typedef struct API IQ_Capture_Index {
    bool Idx[8];
    IQ_Capture_Index();
} tagGetRfRawDataIndex;

API int32_t RF_SetBitPath(char *Path);
API int32_t RF_Boot();
API int32_t RF_GetRFUNumber(uint32_t &uiRFUNumber);
API int32_t RF_GetRFPortNumber(uint32_t &uiRFPortNumber);
API int32_t RF_GetRFSerialNumber(uint32_t RFIndex,char *SerialNumber);
API int32_t RF_GetRFUSerialNumber(uint32_t RFUIndex,char *SerialNumber);
API int32_t RF_GetRFUVersion(char *version);
API int32_t RF_GetRFDriverVersion(const char **version);
API int32_t RF_SetTxState(uint32_t RFIndex,bool State);
API int32_t RF_SetTxPower(uint32_t RFIndex,float Power);
API int32_t RF_SetTxFrequency(uint32_t RFIndex,uint64_t Freq);
API int32_t RF_GetTxFrequency(uint32_t RFIndex,uint64_t &Freq);
API int32_t RF_SetTxSource(uint32_t RFIndex,SOURCE Source);
API int32_t RF_SetSourceFrequency(uint32_t RFIndex,uint64_t Freq);

API int32_t RF_SetRxLevel(uint32_t RFIndex,double Level);
API int32_t RF_GetRxLevel(uint32_t RFIndex,double &Level);
API int32_t RF_SetRxFrequency(uint32_t RFIndex,uint64_t Freq);
API int32_t RF_GetRxFrequency(uint32_t RFIndex,uint64_t &Freq);

API int32_t RF_LoadARBSource(uint32_t RFIndex,char *filename);
API int32_t RF_SetARBEnable(uint32_t RFIndex,bool bState);
API int32_t RF_SetARBTriggerSource(uint32_t RFIndex,ARB_TRIGGERMODE Source);
API int32_t RF_ExcuteARBTrigger(uint32_t RFIndex);
API int32_t RF_SetARBCount(uint32_t RFIndex,int iCnt);

API int32_t RF_SetConnector(uint32_t RFIndex,CONNECTOR Connector);
API int32_t RF_SetClockSource(uint32_t RFIndex,RFU_CLOCKSOURCE ClockSource = RFU_CLOCK_INTERNAL);
API int32_t RF_SetTriggerSource(uint32_t RFIndex,RFU_TRIGGERSOURCE TriggerSource = RFU_TRIGGER_INTERNAL);
API int32_t RF_SetTriggerMode(uint32_t RFIndex,TRIGGERMODE TriggerMode = IF);
API int32_t RF_SetTriggerLevel(uint32_t RFIndex,float TriggerLevel = 0);
API int32_t RF_SetTriggerOffset(uint32_t RFIndex,uint32_t Offset);

API int32_t RF_SetIQCaptureBuffer(uint32_t RFIndex,int16_t *I,int16_t *Q);
API int32_t RF_SetIQCaptureParams(uint32_t RFIndex,IQ_Capture_Param Param);
API int32_t RF_SetIQCaptureStart(uint32_t RFIndex);
API int32_t RF_SetIQCaptureStarts(IQ_Capture_Index &RFIndex);
API int32_t RF_SetIQCaptureAbort(uint32_t RFIndex);

API int32_t RF_InitPowerMeasure(uint32_t RFIndex);
API int32_t RF_AbortPowerMeasure(uint32_t RFIndex);
API int32_t RF_GetMeasProcess(uint32_t RFIndex,PROCESS &Process);
API int32_t RF_GetMeasResult(uint32_t RFIndex,float &Power,float &Crest);

API int32_t RF_WarningInfo(uint32_t RFIndex,uint32_t &State);
API int32_t RF_GetTemperature(uint32_t RFIndex,double &TxTemperature,double &RxTemperature);
API int32_t RF_GetCalTemperature(uint32_t RFIndex,double &Temperature);
API int32_t RF_SetFans(uint32_t Speed);

API int32_t RF_SelfCalTxLOLeakage(uint32_t RFIndex);
API int32_t RF_SelfCalTxSideband(uint32_t RFIndex);

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif
