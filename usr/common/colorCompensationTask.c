#include <colorCompensationTask.h>

#define COLOR_COMP_INTERVAL 400U
#define COLOR_COMP_DIMMING_INTERVAL 0U /* (COLOR_COMP_INTERVAL - 500U)*/
static TaskState_t ccpState = TASK_STATE_INIT;
void doColorCompensationHandle(void);
void colorCompTimerExpired(SoftTimer_t *timer);

static SoftTimer_t colorCompTimer = {
    .mode     = TIMER_PERIODIC_MODE,
    .interval = COLOR_COMP_INTERVAL, /* color compensation detect time interval */
    .handler  = colorCompTimerExpired
};


void colorCompTimerExpired(SoftTimer_t *timer)
{
    /* color compensation only starts until the diming operation has been finished */
    if ( CLM_FadingIsFinished(LED0) == TRUE){
        TM_PostTask(TASK_ID_COLOR_COMPENSATION);
    }
}

void CCP_TaskHandler(void)
{
    switch(ccpState){
    case TASK_STATE_INIT:
      //SoftTimer_Start(&colorCompTimer);
      ccpState = TASK_STATE_ACTIVE;
      break;
    case TASK_STATE_ACTIVE:
      doColorCompensationHandle(); /* do color compensation operation if temperature has changed */
      break;
    default:
      break;
    }
}

void doColorCompensationHandle(void)
{
    /* Get current LED temperature */
    if (PWM_LedIsActive() == TRUE){
        ColorParam_t const *color = CLM_GetCurrentColorParams(LED0);
        if(color != NULL){
            switch(color->colorMode){
            case COLOR_MODE_XYY:
              (void)CLM_SetXYY(LED0, MES_GetLedTemperature(LED0), color->XYY.x, color->XYY.y, color->XYY.Y, COLOR_COMP_DIMMING_INTERVAL);
              break;
            case COLOR_MODE_ACCURATE_XYY:
              (void)CLM_SetAccurateXYY(LED0,color->iRatio, MES_GetLedTemperature(LED0), color->XYY.x, color->XYY.y, (uint8_t)color->XYY.Y, COLOR_COMP_DIMMING_INTERVAL);
              break;
            case COLOR_MODE_RGBL:
              (void)CLM_SetRGBL(LED0,MES_GetLedTemperature(LED0), color->RGBL.red, color->RGBL.green, color->RGBL.blue,color->RGBL.level,COLOR_COMP_DIMMING_INTERVAL);
              break;
            case COLOR_MODE_ACCURATE_RGBL:
              (void)CLM_SetAccurateRGBL(LED0,color->iRatio,MES_GetLedTemperature(LED0), color->RGBL.red, color->RGBL.green, color->RGBL.blue,(uint8_t)color->RGBL.level,COLOR_COMP_DIMMING_INTERVAL);
              break;
            case COLOR_MODE_sRGBL:
              (void)CLM_SetSRGBL(LED0,MES_GetLedTemperature(LED0), color->RGBL.red, color->RGBL.green, color->RGBL.blue,color->RGBL.level,COLOR_COMP_DIMMING_INTERVAL);
              break;
            case COLOR_MODE_ACCURATE_sRGBL:
              (void)CLM_SetAccurateSRGBL(LED0,color->iRatio,MES_GetLedTemperature(LED0), color->RGBL.red, color->RGBL.green, color->RGBL.blue,(uint8_t)color->RGBL.level,COLOR_COMP_DIMMING_INTERVAL);
              break;
            case COLOR_MODE_LUV:
              (void)CLM_SetLUV(LED0,MES_GetLedTemperature(LED0), color->LUV.u, color->LUV.v, color->LUV.L, COLOR_COMP_DIMMING_INTERVAL);
              break;
            case COLOR_MODE_ACCURATE_LUV:
              (void)CLM_SetAccurateLUV(LED0,color->iRatio,MES_GetLedTemperature(LED0), color->LUV.u, color->LUV.v, (uint8_t)color->LUV.L, COLOR_COMP_DIMMING_INTERVAL);
              break;
            case COLOR_MODE_RGB:
              (void)CLM_SetRGB(LED0,MES_GetLedTemperature(LED0), color->RGB.red, color->RGB.green, color->RGB.blue,COLOR_COMP_DIMMING_INTERVAL);
              break;
            case COLOR_MODE_HSL:
              (void)CLM_SetHSL(LED0,MES_GetLedTemperature(LED0), color->HSL.hue, color->HSL.saturation, color->HSL.level, COLOR_COMP_DIMMING_INTERVAL);
              break;
            default:
              break;
            }
        }
    }
}

