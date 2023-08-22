#ifndef SAVENVS_H_INCLUDED
#define SAVENVS_H_INCLUDED



#include <Arduino.h>


extern const char* NvsKeyWifiName ;
extern const char* NvsKeyWifiPassword ;

extern char StrWifiName[50] ;
extern char StrWifiPassword[50] ;



void SysInforInit();
 esp_err_t SaveWifiInfor(const char* nameKey,const char* Value);
esp_err_t SaveClockPos(int32_t Value);


#endif

