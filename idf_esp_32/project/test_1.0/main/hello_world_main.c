
#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LED_PIN CONFIG_LED_GPIO_NUM//define的方式不占用内存
#define DELAY_MS CONFIG_DELAY_TIME_MS

uint32_t staus=0;

void app_main(void)
{  
    gpio_reset_pin(LED_PIN);//重置引脚2
    gpio_set_direction(LED_PIN,GPIO_MODE_OUTPUT);

    ESP_LOGI("blink","GPIO : %d",LED_PIN);
    ESP_LOGI("blink","DELAY : %d",DELAY_MS);

    while(1)
    {
        staus =! staus; 
        gpio_set_level(LED_PIN,staus);
        vTaskDelay(DELAY_MS/portTICK_PERIOD_MS);
    }
}
