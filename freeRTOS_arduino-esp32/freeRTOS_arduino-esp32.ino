void task1(void *pt);

void setup()
{
  Serial.begin(115200);
  xTaskCreate(task1, "show", 1024, NULL, 1, NULL);

}

void loop()
{

}

void task1(void *pt)
{
  static float stockPrice = 99.57;//股票价格
  const TickType_t xFrequency = 3000;//间隔 3000ticks = 3seconds//需要多长时间执行一次

  TickType_t xLastWakeTime = xTaskGetTickCount();//TickType_t是一种重新命名的数据类型-uint32_t
  for(;;)
  {
    Serial.println("xTackGetTickCount: ");Serial.println(xTaskGetTickCount());//用于验证是否真的运行程序是分毫不差的3s
    /*以下，但是有个问题，执行这些程序也是需要时间的，而我们需要分毫不差*/
    // a = xTaskGetTickCount();//获得tick，相当于millis()
    // //1tick=1ms，要求此任务3000ms完成，那么vTickDelay(3000-runtime)
    // b = xTaskGetTickCount();
    // vTaskDelay(3000 - (b - a));
    /*以上，但是有个问题，执行这些程序也是需要时间的，而我们需要分毫不差*/

     // ------- 很复杂的交易股票计算，时间不定 ---------
    stockPrice = stockPrice * (1 + random(1, 20) / 100.0); vTaskDelay(random(500, 2000));
    Serial.print("stockPrice: "); Serial.println(stockPrice);


    xTaskDelayUntil(&xLastWakeTime, xFrequency);//本来xLastWakeTime应该在for循环中不断更新，但是FreeRTOS可以自动更新
  }

}
