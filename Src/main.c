#include "main.h"
#include "printf.h"
#include "lcd.h"
#include "lvgl.h"
#include "display_lvgl.h"
#include "lv_demo_widgets.h"

static void clock_init(void);
static void debug_uart_init(uint32_t baud);
static void timer_init(void);

int main(void)
{
    clock_init();
#if defined (DEBUG_PRINTF_ENABLED)
    debug_uart_init(115200);
#endif

    timer_init();

    lv_init();

    display_init();

    lv_demo_widgets();
    while (1)
    {
        lv_task_handler();
    }
}

static void timer_init(void)
{
    LL_TIM_InitTypeDef TIM_InitStruct;

    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM6);

    TIM_InitStruct.Prescaler = 84 - 1; //
    TIM_InitStruct.Autoreload = 1000 - 1; //
    TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
    TIM_InitStruct.CounterMode = LL_TIM_COUNTERDIRECTION_UP;
    TIM_InitStruct.RepetitionCounter = 0;
    LL_TIM_Init(TIM6, &TIM_InitStruct);

    LL_TIM_EnableIT_UPDATE(TIM6);
    NVIC_SetPriority(TIM6_DAC_IRQn, 2);
    NVIC_EnableIRQ(TIM6_DAC_IRQn);

    LL_TIM_EnableCounter(TIM6);
}

void TIM6_DAC_IRQHandler(void)
{
    if (LL_TIM_IsActiveFlag_UPDATE(TIM6))
    {
        LL_TIM_ClearFlag_UPDATE(TIM6);

        lv_tick_inc(1);
    }
}

static void clock_init(void)
{
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_5);
    while (LL_FLASH_GetLatency() != LL_FLASH_LATENCY_5)
        ;

    LL_FLASH_EnablePrefetch();
    LL_FLASH_EnableInstCache();
    LL_FLASH_EnableDataCache();

    LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);

    LL_RCC_HSE_Enable();
    while (LL_RCC_HSE_IsReady() != 1)
        ;

    // 8M/4/2*168 = 168M
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 168, LL_RCC_PLLP_DIV_2);
    //LL_RCC_PLL_ConfigDomain_48M(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 168,LL_RCC_PLLQ_DIV_7);
    LL_RCC_PLL_Enable();
    while (LL_RCC_PLL_IsReady() != 1)
        ;

    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_4);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);

    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

    while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
        ;

    LL_Init1msTick(168000000);
    LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);

    LL_SetSystemCoreClock(168000000);
    SystemCoreClockUpdate();

    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOE);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOH);

    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
}

static void debug_uart_init(uint32_t baud)
{
    LL_USART_InitTypeDef USART_InitStruct;
    LL_GPIO_InitTypeDef GPIO_InitStruct;

    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);

    GPIO_InitStruct.Pin = LL_GPIO_PIN_9; // | LL_GPIO_PIN_10;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    USART_InitStruct.BaudRate = baud;
    USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
    USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
    USART_InitStruct.Parity = LL_USART_PARITY_NONE;
    USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX;
    USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
    USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
    LL_USART_Init(USART1, &USART_InitStruct);

    LL_USART_ConfigAsyncMode(USART1);
    LL_USART_Enable(USART1);
}

void _putchar(char character)
{
#ifdef DEBUG_PRINTF_ENABLED
    while (!LL_USART_IsActiveFlag_TXE(USART1))
        ;
    LL_USART_TransmitData8(USART1, character);
#endif
}

void HardFault_Handler(void)
{
    printf("!HARD FAULT!\r\n\r\n");
    //NVIC_SystemReset();
    while (1)
        ;
}
