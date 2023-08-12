
#include <stdio.h>
#include "nvs_flash.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
#include "esp_random.h"////此处我查ESP API写的%lu
#include "esp_err.h"//返回错误代码


/*打印内存中的数据*/
void printMemory(unsigned char *address, int size)
{
    int count;
    for(count=0; count<size;count++)
    {
        printf("%.2x", address[count]);
    }
    printf("\n");
}


uint32_t num = 200;//在内存中打印出


void app_main(void)
{
    //vTaskDelay(1000/portTICK_PERIOD_MS);//I (1366) NVS: KEY:VALUE counter:0 
    
    // printMemory((unsigned char *)&num,4);
    // num=100;
    // printMemory((unsigned char *)&num,4);

    char* ocLearn_name_space = "ocLearn_1234";////问题在这，更改了ocLearn_1234到ocLearn_1234567就会出现一直是0
    esp_err_t err = nvs_flash_init();//相当于把u盘插到电脑上
    ESP_ERROR_CHECK(err);//倘若，NVS初始化不成功，会用尝试重启的方式再次初始化
    nvs_handle_t ocLearn_handle;//句柄，所谓句柄就是文件夹的窗口
    nvs_open(ocLearn_handle,NVS_READWRITE, &ocLearn_handle);//这一步相当于建立了个ocLearn_handle文件夹，并打开了他，他是可读可写的//NVS_READWRITE-给了读写权限



uint32_t max_ap=20;
//结构体
typedef struct{
    char ssid[50];
    char password[50];
} ap_t;
ap_t aps_set[max_ap];//创建结构体的数组////声明
memset(aps_set,0,sizeof(aps_set));//初始化//声明后必须初始化



for(int i=0;i<max_ap;i++)
{
    strcpy(aps_set[i].ssid,"ocLean");//c++中不让这么用aps_set[i].ssid="ocLean";而应该strcpy(aps_set[i].ssid,"ocLean");
    strcpy(aps_set[i].password,"123456");
    // aps_set[i].ssid="ocLean";//c++中不让这么用aps_set[i].ssid="ocLean";而应该strcpy(aps_set[i].ssid,"ocLean");
}


char key[15];
sprintf(key,"key %lu", esp_random());////此处我查资料字节写的%lu
esp_err_t erro = nvs_set_blob(ocLearn_handle,key,aps_set,sizeof(aps_set));//向NVS中写入了复杂二进制数据
nvs_commit(ocLearn_handle);//快速执行nvs_set_u32
if(erro ==  ESP_OK)
{
    esp_restart();//它会导致设备立即重新启动，从而重新加载固件和初始化
}
else
{
    ESP_LOGE("NVS", "nvs_set_blob erro:  %s", esp_err_to_name(erro));
    // esp_err_to_name_r(erro);
    // ESP_LOGI("NVS", "nvs_set_blob erro:  %d", erro);//ESP_ERR_NVS_INVALID_HANDLE (0x1107): Handle has been closed or is NUL
}


ap_t aps_get[max_ap];//创建结构体的数组
size_t length = sizeof(aps_get);
nvs_get_blob(ocLearn_name_space,"aps",aps_get,&length);//向NVS中写入了复杂二进制数////文件夹-文件-内容-内容长度


for(int i=0;i<max_ap;i++)
{
        //ESP_LOGI("NVS","ssid:password  %s:%s",aps_get[i].ssid,aps_get[i].password);
}

    nvs_commit(ocLearn_handle);//快速执行nvs_set_u32

    nvs_close(ocLearn_handle);
    nvs_flash_deinit();
    //I (367) NVS: KEY:VALUE counter:1 
}