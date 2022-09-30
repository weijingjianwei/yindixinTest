/**
 * @copyright 2017 indie Semiconductor
 * 
 * This file is proprietary to indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of indie Semiconductor.
 * 
 * @file evthold_sfr.h
 */

#ifndef EVTHOLD_SFR_H__
#define EVTHOLD_SFR_H__

/**
 * @brief A structure to represent Special Function Registers for EVTHOLD.
 */
typedef struct {

   uint8_t HOLD :  1; /*<! Hold */
   uint8_t      :  7; /*<! Hold */
   uint8_t      :  8; /*    (reserved) */
   uint16_t     : 16; /*    (reserved) */

} EVTHOLD_SFRS_t;

/**
 * @brief The starting address of EVTHOLD SFRS.
 */
#define EVTHOLD_SFRS ((__IO EVTHOLD_SFRS_t *)0x50000050)

#endif /* end of __EVTHOLD_SFR_H__ section */

