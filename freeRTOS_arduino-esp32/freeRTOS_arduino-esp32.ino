void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello, ESP32-S3!");
  int heapSize = ESP.getHeapSize();//获取堆大小
  Serial.print("heapSize: ");Serial.println(heapSize);

  int heapFree = ESP.getFreeHeap();//获取空闲堆
  Serial.print("heapFree: ");Serial.println(heapFree);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(10); // this speeds up the simulation
}
