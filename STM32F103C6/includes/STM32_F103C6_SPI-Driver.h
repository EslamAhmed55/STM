/*
 * STM32_F103C6_SPI-Driver.h
 *
 *  Created on: Jul 27, 2023
 *      Author: eslam
 */

#ifndef INCLUDES_STM32_F103C6_SPI_DRIVER_H_
#define INCLUDES_STM32_F103C6_SPI_DRIVER_H_
// includes
#include "../../STM32F103C6/includes/Stm32_F103C6_GPIO-Driver.h"
#include "../../STM32F103C6/includes/Stm32_F103x6.h"



//configuration register

struct S_IRQ_SRC
{
	uint8_t TXE:1;           //1-receive buffer not empty

	uint8_t RXNE:1;			//2-transmit buffer is empty
	uint8_t ERRI:1;			//Error interrupt is Occured
	uint8_t Reserved:5;

};

typedef struct
{
	uint16_t 	Device_Mode	;					//Specifies the SPI operation mode @reference SPI_Device_Mode
	uint16_t 	communication_Mode;				//Specifies the SPI Bidirectional  mode state  @reference SPI_communication_Mode
	uint16_t 	Frame_Format;					//Specifies LSB or MSB @reference SPI_Frame_Format
	uint16_t 	DataSize	;					//Specifies SPI_DataSize
	uint16_t 	CLKPolarity;					//Specifies SPI_CLKPolarity
	uint16_t 	CLKPhase;						//Specifies SPI_CLKPhase
	uint16_t    NSS;							/*Specifies whether the NSS signal is managed by
												Hardware (NSS Pin) or by Software using the SSI bit_enable
												@ reference  SPI_NSS
												*/

	uint16_t   SPI_BAUDRATEPRESCALER;			/* Specifies The Baud Rate Prescaler value which will be
												used to configure the transmit and receive SCK clock
												this parameter can be a value of @ reference SPI_BAUDRATEPRESCALER
												@note the communication clock is derived from master clock.
												the slave clock does not need to be set
												take care you have to configure RCC to enter the correct clock to ABB2 >> to SPI1
												and ABB1 >>>to SPI2

												*/

	 uint16_t 	IRQ_Enable;						//reference SPI_IRQ_Enable

	 void (*P_IRQ_CallBack)(struct S_IRQ_SRC  irq_src);				//set the C Function which will be called  one IRQ Happen
	 	 	 	 	 	 	 	 	 	 	 	/*
	 	 	 	 	 	 	 	 	 	 	 	 * the case will generate interrupt
	 	 	 	 	 	 	 	 	 	 	 	 * 1-receive buffer not empty
	 	 	 	 	 	 	 	 	 	 	 	 * 2-transmit buffer is empty
	 	 	 	 	 	 	 	 	 	 	 	 * 3- Error interrupt is Occured
	 	 	 	 	 	 	 	 	 	 	 	 *
	 	 	 	 	 	 	 	 	 	 	 	 */

}SPI_Config;

/*
 * =================================================================
 *
 *               		Reference Macros
 *
 * =================================================================
 *
 */


//reference SPI_Device_Mode

// Regiser CR1 -> bit:MSTR
#define SPI_Device_Mode_SLAVE			(0x00000000U)

#define SPI_Device_Mode_MASTER			(0x1U<<2)


//reference SPI_communication_Mode

//CR1 Register
#define SPI_DIRECTION_2LLINES										(0x00000000U)//SPI Full duplex
#define SPI_DIRECTION_2LLINES_RXONLY 								(0x1U<<10)   //Receive only - bit 10
#define SPI_DIRECTION_1LLINES_receive_only							(0x1U<<15)	//Receive only one line - bit 15
#define SPI_DIRECTION_1LLINES_transmit_only							((0x1U<<15)|(0x1U<<14))	//Transmit only one line - bit 15	and bit 14

//SPI_DataSize


#define SPI_DataSize_8BIT											(0x00000000U)

#define SPI_DataSize_16BIT											(0x1U << 11)


//reference SPI_Frame_Format

#define SPI_Frame_Format_MSB_transmitted_first						(0x00000000U)
#define SPI_Frame_Format_LSB_transmitted_first						(0x1U << 7)

//SPI_CLKPolarity

#define SPI_CLKPolarity_LOW_when_idle								(0x00000000U)
#define SPI_CLKPolarity_HIGH_when_idle								(0x1U<<1)


//SPI_CLKPhase

#define SPI_Clock_Phase_1EDGE_frist_data_sampling_edge								(0x00000000U)
#define SPI_Clock_Phase_2EDGE_frist_data_sampling_edge								(0x1U<<0)

//SPI_NSS  SPI Slave Select Management


// NSS IS Driven Hardware

#define  SPI_NSS_Hard_Slave									(0x00000000U)

#define  SPI_NSS_Hard_Master_Slave_Select_output_enable		(0x1U<<2) //SPI_CR2 ->  bit2=SSOE :SS out enable
#define  SPI_NSS_Hard_Master_Slave_Select_output_disable	~(0x1U<<2) //SPI_CR2 ->  bit2=SSOE :SS out disable

// NSS IS Driven Software(master or salve)
#define  SPI_NSS_Soft_NSSInternalSoft_Reset					(0x1U<<9)
#define  SPI_NSS_Soft_NSSInternalSoft_Set					((0x1U<<9)|(0x1U<<8))


//reference SPI_BAUDRATEPRESCALER

#define	SPI_BAUDRATEPRESCALER_2								(0x00000000U)
#define	SPI_BAUDRATEPRESCALER_4								(0b001U<<3)
#define	SPI_BAUDRATEPRESCALER_8								(0b010U<<3)
#define	SPI_BAUDRATEPRESCALER_16							(0b011U<<3)
#define	SPI_BAUDRATEPRESCALER_32							(0b100U<<3)
#define	SPI_BAUDRATEPRESCALER_64							(0b101U<<3)
#define	SPI_BAUDRATEPRESCALER_128							(0b110U<<3)
#define	SPI_BAUDRATEPRESCALER_256							(0b111U<<3)





//reference SPI_IRQ_Enable

//CR2 n  Register


#define SPI_IRQ_Enable_NONE										(uint32_t)(0)
#define SPI_IRQ_Enable_TXEIE									(uint32_t)(1<<7)  // 1-receive buffer not empty
#define SPI_IRQ_Enable_RXNEIE									(uint32_t)(1<<6)	//2-transmit buffer is empty
#define SPI_IRQ_Enable_ERRIE									(uint32_t)(1<<5)   //3- Error interrupt is Occured

//Polling_mechanism
enum Polling_mechanism
{
	Pollingenable,
	Pollingdisable
};

//____________________________________________________________
//                APIS Supported by " MCAL SPI DRIVER"
//____________________________________________________________

void MCAL_SPI_Init(SPI_Typedef *SPIx , SPI_Config *SPI_Config );
void MCAL_SPI_DeInit(SPI_Typedef *SPIx);
void MCAL_SPI_Set_Pins(SPI_Typedef *SPIx);
void MCAL_SPI_SendData(SPI_Typedef *SPIx ,uint16_t *pTxBuffer, enum Polling_mechanism  PollinfEn );
void MCAL_SPI_ReceiveData(SPI_Typedef *SPIx ,uint16_t *pTxBuffer, enum Polling_mechanism  PollinfEn );
void MCAL_SPI_TX_RX(SPI_Typedef *SPIx ,uint16_t *pTxBuffer, enum Polling_mechanism  PollinfEn );
void MCAL_SPI_GPIO_Set_Pins(SPI_Typedef *SPIx);

#endif /* INCLUDES_STM32_F103C6_SPI_DRIVER_H_ */
