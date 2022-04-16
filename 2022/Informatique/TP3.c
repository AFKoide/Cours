#include "stm32l1xx.h"

// Initialisation
void ADC1_Config();
uint16_t ADC1_Get(uint8_t ch);

void TIM5_Config();
void TIM5_delay_ms(int delay);
void Init_GPIO();

void LED(uint16_t adcVal);


int main(void)
{
    ADC1_Config();

    TIM5_Config();
    Init_GPIO();

    volatile uint16_t adcVal = 0;
    for(;;)
    {
        adcVal = ADC1_Get(ADC_Channel_5);
        // On branche un potar relié à 5V et la terre. En utilisant "Step Over", on peut relever la tension à cette borne.
        // ici adcVal vaut la valeur lue sur PA54.
        // Quand la résistance est égale à 0, on lit dans la variable 4096.
        // Quand la valeur de la résistance est max, on lit 0.
        // La valeur est logique car on utilise un ADC 12 bits. La valeur max est donc 4096, ou 0b111111111111.
        // Les soudures de la carte ne sont pas parfaites, il est donc normal que l'on ne trouve pas une valeur précise. De plus, on peut observer des variations selon la distance de nos mains à l'appareil.
        LED(adcVal);

    }
}

void LED(uint16_t adcVal)
{
	/* GPIO_SetBits(GPIOB,GPIO_Pin_7);
    unsigned int time = 1000;
    if(adcVal<1024) {
        time -= 900;
        TIM5_delay_ms(time/40);
        // On allume la led 10% du temps.
    }
    else if(adcVal<2048) {
        time -= 800;
        TIM5_delay_ms(time/40);
        // Ici, la led est allumée pendant 20% du temps.
    }
    else if(adcVal<3072) {
        time -= 500;
        TIM5_delay_ms(time/40);
        // 50% du temps.
    }
    else {
        time -= 100;
		TIM5_delay_ms(time);
        // 90%
    }
    GPIO_ResetBits(GPIOB,GPIO_Pin_7);
    TIM5_delay_ms((1000-time)/40);*/
	// En divisant les délais par 40, les clignotements sont si rapide que l'on a l'impression que la led est en permanence allumée.
	// Faire varier la résistance change la luminosité.


	// On fait varier la vitesse graduellement.
    unsigned int time;
    GPIO_SetBits(GPIOB,GPIO_Pin_7);

    time = 1000*adcVal/4096;
    TIM5_delay_ms(time);

    GPIO_ResetBits(GPIOB,GPIO_Pin_7);
    TIM5_delay_ms(1000-time);
    // En divisant par 40, on observe la même chose que précédemment
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


// LED
void Init_GPIO() {
    /* Activer GPIOB sur AHB */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);
    /* Configurer PB7 */
    GPIO_InitTypeDef gpio_b;
    GPIO_StructInit(&gpio_b);
    gpio_b.GPIO_Mode = GPIO_Mode_OUT;
    gpio_b.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOB,&gpio_b);
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
