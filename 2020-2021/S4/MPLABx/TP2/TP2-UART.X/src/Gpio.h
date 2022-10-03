#ifndef _GPIO_H_
#define _GPIO_H_

/* 
 * Header file : Gpio.h         
 * Author:  Michel GALLIEN      
 * Company: UM HLEE407          
 *
 * Application  : Functions GPIO
 * Created on   : 16 mars 2021  
 */

/* Libraries */
#include <p24FJ1024GB610.h>


/* Statments zone */
/******************/

/* Defines outputs for 8 Led on explorer16-32 */
#define LED3    LATAbits.LATA0
#define LED4    LATAbits.LATA1
#define LED5    LATAbits.LATA2
#define LED6    LATAbits.LATA3
#define LED7    LATAbits.LATA4
#define LED8    LATAbits.LATA5
#define LED9    LATAbits.LATA6
//#define LED10 LATAbits.LATA7   used by S5 !!! NOT USED !!!    


/* Defines inputs for 4 buttons on Explorer16-32 */
#define S3      !PORTDbits.RD6
#define S4      !PORTDbits.RD13
#define S5      !PORTAbits.RA7
#define S6      !PORTDbits.RD7


/* List of available Outputs for LED on Explorer16-32 */
enum
{   D3, D4, D5, D6,
    D7, D8, D9
};


/* Prototypes for Gpio */
void gpio_Init(void);   // Initialization general Input Output Pins 
void gpio_Set(unsigned char, unsigned char); // Set designed output 
void gpio_Toggle(unsigned char);  // Toggle designed output         
void gpio_Uart1(void);  // Select IO pins for UART1                 

#endif  // end of "_GPIO_H_"

// End of file Gpio.h
