
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);
TaskHandle_t taskHandle = NULL;//创建句柄
int taskMem = 1024;


QueueHandle_t queueMsq = xQueueCreate(8, sizeof(char[20]));//创建队列，长度8，尺寸sizeof(char[20])////对于lcd一行能够显示20个字符，存了8行。但是实际上每行20个字符，4行是极限
//返回随机的文字
String randomMsg() {
  String myStrings[] = {
    "Nice to meet you",
    "Where are U from?",
    "What do you do?",
    "What do U like?",
    "What is UR num?",
    "Do U have FB?",
    "Thanks so much.",
    "I am Chinese.",
    "I do not KNOW.",
    "Thank you.",
    "That helps.",
    "I Love U",
    "Do U miss me?",
    "Be careful.",
    "Don't worry.",
    "Good idea.",
    "He's right.",
    "I ate already.",
    "More than that.",
    "Nothing else.",
    "See you later.",
    "Take it outside.",
  };
  return myStrings[random(0, 22)];
}










void taskA(void *ptParam);
void taskB(void *ptParma);

void setup() {
  Serial.begin(9600);

  xTaskCreate(taskA,"taskA",taskMem*8,NULL,1,NULL);
  xTaskCreate(taskB,"taskB",taskMem*8,NULL,1,NULL);
  xTaskCreate(taskC,"taskC",taskMem*8,NULL,1,NULL);

  xTaskCreate(lcdTask,"lcd",1024*8,NULL,1,NULL);

}
void loop() {
  // put your main code here, to run repeatedly:
}




void taskA(void *ptParam)
{
  char msg[20];
  String userID = "A: ";

  while(1)
  {
    (userID+randomMsg()).toCharArray(msg,20);//ID+随机返回的一行话。将上述的这些内容存放到msg数组里
    
    // TickType_t timeOut = portMAX_DELAY;//无限期等待49天
    TickType_t timeOut = 10;//等待10个tick
    if (xQueueSend(queueMsq, &msg, timeOut) != pdPASS) //向队列中发送数据，若果队列数据是满的，那么等待timeOut个tick,如果等待这么长时间后依旧是满的，那么就舍弃了这次写操作
    { 
      Serial.print(userID);//获取开机至当前tick
      Serial.println("Queue is full.");
    }
    vTaskDelay(2000);
  }
}

void taskB(void *ptParam)
{
  char msg[20];
  String userID = "B: ";

  while(1)
  {
    (userID + randomMsg()).toCharArray(msg, 20);//将ID+随机显示的存入数组msg中

    TickType_t timeOut = 10;
    if(xQueueSend(queueMsq, &msg, timeOut) != pdPASS)//当timeOut内队列仍旧是满的，则放弃这一次的发送
    {
      Serial.print(userID);
      Serial.println("Queue is full.");
    }

    vTaskDelay(2000);
  }

}

void taskC(void *ptParam)
{
  char msg[20];
  String userID = "C: ";

  while(1)
  {
    (userID + randomMsg()).toCharArray(msg, 20);//将ID+随机显示的存入数组msg中

    TickType_t timeOut = 10;
    if(xQueueSend(queueMsq, &msg, timeOut) != pdPASS)//当timeOut内队列仍旧是满的，则放弃这一次的发送
    {
      Serial.print(userID);
      Serial.println("Queue is full.");
    }

    vTaskDelay(2000);
  }

}



void lcdTask(void *ptParam) {  //LCD任务主体

  lcd.init();
  lcd.backlight();

  char line0[20] = {' '};
  char line1[20] = {' '};
  char line2[20] = {' '};
  char line3[20] = {' '};
  char * lines[] = { line0, line1, line2, line3 };

  while (1) {
    //文字向上滚动
    strcpy(line0, line1);
    strcpy(line1, line2);
    strcpy(line2, line3);

    //TickType_t timeOut = portMAX_DELAY;
    TickType_t timeOut = 10;
    //timeOut-运行这句话时一直是空的，那么就等10个tick，倘若10个tick内读取一直是空的那么就放弃
    if (xQueueReceive(queueMsq, lines[3], timeOut) == pdPASS) {//lines[3]-将最新的话显示在最后一行
      //显示所有的4行文字
      for (int i = 3; i >= 0; i--) {
        lcd.setCursor(0, i);
        lcd.print("                    "); //clear this line
        lcd.setCursor(0, i);
        lcd.print(lines[i]);
      }
    }  else {
      Serial.println("Message Queue is Empty");
    };




     vTaskDelay(10);
  }
}