/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32l1xx.h"

int main() {
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    // Active les ports B & A du microcontrôleur

    GPIO_InitTypeDef leds_PB;
    // Défini une structure qui sera utilisée par le GPIO pour la led B

    GPIO_StructInit( & leds_PB);
    // Initialise la structure de led B
    leds_PB.GPIO_Mode = GPIO_Mode_OUT;
    // Configure le mode pour la structure de led B en OUT
    leds_PB.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_6;
    // Choisit le pin 7 et 6

    GPIO_Init(GPIOB, & leds_PB);
    // Ecrit les paramètres de led B dans GPIOB


	GPIO_InitTypeDef Boutton;

	GPIO_StructInit( & Boutton);
	Boutton.GPIO_Mode = GPIO_Mode_IN;
	Boutton.GPIO_Pin = GPIO_Pin_0;

	GPIO_Init(GPIOA, & Boutton);

    while (1) {
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 1)
		{
			GPIO_SetBits(GPIOB, GPIO_Pin_7);
			for (int k = 0; k < 500000; k++) {}
			GPIO_ResetBits(GPIOB, GPIO_Pin_7);
			for (int k = 0; k < 500000; k++) {}
		}
		GPIO_ResetBits(GPIOB, GPIO_Pin_7);
    }

    while(2)
    {
    	GPIO_SetBits(GPIOB, GPIO_Pin_8);
    }
    
    return 0;
}
