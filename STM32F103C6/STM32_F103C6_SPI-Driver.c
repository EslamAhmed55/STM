/*
 * STM32_F103C6_SPI-Driver.c
 *
 *  Created on: Jul 27, 2023
 *      Author: eslam
 */

#include "../STM32F103C6/includes/STM32_F103C6_SPI-Driver.h"

#include "../STM32F103C6/includes/Stm32_F103C6_GPIO-Driver.h"
/*
 * =================================================================
 *
 *               			Generic Variables
 *
 * =================================================================
 *
 */

// TO share SPI configuration with anther APIS
SPI_Config  *Global_SPI_Comfig[2] = {NULL,NULL}; // index 0 -> SPI1 / index 1 -> SPI2

SPI_Config  Global_SPI_Comfig1;
SPI_Config  Global_SPI_Comfig2;






/*
 * =================================================================
 *
 *               			Generic Macros
 *
 * =================================================================
 *
 */


#define SPI1_INDEX		0

#define SPI2_INDEX		1



#define SPI_SR_TXE		((uint8_t)0x02)
#define SPI_SR_RXNE		((uint8_t)0x01)




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

void MCAL_SPI_Init(SPI_Typedef *SPIx , SPI_Config *SPI_Config )
{
	//Safety for register
	uint16_t tmpreg_CR1=0; // to save Run SPI Before Complete Configuration of SPI Registers
	uint16_t tmpreg_CR2=0; // to save Run SPI Before Complete Configuration of SPI Registers

	if(SPIx ==SPI1)
	{
		Global_SPI_Comfig1=* SPI_Config;
		Global_SPI_Comfig[SPI1_INDEX]=&Global_SPI_Comfig1;
		RCC_SPI1_CLK();
	}
	else if(SPIx==SPI2)
	{
		Global_SPI_Comfig2=* SPI_Config;

		Global_SPI_Comfig[SPI2_INDEX]=&Global_SPI_Comfig2;
		RCC_SPI2_CLK();

	}
	//Enable SPI -> /*Bit 6 SPE: SPI enable
	//0: Peripheral disabled
	//1: Peripheral enabled

	tmpreg_CR1=(0x1U<<6);

	//master or salve
	tmpreg_CR1 |=SPI_Config->Device_Mode;


	//SPI_comminucation mode
	tmpreg_CR1 |= SPI_Config->communication_Mode;

	//SPI_Frame_Formate
	tmpreg_CR1 |= SPI_Config->Frame_Format;


	//SPI_DataSize
	tmpreg_CR1 |= SPI_Config->DataSize;

	//SPI_clock polarity
	tmpreg_CR1 |= SPI_Config->CLKPolarity;


	//SPI_clock Phase
	tmpreg_CR1 |= SPI_Config->CLKPhase;

	//----------------NSS------------------------
	if(SPI_Config->NSS == SPI_NSS_Hard_Master_Slave_Select_output_enable)
	{
		tmpreg_CR2 |= SPI_Config->NSS;

	}

	else if(SPI_Config->NSS == SPI_NSS_Hard_Master_Slave_Select_output_disable)
	{
		tmpreg_CR2 &= SPI_Config->NSS;

	}

	else
	{
		tmpreg_CR1 |= SPI_Config->NSS;

	}


	// SPI_BAUDRATEPRESCALER

	tmpreg_CR1 |=SPI_Config->SPI_BAUDRATEPRESCALER;


	//interrupt
	if(SPI_Config->IRQ_Enable !=SPI_IRQ_Enable_NONE)
	{
		//SPI_IRQ_Enable define
		tmpreg_CR2 |= SPI_Config->IRQ_Enable;
		if(SPIx==SPI1)
		{
			NVIC_IRQ35_SPI1_Enable;
		}
		else if (SPIx==SPI2)
		{
			NVIC_IRQ36_SPI2_Enable;


		}
	}

	SPIx->SPI_CR1=tmpreg_CR1;
	SPIx->SPI_CR2 = tmpreg_CR2;
}
void MCAL_SPI_DeInit(SPI_Typedef *SPIx)
{
	if(SPIx==SPI1)
	{
		NVIC_IRQ35_SPI1_Disable;
		RCC_SPI1_Reset();
	}
	if(SPIx==SPI1)
	{
		NVIC_IRQ36_SPI2_Disable;
		RCC_SPI2_Reset();
	}

}

void MCAL_SPI_SendData(SPI_Typedef *SPIx ,uint16_t *pTxBuffer, enum Polling_mechanism  PollinfEn )
{
	/* you can add another moods */

	//to do receive  only

	if(PollinfEn==Pollingenable)
		while (!((SPIx)->SPI_SR &  SPI_SR_TXE));

	SPIx->SPI_DR = *pTxBuffer;
}
void MCAL_SPI_ReceiveData(SPI_Typedef *SPIx ,uint16_t *pTxBuffer, enum Polling_mechanism  PollinfEn )
{

	if(PollinfEn==Pollingenable)
		while (!((SPIx)->SPI_SR &  SPI_SR_RXNE));

	*pTxBuffer=SPIx->SPI_DR;


}
void MCAL_SPI_TX_RX(SPI_Typedef *SPIx ,uint16_t *pTxBuffer, enum Polling_mechanism  PollinfEn )
{
	if(PollinfEn== Pollingenable)
		while (!((SPIx)->SPI_SR &  SPI_SR_TXE));

	SPIx->SPI_DR = *pTxBuffer;



	if(PollinfEn==Pollingenable)
		while (!((SPIx)->SPI_SR &  SPI_SR_RXNE));

	*pTxBuffer=SPIx->SPI_DR;


}


void MCAL_SPI_GPIO_Set_Pins(SPI_Typedef *SPIx)
{

	GPIO_PinConfig Pincfg;
	if(SPIx ==SPI1)
	{
		//	PA4  :  SPI1_NSS
		//	PA5  :  SPI1_SCK
		//	PA6  :  SPI1_MISO
		//	PA7  :  SPI1_MOSI

		if(Global_SPI_Comfig[SPI1_INDEX]->Device_Mode==SPI_Device_Mode_MASTER)
		{
			//PA4 SPI1_NSS
			switch (Global_SPI_Comfig[SPI1_INDEX]->NSS)
			{
			case SPI_NSS_Hard_Master_Slave_Select_output_disable:

			//Hardware master / Slave input floating
			Pincfg.GPIO_PinNumber = GPIO_PIN_4;
			Pincfg.GPIO_MOOD= GPIO_MOOD_INPUT_FLO;
			MCAL_GPIO_Init(GPIOA,&Pincfg);

			break;
			case SPI_NSS_Hard_Master_Slave_Select_output_enable:

			//Hardware master / Slave input floating
			Pincfg.GPIO_PinNumber = GPIO_PIN_4;
			Pincfg.GPIO_MOOD= GPIO_MOOD_OUTPUT_AF_PP;
			Pincfg.GPIO_Output_speed=GPIO_SPEED_10M;
			MCAL_GPIO_Init(GPIOA,&Pincfg);
			break;
			}

			//PA5 : SPI1_SCK
			//Master Alternative function Push-Pull

			Pincfg.GPIO_PinNumber = GPIO_PIN_5;
			Pincfg.GPIO_MOOD= GPIO_MOOD_OUTPUT_AF_PP;
			Pincfg.GPIO_Output_speed=GPIO_SPEED_10M;
			MCAL_GPIO_Init(GPIOA,&Pincfg);


			//PA6 :SPI1_MISO(Support only Full-duplex
			// Full-duplex -> master input floating

			Pincfg.GPIO_PinNumber = GPIO_PIN_6;
			Pincfg.GPIO_MOOD= GPIO_MOOD_INPUT_FLO;
			MCAL_GPIO_Init(GPIOA,&Pincfg);

			//PA7 :SPI1_MOSI
			// Full-duplex -> master  Alternative function Push-Pull

			Pincfg.GPIO_PinNumber = GPIO_PIN_7;
			Pincfg.GPIO_MOOD= GPIO_MOOD_OUTPUT_AF_PP;
			Pincfg.GPIO_Output_speed=GPIO_SPEED_10M;
			MCAL_GPIO_Init(GPIOA,&Pincfg);

		}

		else //Slave
																{
			//PA4 SPI1_NSS
			if(Global_SPI_Comfig[SPI1_INDEX]->NSS==SPI_NSS_Hard_Slave)
			{
				//Hardware master / Slave input floating
				Pincfg.GPIO_PinNumber = GPIO_PIN_4;
				Pincfg.GPIO_MOOD= GPIO_MOOD_INPUT_FLO;
				MCAL_GPIO_Init(GPIOA,&Pincfg);
			}

			//PA5 :SPI1_SCK
			Pincfg.GPIO_PinNumber = GPIO_PIN_5;
			Pincfg.GPIO_MOOD= GPIO_MOOD_INPUT_FLO;
			MCAL_GPIO_Init(GPIOA,&Pincfg);


			//PA6 :SPI1_MISO(Support only Full-duplex0)
			// Full-duplex -> slave(point to piont ) Alternative function push-Pull output

			Pincfg.GPIO_PinNumber = GPIO_PIN_6;
			Pincfg.GPIO_MOOD= GPIO_MOOD_OUTPUT_AF_PP;
			Pincfg.GPIO_Output_speed=GPIO_SPEED_10M;
			MCAL_GPIO_Init(GPIOA,&Pincfg);


			//PA7 :SPI1_MOSI
			// Full-duplex -> slave input floating
			Pincfg.GPIO_PinNumber = GPIO_PIN_7;
			Pincfg.GPIO_MOOD= GPIO_MOOD_INPUT_FLO;
			MCAL_GPIO_Init(GPIOA,&Pincfg);
																}

	}

	else if (SPIx ==SPI2)
	{
		//	PA12  :  SPI2_NSS
		//	PA13  :  SPI2_SCK
		//	PA14  :  SPI2_MISO
		//	PA15  :  SPI2_MOSI



		if(Global_SPI_Comfig[SPI2_INDEX]->Device_Mode==SPI_Device_Mode_MASTER)
		{
			//PA12 SPI1_NSS
			switch (Global_SPI_Comfig[SPI2_INDEX]->NSS)
			{
			case SPI_NSS_Hard_Master_Slave_Select_output_disable:

			//Hardware master / Slave input floating
			Pincfg.GPIO_PinNumber = GPIO_PIN_12;
			Pincfg.GPIO_MOOD= GPIO_MOOD_INPUT_FLO;
			MCAL_GPIO_Init(GPIOA,&Pincfg);

			break;
			case SPI_NSS_Hard_Master_Slave_Select_output_enable:

			//Hardware master / Slave input floating
			Pincfg.GPIO_PinNumber = GPIO_PIN_4;
			Pincfg.GPIO_MOOD= GPIO_MOOD_OUTPUT_AF_PP;
			Pincfg.GPIO_Output_speed=GPIO_SPEED_10M;
			MCAL_GPIO_Init(GPIOA,&Pincfg);
			break;
			}

			//PA13 : SPI1_SCK
			//Master Alternative function Push-Pull

			Pincfg.GPIO_PinNumber = GPIO_PIN_13;
			Pincfg.GPIO_MOOD= GPIO_MOOD_OUTPUT_AF_PP;
			Pincfg.GPIO_Output_speed=GPIO_SPEED_10M;
			MCAL_GPIO_Init(GPIOA,&Pincfg);


			//PA14 :SPI1_MISO(Support only Full-duplex
			// Full-duplex -> master input floating

			Pincfg.GPIO_PinNumber = GPIO_PIN_14;
			Pincfg.GPIO_MOOD= GPIO_MOOD_INPUT_FLO;
			MCAL_GPIO_Init(GPIOA,&Pincfg);

			//PA15 :SPI1_MOSI
			// Full-duplex -> master  Alternative function Push-Pull

			Pincfg.GPIO_PinNumber = GPIO_PIN_15;
			Pincfg.GPIO_MOOD= GPIO_MOOD_OUTPUT_AF_PP;
			Pincfg.GPIO_Output_speed=GPIO_SPEED_10M;
			MCAL_GPIO_Init(GPIOA,&Pincfg);

		}

		else //Slave
		{
			//PA12 SPI1_NSS
						if(Global_SPI_Comfig[SPI2_INDEX]->NSS==SPI_NSS_Hard_Slave)
						{
							//Hardware master / Slave input floating
							Pincfg.GPIO_PinNumber = GPIO_PIN_12;
							Pincfg.GPIO_MOOD= GPIO_MOOD_INPUT_FLO;
							MCAL_GPIO_Init(GPIOA,&Pincfg);
						}

						//PA13 :SPI1_SCK
						Pincfg.GPIO_PinNumber = GPIO_PIN_13;
						Pincfg.GPIO_MOOD= GPIO_MOOD_INPUT_FLO;
						MCAL_GPIO_Init(GPIOA,&Pincfg);


						//PA14 :SPI1_MISO(Support only Full-duplex0)
						// Full-duplex -> slave(point to piont ) Alternative function push-Pull output

						Pincfg.GPIO_PinNumber = GPIO_PIN_14;
						Pincfg.GPIO_MOOD= GPIO_MOOD_OUTPUT_AF_PP;
						Pincfg.GPIO_Output_speed=GPIO_SPEED_10M;
						MCAL_GPIO_Init(GPIOA,&Pincfg);


						//PA15 :SPI1_MOSI
						// Full-duplex -> slave input floating
						Pincfg.GPIO_PinNumber = GPIO_PIN_15;
						Pincfg.GPIO_MOOD= GPIO_MOOD_INPUT_FLO;
						MCAL_GPIO_Init(GPIOA,&Pincfg);
		}
	}
}



// IRQ
void SPI1_IRQHandler(void)
{
	struct S_IRQ_SRC  irq_src;

	irq_src.TXE = ((SPI1->SPI_SR & (1<<1))>>1);
	irq_src.RXNE = ((SPI1->SPI_SR & (1<<0))>>0);
	irq_src.ERRI = ((SPI1->SPI_SR & (1<<4))>>4);

	Global_SPI_Comfig[SPI1_INDEX]->P_IRQ_CallBack(irq_src);
}

void SPI_IRQHandler(void)
{
	struct S_IRQ_SRC  irq_src;

	irq_src.TXE = ((SPI2->SPI_SR & (1<<1))>>1);
	irq_src.RXNE = ((SPI2->SPI_SR & (1<<0))>>0);
	irq_src.ERRI = ((SPI2->SPI_SR & (1<<4))>>4);

	Global_SPI_Comfig[SPI2_INDEX]->P_IRQ_CallBack(irq_src);
}
