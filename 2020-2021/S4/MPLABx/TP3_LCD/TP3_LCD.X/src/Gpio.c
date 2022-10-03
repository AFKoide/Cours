/* 
 * File:    Gpio.c              
 * Author:  Michel GALLIEN      
 * Company: UM HLEE407          
 *
 * Routine library  : GPIO      
 * Created on   : 27 mars 2021  
 * 
 * Platform:    Explorer-16-32  
 * Device used: PIC24FJ1024GB610
 * Environment: MPLabX  v5.45   
 * Compiler:    XC16    v1.60   
 */

/* Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>


/* Dependencies */
#include "Main.h"


#include "Gpio.h"


/* Code */

/* Initialize Input-Output for Explorer16-32  */
// Initializing of all usable Inputs & Outputs  
// Input:  None                                 
// Output: None                                 
void gpio_Init(void)
{   LATA  = 0;                      // Turn Off all outputs 
    TRISAbits.TRISA0 = _PIN_OUTPUT;	// Output RA0 = Led3    
    ANSAbits.ANSA10  = _PIN_DIGITAL;// Digital Output RA10  
    TRISAbits.TRISA1 = _PIN_OUTPUT; // Output RA1 for Led4  
    ANSAbits.ANSA9   = _PIN_DIGITAL;// Digital Output RA9   
    TRISAbits.TRISA2 = _PIN_OUTPUT; // Output RA2 for Led5  
    TRISAbits.TRISA3 = _PIN_OUTPUT; // Output RA3 for Led6  
    TRISAbits.TRISA4 = _PIN_OUTPUT; // Output RA4 for Led7  
    TRISAbits.TRISA5 = _PIN_OUTPUT; // Output RA5 for Led8  
    ANSAbits.ANSA6   = _PIN_DIGITAL;// Digital Output RA6   
    TRISAbits.TRISA6 = _PIN_OUTPUT; // Output RA6 for Led9  
    ANSAbits.ANSA7   = _PIN_DIGITAL;// Digital Output RA7   
    //TRISAbits.TRISA7 = _PIN_OUTPUT; // Output RA7 for Led10 !!! not used !!!  

    LATD  = 0;                      // Turn off all outputs 
    ANSDbits.ANSD6   = _PIN_DIGITAL;// Digital input RD6    
    TRISDbits.TRISD6 = _PIN_INPUT;  // Input RD6 for S3     
    ANSDbits.ANSD7   = _PIN_DIGITAL;// Digital input RD7    
    TRISDbits.TRISD7 = _PIN_INPUT;  // Input RD7  for S6    
    TRISDbits.TRISD13= _PIN_INPUT;  // Input RD13 for S4    
    TRISAbits.TRISA7 = _PIN_INPUT;  // Input RA7  for S5    

    LATE  = 0;              // Initialize IO for Lcd        
    TRISE = _PIN_OUTPUT;    // 8 outputs for Lcd_Data       
    LATDbits.LATD4   = 0;
    TRISDbits.TRISD4 = _PIN_OUTPUT; // Output for LCD_E     
    LATDbits.LATD5   = 0;
    TRISDbits.TRISD5 = _PIN_OUTPUT; // Ouput for LCD_RW     
    LATBbits.LATB15  = 0;
    TRISBbits.TRISB15= _PIN_OUTPUT; // Output for LCD_E     
}


/* Set On-Off Leds on Explorer16-32    */
// Setup state of led (D3-D9) at On/Off  
// Input: Led number  (D3-D9),_ON||_OFF) 
// Output: None                          
void gpio_Set(unsigned char Led, unsigned char Stat)
{   switch (Led)
    { case D3:
        LED3 = Stat;
        break;

    case D4:
        LED4 = Stat;
        break;

    case D5:
        LED5 = Stat;
        break;

    case D6:
        LED6 = Stat;
        break;

    case D7:
        LED7 = Stat;
        break;

    case D8:
        LED8 = Stat;
        break;

    case D9:
        LED9 = Stat;
        break;

    default:
        break;
    }
}


/* Toggle output on Explorer16-32 */
// Invert state of led (0->1, 1->0) 
// Input: Led number (D3-D9)        
// Output: None                     
void gpio_Toggle(unsigned char Led)
{   switch (Led)
    { case D3:
        if (LED3) gpio_Set(D3, OFF);
        else gpio_Set(D3, ON);
        break;

    case D4:
        LED4 ^= 1;
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


// End of file "Gpio.c" 
