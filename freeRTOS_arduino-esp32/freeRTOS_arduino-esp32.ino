byte LED1PIN = 18;
byte LED2PIN = 19;

void task1(void *pt)//形参-空指针
{
  byte *pdLEDPIN;
  pdLEDPIN = (byte *)pt;//数据还原
  byte LEDPIN;
  LEDPIN = *pdLEDPIN;//获取值
    pinMode(LEDPIN, OUTPUT);
    while(1)
    {
      digitalWrite(LEDPIN,!digitalRead(LEDPIN));
      int timeDelay = 2000;//ms
      //vTaskDelay(pdMS_TO_TICKS(timeDelay));//pdMS_TO_TICKS-将时间转换成节拍数
      vTaskDelay(2000/portTICK_PERIOD_MS);//portTICK_PERIOD_MS将ms单位转换成时钟节拍数
    }
}

void task2(void *pt)//形参-空指针
{

  byte LEDPIN = *(byte *)pt;//此一步操作即可
  pinMode(LEDPIN, OUTPUT);
  while(1)
  {
    digitalWrite(LEDPIN,!digitalRead(LEDPIN));
    vTaskDelay(1000);//freeRTOS的延时函数
  }
}

//很明显，这样不能是的led1亮1s,led2亮2s。因为代码是一行行运行的，led1亮了4秒
void setup() {
  byte * pbLED1PIN;//定义字节型指针
  pbLED1PIN = &LED1PIN;

  void * pvLED1PIN;
  pvLED1PIN = (void *)pbLED1PIN;//强转

  if(xTaskCreate(task1, 
                 "Blik_10",
                 1024,
                 pvLED1PIN,
                 1,
                 NULL) == pdPASS)//返回值-pdPASS任务创建成功
    Serial.println("Task1 Created");

  if(xTaskCreate(task2, 
                "Blik_11",
                1024,
                (void  *)&LED2PIN,//此一步操作就行
                1,
                NULL) == pdPASS)//返回值-pdPASS任务创建成功
  Serial.println("Task2 Created");

  // xTaskCreate(task1, "Blik_10", 1024, NULL, 1, NULL);//创建动态任务//task-任务函数，"Blik 10-任务名词", 1024-任务堆栈大小给任务分配的空间，NULL-传递给任务的参数，1-任务优先级，NULL-任务句柄（创建、删除、挂起……）
  // xTaskCreate(task2, "Blik_11", 1024, NULL, 1, NULL);
  // put your setup code here, to run once:
}
void loop() {
  // put your main code here, to run repeatedly:
}
