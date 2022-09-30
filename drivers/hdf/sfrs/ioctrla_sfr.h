/**
 * @copyright 2018 indie Semiconductor
 * 
 * This file is proprietary to indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of indie Semiconductor.
 * 
 * @file ioctrla_sfr.h
 */

#ifndef IOCTRLA_SFR_H__
#define IOCTRLA_SFR_H__

/**
 * @brief A structure to represent Special Function Registers for IOCTRLA.
 */

#define IOCTRLA_GPIO1_REG                      (*(__IO uint32_t *)(0x50011000))
#define IOCTRLA_GPIO2_REG                      (*(__IO uint32_t *)(0x50011004))
#define IOCTRLA_GPIO3_REG                      (*(__IO uint32_t *)(0x50011008))
#define IOCTRLA_GPIO4_REG                      (*(__IO uint32_t *)(0x5001100C))
#define IOCTRLA_LIN_REG                        (*(__IO uint32_t *)(0x50011010))
#define IOCTRLA_LED_REG                        (*(__IO uint32_t *)(0x50011014))
#define IOCTRLA_ANALOGTESTMUXOVERRIDE_REG      (*(__IO uint32_t *)(0x50011018))

typedef struct {
        struct {
           uint8_t HWMODE  :  2; /*!< hardware mode */
           uint8_t LINM_SEL:  1; /*!< LINM Connection Select. */
           uint8_t PUENA   :  1; /*!< pullup enable (active-low) */
           uint8_t PDENA   :  1; /*!< pulldown enable */
           uint8_t RDENA   :  1; /*!< read enable */
           uint8_t PWM_SEL :  2; /*!< PWM output selection for PWM hardware mode */
           uint8_t         :  8; /*    (reserved) */
           uint8_t         :  8; /*    (reserved) */
           uint8_t MUXSEL  :  7;
           uint8_t         :  1; /*    (reserved) */
        }GPIO1;
    
        struct {
           uint8_t HWMODE  :  2; /*!< hardware mode */
           uint8_t         :  1; /*    (reserved) */
           uint8_t PUENA   :  1; /*!< pullup enable (active-low) */
           uint8_t PDENA   :  1; /*!< pulldown enable */
           uint8_t RDENA   :  1; /*!< read enable */
           uint8_t PWM_SEL :  2; /*!< PWM output selection for PWM hardware mode */
           uint8_t         :  8; /*    (reserved) */
           uint8_t         :  8; /*    (reserved) */
           uint8_t         :  7;
           uint8_t         :  1; /*    (reserved) */
        }GPIO2;
    
        struct {
           uint8_t HWMODE  :  2; /*!< hardware mode */
           uint8_t LINS_SEL:  1; /*!< LINS Connection Select. */
           uint8_t PUENA   :  1; /*!< pullup enable (active-low) */
           uint8_t PDENA   :  1; /*!< pulldown enable */
           uint8_t RDENA   :  1; /*!< read enable */
           uint8_t PWM_SEL :  2; /*!< PWM output selection for PWM hardware mode */
           uint8_t         :  8; /*    (reserved) */
           uint8_t         :  8; /*    (reserved) */
           uint8_t         :  7;
           uint8_t         :  1; /*    (reserved) */
        }GPIO3;
    
        struct {
           uint8_t HWMODE  :  2; /*!< hardware mode */
           uint8_t         :  1; /*!< reserved */
           uint8_t PUENA   :  1; /*!< pullup enable (active-low) */
           uint8_t PDENA   :  1; /*!< pulldown enable */
           uint8_t RDENA   :  1; /*!< read enable */
           uint8_t PWM_SEL :  2; /*!< PWM output selection for PWM hardware mode */
           uint8_t         :  8; /*    (reserved) */
           uint8_t         :  8; /*    (reserved) */
           uint8_t         :  7;
           uint8_t         :  1; /*    (reserved) */
        }GPIO4;
    
      struct {
         uint8_t LINS_HWMODE    :  1; /*!< LIN Slave hardware mode */
         uint8_t                :  2; /*    (reserved) */
         uint8_t LINS_PU30K_ENA :  1; /*!< LIN 30K pullup enable */
         uint8_t                :  1; /*    (reserved) */
         uint8_t LINS_TXENA     :  1; /*!< LIN transmit enable */
         uint8_t LINS_RXENA     :  1; /*!< LIN receive  enable */
         uint8_t                :  1; /*    (reserved) */
         uint8_t LINM_HWMODE    :  1; /*!< LIN Master hardware mode */
         uint8_t                :  3; /*    (reserved) */
         uint8_t LINM_PU1K_ENA  :  1; /*!< LIN 1K  pullup enable */
         uint8_t LINM_TXENA     :  1; /*!< LIN transmit enable */
         uint8_t LINM_RXENA     :  1; /*!< LIN receive  enable */
         uint8_t                :  1; /*    (reserved) */
         uint8_t SWON           :  1; /*!< LIN switch between LIN_IN and LIN_OUT, 1: on between  LIN_IN and LIN_OUT, 0: off between  LIN_IN and LIN_OUT*/
         uint8_t DS_SWON        :  1; /*!< reserved */
         uint8_t                :  6; /*    (reserved) */
         uint8_t SWON_LOCK      :  1; /*!< SWON Lock Bit */
         uint8_t                :  6; /*    (reserved) */
         uint8_t PMODE          :  1; /*!< LIN Power Mode */
      }LIN;

      struct {
         uint8_t HWMODE    :  3; /*!< LED hardware mode */
         uint8_t           :  1; /*    (reserved) */
         uint8_t DATA      :  3; /*!< LED Data Out */
         uint8_t           :  1; /*    (reserved) */
         uint8_t SENSE_ENA :  1; /*!< LED Forward Voltage Sense Enable */
         uint8_t           :  7; /*    (reserved) */
         uint8_t           :  8; /*    (reserved) */
         uint8_t           :  8; /*    (reserved) */
      }LED;

      struct {
         uint8_t ADCCONSEL  :  1; /*!< Hardware/Firmware Select */
         uint8_t GPIOCONSEL :  1; /*!< Hardware/Firmware Select */
         uint8_t ADCSELSEL  :  1; /*!< Hardware/Firmware Select */
         uint8_t            :  5; /*    (reserved) */
         uint8_t ADCCONREG  :  6; /*!< Firmware Debug Value */
         uint8_t            :  2; /*    (reserved) */
         uint8_t GPIOCONREG :  7; /*!< Firmware Debug Value */
         uint8_t            :  1; /*    (reserved) */
         uint8_t ADCSELREG  :  4; /*!< Firmware Debug Value */
         uint8_t            :  4; /*    (reserved) */
      }ANALOGTESTMUXOVERRIDE;
} IOCTRLA_SFRS_t;


/**
 * @brief The starting address of IOCTRLA SFRS.
 */
#define IOCTRLA_SFRS ((__IO IOCTRLA_SFRS_t *)0x50011000)

#endif /* end of __IOCTRLA_SFR_H__ section */

