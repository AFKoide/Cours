#ifndef __MAIN_H_
#define __MAIN_H_
/* 
 * File:    Main.h              
 * Author:  Michel GALLIEN      
 * Company: UM HLEE407          
 *
 * Application  : Using LCD     
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


/* General constants defines */
#define ON      1
#define OFF     0
#define TRUE    1
#define FALSE   0

/* Setup Frame defines */
#define FRAME_SIZE   16   // "=00/00/00 00:00\r"
#define FRAME_HEADER '='
#define FRAME_ENDER '\r'

/* Indexes in Frame array */
#define FRAME_DAY     1
#define FRAME_MONTH   4
#define FRAME_YEAR    7
#define FRAME_HOUR   10
#define FRAME_MINUTE 13

/* Status for Chrono */
enum
{   RTCC,
    STOP,
    GO,
    INTER,
    EFF,
};

/* Prototypes for used functions */
void main_Uart2_Receive_Handler(unsigned char); // Perform input character  
void main_Timer1_Handler(void);         // Perform elapsed time by Timer1   

#endif // __MAIN_H_	// end of the definitions of Main.h

/* End of file */
