//所谓邮箱-将数据打包（打包方法：将数据移位进直接消息通知的uint32_t）发送xTaskNotify(xTaskNeoRing,rgb,eSetValueWithOverwrite)覆盖,接收后结包（移位）xTaskNotifyWait(0x00,0x00,&srgb,0)


#include <Adafruit_NeoPixel.h>
#define NEOPIN 33
#define NUMPIXELS 16

#define RPIN 34
#define GPIN 35
#define BPIN 32
#define SPIN 36 // speed

static TaskHandle_t xTaskNeoRing = NULL;

void configTask(void *pvParam);
void neoRing(void *pvParam);



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  xTaskCreate(configTask, "Configration", 1024*10, NULL, 1, NULL);
  xTaskCreate(neoRing, "Neo Ring", 1024*20, NULL, 1, &xTaskNeoRing);

  vTaskDelete(NULL);//删除任务
}

void loop() {  // put your main code here, to run repeatedly:

}


void configTask(void *pvParam)
{
  pinMode(RPIN, INPUT);
  pinMode(GPIN, INPUT);
  pinMode(BPIN, INPUT);
  pinMode(SPIN, INPUT);

  uint8_t r_value = 0;
  uint8_t g_value = 0;
  uint8_t b_value = 0;
  uint8_t s_value = 100;
  while(1)
  {
    r_value = map(analogRead(RPIN), 0, 4095, 0, 255);
    g_value = map(analogRead(GPIN), 0, 4095, 0, 255);
    b_value = map(analogRead(BPIN), 0, 4095, 0, 255);
    s_value = map(analogRead(SPIN), 0, 4095, 10, 200);

    uint32_t rgb = (s_value <<24) + (b_value << 16) + (g_value << 8) + (r_value << 0);//打包

    xTaskNotify(xTaskNeoRing, rgb, eSetValueWithOverwrite);//覆盖

    vTaskDelay(100);
  }
}
void neoRing(void *pvParam)
{
  Adafruit_NeoPixel pixels(NUMPIXELS, NEOPIN, NEO_GRB + NEO_KHZ800);
  pixels.begin();

  uint32_t srgb = 0;
  uint8_t sr = 0;
  uint8_t sg = 0;
  uint8_t sb = 0;
  uint8_t ss = 100;

  while(1)
  {
    pixels.clear();
    for(int i = 0;i < NUMPIXELS; i++)
    {
      xTaskNotifyWait(0x00, 0x00, &srgb, 0);
      ss = (srgb & 0xff000000) >> 24;//解包
      sg = (srgb & 0x00ff0000) >> 16;
      sb = (srgb & 0x0000ff00) >> 8;
      sr = (srgb & 0x000000ff) >> 0;

      pixels.setPixelColor(i, pixels.Color(sr, sg, sb));
      pixels.show();

      vTaskDelay(ss*5);
    }
  }
}