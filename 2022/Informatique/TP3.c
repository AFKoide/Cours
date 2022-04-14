#include "stm32l1xx.h"

void ADC1_Config();
uint16_t ADC1_Get(uint8_t ch);

int main(void)
{
    ADC1_Config();
    for(;;)
    {
        volatile uint16_t adcVal = 0;
        adcVal = ADC1_Get(ADC_Channel_5);
        adcVal = ADC1_Get(ADC_Channel_5);
        adcVal = ADC1_Get(ADC_Channel_5);
        adcVal = ADC1_Get(ADC_Channel_5);
        adcVal = ADC1_Get(ADC_Channel_5);
        // On branche un potar relié à 5V et la terre. En utilisant "Step Over", on peut relever la tension à cette borne.
        // ici adcVal vaut la valeur lue sur PA54.
        // Quand la résistance est égale à 0, on lit dans la variable 4096.
        // Quand la valeur de la résistance est max, on lit 0.
        // La valeur est logique car on utilise un ADC 12 bits. La valeur max est donc 4096, ou 0b111111111111.
        // Les soudures de la carte ne sont pas parfaites, il est donc normal que l'on ne trouve pas une valeur précise. De plus, on peut observer des variations selon la distance de nos mains à l'appareil.
    }
}

// ### ADC1 sur PA5
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
    /* Configurer ADC1 en 12 bit, simple acquisition */
    ADC_InitTypeDef adc_1;
    ADC_StructInit(&adc_1);
    adc_1.ADC_NbrOfConversion = 1;
    adc_1.ADC_Resolution = ADC_Resolution_12b;
    ADC_Init(ADC1, &adc_1);
    ADC_Cmd(ADC1, ENABLE);
}

// Acquisition d'une valeur
// PA5 : ch = ADC_Channel_5
uint16_t ADC1_Get(uint8_t ch)
{
    ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_4Cycles);
    ADC_SoftwareStartConv(ADC1);
    while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET)
        ;

    return  ADC_GetConversionValue(ADC1);
}

// Dans les bibliothèques ;
uint16_t ADC1_Get(uint8_t ch)
{
    ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_4Cycles);
    ADC_SoftwareStartConv(ADC1);
    while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET)
        ;

    return  ADC_GetConversionValue(ADC1);
}
