#include "stm32l1xx.h"

#include <math.h>
#include <stdlib.h>

void DAC1_Config();
void DAC1_Set(uint16_t value);
void GPIOA_PA0_Config();

void TIM2_IRQ_Config();

unsigned int note_periode[8] = {611,544,484,458,408,363,323,306};


float* T;
int n = 0;
int interrupteur = 0;

void buildSawTooth() {
    for(int k=0; k<100;k++) {
        T[k] = 2047-511 + (k*1022/100);
    }
}
void buildTriangle() {
	int k;
        for(int k=0; k<100/2;k++) {
            T[k] = 2047-511 + (k*1022*2/100);
        }
        for(; k<100;k++) {
            T[k] = 2047 + 511 -((k-100/2)*1022*2/100);
        }
}
void buildSinus() {
	for (int k = 0; k < 100; k++) {
		T[k] = 511 * sin(2 * 3.14159 * k / 100) + 2047;
	}
}

int main(void) {
	TIM2_IRQ_Config();
	DAC1_Config();
	GPIOA_PA0_Config();

	// # LED sur PB7
	/* Activer GPIOB sur AHB */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	/* Configurer PB7 comme sortie tout-ou-rien */
	GPIO_InitTypeDef gpio_b;
	GPIO_StructInit(&gpio_b);
	gpio_b.GPIO_Mode = GPIO_Mode_OUT;
	gpio_b.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOB, &gpio_b);

	T = malloc(100 * sizeof(float));

	//buildSinus();
	//buildSawTooth();
	//buildTriangle();

	GPIOA_PA0_Config();

	int prev_switch_status = 0;

	while (1) {
		int switch_status = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
		if (switch_status == Bit_SET && prev_switch_status == 0) {
			// A REMPLIR : ce que l'on doit executer si le bouton est appuye
			interrupteur++; // Nb d'appuie sur l'interrupteur
			TIM_Cmd(TIM2, DISABLE);
			TIM_SetCounter(TIM2, 0);
			TIM_TimeBaseInitTypeDef timer_2;
			TIM_TimeBaseStructInit(&timer_2);
			timer_2.TIM_Prescaler = 0;
			timer_2.TIM_Period = note_periode[interrupteur%8]; // A 8, on redescend à 0
			TIM_TimeBaseInit(TIM2, &timer_2);
			TIM_Cmd(TIM2, ENABLE);
		}
		prev_switch_status = switch_status;
	}
}

// callback pour l'interruption periodique associee a TIM2
void TIM2_IRQHandler() {
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		GPIO_ToggleBits(GPIOB, GPIO_Pin_7);
		DAC1_Set(T[n % 100]);
		n++;
	}
}

// ### TIMER 2 + IRQ a 500 ms
// Configuration Timer 2 a 500 ms
// avec emission d'IRQ : execute periodiquement TIM2_IRQHandler()
void TIM2_IRQ_Config() {
	/*Activer TIM2 sur APB1 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	/* Configurer TIM2 a 500 ms */
	TIM_TimeBaseInitTypeDef timer_2;
	TIM_TimeBaseStructInit(&timer_2);
	timer_2.TIM_Prescaler = 0;
	timer_2.TIM_Period = 363;
	TIM_TimeBaseInit(TIM2, &timer_2);
	TIM_SetCounter(TIM2, 0);
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

// ### DAC1 (DAC Channel 1) sur PA4
// Configuration
void DAC1_Config() {
	/*Activer GPIOA sur AHB */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	/* Configurer PA4 en mode analogique*/
	GPIO_InitTypeDef gpio_a;
	GPIO_StructInit(&gpio_a);
	gpio_a.GPIO_Mode = GPIO_Mode_AN;
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

void DAC1_Set(uint16_t value) {
	DAC_SetChannel1Data( DAC_Align_12b_R, value);
	DAC_SoftwareTriggerCmd( DAC_Channel_1, ENABLE);

}

void GPIOA_PA0_Config() {
	// switch PA0
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	GPIO_InitTypeDef switch_PA;
	GPIO_StructInit(&switch_PA);
	switch_PA.GPIO_Mode = GPIO_Mode_IN;
	switch_PA.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOB, &switch_PA);
}
