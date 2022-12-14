/**
 * @copyright 2019 indie Semiconductor
 * 
 * This file is proprietary to indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of indie Semiconductor.
 * 
 * @file sysctrla_sfr.h
 */

#ifndef SYSCTRLA_SFR_H__
#define SYSCTRLA_SFR_H__


#define SYSCTRLA_RETAIN0_WORD                   (*(__IO uint32_t *)(0x50012000))
#define SYSCTRLA_RETAIN1_WORD                   (*(__IO uint32_t *)(0x50012004))
#define SYSCTRLA_DEBUG_ACCESS_KEY_WORD          (*(__IO uint32_t *)(0x50012008))
#define SYSCTRLA_DEBUG_ACCESS_ENABLED_WORD      (*(__IO uint32_t *)(0x5001200C))
#define SYSCTRLA_TRIM_ACCESS_KEY_WORD           (*(__IO uint32_t *)(0x50012010))
#define SYSCTRLA_TRIM_ACCESS_ENABLED_WORD       (*(__IO uint32_t *)(0x50012014))
#define SYSCTRLA_PMU_TRIM_WORD                  (*(__IO uint32_t *)(0x50012018))
#define SYSCTRLA_LF_OSC_TRIM_WORD               (*(__IO uint32_t *)(0x5001201C))
#define SYSCTRLA_HF_OSC_TRIM_WORD               (*(__IO uint32_t *)(0x50012020))
#define SYSCTRLA_LED0_WORD                      (*(__IO uint32_t *)(0x50012024))
#define SYSCTRLA_LED1_WORD                      (*(__IO uint32_t *)(0x50012028))
#define SYSCTRLA_LIN_WORD                       (*(__IO uint32_t *)(0x5001202C))
#define SYSCTRLA_DFTCODE_WORD                   (*(__IO uint32_t *)(0x50012030))
#define SYSCTRLA_DFT_ACCESS_ENABLED_WORD        (*(__IO uint32_t *)(0x50012034))
#define SYSCTRLA_DFTTESTMODESTART_WORD          (*(__IO uint32_t *)(0x50012038))
#define SYSCTRLA_NAME_WORD                      (*(__IO uint32_t *)(0x5001203C))
#define SYSCTRLA_REV_WORD                       (*(__IO uint32_t *)(0x50012040))
#define SYSCTRLA_BORTESTMODE_WORD               (*(__IO uint32_t *)(0x50012044))

/**
 * @brief A structure to represent Special Function Registers for SYSCTRLA.
 */
typedef struct {

   uint8_t RETAIN0:  4;
   uint8_t        :  4;
   uint8_t        :  8;
   uint16_t       : 16; /*    (reserved) */

   uint8_t RETAIN1:  4;
   uint8_t        :  4;
   uint8_t        :  8;
   uint16_t       : 16; /*    (reserved) */

   struct {
         uint8_t DEBUG_ACCESS_KEY : 4;
         uint8_t                  : 4; /*    (reserved) */
         uint8_t                  : 8; /*    (reserved) */
         uint8_t                  : 8; /*    (reserved) */
         uint8_t                  : 7; /*    (reserved) */
         uint8_t DEBUG_LOCK       : 1;
   } DEBUG_ACCESS_KEY;

   uint8_t DEBUG_ACCESS_ENABLED:  1;
   uint8_t                     :  7; /*    (reserved) */
   uint8_t                     :  8; /*    (reserved) */
   uint8_t                     :  8; /*    (reserved) */
   uint8_t                     :  8; /*    (reserved) */

   struct {
         uint8_t TRIM_ACCESS_KEY :  4;
         uint8_t                 :  4; /*    (reserved) */
         uint8_t                 :  8; /*    (reserved) */
         uint8_t                 :  8; /*    (reserved) */
         uint8_t                 :  7; /*    (reserved) */
         uint8_t TRIM_LOCK       :  1;
   } TRIM_ACCESS_KEY;

   uint32_t TRIM_ACCESS_ENABLED:  1;
   uint8_t                     :  7; /*    (reserved) */
   uint8_t                     :  8; /*    (reserved) */
   uint8_t                     :  8; /*    (reserved) */
   uint8_t                     :  8; /*    (reserved) */

   struct {
         uint8_t               :  4; /*    (reserved) */
         uint8_t TRIM          :  3; /*!< Band Gap Trim */
         uint8_t               :  1; /*    (reserved) */
         uint8_t RESISTOR_TRIM :  6; /*!< V2I Resistor Trim */
         uint8_t               :  2; /*    (reserved) */
         uint16_t              : 16; /*    (reserved) */
   } PMU_TRIM;

   uint8_t TRIM_LF_RC:  5;
   uint8_t           :  3; /*    (reserved) */
   uint8_t           :  8; /*    (reserved) */
   uint16_t          : 16; /*    (reserved) */

   uint8_t TRIM_HF_RC:  8;
   uint8_t           :  8; /*    (reserved) */
   uint16_t          : 16; /*    (reserved) */

   struct {
         uint8_t BIAS_SEL       :  3; /*!< High Voltage LED bias select */
         uint8_t                :  1; /*    (reserved) */
         uint8_t BIAS_REG       :  3; /*!< High Voltage LED bias select register */
         uint8_t                :  1; /*    (reserved) */
         uint8_t                :  8; /*    (reserved) */
         uint16_t TRIM0         :  9; /*!< High Voltage LED0 trim (100uA step) */
         uint16_t TRIM0_OFF     :  5; /*!< High Voltage LED0 Off trim (100uA step) */
         uint16_t               :  1; /*    (reserved) */
         uint16_t TRIM0_OFFMODE :  1; /*!< High Voltage LED0 Off Mode */
   } LED0;

   struct {
         uint16_t TRIM1         :  9; /*!< High Voltage LED1 trim (100uA step) */
         uint16_t TRIM1_OFF     :  5; /*!< High Voltage LED1 Off trim (100uA step) */
         uint16_t               :  1; /*    (reserved) */
         uint16_t TRIM1_OFFMODE :  1; /*!< High Voltage LED1 Off Mode */
         uint16_t TRIM2         :  9; /*!< High Voltage LED2 trim (100uA step) */
         uint16_t TRIM2_OFF     :  5; /*!< High Voltage LED2 Off trim (100uA step) */
         uint16_t               :  1; /*    (reserved) */
         uint16_t TRIM2_OFFMODE :  1; /*!< High Voltage LED2 Off Mode */
   } LED1;

   struct {
         uint8_t TXLINS_DR_SLOPE   :  2; /*!< LIN Slave IO Drive Slope select */
         uint8_t TXLINM_DR_SLOPE   :  2; /*!< LIN Master IO Drive Slope select */
         uint8_t LINSTX_BIAS_BOOST :  2; /*!< LIN Slave IO TX Bias select */
         uint8_t LINMTX_BIAS_BOOST :  2; /*!< LIN Master IO TX Bias select */
         uint8_t                   :  8; /*    (reserved) */
         uint16_t                  : 16; /*    (reserved) */
   } LIN;

   struct {
         uint8_t DFTCODE   :  8;
         uint8_t           :  8; /*    (reserved) */
         uint8_t           :  8; /*    (reserved) */
         uint8_t           :  7; /*    (reserved) */
         uint8_t DFT_LOCK  :  1;
   } DFTCODE;

   uint8_t DFT_ACCESS_ENABLED:  1;
   uint8_t                   :  7; /*    (reserved) */
   uint8_t                   :  8; /*    (reserved) */
   uint16_t                  :  16; /*    (reserved) */

   uint8_t DFTTESTMODESTART :  1;
   uint8_t                  :  7; /*    (reserved) */
   uint8_t                  :  8; /*    (reserved) */
   uint16_t                 :  16; /*    (reserved) */

   uint32_t NAME;

   uint16_t REV: 16;
   uint16_t    : 16; /*    (reserved) */

   uint8_t ENABORTESTMODE    :  1;
   uint8_t                   :  7; /*    (reserved) */
   uint8_t                   :  8; /*    (reserved) */
   uint16_t                  :  16; /*    (reserved) */

} SYSCTRLA_SFRS_t;

/**
 * @brief The starting address of SYSCTRLA SFRS.
 */
#define SYSCTRLA_SFRS ((__IO SYSCTRLA_SFRS_t *)0x50012000)

#endif /* end of __SYSCTRLA_SFR_H__ section */

