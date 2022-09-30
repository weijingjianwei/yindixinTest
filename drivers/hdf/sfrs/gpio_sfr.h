/**
 * @copyright 2018 indie Semiconductor
 * 
 * This file is proprietary to indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of indie Semiconductor.
 * 
 * @file gpio_sfr.h
 */

#ifndef GPIO_SFR_H__
#define GPIO_SFR_H__

#define GPIO_GPAP03_REG                   (*(__IO uint32_t *)(0x50019000))
#define GPIO_GPAP74_REG                   (*(__IO uint32_t *)(0x50019004))

/**
 * @brief A structure to represent Special Function Registers for GPIO.
 */
typedef struct {

   uint8_t  GPADATA [256*6];
   uint8_t  RESERVED0[4096 - 256*6];
   
   struct {
         uint8_t GPADIR0    :  1; /*!< GPIO1 PIN output enable */
         uint8_t GPAIE0     :  1; /*!< GPIO1 PIN interrupt mask */
         uint8_t GPARE0     :  1; /*!< GPIO1 PIN rising edge enable */
         uint8_t GPAFE0     :  1; /*!< GPIO1 PIN falling edge enable */
         uint8_t GPACLR0    :  1; /*!< GPIO1 PIN interrupt clear */
         uint8_t GPAACTDET0 :  1; /*!< GPIO1 PIN activity interrupt */
         uint8_t GPAHWSYNC0 :  1; /*!< GPIO1 PIN hardware synchronization enable */
         uint8_t            :  1; /*    (reserved) */
         uint8_t GPADIR1    :  1; /*!< GPIO2 PIN output enable */
         uint8_t GPAIE1     :  1; /*!< GPIO2 PIN interrupt mask */
         uint8_t GPARE1     :  1; /*!< GPIO2 PIN rising edge enable */
         uint8_t GPAFE1     :  1; /*!< GPIO2 PIN falling edge enable */
         uint8_t GPACLR1    :  1; /*!< GPIO2 PIN interrupt clear */
         uint8_t GPAACTDET1 :  1; /*!< GPIO2 PIN activity interrupt */
         uint8_t GPAHWSYNC1 :  1; /*!< GPIO2 PIN hardware synchronization enable */
         uint8_t            :  1; /*    (reserved) */
         uint8_t GPADIR2    :  1; /*!< NOT USED. SEE IOCTRL RxLIN_ena & TxLIN_ena */
         uint8_t GPAIE2     :  1; /*!< LIN_IN PIN interrupt mask */
         uint8_t GPARE2     :  1; /*!< LIN_IN PIN rising edge enable */
         uint8_t GPAFE2     :  1; /*!< LIN_IN PIN falling edge enable */
         uint8_t GPACLR2    :  1; /*!< LIN_IN PIN interrupt clear */
         uint8_t GPAACTDET2 :  1; /*!< LIN_IN PIN activity interrupt */
         uint8_t GPAHWSYNC2 :  1; /*!< LIN_IN PIN hardware synchronization enable */
         uint8_t            :  1; /*    (reserved) */
         uint8_t GPADIR3    :  1; /*!< NOT USED. SEE IOCTRL RxLIN_ena & TxLIN_ena */
         uint8_t GPAIE3     :  1; /*!< LIN_OUT PIN interrupt mask */
         uint8_t GPARE3     :  1; /*!< LIN_OUT PIN rising edge enable */
         uint8_t GPAFE3     :  1; /*!< LIN_OUT PIN falling edge enable */
         uint8_t GPACLR3    :  1; /*!< LIN_OUT PIN interrupt clear */
         uint8_t GPAACTDET3 :  1; /*!< LIN_OUT PIN activity interrupt */
         uint8_t GPAHWSYNC3 :  1; /*!< LIN_OUT PIN hardware synchronization enable */
         uint8_t            :  1; /*    (reserved) */
   } GPAP03;

   struct {
         uint8_t GPADIR4    :  1; /*!< GPIO3 PIN output enable */
         uint8_t GPAIE4     :  1; /*!< GPIO3 PIN interrupt mask */
         uint8_t GPARE4     :  1; /*!< GPIO3 PIN rising edge enable */
         uint8_t GPAFE4     :  1; /*!< GPIO3 PIN falling edge enable */
         uint8_t GPACLR4    :  1; /*!< GPIO3 PIN interrupt clear */
         uint8_t GPAACTDET4 :  1; /*!< GPIO3 PIN activity interrupt */
         uint8_t GPAHWSYNC4 :  1; /*!< GPIO3 PIN hardware synchronization enable */
         uint8_t            :  1; /*    (reserved) */
         uint8_t GPADIR5    :  1; /*!< GPIO4 PIN output enable */
         uint8_t GPAIE5     :  1; /*!< GPIO4 PIN interrupt mask */
         uint8_t GPARE5     :  1; /*!< GPIO4 PIN rising edge enable */
         uint8_t GPAFE5     :  1; /*!< GPIO4 PIN falling edge enable */
         uint8_t GPACLR5    :  1; /*!< GPIO4 PIN interrupt clear */
         uint8_t GPAACTDET5 :  1; /*!< GPIO4 PIN activity interrupt */
         uint8_t GPAHWSYNC5 :  1; /*!< GPIO4 PIN hardware synchronization enable */
         uint8_t            :  1; /*    (reserved) */
         uint16_t           : 16; /*    (reserved) */
   } GPAP74;

} GPIO_SFRS_t;

/**
 * @brief The starting address of GPIO SFRS.
 */
#define GPIO_SFRS ((__IO GPIO_SFRS_t *)0x50018000)

#endif /* end of __GPIO_SFR_H__ section */

