
#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

int count = 0;
void app_main(void)
{  
    count = 100;
    ESP_LOGI("COUNTER","Tick(ms) : %ld",portTICK_PERIOD_MS);
    while(1)
    {
        ESP_LOGI("COUNTER","Count Value is %d", count);
        count++;
        //delay(1000);
        // vTaskDelay(1000);//1000 ticks . 1ticks ???? ms--->1ticks-10ms
        //I (10328) COUNTER: Count Value is 指开机后时间-ms,即10s
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }


    // for(;;)
    // {
    // }
}
