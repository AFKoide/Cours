#include "stm32l1xx.h"

#include <math.h>
#define pi 3.1415

void DAC1_Config();
void DAC1_Set(uint16_t value);

void TIM2_IRQ_Config();

float * Tension;
int n = 0;

int main()
{
	Tension = malloc( 100 * sizeof(float) );
// Alloue pour une chaine type float 100 caractères de la taille d'un float.
// 101 car on a une chaine de 100 caractères plus \0.

    for (int k=0 ; k<100 ; k++)
    {
        Tension[k] = 511*sin(k*2*pi/100) + 2047;
// 2^12 = 4096 donc la valeur max du convertisseur est 4096-1. La valeur min est 0.
// Donc pour 3V, le convertisseur est égal à 4095. On fait un produit en croix
    }
//    Convertisseur_min = 2047 - 511; // Valeur min est 2047 plus la valeur quand sin = -1;
//    Tension_min = 3*Convertisseur_min / 4095; // Conversion (produit en croix).
//    Convertisseur_max = 2047 + 511; // Valeur min est 2047 plus la valeur quand sin = 1;
//    Tension_max = 3*Convertisseur_max / 4095; // Conversion (produit en croix).
//    Convertisseur_moy = 2047 + 0; // Valeur min est 2047 plus la valeur quand sin = 0;
//    Tension_moy = 3*Convertisseur_moy / 4095; // Conversion (produit en croix).

    DAC1_Config();
    TIM2_IRQ_Config();

    while (1) {
    }
}

// ### DAC1 (DAC Channel 1) sur PA4
// Configuration
void DAC1_Config ()
{
    /*Activer GPIOA sur AHB */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    /* Configurer PA4 en mode analogique*/
    GPIO_InitTypeDef gpio_a;
    GPIO_StructInit(&gpio_a);
    gpio_a.GPIO_Mode  = GPIO_Mode_AN;
    gpio_a.GPIO_Pin = GPIO_Pin_4;
    GPIO_Init(GPIOA, &gpio_a);

    /*Activer DAC sur APB1 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
    /* Configurer DAC1 avec parametres par defaut */
    DAC_InitTypeDef dac_1;
    DAC_StructInit(&dac_1);
    DAC_Init(DAC_Channel_1, &dac_1);
    /* Activer DAC1 */
    DAC_Cmd(DAC_Channel_1, ENABLE);
}
void DAC1_Set(uint16_t value)
{
    DAC_SetChannel1Data(DAC_Align_12b_R , value);
    DAC_SoftwareTriggerCmd(DAC_Channel_1 , ENABLE);
}

// ### TIMER 2 + IRQ a 500 ms
// Configuration Timer 2 a 500 ms
// avec emission d'IRQ : execute periodiquement TIM2_IRQHandler()
void TIM2_IRQ_Config()
{
    /*Activer TIM2 sur APB1 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
    /* Configurer TIM2 a 500 ms */
    TIM_TimeBaseInitTypeDef timer_2;
    TIM_TimeBaseStructInit(&timer_2);
    timer_2.TIM_Prescaler = 0;
    timer_2.TIM_Period = 363;
    TIM_TimeBaseInit(TIM2,&timer_2);
    TIM_SetCounter(TIM2,0);
    TIM_Cmd(TIM2, ENABLE);

    /* Associer une interruption a TIM2 */
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    NVIC_InitTypeDef nvic;
    /* Configuration de l'interruption */
    nvic.NVIC_IRQChannel = TIM2_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
}

// callback pour l'interruption periodique associee a TIM2
void TIM2_IRQHandler() {
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        DAC1_Set(Tension[n%100]);
        n++;
    }
}
