SemaphoreHandle_t xSemaLED = NULL;//创建信号量句柄

void consumer(void *ptParma);
void production(void *ptParma);

String consumerA = "JD";
String consumerB = "TMALL";
String consumerC = "PDD";


void consumer(void *ptParma)
{
  String website = *(String *)ptParma;//恢复参数
  while(1)
  {
    if(xSemaphoreTake(xSemaLED, portMAX_DELAY) == pdTRUE)//消费者-1返回pdPASS,或者pdFAIL
    {
      for (int i = 0; i < random(200, 400); i++) vTaskDelay(10);
      Serial.print(website);
      Serial.println("抢到并销售一台: ");
    }
  }
}

void production(void *ptParma)
{
  while(1)
  {
     for (int i = 0; i < random(100, 200); i++) vTaskDelay(10);
    xSemaphoreGive(xSemaLED);
    Serial.println("...... 手机再放出一台,");
  }
}


void setup() {
  Serial.begin(9600);

  xSemaLED = xSemaphoreCreateCounting(3, 0);//创建计数累计信号量,3-MAX,0-当前库存

  if(xSemaLED == NULL)//信号量创建失败
  {
        printf("No Enough Ram, Unable to Create Semaphore.");
  } 
  else 
  {
    xTaskCreate(production,"production",   1024*6, NULL,               1, NULL);
    xTaskCreate(consumer,  "consumer a",   1024*6, (void *)&consumerA, 1, NULL);
    xTaskCreate(consumer,  "consumer b",   1024*6, (void *)&consumerB, 1, NULL);
    xTaskCreate(consumer,  "consumer c",   1024*6, (void *)&consumerC, 1, NULL);
    // xTaskCreate(production,"production",   1024*6, NULL,               1, NULL);
  }


}
void loop() {
  // put your main code here, to run repeatedly:
}
