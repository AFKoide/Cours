#include "stm32l1xx.h"

void TIM2_IRQ_Config();

int main(void)
{
        TIM2_IRQ_Config();

    // # LED sur PB7
    /* Activer GPIOB sur AHB */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);
    /* Configurer PB7 comme sortie tout-ou-rien */
    GPIO_InitTypeDef gpio_b;
    GPIO_StructInit(&gpio_b);
    gpio_b.GPIO_Mode = GPIO_Mode_OUT;
    gpio_b.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOB,&gpio_b);

    while(1) {

    }
}

// callback pour l'interruption periodique associee a TIM2
void TIM2_IRQHandler() {
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        GPIO_ToggleBits(GPIOB, GPIO_Pin_7);
    }
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
    timer_2.TIM_Prescaler = 0; // Prescalaire et Période ont au final le même résultat
    timer_2.TIM_Period = 364-1; // Cependant, on utilise prescalaire pour compter du temps et période pour mesurer le temps.
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
