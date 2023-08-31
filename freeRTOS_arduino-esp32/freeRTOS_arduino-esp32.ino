//arduino esp32 默认在core0开启看门狗5s的IDLE任务 
//core0和1都运行IDLE任务，优先级0
//  IDLE任务时用于清除被删除的任务
//core1 运行loopBack任务，即setup loop优先级1

#include "esp_task_wdt.h"

TaskHandle_t taskHandle;

void demo1();
void taskA(void *ptParma);

void demo2();
void taskB(void *ptParma);


void setup()
{
  Serial.begin(9600);
  //disableCore0WDT();//法3：手动关闭CPU上的TWDT////慎重使用
  //demo1();//core0默认开启了看门狗，倘若没有运行IDLE或者没有手动喂狗，或者没有关闭看门狗，那么5s会自动重启（默认值，arduino esp32上不知道如何更尬）
  esp_task_wdt_add(NULL);//NULL指将本任务添加给看门狗//demo2手动喂狗
  demo2();//core1没有默认开启看门狗，所有task占用资源不放，也不会重启



//###demo1和demo2一起运行，demo1 core0 有看门狗，taskA占用资源，无法运行IDLE-会重启。demo2 core1 无看门狗，taskB占用资源，不会重启。
//   lookBack Task - Priority 1
// lookBack Task - Priority 1
// lookBack TasE (10100) task_wdt: Task watchdog got triggered. The following tasks did not reset the watchdog in time:
// E (10100) task_wdt:  - IDLE (CPU 0)
// E (10100) task_wdt: Tasks currently running:
// E (10100) task_wdt: CPU 0: taskA
// E (10100) task_wdt: CPU 1: taskB
// E (10100) task_wdt: Aborting.

// abort() was called at PC 0x420082d0 on core 0


// Backtrace: 0x40376ff6:0x3fc923e0 0x4037a1b5:0x3fc92400 0x4037f845:0x3fc92420 0x420082d0:0x3fc924a0 0x40377fa5:0x3fc924c0 0x4202093c:0x3fced500




// ELF file SHA256: d25f26f1da1bce4f
}

void loop()
{
   esp_task_wdt_reset();//demo2手动喂狗

//    Rebooting...
// �E (10099) task_wdt: Task watchdog got triggered. The following tasks did not reset the watchdog in time:
// E (10099) task_wdt:  - loopTask (CPU 1)
// E (10099) task_wdt: Tasks currently running:
// E (10099) task_wdt: CPU 0: IDLE
// E (10099) task_wdt: CPU 1: taskB
// E (10099) task_wdt: Aborting.

// abort() was called at PC 0x420082b8 on core 0


// Backtrace: 0x40376ff6:0x3fc923e0 0x4037a1b5:0x3fc9
  Serial.println("lookBack Task - Priority 1");
}






void demo1()
{
  //core0上运行两个任务，一个是IDLE 优先级0，一个是taskA优先级1
  xTaskCreatePinnedToCore(taskA, "taskA", 1024*4, NULL, 1, NULL, 0);
}

void taskA(void *ptParma)
{
  //core0上运行两个任务，一个是IDLE 优先级0，一个是taskA 优先级1
  //由于taskA优先级1高，taskA没有Block和Suspend所以不会释放资源，所以不会运行IDLE
  //会重启的理由：core0默认开启看门狗，默认时间5s，由于无法运行IDLE，所以无法喂狗，所以5s重启
  // lookBack Task - Priority 1
// lookBack Task - Priority 1
// lookBack Task - Priority 1
// lookBE (10098) task_wdt: Task watchdog got triggered. The following tasks did not reset the watchdog in time:
// E (10098) task_wdt:  - IDLE (CPU 0)
// E (10098) task_wdt: Tasks currently running:
// E (10098) task_wdt: CPU 0: taskA

  //解决方法1：taskA Block(vTaskDelay)或者Suspend(vTaskSuspend)
// lookBack Task - Priority 1
// lookBack Task - Priority 1
// lookBack Task - Priority 1
// lookBack Task - Priority 1
// lookBack Task - Priority 1
  while(1)
  {
    //vTaskDelay(1);//法1，Block
    // TaskHandle_t taskHandle;//法2 Suspend
    //vTaskSuspend(taskHandle);
  }
}


void demo2()
{
  xTaskCreatePinnedToCore(taskB, "taskB", 1024*4, NULL, 2, NULL, 1);
}

void taskB(void *ptParma)
{
  while(1)//有5s的看门狗
  {
     vTaskDelay(1);
     //demo2 Block 运行了优先级1的loopBlock-手动喂狗。###但是IDEL 优先级为0仍旧无法运行------------->必须给loopBlock中也弄一个tick才行
    //vTaskSuspend(taskHandle);
  }
}



