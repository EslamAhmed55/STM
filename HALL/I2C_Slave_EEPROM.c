

#include "I2C_Slave_EEPROM.h"



void EEPROM_Init(void)
{
	// I2C_init
	// PB6: I2C1_SCL
	// PB7: I2C1_SDA

	I2C_InitTypedef I2C1CFG;

	// I2C Controller act as a master
	I2C1CFG.General_Call_Adderss_Detection = I2C_ENGC_Enable;
	I2C1CFG.I2C_ACK_Control = I2C_ACK_Enable;
	I2C1CFG.I2C_Clock_Speed = I2c_SCLK_SM_100K;
	I2C1CFG.I2C_Mode = I2C_mode_I2C;
	I2C1CFG.P_Slave_Event_CallBack = NULL;
	I2C1CFG.Stretch_Mode = I2C_Stretch_Mode_Enable;

	MCAL_I2C_GPIO_Set_Pins(I2C1);
	MCAL_I2C_Init(I2C1, &I2C1CFG);

}



unsigned char EEPROM_Write_MBytes(unsigned int Memory_Address, unsigned char *bytes,uint8_t DataLength)
{
	uint8_t i=0;

	uint8_t buffer[256];

	buffer[0] = (uint8_t)(Memory_Address >> 8);			// Upper byte memory address
	buffer[1] = (uint8_t)(Memory_Address);				// Lower byte memory address

	for(i=2;i<(DataLength+2);i++)
	{
		buffer[i] = bytes[i-2];
	}
	MCAL_I2C_MASTER_TX(I2C1, EEPROM_Slave_Address, buffer, DataLength+2, with_Stop, start);

	return 0;
}



unsigned char EEPROM_Read_Bytes(unsigned int Memory_Address, uint8_t *data_out,uint8_t DataLength)
{
	unsigned char buffer[2];
	buffer[0] = (uint8_t)(Memory_Address >> 8);			// Upper byte memory address
	buffer[1] = (uint8_t)(Memory_Address);				// Lower byte memory address


	// Write address only
	MCAL_I2C_MASTER_TX(I2C1, EEPROM_Slave_Address,& buffer, 2, without_stop, start);

	// Read Data
	MCAL_I2C_MASTER_RX(I2C1, EEPROM_Slave_Address, data_out, DataLength, with_Stop, repeated_start);


	return 0;


}
