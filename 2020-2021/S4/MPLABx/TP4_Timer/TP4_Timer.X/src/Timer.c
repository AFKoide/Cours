/* 
 * File:    Timer.c                      

 * Author:  Michel GALLIEN              
 * Company: UM HLEE407                  
 * Created on : 6 Avril 2021            

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

/* Initialize Timer for Explorer16-32 */
// Input: Temps en ms         
// Output: None               
void timer_Init(unsigned short Tps)
{   /* Setup Timer1 registers */
    IEC0bits.T1IE = 0;      // Disable Timer1 interrupt             
    T1CON = 0;              // Disable Timer1                       
    T1CONbits.TCKPS = 0b10; // Select 1:64 Prescaler      500Hz 2ms 
    T1CONbits.TCS   = 0;    // Instruction cycle clock              
    T1CONbits.TECS  = 0b10; // Select LPRC 32KHz                    
    T1CONbits.TGATE = 0;    // Disable Gated Timer1 mode            

    T1CONbits.TSYNC = 1;    // Synchronize external clock input     
    TMR1 = 0;               // Clear Timer1 register                
    PR1 = Tps * 106;        // Load the Timer1 reset count value    
    IPC0bits.T1IP = 0x04;   // Set Timer1 Interrupt Priority Level  
    IFS0bits.T1IF = 0;      // Clear  Timer1 Interrupt Flag         
    IEC0bits.T1IE = 1;      // Enable Timer1 Interrupt              
    T1CONbits.TON = 1;      // Start  Timer1                        
    
}


/* Interrupt Service Routine for Timer elapse time  */
void __attribute__ ( ( interrupt, no_auto_psv ) ) _T1Interrupt( void )
{   main_Timer1_Handler();  // Manage elapse time by Timer1 
    IFS0bits.T1IF = _FALSE; // Timer1 Interrupt Acknowledge 
}


// End of file "Timer.c"                                                        
