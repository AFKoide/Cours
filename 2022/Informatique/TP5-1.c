#include "stm32l1xx.h"

void IRQ_EXTI0_Config();

int main(void)
{
    // # LED sur PB7
    /* Activer GPIOB sur AHB */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);
    /* Configurer PB7 comme sortie tout-ou-rien */
    GPIO_InitTypeDef gpio_b;
    GPIO_StructInit(&gpio_b);
    gpio_b.GPIO_Mode = GPIO_Mode_OUT;
    gpio_b.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_6;
    GPIO_Init(GPIOB,&gpio_b);
    /* allumer la LED*/
    GPIO_SetBits(GPIOB,GPIO_Pin_7);
    // configuration de l'interruption
    IRQ_EXTI0_Config();

    while(1) {

    }
}

// callback pour l'interruption externe EXTI0_IRQ
void EXTI0_IRQHandler(void) // Le code a executer quand il y a interruption
{
    if(EXTI_GetITStatus(EXTI_Line0) != RESET) // Permet de faire une seule fois l'interruption au lieu de la répéter
    {
        /* Clear the EXTI line 0 pending bit (enlève le flag) */
        EXTI_ClearITPendingBit(EXTI_Line0);
        GPIO_ToggleBits(GPIOB,GPIO_Pin_7); // Inverse état du pin 7
        GPIO_ToggleBits(GPIOB,GPIO_Pin_6); // Inverse état du pin 6
    }
}

// ### EXTI0 sur PA0
// Configuration
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
    EXTI0_params.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
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
