
#include <stdio.h>
#include "nvs_flash.h"
#include "esp_log.h"





void app_main(void)
{
    char* ocLearn_name_space = "ocLearn_1234";
    nvs_flash_init();//相当于把u盘插到电脑上
    nvs_handle_t ocLearn_handle;//句柄，所谓句柄就是文件夹的窗口

   
    nvs_open(ocLearn_name_space,NVS_READWRITE, &ocLearn_handle);//这一步相当于建立了个ocLearn_handle文件夹，并打开了他，他是可读可写的//NVS_READWRITE-给了读写权限

    //read
    //write

    uint32_t counter_val = 0;
    char* counter_key = "counter";
    nvs_get_u32(ocLearn_handle, counter_key, &counter_val);//找文件夹里有没有counter文件,有的话计数
    ESP_LOGI("NVS","KEY:VALUE %s:%ld ",counter_key,counter_val);
    
    counter_val++;

    nvs_set_u32(ocLearn_handle, counter_key, counter_val);//到文件夹里创建了一个新的文件counter文件，value是1

    nvs_close(ocLearn_handle);
    nvs_flash_deinit();
    //I (367) NVS: KEY:VALUE counter:1 
}