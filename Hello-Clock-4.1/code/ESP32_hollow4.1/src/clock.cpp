#include <Arduino.h>
#include "28BYJ48.h"
#include "sd3078.h"
#include "button.h"
#include "bh1750.h"




#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>
 
#define PIN_LEDS 22
#define NUMPIXELS 60
 
const uint16_t AnimCount = 1; // we only need one
const uint16_t TailLength = 10; // length of the tail, must be shorter than PixelCount
const float MaxLightness = 0.5f; // max lightness at the head of the tail (0.5f is full bright)
const float Minlightness = 0.07f;
int32_t lightnessCnt = 0;
float lightness = Minlightness;
float Doublelightness = 0.0f;
float hue = 60/360.0f; //yellow
NeoGamma<NeoGammaTableMethod> colorGamma; // for any fade animations, best to correct gamma

NeoPixelBus<NeoGrbFeature, NeoWs2812xMethod> strip(NUMPIXELS, PIN_LEDS);
// for esp8266 omit the pin
//NeoPixelBus<NeoGrbFeature, NeoWs2812xMethod> strip(PixelCount);

NeoPixelAnimator animations(AnimCount); // NeoPixel animation management object


static hw_timer_t * timer = NULL; 
#define TouchRight      T8
#define TouchLeft       T9

#define BtnRightDown    (touchRead(TouchRight) < thresholdRight)
#define BtnLeftDown     (touchRead(TouchLeft) < thresholdLeft)

int32_t thresholdRight  = 0;
int32_t thresholdLeft   = 0;
float Angle = 60;


uint8_t ReadTouchRight()
{
    if(BtnRightDown) {return 1;}else{return 0;}
}
uint8_t ReadTouchLeft()
{
    if(BtnLeftDown) {return 1;}else{return 0;}
}

Button_t BtnRight;
Button_t BtnLeft;

void BtnRight_Dowm_CallBack(void *btn)
{
  Angle += 20;
  if(Angle >= 360)
  {
    Angle = 0;
  }
  hue = Angle / 360.0f;
}
void BtnRight_Long_CallBack(void *btn)
{
//  Serial.printf("BtnRight_Long_CallBack :%d\n",thresholdRight);
  Angle += 0.1;
  if(Angle >= 360)
  {
    Angle = 0;
  }
  hue = Angle / 360.0f;
}
void BtnLeft_Dowm_CallBack(void *btn)
{
  Angle -= 20;
  if(Angle <= 0)
  {
    Angle = 360;
  }
  hue = Angle / 360.0f;
    // lightnessCnt++;
    // if(lightnessCnt > 10)
    // {
    //   lightnessCnt = 0;
    // }

    // lightness = Minlightness * pow(MaxLightness / Minlightness, lightnessCnt / 10.0f);
 //     Doublelightness = Minlightness * pow(MaxLightness / Minlightness, (lightnessCnt+1) / 10.0f);
    
}
void BtnLeft_Long_CallBack(void *btn)
{
//  Serial.printf("BtnLeft_Long_CallBack :%d\n",thresholdLeft);
  Angle -= 0.1;
  if(Angle <= 0)
  {
    Angle = 360;
  }
  hue = Angle / 360.0f;
}

void IRAM_ATTR BtnTimer()  // 中断函数
{
    Button_Process();
}

void ButtonInit() 
{
  Button_Create("BtnRight",&BtnRight, ReadTouchRight,1);
  Button_Attach(&BtnRight,BUTTON_DOWM,BtnRight_Dowm_CallBack);  //单击
  Button_Attach(&BtnRight,BUTTON_LONG,BtnRight_Long_CallBack);  //长按

  Button_Create("BtnLeft",&BtnLeft, ReadTouchLeft,1);
  Button_Attach(&BtnLeft,BUTTON_DOWM,BtnLeft_Dowm_CallBack);  //单击
  Button_Attach(&BtnLeft,BUTTON_LONG,BtnLeft_Long_CallBack);  //长按

  

  delay(300);
  for(uint32_t i = 0; i< 64; i++)
  {
      thresholdRight += touchRead(TouchRight);
      thresholdLeft += touchRead(TouchLeft);
  }
  Serial.printf("BtnLeft :%d\n",thresholdLeft);
  Serial.printf("BtnRight :%d\n",thresholdRight);
  thresholdRight >>= 6;
  thresholdLeft >>= 6;
  thresholdRight -= 15;
  thresholdLeft -= 15;

  timer = timerBegin(1, 80, true);
  timerAlarmWrite(timer, 20000, true); 
	timerAttachInterrupt(timer, &BtnTimer, true);
	timerAlarmEnable(timer); // 启用定时器

}






void SetRandomSeed()
{
    uint32_t seed;

    // random works best with a seed that can use 31 bits
    // analogRead on a unconnected pin tends toward less than four bits
    seed = analogRead(14);
    delay(1);

    for (int shifts = 3; shifts < 31; shifts += 3)
    {
        seed ^= analogRead(14) << shifts;
        delay(1);
    }

    // Serial.println(seed);
    randomSeed(seed);
}

void DrawPixelsInit()
{
    // using Hsl as it makes it easy to pick from similiar saturated colors
//    hue = random(360) / 360.0f;

    // RgbColor color = HslColor(hue, 1.0f, 1.0f);
    // strip.SetPixelColor(0, colorGamma.Correct(color));

    for (uint16_t index = 1; index < strip.PixelCount(); index++)
    {
        RgbColor color = HslColor(hue, 1.0f, Minlightness);
        if(! ((index+1) % 5) )//整点
        strip.SetPixelColor(index, colorGamma.Correct(color));

    }
}

void DrawSecondPixelsRotate(SD3078_t* SD3078)
{
//    struct tm timeinfo;
    
    int32_t Seconds = SD3078->second;
    //  if(getLocalTime(&timeinfo,5))
    //  {
    //      Seconds = timeinfo.tm_sec;
    //      Serial.printf("getLocalTime:%d\n",Seconds);
    //  }else{
    //    Read_SD3078(&SD3078);
        // Seconds = ;
        // Serial.printf("Read_SD3078:%02d:%02d:%02d\n",SD3078.hour,SD3078.minute,SD3078.second);
        // Serial.printf("CaliTime: %d:%d\n",(GetMotorGlobalStep()) / STEPS_PER_HOUR, (GetMotorGlobalStep() % STEPS_PER_HOUR)/STEPS_PER_MINUTE);
    //  }

//    static uint8_t HighLightIndex = 0;

    RgbColor color = 0;
     for (uint16_t index = 0; index < strip.PixelCount(); index++)
    {
        if(! ((index+1) % 5) )//整点
        {
            color = HslColor(hue, 1.0f, lightness);
            strip.SetPixelColor(index, colorGamma.Correct(color));
            //strip.SetPixelColor(index, color);
        }else{
            color = HslColor(hue, 0.0f, lightness);
            strip.SetPixelColor(index, colorGamma.Correct(color));
            //strip.SetPixelColor(index, color);
        }     
    }
    color = HslColor(hue, 0.0f, lightness*2);
    strip.SetPixelColor((Seconds + 29) % 60, colorGamma.Correct(color));
    strip.Show();
}

void RightLoopAnimUpdate(const AnimationParam& param)
{
    // wait for this animation to complete,
    // we are using it as a timer of sorts
    if (param.state == AnimationState_Completed)
    {
        // done, time to restart this position tracking animation/timer
        animations.RestartAnimation(param.index);

        // rotate the complete strip one pixel to the right on every update
        strip.RotateRight(1);
    }
}
void LeftLoopAnimUpdate(const AnimationParam& param)
{
    // wait for this animation to complete,
    // we are using it as a timer of sorts
    if (param.state == AnimationState_Completed)
    {
        // done, time to restart this position tracking animation/timer
        animations.RestartAnimation(param.index);

        // rotate the complete strip one pixel to the right on every update
        strip.RotateLeft(1);
    }
}

// void RightDrawTailPixels()
// {
//     // using Hsl as it makes it easy to pick from similiar saturated colors
// //    float hue = random(360) / 360.0f;
//     for (uint16_t index = 0; index < strip.PixelCount() && index <= TailLength; index++)
//     {
//         float _lightness = index * MaxLightness / TailLength;
//         RgbColor color = HslColor(hue, 1.0f, _lightness);

//         strip.SetPixelColor(index, colorGamma.Correct(color));
//     }
// }

// void LeftDrawTailPixels()
// {
//     // using Hsl as it makes it easy to pick from similiar saturated colors
// //    float hue = random(360) / 360.0f;
//     for (uint16_t index = 0; index < strip.PixelCount() && index <= TailLength; index++)
//     {
//         float _lightness = index * MaxLightness / TailLength;
//         RgbColor color = HslColor(hue, 1.0f, MaxLightness - _lightness);

//         strip.SetPixelColor(index, colorGamma.Correct(color));
//     }
// }

void ClearPixels()
{
  for (uint16_t index = 0; index < strip.PixelCount(); index++)
  {
    strip.SetPixelColor(index, 0);
  }
}

void RandomPixels()
{
  static uint16_t angle = 0;
  angle+=5;
  if(angle >= 360)
  {
      angle = 0;
  }
  for (uint16_t index = 0; index < strip.PixelCount(); index++)
  {
    float _hue = angle / 360.0f;
    RgbColor color = HslColor(_hue, 1.0f, 1.0f);
    strip.SetPixelColor(index, colorGamma.Correct(color));
  } 
}
void RandomPixelsUpdate(const AnimationParam& param)
{
    // wait for this animation to complete,
    // we are using it as a timer of sorts
    if (param.state == AnimationState_Completed)
    {
        // done, time to restart this position tracking animation/timer
        animations.RestartAnimation(param.index);
        RandomPixels();
    }
}


#define HallPin1  37
#define HallPin3  39
#define HallPin5  21
#define HallPin7  34
#define HallPin9  35
#define HallPin11 36

#define MinSensorPin 38

uint8_t HourPinArr[] = {0,HallPin1,2,HallPin3,4,HallPin5,6,HallPin7,8,HallPin9,10,HallPin11};


#define HallDetect  LOW
#define HallUndetect  HIGH






bool fgMinuteCalied = false;
uint8_t fgHourCalied = 0;

uint8_t fgHourInt = 0;

void HourPinEvent(uint8_t HourPin)
{
  static int32_t FilterStep = INT_MIN;
  int32_t DefferMinute = (GetMotorGlobalStep() - FilterStep) / STEPS_PER_MINUTE;
  if(digitalRead(HourPinArr[HourPin]) == HallUndetect)
  {
      FilterStep = GetMotorGlobalStep();
  }else{
    if(FilterStep != INT_MIN)
    {
      if(DefferMinute >= 2 && DefferMinute <= 5)
      {
        FilterStep = INT_MIN;
        SetMotorGlobalStep((GetMotorGlobalStep() % STEPS_PER_HOUR) + STEPS_PER_HOUR * (HourPin)); 
        fgHourCalied = HourPin;  
      }
    } 
  } 
  fgHourInt = HourPin;
  
}

void HallPin1Event()
{
  HourPinEvent(1);
}
void HallPin3Event()
{
  HourPinEvent(3);
}
void HallPin5Event()
{
  HourPinEvent(5);
}
void HallPin7Event()
{
  HourPinEvent(7);
}
void HallPin9Event()
{
  HourPinEvent(9);
}
void HallPin11Event()
{
  HourPinEvent(11);
}

void MinSensorPinEvent()
{
  fgMinuteCalied = true;
  SetMotorGlobalStep(GetMotorGlobalStep() - (GetMotorGlobalStep() % STEPS_PER_HOUR) +  31 * STEPS_PER_MINUTE); 
}


static void PinInit()
{
    pinMode(PIN_OrangeLine, OUTPUT);
    pinMode(PIN_YellowLine, OUTPUT);
    pinMode(PIN_PinkLine, OUTPUT);
    pinMode(PIN_BlueLine, OUTPUT);
    
    pinMode(HallPin1,INPUT);
    attachInterrupt(HallPin1, HallPin1Event, CHANGE);

    pinMode(HallPin3,INPUT);
    attachInterrupt(HallPin3, HallPin3Event, CHANGE);

    pinMode(HallPin5,INPUT);
    attachInterrupt(HallPin5, HallPin5Event, CHANGE);

    pinMode(HallPin7,INPUT);
    attachInterrupt(HallPin7, HallPin7Event, CHANGE);

    pinMode(HallPin9,INPUT);
    attachInterrupt(HallPin9, HallPin9Event, CHANGE);

    pinMode(HallPin11,INPUT);
    attachInterrupt(HallPin11, HallPin11Event, CHANGE);

    pinMode(MinSensorPin,INPUT_PULLUP);
    attachInterrupt(MinSensorPin, MinSensorPinEvent, CHANGE);
}


bool clockGearInit()
{
    MotorGoForMinute(12*60);//6 hall for 12 a circle   
    while (!(fgHourCalied  && fgMinuteCalied ))
    {
        if(Motor.MotorState == MotorStop)
        {
            Serial.println("Clock Cali failed");
            return false;
        }
        animations.UpdateAnimations();
        strip.Show();
        delay(5);

        // if(fgMinuteCalied != 0)
        // {           
        //     Serial.printf("Cali:%d,",fgMinuteCalied);
        //     fgMinuteCalied = 0;
        //     Serial.printf("CaliTime: %d\n",(Motor.GlobalStep)/STEPS_PER_MINUTE);           
        // }
    }
    if(Motor.MotorState != MotorStop)
    {
      Motor_Stop();
    }
//    Serial.printf("Get28BYJ48GlobalStep:%d\n",GetMotorGlobalStep());
    return true;
}


void CaliTime(SD3078_t* SD3078)
{
  int32_t MechStepMinute = GetMotorGlobalStep() / STEPS_PER_MINUTE;
  int32_t ElecStepMinute = (SD3078->hour%12) * 60 + SD3078->minute;

  if(ElecStepMinute == MechStepMinute) return;

 // Serial.printf("MechStepMinute:%d,ElecStepMinute:%d\n",MechStepMinute,ElecStepMinute);

  if(ElecStepMinute >= MechStepMinute)
	{
		if((ElecStepMinute - MechStepMinute) > 6*60)//半圈
		{
			MotorGoForMinute(ElecStepMinute - (MechStepMinute + 12*60) );
      Serial.println("1");
      ClearPixels();
      DrawPixelsInit();
 //     LeftDrawTailPixels();
      animations.StartAnimation(0, 60, LeftLoopAnimUpdate); 
		}else{
			MotorGoForMinute(ElecStepMinute- MechStepMinute);
      Serial.println("2");
      ClearPixels();
      DrawPixelsInit();
 ///     RightDrawTailPixels();
      animations.StartAnimation(0, 60, RightLoopAnimUpdate); 
		}
	}else{
		if((MechStepMinute - ElecStepMinute) > 6*60)//半圈
		{
			MotorGoForMinute(ElecStepMinute + 12*60 - (MechStepMinute) );
      Serial.println("3");
      ClearPixels();
      DrawPixelsInit();
 //     RightDrawTailPixels();
      animations.StartAnimation(0, 60, RightLoopAnimUpdate); 
		}else{
			MotorGoForMinute(ElecStepMinute - MechStepMinute);
      Serial.println("4");
      ClearPixels();
      DrawPixelsInit();
 //     LeftDrawTailPixels();
      animations.StartAnimation(0, 60, LeftLoopAnimUpdate); 
		}		
	}
  while(Motor.MotorState != MotorStop)
  {
    animations.UpdateAnimations();
    strip.Show();
    delay(5);
  }
}


void Clock_task(void *arg)
{
    Serial.println("Clock task begain");
    struct tm timeinfo;
    SD3078_t SD3078;

    // touchAttachInterrupt(TouchRight, TouchRightEvent, threshold);
    // touchAttachInterrupt(TouchLeft, TouchLeftEvent, threshold);

    strip.Begin();
    
    DrawPixelsInit();
    strip.Show();
    // Draw the tail that will be rotated through all the rest of the pixels
//    RightDrawTailPixels();
    // we use the index 0 animation to time how often we rotate all the pixels
    animations.StartAnimation(0, 60, RightLoopAnimUpdate); 
    // animations.StartAnimation(0, 1, RandomPixelsUpdate); 
    // while(1)
    // {
    //     animations.UpdateAnimations();
    //     strip.Show();
    //     delay(100);
    // } 
   

    PinInit();
    ButtonInit();
    SD3078_Init();
    BH1750_Init();
    MotorInit();

    clockGearInit();

  //   MotorGoForMinute(12*60);  
  //  while(1)
  //  {
  //   if(fgHourInt != 0)
  //   {    
  //     Serial.printf("CaliHour: %d,",fgHourInt); 
  //     fgHourInt = 0;
  //     Serial.printf("CaliTime: %d\n",(Motor.GlobalStep)/STEPS_PER_MINUTE); 
  //   }
    
  //   delay(5);
  //   }

 //   CaliTime(&SD3078);


 // Serial.printf("Motor.GoalStep:%d\n",Motor.GoalStep);
    // while (!getLocalTime(&GearTime)) 
    // {
    //   vTaskDelay(1000);
    // }

    
 //   SetRandomSeed();
    // Draw the tail that will be rotated through all the rest of the pixels
   
  
    

 //   StepGoForMinute(12*60*5);
    
    while(1)
    {
      
      // Serial.printf("SD3078:Hour:%d,Munite:%d,Second:%d\n",SD3078.hour,SD3078.minute,SD3078.second);
      // LastMinuteStep = TimeAjust(LastMinuteStep);

        if(ulTaskNotifyTake(pdTRUE , 0))
        {
          getLocalTime(&timeinfo);
          SD3078.second = timeinfo.tm_sec;
          SD3078.minute = timeinfo.tm_min;
          SD3078.hour   = timeinfo.tm_hour;
          SD3078.month  = timeinfo.tm_mon;
          SD3078.day    = timeinfo.tm_mday;
          SD3078.week   = timeinfo.tm_wday;
          SD3078.year   = timeinfo.tm_year;
          Write_SD3078(&SD3078);
          Serial.println("WIFI Time sync succeed!");
          delay(5);
        }
         Read_SD3078(&SD3078);
          CaliTime(&SD3078); 

         DrawSecondPixelsRotate(&SD3078);
        
        // if(fgHourCalied != 0)
        // {           
        //     Serial.printf("Cali:%d,",fgHourCalied);
        //     fgHourCalied = 0;
        //     Serial.printf("CaliTime: %d\n",(Motor.GlobalStep)/STEPS_PER_MINUTE);           
        // }
         delay(100);

         float Lightness = BH1750_ReadLight();
//         Serial.printf("Lightness:%0.1f\n",Lightness);
        //lightness = Minlightness * pow(MaxLightness / Minlightness, Lightness / 10200.0f);
        lightness = 0.000043f * Lightness + 0.07f;
        if(lightness > MaxLightness)
        {
          lightness = MaxLightness;
        }else if(lightness < Minlightness){
            lightness = Minlightness;
        }
    }
}













