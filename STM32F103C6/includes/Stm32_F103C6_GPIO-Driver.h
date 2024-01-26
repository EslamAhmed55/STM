/*
 * Stm32_F103C6.h
 *
 *  Created on: Jun 29, 2023
 *      Author: eslam
 */

#ifndef INCLUDES_STM32_F103C6_GPIO_DRIVER_H_
#define INCLUDES_STM32_F103C6_GPIO_DRIVER_H_

//includes
#include "../../STM32F103C6/includes/Stm32_F103x6.h"

//------------------------------
//configure structure (informations of Pins )

typedef struct
{
	uint16_t GPIO_PinNumber; // specifies the GPIO pins to be configured
	//this parameter must be set based on  reference GPOI_PINS _define

	uint8_t GPIO_MOOD;  // specifies GPIO_MOOD for the selected pin
	//this parameter must be set based on  reference GPOI_MOOD _define

	uint8_t GPIO_Output_speed ;// specifies Speed for the selected pin
	//this parameter must be set based on  reference GPOI_Speed _define
}GPIO_PinConfig;




/*
 * =================================================================
 *
 *               		Reference Macros
 *
 * =================================================================
 *
 */
//GPOI_PINS _define
#define GPIO_PIN_0					((uint16_t)0x0001) /*Pin 0 selected */
#define GPIO_PIN_1					((uint16_t)0x0002) /*Pin 1 selected ((With HEXA ) 10) */
#define GPIO_PIN_2					((uint16_t)0x0004) /*Pin 2 selected  ((With HEXA ) 100) */
#define GPIO_PIN_3					((uint16_t)0x0008) /*Pin 3 selected  ((With HEXA ) 1000) */
#define GPIO_PIN_4					((uint16_t)0x0010) /*Pin 4 selected */
#define GPIO_PIN_5					((uint16_t)0x0020) /*Pin 5 selected */
#define GPIO_PIN_6					((uint16_t)0x0040) /*Pin 6 selected */
#define GPIO_PIN_7					((uint16_t)0x0080) /*Pin 7 selected */
#define GPIO_PIN_8					((uint16_t)0x0100) /*Pin 8 selected */
#define GPIO_PIN_9					((uint16_t)0x0200) /*Pin 9 selected */
#define GPIO_PIN_10					((uint16_t)0x0400) /*Pin 10 selected */
#define GPIO_PIN_11					((uint16_t)0x0800) /*Pin 11 selected */
#define GPIO_PIN_12					((uint16_t)0x1000) /*Pin 12 selected */
#define GPIO_PIN_13				    ((uint16_t)0x2000) /*Pin 13 selected */
#define GPIO_PIN_14				    ((uint16_t)0x4000) /*Pin 14 selected */
#define GPIO_PIN_15					((uint16_t)0x8000) /*Pin 15 selected */
#define GPIO_PIN_ALL				((uint16_t)0xFFFF) /*Pin ALL selected */

#define GPOI_PIN_MASK				0x0000FFFFu /* PIN MASK for assert test /

//GPOI_MOOD _define (depend on specs)

/*
0: Analog mode
1: Floating input (reset state)
2: Input with pull-up
3:  pull-down
4: General purpose output push-pull
5: General purpose output Open-drain
6: Alternate function output Push-pull
7: Alternate function output Open-drain
8: Alternative function input
 */

// SORRY MODE==MOOD
#define GPIO_MOOD_Analog 			0x00000000u //Analog mode
#define GPIO_MOOD_INPUT_FLO 		0x00000001u //Floating input
#define GPIO_MOOD_INPUT_PU 			0x00000002u //Input with pull-up
#define GPIO_MOOD_INPUT_PD 			0x00000003u //pull-down
#define GPIO_MOOD_OUTPUT_PP 		0x00000004u //General purpose output push-pull
#define GPIO_MOOD_OUTPUT_OD 		0x00000005u //General purpose output Open-drain
#define GPIO_MOOD_OUTPUT_AF_PP		0x00000006u //Alternate function output Push-pull
#define GPIO_MOOD_OUTPUT_AF_OD 		0x00000007u// Alternate function output Open-drain
#define GPIO_MOOD_AF_INPUT 			0x00000008u //Alternative function input // the  Alternative function is the same configuration of  Floating input

//GPOI_Speed _define
/*
1: Output mode, max speed 10 MHz.
2: Output mode, max speed 2 MHz.
3: Output mode, max speed 50 MHz.
 */

#define GPIO_SPEED_10M			0x00000001u //output mode  , max speed 10MHZ
#define GPIO_SPEED_20M			0x00000002u //output mode  , max speed 20MHZ
#define GPIO_SPEED_50M			0x00000003u //output mode  , max speed 50MHZ



//GPIO_Pin_State
#define GPIO_PIN_SET		1
#define GPIO_PIN_RESET		0



//GPIO_RETURN_LOCK
#define GPIO_RETURN_LOCK_Enable		1
#define GPIO_RETURN_LOCK_ERROR		0


//==========================================================================
//____________________________________________________________
//                APIS Supported by " MCAL GPIO DRIVER"
//____________________________________________________________
void MCAL_GPIO_Init(GPIO_Typedef *GPIOx, GPIO_PinConfig *PinConfig);
void MCAL_GPIO_DeInit(GPIO_Typedef *GPIOx);
uint8_t MCAL_GPIO_ReadPin (GPIO_Typedef *GPIOx , uint16_t PinNumber);
uint16_t MCAL_GPIO_ReadPort (GPIO_Typedef *GPIOx);
uint8_t MCAL_GPIO_WritePin (GPIO_Typedef *GPIOx , uint16_t PinNumber , uint8_t Value);
uint16_t MCAL_GPIO_WritePort (GPIO_Typedef *GPIOx , uint16_t Value );
uint8_t MCAL_GPIO_TogglePin (GPIO_Typedef *GPIOx , uint16_t PinNumber);
uint8_t MCAL_GPIO_LockPin (GPIO_Typedef *GPIOx , uint16_t PinNumber);

#endif /* INCLUDES_STM32_F103C6_GPIO_DRIVER_H_ */
