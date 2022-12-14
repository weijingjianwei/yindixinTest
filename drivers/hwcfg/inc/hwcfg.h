/**
 * @copyright 2017 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file hwcfg.h
 */

#ifndef HWCFG_H__
#define HWCFG_H__

#include "realplum.h"

#define E_HWCFG_SYSTEM_CLOCK_MAIN_RC_HF        (16000000U)
#define E_HWCFG_SYSTEM_CLOCK_MAIN_RC_LF        (10000U)

/* -------------------  Start of section using anonymous unions  ------------------ */
#if defined(__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined(__ICCARM__)
  #pragma language=extended
#elif defined(__GNUC__)
  /* anonymous unions are enabled by default */
#elif defined(__TMS470__)
/* anonymous unions are enabled by default */
#elif defined(__TASKING__)
  #pragma warning 586
#else
  #warning Not supported compiler type
#endif

typedef struct{
  uint16_t LED0_PNL_VOLT;         /*@volt in mV*/
  uint16_t LED0_PNL_CODE;          /*@ADC count 1500mV*/
  uint16_t LED0_PNH_VOLT;         /*@volt in mV*/
  uint16_t LED0_PNH_CODE;          /*@ADC count 4000mV*/
}LED_PN_t;

typedef struct{
  uint16_t trim; 
  uint16_t realValue;/*uA*/
}LED_OFF_Current_t;
/**
 * @brief A structure to represent the data in hardware config block.
 */
typedef struct {
    struct {
        uint8_t NAME[4];
        uint8_t VERSION;
        uint8_t LOT_NO[3];
        uint8_t LOAD_BOARD;
        uint8_t TIME_YEAR;
        uint8_t RESERVED[2];
        uint8_t TIME_250MS[4];
        uint8_t ATE_TEST_REV[3];
        uint8_t NUMBER;
    } CHIP;/*20*/
    
    uint8_t  CUSTOMER_INFO[32];
    uint8_t  CALIB_VERSION;
    uint8_t  BANDGAP_CODE;
    uint16_t BANDGAP_VOLT;     /* mV */
    uint16_t ADC_0V5;          /* 0.5V */
    uint16_t ADC_1V0;          /* 1.0V */
    
    uint16_t T_RESVD[2];
    uint16_t VBAT_CODE_13V5;    /* 13.5V adc count */
    
    uint8_t CLK_RC_LF;
    uint8_t CLK_RC_HF;
    uint16_t CLK_RC_HF_RESVD[2];
    
    LED_OFF_Current_t LED_OFF_TRIM_2mA_25C[3];    /*  2mA current value @batt = 13.5V  */
    LED_OFF_Current_t LED_RESV[3];
    
    uint16_t V2I_30mA_25C;
    uint16_t LED_TRIM_30mA_25C[3];
    
    uint16_t LED_TRIM_RESVD[12];
    /* 13.5V */
    uint16_t LED_VH_PNL_VOLT;         /*@volt in mV*/
    uint16_t LED_VH_PNL_CODE;          /*@ADC count 1500mV*/
    uint16_t LED_VH_PNH_VOLT;         /*@volt in mV*/
    uint16_t LED_VH_PNH_CODE;          /*@ADC count 4000mV*/
    /* LED_PN_t VH 13.5V */
    uint16_t RESVD3[8]; 
    uint16_t TSensor_mV_25C;
    uint16_t TSensor_mV_125C;  /* reserved */
    uint16_t RESVD4[2];   
    
    uint16_t LED_VHH_RESVD[4];        
    uint16_t RESVD5[12];
    /* 8V */
    uint16_t LED_VL_PNL_VOLT;
    uint16_t LED_VL_PNL_CODE;          /*@ADC count 1500mV*/
    uint16_t LED_VL_PNH_VOLT;         
    uint16_t LED_VL_PNH_CODE;          /*@ADC count 4000mV*/
    /* LED_PN_t VL 8V */
    uint16_t RESVD6[7];
    uint16_t VBAT_CODE_8V;  /* 8V adc count*/
    uint16_t RESVD7[3];
    uint16_t VBG_AT_8V; 
    uint32_t CRC32;

} HWCFG_SFRS_t;

/* --------------------  End of section using anonymous unions  ------------------- */
#if defined(__CC_ARM)
  #pragma pop
#elif defined(__ICCARM__)
  /* leave anonymous unions enabled */
#elif defined(__GNUC__)
  /* anonymous unions are enabled by default */
#elif defined(__TMS470__)
  /* anonymous unions are enabled by default */
#elif defined(__TASKING__)
  #pragma warning restore
#else
  #warning Not supported compiler type
#endif

/**
 * @brief The starting address of hardware config block.
 */
#define HWCFG_SFRS ((__IO HWCFG_SFRS_t *) (0x00010200))

#define E_HWCFG_ADC_RANGE_FULL       255
#define E_HWCFG_ADC_RANGE_ZERO       0
#define E_HWCFG_ADC_VBG              1220U       /* 1.22V */

/**
 * @brief Get High Frequency RC clock trim value.
 *
 * @return The trim value stored in flash memory.
 */
uint8_t HWCFG_GetRCHFClockCalibValue(void);

/**
 * @brief Get Low Frequency RC clock trim value.
 *
 * @return The trim value stored in flash memory.
 */
uint8_t HWCFG_GetRCLFClockCalibValue(void);

/**
 * @brief Get band gap voltage.
 *
 * @return The trim value stored in flash memory.
 */
uint16_t HWCFG_GetVBG(void);

/**
 * @brief Get the version of calibration data.
 *
 * @return The version for this calibration data block.
 */
uint32_t HWCFG_GetCalibrationVersion(void);

uint16_t HWCFG_GetV2ITrimValue(void);
uint16_t HWCFG_GetLEDTrimValue(uint8_t index);
uint16_t HWCFG_GetOffLEDTrimValue(uint8_t index);
#endif /* __HWCFG_H__ */

