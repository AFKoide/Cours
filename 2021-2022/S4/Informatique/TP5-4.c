#include "stm32l1xx.h"

#include <math.h>
#define pi 3.14159

void DAC1_Config();
void DAC1_Set(uint16_t value);

void TIM2_IRQ_Config();

void GPIOA_PA0_Config();

unsigned int note_prescalaire[8] = {611,544,484,458,408,363,323,306};
float* Tension;
int i = 0, n = 0;

int main(void)
{
    TIM2_IRQ_Config();
    DAC1_Config();
    GPIOA_PA0_Config();

    Tension = malloc(100 * sizeof(float));
    for(int m=0;m<100;m++) {
    	Tension[m] = 511 * sin(2*pi*m/100) + 2047;
    }

    int prev_switch_status = 0;
    while(1)
    {
        int switch_status = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0);
        if(switch_status == Bit_SET && prev_switch_status == 0) {
            i++;
        }
        prev_switch_status = switch_status;
    }
}

void GPIOA_PA0_Config()
{
    // switch PA0
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
    GPIO_InitTypeDef switch_PA;
    GPIO_StructInit(&switch_PA);
    switch_PA.GPIO_Mode = GPIO_Mode_IN;
    switch_PA.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init(GPIOB,&switch_PA);
}

void TIM2_IRQ_Config()
{
    /*Activer TIM2 sur APB1 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
    /* Configurer TIM2 a 500 ms */
    TIM_TimeBaseInitTypeDef timer_2;
    TIM_TimeBaseStructInit(&timer_2);
    timer_2.TIM_Prescaler = 0;
    timer_2.TIM_Period = note_prescalaire[i%8];

// On retrouve 2kHz, la moitié de la fréquence prévue, à cause du fonctionnement de l'horloge
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
void TIM2_IRQHandler()
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
        DAC1_Set(Tension[n%100]);
        n++;
    }
}


// ### DAC1 (DAC Channel 1) sur PA4
// Configuration
void DAC1_Config()
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
    DAC_SetChannel1Data( DAC_Align_12b_R, value );
    DAC_SoftwareTriggerCmd( DAC_Channel_1, ENABLE );
}
