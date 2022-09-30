/**
 * @copyright 2017 indie Semiconductor
 * 
 * This file is proprietary to indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of indie Semiconductor.
 * 
 * @file wica_sfr.h
 */

#ifndef WICA_SFR_H__
#define WICA_SFR_H__

#define WICA_CTRL_REG                  (*(__IO uint32_t *)(0x50010100))
#define WICA_STATUS_REG                (*(__IO uint32_t *)(0x50010104))

/**
 * @brief A structure to represent Special Function Registers for WICA.
 */
typedef struct {

   struct {
         uint8_t LINSENA            :  1; /*!< LIN Wakeup Enable */
         uint8_t GPIOENA            :  1; /*!< GPIO Wakeup Enable */
         uint8_t WU_TIMERENA        :  1; /*!< Wakeup Timer Enable */
         uint8_t LINMENA            :  1; /*!<LIN Master Wakeup Enable. it enables the detect of a wakeup signal on the LIN_OUT bus */
         uint8_t WU_TIMERTAPSEL     :  4; /*!< WakeUp Timer Tap Select */
         uint8_t LINSIRQENA         :  1; /*!< LIN Wakeup Interrupt Enable */
         uint8_t GPIOIRQENA         :  1; /*!< GPIO Wakeup Interrupt Enable */
         uint8_t WU_TIMERIRQENA     :  1; /*!< Timer Wakeup Interrupt Enable */
         uint8_t LINMIRQENA         :  1; /*!< LIN Master Wakeup Interrupt Enable */
         uint8_t LINIRQCLR          :  1; /*!< clear the wulin_irq */
         uint8_t GPIOIRQCLR         :  1; /*!< clear the wugpio_irq */
         uint8_t WU_TIMERIRQCLR     :  1; /*!< clear the wutimer_irq */
         uint8_t LINMIRQCLR         :  1; /*!< clear the wulin_irq. writting a '1 to this register will clear the wulins_irq */
         uint16_t                   : 16;
   } CTRL;

   struct {
         uint8_t LINS      :  1; /*!< LIN Wakeup Status */
         uint8_t GPIO      :  1; /*!< GPIO Wakeup Status */
         uint8_t TIMER     :  1; /*!< Wakeup Timer Status */
         uint8_t LINM      :  1; /*!< LIN Master Wakeup Status */
         uint8_t           :  4;
         uint8_t           :  8;
         uint16_t TIMERCNT : 16; /*!< Wakeup Timer Counter Value */
   } STATUS;

} WICA_SFRS_t;

/**
 * @brief The starting address of WICA SFRS.
 */
#define WICA_SFRS ((__IO WICA_SFRS_t *)0x50010100)

#endif /* end of __WICA_SFR_H__ section */

