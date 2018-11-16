#ifndef _C_I2C_DEVICE_H_INCLUDED_
#define _C_I2C_DEVICE_H_INCLUDED_

#include "Platform.h"
#include "Types.h"

class I2CDevice
{
public:
	
	I2CDevice(I2C_HandleTypeDef* in_i2cHandle, u8 in_u8Address);

	void Write(u8* in_u8SendBuffer, u8 in_u8Size);
	void Read(u8* in_u8ReadBuffer, u8 in_u8Size);

protected:

	I2C_HandleTypeDef* m_i2cHandle;           // I2C Handle
	u8 m_u8Address;                           // I2C device address

};

#endif   // _C_I2C_DEVICE_H_INCLUDED_
