/*
 * STM32_F103C6_I2C-Driver.h
 *
 *  Created on: Jul 31, 2023
 *      Author: eslam
 */

#ifndef INCLUDES_STM32_F103C6_I2C_DRIVER_H_
#define INCLUDES_STM32_F103C6_I2C_DRIVER_H_

#include "../../STM32F103C6/includes/Stm32_F103C6_GPIO-Driver.h"
#include "../../STM32F103C6/includes/STM32_F103C6_RCC-Driver.h"

struct I2C_Slave_Device_Address_t
{
										// This parameter must be set based on @ref Dual_Address_Mode_DEFINE
		uint32_t Dual_Address_Mode; // 1- Enable   0- Disable

		uint8_t  Primary_Slave_Address;
		uint8_t	 Secondary_Slave_Address;

		// This parameter must be set based on @ref I2C_Addressing_Slave_MODE_DEFINE
		uint32_t Addressing_Slave_Mode;
};

typedef enum
{
	I2C_EV_STOP,
	I2C_EV_ADD_MATCHED,
	I2C_EV_DATA_REQ,	// APP_Layer should send data (I2C slave send data)
	I2C_EV_DATA_RCV	,	// APP_Layer should receive data (I2C slave receive data)
	I2C_ERROR_AF

}Slave_State; // this enum according I2C interrupt table (page 770)

typedef struct
{
	   uint32_t 		 I2C_Clock_Speed;						// Specifies I2C clock speed
													// This parameter must be set based on @ref I2C_CLOCK_SPEED_DEFINE

		uint32_t		 Stretch_Mode;						// specifies Enable or Disable clock stretching in slave mode only
													// This parameter must be set based on @ref I2C_STRETCH_MODE_DEFINE

		uint32_t 		I2C_Mode;								// specifies I2C SMBUS mode or I2C mode
															// This parameter must be set based on @ref I2C_MODE_DEFINE

		struct I2C_Slave_Device_Address_t	I2C_Slave_Address;			// Slave address information from user


		uint32_t 		I2C_ACK_Control;						// Enable ACK by Hardware or Disable ACK
															// This parameter must be set based on @ref I2C_ACK_CONTROL_DEFINE


		uint32_t 		General_Call_Adderss_Detection;						// specifies Enable or Disable General_Call
													// This parameter must be set based on @ref I2C_GENERAL_CALL_DEFINE

		uint32_t 		Master_Mode;			 			// specifies I2C Master mode SM mode or FM mode
													// This parameter must be set based on @ref I2C_MASTER_MODE_DEFINE





		uint8_t 		IRQ_Enable;                         // specifies I2C Interrupt Requests
		                                            // This parameter must be set based on @ref IRQ_ENABLE_DEFINE

		void(*P_Slave_Event_CallBack)(Slave_State state); // Set the C Function which will be called once IRQ Happens
}I2C_InitTypedef;





/*
 * =================================================================
 *
 *               		Reference Macros
 *
 * =================================================================
 *
 */


/*********************** @ref Dual_Address_Mode_DEFINE ************************/
#define Dual_Address_Mode_Enable                       1U
#define Dual_Address_Mode_Disable                      0U

//Reference I2C_CLOCK_SPEED_DEFINE

//form TRM page 752
/*• Supports different communication speeds:
– Standard Speed (up to 100 kHz)
– Fast Speed (up to 400 kHz)
*/
// to configure clock
/*
Bits 5:0 FREQ[5:0]: Peripheral clock frequency
Configure the clock control registers:
					T(high)= CCR* T(PCLK1)  -->page 782
					configure mood : SM or FM

• Configure the rise time register -> I2CTRISE register (page 782)
 */

#define I2c_SCLK_SM_50K						(0x50000U)
#define I2c_SCLK_SM_100K					(0x100000U)
#define I2c_SCLK_SM_200K					(0x200000U) //fast mood not supported yet
#define I2c_SCLK_SM_400K					(0x400000U)//fast mood not supported yet


//Reference I2C_STRETCH_MODE_DEFINE
//I2c_CR1
/*
 * Bit 7 NOSTRETCH: Clock stretching disable (Slave mode)
 * 0: Clock stretching enabled
 * 1: Clock stretching disabled
 */

#define I2C_Stretch_Mode_Enable				   (0x00000000U)
#define I2C_Stretch_Mode_Disable				I2C_CR1_NOSTRETCH



//Reference I2C_MODE_DEFINE
//I2C_CR1
/*
 * Bit 1 SMBUS: SMBus mode
0: I2C mode
1: SMBus mode
 */

#define  I2C_mode_I2C     				0
#define  I2C_mode_SMBus     				I2C_CR1_SMBUS


//Reference I2C_Addressing_Slave_MODE_DEFINE

//I2C_OAR1 ->(page776)
/*
 * Bit 15 ADDMODE Addressing mode (slave mode)
0: 7-bit slave address (10-bit address not acknowledged)
1: 10-bit slave address (7-bit address not acknowledged)
 */

#define I2C_Addressing_Slave_mode_7Bit						(0x00000000)

#define I2C_Addressing_Slave_mode_10Bit						(uint16_t)(1<<15) // i do not define this register  in bit filed macros at "Stm32_F103x6.h"


//Reference I2C_ACK_CONTROL_DEFINE
/*
 * Bit 10 ACK: Acknowledge enable
This bit is set and cleared by software and cleared by hardware when PE=0.
0: No acknowledge returned
1: Acknowledge returned after a byte is received (matched address or data)
 */

#define I2C_ACK_Enable 								(I2C_CR1_ACK)
#define I2C_ACK_Disable 								((uint16_t)0x0000)

//Reference I2C_GENERAL_CALL_DEFINE (When you need send message or any thing to All addresses )

//I2C CR1

/*Bit 6 ENGC: General call enable
0: General call disabled. Address 00h is NACKed.
1: General call enabled. Address 00h is ACKed.
 *
 */

#define I2C_ENGC_Enable					I2C_CR1_ENGC
#define I2C_ENGC_Disable					0x00000000U



#define I2C_EVENT_MASTER_BYTE_TRANSMITTING		((uint32_t)0x00070080) // this method macro to concatenate (SR1 & SR2) to SET ( TRA , BUSY , MSL , TXE flags) in one step

//=======================================================================================
typedef enum
{
	with_Stop,
	without_stop

}Stop_Condition;


typedef enum
{
	start, // first time to check the bus busy or idle ------> to catch it
	repeated_start // i am already  catch the bus and i do not need to check if the bus busy or not ( for examle --> Read Flash Memory)
}Repeated_Start;


typedef enum{ DISABLE=0, ENABLE=1 }FunctionalState;

typedef enum{ RESET=0, SET=1 }FlagStatus;

typedef enum
{
	I2C_FLAG_BUSY=0,
	EV5 ,//EV5: SB=1, cleared by reading SR1 register followed by writing DR register with Address.
	EV6,//EV6: ADDR=1, cleared by reading SR1 register followed by reading SR2.
	EV7,//
	EV8, //EV8: TxE=1, shift register not empty, d. ata register empty, cleared by writing DR register
	EV8_1,//EV8_1: TxE=1, shift register empty, data register empty, write Data1 in DR.

	MASTER_BYTE_TRANSMITTING = ((uint32_t)0x00070080) //check( TRA , BUSY , MSL , TXE flags)

}Status;



typedef enum{ I2C_Direction_Transmitter=0, I2C_Direction_Reciever=1 }I2C_Direction;// I2C_Direction is a command to send what i need (Write or Read)


//____________________________________________________________
//                APIS Supported by " MCAL SPI DRIVER"
//____________________________________________________________


void    MCAL_I2C_Init  (I2C_Typedef* I2Cx, I2C_InitTypedef *I2C_InitStruct);
void    MCAL_I2C_DeInit(I2C_Typedef* I2Cx);

void    MCAL_I2C_GPIO_Set_Pins(I2C_Typedef* I2Cx);
//master polling mechanism

/* 1-select I2C  2- I2C Slave Address 3- pointer on Buffer 4- Lenth of Buffer 5- Enable or Disable Stop condition (Disable Stop in (Repeat condition)) 6- Start condition ( TO check the busy of Bus ) for example  in Repeat start   */

void    MCAL_I2C_MASTER_TX(I2C_Typedef* I2Cx, uint16_t Device_Address, uint8_t *dataOut, uint8_t Data_Length, Stop_Condition Stop, Repeated_Start Start);
void    MCAL_I2C_MASTER_RX(I2C_Typedef* I2Cx, uint16_t Device_Address, uint8_t *dataOut, uint8_t Data_Length, Stop_Condition Stop, Repeated_Start Start);

// Salve interrupt mechanism
void    MCAL_I2C_SlaveSendData(I2C_Typedef* I2Cx, uint8_t data);
uint8_t MCAL_I2C_SlaveRecieveData(I2C_Typedef* I2Cx);

/*-------------------------------------Generic APIs----------------------------------------------*/

void I2C_GenerateSTART(I2C_Typedef* I2Cx , FunctionalState NewState , Repeated_Start start);

FlagStatus I2C_GETFlagStatus(I2C_Typedef *I2Cx, Status flag);

void I2C_SendAddress(I2C_Typedef* I2Cx,uint16_t Address, I2C_Direction Direction);

void I2c_GenerateSTOP(I2C_Typedef* I2Cx , FunctionalState NewState );


void I2C_AcknowledgeConFig(I2C_Typedef* I2Cx, FunctionalState NewState);
#endif /* INCLUDES_STM32_F103C6_I2C_DRIVER_H_ */
