#include "I2CDevice.h"

I2CDevice::I2CDevice(I2C_HandleTypeDef* in_i2cHandle, u8 in_u8Address)
{
	m_i2cHandle = in_i2cHandle;
	m_u8Address = in_u8Address;
};

void I2CDevice::Write(u8* in_u8SendBuffer, u8 in_u8Size)
{
	while(HAL_I2C_Master_Transmit(m_i2cHandle,(u16)m_u8Address, in_u8SendBuffer, (u16)in_u8Size, 1000) != HAL_OK);
};

void I2CDevice::Read(u8* in_u8ReadBuffer, u8 in_u8Size)
{
	while(HAL_I2C_Master_Receive(m_i2cHandle,(u16)m_u8Address, in_u8ReadBuffer, (u16)in_u8Size, 1000) != HAL_OK);
};
