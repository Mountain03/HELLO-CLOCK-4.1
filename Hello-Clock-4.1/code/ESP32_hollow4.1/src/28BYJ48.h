#ifndef __28BYJ_48_H
#define __28BYJ_48_H


#include <Arduino.h>

#define CCW

#ifdef CW   
    #define PIN_OrangeLine	    4//13//
    #define PIN_YellowLine	    2 //15//
    #define PIN_PinkLine	    15//2//
    #define PIN_BlueLine	    13//4//
#else
    #define PIN_OrangeLine	    13//
    #define PIN_YellowLine	    15//
    #define PIN_PinkLine	    2//
    #define PIN_BlueLine	    4//
#endif

// #define HIGH	GPIO_PIN_SET
// #define LOW		GPIO_PIN_RESET

//(32/9)*(22/11)*(26/9)*(31/10) = 567424 / 8910 = 4075.7728395

// Motor and clock parameters
// 4096 * 90 / 12 = 30720  //gear ratio 90:12
#define MINUTE_STEPS_PER_ROTATION 30720 // steps for a full round of minute rotor

//4075.7728395 * 90 /12 = 
//#define MINUTE_STEPS_PER_ROTATION 30568.2962963 // steps for a full round of minute rotor
#define STEPS_PER_MINUTE 	(MINUTE_STEPS_PER_ROTATION / 60)

#define STEPS_PER_HOUR 	    MINUTE_STEPS_PER_ROTATION
#define STEPS_PER_12HOUR 	    (MINUTE_STEPS_PER_ROTATION * 12)

// Please tune the following value if the clock gains or loses.
// Theoretically, standard of this value is 60000.
#define MILLIS_PER_MIN 60000 // milliseconds per a minute



typedef enum __MotorE
{
	MotorStop,
	MotorAcce,
	MotorRun,
	MotorDece,
}MotorState_t;

typedef struct __Motor
{
    int32_t     GlobalStep;
	int32_t	    GoalStep;
    int32_t     WalkedStepThisTime;
	int32_t	    timerDivide;
	int32_t	    fgOrientation;
	MotorState_t MotorState;
}Motor_t;
extern Motor_t Motor;



//void Step_Rotate(int step);
void MotorInit();
void MotorGo(int32_t step);
void MotorGoForMinute(uint32_t min);
void Motor_Stop();

int32_t GetMotorGlobalStep();
void SetMotorGlobalStep(int32_t Step);



// void StepGoForMinute(uint32_t min);
// void ClockLoop();
// int32_t TimeAjust(int32_t StartMinuteStep);




#endif







