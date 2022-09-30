#ifndef APP_CONFIG_H__
#define APP_CONFIG_H__

#include <clock_device.h>
#include <pwm_device.h>
#include <string.h>

#define PRAGMA(x) _Pragma(#x)

#define BEGIN_PACK PRAGMA(pack(push, 1))
#define END_PACK   PRAGMA(pack(pop))

#define TRUE                    (1U)
#define FALSE                   (0U)

#define SDK_APPL_VERSION        (0x0301U)

/* ************************hardware config ********************************/
#define MAIN_CPU_CLOCK          16U       /*MHz*/

#if MAIN_CPU_CLOCK == 16U
    #define SYS_MAIN_CLOCK_DIV    CLOCK_DIV_1
#else
    #error  please use 16MHz for system clock!
#endif


#define PHY_CHANNEL_RED    PWM_CHANNEL_2
#define PHY_CHANNEL_GREEN  PWM_CHANNEL_0
#define PHY_CHANNEL_BLUE   PWM_CHANNEL_1

#define PWM_VALUE_MAX_POS               (16U)
#define PWM_VALUE_MAX                   ((1UL << PWM_VALUE_MAX_POS) -1UL)

#define LED_NUM    (1U)

#if LED_NUM != 1U
#error LED_NUM must be 1 here!!
#endif


#define ROOM_TEMPERATURE        (24U)  /*1C*/
/* ************************System config ********************************/
#define LIN_MASTER_EN                           (0U)
#define WATCH_DOG_EN                            (0U)
#define SAFETY_MONITOR_EN                       (1U)

/* ************************debug config ********************************/

#define CODE_DEBUG_EN    0

#if CODE_DEBUG_EN == 1
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>

  #define DEBUG_OUT printf

#else

  #define DEBUG_OUT(...)

#endif



#endif