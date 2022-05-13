void GPIO_METAL(void)
{
// Faut connaitre les adresses de la memory map.
// ## 1. Activer GPIOB (port B)
    long * RCC_AHBENR =( long *) (0x40023800 + 0 x1C ) ;
    (* RCC_AHBENR ) |= (1 < <1) ;

// ## 2. PB7 en tant que sortie
    long * GPIOB_MODER =( long *) (0x40020400 + 0 x0 ) ;
    (* GPIOB_MODER ) &= ~(0 b11 < <14) ;
    (* GPIOB_MODER ) |= 0 b01 < <14

// ## 1. Allumer la LED
    (* GPIOB_ODR ) |= 0 b1 < <7;
    // ## 2. Eteindre la LED
    (* GPIOB_ODR ) &= ~(0 b1 < <7) ;
}

void GPIO_Biblio()
{
// Active le coté port B ou A du microcontrôleur
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIO{A|B}, {ENABLE|DISABLE});
// Défini une structure qui sera utilisée par le GPIO
    GPIO_InitTypeDef {Nom};
// Initialise la structure utilisé
    GPIO_StructInit( & {Nom});
// Configure la structure en entrée ou sortie
    {Nom}.GPIO_Mode = GPIO_Mode_{OUT|IN};
// Choisi le pin numéro
    {Nom}.GPIO_Pin = GPIO_Pin_{Numéro};
// Ecrit les paramètres de la structure du coté qu'on choisi
    GPIO_Init(GPIO{A|B}, & {Nom});
    while (1) {

// Lit le pin {Numéro} 
    GPIO_ReadInputDataBit(GPIO{A|B}, GPIO_Pin_{Numéro});
// Défini a 1 le pin choisi
    GPIO_SetBits(GPIO{A|B}, GPIO_Pin_{Numéro});
// Défini a 0 le pin choisi
    GPIO_ResetBits(GPIO{A|B}, GPIO_Pin_{Numéro});
}

/*
Prescalaire => Compte x.
A x+1, compteur + 1 ;
Ttimer = T_CPUClock * (prescaler) * (periode) = (prescaler)(periode)/f_CPUClock
Peut etre prescaler+1 & periode+1
f_CPUClock = 16 MHz
*/
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
// Prescalaire = ((16000−1)+1)*1/(16e6)
}
// Fonction Delai en millisecondes
void TIM5_delay_ms(int delay)
{
    TIM_SetCounter(TIM5,0); // Initialise le compteur
    TIM_Cmd(TIM5, ENABLE); // Active le compteur
    while(TIM_GetCounter(TIM5)<delay) {} // Attend que l'on atteigne le delai
    TIM_Cmd(TIM5, DISABLE); // Désactive le compteur
}

/* Sortie Analogique
Génère un signal carré pendant x% de la période.
Système lent voit tension continue = x% de Vmax.
*/
void TIM4_PWM_Config()
{
    /*Activer TIM4 sur APB1 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
    /* Configurer TIM4 a 20 ms */
    TIM_TimeBaseInitTypeDef timer_4;
    TIM_TimeBaseStructInit(&timer_4);
    timer_4.TIM_Period = 32000; // Diminuer le timer et augmenter le prescalaire augmente la période d'allumage de la diode.
    timer_4.TIM_Prescaler = 1;
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
}
// callback pour l'interruption externe EXTI0_IRQ
void EXTI0_IRQHandler(void) // Le code a executer quand il y a interruption.
{
    if(EXTI_GetITStatus(EXTI_Line0) != RESET) // Permet de faire une seule fois l'interruption au lieu de la répéter.
    {
        /* Clear the EXTI line 0 pending bit (enlève le flag) */
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

// ### EXTI0 sur PA0
/* Interruption
Interruption externe
*/
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
    EXTI_InitTypeDef EXTI0_params; // Param. des signaux qui declencheront l'appel de EXTI0_IRQHandler()
    EXTI_StructInit(&EXTI0_params);
    EXTI0_params.EXTI_Line = EXTI_Line0; // Observe Ligne 0
    EXTI0_params.EXTI_LineCmd = ENABLE; // Active Ligne
    EXTI0_params.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
// EXTI_Trigger_Falling ; EXTI_Trigger_Rising ; EXTI_Trigger_Rising_Falling

    EXTI_Init(&EXTI0_params);

    /* Activer l'interruption dans le NVIC */
    NVIC_InitTypeDef nvic;
    NVIC_Init(&nvic);
    nvic.NVIC_IRQChannel = EXTI0_IRQn;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
}

// Interruption Périodique
// Attend un délai pour interruption
void TIM2_IRQ_Config()
{
    /*Activer TIM2 sur APB1 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
    /* Configurer TIM2 a 500 ms
    timer_2 . TIM_Prescaler = 16000 -1;
    timer_2 . TIM_Period = 500;
    44 kHz -> 363 period & 0 prescaler.
    16000000/44000 = 363.
    */
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


// ### ADC1 sur PA5
// Analogic to Decimal Converter
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
    ADC_InitTypeDef adc_1;
    ADC_StructInit(&adc_1);
    adc_1.ADC_NbrOfConversion = 1; // simple acquisition
    adc_1.ADC_Resolution = ADC_Resolution_12b; // ADC1 en 12 bit
// 2^12 = 4096 donc la valeur max du convertisseur est 4096-1. La valeur min est 0.
// Pour 3V = 4095. T = Vmax*Valeur/(2^12-1).
    ADC_Init(ADC1, &adc_1); // Init
    ADC_Cmd(ADC1, ENABLE); // Activation
}
// Acquisition d'une valeur
// PA5 : ch = ADC_Channel_5
uint16_t ADC1_Get(uint8_t ch)
{
    ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_4Cycles);
    ADC_SoftwareStartConv(ADC1);
    while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET) {}
    return  ADC_GetConversionValue(ADC1);
}
// ### DAC1 (DAC Channel 1) sur PA4
// Digital to Analog Converter
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
