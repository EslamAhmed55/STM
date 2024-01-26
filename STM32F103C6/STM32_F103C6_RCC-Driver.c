/*
 * STM32_F103C6_RCC-Driver.c
 *
 *  Created on: Jul 23, 2023
 *      Author: eslam
 */


#include "../STM32F103C6/includes/STM32_F103C6_RCC-Driver.h"


/* From  STM32F103xx_TRM Page 103
 *
 * Bits 10:8 PPRE1: APB low-speed prescaler (APB1)
Set and cleared by software to control the division factor of the APB low-speed clock
(PCLK1).
Warning: the software has to set correctly these bits to not exceed 36 MHz on this domain.
0xx: HCLK not divided
100: HCLK divided by 2
101: HCLK divided by 4
110: HCLK divided by 8
111: HCLK divided by 16
 *
 */

const uint8_t AHBPresectable[8U]= {0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9}; // shift right == division by 2


/* From  STM32F103xx_TRM Page 103
 * Set and cleared by software to control the division factor of the AHB clock.
0xxx: SYSCLK not divided
1000: SYSCLK divided by 2
1001: SYSCLK divided by 4
1010: SYSCLK divided by 8
1011: SYSCLK divided by 16
1100: SYSCLK divided by 64
1101: SYSCLK divided by 128
1110: SYSCLK divided by 256
1111: SYSCLK divided by 512
 */
const uint8_t APBPresectable[8U]= {0,0,0,1,2,3,4}; // shift right == division by 2

uint32_t MCAL_RCC_GetSYS_CLK1Freq (void)
{
	/* Read Status To now clock source will entered
	 * Bits 3:2 SWS: System clock switch status
Set and cleared by hardware to indicate which clock source is used as system clock.
00: HSI oscillator used as system clock
01: HSE oscillator used as system clock
10: PLL used as system clock
11: not applicable
	 */


	switch ((RCC->CFGR >>2)&0b11)
	{
	case 0:

		return HSI_RC_Clk;
		break;


	case 1:
		//to do need to calculate it  // HSE User Should Specify it
			return HSE_RC_Clk;
			break;

	case 2:
		//to do need to calculate it PLLCLK & PLL & PLLMUL Source Mux
			return 16000000;
			break;


	}

}
uint32_t MCAL_RCC_GetHCLK1Freq (void)
{
		//Bits 7:4 HPRE: AHB prescaler
	/*
	 * Set and cleared by software to control the division factor of the AHB clock.
0xxx: SYSCLK not divided
1000: SYSCLK divided by 2
1001: SYSCLK divided by 4
1010: SYSCLK divided by 8
1011: SYSCLK divided by 16
1100: SYSCLK divided by 64
1101: SYSCLK divided by 128
1110: SYSCLK divided by 256
1111: SYSCLK divided by 512
	 */
	   return (MCAL_RCC_GetSYS_CLK1Freq() >>APBPresectable[((RCC->CFGR >> 8) & 0xF)]);

}
uint32_t MCAL_RCC_GetPCLK1Freq (void)
{
	   //Bits 10:8 PPRE1: APB low-speed prescaler (APB1)
	   return (MCAL_RCC_GetHCLK1Freq() >>APBPresectable[((RCC->CFGR >> 8) & 0b111)]);
}
uint32_t MCAL_RCC_GetPCLK2Freq (void)
{
		//Bits 13:11 PPRE2: APB high-speed prescaler (APB2)
	   return (MCAL_RCC_GetHCLK1Freq() >>APBPresectable[((RCC->CFGR >> 8) & 0b111)]);

}
