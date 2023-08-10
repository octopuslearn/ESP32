
#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LED_PIN GPIO_NUM_2//define的方式不占用内存
#define DELAY_MS 2000

uint32_t staus=0;

void app_main(void)
{  
    gpio_reset_pin(LED_PIN);//重置引脚2
    gpio_set_direction(LED_PIN,GPIO_MODE_OUTPUT);
    while(1)
    {
        staus =! staus; 
        gpio_set_level(LED_PIN,staus);
        vTaskDelay(DELAY_MS/portTICK_PERIOD_MS);
    }
}
