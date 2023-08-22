#define led1 10
#define led2 11

//很明显，这样不能是的led1亮1s,led2亮2s。因为代码是一行行运行的，led1亮了4秒
void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  // put your setup code here, to run once:

}

void loop() {
  digitalWrite(led1,!digitalRead(led1));
  delay(1000);
  digitalWrite(led2,!digitalRead(led2));
  delay(3000);
  
  // put your main code here, to run repeatedly:
}
