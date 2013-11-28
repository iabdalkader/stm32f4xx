#include <stdint.h>
#include <stm32f4xx.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_syscfg.h>
#define LED_GPIO_PORT   GPIOD 
#define LED_GPIO_PIN    GPIO_Pin_5
#define LED_RCC_PERIPH  RCC_AHB1Periph_GPIOD
#define LED_RCC_CLKCMD(periph, stat)  \
    RCC_AHB1PeriphClockCmd(periph, stat)

volatile uint32_t sys_ticks; 

void systick_sleep(uint32_t ms)
{
    uint32_t ticks = sys_ticks;
    while ((sys_ticks-ticks) < ms) {

    }
}
int main(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    /* Enable GPIOG clock */
    LED_RCC_CLKCMD(LED_RCC_PERIPH, ENABLE);

    /* Configure LED pin in output mode */
    GPIO_InitStructure.GPIO_Pin   = LED_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

    GPIO_Init(LED_GPIO_PORT, &GPIO_InitStructure);
    GPIO_SetBits(LED_GPIO_PORT, LED_GPIO_PIN);

    /* configure systick to interrupt every 1ms */
    if (SysTick_Config(SystemCoreClock / 1000)) {
        while(1);
    }

    /* Do nothing */
    while (1) {
        /* wait 500ms */
        systick_sleep(500);

        /* Toggle LED */
        GPIO_ToggleBits(LED_GPIO_PORT, LED_GPIO_PIN);
    }
}

