#include "DS3231.h"
#include <cmath>

#define NUMBER_TO_CHAR         0x30
#define CHAR_DOT               0x2E
#define CHAR_DOUBLE_DOT        0x3A
#define CHAR_END               0x00

#define HIGH_MASK              0x70
#define LOW_MASK               0x0F
                               
#define TIME_FORMAT_12         0x40
#define TIME_24_MASK           0x30
#define TIME_12_MASK           0x10
                               
#define TIME_SEC_POS           0x00
#define TIME_MIN_POS           0x01
#define TIME_HOUR_POS          0x02
                               
#define DATE_DAY_MASK          0x07
#define DATE_DATE_HIGH_MASK    0x30
#define DATE_MONTH_HIGH_MASK   0x10
#define DATE_YEAR_CENTURY_MASK 0x80
#define DATE_YEAR_HIGH_MASK    0xF0
#define DATE_YEAR_CONST        0x02

#define DATE_DAY               0x03
#define DATE_DATE              0x04
#define DATE_MONTH             0x05
#define DATE_YEAR              0x06
                               
#define A1_SEC                 0x07
#define A1_MIN                 0x08
#define A1_HOUR                0x09
#define A1_DAY                 0x0A
                               
#define A2_MIN                 0x0B
#define A2_HOUR                0x0C
#define A2_DAY                 0x0D
                               
#define CONTROL                0x0E
#define CONTROL_STATUS         0x0F
#define AGING                  0x10
                               
#define TEMP_MSB               0x11
#define TEMP_LSB               0x12
#define TEMPERATURE_SIGN_MASK  0x80
#define TEMPERATURE_MSB_MASK   0x7F
#define TEMPERATURE_LSB_MASK   0xC0

void DS3231RTC::Update()
{
	g_u8SendBuffer[0] = 0;
	Write(g_u8SendBuffer, 1);
	Read(g_u8ReceiveBuffer, DS3231_PARAM_COUNT);
};

void DS3231RTC::SetTime(u8 in_Hour, u8 in_Min, u8 in_Sec)
{		
	Update();
	
	for(u8 i = 0; i < DS3231_PARAM_COUNT; i++)
		g_u8SendBuffer[i] = g_u8ReceiveBuffer[i];
	
	u8 l_HSec = in_Sec / 10;
	u8 l_LSec = in_Sec % 10;
	
	u8 l_HMin = in_Min / 10;
	u8 l_LMin = in_Min % 10;
	
	u8 l_HHour = in_Hour / 10;
	u8 l_LHour = in_Hour % 10;

	g_u8SendBuffer[TIME_SEC_POS] = l_LSec & LOW_MASK;
	g_u8SendBuffer[TIME_SEC_POS] |= l_HSec << 4;    


	g_u8SendBuffer[TIME_MIN_POS] = l_LMin & LOW_MASK;
	g_u8SendBuffer[TIME_MIN_POS] |= l_HMin << 4;
	
	g_u8SendBuffer[TIME_HOUR_POS] = l_LHour & LOW_MASK;
	g_u8SendBuffer[TIME_HOUR_POS] |= l_HHour << 4;  
		
	g_u8SendBuffer[0] = 1;
	Write(g_u8SendBuffer, 1);
		
	Write(g_u8SendBuffer, DS3231_PARAM_COUNT);
}

void DS3231RTC::SetDate(u8 in_Day, u8 in_Month, u8 in_Year)
{		
	Update();
	
	for(u8 i = 0; i < DS3231_PARAM_COUNT; i++)
		g_u8SendBuffer[i] = g_u8ReceiveBuffer[i];
	
	u8 l_HDate = in_Day / 10;
	u8 l_LDate = in_Day % 10;
	
	u8 l_HMonth = in_Month / 10;
	u8 l_LMonth = in_Month % 10;
	
	u8 l_HYear = in_Year / 10;
	u8 l_LYear = in_Year % 10;

	g_u8SendBuffer[DATE_DATE] = l_LDate & LOW_MASK;
	g_u8SendBuffer[DATE_DATE] |= l_HDate << 4;    


	g_u8SendBuffer[DATE_MONTH] = l_LMonth & LOW_MASK;
	g_u8SendBuffer[DATE_MONTH] |= l_HMonth << 4;
	
	g_u8SendBuffer[DATE_YEAR] = l_LYear & LOW_MASK;
	g_u8SendBuffer[DATE_YEAR] |= l_HYear << 4;  
		
	g_u8SendBuffer[0] = 1;
	Write(g_u8SendBuffer, 1);
		
	Write(g_u8SendBuffer, DS3231_PARAM_COUNT);
}

void DS3231RTC::GetTimeChars(char* out_cStr)
{
	if((g_u8ReceiveBuffer[TIME_HOUR_POS] & TIME_FORMAT_12) == TIME_FORMAT_12)
		out_cStr[0] = ((g_u8ReceiveBuffer[TIME_HOUR_POS] & TIME_12_MASK) >> 4) | NUMBER_TO_CHAR;
	else
		out_cStr[0] = ((g_u8ReceiveBuffer[TIME_HOUR_POS] & TIME_24_MASK) >> 4) | NUMBER_TO_CHAR;	
		out_cStr[1] = ( g_u8ReceiveBuffer[TIME_HOUR_POS] & LOW_MASK)           | NUMBER_TO_CHAR;

		out_cStr[2] = CHAR_DOUBLE_DOT;
	
		out_cStr[3] = ((g_u8ReceiveBuffer[TIME_MIN_POS] & HIGH_MASK) >> 4) | NUMBER_TO_CHAR;
		out_cStr[4] = ( g_u8ReceiveBuffer[TIME_MIN_POS] & LOW_MASK)        | NUMBER_TO_CHAR;
	
		out_cStr[5] = CHAR_DOUBLE_DOT;
	
		out_cStr[6] = ((g_u8ReceiveBuffer[TIME_SEC_POS] & HIGH_MASK) >> 4 ) | NUMBER_TO_CHAR;
		out_cStr[7] = ( g_u8ReceiveBuffer[TIME_SEC_POS] & LOW_MASK)         | NUMBER_TO_CHAR;
	
		out_cStr[8] = CHAR_END;
}

void DS3231RTC::GetDateChars(char* out_cStr)
{
		out_cStr[0] = ((g_u8ReceiveBuffer[DATE_DATE] & DATE_DATE_HIGH_MASK) >> 4) | NUMBER_TO_CHAR;
		out_cStr[1] =  (g_u8ReceiveBuffer[DATE_DATE] & LOW_MASK)                  | NUMBER_TO_CHAR;
	
		out_cStr[2] = CHAR_DOT;
	
		out_cStr[3] = ((g_u8ReceiveBuffer[DATE_MONTH] & DATE_MONTH_HIGH_MASK)  >> 4 ) | NUMBER_TO_CHAR;
		out_cStr[4] =  (g_u8ReceiveBuffer[DATE_MONTH] & LOW_MASK)                     | NUMBER_TO_CHAR;
	
		out_cStr[5] = CHAR_DOT;
	
		out_cStr[6] = DATE_YEAR_CONST | NUMBER_TO_CHAR;
		out_cStr[7] = ((g_u8ReceiveBuffer[DATE_MONTH] & DATE_YEAR_CENTURY_MASK) >> 7) | NUMBER_TO_CHAR;
	
		out_cStr[8] = ((g_u8ReceiveBuffer[DATE_YEAR] & DATE_YEAR_HIGH_MASK) >> 4) | NUMBER_TO_CHAR;
		out_cStr[9] =  (g_u8ReceiveBuffer[DATE_YEAR] & LOW_MASK)                  | NUMBER_TO_CHAR;
	
		out_cStr[10] = 0;
}

void DS3231RTC::GetTemperatureChars(char* out_cStr)
{
		u8 l_u8Sign    = ((g_u8ReceiveBuffer[TEMP_MSB] & TEMPERATURE_SIGN_MASK) == TEMPERATURE_SIGN_MASK);
		s8 l_s8TempMSB =  (g_u8ReceiveBuffer[TEMP_MSB] & TEMPERATURE_MSB_MASK);
	  u8 l_u8TempLSB =  (g_u8ReceiveBuffer[TEMP_LSB] & TEMPERATURE_LSB_MASK);
	
		if(l_u8Sign)
			l_s8TempMSB = 0 - l_s8TempMSB;
	
		l_u8TempLSB = std::pow((float)(((l_u8TempLSB & 0x80) >> 7) * 2), -1) + std::pow((float)(((l_u8TempLSB & 0x40) >> 6) * 2), -2);
	
	  sprintf(out_cStr, "%d.%dC", l_s8TempMSB, l_u8TempLSB);
}

