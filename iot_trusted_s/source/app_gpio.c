#include "app_gpio.h"
#include "fsl_clock.h"

#define SECURE_GPIO_CLOCK         kCLOCK_Gpio_Sec

void ButtonLedInit()
{
    CLOCK_EnableClock(SECURE_GPIO_CLOCK);
    GPIO_PortInit(SW1_GPIO, SW1_GPIO_PORT);
    GPIO_PortInit(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PORT);
    GPIO_PinInit (SW1_GPIO, SW1_GPIO_PORT, SW1_GPIO_PIN, \
                  &(gpio_pin_config_t){kGPIO_DigitalInput, 0});
    LED_GREEN_INIT(0x1U);
}

