/*
 * STM32_F103C6_I2C-Driver.c
 *
 *  Created on: Aug 1, 2023
 *      Author: eslam
 */

#include "../STM32F103C6/includes/Stm32_F103C6_I2C-Driver.h"

/*
 * =================================================================
 *
 *				Includes
 *
 * =================================================================
 *
 */




/*
 * =================================================================
 *
 *               		Generic Variables
 *
 * =================================================================
 *
 */


I2C_InitTypedef Global_I2C_Config[2] = {NULL,NULL};


/*
 * =================================================================
 *
 *               		Generic Macros
 *
 * =================================================================
 *
 */

#define I2C1_INDEX		0
#define I2C2_INDEX		1











/*
 * =================================================================
 *
 *				Generic Functions
 *
 * =================================================================
 *
 */













/*
 * =================================================================
 *
 * 				APIs Functions
 *
 * =================================================================
 *
 */

/*
 * * Fn                -MCAL_I2C_Init.
*
* Brief             -Initializes I2Cx according to the specified parameters in
* 					 I2C_Config.
*
* Param [in]        -I2Cx : where x can be (1..2 depending on device used) to
* 					 select I2C peripheral.
*
* Param [in]        -I2C_Config: All I2C configuration.
*
* Retval            -None.
*
* Note              -Supported for I2C SM mode only.
* 					-Support only 7-bit address mode.
*/



void    MCAL_I2C_Init  (I2C_Typedef* I2Cx, I2C_InitTypedef *I2C_InitStruct)
{
	uint16_t temreg =0 , FreqRange=0;
	uint32_t pclk1=8000000;
	uint16_t result=0;
	//enable RCC clock
	if(I2Cx == I2C1)
	{
		Global_I2C_Config[I2C1_INDEX]= *I2C_InitStruct;
		RCC_I2C1_CLK();
	}
	else
	{
		Global_I2C_Config[I2C2_INDEX]= *I2C_InitStruct;
				RCC_I2C2_CLK();
	}

	if(I2C_InitStruct->I2C_Mode ==I2C_mode_I2C)
	{

		/*---------------------------INIT_Timing---------------------*/

		// I2C_CR2-> FREQ[5:0]:peripheral clock frequency

		temreg =I2Cx->CR2;
		/*clear frequency FREQ[5:0] Bits*/
		temreg &=~(I2C_CR2_FREQ); // form bit filed /* (0x3FUL << I2C_CR2_FREQ_Pos)*/

		pclk1= MCAL_RCC_GetPCLK1Freq();

		FreqRange =(uint16_t)(pclk1/1000000);

		temreg |=FreqRange;
		I2Cx->CR2=temreg;
		//Configure the clock control registers (I2C_CCR)
		/*Disable the selected I2C peripheral to configure time*/
		I2Cx -> CR1 &= ~(I2C_CR1_PE);

		temreg=0;
		/*configure speed in standard mood (SM) --> Bit15 F/S (by defualt =0:SM Mode I2C) */

		if((I2C_InitStruct->I2C_Clock_Speed== I2c_SCLK_SM_50K) || (I2C_InitStruct->I2C_Clock_Speed== I2c_SCLK_SM_100K) )
		{
			/*T(clock)/2= CCR* T(PCLK1)  -->page 782*/
			// go to page 782 and cheek the equations

			result = (uint16_t)(pclk1/(I2C_InitStruct->I2C_Clock_Speed<<1)); // 2* I2C_InitStruct->I2C_Clock_Speed == I2C_InitStruct->I2C_Clock_Speed<<1

		temreg |=result;

		I2Cx->CCR==temreg;

		//• Configure the rise time register -> I2CTRISE register (page 782)
		/*
		 * page 783
		 * For instance: in Sm mode, the maximum allowed SCL rise time is 1000 ns.
			If, in the I2C_CR2 register, the value of FREQ[5:0] bits is equal to 0x08 and TPCLK1 = 125 ns
			therefore the TRISE[5:0] bits must be programmed with 09h
		 */
			I2Cx->TRISE= FreqRange+1;
		}
		else
		{
			// fast mode not supported
		}

		/*----------------------------configure CR1---------------------*/
		/* Set the I2C CR1 value */

		temreg = I2Cx->CR1;

		temreg |= (uint16_t)(I2C_InitStruct->I2C_ACK_Control | I2C_InitStruct->General_Call_Adderss_Detection | I2C_InitStruct->Stretch_Mode);

		I2Cx-> CR1 = temreg;

		/*----------------------------configure  OAR1   &   OAR2 ---------------------*/

		temreg =0;
		if (I2C_InitStruct -> I2C_Slave_Address.Dual_Address_Mode ==1)
		{
			temreg = I2C_OAR2_ENDUAL;
			temreg |=  I2C_InitStruct->I2C_Slave_Address.Secondary_Slave_Address << I2C_OAR2_ADD2_Pos;  //I2C_OAR2_ADD2_Pos == 1<<1
			I2Cx->OAR2 = temreg;
		}
		temreg=0;
		temreg |= I2C_InitStruct->I2C_Slave_Address.Primary_Slave_Address<<1;
		temreg |= I2C_InitStruct->I2C_Slave_Address.Addressing_Slave_Mode;
		I2Cx->OAR1=temreg;
	}

		else
		{
			// SMBUS Not supported
		}

// interrupt mode (slave mode) //check callback pointer |= NULL

	if(I2C_InitStruct->P_Slave_Event_CallBack!=NULL)//that enable salve IRQ State Mode

	{
		//enable IRQ

		I2Cx->CR2 |= (I2C_CR2_ITERREN);
		I2Cx->CR2 |= (I2C_CR2_ITEVTEN);
		I2Cx->CR2 |= (I2C_CR2_ITBUFEN);

		if(I2Cx==I2C1)
		{
			NVIC_IRQ31_I2C1_EV_Enable;
			NVIC_IRQ32_I2C1_ER_Enable;

		}
		else if(I2Cx==I2C2)

		{
			NVIC_IRQ33_I2C2_EV_Enable;
			NVIC_IRQ34_I2C2_ER_Enable;
		}

		// Clear Status Register 1 & 2
		I2Cx->SR1 =0;
		I2Cx->SR2 =0;

	}

/* enable the select I2C peripheral*/
	I2Cx-> CR1|= I2C_CR1_PE;



}

/*
 * * Fn                -MCAL_I2C_DeInit.
*
* Brief             -Resets Selected I2C Module and disable NVIC IRQs.
*
* Param [in]        -I2Cx : where x can be (1..2 depending on device used) to
*                    select I2C peripheral.
*
* Retval            -None.
*
* Note              -Reset The Module By RCC & Disable NVIC.
 */
void    MCAL_I2C_DeInit(I2C_Typedef* I2Cx)
{

	if(I2Cx==I2C1)
	{
		NVIC_IRQ31_I2C1_EV_Disable;
		NVIC_IRQ32_I2C1_ER_Disable;

		RCC_I2C1_Reset();

	}

	else if(I2Cx==I2C2)
		{
			NVIC_IRQ33_I2C2_EV_Disable;
			NVIC_IRQ34_I2C2_ER_Disable;

			RCC_I2C2_Reset();

		}
}


/*
 *  Fn                -MCAL_I2C_GPIO_Set_Pins.
*
* Brief             -Initializes GPIO Pins to be connected with the selected I2C.
*
* Param [in]        -I2Cx : where x can be (1..2 depending on device used) to
*                    select I2C peripheral.
*
* Retval            -None.
*
* Note              -Must open clock for AFIO & GPIO After GPIO Initialization.
*                   -Supported for I2C SM MODE only.
 */
void    MCAL_I2C_GPIO_Set_Pins(I2C_Typedef* I2Cx)
{
	// look at Table 5 page 30 in "STM32F103C6" DataSheet

	/* look at recommended configuration I2C Pins at page 168 in TRM*/

	GPIO_PinConfig PinCfg;

	if(I2Cx == I2C1)
	{
		// PB6 :I2C1_SCL
		// PB7 :I2C1_SDA

		PinCfg.GPIO_PinNumber= GPIO_PIN_6;
		PinCfg.GPIO_MOOD= GPIO_MOOD_OUTPUT_AF_OD;
		PinCfg.GPIO_Output_speed= GPIO_SPEED_10M;
		MCAL_GPIO_Init(GPIOB, & PinCfg);

		PinCfg.GPIO_PinNumber= GPIO_PIN_7;
		PinCfg.GPIO_MOOD= GPIO_MOOD_OUTPUT_AF_OD;
		PinCfg.GPIO_Output_speed= GPIO_SPEED_10M;
		MCAL_GPIO_Init(GPIOB, & PinCfg);


	}

	else if(I2Cx == I2C2)
		{
			// PB10 :I2C2_SCL
			// PB11 :I2C2_SDA

			PinCfg.GPIO_PinNumber= GPIO_PIN_10;
			PinCfg.GPIO_MOOD= GPIO_MOOD_OUTPUT_AF_OD;
			PinCfg.GPIO_Output_speed= GPIO_SPEED_10M;
			MCAL_GPIO_Init(GPIOB, & PinCfg);

			PinCfg.GPIO_PinNumber= GPIO_PIN_11;
			PinCfg.GPIO_MOOD= GPIO_MOOD_OUTPUT_AF_OD;
			PinCfg.GPIO_Output_speed= GPIO_SPEED_10M;
			MCAL_GPIO_Init(GPIOB, & PinCfg);


		}


}

void    MCAL_I2C_MASTER_RX(I2C_Typedef* I2Cx, uint16_t Device_Address, uint8_t *dataOut, uint8_t Data_Length, Stop_Condition Stop, Repeated_Start Start)
{

	uint8_t index = I2Cx==I2C1 ? I2C1_INDEX :I2C2_INDEX;
	int i=0;

	//1- Set the Start bit in the I2C_CR1 Register to generate a start condition




		//2- wait for EV5
		/*EV5: SB=1, cleared by reading SR1 register followed by writing DR register with Address.
		 *
		 */
		while (!I2C_GETFlagStatus(I2Cx,EV5)); // polling to check EV5 is Set or not

		//3- Send Address
		I2C_SendAddress(I2Cx, Device_Address, I2C_Direction_Reciever);

		//3-Wait EV6   	/*EV6: ADDR=1, cleared by reading SR1 register followed by reading SR2.*/

		while (!I2C_GETFlagStatus(I2Cx,EV6));

		//Enable ACK
		I2C_AcknowledgeConFig(I2Cx,ENABLE);

		if(Data_Length)
		{
			//read the data until length become zero

			for (i = Data_Length; i>1;i--)
			{
				//wait until RXNE become 1
				while (!I2C_GETFlagStatus(I2Cx,EV7));
				//read the data from data register in to buffer
				*dataOut=I2Cx->DR;
				//increment the buffer address
				dataOut++;

			}
			I2C_AcknowledgeConFig(I2Cx,DISABLE);

		}
		if(Stop == with_Stop)
		{
			// send stop condition
			I2c_GenerateSTOP(I2Cx,ENABLE);
		}

		if(Global_I2C_Config[index].I2C_ACK_Control== I2C_ACK_Enable)
		{
			I2C_AcknowledgeConFig(I2Cx,ENABLE);

		}
}

void    MCAL_I2C_MASTER_TX(I2C_Typedef* I2Cx, uint16_t Device_Address, uint8_t *dataOut, uint8_t Data_Length, Stop_Condition Stop, Repeated_Start Start)
{
/* please look at Figure 273. Transfer sequence diagram for master transmitter
 *
 */
	int i=0;

	//1- Set the Start bit in the I2C_CR1 Register to generate a start condition

	I2C_GenerateSTART(I2Cx, ENABLE, start);

	//2- wait for EV5
	/*EV5: SB=1, cleared by reading SR1 register followed by writing DR register with Address.
	 *
	 */
	while (!I2C_GETFlagStatus(I2Cx,EV5)); // polling to check EV5 is Set or not

	//3- Send Address
	I2C_SendAddress(I2Cx, Device_Address, I2C_Direction_Transmitter);

	//3-Wait EV6   	/*EV6: ADDR=1, cleared by reading SR1 register followed by reading SR2.*/

	while (!I2C_GETFlagStatus(I2Cx,EV6));

	//4- /*TRA , BUSY , MSL , TXE flags*/
	while(!I2C_GETFlagStatus(I2Cx, MASTER_BYTE_TRANSMITTING));


	for(i;i<Data_Length;i++)
	{
		/*write in DR register the data to be sent*/
		I2Cx->DR=dataOut[i];

		//E//EV8: TxE=1, shift register not empty, d. ata register empty, cleared by writing DR register

		while (!I2C_GETFlagStatus(I2Cx,EV8));


	}


	if(Stop == with_Stop)
	{
		// send stop condition
		I2c_GenerateSTOP(I2Cx,ENABLE);
	}
}

void I2C_GenerateSTART(I2C_Typedef* I2Cx , FunctionalState NewState , Repeated_Start start)
{
	// check the start is normal start or repeated start

	if(start != repeated_start)
	{
		//check if the Bus is idle

		while (I2C_GETFlagStatus(I2Cx,I2C_FLAG_BUSY)); // if I2C_GETFlagStatus return 1 this explain that the bus is  busy
	}

	/*
	Bit 8 START: Start generation
	This bit is set and cleared by software and cleared by hardware when start is sent or PE=0.
	In Master Mode:
	0: No Start generation
	1: Repeated start generation
	In Slave mode:
	0: No Start generation
	1: Start generation when the bus is free
	*/

	if(NewState!=DISABLE)
	{
		/* Generate a start condition */
		I2Cx->SR1 |= I2C_CR1_START;

	}

	else
	{		/* Disable a start condition generation */
		I2Cx->SR1 &= ~(I2C_CR1_START);


	}

}


void I2C_SendAddress(I2C_Typedef* I2Cx,uint16_t Address, I2C_Direction Direction)
{

	//only 7Bit Mode Supported

	Address = (Address <<1);

	if(Direction!= I2C_Direction_Transmitter)
	{
		Address |=1<<0;
	}
	else
	{
		Address &=~(1<<0);

	}

	/* Send the address */

	I2Cx->DR =Address;

}

void I2c_GenerateSTOP(I2C_Typedef* I2Cx , FunctionalState NewState )
{

	if(NewState!=DISABLE)
		{
			/* Generate a start condition */
			I2Cx->CR1 |= I2C_CR1_STOP;

		}

		else
		{		/* Disable a start condition generation */
			I2Cx->CR1 &= ~(I2C_CR1_STOP);


		}
}



FlagStatus I2C_GETFlagStatus(I2C_Typedef *I2Cx, Status flag)
{
	volatile uint32_t dummyRaad;
	FlagStatus bitstatus = RESET;
	uint32_t lastevent =0;

	uint32_t flag1=0,flag2=0;

	switch (flag)
	{
	case I2C_FLAG_BUSY:
		/*
			 * from I2C Status register 2 (I2C_SR2) page(781)
			 * Bit 1 BUSY: Bus busy
			0: No communication on the bus
			1: Communication ongoing on the bus
			– Set by hardware on detection of SDA or SCL low
			– cleared by hardware on detection of a Stop condition.
			 *
			 */

		if((I2Cx->SR2)&(I2C_SR2_BUSY))
		{
			bitstatus = SET;
		}
		else
		{
			bitstatus = RESET;

		}
		break;


	case EV5:
			/*
				 * from I2C Status register 2 (I2C_SR2) page(781)
				 * Bit 1 BUSY: Bus busy
				0: No communication on the bus
				1: Communication ongoing on the bus
				– Set by hardware on detection of SDA or SCL low
				– cleared by hardware on detection of a Stop condition.
				 *
				 */

			if((I2Cx->SR1)&(I2C_SR1_SB))
			{
				bitstatus = SET;
			}
			else
			{
				bitstatus = RESET;

			}
			break;

	case EV6:
		//EV6: ADDR=1,
		/*
		 * Bit 1 ADDR: Address sent (master mode)/matched (slave mode)
This bit is cleared by software reading SR1 register followed reading SR2, or by hardware
when PE=0.
Address matched (Slave)
0: Address mismatched or not received.
1: Received address matched.
		 */
				if((I2Cx->SR1)&(I2C_SR1_ADDR))
				{
					bitstatus = SET;
				}
				else
				{
					bitstatus = RESET;

				}
				//to cleared ADDR field :cleared by reading SR1 register followed by reading SR2.
				dummyRaad = I2Cx->SR2;
				break;


	case MASTER_BYTE_TRANSMITTING:

				{
					/*Read the I2C Status Register*/

					flag1=I2Cx->SR1;
					flag2=I2Cx->SR2;
					flag2= flag2<<16;

					/*Get the last event value from i2c status register*/
					lastevent= (flag1 | flag2) & ((uint32_t)0x00FFFFFF);
					/* check whether the last event is equal to I2C_EVENT*/
					if((lastevent & flag)== flag)
					{
						/* Success last event is equal to I2C_Event */

						bitstatus = SET;
					}
					else
					{
						/* ERROR last event is different to I2C_Event */

						bitstatus = RESET;
					}

				}

	case EV8_1: //EV8_1: TxE=1, shift register empty, data register empty, write Data1 in DR.
	case EV8://EV8: TxE=1, shift register not empty, d. ata register empty, cleared by writing DR register
	{
		/*
		 * I2C Status register 1 (I2C_SR1)
		 * Bit 7 TxE: Data register empty (transmitters)
		0: Data register not empty
		1: Data register empty
		 *
		 */

		if((I2Cx->SR1)&(I2C_SR1_TXE))
					{
						bitstatus = SET;
					}
					else
					{
						bitstatus = RESET;

					}
					break;
	}





	case EV7:
	{
		/*
		 * Bit 6 RxNE: Data register not empty (receivers)
0: Data register empty
1: Data register not empty
– Set when data register is not empty in receiver mode. RxNE is not set during address phase.
– Cleared by software reading or writing the DR register or by hardware when PE=0.
RxNE is not set in case of ARLO event.
		 *
		 */

		if((I2Cx->SR1)&(I2C_SR1_RXNE))
					{
						bitstatus = SET;
					}
					else
					{
						bitstatus = RESET;

					}
					break;
	}










	}






	return bitstatus;
}

void    MCAL_I2C_SlaveSendData(I2C_Typedef* I2Cx, uint8_t data)
{
	I2Cx->DR=data;
}


uint8_t MCAL_I2C_SlaveRecieveData(I2C_Typedef* I2Cx)
{
	return (uint8_t)I2Cx->DR;
}


/*-----------------------Interrupt Hnadler-----------------*/

			/* look at Figure 278. I2C interrupt mapping diagram (page 771) */

//ERROR Hnadler 1
void I2C1_ER_IRQHandler(void)
{

}

// Event Hnadler 1


void I2C1_EV_IRQHandler(void)
{
	volatile uint32_t dummy_read =0;
	I2C_Typedef *I2Cx = I2C1;


	//interrupt handler for both master and salve
	uint32_t temp1 , temp2 , temp3;

	temp1 = I2Cx->CR2 &(I2C_CR2_ITEVTEN); //interrupt event
	temp2 = I2Cx->CR2 &(I2C_CR2_ITBUFEN);
	temp1 = I2Cx->CR2 &(I2C_SR1_STOPF);//stop flag


	// Handle For interrupt generated by STOPF event
		// Note : Stop detection flag is applicable only slave mode
		if(temp1 && temp3)
		{
			//STOF flag is set
			//Clear the STOPF ( i.e 1) read SR1 2) Write to CR1 )
			I2Cx->CR1 |= 0x0000;
			Slave_States(I2Cx,I2C_EV_STOP);


		}
		//----------------------------------------------------------
		temp3  = I2Cx->SR1 & (I2C_SR1_ADDR);
		// Handle For interrupt generated by ADDR event
		//Note : When master mode : Address is sent
		//		 When Slave mode   : Address matched with own address
		if(temp1 && temp3)
		{
			// interrupt is generated because of ADDR event
			//check for device mode
			if(I2Cx->SR2 & ( I2C_SR2_MSL))
			{
				//master

			}else
			{
				//slave mode
				//clear the ADDR flag ( read SR1 , read SR2)
				dummy_read = I2Cx->SR1;
				dummy_read = I2Cx->SR2;
				Slave_States(I2Cx,I2C_EV_ADD_MATCHED);

			}
		}
		//----------------------------------------------------------

		temp3  = I2Cx->SR1 & ( I2C_SR1_TXE);
		// Handle For interrupt generated by TXE event
		if(temp1 && temp2 && temp3)
		{
			//Check for device mode
			if(I2Cx->SR2 & (I2C_SR2_MSL))
			{

			}else
			{
				//slave
				Slave_States(I2Cx,I2C_EV_DATA_REQ);
			}
		}

		//----------------------------------------------------------

		temp3  = I2Cx->SR1 & ( I2C_SR1_RXNE);
		// Handle For interrupt generated by 	SB=1, cleared by reading SR1 register followed by writing DR register with Address.
		if(temp1 && temp2 && temp3)
		{
			//check device mode .
			if(I2Cx->SR2 & ( I2C_SR2_MSL))
			{
				//The device is master

			}else
			{
				//slave
				Slave_States(I2Cx,I2C_EV_DATA_RCV);

			}
		}



}

//ERROR Hnadler 2
void I2C2_ER_IRQHandler(void)
{

}

// Event Hnadler 2


void I2C2_EV_IRQHandler(void)
{

}


void Slave_States (I2C_Typedef* I2Cx  ,Slave_State state)
{
	uint8_t index =    I2Cx == I2C1 ? I2C1_INDEX: I2C2_INDEX ;

	switch (state)
	{

	case I2C_ERROR_AF:
	{
		//make sure that the slave is really in transmitter mode
		if(I2Cx->SR2 & ( I2C_SR2_TRA))
		{
			//Slave Shouldn't Send anything else

		}

		break ;
	}

	case I2C_EV_STOP:
	{
		//make sure that the slave is really in transmitter mode
		if(I2Cx->SR2 & ( I2C_SR2_TRA))
		{
			//Notify APP that the Stop Condition is sent by the master
			Global_I2C_Config [index].P_Slave_Event_CallBack (I2C_EV_STOP) ;
		}

		break ;
	}

	case I2C_EV_ADD_MATCHED:
	{
		//Notify APP that the Stop Condition is sent by the master
		Global_I2C_Config [index].P_Slave_Event_CallBack (I2C_EV_ADD_MATCHED) ;

		break ;
	}

	case I2C_EV_DATA_REQ:
	{
		//make sure that the slave is really in transmitter mode
		if(I2Cx->SR2 & ( I2C_SR2_TRA))
		{
			//the APP layer should send the data (MCAL_I2C_SlaveSendData ) in this state
			Global_I2C_Config [index].P_Slave_Event_CallBack (I2C_EV_DATA_REQ) ;
		}

		break ;
	}
	case I2C_EV_DATA_RCV:
	{
		//make sure that the slave is really in receiver mode
		if(!(I2Cx->SR2 & ( I2C_SR2_TRA)))
		{
			//the APP layer should read the data (MCAL_I2C_SlaveReceiveData ) in this state
			Global_I2C_Config [index].P_Slave_Event_CallBack (I2C_EV_DATA_RCV) ;
		}

		break ;
	}
	}

}


void I2C_AcknowledgeConFig(I2C_Typedef* I2Cx, FunctionalState NewState)
{
	if (NewState != DISABLE)
		{
			/* Enable the acknowledgement */
			I2Cx->CR1 |= I2C_CR1_ACK;
		}
		else
		{
			/* Disable the acknowledgement */
			I2Cx->CR1 &= ~(I2C_CR1_ACK);
		}
}
