byte LED1PIN = 18;
byte LED2PIN = 19;

typedef struct {//定义结构体
  byte ledpin;//led引脚
  int delayTime;//延时时间
}LEDFLASH;

void task1(void *pt)//形参-空指针
{
  LEDFLASH *ptLedFlash = (LEDFLASH *)pt;
  byte pin = ptLedFlash -> ledpin;
  int delayTime = ptLedFlash -> delayTime;
    pinMode(pin, OUTPUT);
    while(1)
    {
      digitalWrite(pin,!digitalRead(pin));
      vTaskDelay(delayTime/portTICK_PERIOD_MS);//portTICK_PERIOD_MS将ms单位转换成时钟节拍数
    }
}

void task2(void *pt)//形参-空指针
{
  LEDFLASH *ptLedFlash = (LEDFLASH *)pt;
  byte pin = ptLedFlash -> ledpin;
  int delayTime = ptLedFlash -> delayTime;
    pinMode(pin, OUTPUT);
    while(1)
    {
      digitalWrite(pin,!digitalRead(pin));
      vTaskDelay(delayTime/portTICK_PERIOD_MS);//portTICK_PERIOD_MS将ms单位转换成时钟节拍数
    }
}



//很明显，这样不能是的led1亮1s,led2亮2s。因为代码是一行行运行的，led1亮了4秒
void setup() {
  LEDFLASH led1,led2;//声明结构体
  led1.ledpin = LED1PIN;//初始化结构体
  led2.ledpin = LED2PIN;
  led1.delayTime = 1000;
  led2.delayTime = 2000;

  if(xTaskCreate(task1, 
                 "Blik_10",
                 1024,
                 (void *)&led1,
                 1,
                 NULL) == pdPASS)//返回值-pdPASS任务创建成功
    Serial.println("Task1 Created");

  if(xTaskCreate(task2, 
                "Blik_11",
                1024,
                (void  *)&led2,//此一步操作就行
                1,
                NULL) == pdPASS)//返回值-pdPASS任务创建成功
  Serial.println("Task2 Created");
}
void loop() {
  // put your main code here, to run repeatedly:
}
