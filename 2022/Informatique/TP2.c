#include "stm32l1xx.h"

// fonctions pour configurer et utiliser le timer 5.
// voir apres le main
void TIM5_Config();
void TIM5_delay_ms(int delay);

int main(void)
{
    TIM5_Config();

    /* Activer GPIOB sur AHB */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);
    /* Configurer PB7 */
    GPIO_InitTypeDef gpio_b;
    GPIO_StructInit(&gpio_b);
    gpio_b.GPIO_Mode = GPIO_Mode_OUT;
    gpio_b.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOB,&gpio_b);

    while(1) {
        GPIO_SetBits(GPIOB,GPIO_Pin_7);
        TIM5_delay_ms(500);
        GPIO_ResetBits(GPIOB,GPIO_Pin_7);
        TIM5_delay_ms(500);
    }
}

// ### TIM5 pour delai
// Configuration
void TIM5_Config()
{
    /* Activer TIM5 sur APB1 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
    /* Configurer TIM5 : prescalaire à 1 ms, periode au maximum*/
    
    TIM_TimeBaseInitTypeDef DelayTimer; // Crée la structure
    TIM_TimeBaseStructInit(&DelayTimer); // Initialise la structure
    DelayTimer.TIM_Prescaler = 16000-1; // Défini le prescalaire
    DelayTimer.TIM_Period = 0xFFFFFFFFU; // Taille que fera la période (pleine)
    TIM_TimeBaseInit(TIM5,&DelayTimer); // Configure le timer
}

// Fonction Delai en millisecondes
void TIM5_delay_ms(int delay)
{
    TIM_SetCounter(TIM5,0);
    TIM_Cmd(TIM5, ENABLE); // Active le compteur
    while(TIM_GetCounter(TIM5)<delay) {} // Attend que l'on atteigne le delai
    TIM_Cmd(TIM5, DISABLE); // Désactive le compteur
}
