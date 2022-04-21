#include "stm32l1xx.h"

void TIM4_PWM_Config();
void TIM4_PWM_Set(uint16_t pulseWidth);

int main(void)
{
    TIM4_PWM_Config(); // Met le PWM sur le Timer 4

    float duty = 0;
    float step;
    while(1)
    {
        duty *= step; // duty augmente de 1% si <32000 et diminue de 1% si >32000
        if (duty > 32000) {
            duty = 32000;
            step = 0.99;
        }
        if (duty < 0.001*32000) {
            duty = 0.001*32000;
            step = 1.01;
        }

        TIM4_PWM_Set(duty); // % de la période de clignotement de la LED.
        // PWM génère des impulsions en fonction de la valeur rentrée.
        int k;
        for(k=0; k<3000; k++) { // Attente.
        }
    }
}

// ## PWM via TIM4 sur PB7
// Configuration
void TIM4_PWM_Config()
{
    /*Activer TIM4 sur APB1 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
    /* Configurer TIM4 a 20 ms */
    TIM_TimeBaseInitTypeDef timer_4;
    TIM_TimeBaseStructInit(&timer_4);
    timer_4.TIM_Period = 3200; // Diminuer le timer et augmenter le prescalaire augmente la période d'allumage de la diode.
    timer_4.TIM_Prescaler = 10;
    TIM_TimeBaseInit(TIM4,&timer_4);
    TIM_Cmd(TIM4, ENABLE);

    /* Configurer le comparateur de sortie OC2 de TIM4 */
    /* pour faire du PWM */
    TIM_OCInitTypeDef timer_4_oc_2;
    TIM_OCStructInit(&timer_4_oc_2);
    timer_4_oc_2.TIM_OCMode =        TIM_OCMode_PWM1;
    timer_4_oc_2.TIM_Pulse = 1000;
    timer_4_oc_2.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OC2Init(TIM4,&timer_4_oc_2);

    /*Activer GPIOB sur AHB */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);
    /* Configurer PB7 comme "Alternative Function" pour preparer son utilisation en PWM */
    GPIO_InitTypeDef gpio_b;
    GPIO_StructInit(&gpio_b);
    gpio_b.GPIO_Mode = GPIO_Mode_AF;
    gpio_b.GPIO_Pin = GPIO_Pin_7;
    gpio_b.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOB, &gpio_b);

    /* relier PB7 a TIM4/OC2 */
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4);
}

// Changer rapport cyclique
void TIM4_PWM_Set(uint16_t pulseWidth)
{
    TIM_SetCompare2(TIM4,pulseWidth);
}
