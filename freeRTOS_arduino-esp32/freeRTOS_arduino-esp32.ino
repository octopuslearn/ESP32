//                 1024*4,//问题在这将1024改成1024*4即可，问题是如何确定内存
//while(1){};vTaskDelay(10);//???while循环还能加分号吗



volatile uint32_t inventory = 100;//总库存
volatile uint32_t retailCount = 0;//线下销售量

void task1(void *pt)//形参-空指针
{
    while(1)
    {
      uint32_t inv = inventory;
      for(int i; i < random(10, 100); i++)//random(10,100)-生成10-100随机数
        vTaskDelay(pdMS_TO_TICKS(i));//将时间转换成节拍数
      if(inventory > 0)//有库存
      {
        inventory = inv - 1;//库存--
        retailCount++;//销售量++
      }
    };//???while循环还能加分号吗
    vTaskDelay(10);//减慢速度，以待升级
}

void task2(void *pt)//形参-空指针
{
    while(1)
    {
      printf("Inventory : %d\n", inventory);
      printf("retailCount : %d\n", retailCount);//打印当前库存和销量

      if(inventory == 0)
      {
        printf("\n----SALES SUMMARY----\n");
        printf("Total Sales: %d\n\n", retailCount);
      }
    }
    vTaskDelay(pdMS_TO_TICKS(100));
}


void setup() {
  Serial.begin(115200);
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
  //               NULL) ;

  if(xTaskCreate(task1, 
                 "Blik_10",
                 1024*4,//问题在这将1024改成1024*4即可，问题是如何确定内存
                 NULL,
                 1,
                 NULL) == pdPASS)//返回值-pdPASS任务创建成功
    Serial.println("Task1 Created");

  if(xTaskCreate(task2, 
                "Blik_11",
                1024*4,
                NULL,//此一步操作就行
                1,
                NULL) == pdPASS)//返回值-pdPASS任务创建成功
    Serial.println("Task2 Created");
}
void loop() {
  // put your main code here, to run repeatedly:
}
