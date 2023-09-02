//注意：创建任务时，每个参数一定要传递正确
  //此处最后一个参数错误最后一个参数都是NULL-为啥错了就不行 xTaskCreate(taskWait, "taskWait", 1024*4, NULL, 1, &xTaskGive);
  // xTaskCreate(taskWait, "taskWait", 1024*4, NULL, 1, &xTaskWait);


//notification 只能操作value,不能操作state.因为state是系统自己操作的
// api:xTaskNotify(xTaskWaite, 0, eIncrement);//将对方value +1
// api:xResult = xTaskNotifyWait(0x00,//当接收到这个命令之前，notification要不要被清除清除那几位。。。0x00即不清除
// 0x00,//运行notification后，要不要清除notification清除哪几位，0x00不清除//重置所有的bits 0x00 or ULONG_MAX 0r 0xffffffff
// &ulNotificationValue,//将接收到的值存在这里//重置前的notification value
// protMAX_DELAY);//block不占用cpu资源//一直等待，直到接收到nocation 为止

void taskGive(void *ptParam);
void taskWait(void *ptParam);

static TaskHandle_t xTaskWait = NULL;
static TaskHandle_t xTaskGive = NULL;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  xTaskCreate(taskGive, "taskGive", 1024*4, NULL, 1, &xTaskGive);
  //此处最后一个参数错误最后一个参数都是NULL-为啥错了就不行 xTaskCreate(taskWait, "taskWait", 1024*4, NULL, 1, &xTaskGive);
  xTaskCreate(taskWait, "taskWait", 1024*4, NULL, 1, &xTaskWait);
}

void loop() {
  // put your main code here, to run repeatedly:
}






void taskGive(void *ptParam)
{
  pinMode(23, INPUT_PULLUP);

  BaseType_t xResult;
  while(1)
  {
    if (digitalRead(23) == LOW) 
    {
      xResult = xTaskNotify(xTaskWait, 0, eIncrement);//Increment增量，即累加//对方的notification value +1
      Serial.println(xResult ==  pdPASS ? "成功\n" : "失败\n");

      vTaskDelay(120);
    }
  }
  
}


void taskWait(void *ptParam)
{
  uint32_t ulNotificationValue;//用来存放任务的4个字节的notification value
  BaseType_t xReslt;
  while(1)
  {
    xReslt = xTaskNotifyWait(0x00,//在运行这个指令前，要不要清除notification,清除哪几位。。。0x00不清除
                                   0x00,//运行notification后，要不要清除notification清除哪几位。。。0x00不清除//重置所有的bits 0x00 or ULONG_MAX or 0xffffffff
                                   &ulNotificationValue,//将接受到的值存在这里//重置前的notification value
                                   portMAX_DELAY);//一直等待，直到有通知来。此时Block状态，不占用cpu
    if(xReslt == pdTRUE)
    {
      Serial.println(ulNotificationValue);
    }
    else 
    {
      Serial.println("Timeout");
    }
  }
}