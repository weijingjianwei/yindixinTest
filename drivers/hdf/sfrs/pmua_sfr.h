/**
 * @copyright 2017 indie Semiconductor
 * 
 * This file is proprietary to indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of indie Semiconductor.
 * 
 * @file pmua_sfr.h
 */

#ifndef PMUA_SFR_H__
#define PMUA_SFR_H__

#include <stdint.h>
#include <stdbool.h>

#define PMUA_CTRL_REG          (*(__IO uint32_t *)(0x50000040))
#define PMUA_DEBUG_REG         (*(__IO uint32_t *)(0x50000044))
#define PMUA_DWELL_REG         (*(__IO uint32_t *)(0x50000048))
#define PMUA_VBAT_REG          (*(__IO uint32_t *)(0x5000004C))

/**
 * @brief A structure to represent Special Function Registers for PMUA.
 */
typedef struct {
    struct {
        uint8_t HIBERNATE    :  1;
        uint8_t FASTBOOT     :  1; /*!< Fast boot */
        uint8_t FASTSHUTDOWN :  1; /*!< Fast shutdown */
        uint8_t              :  5;
        uint8_t              :  8;
        uint16_t             : 16; /*    (reserved) */
    } CTRL;

    struct {
        uint8_t IGNORE_CIFS  :  1; /*<! Debug */
        uint8_t              :  7;
        uint8_t              :  8;
        uint16_t             : 16; /*    (reserved) */
    }DEBUG;
    struct {
       uint8_t ENABLE_BIAS     :  4; /*!< Enable bias dwell time */
       uint8_t ENABLE_1V8      :  4; /*!< Enable 1.8V dwell time */
       uint8_t ATTACH_1V8      :  4; /*!< Attach 1.8V dwell time */
       uint8_t ATTACH_2V6      :  4; /*!< Attach 2.6V dwell time */
       uint8_t POWER_DOWN_MCU  :  4; /*!< Power down MCU dwell time */
       uint8_t ENABLE_MAIN_REG :  4; /*!< Enable main regulator dwell time */
       uint8_t                 :  8; /*    (reserved) */
    } DWELL;

    struct {
       uint8_t UV_FLAG         :  1; /*!< Under voltage interrupt flag */
       uint8_t OV_FLAG         :  1; /*!< Over voltage interrupt flag */
       uint8_t                 :  6; /*    (reserved) */
       uint8_t UV_IRQ_ENA      :  1; /*!< Battery Voltage Monitor Under Voltage Interrupt Enable note that keep this default disabled*/
       uint8_t OV_IRQ_ENA      :  1; /*!< Battery Voltage Monitor Over Voltage Interrupt Enable */
       uint8_t UV_IRQ_CLR      :  1; /*!< Battery Voltage Monitor Under Voltage Interrupt Clear */
       uint8_t OV_IRQ_CLR      :  1; /*!< Battery Voltage Monitor Over Voltage Interrupt Clear */
       uint8_t UV_IRQ_POL      :  1; /*!< Battery Voltage Monitor Under Voltage Interrupt Event Polarity */
       uint8_t OV_MONITOR_POL  :  1; /*!< Battery Voltage Monitor Over Voltage Interrupt Event Polarity */
       uint8_t                 :  2; /*    (reserved) */
       uint8_t UVLEVEL_SEL     :  6; /*!< Battery Voltage Monitor Under Voltage Select */
       uint8_t LOW_MONITOR_ENA :  1; /*!< Battery Under Voltage Monitor Enable , note that keep this default disabled*/
       uint8_t LOW             :  1; /*!< Battery Voltage Low Status */
       uint8_t OVLEVEL_SEL     :  6; /*!< Battery Voltage Monitor Over Voltage Select */
       uint8_t OV_MONITOR_ENA  :  1; /*!< Battery Over Voltage Monitor Enable */
       uint8_t HIGH            :  1; /*!< Battery Voltage High Status */
    } VBAT;
} PMUA_SFRS_t;

/**
 * @brief The starting address of PMUA SFRS.
 */
#define PMUA_SFRS ((__IO PMUA_SFRS_t *)0x50000040)

#endif /* end of __PMUA_SFR_H__ section */

