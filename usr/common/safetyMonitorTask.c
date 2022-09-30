#include <safetyMonitorTask.h>


#define SW_OVER_VOLT                            (1U << 0)
#define SW_UNDER_VOLT                           (1U << 1)
#define SW_OVER_TEMP_FLAG_HIGH                  (1U << 2)
#define SW_OVER_TEMP_FLAG_SUPER_HIGH            (1U << 3)

#define SW_OVER_VOLT_LEVEL                      (19000U)/*mV*/
#define SW_OVER_VOLT_LEVEL_REELEASE             (17000U)/*mV*/
#define SW_UNDER_VOLT_LEVEL                     (6000U) /*mV*/
#define SW_UNDER_VOLT_LEVEL_RELEASE             (7000U) /*mV*/

#define SW_OVER_TEMP_RELEASE                    (100)   /*celsius degree */
#define SW_OVER_TEMP_HIGH                       (110)   /*celsius degree */
#define SW_OVER_TEMP_SUPER_HIGH                 (120)   /*celsius degree */

#define OVER_VOLT_MAX_INTENSITY                 (80U*1024U/100U)/* 80% of max intensity */
#define UNDER_VOLT_MAX_INTENSITY                (0U*1024U/100U) /* 50% of max intensity */

#define OVER_TEMP_HIGH_MAX_INTENSITY            (80U*1024U/100U)/* 80% of max intensity */
#define OVER_TEMP_SUPER_HIGH_MAX_INTENSITY      (50U*1024U/100U)/* 50% of max intensity */

#define NORMAL_STATE_MAX_INTENSITY              (1024U)         /* 100% of max intensity */

#define SAFETY_MONITOR_INTERVEL   (200U)       /*ms*/


static TaskState_t safeMonitorState = TASK_STATE_INIT;
static uint8_t safetyState = 0U;

void safeMonitorTimerExpired(SoftTimer_t *timer);
void safetyHandle(void);

static SoftTimer_t safeMonitorTimer = {
    .mode     = TIMER_PERIODIC_MODE,
    .interval = SAFETY_MONITOR_INTERVEL,/* ms  */
    .handler  = safeMonitorTimerExpired
};

static int16_t chipTemperature;

void SAFM_TaskHandler(void)
{
    switch(safeMonitorState){
    case TASK_STATE_INIT:
      /* for software protection monitor */
      /* SoftTimer_Start(&safeMonitorTimer); */
      safeMonitorState = TASK_STATE_ACTIVE;
      break;
    case TASK_STATE_ACTIVE:
      safetyHandle();
      break;
    default:
      break;
    }
}

void safetyHandle(void)
{
#if SAFETY_MONITOR_EN == 1U

    uint16_t intensityRatio = NORMAL_STATE_MAX_INTENSITY;
    /* Set maximum intensity based on over/under volt status */
    if ( (safetyState & SW_OVER_VOLT) == SW_OVER_VOLT ){
        intensityRatio = OVER_VOLT_MAX_INTENSITY;
    }else if ( (safetyState & SW_UNDER_VOLT) == SW_UNDER_VOLT){
        intensityRatio = UNDER_VOLT_MAX_INTENSITY;
    }else{
        /* no over/under volt occurs */
    }
    
    /* Set maximum intensity based on current */
    if ( (safetyState & SW_OVER_TEMP_FLAG_HIGH) == SW_OVER_TEMP_FLAG_HIGH ){
        if (intensityRatio > OVER_TEMP_SUPER_HIGH_MAX_INTENSITY){
            intensityRatio = OVER_TEMP_SUPER_HIGH_MAX_INTENSITY;
        }
    }else if ( (safetyState & SW_OVER_TEMP_FLAG_SUPER_HIGH) == SW_OVER_TEMP_FLAG_SUPER_HIGH){
        if (intensityRatio > OVER_TEMP_HIGH_MAX_INTENSITY){
            intensityRatio = OVER_TEMP_HIGH_MAX_INTENSITY;
        }
    }else{
        /* no over temperature occurs */
    }
    
    (void)CLM_SetLimitedMaxIntensity(intensityRatio);
    
    /* Update color parameters */
    if ( PWM_LedIsActive() == TRUE){
        ColorParam_t const *color = CLM_GetCurrentColorParams(LED0);
        if(color != NULL){
            switch(color->colorMode){
            case COLOR_MODE_XYY:
              (void)CLM_SetXYY(LED0,MES_GetLedTemperature(LED0), color->XYY.x, color->XYY.y, color->XYY.Y, 1000U);
              break;
            case COLOR_MODE_ACCURATE_XYY:
              (void)CLM_SetAccurateXYY(LED0,color->iRatio, MES_GetLedTemperature(LED0), color->XYY.x, color->XYY.y, (uint8_t)color->XYY.Y, 1000U);
              break;
            case COLOR_MODE_RGBL:
              (void)CLM_SetRGBL(LED0,MES_GetLedTemperature(LED0), color->RGBL.red, color->RGBL.green, color->RGBL.blue,color->RGBL.level,1000U);
              break;
            case COLOR_MODE_ACCURATE_RGBL:
              (void)CLM_SetAccurateRGBL(LED0,color->iRatio,MES_GetLedTemperature(LED0), color->RGBL.red, color->RGBL.green, color->RGBL.blue,(uint8_t)color->RGBL.level,1000U);
              break;
            case COLOR_MODE_LUV:
              (void)CLM_SetLUV(LED0,MES_GetLedTemperature(LED0), color->LUV.u, color->LUV.v, color->LUV.L, 1000U);
              break;
            case COLOR_MODE_ACCURATE_LUV:
              (void)CLM_SetAccurateLUV(LED0,color->iRatio,MES_GetLedTemperature(LED0), color->LUV.u, color->LUV.v, (uint8_t)color->LUV.L, 1000U);
              break;
            case COLOR_MODE_RGB:
              (void)CLM_SetRGB(LED0,MES_GetLedTemperature(LED0), color->RGB.red, color->RGB.green, color->RGB.blue,1000U);
              break;
            case COLOR_MODE_HSL:
              (void)CLM_SetHSL(LED0,MES_GetLedTemperature(LED0), color->HSL.hue, color->HSL.saturation, color->HSL.level, 1000U);
              break;
            default:
              break;
            }
        }
    }
#endif
}



void safeMonitorTimerExpired(SoftTimer_t *timer)
{
    uint16_t batteryVolt;
#if SAFETY_MONITOR_EN == 1U
    if ( MES_GetChipTemperature(&chipTemperature) == 0){
        /*  Over temperature protection level 1  */
        if (chipTemperature >= SW_OVER_TEMP_SUPER_HIGH){
            safetyState |= SW_OVER_TEMP_FLAG_SUPER_HIGH;
            TM_PostTask(TASK_ID_SAFETY_MONITOR);
        }
        /*  Over temperature protection level 0  */
        if (chipTemperature >= SW_OVER_TEMP_HIGH){
            safetyState |= SW_OVER_TEMP_FLAG_HIGH;
            TM_PostTask(TASK_ID_SAFETY_MONITOR);
        }
        /*  Over temperature protection release  */
        if ((chipTemperature <= SW_OVER_TEMP_RELEASE) && (safetyState !=0U) ){
            safetyState &= ~(SW_OVER_TEMP_FLAG_HIGH | SW_OVER_TEMP_FLAG_SUPER_HIGH);
            TM_PostTask(TASK_ID_SAFETY_MONITOR);
        }
    }
    if ( MES_GetBatteryVolt(&batteryVolt) == 0){
        /* Over volt */
        if (batteryVolt >= SW_OVER_VOLT_LEVEL){
            safetyState |= SW_OVER_VOLT;
            TM_PostTask(TASK_ID_SAFETY_MONITOR);
        }
        /* Over volt release*/
        if (batteryVolt <= SW_UNDER_VOLT_LEVEL){
            safetyState |= SW_UNDER_VOLT;
            TM_PostTask(TASK_ID_SAFETY_MONITOR);
        }
        /* under volt*/
        if (batteryVolt <= SW_OVER_VOLT_LEVEL_REELEASE){
            safetyState &= ~SW_OVER_VOLT;
            TM_PostTask(TASK_ID_SAFETY_MONITOR);
        }
        /* under volt release*/
        if (batteryVolt >= SW_UNDER_VOLT_LEVEL_RELEASE){
            safetyState &= ~SW_UNDER_VOLT;
            TM_PostTask(TASK_ID_SAFETY_MONITOR);
        }
    }
#endif
}


