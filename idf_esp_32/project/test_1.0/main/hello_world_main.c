
#include <stdio.h>
#include "nvs_flash.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
#include "esp_random.h" ////此处我查ESP API写的%lu
#include "esp_err.h"    //返回错误代码

/*打印内存中的数据*/
void printMemory(unsigned char *address, int size)
{
    int count;
    for (count = 0; count < size; count++)
    {
        printf("%.2x", address[count]);
    }
    printf("\n");
}

uint32_t num = 200; //在内存中打印出

void app_main(void)
{
    char *ocLearn_name_space = "ocLearn_1234"; ////问题在这，更改了ocLearn_1234到ocLearn_1234567就会出现一直是0
    char *part_name = "mynvs";
    ESP_ERROR_CHECK(nvs_flash_init_partition(part_name)); ////注意：分区更改后，这里也要改

    nvs_handle_t ocLearn_handle;                                                            //句柄，所谓句柄就是文件夹的窗口
    nvs_open_from_partition(part_name, ocLearn_name_space, NVS_READWRITE, &ocLearn_handle); ////注意：分区更改后，这里也要改




/*以下，迭代器，可以看出所有的数据*/
    // Example of listing all the key-value pairs of any type under specified partition and namespace
    nvs_iterator_t it = NULL;
    esp_err_t res = nvs_entry_find(part_name, ocLearn_name_space, NVS_TYPE_ANY, &it);
    while (res == ESP_OK)
    {
        nvs_entry_info_t info;
        nvs_entry_info(it, &info); // Can omit error check if parameters are guaranteed to be non-NULL
        printf("key '%s', type '%d' \n", info.key, info.type);
        res = nvs_entry_next(&it);
    }
    nvs_release_iterator(it);
/*以上，迭代器，可以看出所有的数据*/


    nvs_commit(ocLearn_handle); //快速执行nvs_set_u32
    nvs_close(ocLearn_handle);
    nvs_flash_deinit();
}
