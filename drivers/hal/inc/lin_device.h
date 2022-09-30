/**
 * @copyright 2015 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file lin_device.h
 */

#ifndef LIN_DEVICE_H__
#define LIN_DEVICE_H__

#include <stdint.h>
#include <stdbool.h>
#include <realplum.h>
#include "lins_sfr.h"
#include "linm_sfr.h"

#define E_LIN_STATUS_COMPLETE       (1U << 0)
#define E_LIN_STATUS_WAKEUP         (1U << 1)
#define E_LIN_STATUS_ERROR          (1U << 2)
#define E_LIN_STATUS_INT_REQ        (1U << 3)
#define E_LIN_STATUS_DATA_REQ       (1U << 4)
#define E_LIN_STATUS_ABORT          (1U << 5)
#define E_LIN_STATUS_IDLE_TIMEOUT   (1U << 6)
#define E_LIN_STATUS_ACTIVE         (1U << 7)

#define E_LIN_ERROR_BIT             (1U << 0)
#define E_LIN_ERROR_CHECKSUM        (1U << 1)
#define E_LIN_ERROR_TIMEOUT         (1U << 2)
#define E_LIN_ERROR_PARITY          (1U << 3)


#define E_LIN_TIME_INACTIVE_SEC_4       (0U)
#define E_LIN_TIME_INACTIVE_SEC_6       (1U)
#define E_LIN_TIME_INACTIVE_SEC_8       (2U)
#define E_LIN_TIME_INACTIVE_SEC_10      (3U)
#define E_LIN_TIME_INACTIVE_MAX         (3U) /*E_LIN_TIME_INACTIVE_SEC_10*/

#define E_LIN_TIME_WAKEUP_REPEAT_MS_180 (0U)
#define E_LIN_TIME_WAKEUP_REPEAT_MS_200 (1U)
#define E_LIN_TIME_WAKEUP_REPEAT_MS_220 (2U)
#define E_LIN_TIME_WAKEUP_REPEAT_MS_240 (3U)
#define E_LIN_TIME_WAKEUP_REPEAT_MAX    (3U)/*E_LIN_TIME_WAKEUP_REPEAT_MS_240*/


#define LIN_TX_SLEW_RATE_2_1V_PER_US    (0U)/* Slew from 40%*Vbat to 60%*Vbat @Vbat=13V,Cbus=1nF */
#define LIN_TX_SLEW_RATE_4_0V_PER_US    (1U)
#define LIN_TX_SLEW_RATE_4_9V_PER_US    (2U)
#define LIN_TX_SLEW_RATE_5_1V_PER_US    (3U)


#define LIN_BUFF_SIZE                  8U

/* Defines for LIN device to handle message direction.  */
typedef enum{
    LIN_MSG_TYPE_RX = 0U,  /*received data from sender for slave; */
    LIN_MSG_TYPE_TX,       /*data request from sender for slave; */
    LIN_MSG_TYPE_TX_EVENT, /*event trigger data request from sender for slave; */
}LinMessageType_t;

typedef enum{
    E_LIN_BAUDRATE_19200= 0U,
}LIN_BaudRate_t;

#define E_LIN_BAUDRATE_MAX   (1U)/*E_LIN_BAUDRATE_19200*/


typedef enum{
    LIN_CHECKSUM_CLASSIC = 0U,  /*!< classic checksum does not include ID Byte.  */ 
    LIN_CHECKSUM_ENHANCED       /*!< "enhanced" checksum includes ID Byte.  */ 
}ChecksumType_t;


typedef enum{
  AA_SWITCH_OFF = 0U,
  AA_SWITCH_ON,
}AutoaddressingSwitchState_t;

/**
* @brief A structure to represent LIN frame information.
*/
typedef struct { 
    uint8_t             frame_id;                  /*!< frame ID.  */ 
    LinMessageType_t    msg_type;                  /*!< Type of message, rx,tx or event trigger.  */ 
    uint8_t             checksum;                  /*!< Type of checksum (enhanced or classic).  */ 
    uint8_t             length;                    /*!< Number of bytes in the data. */ 
    uint8_t             data[LIN_BUFF_SIZE];       /*!< data buffer. */
    uint8_t             responseValid;
    uint8_t             frameIsValid;
    uint8_t             eventTriggered;                  /*!< event triggered when the msg_type is LIN_MSG_TYPE_TX_EVENT */
    uint8_t             linkedEventTriggerFidTableIndex; /*!< event triggered when the msg_type is LIN_MSG_TYPE_TX_EVENT */
} LIN_Device_Frame_t;


typedef void (*lin_cb_func_t)(LIN_Device_Frame_t const *frame);
typedef void (*lin_stack_cb_func_t)(void);

void LINS_Init(void);
void LINS_AutoAddressingSwitch(AutoaddressingSwitchState_t state);
void LINS_SetSlaveModuleToSleep(void);
void LINS_RegisterLINStack_IRQ(lin_stack_cb_func_t callback);




void LINM_Init(void);
void LINM_RegisterLINM_IRQ(lin_cb_func_t cb);
void LINM_UnRegisterLINM_IRQ(void);
int8_t LINM_SendFrame(LIN_Device_Frame_t *frame);




#endif /* __LIN_DEVICE_H__ */
