/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>

void app_main(void)
{
    printf("Hello world!");//没有\n啥也打印不出来,可是实测可以打印出来
    printf("sss");
    fflush(stdout);//将标准输出冲马桶
}
