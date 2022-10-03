#ifndef _GPIO_H_
#define _GPIO_H_

/* 
 * Header file :    Gpio.h      
 * Author:  Michel GALLIEN      
 * Company: Grand Sud Automation
 *
 * Application  : Test GPIO     
 * Created on   : 11 mars 2021   
 */

/* Libraries */
#include <p24FJ1024GB610.h>


/* Statments zone */
/******************/

/* Defines of 8 Led on explorer16-32 */
#define LED3    PORTAbits.RA0
#define LED4    PORTAbits.RA1
#define LED5    PORTAbits.RA2
#define LED6    PORTAbits.RA3
#define LED7    PORTAbits.RA4
#define LED8    PORTAbits.RA5
#define LED9    PORTAbits.RA6
//#define LED10 PORTAbits.RA7   used by S5


/* Defines of 4 buttons on Explorer16-32 */
#define S3      !PORTDbits.RD6
#define S4      !PORTDbits.RD13
#define S5      !PORTAbits.RA7
#define S6      !PORTDbits.RD7


/* List of available LED on Explorer16-32 */
enum
{   D3, D4, D5, D6,
    D7, D8, D9
};


/* Miscelaneous constants */
#define ON  1
#define OFF 0


/* Prototypes for Gpio */
void gpio_Init(void);
void gpio_Set(unsigned char, unsigned char);
void gpio_Toggle(unsigned char);

#endif  // end of "_GPIO_H_"

// End of file Gpio.h
