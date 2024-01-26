/*
 * STM32_F103C6_USART-Driver.c
 *
 *  Created on: Jul 24, 2023
 *      Author: eslam
 *
 *
 */





#include "../STM32F103C6/includes/STM32_F103C6_USART-Driver.h"
/*
 * =================================================================
 *
 *               			Generic Variables
 *
 * =================================================================
 *
 */

//خلي بالك ان لو استخدمت كذا uart
//مع بعض هيحصل مشكلة بسبب الvarible  ده
// والحل انو يتحول ل array
//عشان ت handle
//لو كذا uartاشتغله مع بعض
USART_Config * Global_USART_Config = NULL;






/*
 * =================================================================
 *
 *               			Generic Macros
 *
 * =================================================================
 *
 */








/*
 * =================================================================
 *
 *					Generic Functions
 *
 * =================================================================
 *
 */








/*
 * =================================================================
 *
 *					APIs
 *
 * =================================================================
 *
 */






#include "../STM32F103C6/includes/STM32_F103C6_USART-Driver.h"

void MCAL_UART_Init(USART_Typedef *USARTx , USART_Config *USART_Config )
{
	uint32_t pclk , BRR;

	Global_USART_Config= USART_Config;


	//enable the clock UART peripheral

	if(USARTx==USART1)
	{
		RCC_USART1_CLK();
	}
	else if (USARTx==USART2)
	{
		RCC_USART2_CLK();

	}
	else if (USARTx==USART3)
	{
		RCC_USART3_CLK();

	}

	//enable UART module
	//Bit 13 UE: USART enable *from (USART_CR1) register

	USARTx->CR1 |=1<<13;

	//enable TX/RX according to the UART_Mode configuration item
	/**from (USART_CR1) register
	 * Bit 2 RE: Receiver enable
	 * Bit 3 TE: Transmitter enable
	 */
	USARTx->CR1 |=USART_Config->USART_Mode;

	//enable Paylaod_Length
	//Bit 12 M: Word length *from (USART_CR1) register

	USARTx ->CR1 |= USART_Config->Paylaod_Length;

	//configuration parity
	//Bit 10 PCE: Parity control enable *from (USART_CR1) register

	USARTx ->CR1 |= USART_Config->Parity;


	//configuration the number of stop bits
	//Bits 13:12 STOP: STOP bits *from (USART_CR2) register

	USARTx ->CR2 |= USART_Config->StopBits;

	//UART Hardware Flow Control
	/*
	 * from (USART_CR2) register
	 *
	 * Bit 9 CTSE: CTS enable
	 *
	 * Bit 8 RTSE: RTS enable
	 */

	USARTx ->CR3 |= USART_Config->HwFlowCtl;

	//Configuration the BoudRate
	// PCLK1 for UASRT 2,3
	// PCLK2 for UASRT 1
	/*
	 * from Baud rate register (USART_BRR)
	 *
	 *
	 */

	if(USARTx== USART1)
	{
		pclk=MCAL_RCC_GetPCLK2Freq();
	}
	else
	{
		pclk=MCAL_RCC_GetPCLK1Freq();
	}

	BRR = UART_BRR_Register(pclk , USART_Config->BuadRate);
	USARTx->BRR = BRR;

	//Enable And Disable Interrupt
	//USART CR1
	if(USART_Config->IRQ_Enable!= UART_IRQ_Enable_NONE)
	{
		USARTx->CR1 |= (USART_Config->IRQ_Enable);
		//Enable NVIC For USARTx IRQ

			if(USARTx==USART1)
			{
				NVIC_IRQ37_USART1_Enable;
			}
			else if(USARTx==USART2)
			{
				NVIC_IRQ38_USART2_Enable;
			}
			else if(USARTx==USART3)
			{
				NVIC_IRQ39_USART3_Enable;
			}
	}


}


void MCAL_UART_DeInit(USART_Typedef *USARTx  )
{

	if(USARTx==USART1)
	{
		RCC_USART1_Reset();
		NVIC_IRQ37_USART1_Disable;
	}
	else if(USARTx==USART2)
	{
		RCC_USART2_Reset();
		NVIC_IRQ38_USART2_Disable;
	}
	else if(USARTx==USART3)
	{
		RCC_USART3_Reset();
		NVIC_IRQ39_USART3_Disable;
	}

}
void MCAL_UART_GPIO_Set_Pins(USART_Typedef *USARTx)
{
	GPIO_PinConfig PinCfg;

	// according to GPIO configurations for device peripherals at page 166
	if(USARTx==USART1)
	{
		//PA9 TX
		//PA10 RX
		//PA11 CTS
		//PA12 RTS


		//PA9 TX
		PinCfg.GPIO_PinNumber = GPIO_PIN_9;
		PinCfg.GPIO_MOOD=GPIO_MOOD_OUTPUT_AF_PP;
		PinCfg.GPIO_Output_speed=GPIO_SPEED_10M;
		MCAL_GPIO_Init(GPIOA , & PinCfg);



		//PA10 TX
		PinCfg.GPIO_PinNumber = GPIO_PIN_10;
		PinCfg.GPIO_MOOD=GPIO_MOOD_AF_INPUT;
		MCAL_GPIO_Init(GPIOA , & PinCfg);

		if(Global_USART_Config->HwFlowCtl== UART_HwFlowCtl_RTSE  ||  Global_USART_Config->HwFlowCtl == UART_HwFlowCtl_CTSE)
		{
			//PA11 RTSE
			PinCfg.GPIO_PinNumber = GPIO_PIN_11;
			PinCfg.GPIO_MOOD=GPIO_MOOD_INPUT_FLO;
			MCAL_GPIO_Init(GPIOA , & PinCfg);

		}

		if(Global_USART_Config->HwFlowCtl== UART_HwFlowCtl_RTSE  ||  Global_USART_Config->HwFlowCtl == UART_HwFlowCtl_CTSE)
		{
			//PA12 RTSE
			PinCfg.GPIO_PinNumber = GPIO_PIN_12;
			PinCfg.GPIO_MOOD=GPIO_MOOD_OUTPUT_AF_PP;
			PinCfg.GPIO_Output_speed=GPIO_SPEED_10M;
			MCAL_GPIO_Init(GPIOA , & PinCfg);


		}
	}

	else if(USARTx==USART2)
	{
		//PA2 TX
		//PA3 RX
		//PA0 CTS
		//PA1 RTS


		//PA2 TX
		PinCfg.GPIO_PinNumber = GPIO_PIN_2;
		PinCfg.GPIO_MOOD=GPIO_MOOD_OUTPUT_AF_PP;
		PinCfg.GPIO_Output_speed=GPIO_SPEED_10M;
		MCAL_GPIO_Init(GPIOA , & PinCfg);



		//PA3 TX
		PinCfg.GPIO_PinNumber = GPIO_PIN_3;
		PinCfg.GPIO_MOOD=GPIO_MOOD_AF_INPUT;
		MCAL_GPIO_Init(GPIOA , & PinCfg);

		if(Global_USART_Config->HwFlowCtl== UART_HwFlowCtl_RTSE  ||  Global_USART_Config->HwFlowCtl == UART_HwFlowCtl_CTSE)
		{
			//PA0 RTSE
			PinCfg.GPIO_PinNumber = GPIO_PIN_0;
			PinCfg.GPIO_MOOD=GPIO_MOOD_INPUT_FLO;
			MCAL_GPIO_Init(GPIOA , & PinCfg);

		}

		if(Global_USART_Config->HwFlowCtl== UART_HwFlowCtl_RTSE  ||  Global_USART_Config->HwFlowCtl == UART_HwFlowCtl_CTSE)
		{
			//PA1 RTSE
			PinCfg.GPIO_PinNumber = GPIO_PIN_1;
			PinCfg.GPIO_MOOD=GPIO_MOOD_OUTPUT_AF_PP;
			PinCfg.GPIO_Output_speed=GPIO_SPEED_10M;
			MCAL_GPIO_Init(GPIOA , & PinCfg);


		}
	}
	else if(USARTx==USART3)
	{
		//PB10 TX
		//PB11 RX
		//PB13 CTS
		//PB14 RTS


		//PB10 TX
		PinCfg.GPIO_PinNumber = GPIO_PIN_10;
		PinCfg.GPIO_MOOD=GPIO_MOOD_OUTPUT_AF_PP;
		PinCfg.GPIO_Output_speed=GPIO_SPEED_10M;
		MCAL_GPIO_Init(GPIOB , & PinCfg);



		//PB11 TX
		PinCfg.GPIO_PinNumber = GPIO_PIN_11;
		PinCfg.GPIO_MOOD=GPIO_MOOD_AF_INPUT;
		MCAL_GPIO_Init(GPIOB , & PinCfg);

		if(Global_USART_Config->HwFlowCtl== UART_HwFlowCtl_RTSE  ||  Global_USART_Config->HwFlowCtl == UART_HwFlowCtl_CTSE)
		{
			//PB13 RTSE
			PinCfg.GPIO_PinNumber = GPIO_PIN_13;
			PinCfg.GPIO_MOOD=GPIO_MOOD_INPUT_FLO;
			MCAL_GPIO_Init(GPIOB , & PinCfg);

		}

		if(Global_USART_Config->HwFlowCtl== UART_HwFlowCtl_RTSE  ||  Global_USART_Config->HwFlowCtl == UART_HwFlowCtl_CTSE)
		{
			//PB14 RTSE
			PinCfg.GPIO_PinNumber = GPIO_PIN_14;
			PinCfg.GPIO_MOOD=GPIO_MOOD_OUTPUT_AF_PP;
			PinCfg.GPIO_Output_speed=GPIO_SPEED_10M;
			MCAL_GPIO_Init(GPIOB , & PinCfg);



		}

	}

}
void MCAL_UART_SendData(USART_Typedef *USARTx ,uint16_t * pTxBuffer ,enum polling_mechanism PollingEn)
{
	//wait until TXE flag is set in Status register
	if (PollingEn == enable)
	{
		while (!(USARTx->SR & 1<<7));
	}

	//check the USART WordLength item for 9BIT or 8BIT in a frame

	/*
	 * When transmitting with the parity enabled (PCE bit set to 1 in the USART_CR1 register),
the value written in the MSB (bit 7 or bit 8 depending on the data length) has no effect
because it is replaced by the parity.
When receiving with the parity enabled, the value read in the MSB bit is the received parity
bit
	 */
	if(Global_USART_Config->Paylaod_Length == UART_Payload_Length_9B)
	{
		USARTx->DR =(*pTxBuffer & (uint16_t)0x01FF);


	}

	else
	{
		USARTx->DR =(*pTxBuffer & (uint16_t)0xFF);
	}
}

void MCAL_UART_ReceiveData(USART_Typedef *USARTx ,uint16_t * pTxBuffer ,enum polling_mechanism PollingEn)
{
	//wait until RXNE flag is set in Status register
	if (PollingEn == enable)
	{
		while (!(USARTx->SR & 1<<5));
	}
	//check the USART WordLength item for 9BIT or 8BIT in a frame
	if(Global_USART_Config->Paylaod_Length == UART_Payload_Length_9B)
	{
		if(Global_USART_Config->Parity==UART_Parity_NONE)
		{

			//no parity So all 9bit are considered data
			*((uint16_t*)pTxBuffer)=USARTx->DR;
		}
		else
		{
			//parity is used so , 8bits will be of user data and 1 bit is parity
			*((uint16_t*)pTxBuffer)=(USARTx->DR & (uint8_t)0xFF);

		}
	}
	else
	{
		if(Global_USART_Config->Parity==UART_Parity_NONE)
		{

			//no parity So all 9bit are considered data
			*((uint16_t*)pTxBuffer)=(USARTx->DR & (uint8_t)0xFF);
		}
		else
		{
			//parity is used so , 7bits will be of user data and 1 bit is parity
			*((uint16_t*)pTxBuffer)=(USARTx->DR & (uint8_t)0x7F);

		}

	}

}
void MCAL_UART_WAIT_TC(USART_Typedef *USARTx)
{
	//this function to user make sure the data is transmitted
	while (!(USARTx->SR & 1<<6));
}



//ISR

void USART1_IRQHandler()
{
	Global_USART_Config->P_IRQ_CallBack();
}


void USART2_IRQHandler()
{
	Global_USART_Config->P_IRQ_CallBack();

}

void USAR3_IRQHandler()
{
	Global_USART_Config->P_IRQ_CallBack();

}

