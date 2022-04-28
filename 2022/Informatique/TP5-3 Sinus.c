#include "stm32l1xx.h"

#include <math.h>
#define pi 3.1415
#include <stdlib.h>

void DAC1_Config ();
void DAC1_Set(uint16_t value);

void main()
{
    Convertisseur = (float *) malloc( 101 * sizeof(float) ); 
    Tension = (float *) malloc( 101 * sizeof(float) );
// Alloue pour une chaine type float 101 caractères de la taille d'un float.
// 101 car on a une chaine de 100 caractères plus \0.

    for (int k=0 ; k<100 ; k++)
    {
        Convertisseur[k] = 511.0*sin(k*2*pi/100) + 2047;
        Tension[k] = 3*Convertisseur[k] / 4095;
// 2^12 = 4096 donc la valeur max du convertisseur est 4096-1. La valeur min est 0.
// Donc pour 3V, le convertisseur est égal à 4095. On fait un produit en croix
    }
//    Convertisseur_min = 2047 - 511; // Valeur min est 2047 plus la valeur quand sin = -1;
//    Tension_min = 3*Convertisseur_min / 4095; // Conversion (produit en croix).
//    Convertisseur_max = 2047 + 511; // Valeur min est 2047 plus la valeur quand sin = 1;
//    Tension_max = 3*Convertisseur_max / 4095; // Conversion (produit en croix).
//    Convertisseur_moy = 2047 + 0; // Valeur min est 2047 plus la valeur quand sin = 0;
//    Tension_moy = 3*Convertisseur_moy / 4095; // Conversion (produit en croix).

    DAC1_Config ();
    uint16_t dac_value = 0;
    while (1) {        
        if(k <= 0)
        {
            dac_value = Tension[k];
            k = 100;
        }
        DAC1_Set(dac_value);

        for(int i=0; i <200000; i++) { } // perdre du temps pour "attendre"
        k --;
    }
}

// ### DAC1 (DAC Channel 1) sur PA4
// Configuration
void DAC1_Config ()
{
/* Activer GPIOA sur AHB */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA , ENABLE);
    V. Interruptions 25
    /* Configurer PA4 en mode analogique */
    GPIO_InitTypeDef gpio_a;
    GPIO_StructInit (& gpio_a);
    gpio_a.GPIO_Mode = GPIO_Mode_AN;
    gpio_a.GPIO_Pin = GPIO_Pin_4;
    GPIO_Init(GPIOA , &gpio_a);
    /* Activer DAC sur APB1 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC , ENABLE);
    /* Configurer DAC1 avec parametres par defaut */
    DAC_InitTypeDef dac_1;
    DAC_StructInit (&dac_1);
    DAC_Init(DAC_Channel_1 , &dac_1);
    /* Activer DAC1 */
    DAC_Cmd(DAC_Channel_1 , ENABLE);
}

void DAC1_Set(uint16_t value)
{
    DAC_SetChannel1Data( DAC_Align_12b_R , value );
    DAC_SoftwareTriggerCmd( DAC_Channel_1 , ENABLE );
}