#include <Arduino.h>
#include <Wifi.h>
#include "crstrlib.h"
#include "sntp.h"

#include "nvs_flash.h"
#include "nvs.h"

#include "SaveNVS.h"

#include "sd3078.h"




//-----------网络时间获取-----------//
const char *ntpServer1 = "pool.ntp.org"; //网络时间服务器
const char *ntpServer2 = "ntp1.aliyun.com";// "ntp3.aliyun.com"
const char *ntpServer3 = "ntp2.aliyun.com";

//时区设置函数，东八区（UTC/GMT+8:00）写成8*3600
const long gmtOffset_sec = 8 * 3600;    
const int daylightOffset_sec = 0;   //夏令时填写3600，否则填0




bool Wifi_Init()
{
  int32_t Wifi_Timeout = 0;

  delay(500);
 // Serial.printf("WifiName:%s password:%s",StrWifiName,StrWifiPassword);
  WiFi.begin(StrWifiName,StrWifiPassword);
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
    Wifi_Timeout++;
    if(Wifi_Timeout >= 120)
    {break; }
  }

  if(Wifi_Timeout >= 120)
  {
    Serial.println("WiFi connected timeout, will try later");
    return false;
  }else{
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP().toString());
    return true;
  }
}

void printLocalTime()
{
    struct tm timeinfo;
    static uint8_t fgWifiStop = 1;

    if (!getLocalTime(&timeinfo))
    {
        Serial.println("Failed to obtain time");
        return;
    }
    // if(fgWifiStop == 1)
    // {
    //   fgWifiStop = 0;
    //   WiFi.disconnect(true);
    //   WiFi.mode(WIFI_OFF);
    //   Serial.println("WiFi disconnected!");
    // }

    Serial.println(&timeinfo, "%F %T %A"); // 格式化输出
}


//clock:wifiname:zhangsan
//clock:wifipassword:123456
static void GetWifiInforFromUart(char* data)
{

    int strLen = CRStrLib_strLen(data);
    int strIdx = 0;
    char nextString[100] = {0};
    int status;

    CRStrLib_findNextString(data,strLen,&strIdx,nextString);
    if(strcmp(nextString,"clock") == 0)
    {
      CRStrLib_findNextString(data,strLen,&strIdx,nextString);
      if(strcmp(nextString,NvsKeyWifiName) == 0)
      {
        if(strIdx+1 < strLen)
        {
          strcpy(StrWifiName,data + strIdx + 1);
          if(SaveWifiInfor(NvsKeyWifiName,StrWifiName) == ESP_OK)
          {
            Serial.println("Wifi Info Save SUCCESS");
          }else{
            Serial.println("Wifi Info Save FAIL"); 
          }
        }else{
          Serial.println("Wifi Name Got FAIL");
        }
      }
      else if(strcmp(nextString,NvsKeyWifiPassword) == 0)
      {
        if(strIdx+1 < strLen)
        {
          strcpy(StrWifiPassword,data + strIdx + 1);
          if(SaveWifiInfor(NvsKeyWifiPassword,StrWifiPassword) == ESP_OK)
          {
            Serial.println("Wifi Info Save SUCCESS");          
          }else{
            Serial.println("Wifi Info Save FAIL"); 
          }
        }else{
          Serial.println("Wifi Password Got FAIL"); 
        }              
      }
    }
}

#define RX_BUF_SIZE  1024
static char UartReadData[RX_BUF_SIZE] = {0};
static void rx_task(void *arg)
{
  delay(10);
  Serial.println("rx_task begain");

    while (1) 
    {
        const int rxBytes = Serial.read(UartReadData,sizeof(UartReadData));     //读取数据
        if (rxBytes > 0) 
        {                                      //判断数据长度
            UartReadData[rxBytes] = '\0';                                  
 //           Serial.printf( "%s:Read %d bytes: '%s'\n", "RX_TASK",rxBytes, UartReadData);        //log打印
            GetWifiInforFromUart(UartReadData);
        }
        delay(5);
    }
}

TaskHandle_t ClockTask;
// Callback function (get's called when time adjusts via NTP)
void timeavailable(struct timeval *t)
{
  if(ClockTask)
  {
    xTaskNotifyGive(ClockTask);
  }

  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  Serial.println("WiFi disconnected!");
}

#include "28byj48.h"
static void Wifi_task(void *arg)
{
  struct tm timeinfo;

  sntp_set_time_sync_notification_cb( timeavailable );
  Serial.println("Wifi init....");

//   delay(100);

// //  Serial.printf("Motor.GoalStep:%d\n",Motor.GoalStep);

  if(Wifi_Init())
  {
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1,ntpServer2,ntpServer3);
  } 
  delay(100);

  // while (!getLocalTime(&timeinfo))
  // {
  //   vTaskDelay(1000);
  // }

  // WiFi.disconnect(true);
  // WiFi.mode(WIFI_OFF);
  // Serial.println("WiFi disconnected!");
  while(1)
  {
      
      getLocalTime(&timeinfo);
 //   printLocalTime();
      if(timeinfo.tm_hour == 2 && timeinfo.tm_min == 30)
      {
          if(Wifi_Init())
          {
            configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1,ntpServer2,ntpServer3);
          } 
          while(timeinfo.tm_hour == 2 && timeinfo.tm_min == 30)
          {
            getLocalTime(&timeinfo);
            delay(1000);
          }
      }
    
      delay(1000);

    // Serial.printf("CaliTime: %d:%d\n",Motor.GlobalStep / STEPS_PER_HOUR, (Motor.GlobalStep % STEPS_PER_HOUR)/STEPS_PER_MINUTE);
  }
}



void setup() {
  // put your setup code here, to run once:
  extern void Clock_task(void *arg);

  Serial.begin(115200);
  delay(3000);
 // setCpuFrequencyMhz(240);
  Serial.println(getCpuFrequencyMhz());

  SysInforInit();

 xTaskCreate(rx_task, "uart_rx_task", 1024*4, NULL, configMAX_PRIORITIES-1, NULL);
 xTaskCreate(Wifi_task, "Wifi_task", 1024*8, NULL, configMAX_PRIORITIES-2, NULL);
  xTaskCreate(Clock_task, "Clock_task", 1024*8, NULL, configMAX_PRIORITIES-3, &ClockTask);
//  vTaskStartScheduler();
}

void loop() {
  // put your main code here, to run repeatedly:
  // Serial.println("idle task");
  // delay(1000);
  // Serial.println("clock loop begain");
  // StepGoForMinute(1000);
  // Serial.println("clock loop end"); 


  // Serial.printf("touch0:%d,",touchRead(T0));
  // Serial.printf("touch2:%d,",touchRead(T2));
  // Serial.printf("touch3:%d,",touchRead(T3));
  // Serial.printf("touch4:%d,",touchRead(T4));
  // Serial.printf("touch5:%d,",touchRead(T5));
  // Serial.printf("touch6:%d,",touchRead(T6));
  // Serial.printf("touch7:%d,",touchRead(T7));
  //  Serial.printf("touch8:%d,",touchRead(T8));
  // Serial.printf("touch9:%d\n",touchRead(T9));
    delay(1000);

  // vTaskDelay(5000);
  // Serial.println("Colors R, G, B, W...");

    
 



}