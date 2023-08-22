﻿#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>


#define BTN_NAME_MAX  32     //名字最大为32字节

/* 按键消抖时间40ms, 建议调用周期为20ms
 只有连续检测到40ms状态不变才认为有效，包括弹起和按下两种事件
*/

#define CONTINUOS_TRIGGER             0  //是否支持连续触发，连发的话就不要检测单双击与长按了	

/* 是否支持单击&双击同时存在触发，如果选择开启宏定义的话，单双击都回调，只不过单击会延迟响应，
   因为必须判断单击之后是否触发了双击否则，延迟时间是双击间隔时间 BUTTON_DOUBLE_TIME。
   而如果不开启这个宏定义，建议工程中只存在单击/双击中的一个，否则，在双击响应的时候会触发一次单击，
   因为双击必须是有一次按下并且释放之后才产生的 */
#define SINGLE_AND_DOUBLE_TRIGGER     0 

/* 是否支持长按释放才触发，如果打开这个宏定义，那么长按释放之后才触发单次长按，
   否则在长按指定时间就一直触发长按，触发周期由 BUTTON_LONG_CYCLE 决定 */
#define LONG_FREE_TRIGGER             0 

#define BUTTON_DEBOUNCE_TIME 	  1   //消抖时间      (n-1)*调用周期
#define BUTTON_CONTINUOS_CYCLE  1	  //连按触发周期时间  (n-1)*调用周期  
#define BUTTON_LONG_CYCLE       1	  //长按触发周期时间  (n-1)*调用周期 
#define BUTTON_DOUBLE_TIME      15 	//双击间隔时间  (n-1)*调用周期  建议在200-600ms
#define BUTTON_LONG_TIME 	      50		/* 持续n秒((n-1)*调用周期 ms)，认为长按事件 */

#define TRIGGER_CB(event)   \
        if(btn->CallBack_Function[event]) \
          btn->CallBack_Function[event]((Button_t*)btn)

typedef void (*Button_CallBack)(void*);   /* 按键触发回调函数，需要用户实现 */



typedef enum {
  BUTTON_DOWM = 0,
  BUTTON_UP,
  BUTTON_DOUBLE,
  BUTTON_LONG,
  BUTTON_LONG_FREE,
  BUTTON_CONTINUOS,
  BUTTON_CONTINUOS_FREE,
  BUTTON_ALL_RIGGER,
  number_of_event, /* 触发回调的事件 */
  NONE_TRIGGER
}Button_Event;

/*
	每个按键对应1个全局的结构体变量。
	其成员变量是实现滤波和多种按键状态所必须的
*/
typedef struct button
{
	/* 下面是一个函数指针，指向判断按键手否按下的函数 */
	uint8_t (*Read_Button_Level)(void); /* 读取按键电平函数，需要用户实现 */
  
  char Name[BTN_NAME_MAX];
  	
  uint8_t Button_State              :   4;	  /* 按键当前状态（按下还是弹起） */
  uint8_t Button_Last_State         :   4;	  /* 上一次的按键状态，用于判断双击 */
  uint8_t Button_Trigger_Level      :   2;    /* 按键触发电平 */
  uint8_t Button_Last_Level         :   2;    /* 按键当前电平 */
  
  uint8_t Button_Trigger_Event;     /* 按键触发事件，单击，双击，长按等 */
  
  Button_CallBack CallBack_Function[number_of_event];
  
	uint8_t Button_Cycle;	           /* 连续按键周期 */
  
  uint8_t Timer_Count;			/* 计时 */
	uint8_t Debounce_Time;		/* 消抖时间 */
  
	uint8_t Long_Time;		  /* 按键按下持续时间 */
  
  struct button *Next;
  
}Button_t;




/* 供外部调用的函数声明 */

void Button_Create(const char *name,
                  Button_t *btn, 
                  uint8_t(*read_btn_level)(void),
                  uint8_t btn_trigger_level);
                  
void Button_Attach(Button_t *btn,Button_Event btn_event,Button_CallBack btn_callback);   
                  
void Button_Cycle_Process(Button_t *btn);   
                  
void Button_Process(void);
                  
void Button_Delete(Button_t *btn);   
  
void Search_Button(void);     
                  
void Get_Button_EventInfo(Button_t *btn);
uint8_t Get_Button_Event(Button_t *btn);
uint8_t Get_Button_State(Button_t *btn);
void Button_Process_CallBack(void *btn);





/****************************************************************************/
/****************************************************************************/
/****************************************************************************/


/**
* @name Debug print 
* @{
*/
#define PRINT_DEBUG_ENABLE		0		/* 打印调试信息 */
#define PRINT_ERR_ENABLE			0	  /* 打印错误信息 */
#define PRINT_INFO_ENABLE			0		/* 打印个人信息 */


#if PRINT_DEBUG_ENABLE
#define PRINT_DEBUG(fmt, args...) 	 do{(printf("\n[DEBUG] >> "), printf(fmt, ##args));}while(0)     
#else
#define PRINT_DEBUG(fmt, args...)	     
#endif

#if PRINT_ERR_ENABLE
#define PRINT_ERR(fmt, args...) 	 do{(printf("\n[ERR] >> "), printf(fmt, ##args));}while(0)     
#else
#define PRINT_ERR(fmt, args...)	       
#endif

#if PRINT_INFO_ENABLE
#define PRINT_INFO(fmt, args...) 	 do{(printf("\n[INFO] >> "), printf(fmt, ##args));}while(0)     
#else
#define PRINT_INFO(fmt, args...)	       
#endif

/**@} */
	
//针对不同的编译器调用不同的stdint.h文件
#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)
    #include <stdint.h>
#endif

/* 断言 Assert */
#define AssertCalled(char,int) 	printf("\nError:%s,%d\r\n",char,int)
#define ASSERT(x)   if((x)==0)  AssertCalled(__FILE__,__LINE__)

typedef enum 
{
	ASSERT_ERR = 0,								/* 错误 */
	ASSERT_SUCCESS = !ASSERT_ERR	/* 正确 */
} Assert_ErrorStatus;

typedef enum 
{
	FALSE = 0,		/* 假 */
	TRUE = !FALSE	/* 真 */
}ResultStatus;











#endif
