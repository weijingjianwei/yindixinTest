#include <systemInit.h>
#include <validation.h>

void leds_driver_Init(void);
void gpios_init(void);
void pmu_init(void);

void leds_driver_Init(void)
{
    PWM_Init(PHY_CHANNEL_RED,   PWM_PRESCALER_DIVIDE_1, PWM_VALUE_MAX, FALSE);
    PWM_Init(PHY_CHANNEL_GREEN, PWM_PRESCALER_DIVIDE_1, PWM_VALUE_MAX, FALSE);
    PWM_Init(PHY_CHANNEL_BLUE,  PWM_PRESCALER_DIVIDE_1, PWM_VALUE_MAX, FALSE);

    PWM_SetMatchValue(PHY_CHANNEL_RED,   PWM_VALUE_MAX, PWM_VALUE_MAX);
    PWM_SetMatchValue(PHY_CHANNEL_GREEN, PWM_VALUE_MAX, PWM_VALUE_MAX);
    PWM_SetMatchValue(PHY_CHANNEL_BLUE,  PWM_VALUE_MAX, PWM_VALUE_MAX);
    
    PWM_EnableAllChannels();
    (void)PWM_SetInitChannelCurrent(PHY_CHANNEL_RED);
    (void)PWM_SetInitChannelCurrent(PHY_CHANNEL_GREEN);
    (void)PWM_SetInitChannelCurrent(PHY_CHANNEL_BLUE);
    
}

void gpios_init(void)
{
    /* set gpios to input with power up resistor */
    GPIO_Init(GPIO_PORT_1, GPIO_MUX_GPIO, GPIO_DIR_OUTPUT,GPIO_PULL_MODE_NONE,GPIO_PWM_NONE);
    GPIO_Init(GPIO_PORT_2, GPIO_MUX_GPIO, GPIO_DIR_OUTPUT,GPIO_PULL_MODE_NONE,GPIO_PWM_NONE);
    GPIO_Init(GPIO_PORT_3, GPIO_MUX_GPIO, GPIO_DIR_OUTPUT,GPIO_PULL_MODE_NONE,GPIO_PWM_NONE);
    GPIO_Init(GPIO_PORT_4, GPIO_MUX_GPIO, GPIO_DIR_OUTPUT,GPIO_PULL_MODE_NONE,GPIO_PWM_NONE);
    
    GPIO_Set(GPIO_PORT_1, GPIO_LOW);
    GPIO_Set(GPIO_PORT_2, GPIO_LOW);
    GPIO_Set(GPIO_PORT_3, GPIO_LOW);
    GPIO_Set(GPIO_PORT_4, GPIO_LOW);
}

void pmu_init(void)
{
    /* Init set BOR voltage level for cpu low voltage safety*/
    PMU_BORInit(BOR_1V5_1400mV, BOR_3V3_3000mV);
    /* Disable wake up timer */
    PMU_WakeTimerInit(WAKEUP_TIMEER_DISABLE, WAKEUP_TIMEER_INTERVAL_32768ms);
}

void SYS_Init(void)
{
    /* Init system clock */
    Clock_SystemMainClockInit(CLOCK_RC_16MHz, SYS_MAIN_CLOCK_DIV);
    pmu_init();
    /* Init global timer engine for driving soft timer */
    SysTick_Init(SOFT_TIMER_INTERVAL *1000U * MAIN_CPU_CLOCK, SoftTimer_ExpireCallback);
#if WATCH_DOG_EN == 1U
      WDTA_Enable(WDTA_MODE_RESET, WDTA_INTERVAL_1000MS, NULL); /* 1s */
#endif
    /* Init gpios settings */
    gpios_init();
    /* Init LED current and PWM settings */
    leds_driver_Init();

    /* tasks init must be called before use. */
    TM_PostTask(TASK_ID_SOFT_TIMER);
    TM_PostTask(TASK_ID_SAFETY_MONITOR);
    TM_PostTask(TASK_ID_LINS);
    TM_PostTask(TASK_ID_PDS);
#if LIN_MASTER_EN == 1
    TM_PostTask(TASK_ID_LINM);
#endif
    TM_PostTask(TASK_ID_ADC_MEASURE);
    TM_PostTask(TASK_ID_COLOR_MIXING);
    TM_PostTask(TASK_ID_COLOR_COMPENSATION);
    TM_PostTask(TASK_ID_APPL);
}



