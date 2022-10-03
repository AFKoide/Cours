#ifndef _UART_H_
#define _UAR3_H_

/* 
 * Header-File:     Uart3.h             

 * Author:  Michel GALLIEN              
 * Company: HLEE407  UM                 
 * Created on : 10 avril 2021           

 * Platform:    Explorer-16-32          
 * Device used: PIC24FJ1024GB610        
 * Environment: MPLabX  v4.15           
 * Compiler:    XC16    v1.31           

 ************************************** 
 * Routine library:     Manage Uart3  * 
 ************************************** 

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

#define U3_OUT_BUFF_SIZE 64


/* External memories */
extern char U3OutBuff[];


/* Statments zone */
#define U2TRX   TRISFbits.TRISF4    // Direction register for RX2   
#define U2TX    LATFbits.LATF5      // Output for TX2               


/* Prototypes for Uart-3 */
void uart3_Open(unsigned short);    // Initialize Uart2 Port        
void uart3_Put_Char(char); // Send one character           
void uart3_Put_Str(char *);// Send string of characters    
unsigned char uart3_Get_Char(void); // Get one character            

#endif // __UART3_H_	// end of the definitions of Uart3.h


/* End of file */                                                      
