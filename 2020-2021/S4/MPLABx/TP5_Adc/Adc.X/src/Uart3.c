/* 
 * File:    Uart3.c                     

 * Author:  Michel GALLIEN              
 * Company: HLEE707 UM                  
 * Created on : 10 avril 2021           

 * Platform:    Explorer-16-32          
 * Device used: PIC24FJ1024GB610        
 * Environment: MPLabX  v4.15           
 * Compiler:    XC16    v1.31           

 ************************************** 
 * Routine library:     Manage Uart   * 
 ************************************** 

 */

/* Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <p24FJ1024GB610.h>


/* Dependencies */
#include "Main.h"

/* Defines for Uart3 */
#include "Uart3.h"

/* Memories for Uart3 */
char U3OutBuff[U3_OUT_BUFF_SIZE+1];

/* Own prototypes */
void _u3_Clr_Out_Buff(void);


/*  Using Uart3       */
// pin-49 RX RF4 RP10   
// pin-50 TX RF5 RP17   


/* Program zone */
/****************/

/* Initialize Uart3 for Explorer16-32 */
// Input: Divider for BaudRate          
// Output: None                         
void uart3_Open(unsigned short Bdr)
{   _u3_Clr_Out_Buff();
    U2MODE = 0;    // Disabling Uart3 if open yet       
    U2STA  = 0;
    
    LATFbits.LATF5   = 1;   // RF5 = TX Force idle = 1  
    TRISFbits.TRISF5 = 0;   // RF5 = TX Output          
    TRISFbits.TRISF4 = 1;   // RF4 = RX Input           

    /* Set the PPS for Uart3 */
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS 
    RPINR17bits.U3RXR = 10;     // RP10 -> UART3: U3RX    
    RPOR8bits.RP17R   = 19;     // 19   -> RP17 : U3TX    
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS 

    /* Setup Uart3 registers */
    U3MODEbits.UARTEN = 1;  // Enable Uart3         
    U3MODEbits.BRGH   = 1;  // High speed mode      
  
    U3MODEbits.STSEL = 0;   // 1 StopBit            
    U3MODEbits.PDSEL = 00;  // 8 Bits, no parity    

    U3MODEbits.BRGH = _BRGH;//1 = High Speed mode x4
    U3BRG = Bdr;    // BaudRate = 9600Bds; Frequency = 8000000 Hz; Brg = 103 
    U3MODEbits.UARTEN = 1;  // Enabling Uart Device 
    U3STAbits.UTXEN   = 1;  // Enabling TX Device   
    U3STAbits.URXEN   = 1;  // Enabling RX Device   
   
    /* Setup Interrupt ability */
//  IPC20bits.U3ERIP = 1;   // Interrupt Uart3 Error    Priority 1  
//  IEC5bits.U3ERIE  = 1;   // Enabling  Uart3 Error Interrupt      
//  IPC20bits.U3TXIP = 1;   // Interrupt Uart3 Transmit Priority 1  
//  IEC5bits.U3TXIE  = 1;   // Enabling  Uart3 TX Interrupt         
    IFS5bits.U3RXIF  = 0;   // Clear interrupt flag for RX          
    IFS5bits.U3ERIF  = 0;   // Clear interrupt flag for Error       
    IFS5bits.U3TXIF  = 0;   // Clear interrupt flag for TX          
    IPC20bits.U3RXIP = 1;   // Interrupt Uart3 Receive  Priority 1  
    IEC5bits.U3RXIE  = 1;   // Enabling  Uart3 RX Interrupt         

    U3ADMD = 0x0000;        // ADMADDR 0; ADMMASK 0;                
}

/* Clear Out Buffer for Uart3 */
void _u3_Clr_Out_Buff(void)
{ unsigned short I;
    for (I = 0; I < U3_OUT_BUFF_SIZE; I++) U3OutBuff[I] = 0;
    U3OutBuff[I] = 0;
}

/* Send one character to Uart3 */
void uart3_Put_Char(char Car)
{ unsigned short ShortCar;
    while (!U3STAbits.TRMT);
    ShortCar = (unsigned short) Car;
    ShortCar &= 0x007f;
    U3TXREG = ShortCar;
}

void __putch (char Car)
{   uart3_Put_Char(Car);
}



/* Send a string of characters to Uart3 */
void uart3_Put_Str(char *Str)
{   while (*Str)    uart3_Put_Char(*Str++);
}


/* Interrupt Service Routine for Uart3 receiving  */
void __attribute__ ( ( interrupt, no_auto_psv ) ) _U3RXInterrupt( void )
{   main_Uart3_Receive_Handler(U3RXREG);    // Manage received character    
    IFS5bits.U3RXIF = FALSE;       // Receive Interrupt Acknowledge         
}


// End of file "Uart3.c"                                                        

