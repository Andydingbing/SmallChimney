#ifndef RD_SP9500X_H
#define RD_SP9500X_H

#include "stdint_common.h"
#include "rd_types.h"

#ifndef __cplusplus
#define __cplusplus
#endif

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

// UL IQ Capture
enum RD_SP9500X_UL_SR {
    SP9500X_UL_SR_98304,
    SP9500X_UL_SR_49152,
    SP9500X_UL_SR_24576,
    SP9500X_UL_SR_12288
};

struct API RD_SP9500X_IQ_Capture_Param {
    int TriggerType;
    int RadioFrameCondition_X;
    int RadioFrameCondition_Y;
    int TriggerOffset;
    int MeasLength;
    int Channel;
    RD_SP9500X_UL_SR SampleRate;
    RD_SP9500X_IQ_Capture_Param();
};

// DL/UL CA
enum RD_SP9500X_CA_Bandwidth {
    SP9500X_CA_Invalid,
    SP9500X_CA_100M,
    SP9500X_CA_200M,
    SP9500X_CA_400M
};

enum RD_SP9500X_JESD {
    SP9500X_JESD_TX = 1,
    SP9500X_JESD_RX,
    SP9500X_JESD_TRX,
};

struct API RD_SP9500X_CA_Carrier {
    int64_t Freq;
    uint8_t Channel;
    RD_SP9500X_CA_Bandwidth Bandwidth;
};

struct API RD_SP9500X_CA_Map {
    RD_SP9500X_CA_Carrier Carrier[8];
    RD_SP9500X_CA_Map();
};

API int32_t SP9500X_RF_SetBitPath(const char *Path);
API int32_t SP9500X_RF_Boot();
API int32_t SP9500X_RF_GetSerialNumber(const uint32_t RFIndex,char *SerialNumber);
API int32_t SP9500X_RF_IsRRHConnected(const uint32_t RFIndex,bool &Status);

API int32_t SP9500X_RF_SetConnector(const uint32_t RFIndex,const RD_IO_Mode Connector);

API int32_t SP9500X_RF_SetTxState(const uint32_t RFIndex,const bool State);
API int32_t SP9500X_RF_SetTxPower(const uint32_t RFIndex,const float Power);
API int32_t SP9500X_RF_SetTxFrequency(const uint32_t RFIndex,const uint64_t Freq);
API int32_t SP9500X_RF_GetTxFrequency(const uint32_t RFIndex,uint64_t &Freq);

API int32_t SP9500X_RF_SetRxLevel(const uint32_t RFIndex,const double Level);
API int32_t SP9500X_RF_GetRxLevel(const uint32_t RFIndex,double &Level);
API int32_t SP9500X_RF_SetRxFrequency(const uint32_t RFIndex,const uint64_t Freq);
API int32_t SP9500X_RF_GetRxFrequency(const uint32_t RFIndex,uint64_t &Freq);
API int32_t SP9500X_RF_SetRxFrequencyShift(const uint32_t RFIndex,const int64_t Freq);

API int32_t SP9500X_RF_SetTxCAMap(const uint32_t RFIndex,const RD_SP9500X_CA_Map &Map);
API int32_t SP9500X_RF_SetRxCAMap(const uint32_t RFIndex,const RD_SP9500X_CA_Map &Map);

API int32_t SP9500X_RF_LoadARBSource(const uint32_t RFIndex,const char *Path);
API int32_t SP9500X_RF_SetARBEnable(const uint32_t RFIndex,const bool State);
API int32_t SP9500X_RF_SetARBCount(const uint32_t RFIndex,const int Count);

API int32_t SP9500X_RF_SetIQCaptureBuffer(const uint32_t RFIndex,int16_t **I,int16_t **Q);
API int32_t SP9500X_RF_SetIQCaptureParams(const uint32_t RFIndex,const RD_SP9500X_IQ_Capture_Param &Param);
API int32_t SP9500X_RF_SetIQCaptureStart(const uint32_t RFIndex);
API int32_t SP9500X_RF_SetIQCaptureAbort(const uint32_t RFIndex);

API int32_t SP9500X_HardResetChips();
API int32_t SP9500X_SoftResetChips();
API int32_t SP9500X_ZU21_ConfigStatus(bool &done);
API int32_t SP9500X_JESD_Reset(const RD_SP9500X_JESD jesd);
API int32_t SP9500X_AD998X_Status(bool &status);

API int32_t SP9500X_SetFan(const uint32_t FanIndex,const uint32_t Speed);
API int32_t SP9500X_SetFans(const uint32_t Speed);

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif
