#ifndef _UART2_H_
#define _UART2_H_

/* 
 * Header-File:  "Uart2.h"      

 * Author:  Michel GALLIEN      
 * Company: UM HLEE407          
 * Created on : 27 mars 2021    

 * Platform:    Explorer-16-32  
 * Device used: PIC24FJ1024GB610
 * Environment: MPLabX  v5.45   
 * Compiler:    XC16    v1.60   

 ************************************** 
 * Routine library:     Manage Uart   * 
 ************************************** 

 */

/* Libraries */
#include <xc.h>


/* Constants for TX & RX on explorer16-32 FRC=8MHz PLL=1*/
#define _BRGH       1   // High speed (x4 cycles)
#define _4800Bds    206 // 103
#define _9600Bds    103 // 51
#define _19200Bds   51  // 25
#define _38400Bds   25  // 12
#define _115200Bds  12  //  3


/* Statments zone */
#define U2TRX   TRISFbits.TRISF4    // Direction register for RX2   
#define U2TX    LATFbits.LATF5      // Output for TX2               


/* Prototypes for Uart-2 */
void uart2_Open(unsigned short);    // Initialize Uart2 Port        
void uart2_Put_Char(unsigned char); // Send one character           
void uart2_Put_Str(unsigned char *);// Send string of characters    
unsigned char uart2_Get_Char(void); // Get one character            

#endif // __UART2_H_	// end of the definitions of Uart2.h


/* End of file */                                                      
