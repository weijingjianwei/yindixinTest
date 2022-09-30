/**
 * @copyright 2018 indie Semiconductor
 * 
 * This file is proprietary to indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of indie Semiconductor.
 * 
 * @file adc_sfr.h
 */

#ifndef ADC_SFR_H__
#define ADC_SFR_H__

#define ADC_CONF_REG            (*(__IO uint32_t *)(0x50010D00))
#define ADC_CNTRL_REG           (*(__IO uint32_t *)(0x50010D04))
#define ADC_TSET_REG            (*(__IO uint32_t *)(0x50010D08))
#define ADC_DATA1_REG           (*(__IO uint32_t *)(0x50010D0C))
#define ADC_DATA2_REG           (*(__IO uint32_t *)(0x50010D10))
#define ADC_DATA0345_REG        (*(__IO uint32_t *)(0x50010D14))
#define ADC_STATUS_REG          (*(__IO uint32_t *)(0x50010D18))
/**
 * @brief A structure to represent Special Function Registers for ADC.
 */
typedef struct {
  
   struct {
     uint8_t MODE             :  1; /*!< ADC mode select */
     uint8_t AUTOEN           :  1; /*!< Bias Enable mode */
     uint8_t ATTEN            :  1; /*!< ADC input Attenuation setting */
     uint8_t SAMPCYC          :  3; /*!< Sample cycle */
     uint8_t SWSYNCIN         :  1; /*!< Soft Sync Input */
     uint8_t                  :  1;
     uint8_t                  :  8; /*    (reserved) */
     uint16_t                 :  1; /*!< LED BUFFER ENABLE */
     uint16_t                 : 15; /*    (reserved) */
   } CONF;

   struct {
     uint8_t CONVERT :  1; /*!< ADC START/STATUS Register */
     uint8_t CONT    :  1; /*!< Continuous Convesion Enable */
     uint8_t SYNCENA :  1; /*!< Sync Enable */
     uint8_t SYNCEDGE:  1; /*!< Sync Edge Select. Select edge sensitivity of the sync signal. 0x0: Triggered by the posedge of the sync signal.0x1: Triggered by the negedge of the sync signal.*/
     uint8_t STUPDLY :  2; /*!< Startup Delay */
     uint8_t IRQENA  :  1; /*!< IRQ Enable */
     uint8_t IRQCLR  :  1; /*!< IRQ Clear */
     uint8_t CHSEQ   :  6; /*!< Channel Sequence */
     uint8_t         :  2; /*    (reserved) */
     uint8_t CH2SEL  :  2; /*!< Channel2 Selection */
     uint8_t         :  1; /*!< reserved */
     uint8_t         :  1; /*    (reserved) */
     uint8_t CH4SEL  :  2; /*!< Channel4 Selection */
     uint8_t         :  2; /*    (reserved) */
     uint8_t         :  8; /*    (reserved) */
   } CNTRL;

   struct {
     uint8_t TCURR  :  4; /*!< TCURR setting */
     uint8_t TCHNL  :  4; /*!< TCHNL setting */
     uint8_t TGUARD :  4; /*!< TGUARD setting */
     uint8_t        :  4; /*    (reserved) */
     uint16_t       : 16; /*    (reserved) */
   } TSET;

   uint16_t DATA1: 12;
   uint16_t      : 4;
   uint16_t      : 16; /*    (reserved) */

   uint16_t DATA2: 12;
   uint16_t      : 4;
   uint16_t      : 16; /*    (reserved) */

   uint16_t DATA0345: 12;
   uint16_t         : 4;
   uint16_t         : 16; /*    (reserved) */

   struct {
     uint8_t CONVDONE :  1; /*!< The set Sequence of Conversions is Done */
     uint8_t FSM      :  6; /*!< current state of the ADC Sequencer */
     uint8_t          :  1; /*    (reserved) */
     uint8_t          :  8;
     uint16_t         :  16; /*    (reserved) */
   } STATUS;

} ADC_SFRS_t;

/**
 * @brief The starting address of ADC SFRS.
 */
#define ADC_SFRS ((__IO ADC_SFRS_t *)0x50010d00)

#endif /* end of __ADC_SFR_H__ section */

