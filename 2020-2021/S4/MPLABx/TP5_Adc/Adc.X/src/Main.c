/* 
 * File:    Main.c              
 * Author:  Michel GALLIEN      
 * Company: HLEE407  UM         
 *
 * Application  : Test ADC12bits
 * Created on   : 10 avril 2021 
 */

/* Resume:                  
 * Initialize GPIOs         
 * Initialize UART2         
 * Initialize LCD           
 * Initialize ADC 12bits    
 * Welcome message on LCD   
 * Show Message1 on line1   
 * Show Potar value on line2
 * Blinking D4 for alive    
 * D6  = S4                 
 */


/* Libraries */
#include <stdio.h>
#include <libpic30.h>
#include <stdlib.h>
#include <xc.h>
#include <p24FJ1024GB610.h>


/* Bits-Configuration of Microcontroler */
//#include "ConfigBits.h"


/* Dependencies */
#include "Gpio.h"
#include "Uart3.h"
#include "Lcd.h"
#include "Adc12b.h"

#include "Main.h"


/* Own prototypes */
void _setUp(void);  // Initialization   
void _loop(void);   // Background Task  


/* Memories */
unsigned short Count;   // Count for main loop              
unsigned short Potar;   // Brut value for potentiometer     
unsigned short Temp;    // Brut value for temperature sensor

/* Program zone */
int main(void)
{   _setUp();               // General initializations      
    while(1)    _loop();    // Background task ! always !   
    return (0);
}


/* Initalize the application */
void _setUp(void)
{   gpio_Init();            // Initializing IO                    
    uart3_Open(_9600Bds);   // Initializing Serial communication  
    lcd_Init();             // Initializing Lcd Display           
    adc_Init();             // Initializing ADC converter 12bits  
    Count  = 0;             // Initializing Counter for main loop 

    sprintf (LcdBuffL1, "-Test ADC12bits-");
    LcdToDoL1 = ON;

    sprintf (U3OutBuff, "\r\n--- Test ADC.12bits ---\r\n");
    uart3_Put_Str (U3OutBuff);
}


/* Background task */
void _loop(void)        // Main loop                        
{   lcd_Task();         // Background task for Lcd display  

    if (S4) gpio_Set(D3, ON);  // Test S4 button status     
    else    gpio_Set(D3, OFF);

    Count++;        // Loops counter for elapsed  time      
    if (Count > 2)
    {   Count = 0;
        gpio_Toggle(D4);    // Blink Led5 every second      
    }   

    Potar = adc_Read_Channel(ADC_POTAR);
    

/*
    sprintf (LcdBuffL2, "- Potar = %4d -", Potar);
    LcdToDoL2 = ON;   

    sprintf (U3OutBuff, "\r- Potar=0x%04x", Potar);
    uart3_Put_Str (U3OutBuff);

    sprintf (U3OutBuff, " Temp=0x%04x", Temp);
    uart3_Put_Str (U3OutBuff);
*/
//    Potentiometre();

//    Voltmetre();
    
    Termometre();
}


/* Handler for Uart3 */
void main_Uart3_Receive_Handler(unsigned char Car)    // Perform this character
{   uart3_Put_Char(Car);    // Echo for the last character  

}

//================================================================

/* EXERCICE 1 */ 
/* Realiser un dispositif de stabilisation de la mesure */
/* Moyenne glissante sur 16 valeurs                     */
void Stabilisation()
{

}
//================================================================

/* EXERCICE 2 */
/* Visualisation du potentiometre sur ligne 1  */
/* Realiser un barregraphe sur la ligne du bas */
/* Disposition de 0 � 15 barres sur la ligne 2 */
/* Code barre noire =0xff & barre blanche =0x20*/

#define NOIR 0xff
#define BLANC 0x20

void Potentiometre(void)
{
    unsigned char barre[16];
    int i;
    unsigned char calcul;

    //Init ligne
    for (i = 0; i < 16; i++)
        barre[i] = BLANC;   

    // TODO: Division de la valeur actuelle de Potar par sa valeur totale possible

    calcul = Potar / 256;

    for (i = 0; i < calcul + 1; i++)
        barre[i] = NOIR;
   
    sprintf(LcdBuffL1, "Potentiel:%5d", Potar);
    LcdToDoL1 = ON;

    sprintf(LcdBuffL2, "%s", barre);
    LcdToDoL2 = ON;

}

//================================================================

/* EXERCICE 3 */ 
/* Realiser un Voltmetre avec mise � l'�chelle en V */
/* Affichage de la tension de 0.0V � 3.3V           */
/* ATTENTION on ne travaillera qu'avec des entiers  */
/* Faire la d�tection d'un seuil D6=1 si > 2V       */
/* et detection d'un niveau D7=1 si < 1V            */

void Voltmetre()
{
    unsigned long Volt;
    unsigned short Volt_Unite, Volt_Decimal;
    Volt = Potar/124;

    Volt_Unite = Volt/10;
    Volt_Decimal = Volt - (Volt_Unite*10);

    if(Volt<10)
        gpio_Set(D7, ON);
    else if (Volt>20)
        gpio_Set(D6, ON);
    else
    {
        gpio_Set(D7, OFF);
        gpio_Set(D6, OFF);
    }

    sprintf(LcdBuffL1, "Tension : %2d.%1d V", Volt_Unite, Volt_Decimal);
    LcdToDoL1 = ON;
}

//================================================================

/* EXERCICE 4 */
/* Affichage de la temperature en utilisant le capteur TC1047A */

void Termometre()
{
    unsigned int Unite, Decimal;
    unsigned long Temperature;

    Temp = adc_Read_Channel(ADC_TEMP);

    Temperature = Temp/3 - (50*2);// Offset  
    sprintf (LcdBuffL2, "Degres = %2d.%01d C ", Temperature/10, (Temperature - ((Temperature/10)*10)));
    LcdToDoL2 = ON;   
}
//================================================================


// End of Main.c
