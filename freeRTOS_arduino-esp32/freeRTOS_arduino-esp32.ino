#include <LiquidCrystal_I2C.h>
#include <freertos/message_buffer.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);


MessageBufferHandle_t xMessageBuffer = NULL;//创建对象



/**********************以下，不重要，不研究***********************/
//随机发送不同长度的信息
String randomGPS() {
  char gps[30];

  static int counter = 100;
  counter++;
  switch (random(0, 3)) {
    case 0:
      // 返回 经度 纬度 海拔
      sprintf(gps, "%d-%d-%d-%d", counter, random(100, 999), random(100, 999), random(100, 999));
      break;
    case 1:
      // 返回 经度 纬度
      sprintf(gps, "%d-%d-%d", counter, random(100, 999), random(100, 999));
      break;
    case 2:
      // 返回 海拔
      sprintf(gps, "%d-%d", counter, random(100, 999));
      break;
    default:
      break;
  }

  return String(gps);
}



//以下屎山代码是对不同长度的GPS信息进行解码，然后显示到LCD上
void gpsDecoder(String gpsinfo) {
  struct GPS {
    int counter;
    int longVal;
    int latVal;
    int AltVal;
  };

  String s1, s2, s3, s4;
  int counter = 0;
  String gpsinfo2 = gpsinfo;
  do {
    counter++;
    int index = gpsinfo.indexOf('-');
    if (gpsinfo.indexOf('-') < 0) gpsinfo = "";
    gpsinfo = gpsinfo.substring(index + 1, gpsinfo.length());
  } while (gpsinfo.length() > 0);

  GPS gps;

  switch (counter) {
    case 2:
      gps.counter = gpsinfo2.substring(0, 3).toInt();
      gps.longVal = -1;
      gps.latVal = -1;
      gps.AltVal = gpsinfo2.substring(4, 7).toInt();
      break;
    case 3:
      gps.counter = gpsinfo2.substring(0, 3).toInt();
      gps.longVal = gpsinfo2.substring(4, 7).toInt();
      gps.latVal = gpsinfo2.substring(8, 11).toInt();
      gps.AltVal = -1;
      break;
    case 4:
      gps.counter = gpsinfo2.substring(0, 3).toInt();
      gps.longVal = gpsinfo2.substring(4, 7).toInt();
      gps.latVal = gpsinfo2.substring(8, 11).toInt();
      gps.AltVal = gpsinfo2.substring(12, 15).toInt();
      break;
    default:
      break;
  }

  //return gps;
  // Serial.println(gps.counter);
  // Serial.println(gps.longVal);
  // Serial.println(gps.latVal);
  // Serial.println(gps.AltVal);
  // Serial.println();

  lcd.setCursor(13, 0);
  lcd.print(gps.counter);

  lcd.setCursor(0, 1);
  lcd.print("     LONG : ");
  if (gps.longVal == -1) {
    lcd.print("N/C");
  } else {
    lcd.print(gps.longVal);
  }

  lcd.setCursor(0, 2);
  lcd.print("      LAT : ");
  if (gps.latVal == -1) {
    lcd.print("N/C");
  } else {
    lcd.print(gps.latVal);
  }

  lcd.setCursor(0, 3);
  lcd.print("      ALT : ");
  if (gps.AltVal == -1) {
    lcd.print("N/C");
  } else {
    lcd.print(gps.AltVal);
    //Serial.println(gpsinfo2);
    //Serial.println(gps.AltVal);
  }

}
/**********************以上，不重要，不研究***********************/





void readGPS(void *ptParam)
{
  size_t xBytesSent;
  String gpsInfo;
  while(1)
  {
    gpsInfo = randomGPS();//随机发送不同长度的信息
    xBytesSent = xMessageBufferSend(xMessageBuffer,
                                    (void *)&gpsInfo
                                    sizeof(gpsInfo),
                                    portMAX_DELAY);
    if(xBytesSent != sizeof(gpsInfo))
    {
      Serial.println("危险: xMessageBufferSend 发送数据不完整");
    }
    else 
    {
      vTaskDelay(3000);
    }
  }
}


void showGPS(void *ptParam)
{
  size_t xReceivedBytes;
  String gpsInfo;
  const size_t xMessageSizeMax = 100;

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("   GPS INFO"); //clear this line

  while(1)
  {
    xReceivedBytes = xMessageBufferReceive(xMessageBuffer,
                                           (void *)&gpsInfo,
                                           xMessageSizeMax,//######最大数据
                                           portMAX_DELAY);
    if(xReceivedBytes > 0)
    {
      gpsDecoder(gpsInfo);
    }

    vTaskDelay(1000);
  }

}
void monitorTask(void *ptParam)
{
  size_t xAvailable, xUsed;
  bool isFull;
  while(1)
  {
    if(xMessageBufferIsFull(xMessageBuffer) == pdTURE) Serial.println("xMessageBuffer 已满");
    xAvailable = xMessageBufferSpacesAvailable(xMessageBuffer);
    char mas[40];
    sprintf(msg, "xMessageBuffer可用空间为 %d 字节", xAvailable);
    Serial.println(msg);

    vTaskDelay(1000);
  }
}

void setup()
{
  Serial.begin(115200);
  const size_t xMessageBufferSizeBytes = 100;//收发数据大小
  xMessageBuffer = xMessageBufferCreate(xMessageBufferSizeBytes);//创建MessageBuffer信息缓冲区

  if(xMessageBuffer == NULL)
  {
    Serial.println("Unable to Create Message Buffer");
  }
  else 
  {
    xTaskCreate(readGPS, "Read GPX", 1024 * 4, NULL, 1, NULL);
    xTaskCreate(showGPS, "Show GPX", 1024 * 4, NULL, 1, NULL);
    xTaskCreate(monitorTask, "Monitor Message Buffer", 1024 * 8, NULL, 1, NULL); //对Stream Buffer进行监控
  }
}

void loop()
{
  
}