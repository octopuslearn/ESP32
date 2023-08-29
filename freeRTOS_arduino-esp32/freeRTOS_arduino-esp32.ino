
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);
TaskHandle_t taskHandle = NULL;//创建句柄
int taskMem = 1024;
void taskA(void *ptParam);
void taskB(void *pt);
void lcdClear();//清除屏幕
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  int heapSize = ESP.getHeapSize();//获取堆大小
  Serial.print("heapSize: ");Serial.println(heapSize);
  int heapFree = ESP.getFreeHeap();//获取空闲堆
  Serial.print("heapFree: ");Serial.println(heapFree);


  xTaskCreate(taskA,//创建完任务后，就给个这个任务一桶水，水就是内存
            "get now freeHeap",
            taskMem*8,
            NULL,
            1,
            NULL);


  int heapSizes = ESP.getHeapSize();//获取堆大小
  Serial.print("任务创建后heapSize: ");Serial.println(heapSizes);
  int heapFrees = ESP.getFreeHeap();//获取空闲堆
  Serial.print("任务创建后heapFree: ");Serial.println(heapSizes);
}
void loop() {
  // put your main code here, to run repeatedly:
}




void taskA(void *ptParam)
{
  pinMode(32, INPUT_PULLUP);
  pinMode(33, INPUT_PULLUP);
  pinMode(25, INPUT_PULLUP);
  pinMode(26, INPUT_PULLUP);
  while(1)
  {
    //创建任务
    if (digitalRead(32) == LOW) 
    {
      if(taskHandle == NULL)//倘若任务未创建
      {
        if(xTaskCreate(taskB, "LCD_show", taskMem*8,  NULL, 1, &taskHandle) == pdPASS)
        {
          Serial.print(xTaskGetTickCount());//获取开机至当前tick
          Serial.println("- LOG: Task is Created.");
        }
        else
        {
          Serial.print(xTaskGetTickCount());
          Serial.println("- WARNING: Unable to Create Task.");
        }
      }
      else
      {
        Serial.print(xTaskGetTickCount());
        Serial.println("- WARNING: Task **WAS** Created.");
      }
      vTaskDelay(120);
    }
    //删除任务
    //删除任务首要条件-任务存在，任务存在依据-taskHandle的值不为空NLL
    //1.删除任务，清空屏幕，将taskHandle句柄手动置空
    //2.打印提示信息
    if(digitalRead(33) == LOW)
    {
      if(taskHandle != NULL)//保证任务是存在的
      {
        vTaskDelete(taskHandle);//删除任务
        lcdClear();//清除屏幕
        taskHandle = NULL;//手动将taskHandle设置为空
      }
      if(taskHandle != NULL)//检测是否删完
      {
        Serial.print(xTaskGetTickCount());
        Serial.println("- WARNING: Unable to Delete Task.");
      }
      else
      {
        Serial.print(xTaskGetTickCount());
        Serial.println("- LOG: Task is Deleted.");
      }
      vTaskDelay(120);
    }
    //暂停任务
    //1.首要条件-任务存在，任务存在依据-taskHandle的值不为空NLL
    //2.暂停任务 vTaskSuspend
    if(digitalRead(25) == LOW)
    {
      if(taskHandle != NULL)
      {
        vTaskSuspend(taskHandle);//暂停任务
        Serial.print(xTaskGetTickCount());
        Serial.println("- LOG: Task is suspended.");
      }
      else
      {
        Serial.print(xTaskGetTickCount());
         Serial.println("- WARNING: Unable to suspended Task.");
      }
      vTaskDelay(120);
    }
    //恢复任务
    if(digitalRead(26) == LOW)
    {
      if(taskHandle != NULL)
      {
        vTaskResume(taskHandle);//恢复任务
        Serial.print(xTaskGetTickCount());
        Serial.println("- LOG: Task is resumed.");
      }
      else
      {
        Serial.print(xTaskGetTickCount());
        Serial.println("- WARNING: Unable to resumed Task.");
      }
      vTaskDelay(120);
    }
  }
}


void taskB(void *pt)
{
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);//设定光标位置
  lcd.print(" Bilibili   Channel ");
  lcd.setCursor(0, 1);
  lcd.print("-FreeRTOS on EPS32- ");
  lcd.setCursor(0, 2);
  lcd.print("Study Hard  &  Smart");
  while(1)
  {
    lcd.setCursor(9, 3);
    lcd.print(xTaskGetTickCount()/100);//屏幕加载值更新速度
    vTaskDelay(100);
  }
}
  void lcdClear()
  {
    lcd.clear();
    lcd.noBacklight();
    lcd.setCursor(0, 0);
    lcd.print(" ");
  }