#include "sd3078.h"
#include "Wire.h"

SD3078_t SD3078 = {0};

#define IMU_I2C_SDA 25
#define IMU_I2C_SCL 26

uint8_t bcd_to_decimal_code( uint8_t bcd)
{
//    return (bcd + (bcd/10) * 6);
    return (bcd >> 4) * 10 + (bcd & 0x0f);
}

uint8_t decimal_to_bcd_code(uint8_t decimal)
{
//    return (decimal - (decimal>>4) * 6);
    return  ((decimal / 10) << 4) | (decimal % 10);
}

void SD3078_Init()
{
    Wire.begin(IMU_I2C_SDA, IMU_I2C_SCL,4000000);
//    Wire.setClock(10000);
}

void Read_SD3078(SD3078_t* Time_Def)
{
    Wire.requestFrom(RTC_Address, 7);
    Time_Def->second = bcd_to_decimal_code(Wire.read());
    Time_Def->minute = bcd_to_decimal_code(Wire.read());
    Time_Def->hour= bcd_to_decimal_code(Wire.read()&(~0x80));
    Time_Def->week= bcd_to_decimal_code(Wire.read());
    Time_Def->day= bcd_to_decimal_code(Wire.read());
    Time_Def->month= bcd_to_decimal_code(Wire.read());
    Time_Def->year= bcd_to_decimal_code(Wire.read());
}

static void Write_SD3078_Enable(void)
{
    Wire.beginTransmission(RTC_Address);
    Wire.write(CTR2);
    Wire.write(0x80);//WRTC1=1 
    Wire.endTransmission();

    Wire.beginTransmission(RTC_Address);
    Wire.write(CTR1);
    Wire.write(0x84);//WRTC2,WRTC3=1 
    Wire.endTransmission();
}

static void Write_SD3078_Disable(void)
{
    Wire.beginTransmission(RTC_Address);
    Wire.write(CTR1);
    Wire.write(0);//WRTC1=1 
    Wire.write(0);//WRTC2,WRTC3=0 
    Wire.endTransmission();
}

void Write_SD3078(SD3078_t* Time_Def)
{
    Write_SD3078_Enable();

    Wire.beginTransmission(RTC_Address);     //发开启传输Start + 地址0x44（7位） + 写入Bit（1位）
    Wire.write(0);
    Wire.write(decimal_to_bcd_code(Time_Def->second));
    Wire.write(decimal_to_bcd_code(Time_Def->minute));
    Wire.write(decimal_to_bcd_code(Time_Def->hour)|0x80);
    Wire.write(decimal_to_bcd_code(Time_Def->week));
    Wire.write(decimal_to_bcd_code(Time_Def->day));
    Wire.write(decimal_to_bcd_code(Time_Def->month));
    Wire.write(decimal_to_bcd_code(Time_Def->year));
    Wire.endTransmission();

    Write_SD3078_Disable();
}







