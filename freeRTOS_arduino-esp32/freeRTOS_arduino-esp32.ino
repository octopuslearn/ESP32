/********以下，删除任务之理由*******/
//添加进购物车
// void consumerA(void *ptParma)
// {
//   EventBits_t uxBits;//用于存放事件组 24bits 的值 EventBits_t-uin16_t或者uin32_t
  
//   while(1)
//   {
//     uxBits = xEventGroupSetBits(xEventPurchase, ADDTOCART_0);//将ADDTOCART_0设置为1
//     if(uxBits & ADDTOCART_0)//判断是否设置完成？？？
//     {
//       Serial.println("商品已经添加到了购物车，付款中...");
//       Serial.print("   Event Group Value:");
//       Serial.println(uxBits ,BIN);//以二进制格式打印
//     }
//     vTaskDelete(NULL);//删除任务理由：不删除的话，会一直执行这个任务
//   }
// }
/********以上，删除任务之理由*******/



#define ADDTOCART_0 (1<<0)
#define PAYMENT_1   (1<<1)
#define INVENTORY_2 (1<<2)

#define ALLBITS 0xffffff  //24bits都是1

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
      
      Serial.println("用户真心决定下单了...");

      //放一些随机的延迟，否则运行的太快了，看不出效果      
      for (int i = 0; i < random(100, 200); i++) vTaskDelay(10);
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
    uxBits = xEventGroupSetBits(xEventPurchase, ADDTOCART_0);//将ADDTOCART_0设置为1
    if(uxBits & ADDTOCART_0)//判断是否设置完成？？？
    {
      Serial.println("商品已经添加到了购物车，付款中...");
      Serial.print("   Event Group Value:");
      Serial.println(uxBits ,BIN);//以二进制格式打印
    }

    //////最终------------------------------------------------
    uxBits = xEventGroupWaitBits(xEventPurchase,//Event Group Hanler-事件组句柄
                                ADDTOCART_0|PAYMENT_1|INVENTORY_2,    //等待事件组中的那个bit置1，即ADDTOCART_0 = 1
                                pdFALSE,        //执行后，对应的的bits是否重置为0
                                pdTRUE,         //等待的bit的关系，True-&，Fall-|
                                TimeOut);    
    if((uxBits & ADDTOCART_0)&&(uxBits & PAYMENT_1)&&(uxBits & INVENTORY_2))
    {
      for (int i = 0; i < random(100, 200); i++) vTaskDelay(10);

      xEventGroupClearBits(xEventPurchase, ALLBITS);//重置24bits
      uxBits = xEventGroupGetBits(xEventPurchase);
      Serial.println("交易完成, RESET Event Group");
      Serial.print("   Event Group Value:");
      Serial.println(uxBits, BIN);
      Serial.println("");
    }

    vTaskDelete(NULL);
  }
}

//支付
void consumerB(void *ptParma)//完成添加到购物车后才能支付
{
  EventBits_t uxBits;
  while(1)
  {
    //###问：问什么等待完成后，还要再判断以一次相应的位是否置1，岂不是多此一举？？？
    //等待事件组中相应的位置1，即等待添加到购物车完成
    uxBits = xEventGroupWaitBits(xEventPurchase,//Event Group Hanler-事件组句柄
                                ADDTOCART_0,    //等待事件组中的那个bit置1，即ADDTOCART_0 = 1
                                pdFALSE,        //执行后，对应的的bits是否重置为0
                                pdTRUE,         //等待的bit的关系，True-&，Fall-|
                                TimeOut);

    if(uxBits & ADDTOCART_0)
    {
      //随机延迟, 模拟付款验证过程
      for (int i = 0; i < random(200, 400); i++) vTaskDelay(10);
      uxBits = xEventGroupSetBits(xEventPurchase, PAYMENT_1);//当前任务完成后在事件组中将相应的位置1，即将bit1 PAYMENT_1设置为1
      
      Serial.println("支付宝付款完成,可以出货...");
      Serial.print("   Event Group Value:");
      Serial.println(uxBits, BIN);

      //###问：为什么要吧任务删除呢,之前的代码中几乎没有过用完后就删除
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
    uxBits = xEventGroupWaitBits(xEventPurchase,
                                ADDTOCART_0|PAYMENT_1,
                                pdFALSE,
                                pdTRUE,
                                TimeOut);
    if((uxBits & ADDTOCART_0) && (uxBits & PAYMENT_1))//***注意这里
    //错误 if(uxBits & (ADDTOCART_0|PAYMENT_1))//消费者-1返回pdPASS,或者pdFAIL
    {
      for (int i = 0; i < random(100, 200); i++) vTaskDelay(10);
      uxBits = xEventGroupSetBits(xEventPurchase, INVENTORY_2);//将INVENTORY_2置1
      
      Serial.println("仓库出货完成,快递已取货...");
      Serial.print("   Event Group Value:");
      Serial.println(uxBits, BIN);

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
