/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include "esp_log.h"

const char* tag="绒布球";

void app_main(void)
{

/*E,W,I是给用户看的*/
    //E error
    ESP_LOGE(tag,"眼睛失去高光");
    //W warning
    // ESP_LOGW(tag,"寿命剩余50%%");//需要两个%，否则会报错
    ESP_LOGW(tag,"寿命剩余%i%%",50);//可以把数字弄出来
    //I information
    ESP_LOGI(tag,"寿命剩余10%%");




/*D和V是给程序员看的，所以一般选的INFO*/
    //D Debug
    ESP_LOGD(tag,"过度使用10000小时");
    //V verbose
    ESP_LOGV(tag,"损毁严重");
/*//因为是指啥都不显示
    //N no log output
    // ESP_LOGN("no log output","错误使用");//写ESP_LOGN就报错
//    26 |     ESP_LOGN("no log output","閿欒浣跨敤");
//       |     ^~~~~~~~
//       |     ESP_LOGD
*/

    fflush(stdout);//将标准输出冲马桶
}
