#include "DHTesp.h"
#include <LiquidCrystal_I2C.h>
#define DHT22_ID 0
#define LDR_ID 1

LiquidCrystal_I2C lcd(0x27, 20, 4);
int taskMem = 1024;


//定义结构体
typedef struct{
  byte deviceID;
  float value1;
  float value2;
}SENSOR;


//此处参数传递错误 QueueHandle_t queueMsq = xQueueCreate(8, sizeof(char[20]));//创建队列，长度8，尺寸sizeof(char[20])////对于lcd一行能够显示20个字符，存了8行。但是实际上每行20个字符，4行是极限
QueueHandle_t queueMsq = xQueueCreate(8, sizeof(SENSOR));//创建队列，长度8，尺寸sizeof(char[20])////对于lcd一行能够显示20个字符，存了8行。但是实际上每行20个字符，4行是极限



void dht22(void *ptParam)
{
  const byte dhtPin = 32;
  DHTesp dhtSensor;
  dhtSensor.setup(dhtPin, DHTesp::DHT22);


  SENSOR dht22Sensor;//声明结构体
  dht22Sensor.deviceID =  DHT22_ID;//初始化结构体

  while(1)
  {
    TempAndHumidity  data = dhtSensor.getTempAndHumidity();

    dht22Sensor.value1 = data.temperature;
    dht22Sensor.value2 = data.humidity;

    TickType_t timeOut = 2000;
    //错误 if(xQueueSend(queueMsq, &htSensor.deviceID, timeOut) == pdPASS)
    if(xQueueSend(queueMsq, &dht22Sensor, timeOut) != pdPASS)
    {
      Serial.print("dht22: xQueue is full.");
    }

    vTaskDelay(1000);
  }
}



void ldr(void *ptParam) 
{
  const float GAMMA = 0.7;
  const float RL10 = 50;
  const byte ldrPIN = 27;
  pinMode(ldrPIN, INPUT);


  SENSOR ldrSensor;//声明结构体
  ldrSensor.deviceID = LDR_ID;

  while(1)
  {
    int analogValue = analogRead(ldrPIN);

    float voltage = analogValue / 4095. * 5;
    float resistance = 2000 * voltage / (1 - voltage / 5);
    float lux = pow(RL10 * 1e3 * pow(10, GAMMA) / resistance, (1 / GAMMA));

    ldrSensor.value1 = lux;
    ldrSensor.value2 = 0.0;

    TickType_t timeOut = 2000;
    if(xQueueSend(queueMsq, &ldrSensor, timeOut) != pdPASS)
    {
      Serial.print("ldr: xQueue is full.");
    }

    vTaskDelay(1000);
  }
}

//LCD主体任务，因为LCD是接收，所以xQueueReceive
void lcdTask(void *ptParam)
{
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("   LONELY  BINARY  ");

  SENSOR data;//声明结构体

  while(1)
  {
    TickType_t timeOut = 2000;
    //此处写错了if(xQueueReceive(queueMsq, &data, timeOut) != pdPASS)
    if(xQueueReceive(queueMsq, &data, timeOut) == pdPASS)//当timeOut内不为空时才，显示
    {
      switch(data.deviceID)
      //错的switch(data)
      {
        case DHT22_ID:
          lcd.setCursor(0, 1);
          lcd.print("Temp: " + String(data.value1, 2) + "c");
          lcd.setCursor(0, 2);
          lcd.print("Humidity: " + String(data.value2, 1) + "%");
          break;
        case LDR_ID:
          lcd.setCursor(0, 3);
          if (data.value1 > 50) {
            lcd.print("Bright ");
          } else {
            lcd.print("Dark ");
          }
          //lcd.setCursor(0, 3);
          lcd.print(String(data.value1, 2) + " lux");
          break;
        default:
          Serial.println("LCD: Unkown Device");
          break;
      }
    }
    else {
      Serial.println("LCD: Message Queue is Empty");
    };

    vTaskDelay(2000);
  }
}




void setup() {
  Serial.begin(9600);

  xTaskCreate(dht22,  "dht22",  taskMem*4,NULL,1,NULL);
  xTaskCreate(ldr,    "ldr",    taskMem*4,NULL,1,NULL);
  xTaskCreate(lcdTask,"lcdTask",taskMem*8,NULL,1,NULL);
}
void loop() {
  // put your main code here, to run repeatedly:
}
