static TaskHandle_t xledTask = NULL;//到底啥时候是静态的
void dialTask(void * pvParam);
void ledTask(void * pvParam);



#define RESET_0 0b1
#define LEDBIT_1 0b10
#define LEDBIT_2 0b100
#define LEDBIT_3 0b1000
#define LEDBIT_4 0b10000
#define LEDBIT_5 0b100000
#define LEDBIT_6 0b1000000
#define LEDBIT_7 0b10000000
#define LEDBIT_8 0b100000000
#define LEDBIT_9 0b1000000000






void dialTask(void * pvParam) {

  const byte INDIALPIN = 33;
  const byte PULSEPIN = 32;
  pinMode(INDIALPIN, INPUT_PULLUP);
  pinMode(PULSEPIN, INPUT_PULLUP);

  byte counter = 0;
  boolean inDialPinLastState;
  boolean pulsPinLastState;

  inDialPinLastState = digitalRead(INDIALPIN);
  pulsPinLastState = digitalRead(PULSEPIN);

  while (1) {

    boolean inDialPinState = digitalRead(INDIALPIN);
    boolean pulsPinState = digitalRead(PULSEPIN);
    if (inDialPinState != inDialPinLastState) {
      if (!inDialPinState) {
        counter = 0;
      } else {
        if (counter) {
          counter = counter % 10;
          // ---- 横线上方Dial的判断代码为抄的 ----
          // Serial.println(counter);
          uint32_t ulEventGroup;
          
          switch(counter)
          {
            case 0:ulEventGroup = RESET_0;break;
            case 1: ulEventGroup = LEDBIT_1 ; break;
            case 2: ulEventGroup = LEDBIT_2 ; break;
            case 3: ulEventGroup = LEDBIT_3 ; break;
            case 4: ulEventGroup = LEDBIT_4 ; break;
            case 5: ulEventGroup = LEDBIT_5 ; break;
            case 6: ulEventGroup = LEDBIT_6 ; break;
            case 7: ulEventGroup = LEDBIT_7 ; break;
            case 8: ulEventGroup = LEDBIT_8 ; break;
            case 9: ulEventGroup = LEDBIT_9 ; break;
            default: ulEventGroup = RESET_0 ; break;
          }

          xTaskNotify(xledTask, ulEventGroup, eSetBits);//相当于xEventGroupSetBits(xledTask,a);

                  // ---- 横线下方Dial的判断代码为抄的 ----
        }
      }
      inDialPinLastState = inDialPinState;
    }
    if (pulsPinLastState != pulsPinState) {
      if (!pulsPinLastState) {
        counter++;
      }
      pulsPinLastState = pulsPinState;
    }
  }
}




void ledTask(void * pvParam) {
  byte led_pins[9] = {23,22,21,19,18,5,15,16,4};
  for(byte pin:led_pins) pinMode(pin, OUTPUT);

  uint32_t ulNotificationValue;
  while(1)
  {
    xTaskNotifyWait(pdFALSE, 
                    ULONG_MAX,
                    &ulNotificationValue,
                    portMAX_DELAY);
    

    if(ulNotificationValue &(1<<0) == 1)//第0位是1-RESET
      for(int i=0;i<=9;i++)
        digitalWrite(led_pins[i-1],LOW);
    
    for(int i=1;i<=9;i++)//否则查询所有的，置1就点亮
    {
      if(ulNotificationValue & (1<<i))
      {
        digitalWrite(led_pins[i-1],HIGH);
      }
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  xTaskCreate(dialTask, "Dial panel", 1024*10, NULL, 1, NULL);
  xTaskCreate(ledTask, "Nine LED", 1024*10,NULL, 1, &xledTask);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(10); // this speeds up the simulation
}
