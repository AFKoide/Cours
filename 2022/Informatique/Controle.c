#include "stm32l1xx.h"
// Biblio du microcontrôleur

void GPIO_METAL(void)
{
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
}

void GPIO_Biblio()
{
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);
    //Active le périphérique GPIOB

    GPIO_InitTypeDef leds_PB; //Structure pour leds
    GPIO_StructInit(&leds_PB); //Init la structure
    leds_PB.GPIO_Mode = GPIO_Mode_OUT; //Défini le mode
    leds_PB.GPIO_Pin = GPIO_Pin_8; //Choisi le pin 8
    GPIO_Init(GPIOB,&leds_PB); //Compile le programme

    while(1) {
        GPIO_SetBits(GPIOB,GPIO_Pin_8); //Allume le pin 8
        for(int k=0; k<100000;k++){} //Attente
        GPIO_ResetBits(GPIOB,GPIO_Pin_8); //Eteint le pin 8
        for(int k=0; k<100000;k++){} //Attente
    }

    return 0;
}

void TIM5_Config()
{
    /* Activer TIM5 sur APB1 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
    /* Configurer TIM5 : prescalaire à 1 ms, periode au maximum */

    TIM_TimeBaseInitTypeDef DelayTimer; // Crée la structure de configuration
    TIM_TimeBaseStructInit(&DelayTimer); // Initialise la structure
    DelayTimer.TIM_Prescaler = 16000-1; // Défini le prescalaire
    DelayTimer.TIM_Period = 0xFFFFFFFFU; // Taille du compteur (pleine)
    TIM_TimeBaseInit(TIM5,&DelayTimer); // Configure le timer

    /* Tprescalaire = Prescalaire/16x10^6 (fréquence de l'horloge du microcontroleur) */

}
// Fonction Delai en millisecondes
void TIM5_delay_ms(int delay)
{
    TIM_SetCounter(TIM5,0); // Initialise le compteur
    TIM_Cmd(TIM5, ENABLE); // Active le compteur
    while(TIM_GetCounter(TIM5)<delay) {} // Attend que l'on atteigne le delai
    TIM_Cmd(TIM5, DISABLE); // Désactive le compteur
}

// ### ADC1 sur PA5
// Configuration
void ADC1_Config()
{   /* Activer GPIOA sur AHB */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    /* Parametrer PA5 en mode analogique */
    GPIO_InitTypeDef gpio_a;
    gpio_a.GPIO_Pin = GPIO_Pin_5;
    gpio_a.GPIO_Mode = GPIO_Mode_AN;
    gpio_a.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &gpio_a);

    /* Activer ADC1 sur APB2 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    /* Configurer ADC1 en 12 bit, simple acquisition */
    ADC_InitTypeDef adc_1;
    ADC_StructInit(&adc_1);
    adc_1.ADC_NbrOfConversion = 1;
    adc_1.ADC_Resolution = ADC_Resolution_12b;
    ADC_Init(ADC1, &adc_1);
    ADC_Cmd(ADC1, ENABLE);
}

// Acquisition d'une valeur
// PA5 : ch = ADC_Channel_5
uint16_t ADC1_Get(uint8_t ch)
{
    ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_4Cycles);
    ADC_SoftwareStartConv(ADC1);
    while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET)
        ;

    return  ADC_GetConversionValue(ADC1);
}
