/* 
 * File:    Timer.c                      

 * Author:  Michel GALLIEN              
 * Company: UM HLEE407                  
 * Created on : 27 mars 2021            

 * Platform:    Explorer-16-32          
 * Device used: PIC24FJ1024GB610        
 * Environment: MPLabX  v5.45           
 * Compiler:    XC16    v1.60           

 ************************************** 
 * Routine library:     Manage Timer  * 
 ************************************** 

 */

/* Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>


/* Dependencies */
#include "Constants.h"
#include "Main.h"

/* Defines for Timer */
#include "Timer.h"


/* Program zone */
/****************/

/* Initialize Timer1 for Explorer16-32 */
// Input: Temps en ms         
// Output: None               
void timer1_Init(unsigned short Tps)
{   /* Setup Timer1 registers */
	T1CON =0;   // Desactivate Timer1 before initializing
    T1CONbits.TCKPS = 1;    // FCY /8
    
    /* Setup PR1 register; Frequency = 10 ms */
    TMR1 = 0;
    PR1  = 250 * Tps;

    /* Setup Interrupt ability */
    IPC0bits.T1IP  = 1;   // Interrupt Timer1 Priority 1  
    IEC0bits.T1IE  = 1;   // Enabling  Timer1 Interrupt   
    T1CONbits.TON = 1;      // Activate timer1
}


/* Interrupt Service Routine for Timer elapse time  */
void __attribute__ ( ( interrupt, no_auto_psv ) ) _T1Interrupt( void )
{   main_Timer1_Handler();      // Manage elapse time for Timer1
    IFS0bits.T1IF = _FALSE;     // Timer1 Interrupt Acknowledge 
}


// End of file "Timer.c"                                                        
