#include "stm32l1xx.h"

#include <stdio.h>
#include <string.h>
#include <math.h>
#define pi 3.1415

void IRQ_EXTI0_Config();
void TIM2_IRQ_Config()

unsigned int init_prescalaire[8] = {611,544,484,458,408,363,323,306};
int i = 0;

void main(void) 
{
/*    struct note {
    char nom[4];
    unsigned char frequence;
    unsigned char prescalaire;
};
    struct clavier[20];*/

    // char init_frequence[8] = {262,294,330,350,392,440,494,524};
    // char init_prescalaire[8] = {611,544,484,458,408,363,323,306};

    IRQ_EXTI0_Config();
    TIM2_IRQ_Config();

    
    while(1)
    {

    }
}

void EXTI0_IRQHandler(void) // Le code a executer quand il y a interruption.
{
    if(EXTI_GetITStatus(EXTI_Line0) != RESET) // Permet de faire une seule fois l'interruption au lieu de la répéter.
    {
        /* Clear the EXTI line 0 pending bit (enlève le flag) */
        EXTI_ClearITPendingBit(EXTI_Line0);

        if(i>=8)
            i=0;
        else
            i++;
    }
}

void IRQ_EXTI0_Config()
{
    // # Interrupteur
    /* Activer GPIOA sur AHB */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
    /* Configurer PB7 comme entree tout-ou-rien */
    GPIO_InitTypeDef gpio_a;
    GPIO_StructInit(&gpio_a);
    gpio_a.GPIO_Mode = GPIO_Mode_IN;
    gpio_a.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init(GPIOA,&gpio_a);

    /* Activer SYSCFG sur APB2
     * pour permettre l'utilisation des interruptions externes */
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_SYSCFG,ENABLE);
    /* Declarer PA0 comme source d'interruption */
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);
    /* Param. des signaux qui declencheront l'appel de EXTI0_IRQHandler()
     * autrement dit on parametre les signaux associes a la "ligne 0"
     * ici : sur front montant ("Trigger_Rising")
     */
    EXTI_InitTypeDef EXTI0_params;
    EXTI_StructInit(&EXTI0_params);
    EXTI0_params.EXTI_Line = EXTI_Line0;
    EXTI0_params.EXTI_LineCmd = ENABLE;
    EXTI0_params.EXTI_Trigger = EXTI_Trigger_Rising;
// Front Descendant : EXTI_Trigger_Falling
// Front Montant : EXTI_Trigger_Rising
// Front : EXTI_Trigger_Rising_Falling

    EXTI_Init(&EXTI0_params);

    /* Activer l'interruption dans le NVIC */
    NVIC_InitTypeDef nvic;
    NVIC_Init(&nvic);
    nvic.NVIC_IRQChannel = EXTI0_IRQn;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
}

void TIM2_IRQ_Config()
{
    /*Activer TIM2 sur APB1 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
    /* Configurer TIM2 a 500 ms */
    TIM_TimeBaseInitTypeDef timer_2;
    TIM_TimeBaseStructInit(&timer_2);
    timer_2.TIM_Prescaler = init_prescalaire[i];
    timer_2.TIM_Period = 0xFFFFFFFFU; 

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