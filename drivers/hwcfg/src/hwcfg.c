/**
 * @copyright 2017 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file hwcfg.c
 */

#include <stddef.h>
#include <stdint.h>
#include "realplum.h"
#include "hwcfg.h"


uint8_t HWCFG_GetRCHFClockCalibValue(void)
{
    uint8_t trim = HWCFG_SFRS->CLK_RC_HF;
    if (trim == 0xFFU){
        trim = 0x3EU;
    }
    return trim;
}

  
uint8_t HWCFG_GetRCLFClockCalibValue(void)
{
    uint8_t trim = HWCFG_SFRS->CLK_RC_LF;
    if (trim == 0xFFU){
        trim = 0x12U;
    }
    return trim;
}


uint16_t HWCFG_GetVBG(void)
{
    uint16_t volt =  HWCFG_SFRS->BANDGAP_VOLT;
    if (volt == 0xFFU){
        volt = E_HWCFG_ADC_VBG;
    }
    return volt;
}


/**
 * @brief Get the version of calibration data.
 *
 * @return The version for this calibration data block.
 */
uint32_t HWCFG_GetCalibrationVersion(void)
{
    uint32_t version;
    version = HWCFG_SFRS->CHIP.ATE_TEST_REV[2];
    version = (version << 8U) + HWCFG_SFRS->CHIP.ATE_TEST_REV[1];
    version = (version << 8U) + HWCFG_SFRS->CHIP.ATE_TEST_REV[0];
    return version;
  
}

uint16_t HWCFG_GetV2ITrimValue(void)
{
    uint16_t trimValue = HWCFG_SFRS->V2I_30mA_25C;
    if (trimValue == 0xFFU){
        trimValue =  0x20U;
    }
    return trimValue;
}


uint16_t HWCFG_GetLEDTrimValue(uint8_t index)
{
    uint16_t trimValue = HWCFG_SFRS->LED_TRIM_30mA_25C[index];
    if ( trimValue > 0x1FFU){
        trimValue =  300U; /* 100uA*300 = 30mA */
    }
    return trimValue;
}

uint16_t HWCFG_GetOffLEDTrimValue(uint8_t index)
{
    uint16_t trimValue = HWCFG_SFRS->LED_OFF_TRIM_2mA_25C[index].trim;
    if ( trimValue > 0x1FU){
        trimValue = 20U; /* 100uA*20 = 2mA */
    }
    return trimValue;
}





























