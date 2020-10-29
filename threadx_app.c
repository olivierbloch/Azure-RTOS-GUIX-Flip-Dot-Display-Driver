#include "tx_api.h"
#include "printf.h"
#include "os_hal_gpio.h"

TX_TIMER                my_timer;

static const uint8_t gpio_num = OS_HAL_GPIO_9;

static int gpio_output(u8 gpio_no, u8 level)
{
	mtk_os_hal_gpio_set_direction(gpio_no, OS_HAL_GPIO_DIR_OUTPUT);
	mtk_os_hal_gpio_set_output(gpio_no, level);

	return 0;
}

static int gpio_input(u8 gpio_no, os_hal_gpio_data *pvalue)
{
	mtk_os_hal_gpio_set_direction(gpio_no, OS_HAL_GPIO_DIR_INPUT);
	mtk_os_hal_gpio_get_input(gpio_no, pvalue);

	return 0;
}

static bool led_on = true;

void timer_expired(ULONG param)
{
    gpio_output(gpio_num, led_on);
    led_on = !led_on;
}

/* Define main entry point.  */

int main(void)
{
    /* Enter the ThreadX kernel.  */
    tx_kernel_enter();
}


/* Define what the initial system looks like.  */

void    tx_application_define(void *first_unused_memory)
{
    
    tx_timer_create(&my_timer, "myTimer", timer_expired, 0, TX_TIMER_TICKS_PER_SECOND, TX_TIMER_TICKS_PER_SECOND, TX_AUTO_ACTIVATE);

}
