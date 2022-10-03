/* 
 * File: "Uart2.c"              

 * Author:  Michel GALLIEN      
 * Company: UM HLEE407          
 * Created on : 27 mars 2019    

 * Platform:    Explorer-16-32  
 * Device used: PIC24FJ1024GB610
 * Environment: MPLabX  v5.45   
 * Compiler:    XC16    v1.60   

 ************************************** 
 * Routine library:     Manage Uart   * 
 ************************************** 

 */

/* Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>


/* Dependencies */
#include "Constants.h"
#include "Main.h"

/* Defines for Uart2 */
#include "Uart2.h"

/*  Using Uart2       */
// pin-49 RX RF4 RP10   
// pin-50 TX RF5 RP17   


/* Code */

/* Initialize Uart2 for Explorer16-32 */
// Input: Divider for BaudRate          
// Output: None                         
void uart2_Open(unsigned short Bdr)
{   U2MODE = 0;    // Disabling Uart2 if open yet       
    U2STA  = 0;
    
    LATFbits.LATF5   = 1;   // RF5 = TX Force idle = 1  
    TRISFbits.TRISF5 = 0;   // RF5 = TX Output          
    TRISFbits.TRISF4 = 1;   // RF4 = RX Input           

    /* Set the PPS for Uart2 */
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
    RPINR19bits.U2RXR = 10;   // RF4 -> UART2:U2RX
    RPOR8bits.RP17R   = 5;    // RF5 -> UART2:U2TX
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS

    /* Setup Uart2 registers */
    U2MODEbits.UARTEN = 1;  // Enable Uart2     
    U2MODEbits.BRGH   = 1;  // High speed mode  
  
    U2MODEbits.STSEL = 0;   // 1 StopBit            
    U2MODEbits.PDSEL = 00;  // 8 Bits, no parity    

    U2MODEbits.BRGH = _BRGH;// 1 = High Speed mode x4 cycles
    U2BRG = Bdr;    // BaudRate = 9600Bds; Frequency = 8000000 Hz; Brg = 103 
    U2MODEbits.UARTEN = 1;  // Enabling Uart Device 
    U2STAbits.UTXEN   = 1;  // Enabling TX Device   
    U2STAbits.URXEN   = 1;  // Enabling RX Device   
   
    /* Setup Interrupt ability */
//  IPC16bits.U2ERIP = 1;   // Interrupt Uart2 Error    Priority 1  
//  IEC4bits.U2ERIE  = 1;   // Enabling  Uart2 Error Interrupt      
//  IPC7bits.U2TXIP  = 1;   // Interrupt Uart2 Transmit Priority 1  
//  IEC1bits.U2TXIE  = 1;   // Enabling  Uart2 TX Interrupt         
    IPC7bits.U2RXIP  = 1;   // Interrupt Uart2 Receive  Priority 1  
    IEC1bits.U2RXIE  = 1;   // Enabling  Uart2 RX Interrupt         
    U2ADMD = 0x0000; // ADMADDR 0; ADMMASK 0; 

}


/* Send one character to Uart2 */
void uart2_Put_Char(unsigned char Car)
{ unsigned short ShortCar;
    while (!U2STAbits.TRMT);
    ShortCar = (unsigned short) Car;
    ShortCar &= 0x007f;
    U2TXREG = ShortCar;
}

void __putch (unsigned char Car)
{   uart2_Put_Char(Car);
}



/* Send a string of characters to Uart2 */
void uart2_Put_Str(unsigned char *Str)
{   while (*Str)    uart2_Put_Char(*Str++);
}


/* Interrupt Service Routine for Uart2 receiving  */
void __attribute__ ( ( interrupt, no_auto_psv ) ) _U2RXInterrupt( void )
{   main_Uart2_Receive_Handler(U2RXREG);    // Manage received character    
    IFS1bits.U2RXIF = FALSE;       // Receive Interrupt Acknowledge         
}


// End of file "Uart2.c"                                                        

