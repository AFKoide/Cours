/* 
 * File:    Gpio.c              
 * Author:  Michel GALLIEN      
 * Company: UM HLEE407          
 *
 * Routine library  : GPIO      
 *
 * Target : PIC24FV             
 * IDE :    MPLabX v5.45        
 * Compiler :   XC16 v1.60      
 * Created on   : 23 mars 2021  
 * 
 */

/* Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <p24FJ1024GB610.h>

/* Own declarations */
#include "Gpio.h"


/* Dependencies */
#include "Main.h"

/* Program zone */

void gpio_Init(void)
{   LATA  = 0;  // Force Output to "0" for no glitch
    TRISAbits.TRISA0 = 0;   // Output RA0 = Led3    
    TRISAbits.TRISA1 = 0;   // Output RA1 for Led4  
    ANSAbits.ANSA9   = 0;   // Digital port for RA9 
    TRISAbits.TRISA2 = 0;   // Output RA2 for Led5  
    TRISAbits.TRISA3 = 0;   // Output RA3 for Led6  
    TRISAbits.TRISA4 = 0;   // Output RA4 for Led7  
    TRISAbits.TRISA5 = 0;   // Output RA5 for Led8  
    ANSAbits.ANSA6   = 0;   // Digital prt for  RA6 
    TRISAbits.TRISA6 = 0;   // Output RA6 for Led9  
    ANSAbits.ANSA7   = 0;   // Digital port for RA7 
    // TRISAbits.TRISA7 = 0;// Output RA7 for Led10 used by S5  
    TRISAbits.TRISA7 = 1;   // Input RA7  for S5 instead Led10  

    LATD  = 0;  // Force Output to "0" for no glitch
    ANSDbits.ANSD6   = 0;   // Digital port for RD6 
    TRISDbits.TRISD6 = 1;   // Input RD6  for S3    
    ANSDbits.ANSD7   = 0;   // Digital port fort RD7
    TRISDbits.TRISD7 = 1;   // Input RD7  for S6    
    TRISDbits.TRISD13= 1;   // Input RD13 for S4    
}


/* Set output status on Led */
void gpio_Set(unsigned char Led, unsigned char Stat)
{   switch (Led)
    {   case D3:
            if (ON == Stat) LED3 = 1;
            else            LED3 = 0;
            break;

        case D4:
            if (Stat)   LED4 = 1;
            else        LED4 = 0;
            break;

        case D5:
            if (Stat)   LED5 = 1;
            else        LED5 = 0;
            break;

        case D6:
            if (Stat)   LED6 = 1;
            else        LED6 = 0;
            break;

        case D7:
            if (Stat)   LED7 = 1;
            else        LED7 = 0;
            break;

        case D8:
            if (Stat)   LED8 = 1;
            else        LED8 = 0;
            break;

        case D9:
            if (Stat)   LED9 = 1;
            else        LED9 = 0;
            break;

        default:
            break;
    }
}


/* Toggle output Led */
void gpio_Toggle(unsigned char Led)
{   switch (Led)
    {   case D3:
            LED3 = ~LED3;
            break;

        case D4:
            LED4 = ~LED4;
            break;

        case D5:
            LED5 = ~LED5;
            break;

        case D6:
            LED6 = ~LED6;
            break;

        case D7:
            LED7 = ~LED7;
            break;

        case D8:
            LED8 = ~LED8;
            break;

        case D9:
            LED9 = ~LED9;
            break;

        default:
            break;
    }        
}


/* Select IO pins for UART1 */
void gpio_Uart1(void)
{   /* Pins select for Uart1 */
    __builtin_write_OSCCONL(OSCCON & 0xbf);
    RPINR18bits.U1RXR = 10; // RF4 -> UART1:U1RX
    RPOR8bits.RP17R   = 3;  // RF5 -> UART1:U1TX
    __builtin_write_OSCCONL(OSCCON | 0x40);
    
    /* Adapt IO pins for TX & RX direction */
    LATFbits.LATF5   = 1;   // RF5 = TX Force idle = 1  
    TRISFbits.TRISF5 = 0;   // RF5 = TX Output          
    TRISFbits.TRISF4 = 1;   // RF4 = RX Input    
}

// End of file Gpio.c
