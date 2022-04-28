#include "stm32l1xx.h"

void DAC1_Config();
void DAC1_Set(uint16_t value);


int main(void)
{
    DAC1_Config();
    uint16_t dac_value = 0;
    while(1) {
        if(dac_value <= 0) {
            dac_value = 0xfff;
        }
        DAC1_Set(dac_value);
        for(int k=0; k<200000; k++) { } // perdre du temps pour "attendre"
        dac_value = dac_value-10;
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
