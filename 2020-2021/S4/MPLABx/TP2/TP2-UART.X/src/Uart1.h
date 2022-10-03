#ifndef _UART1_H_
#define _UART1_H_

/* 
 * Header file : Uart1.h        
 * Author:  Michel GALLIEN      
 * Company: UM HLEE407          
 *
 * Application  : Functions Uart
 * Created on   : 23 mars 2021  
 */

/* Libraries */
#include <p24FJ1024GB610.h>
#include <xc.h>


/* Constants for TX & RX on explorer16-32 FRC=8MHz PLL=1*/
#define _BRGH       1   // High speed (x4 cycles)
#define _4800Bds    103
#define _9600Bds    51
#define _19200Bds   25
#define _38400Bds   12
#define _115200Bds  3


/* Prototypes for Uart */
void uart1_Open(unsigned short);     // Initialize Uart with baud rate  
void uart1_Write_Char(unsigned char);// Write the data byte to the Uart1
void uart1_Put_Char(unsigned char);  // Out one character to Uart1      
void uart1_Put_Str(unsigned char *); // Out string to Uart1             
unsigned char uart1_Get_Char(void);  // Wait one character from Uart1   
unsigned char uart1_Read_Char(void); // Read last char received on Uart1
unsigned short uart1_Get_Status (void); // Get Status of Uart1          

#endif  // end of "_UART1_H_"    


// End of file "Uart1.h" 
