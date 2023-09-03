TaskHandle_t xflashLED = NULL;
void flashLED(void *pvParam);
void ReadBth(void *pvParam);



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  xTaskCreate(flashLED,"Flash LED", 1024*4, NULL, 1, &xflashLED);
  xTaskCreate(ReadBth,"Read Button", 1024*4, NULL, 1, NULL);
}

void loop() {
  // put your main code here, to run repeatedly:
}



void flashLED(void *pvParam)
{
  pinMode(23, OUTPUT);
  uint32_t ulNotificationValue;
  while(1)
  {
    //注意：此函数返回的是运行这个函数之前的值，也就是说是1
    //命令含义：waitting for notification,then reset
    ulNotificationValue = ulTaskNotifyTake(pdTRUE,//运行完后，清零
                                           portMAX_DELAY);

    if(ulNotificationValue > 0)
    {
      digitalWrite(23, !digitalRead(23));
      vTaskDelay(1000);
    }
  }
}

void ReadBth(void *pvParam)
{
  pinMode(22, INPUT_PULLUP);
  while(1)
  {
    if(digitalRead(22) == LOW) {
      xTaskNotifyGive(xflashLED);//相当于xTaskNotify()+eIncrement//也就是xSemaphoreGive
      vTaskDelay(120);
    }
  }
}

