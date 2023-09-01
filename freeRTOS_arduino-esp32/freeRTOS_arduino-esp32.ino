SemaphoreHandle_t xSemaLED = NULL;//创建信号量句柄

TickType_t timeOut = 1000;

void button(void *ptParma);
void led(void *ptParma);
void button(void *ptParma)
{
  pinMode(22, INPUT_PULLUP);
  while(1)
  {
    if(digitalRead(22) == LOW)//###发现按键按下去了，则给了个信号量--->内存中的这个量就变成1
    {
      xSemaphoreGive(xSemaLED);
      vTaskDelay(100);
    }
  }
}

void led(void *ptParma)
{
  pinMode(23, OUTPUT);
  while(1)
  {
    if(xSemaphoreTake(xSemaLED, timeOut) == pdTRUE)//###判断当前信号量，是1则执行下面的程序，并且将1变成了0
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
