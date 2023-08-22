#define led1 18
#define led2 19


void task1(void *pt)//形参-空指针
{
    pinMode(led1, OUTPUT);
    while(1)
    {
      digitalWrite(led1,!digitalRead(led1));
      //delay(1000);//阻塞
      int timeDelay = 2000;//ms

      //vTaskDelay(pdMS_TO_TICKS(timeDelay));//pdMS_TO_TICKS-将时间转换成节拍数
      vTaskDelay(2000/portTICK_PERIOD_MS);//portTICK_PERIOD_MS将ms单位转换成时钟节拍数
    }
}


void task2(void *pt)//形参-空指针
{
    pinMode(led2, OUTPUT);
    while(1)
    {
      digitalWrite(led2,!digitalRead(led2));
      vTaskDelay(1000);//freeRTOS的延时函数
    }
}


//很明显，这样不能是的led1亮1s,led2亮2s。因为代码是一行行运行的，led1亮了4秒
void setup() {
  xTaskCreate(task1, "Blik 10", 1024, NULL, 1, NULL);//task-任务函数，"Blik 10-任务名词", 1024-任务堆栈大小给任务分配的空间，NULL-传递给任务的参数，1-任务优先级，NULL-任务句柄（创建、删除、挂起……）
  xTaskCreate(task2, "Blik 11", 1024, NULL, 1, NULL);
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
}
