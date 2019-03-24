#ifndef _C_DS3231_H_INCLUDED_
#define _C_DS3231_H_INCLUDED_

#include "I2CDevice.h"

#define DS3231_SEND_BUFF_SIZE 0x01
#define DS3231_PARAM_COUNT    0x13

class DS3231RTC : public I2CDevice
{
	
public:
	
	DS3231RTC(I2C_HandleTypeDef* in_i2cHandle, u8 in_u8Address) : I2CDevice(in_i2cHandle, in_u8Address){};
	void Update();

	void SetTime(u8 in_Hour, u8 in_Min, u8 in_Sec);
   void SetDate(u8 in_Day, u8 in_Month, u8 in_Year);		
		
	void GetTimeChars(char* out_cStr);
	void GetDateChars(char* out_cStr);
	void GetTemperatureChars(char* out_cStr);

protected:
	
  u8 g_u8SendBuffer[DS3231_PARAM_COUNT];    // Buffer for send
  u8 g_u8ReceiveBuffer[DS3231_PARAM_COUNT]; // Buffer for device parametrs

};

#endif   // _C_DS3231_H_INCLUDED_
