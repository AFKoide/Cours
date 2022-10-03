#ifndef __MAIN_H_
#define __MAIN_H_
/* 
 * File:    Main.h              
 * Author:  Michel GALLIEN      
 * Company: HLEE407  UM         
 *
 * Application  : Test LCD      
 * Created on   : 10 avril 2021 
 */

/* Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <p24FJ1024GB610.h>


/* General constants */
#define ON      1
#define OFF     0
#define TRUE    1
#define FALSE   0


/* Prototypes for used functions */
void main_Uart3_Receive_Handler(unsigned char);    // Perform this character
void main_Timer1_Handler(void); // Perform elapse time

#endif // __MAIN_H_	// end of the definitions of Main.h

/* End of file */
