/*
 * bh1750.c
 *
 *	The MIT License.
 *  Created on: 02.12.2018
 *      Author: Mateusz Salamon
 *		www.msalamon.pl
 *		mateusz@msalamon.pl
 *
 *	https://msalamon.pl/pomiar-natezenia-swiatla-z-wykorzystaniem-stm32/
 *	https://github.com/lamik/Light_Sensors_STM32
 */


//#include "i2c.h"

#include "bh1750.h"
#include "Wire.h"

// #define I2C_WR	        0		/* д����bit */
// #define I2C_RD	        1		/* ������bit */

// #define I2C_SCL_HIGH()                      HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET)    // ����ߵ�ƽ
// #define I2C_SCL_LOW()                       HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET)  // ����͵�ƽ
// #define I2C_SDA_HIGH()                      HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET)    // ����ߵ�ƽ
// #define I2C_SDA_LOW()                       HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_RESET)  // ����͵�ƽ
// #define I2C_SDA_READ()                      HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_9)




// static void I2C_Delay(void)
// {
// 	uint16_t i;

// 	/*��
// 	 	�����ʱ����ͨ���߼������ǲ��Եõ��ġ�
// 		CPU��Ƶ72MHzʱ�����ڲ�Flash����, MDK���̲��Ż�
// 		ѭ������Ϊ10ʱ��SCLƵ�� = 205KHz 
// 		ѭ������Ϊ7ʱ��SCLƵ�� = 347KHz�� SCL�ߵ�ƽʱ��1.5us��SCL�͵�ƽʱ��2.87us 
// 	 	ѭ������Ϊ5ʱ��SCLƵ�� = 421KHz�� SCL�ߵ�ƽʱ��1.25us��SCL�͵�ƽʱ��2.375us 
        
//     IAR���̱���Ч�ʸߣ���������Ϊ7
// 	*/
// 	for (i = 0; i < 5; i++);
// }


// static void I2C_Start(void)
// {
// 	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C���������ź� */
// 	I2C_SDA_HIGH();
// 	I2C_SCL_HIGH();
// 	I2C_Delay();
// 	I2C_SDA_LOW();
// 	I2C_Delay();
// 	I2C_SCL_LOW();
// 	I2C_Delay();
// }

// static void I2C_Stop(void)
// {
// 	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C����ֹͣ�ź� */
// 	I2C_SDA_LOW();
// 	I2C_SCL_HIGH();
// 	I2C_Delay();
// 	I2C_SDA_HIGH();
// }

// static uint8_t I2C_WaitAck(void)
// {
// 	uint8_t re;

// 	I2C_SDA_HIGH();	/* CPU�ͷ�SDA���� */
// 	I2C_Delay();
// 	I2C_SCL_HIGH();	/* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
// 	I2C_Delay();
// 	if (I2C_SDA_READ())	/* CPU��ȡSDA����״̬ */
// 	{
// 		re = 1;// ��Ӧ��
// 	}
// 	else
// 	{
// 		re = 0;//Ӧ��
// 	}
// 	I2C_SCL_LOW();
// 	I2C_Delay();
// 	return re;
// }

// static void I2C_Ack(void)
// {
// 	I2C_SDA_LOW();	/* CPU����SDA = 0 */
// 	I2C_Delay();
// 	I2C_SCL_HIGH();	/* CPU����1��ʱ�� */
// 	I2C_Delay();
// 	I2C_SCL_LOW();
// 	I2C_Delay();
// 	I2C_SDA_HIGH();	/* CPU�ͷ�SDA���� */
// }

// static void I2C_NAck(void)
// {
// 	I2C_SDA_HIGH();	/* CPU����SDA = 1 */
// 	I2C_Delay();
// 	I2C_SCL_HIGH();	/* CPU����1��ʱ�� */
// 	I2C_Delay();
// 	I2C_SCL_LOW();
// 	I2C_Delay();	
// }

// static void I2C_SendByte(uint8_t Byte)
// {
// 	uint8_t i;

// 	/* �ȷ����ֽڵĸ�λbit7 */
// 	for (i = 0; i < 8; i++)
// 	{		
// 		if (Byte & 0x80)
// 		{
// 			I2C_SDA_HIGH();
// 		}
// 		else
// 		{
// 			I2C_SDA_LOW();
// 		}
// 		I2C_Delay();
// 		I2C_SCL_HIGH();
// 		I2C_Delay();	
// 		I2C_SCL_LOW();
// 		if (i == 7)
// 		{
// 			I2C_SDA_HIGH(); // �ͷ�����
// 		}
// 		Byte <<= 1;	/* ����һ��bit */
// 		I2C_Delay();
// 	}
// }

// static uint8_t I2C_ReadByte(void)
// {
// 	uint8_t i;
// 	uint8_t value;

// 	/* ������1��bitΪ���ݵ�bit7 */
// 	value = 0;
// 	for (i = 0; i < 8; i++)
// 	{
// 		value <<= 1;
// 		I2C_SCL_HIGH();
// 		I2C_Delay();
// 		if (I2C_SDA_READ())
// 		{
// 			value++;
// 		}
// 		I2C_SCL_LOW();
// 		I2C_Delay();
// 	}
// 	return value;
// }

// static void I2C_InitGPIO(void)
// {
//   GPIO_InitTypeDef GPIO_InitStruct;
  
//   /* ��GPIOʱ�� */
// //	I2C_GPIO_CLK_ENABLE();

//   GPIO_InitStruct.Pin = GPIO_PIN_8;
//   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
//   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
//   GPIO_InitStruct.Pull = GPIO_PULLUP;
//   HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
// 	GPIO_InitStruct.Pin = GPIO_PIN_9;
//   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
//   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
//   GPIO_InitStruct.Pull = GPIO_PULLUP;
//   HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
//   /* ��һ��ֹͣ�ź�, ��λI2C�����ϵ������豸������ģʽ */
//   I2C_Stop();

// 	HAL_Delay(1);
// }


// static uint8_t BH1750_CheckDevice(uint8_t _Address)
// {
// 	uint8_t ucAck;

// 	I2C_InitGPIO();		/* ����GPIO */	
// 	I2C_Start();		/* ���������ź� */
// 	/* �����豸��ַ+��д����bit��0 = w�� 1 = r) bit7 �ȴ� */
// 	I2C_SendByte(_Address | I2C_WR);
// 	ucAck = I2C_WaitAck();	/* ����豸��ACKӦ�� */
// 	I2C_Stop();			/* ����ֹͣ�ź� */
// 	return ucAck;
// }

// uint8_t BH1750_ReadBytes(uint8_t *ReadBuf, uint16_t Address, uint16_t Size)
// {
// 	uint16_t i;

// //	/* ��6������������I2C���ߡ�ǰ��Ĵ����Ŀ����EEPROM���͵�ַ�����濪ʼ��ȡ���� */
// 	I2C_Start();	
  
// 	/* ��7������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
// 	I2C_SendByte(Address | I2C_RD);	/* �˴��Ƕ�ָ�� */
	
// 	/* ��8��������ACK */
// 	if (I2C_WaitAck() != 0)
// 	{
// 		goto cmd_fail;	/* EEPROM������Ӧ�� */
// 	}	
	
// 	/* ��9����ѭ����ȡ���� */
// 	for (i = 0; i < Size; i++)
// 	{
// 		ReadBuf[i] = I2C_ReadByte();	/* ��1���ֽ� */
		
// 		/* ÿ����1���ֽں���Ҫ����Ack�� ���һ���ֽڲ���ҪAck����Nack */
// 		if (i != Size - 1)
// 		{
// 			I2C_Ack();	/* �м��ֽڶ����CPU����ACK�ź�(����SDA = 0) */
// 		}
// 		else
// 		{
// 			I2C_NAck();	/* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */
// 		}
// 	}
// 	/* ����I2C����ֹͣ�ź� */
// 	I2C_Stop();
// //	SYS_LOG("ee2prom read succeed\n");
// 	return 0;	/* ִ�гɹ� */

// cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
// 	/* ����I2C����ֹͣ�ź� */
// 	I2C_Stop();
// //	SYS_LOG("ee2prom read failed\n");
// 	return 1;
// }

// uint8_t BH1750_WriteBytes(uint8_t *WriteBuf, uint16_t Address, uint16_t Size)
// {
// 	uint16_t i,m;
// 	uint16_t usAddr;
	
// 	I2C_Start();
// 	I2C_SendByte(Address | I2C_WR);	/* �˴���дָ�� */
// 	if (I2C_WaitAck() != 0)
// 	{
// 		goto cmd_fail;	/* EEPROM������Ӧ�� */
// 	}
// 	I2C_SendByte(WriteBuf[0]);
// 	I2C_WaitAck();
// 	I2C_Stop();
// 	return 0;

// cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
// 	/* ����I2C����ֹͣ�ź� */
// 	I2C_Stop();
// //	SYS_LOG("ee2prom write failed\n");
// 	return 1;	
// }	
	
	

uint8_t BH1750_WriteBytes(uint8_t WriteBuf)
{
	Wire.beginTransmission(BH1750_ADDRESS);     //发开启传输Start + 地址0x44（7位） + 写入Bit（1位）
    Wire.write(WriteBuf);
    Wire.endTransmission();
	return BH1750_OK;
}

uint8_t BH1750_ReadBytes(uint8_t* WriteBuf)
{
	Wire.requestFrom(BH1750_ADDRESS, 2);
	WriteBuf[0] = Wire.read();
	WriteBuf[1] = Wire.read();
	return BH1750_OK;
}


//I2C_HandleTypeDef 	*bh1750_i2c;	// Handler to I2C interface
bh1750_mode 		Bh1750_Mode;	// Current sensor mode
uint8_t 			Bh1750_Mtreg;	// Current MT register value

//
//	Initialization.
//
BH1750_STATUS BH1750_Init()
{
//	bh1750_i2c = hi2c;
	
	 BH1750_STATUS err = BH1750_ERROR;
	// if(BH1750_OK != BH1750_CheckDevice(BH1750_ADDRESS))
	// {
	// 	return BH1750_ERROR;
	// }
	if(BH1750_OK == BH1750_Reset())
	{
		delay(500);
		if(BH1750_OK == BH1750_SetMtreg(BH1750_DEFAULT_MTREG)) // Set default value;
			err =  BH1750_OK;
	}
	BH1750_SetMode(CONTINUOUS_LOW_RES_MODE);
	return err;
}

//
//	Reset all registers to default value.
//
BH1750_STATUS BH1750_Reset(void)
{
	uint8_t tmp = 0x07;
//	if(HAL_OK == HAL_I2C_Master_Transmit(bh1750_i2c, BH1750_ADDRESS, &tmp, 1, 10))
	if(BH1750_OK == BH1750_WriteBytes(tmp))
		return BH1750_OK;

	return BH1750_ERROR;
}

//
//	Set the power state.
//	0 - sleep, low power.
//	1 - running.
//
BH1750_STATUS BH1750_PowerState(uint8_t PowerOn)
{
	PowerOn = (PowerOn? 1:0);
//	if(HAL_OK == HAL_I2C_Master_Transmit(bh1750_i2c, BH1750_ADDRESS, &PowerOn, 1, 10))
	if(BH1750_OK == BH1750_WriteBytes(PowerOn))
		return BH1750_OK;

	return BH1750_ERROR;
}

//
//	Set the mode of converting. Look into bh1750_mode enum.
//
BH1750_STATUS BH1750_SetMode(bh1750_mode Mode)
{
	if(!((Mode >> 4) || (Mode >> 5))) return BH1750_ERROR;
	if((Mode & 0x0F) > 3) return BH1750_ERROR;

	Bh1750_Mode = Mode;
//	if(HAL_OK == HAL_I2C_Master_Transmit(bh1750_i2c, BH1750_ADDRESS, &Mode, 1, 10))
	if(BH1750_OK == BH1750_WriteBytes(Mode))
		return BH1750_OK;

	return BH1750_ERROR;
}

//
//	Set the Measurement Time register. It allows to increase or decrease the sensitivity.
//
BH1750_STATUS BH1750_SetMtreg(uint8_t Mtreg)
{
//	HAL_StatusTypeDef retCode;
	if (Mtreg < 31 || Mtreg > 254) {
		return BH1750_ERROR;
	}

	Bh1750_Mtreg = Mtreg;

	uint8_t tmp[2];

	tmp[0] = (0x40 | (Mtreg >> 5));
	tmp[1] = (0x60 | (Mtreg & 0x1F));

//	retCode = HAL_I2C_Master_Transmit(bh1750_i2c, BH1750_ADDRESS, &tmp[0], 1, 10);
//	if (HAL_OK != retCode) {
//		return BH1750_ERROR;
//	}

//	retCode = HAL_I2C_Master_Transmit(bh1750_i2c, BH1750_ADDRESS, &tmp[1], 1, 10);
	if(BH1750_OK == BH1750_WriteBytes(Mtreg))
//	if (HAL_OK == retCode) {
	{
		return BH1750_OK;
	}

	return BH1750_ERROR;
}

//
//	Trigger the conversion in manual modes.
//	For low resolution conversion time is typical 16 ms,
//	for high resolution 120 ms. You need to wait until read the measurement value.
//	There is no need to exit low power mode for manual conversion. It makes automatically.
//
BH1750_STATUS BH1750_TriggerManualConversion(void)
{
	if(BH1750_OK == BH1750_SetMode(Bh1750_Mode))
	{
		return BH1750_OK;
	}
	return BH1750_ERROR;
}

//
//	Read the converted value and calculate the result.
//
float BH1750_ReadLight()
{
	float result;
	uint8_t tmp[2];

//	if(HAL_OK == HAL_I2C_Master_Receive(bh1750_i2c, BH1750_ADDRESS, tmp, 2, 10))
	if(BH1750_OK == BH1750_ReadBytes(tmp))
	{
		result = (tmp[0] << 8) | (tmp[1]);

		if(Bh1750_Mtreg != BH1750_DEFAULT_MTREG)
		{
			result *= (float)((uint8_t)BH1750_DEFAULT_MTREG/(float)Bh1750_Mtreg);
		}

		if(Bh1750_Mode == ONETIME_HIGH_RES_MODE_2 || Bh1750_Mode == CONTINUOUS_HIGH_RES_MODE_2)
		{
			result /= 2.0f;
		}

		result = result / (float)BH1750_CONVERSION_FACTOR;
		return result;
	}
	
	return 0;
}


