#include "stm32l1xx.h"

#include <math.h>
#define pi 3.1415
#include <stdio.h>

void TIM4_PWM_Config();
void TIM4_PWM_Set(uint16_t pulseWidth);

void TIM5_Config();

int main(void)
{
    TIM4_PWM_Config(); // Met le PWM sur le Timer 4
    TIM5_Config(); // Configure le compteur-horloge

    float duty = 0;
    float step;

    float PB6, PB7, PB8, t = 0;

    TIM_SetCounter(TIM5,0); // Initialise
    TIM_Cmd(TIM5 , ENABLE); // Démarre le compteur-horloge

    while(1)
    {/*
        duty *= step; // duty augmente de 1% si <32000 et diminue de 1% si >32000
        if (duty > 32000) {
            duty = 32000;
            step = 0.99;
        }
        if (duty < 0.001*32000) {
            duty = 0.001*32000;
            step = 1.01;
        }
*/
        t = 1.0*TIM_GetCounter(TIM5)/1000; 
// TIM5 est configuré pour compter les ms. 1000ms = 1s

        PB6 = 32000.0*powf(sinf(t*2*pi/30),2);
        TIM_SetCompare1(TIM4,PB6);              // Vert
        PB7 = 32000.0*powf(cosf(t*2*pi/22),2);
        TIM_SetCompare2(TIM4,PB7);              // Bleu
        PB8 = 32000.0*powf(sinf(t*2*pi/12),2);
        TIM_SetCompare3(TIM4,PB8);              // Rouge

        // TIM4_PWM_Set(duty); // % de la période de clignotement de la LED.
        // PWM génère des impulsions en fonction de la valeur rentrée.
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

    /* Configurer le comparateur de sortie OC1 de TIM4 */
    /* pour faire du PWM */
    TIM_OCInitTypeDef timer_4_oc_1;
    TIM_OCStructInit(&timer_4_oc_1);
    timer_4_oc_1.TIM_OCMode = TIM_OCMode_PWM1;
    timer_4_oc_1.TIM_Pulse = 1000;
    timer_4_oc_1.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OC1Init(TIM4,&timer_4_oc_1);

    /* Configurer le comparateur de sortie OC2 de TIM4 */
    /* pour faire du PWM */
    TIM_OCInitTypeDef timer_4_oc_2;
    TIM_OCStructInit(&timer_4_oc_2);
    timer_4_oc_2.TIM_OCMode = TIM_OCMode_PWM1;
    timer_4_oc_2.TIM_Pulse = 1000;
    timer_4_oc_2.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OC2Init(TIM4,&timer_4_oc_2);

    /* Configurer le comparateur de sortie OC3 de TIM4 */
    /* pour faire du PWM */
    TIM_OCInitTypeDef timer_4_oc_3;
    TIM_OCStructInit(&timer_4_oc_3);
    timer_4_oc_3.TIM_OCMode = TIM_OCMode_PWM1;
    timer_4_oc_3.TIM_Pulse = 1000;
    timer_4_oc_3.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OC3Init(TIM4,&timer_4_oc_3);

    /*Activer GPIOB sur AHB */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);
    /* Configurer PB7 comme "Alternative Function" pour preparer son utilisation en PWM */
    GPIO_InitTypeDef gpio_b;
    GPIO_StructInit(&gpio_b);
    gpio_b.GPIO_Mode = GPIO_Mode_AF;
    gpio_b.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;
    gpio_b.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOB, &gpio_b);

    /* relier PB7 a TIM4/OC2 */
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4); // Vert
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4); // Bleu
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_TIM4); // Rouge
}

// Changer rapport cyclique
void TIM4_PWM_Set(uint16_t pulseWidth)
{
    TIM_SetCompare2(TIM4,pulseWidth);
}

void TIM5_Config() // On utilise le compteur pour mesurer le temps tel une horloge
{
    /* Activer TIM5 sur APB1 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
    /* Configurer TIM5 : prescalaire à 1 ms, periode au maximum */
    // Fréquence du CPU 16 MHz
    TIM_TimeBaseInitTypeDef DelayTimer; // Crée la structure de configuration
    TIM_TimeBaseStructInit(&DelayTimer); // Initialise la structure
    DelayTimer.TIM_Prescaler = 16000-1; // Défini le prescalaire
    DelayTimer.TIM_Period = 0xFFFFFFFFU; // Taille du compteur (pleine)
    TIM_TimeBaseInit(TIM5,&DelayTimer); // Configure le timer

    /* Tprescalaire = Prescalaire/16x10^6 (fréquence de l'horloge du microcontroleur) */
}
