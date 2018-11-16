#include "LCD1602.h"
#include "LCD1602Defines.h"

#define SET_FLAG_LED()     (m_u8BaseRegister|=LCD_MODE_LED)
#define SET_FLAG_WRITE()   (m_u8BaseRegister&=~LCD_MODE_WRITE)

/*
   Инициализация дисплея, обязательнаяпроцедура
   на входе: *
   на выходе: *
*/
void LCD1602::Initialization()
{
   // Инициализация экрана обязательна
	HAL_Delay(15);
	_SendHalfByte(LCD_INITIALIZATION);
	HAL_Delay(4);
	_SendHalfByte(LCD_INITIALIZATION);
	DelayMicro(100);
	_SendHalfByte(LCD_INITIALIZATION);
	HAL_Delay(1);
	_SendHalfByte(LCD_SET_CURSOR_TO_START);
	
   // Найстрока дисплея
	_SendCommand(LCD_SET_INTERFACE_LINES_FONT|LCD_4BIT_INTERFACE|LCD_TWO_LINE|LCD_5x8_FONT);
	_SendCommand(LCD_SET_CURSOR_AND_POWE_MODE|LCD_DISPLAY_ON|LCD_CURSOR_OFF|LCD_CURSOR_BLINK_OFF);
	_SendCommand(LCD_SET_SHIFT_AND_CHAR_DIRECTION|LCD_CHAR_DIRECTION_LEFT_RIGHT|LCD_SHIFT_DISABLE);
	
   // Очистить экран
	Clear();
	
   // Включить подсветку и режим записи
	SET_FLAG_LED();
	SET_FLAG_WRITE();
};

/*
   Отправить строку на экран с указанием позиции
   на входе: in_u8X   - позиция символа в строке
             in_u8Y   - номер строки
             in_cChar - символ для установки
   на выходе: *
*/
void LCD1602::WriteStr(u8 in_u8X, u8 in_u8Y, char* in_cChar)
{
	SetPos(in_u8X, in_u8Y);
	SendString(in_cChar);
};

/*
   Установка курсора
   на входе: in_u8X   - позиция символа в строке
             in_u8Y   - номер строки
   на выходе: *
*/
void LCD1602::SetPos(u8 in_u8X, u8 in_u8Y)
{
	switch(in_u8Y)
	{
		case 0:
			_SendCommand(in_u8X|LCD_SET_DDRAM_TO_ADDRESS);
			break;
		case 1:
			_SendCommand((LCD_2_LINE_OFFSET+in_u8X)|LCD_SET_DDRAM_TO_ADDRESS);
			break;
	}
};

/*
   Отправка строки на экран
   на входе: in_cChar - указатель на строку
   на выходе: *
*/
void LCD1602::SendString(char* in_pszChar)
{
	char* l_pszChar = in_pszChar;
	while((l_pszChar)[0])
		SendChar((l_pszChar++)[0]);
};

/*
   Отправка символа на экран
   на входе: in_cChar - символ
   на выходе: *
*/
void LCD1602::SendChar(char in_cChar)
{
	m_u8BaseRegister|=LCD_MODE_DATA;
	_SendHalfByte(in_cChar >> 4);
	_SendHalfByte(in_cChar);
};

/*
   Очистить экран
   на входе: *
   на выходе: *
*/
void LCD1602::Clear()
{
	_SendCommand(LCD_SET_CLEAR);
	DelayMicro(1530);
};

/*
   Отправка байта на шину
   на входе: in_u8Byte - байт с командой
   на выходе: *
*/
void LCD1602::_WriteByte(u8 in_u8Byte)
{
	g_u8SendBuffer[0] = m_u8BaseRegister|in_u8Byte;
	Write(g_u8SendBuffer, 1);
	DelayMicro(39);
};

/*
   Отправка половины байта экрану
   на входе: in_u8Byte - байт с командой
   на выходе: *
*/
void LCD1602::_SendHalfByte(u8 in_u8Byte)
{
	_WriteByte(LCD_MODE_E_SET|(in_u8Byte << 4));
	_WriteByte(LCD_MODE_E_RESET);
};

/*
   Отправка команды дисплею
   на входе: in_u8Byte - байт с командой
   на выходе: *
*/
void LCD1602::_SendCommand(u8 in_u8Byte)
{	
	m_u8BaseRegister&=~LCD_MODE_DATA;
	_SendHalfByte(in_u8Byte >> 4);
	_SendHalfByte(in_u8Byte);
};

/*
   Отправка половины байта экрану
   на входе: in_u8Byte - байт с командой
   на выходе: *
*/
__STATIC_INLINE void DelayMicro(__IO u32 in_u8micros)
{
	in_u8micros *= (SystemCoreClock / 1000000) / 5;
	while (in_u8micros--);
};
