#ifndef _ADC_MEASUREMENT_TASK_H
#define _ADC_MEASUREMENT_TASK_H

#include <appConfig.h>
#include <softTimerTask.h>
#include "systick_device.h"
#include <taskManager.h>
#include <adc_device.h>
#include "hwcfg.h"
#include <colorMixingTask.h>
#include <ledParamTables.h>


typedef struct{
  int32_t coefficient;
  int32_t offset;
}mParam_t;

typedef struct{
  mParam_t batt;
  mParam_t chipTemp;
  mParam_t adc;
}MeasureParam_t;

#define MEASURE_GAIN_POS         12U
#define MEASURE_GAIN             4096 /*( 1UL << MEASURE_GAIN_POS)*/

#define MEASURE_TEMP_GAIN_POS    14U
#define MEASURE_TEMP_GAIN        6511 /*16384*0.3974 */

#define INVALID_PARAM            (0x7FFFU)
#define INVALID_VOLT             (0x7FFFU)
#define INVALID_TEMP             (-127)
/*
 *  Get battery voltage in mV
 *  @param [out] battVolt: 0-32678
 *  @return 0: get voltage success, other : failed
 */
int8_t MES_GetBatteryVolt(uint16_t *battVolt);
/*
 *  Get chip temperature in 1C
 *  @param [out] chipTemperature: -127-127C
 *  @return 0: get voltage success, other : failed
 */
int16_t MES_GetChipTemperature(int16_t *chipTemperature);

/*
 *  Get Led temperature in 1C
 *  @param [in] 
 *  @return : temperature  -127-127C
 */
 ColorTemperature_t MES_GetLedTemperature(LedNum_t index);

/*
 *  measurement Task handle
 *  @return none
 */
void MES_TaskHandler(void);


/*
 *  Get Led PN voltage in mV when all of leds are off for init pn voltage mesurement 
 *  @param [out] pnVoltR/pnVoltG/pnVoltB: 0-4000mV
 *  @return none
 */
void MES_MnftGetLedPNVolt(LedNum_t ledIndex,uint16_t *pnVoltR, uint16_t *pnVoltG,uint16_t *pnVoltB);

/*
 *  Get Led R,G,B temperature in C
 *  @param [out] red temperature,green temperature,blue temperature
 *  @return 0
 */
int8_t MES_GetLedRGBTemperature(LedNum_t index, int8_t *rTemp,int8_t *gTemp,int8_t *bTemp);

/*
 *  Get Runtime Led PN voltage in mV
 *  @param [out] pnVoltR/pnVoltG/pnVoltB: 0-4000mV
 *  @return :none
 */
void MES_MnftGetRunTimeLedPNVolt(LedNum_t ledIndex,uint16_t *pnVoltR, uint16_t *pnVoltG,uint16_t *pnVoltB);

#endif