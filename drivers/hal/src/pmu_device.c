/**
 * @copyright 2017 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file pmu_device.c
 */

#include <stddef.h>
#include <pmu_device.h>
#include <errno.h>
#include <isrfuncs.h>
#include <gpio_device.h>
#include <lin_device.h>
#include <wica_sfr.h>
#include <pwm_device.h>
#include <wdt_device.h>

static __INLINE void WDTA_Start(void);

void BOR_Handler(void) 
{
    
}
   
void PMU_BORInit(Bor1V5Thres_t lowThreshold, Bor3V3Thres_t highThreshold)
{
    CRGA_SFRS->BORCONFIG.BOR1V5THRESH = (uint8_t)lowThreshold;
    CRGA_SFRS->BORCONFIG.BOR3V3THRESH = (uint8_t)highThreshold;
    CRGA_SFRS->BORCONFIG.BORBIASOVERRIDESEL = 0U;
    CRGA_SFRS->BORACTION.VDD1V5 = (uint8_t)PMU_BROWNOUT_RESET;
    CRGA_SFRS->BORACTION.VDD3V3 = (uint8_t)PMU_BROWNOUT_RESET;
    
    CRGA_SFRS->OVTEMPCONFIG.TEMP_SENSOR_DIS = 0U;/* enable temperature sensor */
    
    EVTHOLD_SFRS->HOLD = 0U;
}

void PMU_WakeTimerInit(PMU_WAKEUP_TIMEER_MODE_t mode, PMU_WAKEUP_TIMEER_Interval_t interval)
{
  if (mode == WAKEUP_TIMEER_DISABLE){
      WICA_SFRS->CTRL.WU_TIMERENA = 0U;
  }else{
      WICA_SFRS->CTRL.WU_TIMERTAPSEL = (uint8_t)interval;
      WICA_SFRS->CTRL.WU_TIMERENA = 1U;
  }
}

void PMU_EnterToDeepSleepMode(void)
{
    PWM_DisableAllChannels();        /* disable Led channels */
    (void)PWM_TurnOffChannelCurrent();
  
    /*  Add GPIO function here for low power mode*/

    /*  end of GPIO settigns   */

    CRGA_SFRS->OVTEMPCONFIG.OVTEMPENA = 0U;       /* Disable temperature sensor when sleep */
    CRGA_SFRS->OVTEMPCONFIG.TEMP_SENSOR_DIS = 1U;
    
    IOCTRLA_SFRS->LIN.LINS_PU30K_ENA    = 0U;     /* LIN 30K pullup disable low power consumption.*/

    WICA_SFRS->CTRL.GPIOENA     = 1U;   /* enable GPIO wake up */
    WICA_SFRS->CTRL.LINSENA     = 1U;   /* enable LIN slave wake up */
    WICA_SFRS->CTRL.LINMENA     = 0U;   /* disable LIN master wake up */
    /* clear all of wake up flags */
    WICA_SFRS->CTRL.LINIRQCLR           = 1U;
    WICA_SFRS->CTRL.GPIOIRQCLR          = 1U;
    WICA_SFRS->CTRL.WU_TIMERIRQCLR      = 1U;
    WICA_SFRS->CTRL.LINMIRQCLR          = 1U;
    
    NVIC_EnableIRQ(Lullaby_IRQn);/*  enable  Lullaby interrupt*/
    EVTHOLD_SFRS->HOLD = 1U;     /*  trigger ISR then entering sleep in ISR */
    /* Waiting to enter sleep mode */
    for(;;){
    }
}

/* Please make sure here and don't delete these functions!!!! or it would cause severe error*/
/* hibernate wake up interrupt ISR */
void Lullaby_Handler(void)
{
    /* Set lin to sleep mode */
    LINS_REG_CTRL.SLEEP = 1U;
    /*  Enter sleep mode  */
    PMUA_SFRS->CTRL.HIBERNATE = 1U;
}

#pragma optimize=none
static __INLINE void WDTA_Start(void)
{
    WDTA_SFRS->CTRL.TIMEOUTSEL     = (uint8_t)WDTA_INTERVAL_13000MS;
    CRGA_SFRS->WDTACTION.WDTACTION = 1U; /*  reset */
    WDTA_SFRS->CLEAR = 0x3c574ad6U;
    WDTA_SFRS->STOP = 0U;                /* start wdt*/
}


void NMI_Handler(void)
{
    WDTA_Start();
    for(;;){}
}

void HardFault_Handler(void)
{
    WDTA_Start();
    for(;;){}
}

void SVC_Handler(void)
{
    WDTA_Start();
    for(;;){}
}

void PendSV_Handler(void)
{
    WDTA_Start();
    for(;;){}
}


