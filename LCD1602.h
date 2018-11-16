#ifndef _C_LCD1602_H_INCLUDED_
#define _C_LCD1602_H_INCLUDED_

#include "I2CDevice.h"

class LCD1602 : public I2CDevice
{
public:
   LCD1602(I2C_HandleTypeDef* in_i2cHandle, u8 in_u8Address) : I2CDevice(in_i2cHandle, in_u8Address){};

   void Initialization();                               // Инициализация дисплея
   void WriteStr(u8 in_u8X, u8 in_u8Y, char* in_cChar); // Отправить строку на экран с указанием позиции
   void SendChar(char in_cChar);                        // Отправить символ на экран
   void SendString(char* in_cChar);                     // Отправить строку на экран
   void SetPos(u8 in_u8X, u8 in_u8Y);                   // Установить позицию курсора
   void Clear();                                        // Очистить экран
		
protected:

   void _WriteByte(u8 in_u8Byte);    // Отпвить байт на шину
   void _SendHalfByte(u8 in_u8Byte); // Отправить пол байта
   void _SendCommand(u8 in_u8Byte);  // Отправить байт команду

   I2C_HandleTypeDef* m_i2cHandle;  // I2C 
   u8 m_u8Address;                  // I2C адрес устройства
   u8 m_u8BaseRegister;             // Младшая часть байта с параметрами
   u8 g_u8SendBuffer[1];            // Буффер для отправки
};

#endif   // _C_LCD1602_H_INCLUDED_
