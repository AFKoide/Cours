/* 
 * File:    Uart1.c             
 * Author:  Michel GALLIEN      
 * Company: UM HLEE407          
 *
 * Routine library  : Uart1     
 * 
 * Target : PIC24FV             
 * IDE :    MPLabX v5.45        
 * Compiler :   XC16 v1.60      
 * Created on   : 23 mars 2021  
 */


/* Libraries */
#include <xc.h>
#include <p24FJ1024GB610.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>


/* Own declarations */
#include "Uart1.h"


/* Dependencies */
#include "Gpio.h"
#include "Main.h"

//  Using Uart1         
// pin-49 RX RF4 RP10   
// pin-50 TX RF5 RP17   


/* Program zone */

/* Initializing Uart1 */
void uart1_Open(unsigned short Brg)
{   gpio_Uart1();
    U1MODE = 0;    // Disabling Uart1 if open yet       
    U1STA  = 0;
    U1MODEbits.STSEL = 0;   // 1 StopBit            
    U1MODEbits.PDSEL = 00;  // 8 Bits, no parity    

    U1MODEbits.BRGH = _BRGH;// 1 = High Speed mode x4 cycles
    U1BRG = Brg;    // BaudRate = 9600Bds; Frequency = 8000000 Hz; Brg = 103 
    U1MODEbits.UARTEN = 1;  // Enabling Uart Device 
    U1STAbits.UTXEN   = 1;  // Enabling TX Device   
    U1STAbits.URXEN   = 1;  // Enabling RX Device   
       
    /* URXI: U1RX - UART1 Receiver Priority: 1 */
    IPC2bits.U1RXIP = 1;
    /* Allow Interrupt on receive one character */
    IEC0bits.U1RXIE = 1;
}


/* Get Status of Uart1 */
unsigned short uart1_Get_Status (void)
{   return U1STA;
}


/* Get one character from Uart1 */
unsigned char uart1_Get_Char(void)
{   while(!(U1STAbits.URXDA == 1)); // Wait receiving   
    if (U1STAbits.OERR)         // if over-run Error    
    {   U1STAbits.OERR = 0;     // Acknowledge Error    
    }
    return uart1_Read_Char();     // Read the character received  
}


/* Read last char received on Uart1 */
unsigned char uart1_Read_Char(void)
{   return U1RXREG;
}


/* Write the data byte to the Uart1 */
void uart1_Write_Char(unsigned char Data)
{   U1TXREG = Data; // Write one character on buffer of Uart1   
}


/* Output one character to Uart1 */
void uart1_Put_Char(unsigned char Data)
{   while(U1STAbits.UTXBF == 1);// Wait buffer empty    
    uart1_Write_Char(Data);     // Send character on Uart1   
}


/* Write String of characters to Uart1 */
// String must be finished by "0x00"    
void uart1_Put_Str(unsigned char *Str)
{   while (*Str)    uart1_Put_Char(*Str++);
}


/* Interrupt Service Routine for Receiving character from Uart1*/
void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void)
{  {    main_Uart1_RX_Handler(U1RXREG);
        IFS0bits.U1RXIF =0; // Efface Flag = Acquittement Interruption
   }
}


// End of file "Uart1.c"
