#include <measureTask.h>
#include <pdsTask.h>

#define AVERAGE_MEASURE_POS     (4U)
#define AVERAGE_MEASURE_GAIN    (1U << AVERAGE_MEASURE_POS)

typedef struct{
    /* Bandgap Volt */
    uint32_t bandgapVolt;
    /* VBat Calib Param */
    int32_t calibVBatVL;      /* battery volt calib point low 8000mV   */
    int32_t calibVBatVH;      /* battery volt calib point high 13500mV */
    int32_t calibVBatVLCode;  /* adc count @ battery volt low(8000mV)  calib point low */
    int32_t calibVBatVHCode;  /* adc count @ battery volt high(8000mV) calib point high */
    /* PN Volt Calib Param */
    int32_t calibVL;          /* calib Point: 8000mV    */
    int32_t calibVH;          /* calib Point: 13500mV   */
    int32_t calibPNLVolt;     /* delta PN volt : 1500mV */
    int32_t calibPNHVolt;     /* delta PN volt : 4000mV */

    int32_t calibCodeVLPNL;   /* VBAT=8V    deltaPN:1500mV adc code */
    int32_t calibCodeVLPNH;   /* VBAT=8V    deltaPN:4000mV adc code */
    int32_t calibCodeVHPNL;   /* VBAT=13.5V deltaPN:1500mV adc code */
    int32_t calibCodeVHPNH;   /* VBAT=13.5V deltaPN:4000mV adc code */
    
    int32_t lowGain;
    int32_t highGain;
    int32_t lowOffset;
    int32_t highOffset;
    
}AdcCompParam_t;
    

typedef struct{
    uint32_t currCode;
    uint16_t buff[AVERAGE_MEASURE_GAIN];
    uint16_t count;
}AdcDatabuff_t;


typedef struct{
    AdcDatabuff_t chipTemp;
    AdcDatabuff_t batt;
    AdcDatabuff_t ledPN[3];
    AdcDatabuff_t ledBatt[3];
}AdcResult_t;

static AdcCompParam_t acParam;
static MeasureParam_t msParam;
static volatile uint32_t measStart = 0U;
static volatile uint16_t measGeneralAdcCode[3];

static uint8_t updateParamIndex = 0U;
static uint8_t updateParamStart = FALSE;

static AdcResult_t adcResult;
static TaskState_t adcTaskState = TASK_STATE_INIT;
static AdcMeasureItem_t adcItem = ADC_MEASURE_ITEM_NONE;
static uint8_t syncEnable = FALSE;

void measureTimerExpired(SoftTimer_t *timer);
void measureDoneISR(ADCMeasureParam_t item, uint16_t *const result);
void measureParamInit(void);
void measureParamStart(void);

void   add_data_to_buff(uint32_t *original, uint16_t newData, uint16_t *bufferIndex,uint16_t *buff);
int8_t get_led_temperature(TempParams_t const table[],int16_t deltaPNVolt);
int8_t adc_getVolt(uint16_t adcCode,uint16_t *battVolt);
uint16_t get_led_volt(int16_t battVolt, uint16_t ledAdcCode);
void get_led_pn_volts(uint16_t *voltR,uint16_t *voltG,uint16_t *voltB);
void get_runtime_led_pn_volt(uint16_t *pnVoltR, uint16_t *pnVoltG,uint16_t *pnVoltB);

void updateBatteryVolt(void);
void updateChipTemperature(void);
void updateLedsTemperature(uint16_t voltR,uint16_t voltG,uint16_t voltB);
void updateParams(void);

static uint16_t battVolt_mV;
static int8_t   pnTemp_C[3];
static int16_t  chipTemp_C;
static volatile uint32_t adcConvertDone = 0U;
static uint16_t rtLedPNVoltR = INVALID_VOLT ,rtLedPNVoltG = INVALID_VOLT ,rtLedPNVoltB = INVALID_VOLT ;


void add_data_to_buff(uint32_t *original, uint16_t newData, uint16_t *bufferIndex,uint16_t *buff)
{
    if (*original == INVALID_PARAM){
        *original = newData;
        *bufferIndex = 0;
        for (uint8_t i = 0U; i < AVERAGE_MEASURE_GAIN; i++){
            buff[i] = newData;
        }
    }else{
        buff[*bufferIndex] = newData;
    }
    (*bufferIndex) ++;
    if ((*bufferIndex) >= AVERAGE_MEASURE_GAIN){
        (*bufferIndex) = 0U;
    }
}


static SoftTimer_t measureTimer = {
    .mode     = TIMER_PERIODIC_MODE,
    .interval = 50U,
    .handler  = measureTimerExpired
};


void measureTimerExpired(SoftTimer_t *timer)
{
    static uint8_t measWaitCount = 3U;
    /*  Update then start next measurement  */
    if (++measWaitCount >= 3U){
        measWaitCount = 0U;
        updateParamStart = TRUE;
    }
    measStart = TRUE;
    TM_PostTask(TASK_ID_ADC_MEASURE);
}


void measureDoneISR(ADCMeasureParam_t item, uint16_t *const result)
{
    measGeneralAdcCode[0] = result[0];
    measGeneralAdcCode[1] = result[1];
    measGeneralAdcCode[2] = result[2];
    adcConvertDone = 1U;
}

void measureParamStart(void)
{
    static uint8_t ledPhyChannel = 0U;
    switch(adcItem){
    case ADC_MEASURE_ITEM_NONE:
      syncEnable = FALSE;
      adcItem = ADC_MEASURE_ITEM_VTEMP_VBAT;
      break;
    case ADC_MEASURE_ITEM_VTEMP_VBAT:
      add_data_to_buff(&adcResult.chipTemp.currCode, measGeneralAdcCode[0], &adcResult.chipTemp.count,adcResult.chipTemp.buff);
      add_data_to_buff(&adcResult.batt.currCode,     measGeneralAdcCode[1], &adcResult.batt.count,    adcResult.batt.buff);
      if (PWM_LedIsActive() == TRUE){
          syncEnable = TRUE;
          adcItem = ADC_MEASURE_ITEM_VLEDx_VBAT;
          ledPhyChannel = 0U;
      }else{
          adcResult.ledPN[0].currCode =  INVALID_PARAM;
          adcResult.ledPN[1].currCode =  INVALID_PARAM;
          adcResult.ledPN[2].currCode =  INVALID_PARAM;
          adcItem = ADC_MEASURE_ITEM_VTEMP_VBAT;
          syncEnable = FALSE;
      }
      break;
    case ADC_MEASURE_ITEM_VLEDx_VBAT:
      if (PWM_LedIsActive() == TRUE){
          add_data_to_buff(&adcResult.ledPN[ledPhyChannel].currCode,   measGeneralAdcCode[2], &adcResult.ledPN[ledPhyChannel].count,  adcResult.ledPN[ledPhyChannel].buff);
          add_data_to_buff(&adcResult.ledBatt[ledPhyChannel].currCode, measGeneralAdcCode[1], &adcResult.ledBatt[ledPhyChannel].count,adcResult.ledBatt[ledPhyChannel].buff);
          ledPhyChannel++;
          if (ledPhyChannel < 3U){
              syncEnable = TRUE;
              adcItem = ADC_MEASURE_ITEM_VLEDx_VBAT;
          }else{
              adcItem = ADC_MEASURE_ITEM_VTEMP_VBAT;
              syncEnable = FALSE;
          }
      }else{
          adcResult.ledPN[0].currCode =  INVALID_PARAM;
          adcResult.ledPN[1].currCode =  INVALID_PARAM;
          adcResult.ledPN[2].currCode =  INVALID_PARAM;
          adcItem = ADC_MEASURE_ITEM_VTEMP_VBAT;
          syncEnable = FALSE;
      }
      break;
    default:
      break;
    }
    if (adcItem != ADC_MEASURE_ITEM_NONE){
        ADC_Init(adcItem, ledPhyChannel,syncEnable);
        adcConvertDone = 0U;
        ADC_Start();
    }
}

int8_t adc_getVolt(uint16_t adcCode,uint16_t *battVolt)
{
    int32_t volt;
    uint32_t uVolt;
    volt = msParam.batt.coefficient * (int32_t)adcCode;
    volt = volt + msParam.batt.offset;
    uVolt = (uint32_t)volt;
    uVolt = uVolt >> MEASURE_GAIN_POS;
    *battVolt = (uint16_t)uVolt;
    return 0;
}


int8_t get_led_temperature(TempParams_t const table[],int16_t deltaPNVolt)
{
    int8_t temperature;
    uint8_t temp;
    uint8_t index;
    for (index = 0U; index < TEMP_CMP_NUM_MAX; index++){
        if (deltaPNVolt >= table[index].deltaVoltPN){
            break;
        }
    }
    temp = index << DELTA_TEMP_POS;
    temperature = (int8_t)temp;
    temperature = temperature + MINIMUM_TEMP;
    return temperature;
}

uint16_t get_led_volt(int16_t battVolt, uint16_t ledAdcCode)
{
    
    int32_t codeA,codeB;
    uint32_t codeU;
    int32_t volt; 
    codeA = (int32_t)(battVolt - acParam.calibVL)*acParam.lowGain + acParam.lowOffset;
    codeB = (int32_t)(battVolt - acParam.calibVL)*acParam.highGain+ acParam.highOffset;
    
    if (codeA >= 0){
        codeU = (uint32_t)codeA;
        codeU = codeU >> 16;
        codeA = (int32_t)codeU;
    }else{
        codeA = -codeA;
        codeU = (uint32_t)codeA;
        codeU = codeU >> 16;
        codeA = (int32_t)codeU;
        codeA = -codeA;
    }
    
    if (codeB >= 0){
        codeU = (uint32_t)codeB;
        codeU = (uint32_t)codeU >> 16;
        codeB = (int32_t)codeU;
    }else{
        codeB = -codeB;
        codeU = (uint32_t)codeB;
        codeU = codeU >> 16;
        codeB = (int32_t)codeU;
        codeB = -codeB;
    }
    
    volt = ((int32_t)ledAdcCode - codeA)*(acParam.calibPNHVolt - acParam.calibPNLVolt)/(codeB - codeA) + acParam.calibPNLVolt;
    return (uint16_t)volt;
}

void get_led_pn_volts(uint16_t *voltR,uint16_t *voltG,uint16_t *voltB)
{
    uint32_t sumLed;
    uint32_t sumBatt;
    uint16_t volt[3] = {INVALID_VOLT,INVALID_VOLT,INVALID_VOLT};
    if (adcResult.ledPN[0].currCode != INVALID_PARAM && adcResult.ledPN[1].currCode != INVALID_PARAM && adcResult.ledPN[2].currCode != INVALID_PARAM){
        for (uint8_t index = 0U; index < 3U; index++){
            sumLed  = 0U;
            sumBatt = 0U;
            for (uint8_t i = 0U; i < AVERAGE_MEASURE_GAIN; i++){
                sumBatt += adcResult.ledBatt[index].buff[i];
                sumLed  += adcResult.ledPN[index].buff[i];
            }
            sumBatt = sumBatt >> AVERAGE_MEASURE_POS;
            sumLed  = sumLed  >> AVERAGE_MEASURE_POS;
            (void)adc_getVolt((uint16_t)sumBatt,&volt[index]);
            volt[index] = get_led_volt((int16_t)volt[index], (uint16_t)sumLed);
        }
    }
    
    *voltR = volt[PHY_CHANNEL_RED];
    *voltG = volt[PHY_CHANNEL_GREEN];
    *voltB = volt[PHY_CHANNEL_BLUE];
}




void updateBatteryVolt(void)
{
    uint32_t sumCode;
    if (adcResult.batt.currCode != INVALID_PARAM){
        sumCode = 0U;
        for (uint8_t i = 0U; i < AVERAGE_MEASURE_GAIN; i++){
            sumCode += adcResult.batt.buff[i];
        }
        sumCode = sumCode >> AVERAGE_MEASURE_POS;
        (void)adc_getVolt((uint16_t)sumCode,&battVolt_mV);
    }else{
        battVolt_mV = INVALID_VOLT;
    }
}


void updateChipTemperature(void)
{
    uint32_t uVolt;
    uint32_t sumCode;
    if (adcResult.chipTemp.currCode != INVALID_PARAM){
        sumCode = 0U;
        for (uint8_t i = 0U; i < AVERAGE_MEASURE_GAIN; i++){
            sumCode += adcResult.chipTemp.buff[i];
        }
        sumCode = sumCode >> AVERAGE_MEASURE_POS;
        /* get adc volt */
        int32_t volt = msParam.adc.coefficient * (int32_t)sumCode;
        volt = volt + msParam.adc.offset;
        uVolt = (uint32_t)volt;
        uVolt = uVolt >> MEASURE_GAIN_POS; /* mV */
        volt = (int32_t)uVolt;
        
        volt = msParam.chipTemp.coefficient * volt;
        volt = volt + msParam.chipTemp.offset;
        if (volt <= 0){
            volt = -volt;
            uVolt = (uint32_t)volt;
            uVolt = uVolt >> MEASURE_TEMP_GAIN_POS;
            volt = (int32_t)uVolt;
            volt = - volt;
        }else{
            uVolt = (uint32_t)volt;
            uVolt = uVolt >> MEASURE_TEMP_GAIN_POS;
            volt = (int32_t)uVolt;
        }
        if (volt <= -127){
            chipTemp_C = INVALID_TEMP + 1;
        }else{
            chipTemp_C = (int16_t)volt;
        }
    }else{
        chipTemp_C = INVALID_TEMP;
    }
}


void updateLedsTemperature(uint16_t voltR,uint16_t voltG,uint16_t voltB)
{
    int16_t typicalVolt_R, typicalVolt_G, typicalVolt_B;
    ColorTemperature_t temperature;
    temperature.value[0] = ROOM_TEMPERATURE;
    temperature.value[1] = ROOM_TEMPERATURE;
    temperature.value[2] = ROOM_TEMPERATURE;
    
    /* get typical LED PN volt @24C*/
    (void)CLM_GetLedPNVolts(LED0,temperature, &typicalVolt_R, &typicalVolt_G, &typicalVolt_B); /* get init PN volt @24C */
    /* get LED R volt and temperature */
    if (voltR != INVALID_VOLT && voltG != INVALID_VOLT && voltB != INVALID_VOLT){
        pnTemp_C[LED_RED]   = get_led_temperature(LP_GetTempTableRed(LED0),  ((int16_t)voltR - typicalVolt_R));
        pnTemp_C[LED_GREEN] = get_led_temperature(LP_GetTempTableGreen(LED0),((int16_t)voltG - typicalVolt_G));
        pnTemp_C[LED_BLUE]  = get_led_temperature(LP_GetTempTableBlue(LED0), ((int16_t)voltB - typicalVolt_B));
    }else{
        pnTemp_C[LED_RED]       = ROOM_TEMPERATURE;
        pnTemp_C[LED_GREEN]     = ROOM_TEMPERATURE;
        pnTemp_C[LED_BLUE]      = ROOM_TEMPERATURE;
    }
    /*DEBUG_OUT("R:%d,G:%d,B:%d\r\n",pnTemp_C[LED_RED],pnTemp_C[LED_GREEN],pnTemp_C[LED_BLUE]);*/
}


/*
 *  Get battery voltage in mV
 *  @param [out] battVolt: 0-65535
 *  @return 0: get voltage success, other : failed
 */
int8_t MES_GetBatteryVolt(uint16_t *battVolt)
{
    int8_t result = -1;
    if (battVolt_mV != INVALID_VOLT){
        result = 0;
    }
    *battVolt = battVolt_mV;
    return result;
}

int16_t MES_GetChipTemperature(int16_t *chipTemperature)
{
    int8_t result = -1;
    if (chipTemp_C != INVALID_TEMP){
        result = 0;
    }
    *chipTemperature = chipTemp_C;
    return result;
}


 ColorTemperature_t MES_GetLedTemperature(LedNum_t index)
 {
    ColorTemperature_t temperature;
   
    temperature.value[0] = pnTemp_C[LED_RED];
    temperature.value[1] = pnTemp_C[LED_GREEN];
    temperature.value[2] = pnTemp_C[LED_BLUE];

    return temperature;
 }

 int8_t MES_GetLedRGBTemperature(LedNum_t index, int8_t *rTemp,int8_t *gTemp,int8_t *bTemp)
 {
    *rTemp = pnTemp_C[LED_RED];
    *gTemp = pnTemp_C[LED_GREEN];
    *bTemp = pnTemp_C[LED_BLUE];
    return 0;
 }

void measureParamInit(void)
{
    int32_t adcCode0_5V;
    int32_t adcCode1_0V;
    int32_t tempVolt_mV_25C;
    
    acParam.bandgapVolt  = HWCFG_SFRS->BANDGAP_VOLT;
    
    acParam.calibVBatVL      = 8000;    /* Battery Volt 8V */
    acParam.calibVBatVH      = 13500;   /* Battery Volt 13.5V */
    acParam.calibVBatVLCode  = (int32_t)HWCFG_SFRS->VBAT_CODE_8V;     /*8V adc count 1650*/
    acParam.calibVBatVHCode  = (int32_t)HWCFG_SFRS->VBAT_CODE_13V5;   /*13.5V adc count */
    
    acParam.calibVL      = 8000;        /* PN @Battery Volt 8V */
    acParam.calibVH      = 13500;       /* PN @Battery Volt 13.5V */
    acParam.calibPNLVolt = 1500;        /* Delta PN 1.5V*/
    acParam.calibPNHVolt = 4000;        /* Delta PN 4.0V*/
    
    acParam.calibCodeVLPNL   = (int32_t)HWCFG_SFRS->LED_VL_PNL_CODE;
    acParam.calibCodeVLPNH   = (int32_t)HWCFG_SFRS->LED_VL_PNH_CODE;
    acParam.calibCodeVHPNL   = (int32_t)HWCFG_SFRS->LED_VH_PNL_CODE;
    acParam.calibCodeVHPNH   = (int32_t)HWCFG_SFRS->LED_VH_PNH_CODE;
    
    acParam.lowGain = (acParam.calibCodeVHPNL - acParam.calibCodeVLPNL)*65535/(acParam.calibVH - acParam.calibVL);
    acParam.highGain= (acParam.calibCodeVHPNH - acParam.calibCodeVLPNH)*65535/(acParam.calibVH - acParam.calibVL);
    acParam.lowOffset  = 65535*acParam.calibCodeVLPNL;
    acParam.highOffset = 65535*acParam.calibCodeVLPNH;
    
    adcResult.chipTemp.currCode = INVALID_PARAM;
    adcResult.batt.currCode     = INVALID_PARAM;
    
    for (uint8_t i = 0U; i < 3U; i++){
        adcResult.ledBatt[i].currCode = INVALID_PARAM;
        adcResult.ledPN[i].currCode   = INVALID_PARAM;
    }

    /****************************************************************************/
    /* Vadc*/
    adcCode0_5V = (int16_t)HWCFG_SFRS->ADC_0V5;
    adcCode1_0V = (int16_t)HWCFG_SFRS->ADC_1V0;
    msParam.adc.coefficient = (1000 - 500)*MEASURE_GAIN/(adcCode1_0V - adcCode0_5V);
    msParam.adc.offset      = 500*MEASURE_GAIN - msParam.adc.coefficient*adcCode0_5V;
    
    /* Vbattery */
    msParam.batt.coefficient = (acParam.calibVBatVH - acParam.calibVBatVL)*MEASURE_GAIN/(acParam.calibVBatVHCode - acParam.calibVBatVLCode);
    msParam.batt.offset      = acParam.calibVBatVL*MEASURE_GAIN - msParam.batt.coefficient*acParam.calibVBatVLCode;
    
    /* Tsensor */
    /* Tsensor_Volt = 2.5162*T_Sensor_Temp + offset  T_Sensor_Temp = (Tsensor_Volt - offset)/2.5162; offset = tempVolt_mV_25C - 2.5162*25(63)
    */
    tempVolt_mV_25C = (int32_t)HWCFG_SFRS->TSensor_mV_25C;
    msParam.chipTemp.coefficient   = MEASURE_TEMP_GAIN;
    msParam.chipTemp.offset        = 25*(int32_t)(1UL << MEASURE_TEMP_GAIN_POS) - MEASURE_TEMP_GAIN*tempVolt_mV_25C;
    
    /*  init ADC */
    ADC_GeneralInit();
    ADC_RegisterIRQ(measureDoneISR);
}

void updateParams(void)
{

    if( updateParamIndex == 0U){
        /* calculate battery volt */
        updateBatteryVolt();
    }else if( updateParamIndex == 1U){
        /* calculate chip temperature */
        updateChipTemperature();
    }else{
        /* get current LED PN volt */
        get_led_pn_volts(&rtLedPNVoltR,&rtLedPNVoltG,&rtLedPNVoltB);
        /* calculate led temperture*/
        updateLedsTemperature(rtLedPNVoltR,rtLedPNVoltG,rtLedPNVoltB);
    }
    if (++updateParamIndex > 2U){
        updateParamIndex = 0U;
    }
}


void MES_TaskHandler(void)
{
    static uint8_t ledOffTrapCount = 0;
    switch(adcTaskState){
    case TASK_STATE_ACTIVE:
      /* start parameters updating  */
      if (updateParamStart == TRUE){
          updateParamStart = FALSE;
          updateParams();
      }
      /*  start parameters measuring */
      if (measStart == TRUE){
          measStart = FALSE;
          if (adcConvertDone == 0U){
              ledOffTrapCount++;
              if (ledOffTrapCount >= 3U){
                  ledOffTrapCount = 0;
                  adcItem = ADC_MEASURE_ITEM_NONE;
              }
          }else{
              ledOffTrapCount = 0U;
          }
          measureParamStart();
      }
      break;
    case TASK_STATE_INIT:
      measureParamInit();
      SoftTimer_Start(&measureTimer);
      adcTaskState = TASK_STATE_ACTIVE;
      break;

    default:
      break;
    }
}


void get_runtime_led_pn_volt(uint16_t *pnVoltR, uint16_t *pnVoltG,uint16_t *pnVoltB)
{
    uint16_t ledPNVolt[3];
    uint16_t ledBattVolt[3];
    uint32_t ledBattCode[3] = {0,0,0};
    uint32_t ledCode[3]     = {0,0,0};  
    for (uint8_t channel = 0U; channel < 3U; channel++){
        ADC_Init( ADC_MEASURE_ITEM_VLEDx_VBAT, channel, TRUE);
        for (uint8_t i = 0U; i < AVERAGE_MEASURE_GAIN; i++){
            ADC_Start();
            adcConvertDone = 0U;
            ADC_SFRS->CONF.SWSYNCIN = 1;
            ADC_SFRS->CONF.SWSYNCIN = 0;
            while(adcConvertDone == 0U){}
            ledBattCode[channel] += measGeneralAdcCode[1];
            ledCode[channel] += measGeneralAdcCode[2];
        }
        ledBattCode[channel] = ledBattCode[channel] >> AVERAGE_MEASURE_POS;
        ledCode[channel]     = ledCode[channel] >> AVERAGE_MEASURE_POS;
        (void)adc_getVolt((uint16_t)ledBattCode[channel],&ledBattVolt[channel]);
        ledPNVolt[channel] = get_led_volt((int16_t)ledBattVolt[channel], (uint16_t)ledCode[channel]);
    }
    *pnVoltR = ledPNVolt[PHY_CHANNEL_RED];
    *pnVoltG = ledPNVolt[PHY_CHANNEL_GREEN];
    *pnVoltB = ledPNVolt[PHY_CHANNEL_BLUE];
    
}
/*
 *  Get Led PN voltage in mV only for manufacturing PN volt reading block mode !!!!!
 *  @param [out] pnVoltR/pnVoltG/pnVoltB: 0-4000mV
 *  @return 0: get voltage success, other : failed
 */
void MES_MnftGetLedPNVolt(LedNum_t ledIndex,uint16_t *pnVoltR, uint16_t *pnVoltG,uint16_t *pnVoltB)
{     
    (void)CLM_SetPWMs(ledIndex,0U, 0U, 0U, 0U);  /* turn off all of channels */
    while(PWM_UpdateFInished()!= 0){}   /* wait until taking effect */
    get_runtime_led_pn_volt(pnVoltR, pnVoltG,pnVoltB);
}

/*
 *  Get Runtime Led PN voltage in mV
 *  @param [out] pnVoltR/pnVoltG/pnVoltB: 0-4000mV
 *  @return :none
 */
void MES_MnftGetRunTimeLedPNVolt(LedNum_t ledIndex,uint16_t *pnVoltR, uint16_t *pnVoltG,uint16_t *pnVoltB)
{     
    *pnVoltR = rtLedPNVoltR;
    *pnVoltG = rtLedPNVoltG;
    *pnVoltB = rtLedPNVoltB;
}

