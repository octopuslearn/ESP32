TaskHandle_t taskHandle;

void demo1();
void taskA(void *ptParma);



void setup()
{
  Serial.begin(9600);
  demo1();
}

void loop()
{
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
    // vTaskSuspend(taskHandle);
  }
}


