/**
 * @copyright 2018 indie Semiconductor
 * 
 * This file is proprietary to indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of indie Semiconductor.
 * 
 * @file lins_sfr.h
 */

#ifndef LINS_SFR_H__
#define LINS_SFR_H__

#include <stdint.h>

#define LINS_DL_LENGTH_POS              (0)
#define LINS_DL_DISBITMON_POS           (4)
#define LINS_DL_ENHCHK_POS              (7)

#define DATAACK_TPYE_RECEIVE              (0x01)
#define DATAACK_TPYE_TRANSMIT             (0x03)


#define ERROR_BITMON_POS           (0)     /*!< Bit Monitor Error when sentding data to Master to trigger ISR*/
#define ERROR_CHK_POS              (1)     /*!< Checksum Error */
#define ERROR_TIMEOUT_POS          (2)     /*!< Timeout Error -> 1. send wake up no master request trigger ISR; 2. feeding Data length and ACK to LIN IP too later trigger ISR*/
#define ERROR_PARITY_POS           (3)     /*!< Parity Error. Identifier parity error */


#define LINS_VALID_ERRORS          ( (1U << ERROR_BITMON_POS) | (1U << ERROR_CHK_POS) )

#define ERROR_FRAMEERR_POS         (6)     /*!< Byte Field Framing Error */
#define LINS_INCOMPLETE_FRAME_TIMEOUT      (1U << ERROR_FRAMEERR_POS)

typedef union{
  struct{
    uint8_t  DATA;                          /*<! Data Buffer 1 +0x000 */
    uint8_t  _RESERVED_01[3];                   /* +0x001 */
  };
  uint32_t WORD;
}LINS_DATABUF_t;


typedef union {
  struct {
     uint8_t                    :  1; /*    (reserved) */
     uint8_t WAKEUPREQ          :  1; /*!< WakeUp Request */
     uint8_t RST_INT_ERR        :  2; /*!< Reset Reset interrupt, Error */
     uint8_t DATAACK            :  1; /*!< Data Acknowledgement */
     uint8_t TRANSMIT           :  1; /*!< Transmit Operation */
     uint8_t SLEEP              :  1; /*!< Sleep Request */
     uint8_t STOP               :  1; /*!< Stop Register */
  };
  uint8_t BYTE;
}LINS_CTRL_t;


typedef union{
  struct {
    uint8_t                           :  1; /*   (reserved) */
    uint8_t  WAKEUPREQ                :  1; /*!< WakeUp Request when LINS send a wakeup signal*/
    uint8_t  RST_INT_ERR              :  2; /*!< Reset Reset interrupt, Error */
    uint8_t  DATAACK_TPYE             :  2; /*!< Data Acknowledgement type : 0x01: received ack, 0x03: Transmit ack*/
    uint8_t  SLEEP                    :  1; /*!< Sleep Request */
    uint8_t  STOP                     :  1; /*!< Stop Register */
  };
  uint8_t BYTE;
}LINS_CTRL_T2_t;


typedef union{
  struct {
     uint8_t COMPLETE       :  1;
     uint8_t WAKEUP         :  1;
     uint8_t ERROR          :  1; /*!< Lin Error */
     uint8_t INTR           :  1; /*!< Interupt Request */
     uint8_t DATAREQ        :  1; /*!< Data Request */
     uint8_t ABORTED        :  1;
     uint8_t BUSIDLETIMEOUT :  1; /*!< BUS Idle Timeout */
     uint8_t ACTIVE         :  1; /*!< Lin Bus Active */
  };
  uint8_t BYTE;
}LINS_STATUS_t;

typedef union {
  struct{
     uint8_t BITMON  :  1; /*!< Bit Error */
     uint8_t CHK     :  1; /*!< Checksum Error */
     uint8_t TIMEOUT :  1; /*!< Timeout Error */
     uint8_t PARITY  :  1; /*!< Parity Error */
     uint8_t         :  4; /*    (reserved) */
  };
  uint8_t BYTE;
}LINS_ERROR_t;

typedef union{
  struct {
     uint8_t LENGTH :  4; /*!< Data Length */
     uint8_t        :  3; /*    (reserved) */
     uint8_t ENHCHK :  1; /*!< Enhancement Check */
  };
  uint8_t BYTE;
}LINS_DL_t;

typedef union{
  struct {
    uint8_t  BTDIV8                   :  1; /*!< Bt Div Most Significant bit */
    uint8_t                           :  5; /*   (reserved) */
    uint8_t  PRESCL                   :  2; /*!< Prescaler Register */
  };
  uint8_t BYTE;
}LINS_BITTIME_t;

typedef union{
  struct {
    uint8_t  WUPREPEAT                :  2; /*!< wakeup repeat time */
    uint8_t  BUSINACTIVE              :  2; /*!< Bus Inactivity Time */
    uint8_t                           :  4; /*   (reserved) */
  };
  uint8_t BYTE;
}LINS_BUSTIME_t;


/**
 * @brief A structure to represent Special Function Registers for LINS.
 */
typedef struct {
  LINS_DATABUF_t        DATABUFF[8];
  LINS_CTRL_t           CTRL;
  uint8_t               _RESERVED_20[3];
  LINS_STATUS_t         STATUS;
  uint8_t               _RESERVED_24[3];
  LINS_ERROR_t          ERROR;
  uint8_t               _RESERVED_30[3];
  LINS_DL_t             DL;
  uint8_t               _RESERVED_2C[3];
  uint8_t               BTDIV07;                /*<! Bt Div LSBs +0x030 */
  uint8_t               _RESERVED_31[3];        /* +0x031 */
  LINS_BITTIME_t        BITTIME;
  uint8_t               _RESERVED_34[3]; 
  uint8_t               ID;
  uint8_t               _RESERVED_39[3];        /* +0x039 */
  LINS_BUSTIME_t        BUSTIME;
  uint8_t               _RESERVED_3C[3]; 
} LINS_SFRS_t;

/**
 * @brief The starting address of LINS SFRS.
 */
#define LINS_REG_DATA(index)    (*(__IO            uint8_t *)(0x50010700 + index*4U))
#define LINS_REG_CTRL           (*(__IO     LINS_CTRL_T2_t *)(0x50010720))
#define LINS_REG_STATUS         (*(__IO            uint8_t *)(0x50010724))
#define LINS_REG_ERROR          (*(__IO       LINS_ERROR_t *)(0x50010728))
#define LINS_REG_DL             (*(__IO            uint8_t *)(0x5001072C))
#define LINS_REG_BTDIV07        (*(__IO            uint8_t *)(0x50010730))
#define LINS_REG_BITTIME        (*(__IO     LINS_BITTIME_t *)(0x50010734))
#define LINS_REG_ID             (*(__IO            uint8_t *)(0x50010738))
#define LINS_REG_BUSTIME        (*(__IO     LINS_BUSTIME_t *)(0x5001073C))

#define LINS_SFRS ((__IO LINS_SFRS_t *)0x50010700)

#endif /* end of __LINS_SFR_H__ section */

