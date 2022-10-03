#ifndef __GPIO_H_
#define __GPIO_H_

/* 
 * File:    Gpio.h              
 * Author:  Michel GALLIEN      
 * Company: UM HLEE407          
 *
 * Application  : Test GPIO     
 * Created on   : 27 mars 2021  
 *
 * Platform:    Explorer-16-32          
 * Device used: PIC24FJ1024GB610        
 * Environment: MPLabX  v5.45           
 * Compiler:    XC16    v1.60           
 */

/* Libraries */
#include <xc.h>

/* GPIO Constants */
#define _PIN_INPUT	 1
#define _PIN_OUTPUT	 0
#define _PIN_DIGITAL 0
#define _PIN_ANALOG	 1


/* Statments zone */
#define LED3    LATAbits.LATA0
#define LED4    LATAbits.LATA1
#define LED5    LATAbits.LATA2
#define LED6    LATAbits.LATA3
#define LED7    LATAbits.LATA4
#define LED8    LATAbits.LATA5
#define LED9    LATAbits.LATA6
#define LED10   LATAbits.LATA7

#define S3    !PORTDbits.RD6  // State of BPx (activate if =0)
#define S4    !PORTDbits.RD13
#define S6    !PORTDbits.RD7

#define LCD     LATE
#define LCD_D0  LATEbits.LATE0
#define LCD_D1  LATEbits.LATE1
#define LCD_D2  LATEbits.LATE2
#define LCD_D3  LATEbits.LATE3
#define LCD_D4  LATEbits.LATE4
#define LCD_D5  LATEbits.LATE5
#define LCD_D6  LATEbits.LATE6
#define LCD_D7  LATEbits.LATE7
#define LCD_E   LATDbits.LATD4
#define LCD_RW  LATDbits.LATD5
#define LCD_RS  LATBbits.LATB15


/* Enumeration Gpio */
enum
{   D3, D4, D5, D6, D7, D8, D9
};


/* Prototypes for Gpio */
void gpio_Init(void);














































void gpio_Set(unsigned char, unsigned char);
void gpio_Toggle(unsigned char);
  

#endif // __GPIO_H_	// end of the definitions of Gpio.h


/* End of file */
