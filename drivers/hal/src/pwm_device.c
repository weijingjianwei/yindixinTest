/**
 * @copyright 2015 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file pwm_device.c
 */


#include <stdint.h>
#include <string.h>
#include "realplum.h"
#include "errno.h"
#include "pwm_device.h"
#include "appConfig.h"
#include <isrfuncs.h>
#include <adc_device.h>
#include "gpio_device.h"
#include <pdsTask.h>

#if defined PWM_SFRS

static uint16_t currPWM[LED_NUM][3];
static uint8_t ledIsActive = FALSE;

void PWM_Handler(void)
{

}

/**
 * @brief The instance of PMW device
 */
void PWM_Init(PwmChannel_t channel, PwmPrescaler_t divide, uint16_t period, uint8_t invertEn)
{
    IOCTRLA_SFRS->LED.HWMODE |= (1U << channel);
    PWM_SFRS->BASE[channel].PRESCALESEL = (uint8_t)divide;
    PWM_SFRS->BASE[channel].PERIOD = period;
    PWM_SFRS->CTRL.INVERT |= (uint8_t)invertEn << channel;
    
    /* set duty cycle to 0*/
    PWM_SFRS->PULSE[channel].PFALL = 0U;
    PWM_SFRS->PULSE[channel].PRISE = 0U;
    
}


/**
 * @brief The instance of PMW device
 */
void PWM_SetMatchValue(PwmChannel_t channel, uint16_t matchRisingValue, uint16_t matchFaillValue)
{
    PWM_SFRS->PULSE[channel].PRISE = matchRisingValue;
    PWM_SFRS->PULSE[channel].PFALL = matchFaillValue;
}

/**
 * @brief The instance of PMW device
 */
void PWM_StartAndUpdate(void)
{
    PWM_SFRS->CTRL.ENAREQ = (1U << PWM_CHANNEL_0) | (1U << PWM_CHANNEL_1) | (1U << PWM_CHANNEL_2);
    PWM_SFRS->CTRL.UPDATE = (1U << PWM_CHANNEL_0) | (1U << PWM_CHANNEL_1) | (1U << PWM_CHANNEL_2);
}


/**
 * @brief The instance of PWM_SetRGBValue
 */
void PWM_SetRGBValue(uint8_t ledNo,uint16_t red, uint16_t green, uint16_t blue)
{
    if (ledNo < LED_NUM){
        uint16_t r,g,b;

        r= (uint16_t)(((uint32_t)red*31U   >> 5U) >> (16U-PWM_VALUE_MAX_POS));
        g= (uint16_t)(((uint32_t)green*31U >> 5U) >> (16U-PWM_VALUE_MAX_POS));
        b= (uint16_t)(((uint32_t)blue*31U  >> 5U) >> (16U-PWM_VALUE_MAX_POS));
        
        currPWM[ledNo][PHY_CHANNEL_RED]   = r;
        currPWM[ledNo][PHY_CHANNEL_GREEN] = g;
        currPWM[ledNo][PHY_CHANNEL_BLUE]  = b;
        
        if ((red != 0U) && (green != 0U) && (blue != 0U)){
            ledIsActive = TRUE;
        }else if ( red >= (65535U/256U) || green >= (65535U/256U) || blue >= (65535U/8U)){
            /* simulate falling edge measurement */
            r = (r == 0U)? 1U: r;
            g = (g == 0U)? 1U: g;
            b = (b == 0U)? 1U: b;
            ledIsActive = TRUE;
        }else{
            ledIsActive = FALSE;
        }
        
        PWM_SetMatchValue(PHY_CHANNEL_RED,  ((uint16_t)PWM_VALUE_MAX - r),     (uint16_t)PWM_VALUE_MAX);
        PWM_SetMatchValue(PHY_CHANNEL_GREEN,((uint16_t)PWM_VALUE_MAX - g),     (uint16_t)PWM_VALUE_MAX);
        PWM_SetMatchValue(PHY_CHANNEL_BLUE, ((uint16_t)PWM_VALUE_MAX - b),     (uint16_t)PWM_VALUE_MAX);
        
        PWM_StartAndUpdate();
    }
}

int8_t PWM_UpdateFInished(void)
{
    int8_t result = -1;
    uint32_t update = PWM_SFRS->CTRL.UPDATE;
    if (update == 0U){
        result = 0;
    }
    return result;
}

uint16_t PWM_GetCurrentPWM(uint8_t ledNo,uint8_t channel)
{
    return currPWM[ledNo][channel];
}

uint8_t PWM_LedIsActive(void)
{
    return ledIsActive;
}
/**
 * @brief The instance of PMW device
 */
void PWM_RegisterPWMMatchIRQ(PwmChannel_t channel, PWMTriggerEdge_t edge, PwmIsrCallback_t callback)
{
    if (edge == PWM_TRIGGER_POS_EDGE){
        PWM_SFRS->INTCTRL.ENABLE |=( 1U << (uint16_t)channel);
    }else{
        PWM_SFRS->INTCTRL.ENABLE |=( 1U << ((uint16_t)channel + 3U));
    }
    PWM_SFRS->CTRL.UPDATE |= (1U << (uint16_t)channel);
    NVIC_EnableIRQ(PWM_IRQn);
}

/**
 * @brief The instance of PMW device
 */
void PWM_UnRegisterPWMMatchIRQ(PwmChannel_t channel)
{
    PWM_SFRS->INTCTRL.ENABLE &=~( 1U << (uint16_t)channel);
    PWM_SFRS->INTCTRL.ENABLE &=~( 1U << ((uint16_t)channel + 4U));
    PWM_SFRS->CTRL.UPDATE |= (uint8_t)(1U << (uint16_t)channel);
    NVIC_DisableIRQ(PWM_IRQn);
}

/**
 * @brief The instance of PMW device
 */
void PWM_RegisterPWMReloadIRQ(PwmChannel_t channel, PwmIsrCallback_t callback)
{
    PWM_SFRS->CTRL.ENAREQ = (1U << PWM_CHANNEL_0) | (1U << PWM_CHANNEL_1) | (1U << PWM_CHANNEL_2);
    PWM_SFRS->INTUPDATED.ENABLE |=( 1U << (uint8_t)channel );
    PWM_SFRS->CTRL.UPDATE |= (1U << (uint8_t)channel);
    NVIC_EnableIRQ(PWM_IRQn);
}

/**
 * @brief The instance of PMW device
 */
void PWM_UnRegisterPWMReloadIRQ(PwmChannel_t channel)
{
    PWM_SFRS->INTUPDATED.ENABLE &=~(1U << channel);
    PWM_SFRS->CTRL.UPDATE |= (1U << channel);
    NVIC_DisableIRQ(PWM_IRQn);
}

/**
 * @brief The instance of PMW device
 */
void PWM_Enable(PwmChannel_t channel)
{
    PWM_SFRS->CTRL.ENAREQ |= (1U << channel);
    PWM_SFRS->CTRL.UPDATE |= (1U << channel);
}
/**
 * @brief The instance of PMW device
 */
void PWM_EnableAllChannels(void)
{
    PWM_SFRS->CTRL.ENAREQ = 0x07U;

}

/**
 * @brief The instance of PMW device
 */
void PWM_Disable(PwmChannel_t channel)
{
    PWM_SFRS->CTRL.ENAREQ &= ~(1U << channel);
    PWM_SFRS->CTRL.UPDATE |= (1U << channel);
}

/**
 * @brief The instance of PMW device
 */
void PWM_DisableAllChannels(void)
{
    PWM_SFRS->CTRL.ENAREQ = 0x00U;
    PWM_SFRS->CTRL.UPDATE = 0x07U;
}
/**
 * @brief The instance of PMW device
 */
void PWM_SetPrescaler(PwmChannel_t channel, PwmPrescaler_t divide)
{
    PWM_SFRS->BASE[channel].PRESCALESEL = (uint8_t)divide;
    PWM_SFRS->CTRL.UPDATE |= (1U << channel);
}

/**
 * @brief The instance of PMW device
 */
void PWM_SetPeriod(PwmChannel_t channel, uint16_t period)
{
    PWM_SFRS->BASE[channel].PERIOD = period;
    PWM_SFRS->CTRL.UPDATE |= (1U << channel);
}

/**
 * @brief The instance of PMW device
 */
void PWM_SetInvert(PwmChannel_t channel, uint8_t invertEn)
{
    PWM_SFRS->CTRL.INVERT |= (uint8_t)((uint8_t)invertEn << channel);
    PWM_SFRS->CTRL.UPDATE |= (uint8_t)(1U << channel);
}

/**
 * @brief The instance of PMW device
 */
int8_t PWM_SetInitChannelCurrent(PwmChannel_t channel)
{
    SYSCTRLA_SFRS->TRIM_ACCESS_KEY.TRIM_ACCESS_KEY = 0x0EU;
    if (channel == PWM_CHANNEL_0){
        SYSCTRLA_SFRS->LED0.TRIM0     = HWCFG_GetLEDTrimValue((uint8_t)channel);
        SYSCTRLA_SFRS->LED0.TRIM0_OFF = HWCFG_GetOffLEDTrimValue((uint8_t)channel);
    }else if (channel == PWM_CHANNEL_1){
        SYSCTRLA_SFRS->LED1.TRIM1     = HWCFG_GetLEDTrimValue((uint8_t)channel);
        SYSCTRLA_SFRS->LED1.TRIM1_OFF = HWCFG_GetOffLEDTrimValue((uint8_t)channel);
    }else{
        SYSCTRLA_SFRS->LED1.TRIM2     = HWCFG_GetLEDTrimValue((uint8_t)channel);
        SYSCTRLA_SFRS->LED1.TRIM2_OFF = HWCFG_GetOffLEDTrimValue((uint8_t)channel);
    }
    SYSCTRLA_SFRS->PMU_TRIM.RESISTOR_TRIM =  (uint8_t)HWCFG_GetV2ITrimValue();
    
    SYSCTRLA_SFRS->TRIM_ACCESS_KEY.TRIM_ACCESS_KEY = 0x00U;
    
/*    SYSCTRLA_SFRS->TRIM_ACCESS_KEY.TRIM_LOCK = 1u;*/
    return 0;
}

void PWM_TurnOffChannelCurrent(void)
{
    SYSCTRLA_SFRS->TRIM_ACCESS_KEY.TRIM_ACCESS_KEY = 0x0EU;
    SYSCTRLA_SFRS->LED0.TRIM0     = 0;
    SYSCTRLA_SFRS->LED0.TRIM0_OFF = 0;
    SYSCTRLA_SFRS->LED1.TRIM1     = 0;
    SYSCTRLA_SFRS->LED1.TRIM1_OFF = 0;
    SYSCTRLA_SFRS->LED1.TRIM2     = 0;
    SYSCTRLA_SFRS->LED1.TRIM2_OFF = 0;
    
    SYSCTRLA_SFRS->TRIM_ACCESS_KEY.TRIM_ACCESS_KEY = 0x00U;
/*    SYSCTRLA_SFRS->TRIM_ACCESS_KEY.TRIM_LOCK = 1u;*/
}


#endif