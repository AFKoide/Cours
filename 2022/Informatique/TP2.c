#include "stm32l1xx.h"

int main() {
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);
    //Active le périphérique GPIOB

    GPIO_InitTypeDef leds_PB; //Structure pour leds
    GPIO_StructInit(&leds_PB); //Init la structure
    leds_PB.GPIO_Mode = GPIO_Mode_OUT; //Défini le mode
    leds_PB.GPIO_Pin = GPIO_Pin_7; //Choisi le pin 7
    GPIO_Init(GPIOB,&leds_PB); //Compile le programme

    while(1) {
        GPIO_SetBits(GPIOB,GPIO_Pin_7); //Allume la led
        for(int k=0; k<100000;k++){} //Attente
        GPIO_ResetBits(GPIOB,GPIO_Pin_7); //Eteind la led
        for(int k=0; k<100000;k++){} //Attente
    }

    return 0;
}
