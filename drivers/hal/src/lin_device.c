/**
 * @copyright 2015 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file lin_device.c
 */

#include <stdint.h>
#include <string.h>
#include <appConfig.h>
#include "timer_device.h"
#include "errno.h"
#include <isrfuncs.h>
#include <lin_device.h>

static lin_stack_cb_func_t lin_stack_callback = NULL;
static lin_cb_func_t linm_callback = NULL;

typedef struct {
    uint16_t divider;
    uint16_t multiplier;
    uint8_t prescale;
} lin_speed_setting_t;

/**********************************************************************  
 *    Local definitions
 *********************************************************************/   

#if SYS_MAIN_CLOCK_DIV == CLOCK_DIV_1   /* 16MHz */
  static const lin_speed_setting_t lins_speed_map[E_LIN_BAUDRATE_MAX] = {
      [E_LIN_BAUDRATE_19200] = {.divider = 200U, .multiplier = 0U, .prescale = 1U},
  };
  static const lin_speed_setting_t linm_speed_map[E_LIN_BAUDRATE_MAX] = {
      [E_LIN_BAUDRATE_19200] = {.divider = 200U, .multiplier = 0U, .prescale = 1U},
  };
#else   /* 2MHz */
  #error only support SYS_MAIN_CLOCK_DIV = CLOCK_DIV_1!
#endif

void lins_SetTiming(LIN_BaudRate_t BaudRate);
int8_t LINS_WriteBuffer(uint8_t buff[], uint32_t dataLength);
int8_t LINS_ReadBuffer(uint8_t *buff, uint8_t dataLength);
int8_t is_valid_frame(LIN_Device_Frame_t *frameTable, uint8_t id);


int8_t LINM_WriteBuffer(uint8_t buff[], uint32_t dataLength);
int8_t LINM_ReadBuffer(uint8_t buff[], uint32_t dataLength);
void linm_SetTiming(LIN_BaudRate_t BaudRate);

 void lins_SetTiming(LIN_BaudRate_t BaudRate)
{
    LINS_SFRS->BTDIV07          = (uint8_t)(lins_speed_map[BaudRate].divider & 0xFFU);
    LINS_SFRS->BITTIME.BTDIV8   = (uint8_t)(lins_speed_map[BaudRate].divider >> 8U);
    LINS_SFRS->BITTIME.PRESCL   = (uint8_t)(lins_speed_map[BaudRate].prescale);
}


int8_t LINS_WriteBuffer(uint8_t buff[], uint32_t dataLength)
{
    int8_t result;
    if (dataLength > 8U){
        result = -1;
    }else{
        result = 0;
        for (uint8_t i = 0U; i < dataLength; i++){
            LINS_SFRS->DATABUFF[i].DATA = buff[i];
        }
    }
    return result;
}


int8_t LINS_ReadBuffer(uint8_t *buff, uint8_t dataLength)
{
    int8_t result;
    if (dataLength > 8U){
        result = -1;
    }else{
        result = 0;
        for (uint8_t i = 0U; i < dataLength; i++){
            buff[i] = LINS_SFRS->DATABUFF[i].DATA;
        }
    }
    return result;
}


void LINS_Init(void)
{
    LINS_REG_CTRL.SLEEP = 0U;                     /*  wake up lins when init if sleep  */
    /* Config GPIO to LIN mode, enable transmission */
    IOCTRLA_SFRS->LIN.LINS_HWMODE       = 1U;     /* Hardware Mode Enabled. LIN slave peripheral writes/read the LIN I/O pin.*/
    IOCTRLA_SFRS->LIN.LINS_RXENA        = 1U;     /* LIN receive enable. */
    IOCTRLA_SFRS->LIN.LINS_TXENA        = 1U;     /* LIN transmit enable.*/
    IOCTRLA_SFRS->LIN.LINS_PU30K_ENA    = 1U;     /* LIN 30K pullup enable.*/
    
    IOCTRLA_SFRS->LIN.SWON    = 1U;      /* SWITCH ON Between LIN_IN and LIN_OUT; */
  
    LINS_SFRS->BUSTIME.BUSINACTIVE      = E_LIN_TIME_INACTIVE_MAX;
    LINS_SFRS->BUSTIME.WUPREPEAT        = E_LIN_TIME_WAKEUP_REPEAT_MS_180;
    LINS_SFRS->CTRL.RST_INT_ERR         = 0x03U;    /* reset error,reset interrupt */
    
    lins_SetTiming(E_LIN_BAUDRATE_19200);       /* set the maximum supported baud rate */

}


void LINS_RegisterLINStack_IRQ(lin_stack_cb_func_t callback)
{
    lin_stack_callback = callback;
    NVIC_EnableIRQ(LINS_IRQn);
}

void LINS_SetSlaveModuleToSleep(void)
{
   LINS_REG_CTRL.SLEEP = 1U;
}

void LINS_AutoAddressingSwitch(AutoaddressingSwitchState_t state)
{
   IOCTRLA_SFRS->LIN.SWON = (uint8_t)state;
}


void LINS_Handler(void)
{
    if (lin_stack_callback != NULL){
        lin_stack_callback();
    }
}



static LIN_Device_Frame_t linmFrame;


int8_t LINM_WriteBuffer(uint8_t buff[], uint32_t dataLength)
{
    int8_t result;
    if (dataLength > 8U){
        result = -1;
    }else{
        result = 0;
        for (uint8_t i = 0U; i < dataLength; i++){
            LINM_SFRS->DATABUFF[i].DATA = (uint8_t)buff[i];
        }
    }
    return result;
}


int8_t LINM_ReadBuffer(uint8_t buff[], uint32_t dataLength)
{
    int8_t result;
    if (dataLength > 8U){
        result = -1;
    }else{
        result = 0;
        for (uint8_t i = 0U; i < dataLength; i++){
            buff[i] = (uint8_t)LINM_SFRS->DATABUFF[i].DATA;
        }
    }
    return result;
}


void linm_SetTiming(LIN_BaudRate_t BaudRate)
{
    LINM_SFRS->BTDIV07 = (uint8_t)linm_speed_map[BaudRate].divider & 0xFFU;
    LINM_SFRS->BITTIME.BTDIV8 = (uint8_t)linm_speed_map[BaudRate].divider >> 8U;
    LINM_SFRS->BITTIME.BTMULT = (uint8_t)linm_speed_map[BaudRate].multiplier;
    LINM_SFRS->BITTIME.PRESCL = (uint8_t)linm_speed_map[BaudRate].prescale;
}


void LINM_Init(void)
{
    IOCTRLA_SFRS->LIN.LINM_HWMODE       = 1U;    /* Hardware Mode Enabled. LIN slave peripheral writes/read the LIN I/O pin.*/
    IOCTRLA_SFRS->LIN.LINM_RXENA        = 1U;    /* LIN receive enable. */
    IOCTRLA_SFRS->LIN.LINM_TXENA        = 1U;    /* LIN transmit enable.*/
    IOCTRLA_SFRS->LIN.LINM_PU1K_ENA     = 1U;    /* LIN 1K pullup enable.*/
    IOCTRLA_SFRS->LIN.SWON = 0U;                 /* 1: disable Master function, 0: enable Master function on LINOUT pin */

    LINM_SFRS->BUSTIME.BUSINACTIVE      = E_LIN_TIME_INACTIVE_SEC_4;
    LINM_SFRS->BUSTIME.WUPREPEAT        = E_LIN_TIME_WAKEUP_REPEAT_MS_180;
    LINM_SFRS->CTRL.RSTERR              = 1U;
    LINM_SFRS->CTRL.RSTINT              = 1U;
    
    linm_SetTiming(E_LIN_BAUDRATE_19200);
}

void LINM_RegisterLINM_IRQ(lin_cb_func_t cb)
{
    linm_callback = cb;
    NVIC_EnableIRQ(LINM_IRQn);
}


void LINM_UnRegisterLINM_IRQ(void)
{
    linm_callback = NULL;
    NVIC_DisableIRQ(LINM_IRQn);
}

int8_t LINM_SendFrame(LIN_Device_Frame_t *frame)
{
    linmFrame = *frame;
    LINM_SFRS->ID        = frame->frame_id;
    if (frame->msg_type == LIN_MSG_TYPE_TX){
        LINM_SFRS->DL.LENGTH = frame->length;
        LINM_SFRS->CTRL.TRANSMIT = 1U;
    }else{
        LINM_SFRS->DL.LENGTH = frame->length;
        LINM_SFRS->CTRL.TRANSMIT = 0U;
    }
    LINM_SFRS->DL.ENHCHK = frame->checksum;
    
    (void)LINM_WriteBuffer(frame->data, (uint32_t)frame->length);
    LINM_SFRS->CTRL.STARTREQ = 1U;
    
    return 0;
}

void LINM_Handler(void)
{
    uint8_t status   = (uint8_t)LINM_STATUS_REG;
    if( ((status & (uint8_t)E_LIN_STATUS_ERROR) == 0U) &&  ((status & (uint8_t)E_LIN_STATUS_WAKEUP) == 0U) ){
        if ( (status & (uint8_t)E_LIN_STATUS_COMPLETE) != 0U ){
            if (linmFrame.msg_type == LIN_MSG_TYPE_RX){
                linmFrame.length   = (uint8_t)LINM_SFRS->DL.LENGTH;
                (void)LINM_ReadBuffer(linmFrame.data, LIN_BUFF_SIZE);
                if (linm_callback != NULL){
                    linm_callback(&linmFrame);
                }
            }
        } 
    }
    LINM_SFRS->CTRL.RSTERR              = 1U;    /*reset error*/
    LINM_SFRS->CTRL.RSTINT              = 1U;    /*reset interrupt*/
}


/* Please make sure here and don't delete these functions!!!!*/
/* LIN slave wake up interrupt ISR */
void WULINS_Handler(void)
{
    WICA_SFRS->CTRL.LINIRQCLR   = 1U;
}
/* LIN master wake up interrupt ISR */
void WULINM_Handler(void)
{
    WICA_SFRS->CTRL.LINMIRQCLR  = 1U;
}
