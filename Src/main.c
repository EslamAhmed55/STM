/*

 * main.c
 *
 *  Created on: Jun 24, 2023
 *      Author: eslam
 */
#if !defined (_SOFT_FP__) && defined(__ARM_FP)
#warning "FPU is not initialize, but the project is compiling for an FPU "
#endif

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>


#include "../STM32F103C6/includes/Stm32_F103C6_GPIO-Driver.h"
#include "../STM32F103C6/includes/Stm32_F103x6.h"
#include "../STM32F103C6/includes/Stm32_F103C6_EXTI-Driver.h"
#include "../STM32F103C6/includes/STM32_F103C6_USART-Driver.h"
#include "STM32_F103C6_SPI-Driver.h"
#include "STM32_F103C6_I2C-Driver.h"
#include "I2C_Slave_EEPROM.h"
#include "lcd.h"
#include "keypad.h"
/*
////////////////////////////////////////////
// LESSON 3 GPIO

void clock_init()
{
	//Enable clock GPIOA
	RCC_GPIOA_CLK();
	//Enable clock GPIOB BIT 3 IOPBEN : TO port B clock enable
	RCC_GPIOB_CLK();
}

void GPIO_init()
{
	GPIO_PinConfig Pincfg;


	Pincfg.GPIO_PinNumber = GPIO_PIN_1;
	Pincfg.GPIO_MOOD = GPIO_MOOD_INPUT_FLO;
	MCAL_GPIO_Init(GPIOA, & Pincfg);

	// PB1 (output PUSH pull Mode)
	//01: output mode , max speed 10 MHZ
	// CNF 00: GEneral purpose output push pull

	Pincfg.GPIO_PinNumber = GPIO_PIN_1;
		Pincfg.GPIO_MOOD = GPIO_MOOD_OUTPUT_PP;
		Pincfg.GPIO_Output_speed = GPIO_SPEED_10M;
		MCAL_GPIO_Init(GPIOB, & Pincfg);
	//==================================================

	//PA13 floating input

		Pincfg.GPIO_PinNumber = GPIO_PIN_13;
		Pincfg.GPIO_MOOD = GPIO_MOOD_INPUT_FLO;
		MCAL_GPIO_Init(GPIOA, & Pincfg);




		//PB13 output pushpull

		Pincfg.GPIO_PinNumber = GPIO_PIN_13;
				Pincfg.GPIO_MOOD = GPIO_MOOD_OUTPUT_PP;
				Pincfg.GPIO_Output_speed = GPIO_SPEED_10M;
				MCAL_GPIO_Init(GPIOB, & Pincfg);


}


void wait_ms (uint32_t time )
{
	uint32_t i,j;
	for(i =0 ;i<time ; i++)
		for(j=0 ; j<255 ; j++);
}

int main(void)
{
	clock_init();
	GPIO_init();

	while(1)
	{
	if (MCAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1)==0)
	{
		MCAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
		while (MCAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1)==0);
	}

	if (MCAL_GPIO_ReadPin(GPIOA, GPIO_PIN_13)==1)
		{
		MCAL_GPIO_TogglePin(GPIOB, GPIO_PIN_13);
		}
	wait_ms(1);
}
}

 */
/*
//////////////////////////////////////////////////////////
//LESSON 4 Interrupt

void wait_ms (uint32_t time )
{
	uint32_t i,j;
	for(i =0 ;i<time ; i++)
		for(j=0 ; j<255 ; j++);
}



 int main(void)
 {//Enable clock
	 RCC_GPIOA_CLK();
	 RCC_GPIOB_CLK();
	 //دي باين نسيت اكتبها
	RCC_AFIO_CLK();



 }
 */



//LESSON1 UART DONE
/*
void wait_ms (uint32_t time )
{
	uint32_t i,j;
	for(i =0 ;i<time ; i++)
		for(j=0 ; j<255 ; j++);
}

void clock_init()
{
	//Enable clock GPIOA
	RCC_GPIOA_CLK();
	//Enable clock GPIOB BIT 3 IOPBEN : TO port B clock enable
	RCC_GPIOB_CLK();
	RCC_AFIO_CLK();
}


unsigned char ch ;


void UART_interrupt_callback(void)
{
	MCAL_UART_ReceiveData(USART1, &ch,disable);
	MCAL_UART_SendData(USART1, &ch, enable);
}

void main()
{
	USART_Config uartCFG;
	clock_init();
	uartCFG.BuadRate=UART_BuadRate_115200;
	uartCFG.HwFlowCtl=UART_HwFlowCtl_NONE;
	uartCFG.IRQ_Enable = UART_IRQ_Enable_NONE;
	uartCFG.P_IRQ_CallBack=NULL;
	uartCFG.Parity=UART_Parity_NONE;
	uartCFG.Paylaod_Length=UART_Payload_Length_8B;
	uartCFG.StopBits= UART_StopBits_1;
	uartCFG.USART_Mode=UART_Mode_TX_Rx;
	MCAL_UART_Init(USART1, &uartCFG);
	MCAL_UART_GPIO_Set_Pins(USART1);



	while(1)
	{
		MCAL_UART_ReceiveData(USART1, &ch,enable);
		MCAL_UART_SendData(USART1, &ch, enable);



	}
}


 */


//Lesson SPI

//#define MCU_Act_As_Master

#define MCU_Act_As_Slave

void clock_init()
{
	//Enable clock GPIOA
	RCC_GPIOA_CLK();
	//Enable clock GPIOB BIT 3 IOPBEN : TO port B clock enable
	RCC_GPIOB_CLK();
	RCC_AFIO_CLK();
}


unsigned char ch ;

void SPI1_IRQ_callback(struct S_IRQ_SRC irq_src)
{
#ifdef MCU_Act_As_Slave
	if(irq_src.RXNE)
	{
		ch=0xf;
		MCAL_SPI_TX_RX(SPI1, &ch, Pollingdisable);
		MCAL_UART_SendData(USART1, &ch, enable);
	}

#endif
}

void UART_interrupt_callback(void)
{
#ifdef MCU_Act_As_Master

S	MCAL_UART_ReceiveData(USART1, &ch,disable);
	MCAL_UART_SendData(USART1, &ch, enable);

	//send to SPI
	MCAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 0);

	MCAL_SPI_TX_RX(SPI1, &ch, Pollingenable);

	MCAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 1);


#endif

}

void main()
{

	GPIO_PinConfig Pincfg;

	/********UART INIT**************/
	USART_Config uartCFG;
	clock_init();
	uartCFG.BuadRate=UART_BuadRate_115200;
	uartCFG.HwFlowCtl=UART_HwFlowCtl_NONE;
	uartCFG.IRQ_Enable = UART_IRQ_Enable_RXNEIE;
	uartCFG.P_IRQ_CallBack=UART_interrupt_callback;
	uartCFG.Parity=UART_Parity_NONE;
	uartCFG.Paylaod_Length=UART_Payload_Length_8B;
	uartCFG.StopBits= UART_StopBits_1;
	uartCFG.USART_Mode=UART_Mode_TX_Rx;
	MCAL_UART_Init(USART1, &uartCFG);
	MCAL_UART_GPIO_Set_Pins(USART1);


	/********SPI Init*************/
	//	PA4  :  SPI1_NSS
	//	PA5  :  SPI1_SCK
	//	PA6  :  SPI1_MISO
	//	PA7  :  SPI1_MOSI

	SPI_Config SPI1_CFG;

	//common configuration
	SPI1_CFG.CLKPhase=SPI_Clock_Phase_2EDGE_frist_data_sampling_edge;
	SPI1_CFG.CLKPolarity=SPI_CLKPolarity_HIGH_when_idle;
	SPI1_CFG.DataSize=SPI_DataSize_8BIT;
	SPI1_CFG.Frame_Format=SPI_Frame_Format_MSB_transmitted_first;
	//by defualt pclk2=8MHZ
	SPI1_CFG.SPI_BAUDRATEPRESCALER=SPI_BAUDRATEPRESCALER_8;
	SPI1_CFG.communication_Mode=SPI_DIRECTION_2LLINES;

#ifdef MCU_Act_As_Master
	SPI1_CFG.Device_Mode=SPI_Device_Mode_MASTER;
	SPI1_CFG.IRQ_Enable=SPI_IRQ_Enable_NONE;
	SPI1_CFG.NSS=SPI_NSS_Soft_NSSInternalSoft_Set;
	SPI1_CFG.P_IRQ_CallBack=NULL;


	//configure slave select (NSS) on PORTA PIN4 on GPIO

		Pincfg.GPIO_PinNumber = GPIO_PIN_4;
		Pincfg.GPIO_MOOD= GPIO_MOOD_OUTPUT_PP;
		Pincfg.GPIO_Output_speed=GPIO_SPEED_10M;
		MCAL_GPIO_Init(GPIOA,&Pincfg);

		//force slave select high idle mode
		MCAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 1);
#endif


#ifdef MCU_Act_As_Slave

		SPI1_CFG.Device_Mode=SPI_Device_Mode_SLAVE;
			SPI1_CFG.IRQ_Enable=SPI_IRQ_Enable_RXNEIE;
			SPI1_CFG.NSS=SPI_NSS_Hard_Slave;
			SPI1_CFG.P_IRQ_CallBack=SPI1_IRQ_callback;


#endif

	MCAL_SPI_Init(SPI1, &SPI1_CFG);
	MCAL_SPI_GPIO_Set_Pins(SPI1);




	while(1)
	{



	}
}



/*
// Lesson I2C


void main(void)
{
	//Enable clock
	RCC_GPIOA_CLK();
	RCC_GPIOB_CLK();
	RCC_AFIO_CLK();


	//test case 1
	unsigned char ch1[]={ 0x1,0x2,0x3,0x4,0x5,0x6,0x7};
	unsigned char ch2[7]={0};



	EEPROM_Init();
	EEPROM_Write_MBytes(0xAF, ch1, 7);
	EEPROM_Read_Bytes(0xAF, ch2, 7);

	//test case 2

	ch1[0]=0xA;
	ch1[1]=0xB;
	ch1[2]=0xC;
	ch1[3]=0xD;

		EEPROM_Write_MBytes(0xAF, ch1, 4);
		EEPROM_Read_Bytes(0xAF, ch2, 4);

}



 */



