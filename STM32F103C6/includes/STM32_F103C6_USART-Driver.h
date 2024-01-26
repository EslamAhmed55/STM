/*
 * STM32_F103C6_USART-Driver.h
 *
 *  Created on: Jul 24, 2023
 *      Author: eslam
 */

#ifndef INCLUDES_STM32_F103C6_USART_DRIVER_H_
#define INCLUDES_STM32_F103C6_USART_DRIVER_H_
#include "../../STM32F103C6/includes/Stm32_F103C6_EXTI-Driver.h"
#include "../../STM32F103C6/includes/Stm32_F103C6_GPIO-Driver.h"
#include "../../STM32F103C6/includes/Stm32_F103x6.h"
#include "../../STM32F103C6/includes/STM32_F103C6_RCC-Driver.h"

//Configuration Structure

typedef struct
{
	uint8_t			USART_Mode; //Specifies TX/RX Enable or Disable
								//this parameter must be set based on reference UART_Mode_define


	uint32_t 		BuadRate ;   //This member configuration the UART communication BoudRate
								//this parameter must be set based on reference UART_BoudRate_define


	uint8_t			Paylaod_Length; //Specifies the number of data bits transmitted or received in a fram
								  //this parameter must be set based on reference UART_Paylaod_Length_define


	uint8_t			Parity;			//Specifies Parity mode
									//reference UART_Parity_define


	uint8_t			StopBits;		//Specifies Stop Bits mode
									//reference UART_StopBits_define

	uint8_t 		HwFlowCtl;		//Specifies Whether the hardware flow control mode is enable or disable
									//reference UART_HwFlowCtl_define

	uint8_t 		IRQ_Enable;		//enable or disable interrupt TX/RX
									// reference UART_IRQ_Enable_define , you can select two or three parameter EX.UART_IRQ_Enable_TXE | UART_IRQ_Enable_TC


	void(*P_IRQ_CallBack)(void) ;    //Set the C function() which will be called once the IRQ Happen


}USART_Config;

/*
 * =================================================================
 *
 *               		Reference Macros
 *
 * =================================================================
 *
 */

//reference UART_Mode_define

#define UART_Mode_Rx				(uint32_t)(1<<2)   //Bit 2 RE: Receiver enable
#define UART_Mode_Tx				(uint32_t)(1<<3)  //Bit 3 TE: Transmitter enable
#define UART_Mode_TX_Rx				((uint32_t)(1<<2 | 1<<3))

//reference UART_BoudRate_define
/* From Table 192. Error calculation for programmed baud rates at page 799*/

#define UART_BuadRate_2400				2400
#define UART_BuadRate_9600				9600
#define UART_BuadRate_19200				19200
#define UART_BuadRate_57600				57600
#define UART_BuadRate_115200			115200
#define UART_BuadRate_230400			230400
#define UART_BuadRate_460800			460800
#define UART_BuadRate_921600			921600
#define UART_BuadRate_2250000			2250000
#define UART_BuadRate_4500000			4500000

//reference UART_Paylaod_Length_define

/*from (USART_CR1) */
/*
 * Bit 12 M: Word length
This bit determines the word length. It is set or cleared by software.
0: 1 Start bit, 8 Data bits, n Stop bit
1: 1 Start bit, 9 Data bits, n Stop bit
Note: The M bit must not be modified during a data transfer (both transmission and reception)
 */

#define UART_Payload_Length_8B					(uint32_t)(0)
#define UART_Payload_Length_9B					(uint32_t)(1<<12)


//reference UART_Parity_define

/*from (USART_CR1) */
/*
 * Bit 10 PCE: Parity control enable
This bit selects the hardware parity control (generation and detection). When the parity
control is enabled, the computed parity is inserted at the MSB position (9th bit if M=1; 8th bit
if M=0) and parity is checked on the received data. This bit is set and cleared by software.
Once it is set, PCE is active after the current byte (in reception and in transmission).
0: Parity control disabled
1: Parity control enabled
 */

/*
 * Bit 9 PS: Parity selection
This bit selects the odd or even parity when the parity generation/detection is enabled (PCE
bit set). It is set and cleared by software. The parity will be selected after the current byte.
0: Even parity
1: Odd parity
 */
#define UART_Parity_NONE					(uint32_t)(0)
#define UART_Parity_EVEN					(uint32_t)(1<<10)
#define UART_Parity_ODD					   (uint32_t)(1<<10 | 1<<9)


//reference UART_StopBits_define
/*from (USART_CR2) */

/*
 * Bits 13:12 STOP: STOP bits
These bits are used for programming the stop bits.
00: 1 Stop bit
01: 0.5 Stop bit
10: 2 Stop bits
11: 1.5 Stop bit
 */

#define UART_StopBits_half				(uint32_t)(1<<12)
#define UART_StopBits_1					(uint32_t)(0)
#define UART_StopBits_1_half			(uint32_t)(3<<12) // set 1 in bit 12 and bit 13
															//So 3= 0b 11
#define UART_StopBits_2					(uint32_t)(2<<12)


//reference UART_	_define
/*from (USART_CR3) */
/*
 * Bit 9 CTSE: CTS enable
0: CTS hardware flow control disabled
1: CTS mode enabled, data is only transmitted when the CTS input is asserted (tied to 0). If
the CTS input is deasserted while a data is being transmitted, then the transmission is
completed before stopping. If a data is written into the data register while CTS is deasserted,
the transmission is postponed until CTS is asserted.
This bit is not available for UART4 & UART5.
 *
 */

/*Bit 8 RTSE: RTS enable
0: RTS hardware flow control disabled
1: RTS interrupt enabled, data is only requested when there is space in the receive buffer.
The transmission of data is expected to cease after the current character has been
transmitted. The RTS output is asserted (tied to 0) when a data can be received.
This bit is not available for UART4 & UART5
*/

#define UART_HwFlowCtl_NONE					(uint32_t)(0)
#define UART_HwFlowCtl_CTSE					(uint32_t)(1<<8)
#define UART_HwFlowCtl_RTSE					(uint32_t)(1<<9)
#define UART_HwFlowCtl_CTSE_RTSE			((uint32_t)(1<<8 | 1<<9))


// reference UART_IRQ_Enable_define
/*from USART interrupts --> Table 196. USART interrupt requests Page 816 */

/*
 * AND WE WILL USE THIS INTRRUPT
 *
 * 1-Transmit data register empty
 * 2-Transmission complete
 * 3-Received data ready to be read
 * 4-Overrun error detected
 * 5-Parity error
 *
 */
#define UART_IRQ_Enable_NONE				(uint32_t)(0)
#define UART_IRQ_Enable_TXE					(uint32_t)(1<<7)//Transmit data register empty
#define UART_IRQ_Enable_TC					(uint32_t)(1<<6)//Transmission complete
#define UART_IRQ_Enable_RXNEIE				(uint32_t)(1<<5)//Received data ready to be read
#define UART_IRQ_Enable_PE					(uint32_t)(1<<8)//Parity error


// BuadRate Calculations
#define USARTDIV(_PCLK_,_BAUD_)						(uint32_t)(_PCLK_/(16 * _BAUD_))
#define USARTDIV_MUL100(_PCLK_,_BAUD_)				(uint32_t)((25* _PCLK_)/(4 * _BAUD_))
#define Mantissa_MUL100(_PCLK_,_BAUD_)				(uint32_t)(USARTDIV(_PCLK_,_BAUD_) *100)
#define Mantissa(_PCLK_,_BAUD_)						(uint32_t)(USARTDIV(_PCLK_,_BAUD_))
#define DIV_Fraction(_PCLK_,_BAUD_)					(uint32_t)(((USARTDIV_MUL100(_PCLK_,_BAUD_) - Mantissa(_PCLK_,_BAUD_) ) *16)/100)
#define UART_BRR_Register(_PCLK_,_BAUD_)			(uint32_t)((Mantissa(_PCLK_,_BAUD_) <<4)|(DIV_Fraction(_PCLK_,_BAUD_))&0xF)


//Polling_mechanism
enum polling_mechanism
{
	enable,
	disable
};






//==========================================================================
//____________________________________________________________
//                APIS Supported by " MCAL USART DRIVER"
//____________________________________________________________

void MCAL_UART_Init(USART_Typedef *UARTx , USART_Config *USART_Config );
void MCAL_UART_DeInit(USART_Typedef *UARTx  );
void MCAL_UART_GPIO_Set_Pins(USART_Typedef *UARTx);
void MCAL_UART_SendData(USART_Typedef *UARTx ,uint16_t * pTxBuffer ,enum polling_mechanism PollingEn);
void MCAL_UART_ReceiveData(USART_Typedef *UARTx ,uint16_t * pTxBuffer ,enum polling_mechanism PollingEn);
void MCAL_UART_WAIT_TC(USART_Typedef *UARTx);
#endif /* INCLUDES_STM32_F103C6_USART_DRIVER_H_ */
