
#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

int count = 0;
char ptrTaskList[250];


void app_main(void)
{  
    /*不知道上哪去设置，v5.1没找到，找到了*/
    vTaskList(ptrTaskList);
    printf("***********************\n");
    printf("Task        State       Prio    Stack   Num\n");
    printf("***********************\n");
    printf(ptrTaskList);
    printf("***********************\n");
    /*不知道上哪去设置，v5.1没找到*/
//5.1需要找到FreeRTOS>Kernel>configUSE_TRACE_FACILITY（打勾）>Enable display of xCoreID in vTaskList（打勾）
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
