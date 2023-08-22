#ifndef SD3078_H_INCLUDED
#define SD3078_H_INCLUDED


#include <Arduino.h>




/*
 * Í¨¹ýºê¶¨ÒåÑ¡ÔñÊ¹ÓÃµÄRTCÐÍºÅ
 * SD30XX_25XX -> Ê¹ÓÃSD30»òSD25ÏµÁÐ
 * SD20XX_24XX -> Ê¹ÓÃSD20»òSD24ÏµÁÐ
 */
#define		SD30XX_25XX
#undef		SD20XX_24XX

/********************************************************/
// #define		TRUE            1
// #define		FALSE           0
// #define		H               1
// #define		L               0
// #define		Chg_enable		0x82
// #define		Chg_disable		0

/******************** Device Address ********************/
#define		RTC_Address     0x32 

/******************** Alarm register ********************/
#define		Alarm_SC				0x07
#define		Alarm_MN				0x08
#define		Alarm_HR				0x09
#define		Alarm_WK				0x0A
#define		Alarm_DY				0x0B
#define		Alarm_MO				0x0C
#define		Alarm_YR				0x0D
#define		Alarm_EN				0x0E

/******************** Control Register *******************/
#define		CTR1            0x0F
#define		CTR2            0x10
#define		CTR3            0x11

/***************** Timer Counter Register ****************/
#define		Timer_Counter1	0x13
#define		Timer_Counter2	0x14
#define		Timer_Counter3	0x15

/******************** Battery Register ********************/
#define		Chg_MG          0x18		//³äµç¹ÜÀí¼Ä´æÆ÷µØÖ·
#define		Bat_H8          0x1A		//µçÁ¿×î¸ßÎ»¼Ä´æÆ÷µØÖ·
#define		Bat_L8          0x1B		//µçÁ¿µÍ°ËÎ»¼Ä´æÆ÷µØÖ·

/*********************** ID Register **********************/
#define		ID_Address			0x72		//IDºÅÆðÊ¼µØÖ·

/********************** ±¨¾¯ÖÐ¶Ïºê¶¨Òå *********************/
#define		sec_ALM					0x01
#define		min_ALM					0x02
#define		hor_ALM					0x04
#define		wek_ALM					0x08
#define		day_ALM					0x10
#define		mon_ALM					0x20
#define		yar_ALM					0x40

/********************** ÖÐ¶ÏÊ¹ÄÜºê¶¨Òå **********************/
#define		INTDE						0x04		//µ¹¼ÆÊ±ÖÐ¶Ï
#define		INTAE						0x02		//±¨¾¯ÖÐ¶Ï
#define		INTFE						0x01		//ÆµÂÊÖÐ¶Ï

/********************** ÖÐ¶ÏÑÝÊ¾ºê¶¨Òå **********************/
#define 	FREQUENCY				0				//ÆµÂÊÖÐ¶Ï
#define 	ALARM					1				//±¨¾¯ÖÐ¶Ï
#define 	COUNTDOWN				2				//µ¹¼ÆÊ±ÖÐ¶Ï
#define 	DISABLE					3				//½ûÖ¹ÖÐ¶Ï






typedef	struct
{
	uint8_t	second;
	uint8_t	minute;
	uint8_t	hour;
	uint8_t	week;
	uint8_t	day;
	uint8_t	month;
	uint8_t	year;
} SD3078_t;
extern SD3078_t SD3078;


void SD3078_Init();
void Read_SD3078(SD3078_t* Time_Def);
void Write_SD3078(SD3078_t* Time_Def);



#endif




