TaskHandle_t taskHandle;//创建句柄
int taskMem = 1024;


void tastA(void *ptParam);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello, ESP32-S3!");
  int heapSize = ESP.getHeapSize();//获取堆大小
  Serial.print("heapSize: ");Serial.println(heapSize);

  int heapFree = ESP.getFreeHeap();//获取空闲堆
  Serial.print("heapFree: ");Serial.println(heapFree);
  xTaskCreate(taskA,//创建完任务后，就给个这个任务一桶水，水就是内存
            "get now freeHeap",
            1024,
            NULL,
            1,
            &taskHandle);
  Serial.print("now_heapFree: ");Serial.println(ESP.getFreeHeap());
  //375744-374344=1400创建任务占用的的总字节
  //1400-1024=376//创建任务额外占用376字节


  //对于任务本身空余的字节
  //水位线
  int waterMark = uxTaskGetStackHighWaterMark(taskHandle);//最高水位，即当前剩余最多的水
  Serial.print("Task Free Memory: ");Serial.println(waterMark);
  Serial.print("Task Used Memory: ");Serial.println(taskMem - waterMark);//桶的尺寸-当前水位=使用的水
//Task Free Memory: 7080
//Task Used Memory: -6056//使用了负的6056个字节？？？
//而且桶的尺寸int taskMem = 1024;为啥超了尺寸Task Free Memory: 7080
//理由是xTaskCreate少传递了参数
// 错误的为
//   xTaskCreate(taskA,//创建完任务后，就给个这个任务一桶水，水就是内存
//             "get now freeHeap",
//             1024,
//             NULL,
//             1,
//             NULL);
// 正确的为
// xTaskCreate(taskA,//创建完任务后，就给个这个任务一桶水，水就是内存
// "get now freeHeap",
// 1024,
// NULL,
// 1,
// &taskHandle);


//Task Free Memory: 392
//Task Used Memory: 632//原则是设定为使用的两倍632*2=1264
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(10); // this speeds up the simulation
}


void taskA(void *ptParam)
{
  //volatile char hello[1000]={0};//esp32s3这里不行，但是esp32可以
  while(1)
  {

  }
}