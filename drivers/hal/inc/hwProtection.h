#ifndef __HW_PROTECTION_H__
#define __HW_PROTECTION_H__
#include <realplum.h>
#include <realplum_sfr.h>
#include <core_cm0.h>
typedef enum{
  BOR_1V5_THRS_1310V = 0,
  BOR_1V5_THRS_1340V,
  BOR_1V5_THRS_1370V,
  BOR_1V5_THRS_1400V,
  BOR_1V5_THRS_1440V,
  BOR_1V5_THRS_1480V,
  BOR_1V5_THRS_1510V,
  BOR_1V5_THRS_1560V,
  BOR_1V5_THRS_1600V,
}BOR_1V5_Thres_t;


typedef enum{
  BOR_3V3_THRS_2160V = 0,
  BOR_3V3_THRS_2220V,
  BOR_3V3_THRS_2290V,
  BOR_3V3_THRS_2360V,
  BOR_3V3_THRS_2430V,
  BOR_3V3_THRS_2510V,
  BOR_3V3_THRS_2590V,
  BOR_3V3_THRS_2680V,
  BOR_3V3_THRS_2780V,
  BOR_3V3_THRS_2890V,
  BOR_3V3_THRS_3000V,
  BOR_3V3_THRS_3120V,
  BOR_3V3_THRS_3250V,
  BOR_3V3_THRS_3550V,
}BOR_3V3_Thres_t;


typedef enum{
  OVT_PROT_100C = 0,
  OVT_PROT_105C,
  OVT_PROT_110C,
  OVT_PROT_115C,
  OVT_PROT_120C,
  OVT_PROT_125C,
  OVT_PROT_130C,
  OVT_PROT_135C,
  OVT_PROT_140C,
  OVT_PROT_145C,
  OVT_PROT_150C,
}OverTempThres_t;

typedef enum{
  OVT_ACTION_NONE = 0,
  OVT_ACTION_IRQ,
  OVT_ACTION_HW_RESET,
}OverTempAction_t;

typedef enum{
  OV_VOLT_18810mV = 0x01,
  OV_VOLT_18400mV = 0x02,
  OV_VOLT_18030mV = 0x04,
  OV_VOLT_17660mV = 0x08,
  OV_VOLT_17300mV = 0x10,
  OV_VOLT_16960mV = 0x20,
}OverVoltThres_t;

typedef void (*hwProtIsrCallback_t)(void);


void HW_PROT_SetOverTmperature(OverTempThres_t threshold, OverTempAction_t action);
void HW_PROT_RegisterOVTemperatureIRQ(hwProtIsrCallback_t callback);
void HW_PROT_UnRegisterOVTemperatureIRQ(void);

void HW_PROT_SetOverVoltage(OverVoltThres_t threshold);
void HW_PROT_RegisterOverVoltageIRQ(hwProtIsrCallback_t callback);
void HW_PROT_UnRegisterOverVoltageIRQ(void);

/* return 0: normal , others : too high */
uint8_t HW_PROT_BattVoltIsTooHigh(void);
/* return 0: normal , others : over heat */
uint8_t HW_PROT_ChipIsOverHeat(void);




#endif