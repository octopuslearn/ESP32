//学事件组是为了看懂别人写的代码，从10.0开始有更好的东西


//sync 1.set-1 2.wait-1



#define ADDTOCART_0 (1<<0)
#define PAYMENT_1   (1<<1)
#define INVENTORY_2 (1<<2)

#define ALLBITS 0xffffff  //24bits都是1

//错误#define BOUGHT_PAID_SENT(ADDTOCART_0 | PAYMENT_1 | NVENTORY_2)  //000111
#define BOUGHT_PAID_SENT  (ADDTOCART_0 | PAYMENT_1 | INVENTORY_2)  //000111

EventGroupHandle_t xEventPurchase = NULL;//创建事件组
const TickType_t TimeOut = 1000;


void button(void *ptParma);
void consumerA(void *ptParma);//添加进购物车
void consumerB(void *ptParma);//支付
void consumerC(void *ptParma);//出货


void button(void *ptParma)
{
  pinMode(22, INPUT_PULLUP);
  while(1)
  {
    if(digitalRead(22) == LOW)//消费者-1返回pdPASS,或者pdFAIL
    {
      xEventGroupClearBits(xEventPurchase, ALLBITS);
      Serial.println("用户真心决定下单了...");
    
      //放一些随机的延迟，否则运行的太快了，看不出效果
      for (int i = 0; i < random(100, 200); i++) vTaskDelay(10);
      Serial.println("商品已经添加到了购物车，付款中...");

      //添加进购物车
      xTaskCreate(consumerA,  "consumer a",   1024*6, NULL, 1, NULL);
      //支付
      xTaskCreate(consumerB,  "consumer b",   1024*6, NULL, 1, NULL);
      //出货
      xTaskCreate(consumerC,  "consumer c",   1024*6, NULL, 1, NULL);
    
    vTaskDelay(100);
    }
  }
}


//添加进购物车
void consumerA(void *ptParma)
{
  EventBits_t uxBits;//用于存放事件组 24bits 的值 EventBits_t-uin16_t或者uin32_t
  
  while(1)
  {
    uxBits = xEventGroupSync(xEventPurchase,  //Event Group Handler
                            ADDTOCART_0,      //将ADDTOCART_0设置为1
                            BOUGHT_PAID_SENT, //等待BOUGHT_PAID_SENT为1
                            TimeOut);
    if((uxBits & BOUGHT_PAID_SENT) == BOUGHT_PAID_SENT)
    {
      Serial.println("purchaseTask,已经自我了断. ");
      vTaskDelete(NULL);
    }
  }
}

//支付
void consumerB(void *ptParma)//完成添加到购物车后才能支付
{
  EventBits_t uxBits;
  while(1)
  {
    //随机延迟, 模拟付款验证过程
    for (int i = 0; i < random(100, 200); i++) vTaskDelay(10);
    Serial.println("支付宝付款完成,可以出货...");
    //理论上应该是一样的
    // uxBits = xEventGroupSync(xEventPurchase,//Event Group Hanler-事件组句柄
    //                         PAYMENT_1,    //将PAYMENT_1置1
    //                         ADDTOCART_0,  //等待ADDTOCART_0置1
    //                         TimeOut);

    // if(uxBits & PAYMENT_1)
    // {


    uxBits = xEventGroupSync (xEventPurchase,  //Event Group Handler
                             PAYMENT_1,     // 先将这个bit(s)设置为 1,然后再等待
                             BOUGHT_PAID_SENT,  //等待这些bits为 1
                             TimeOut);
    if ((uxBits & BOUGHT_PAID_SENT) == BOUGHT_PAID_SENT){
      Serial.println("purchaseTask,已经自我了断. ");
      vTaskDelete(NULL);//删除此任务//完成当前任务后将任务删除
    }
  }
}

//出货
void consumerC(void *ptParma)
{
  
  EventBits_t uxBits;

  while(1)
  {
    for (int i = 0; i < random(100, 200); i++) vTaskDelay(10);
    Serial.println("仓库出货完成,快递已取货...");
    // uxBits = xEventGroupSync(xEventPurchase,
    //                         INVENTORY_2,
    //                         ADDTOCART_0|PAYMENT_1,
    //                         TimeOut);
    // if((uxBits & ADDTOCART_0) && (uxBits & PAYMENT_1))//***注意这里
    // //错误 if(uxBits & (ADDTOCART_0|PAYMENT_1))//消费者-1返回pdPASS,或者pdFAIL
    // {


        uxBits = xEventGroupSync (xEventPurchase,  //Event Group Handler
                              INVENTORY_2,     // 先将这个bit(s)设置为 1,然后再等待
                              BOUGHT_PAID_SENT,  //等待这些bits为 1
                              TimeOut);
    if ((uxBits & BOUGHT_PAID_SENT) == BOUGHT_PAID_SENT)  {
      Serial.println("purchaseTask,已经自我了断. ");
      vTaskDelete(NULL);
    }
  }
}




void setup() {
  Serial.begin(9600);
  
  xEventPurchase = xEventGroupCreate();//创建 event group-事件组

  xTaskCreate(button,"button",   1024*6, NULL,               1, NULL);

}
void loop() {
  // put your main code here, to run repeatedly:
}
