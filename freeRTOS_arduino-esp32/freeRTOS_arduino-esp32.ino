//                 1024*4,//问题在这将1024改成1024*4即可，问题是如何确定内存
//while(1){};vTaskDelay(10);//???while循环还能加分号吗

TickType_t timeOut = 1000;//用于获取信号量的Timeout 1000 ticks //TickType_t是一种数据类型


volatile uint32_t inventory = 100;//总库存
volatile uint32_t retailCount = 0;//线下销售量
volatile uint32_t onlineCount = 0;//线上销售量


SemaphoreHandle_t xMutexInventory;//创建信号量句柄

void task1(void *pt)//形参-空指针
{

    while(1)
    {
      //要在需要使用的时候获取钥匙，这也是将程序写在这里而不是while前的原因
      //在timeout的时间内如果能够获取二进制信号量就继续
      //通俗一些：获取钥匙
      if(xSemaphoreTake(xMutexInventory, timeOut) == pdPASS)//获取信号量成功
      {
          //被MUTEX保护的内容叫 Critical Section//被保护的内容就是锁在保险柜里的资源//以下代码越短越好，使用完了好让别人使用
          //以下实现了带有随机延迟的 inventory减1
          //等效为 inventory--;invent++;
        uint32_t inv = inventory;
        for(int i; i < random(10, 100); i++)//random(10,100)-生成10-100随机数
          vTaskDelay(pdMS_TO_TICKS(i));//将时间转换成节拍数
        if(inventory > 0)//有库存
        {
          inventory = inv - 1;//库存--
          retailCount++;//销售量++

          //释放钥匙
          xSemaphoreGive(xMutexInventory);
        }
      }
      else{//无法获得钥匙}
    };//???while循环还能加分号吗
    vTaskDelay(10);//减慢速度，以待升级
  }
}

void task2(void *pt)//形参-空指针
{
   uint32_t sum;
    while(1)
    {

      printf("Inventory : %d\n", inventory);
      printf("retailCount: %d   onlineCount %d\n", retailCount,onlineCount);//打印当前库存和销量

      if(inventory == 0)
      {
        sum = onlineCount + retailCount;
        printf("\n----SALES SUMMARY----\n");
        printf("Total Sales: %d\n\n", sum);
        printf("sum over : %d\n",100 - sum);
      }
      vTaskDelay(pdMS_TO_TICKS(100));
    } 
}

void task3(void * pt)
{
  while(1)
  {
    uint32_t val = inventory;
    for(int i;i < random(10,100);i++)
      vTaskDelay(pdMS_TO_TICKS(i));
    if(inventory > 0)
    {
      inventory = val - 1;
      onlineCount++;
    }
    vTaskDelay(10);//老板要求慢一点，客户升级后，可提速
  }
}


void setup() {
  Serial.begin(115200);

  xMutexInventory = xSemaphoreCreateMutex();//创建MUTEX
  if(xMutexInventory == NULL)//NULL-互斥信号量创建失败。
  {
    printf("No Enough Ram,Unable to Create Semaphore.");
  }
  else
  {
        // xTaskCreate(task1, 
    //                "Blik_10",
    //                1024*4,
    //                NULL,
    //                1,
    //                NULL);

    // xTaskCreate(task2, 
    //               "Blik_11",
    //               1024*4,
    //               NULL,//此一步操作就行
    //               1,
    //               NULL) ;S

    if(xTaskCreate(task1, 
                  "Blik_10",
                  1024*4,//问题在这将1024改成1024*4即可，问题是如何确定内存
                  NULL,
                  1,
                  NULL) == pdPASS);//返回值-pdPASS任务创建成功

    if(xTaskCreate(task2, 
                  "Blik_11",
                  1024*4,
                  NULL,//此一步操作就行
                  1,
                  NULL) == pdPASS);//返回值-pdPASS任务创建成功

    if(xTaskCreate(task3,
                  "onlineCount",
                  1024*4,
                  NULL,
                  1,
                  NULL) == pdPASS);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
}
