#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_system.h"
//#include "esp_spi_flash.h"

#include "test1.h"
void app_main(void)
{
    test1();
    vTaskDelay(100);
}
