#include "28byj48.h"
#include "sd3078.h"
#include <Arduino.h>
#include "nvs_flash.h"
#include "nvs.h"
#include "SaveNVS.h"


static hw_timer_t * timer = NULL;                        // 声明一个定时器

 Motor_t Motor = {0};

// ports used to control the stepper motor
// if your motor rotate to the opposite direction, 
// change the order as {2, 3, 4, 5};
static int port[4] = {PIN_OrangeLine, 
						PIN_YellowLine,
						PIN_PinkLine, 
						PIN_BlueLine};

// sequence of stepper motor control
static int seq[8][4] = {
  {  LOW, HIGH, HIGH,  LOW},
  {  LOW,  LOW, HIGH,  LOW},
  {  LOW,  LOW, HIGH, HIGH},
  {  LOW,  LOW,  LOW, HIGH},
  { HIGH,  LOW,  LOW, HIGH},
  { HIGH,  LOW,  LOW,  LOW},
  { HIGH, HIGH,  LOW,  LOW},
  {  LOW, HIGH,  LOW,  LOW}
};

static int ClockLoopDelayTime = 2;

static int32_t MotorGlobalStep = 0 ;

int32_t GetMotorGlobalStep()
{
	return Motor.GlobalStep;
}
void SetMotorGlobalStep(int32_t Step)
{
	Motor.GlobalStep = Step;
}


uint32_t timerStartDivide	= 5000;
uint32_t timerEndDivide		= 1000;

void Motor_Stop()
{
	timerDetachInterrupt(timer);
	for(uint32_t i = 0; i < 4; i++) 
	{
		digitalWrite(port[i],LOW);
	}
	Motor.MotorState = MotorStop;
}

void IRAM_ATTR onTimer()  // 中断函数
{     
	static int phase = 0; 
	uint32_t i;  
	int delta; 
 
	if(Motor.fgOrientation == 1)
	{
 		delta = 8-1;
        Motor.GlobalStep--;
		if(Motor.GlobalStep <= 0)
		{
			Motor.GlobalStep = 12 * 60 * STEPS_PER_MINUTE;
		}
	}else{
		delta = 8+1;
		Motor.GlobalStep++;
		if(Motor.GlobalStep >= 12 * 60 * STEPS_PER_MINUTE)
		{
			Motor.GlobalStep = 0;
		}
	}
	
	Motor.WalkedStepThisTime++;
	//accelorate
	if(Motor.timerDivide > timerEndDivide)
	{
		Motor.MotorState = MotorAcce;
		Motor.timerDivide -= 50;
		timerAlarmWrite(timer, Motor.timerDivide, true); 
	}else{Motor.MotorState = MotorRun;}

	phase = (phase + delta) % 8;

	for(i = 0; i < 4; i++) 
	{
		digitalWrite(port[i],seq[phase][i]);
	}
	//decelorate
	if((Motor.GoalStep - Motor.WalkedStepThisTime) < ((timerStartDivide - timerEndDivide)/50))
	{
		Motor.timerDivide += 100;
		timerAlarmWrite(timer, Motor.timerDivide, true); 
		Motor.MotorState = MotorDece;		
	}

	if(Motor.WalkedStepThisTime >= Motor.GoalStep)
	{
		Motor_Stop();
	}
//	Serial.printf("Motor.GoalStep:%d,Motor.WalkedStepThisTime:%d ,,Motor.MotorState:%d\n",Motor.GoalStep,Motor.WalkedStepThisTime,Motor.MotorState);
}

void MotorInit()
{
	timer = timerBegin(0, 80, true);		
}



void MotorGo(int32_t step)
{
	if(step < 0) //backward
	{
		Motor.fgOrientation = 1;
		Motor.GoalStep = -step;
	}else{		//foreward
		Motor.fgOrientation = 0;
		Motor.GoalStep = step;
	}

	Motor.WalkedStepThisTime = 0;
	Motor.timerDivide = timerStartDivide;
	Motor.MotorState = MotorAcce;

	timerAlarmWrite(timer, timerStartDivide, true); 
	timerAttachInterrupt(timer, &onTimer, true);
	timerAlarmEnable(timer); // 启用定时器
}


void MotorGoForMinute(uint32_t min)
{
	MotorGo(-20);
	while(Motor.MotorState != MotorStop){delay(5);}
	MotorGo(20);
	while(Motor.MotorState != MotorStop){delay(5);};
	MotorGo(STEPS_PER_MINUTE * min);
	
}


