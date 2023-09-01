SemaphoreHandle_t xSemaLED = NULL;//创建信号量句柄

TickType_t timeOut = 1000;

void button(void *ptParma);
void led(void *ptParma);
void button(void *ptParma)
{
  pinMode(22, INPUT_PULLUP);
  while(1)
  {
    if(digitalRead(22) == LOW)
    {
      xSemaphoreGive(xSemaLED);//发送信号
      vTaskDelay(100);
    }
  }
}

void led(void *ptParma)
{
  pinMode(23, OUTPUT);
  while(1)
  {
    if(xSemaphoreTake(xSemaLED, timeOut) == pdTRUE)//消费者-1返回pdPASS,或者pdFAIL
    {
      digitalWrite(23,!digitalRead(23));
      vTaskDelay(1000);
    }
  }
}



void setup() {
  Serial.begin(9600);

  xSemaLED = xSemaphoreCreateBinary();//创建二进制信号量,返回NULL或者handler

  if(xSemaLED == NULL)//信号量创建失败
  {
        printf("No Enough Ram, Unable to Create Semaphore.");
  } 
  else 
  {
    xTaskCreate(led, "led", 1024*4, NULL, 1, NULL);
    xTaskCreate(button, "button", 1024*4, NULL, 1, NULL);
  }

}
void loop() {
  // put your main code here, to run repeatedly:
}
