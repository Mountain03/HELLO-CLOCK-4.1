#include "SaveNVS.h"

#include "nvs_flash.h"
#include "nvs.h"


const char* my_nvs_namespace = "my_nvs";
const char* NvsKeyWifiName = "wifiname";
const char* NvsKeyWifiPassword = "wifipassword";
const char* NvsKeyBootTimes = "boottimes"; 
const char* NvsKeyClockPos = "clockpos"; 


char StrWifiName[50] = {0};
char StrWifiPassword[50] = {0};

int32_t ClockPos = INT32_MAX;


void SysInforInit()
{
    esp_err_t esp_err = ESP_OK;
  uint32_t nvs_get_size = 0;

  nvs_flash_init();
  nvs_handle_t my_nvs;
 nvs_open(my_nvs_namespace,NVS_READWRITE,&my_nvs);
  
//print boot times
  uint32_t counter_val = 0;
  nvs_get_u32(my_nvs,NvsKeyBootTimes,&counter_val);
  Serial.printf("boot times:%d\n", counter_val);
  counter_val++;
  nvs_set_u32(my_nvs,NvsKeyBootTimes,counter_val);
  nvs_commit(my_nvs);

//   nvs_get_i32(my_nvs,NvsKeyClockPos,&ClockPos);
//   Serial.printf("ClockPos:%d\n", ClockPos);


  nvs_get_str(my_nvs, NvsKeyWifiName, NULL, &nvs_get_size);
  esp_err = nvs_get_str(my_nvs,NvsKeyWifiName,StrWifiName,&nvs_get_size);
  if(esp_err != ESP_OK)
  {    
      esp_err = ESP_FAIL;
  }

  nvs_get_str(my_nvs, NvsKeyWifiPassword, NULL, &nvs_get_size);
  esp_err = nvs_get_str(my_nvs,NvsKeyWifiPassword,StrWifiPassword,&nvs_get_size);
  if(esp_err != ESP_OK)
  {   
     esp_err = ESP_FAIL;
  }

 nvs_close(my_nvs);

   if(esp_err != ESP_OK)
  {
    Serial.println("wifi infor restore error");

      strcpy(StrWifiName,"MERCURY_F39F");
      strcpy(StrWifiPassword,"15236634549");
  }
}


 esp_err_t SaveWifiInfor(const char* nameKey,const char* Value)
{
  esp_err_t esp_err = ESP_OK;
  nvs_flash_init();
  nvs_handle_t my_nvs;
  nvs_open(my_nvs_namespace,NVS_READWRITE,&my_nvs);

  if(nvs_set_str(my_nvs,nameKey,Value) != ESP_OK)
  {
    esp_err = ESP_FAIL;
    // Serial.println(NvsKeyWifiName);
  }

  nvs_commit(my_nvs);

  nvs_close(my_nvs);
  nvs_flash_deinit();

  return esp_err;
}


 esp_err_t SaveClockPos(int32_t Value)
{
  esp_err_t esp_err = ESP_OK;
  nvs_flash_init();
  nvs_handle_t my_nvs;
  nvs_open(my_nvs_namespace,NVS_READWRITE,&my_nvs);

  if(nvs_set_i32(my_nvs,NvsKeyClockPos,Value) != ESP_OK)
  {
    esp_err = ESP_FAIL;
    // Serial.println(NvsKeyWifiName);
  }

  nvs_commit(my_nvs);
  nvs_close(my_nvs);
//  nvs_flash_deinit();

  return esp_err;
}



