

TickType_t timeOut = 1000;//用于获取信号量的Timeout 1000 ticks //TickType_t是一种数据类型


SemaphoreHandle_t xMutexInventory;//创建信号量句柄



void setup() {
  Serial.begin(115200);
  //指派任务给指定核心，最后一个参数为0-核心1,1-核心2
  if(xTaskCreatePinnedToCore(task1, 
              "Blik_10",
              1024*4,//问题在这将1024改成1024*4即可，问题是如何确定内存
              NULL,
              1,
              NULL,
              0) == pdPASS);//返回值-pdPASS任务创建成功

}

void loop() {

  Serial.println(xPortGetCoreID());//--->由此可验证setpu和loop运行在cup1上//获取当前任务运行的核心
  // put your main code here, to run repeatedly:
}



void task1(void *pt)//形参-空指针
{

  while(1)
  {
    Serial.println(xPortGetCoreID());
  }
}
