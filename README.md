# LCD1602-PCF8574-STM32-HAL



// Подключаем файлик                                                                   
#include "LCD1602.h"                                                                   
                                                                   
// Инициализируем класс дислея передав ему указатель на структуру дял работы с i2c и адрес дисплея
LCD1602 Lcd(&hi2c1, 0x4E);                                                                   
                                                                   
// Запусти инициализацию дисплея                                                                   
Lcd.Initialization();                                                                   

// И используй                                                                    
Lcd.WriteStr(0, 0, "Hello");                                                                   
Lcd.WriteStr(5, 1, "World");                                                                   
