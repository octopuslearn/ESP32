
#include <stdio.h>
#include "nvs_flash.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
#include "esp_random.h" ////此处我查ESP API写的%lu
#include "esp_err.h"    //返回错误代码



void app_main(void)
{
    vTaskDelay(1000/portTICK_PERIOD_MS);//I (1366) NVS: KEY:VALUE counter:0 


    nvs_flash_init();
    char *ocLearn_name_space = "ocLearn_1234"; ////问题在这，更改了ocLearn_1234到ocLearn_1234567就会出现一直是0
    char *part_name = "mynvs";
    ESP_ERROR_CHECK(nvs_flash_init_partition(part_name)); ////注意：分区更改后，这里也要改

    nvs_handle_t ocLearn_handle;                                                            //句柄，所谓句柄就是文件夹的窗口
    nvs_open_from_partition(part_name, ocLearn_name_space, NVS_READWRITE, &ocLearn_handle); ////注意：分区更改后，这里也要改




    // Example of nvs_get_stats() to get the number of used entries and free entries:
    nvs_stats_t nvs_stats;
    nvs_get_stats("nvs", &nvs_stats);
    printf("Count: UsedEntries = (%d), FreeEntries = (%d), AllEntries = (%d)\n",
           nvs_stats.used_entries, nvs_stats.free_entries, nvs_stats.total_entries);


    nvs_get_stats(ocLearn_handle, &nvs_stats);
    printf("Count: UsedEntries = (%d), FreeEntries = (%d), AllEntries = (%d)\n",
           nvs_stats.used_entries, nvs_stats.free_entries, nvs_stats.total_entries);



    nvs_commit(ocLearn_handle); //快速执行nvs_set_u32
    nvs_close(ocLearn_handle);
    nvs_flash_deinit();
}
