#include "stm32l1xx.h"

#include <math.h>
#define pi 3.14
#include <stdlib.h>

void DAC1_Config ();
void DAC1_Set(uint16_t value);

void main()
{
    Convertisseur = (float *) malloc( 101 * sizeof(float) ); 
    Tension = (float *) malloc( 101 * sizeof(float) );
// Alloue pour une chaine type float 101 caractères de la taille d'un float.
// 101 car on a une chaine de 100 caractères plus \0.

    for (int k = 0 ; k<100 ; k++)
    {
        Convertisseur[k] = 511.0*sin(k*2*pi/100) + 2047;
        Tension[k] = 3*x[k] / 4095;
// 2^12 = 4096 donc la valeur max du convertisseur est 4096-1. La valeur min est 0.
// Donc pour 3V, le convertisseur est égal à 4095. On fait un produit en croix
    }
//    Convertisseur_min = 2047 - 511; // Valeur min est 2047 plus la valeur quand sin = -1;
//    Tension_min = 3*Convertisseur_min / 4095; // Conversion (produit en croix).
//    Convertisseur_max = 2047 + 511; // Valeur min est 2047 plus la valeur quand sin = 1;
//    Tension_max = 3*Convertisseur_max / 4095; // Conversion (produit en croix).
//    Convertisseur_moy = 2047 + 0; // Valeur min est 2047 plus la valeur quand sin = 0;
//    Tension_moy = 3*Convertisseur_moy / 4095; // Conversion (produit en croix).

    
}

