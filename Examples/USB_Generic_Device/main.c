#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_syscfg.h>
#include <stm32f4xx_misc.h>
#include "usb_generic.h"

volatile uint32_t sys_ticks=0;

void usb_data_in(void *buffer, int *length, void *user_data)
{
    char buf[64];
    /* send back the current ticks */
    sprintf(buf, "%lu", sys_ticks);
    *length = 64;
    memcpy(buffer, buf, *length);
}

void usb_data_out(void *buffer, int *length, void *user_data)
{
    char buf[64];

    memcpy(user_data, buffer, *length);
    
    /* send back the current ticks */
    sprintf(buf, "%lu", sys_ticks);
    *length = 64;
    memcpy(buffer, buf, *length);
}

int main(void)
{
    char buf[64];

    /* configure systick to interrupt every 1ms */
    if (SysTick_Config(SystemCoreClock / 1000)) {
        while(1);
    }

    /* init usb device */
    struct usb_user_cb usb_cb = {
        buf,           /* user data */
        usb_data_in,    /* IN endpoint callback  */
        usb_data_out,   /* OUT endpoint callback */
    };

    usb_dev_init(&usb_cb);

    while (1) {
        /* Do nothing */
    }
}
