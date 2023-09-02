#include <freertos/stream_buffer.h>

StreamBufferHandle_t xStreamMusic = NULL;//创建句柄
/******************以下,为获取音频，解码。。。不研究******************/
#define BUZZER_PIN 32
#define BUZZER_CHANNEL 0

 typedef struct {
  int noteVal;
  int octaveVal;
  int restVal;
} BUZZERTONE;

//随机返回生成的音乐代码，每次的长度不一样
String randomMusic() {
  String randomNote;
  randomSeed(analogRead(34));
  for (int i = 0; i < random(5, 20); i++) {
    char note[10];
    sprintf(note, "%d,%d,%d", random(0, 9), random(3, 6), random(100, 999));
    randomNote = randomNote + "-" + String(note);
  }
  randomNote = randomNote.substring(1, randomNote.length());
  return randomNote;
}

//对音乐流进行解码后，通过buzzer放出来
void decode(String music) {
  ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL);
  do {
    int index = music.indexOf('-');
    String val = music.substring(0, index);
    BUZZERTONE buzzertone;
    buzzertone.noteVal = val.substring(0, 1).toInt();
    buzzertone.octaveVal = val.substring(2, 3).toInt();
    buzzertone.restVal = val.substring(4, 7).toInt();
    ledcWriteNote(BUZZER_CHANNEL, (note_t)buzzertone.noteVal, buzzertone.octaveVal);
    vTaskDelay(buzzertone.restVal);
    if (music.indexOf('-') < 0) music = "";
    music = music.substring(index + 1, music.length());
  } while (music.length() > 0);
  ledcDetachPin(BUZZER_PIN);
}
/******************以上，为获取音频，解码。。。不研究******************/



void setup()
{
   Serial.begin(115200);
   //设置Stream Buffer最大尺寸，若果超出内存空间，那么创建Steam Buffer就会失败
   const size_t xStreamBufferSizeBytes = 540;
   const size_t xTriggerLevel = 8;//Stream Buff内部数据超过这个值，才会被读取--->相当于要传递的数据一帧有多少
  
   xStreamMusic = xStreamBufferCreate(xStreamBufferSizeBytes, xTriggerLevel);//创建Stream Buffer xStreamBufferSizeBytes-StreamBuffer大小，xTriggerLevel-一帧数据大小
  
  if(xStreamMusic == NULL)//创建失败，创建成功的话内部分配了内存，不能为NULL
  {
    Serial.println("UNABLE TO CREATE STREAM BUFFER");
  }
  else
  {
    xTaskCreate(taskA, "taskA", 1024*8, NULL, 1, NULL);//下载音乐
    xTaskCreate(taskB, "taskB", 1024*8, NULL, 1, NULL);//解码并播放
    xTaskCreate(taskC, "taskC", 1024*8, NULL, 1, NULL);
  }

  vTaskDelete(NULL);  //setup和loop这个loopback任务没用了，删除
}

void loop()
{

}


//下载音乐
void taskA(void *ptParma)
{
  String music;
  size_t xBytesSent;

  while(1)
  {
    //从网络下载音乐，放一些随机的延迟
    for (int i = 0; i < random(20, 40); i++) vTaskDelay(1);
    music = randomMusic(); //随机生成一些数据

//#######正确的是在这里打印
// Serial.println(music);


    xBytesSent = xStreamBufferSend(xStreamMusic,
                                  (void *)&music, //复制到流缓冲区的内容
                                  sizeof(music),  //复制到流缓冲区的最大字节数
                                  portMAX_DELAY); //等到流缓冲区中有足够的空间
    if(xBytesSent != sizeof(music))//确定尺寸
    {
      Serial.println("警告: xStreamBufferSend 写入数据出错");  //Optional
    }

    vTaskDelay(100);
  }

}

//解码并播放
void taskB(void *ptParma)
{
  size_t xReceiveBytes;
  size_t xReadBytes = 8*10-1;//要解码的数据大小
  String music;

  while(1)
  {
    xReceiveBytes = xStreamBufferReceive(xStreamMusic,
                                          (void *)&music,
                                          xReadBytes,
                                          portMAX_DELAY);//倘若流缓存中没有数据，或者数据小于8，那么等
    
    if(xReceiveBytes > 0)//流缓冲区中有数据
    {
      decode(music);
      // 错误，不是在这里打印而是上面taskA下载音乐，Serial.println(music);
    }
  }
}

//监视Stream Buffer
void taskC(void *ptParma)
{
  size_t xAvailable, xUsed;
  bool isFull;

  while(1)
  {

    if(xStreamBufferIsFull(xStreamMusic) == pdTRUE)  Serial.println("StreamBuffer已满！！！");
    xUsed = xStreamBufferBytesAvailable(xStreamMusic);
    xAvailable = xStreamBufferSpacesAvailable(xStreamMusic);
    char msg[40];
    sprintf(msg,"xStreamBuffer已使用 %d 字节", xUsed);
    Serial.println(msg);
    sprintf(msg,"xStreamBuffer可用空间 %d 字节", xAvailable);
    Serial.println(msg);

    vTaskDelay(2000);
  }
}