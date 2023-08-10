/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include "esp_log.h"

void app_main(void)
{
    //E error
    ESP_LOGE("error","不亮了");
    //W warning
    ESP_LOGW("waring","还能活两天");
    //I information
    ESP_LOGI("information","还能活一天");



    //D Debug
    ESP_LOGD("Debug","过度使用10000小时");
    //V verbose
    ESP_LOGV("verbose","损毁严重");
/*
    //N no log output
    // ESP_LOGN("no log output","错误使用");//写ESP_LOGN就报错
//    26 |     ESP_LOGN("no log output","閿欒浣跨敤");
//       |     ^~~~~~~~
//       |     ESP_LOGD
*/

    fflush(stdout);//将标准输出冲马桶
}
