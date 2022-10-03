#ifndef _MAIN_H_
#define _MAIN_H_

/* 
 * Header File : Main.h         
 * Author:  Michel GALLIEN      
 * Company: UM HLEE407          
 *
 * Application  : Test UART     
 * Created on   : 23 mars 2021  
 */

/* Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <p24FJ1024GB610.h>


/* Global memories */


/* Usable Constants */
#define OFF 0
#define ON  1
#define FALSE 0
#define TRUE 1
#define FRAME_SIZE 5


/* Usable functions */
void main_Uart1_RX_Handler(unsigned char);  // Main Interrupt handler for Uart1


#endif  // end of "_MAIN_H_"


// End of file Main.h
        
