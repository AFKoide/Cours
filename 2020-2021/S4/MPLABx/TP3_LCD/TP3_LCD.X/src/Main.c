/* 
 * File:    Main.c              
 * Author:  Michel GALLIEN      
 * Company: UM HLEE407          
 *
 * Application  : Test TP3 LCD  
 * Created on   : 27 mars 2021  
 * 
 * Platform:    Explorer-16-32  
 * Device used: PIC24FJ1024GB610
 * Environment: MPLabX  v5.45   
 * Compiler:    XC16    v1.60   
 */

/* Specifications :         
 * **************           
 * Initialize GPIOs         
 * Initialize Uart2         
 * Welcome message on LCD   
 * Blinking D4 for alive    
 * D3  = S4                 
 */

/* Exercise-1
 * In store people Counter  
 * Entry people  by S2      
 * People out    by S4      
 * Clear counter by S6      
 * Show inner people   Line1
 * Show total visitors Line2
 * TimeOut by WDT           
 */

/* What's new today...      
 * ****************         
 * Understand PPS function  
 * Configuraton bits        
 * Choose any output port   
 * Watchdog timer           
 * Pilotage LCD display     
 * Using sprintf procedure  
 * Background task role     
 */


/* Libraries */
#include <stdio.h>
#include <libpic30.h>
#include <stdlib.h>
#include <xc.h>


/* Bits-Configuration of Microcontroler */
//#include "ConfigBits.h"


/* Dependencies */
#include "Gpio.h"
#include "Uart2.h"
#include "Lcd.h"

#include "Main.h"


/* Own prototypes */
void _setUp(void);  // Initialization   
void _loop(void);   // Background Task  
void _Display_Visiteurs(void);

/* Memories */
unsigned short Count;   // Count for main loop      
unsigned char EdgeS3;   // Detection of Edge on BpS3
unsigned char EdgeS4;   // Detection of Edge on BpS4
unsigned char EdgeS6;   // Detection of Edge on BpS6
unsigned char Presents; // Nombre de presents       
unsigned char Total;    // Nombre total de visiteurs


/* Code */
int main(void)
{   __C30_UART = 2;         // Allow using of "printf" on Uart2 by xc16 
    _setUp();               // General initializations      
    while(1)    _loop();    // Background task ! always !   
    return (0);
}


/* Initalize the application */
void _setUp(void)
{   gpio_Init();            // Initializing IO                  
    uart2_Open(_9600Bds);   // Initializing Serial communication
    lcd_Init();             // Initializing Lcd Display         
    Count  = 0;             // Reseting Counter for main loop   
    EdgeS3 = 0;             // Preparing Edges detection        
    EdgeS4 = 0;
    EdgeS6 = 0;
    Presents = 0;
    Total  = 0;
    /* Welcome messages */
    
    sprintf (LcdBuffL1, "-Afficheur  LCD-");
    LcdToDoL1 = ON;
    sprintf (LcdBuffL2, "- TP3  HLEE407 -");
    LcdToDoL2 = ON;   
    printf ("\r\n--- TP3- Afficheur LCD ---\r\n");
}
//TODO

/* Background task */
void _loop(void)        // Main loop                        
{   lcd_Task();         // Background task for Lcd display  
//todo ola

    if (S4) gpio_Set(D3, ON);  // Test S4 button status     
    else    gpio_Set(D3, OFF);

    Count++;        // Loops counter for elapsed  time      
    if (Count > 10000)
    {   Count = 0;
        gpio_Toggle(D4);    // Blink Led5 every second      
    }   

    
    if (S3)     // Action on S3 button  Compte visiteurs    
    {  
        if(!EdgeS3)
        {
            EdgeS3 = 1;

            Presents++;
            Total++;
            _Display_Visiteurs();
            while (S3);
        }
        else
            EdgeS3 = 0;
    }

    if (S4)     // Action on S4 button  Decompte pr�sence    
    {   
        if (!EdgeS4)
        {
            EdgeS4 = 1;

            if(Presents>0)
                Presents--;
            _Display_Visiteurs();
            while (S4);
        }
        else
            EdgeS4 = 0;
    }

    if (S6)     // Action on S6 button Initialise systeme             
    {  
        if(!EdgeS6)
        {
            EdgeS6 = 1;

            Presents = 0;
            Total = 0;
            _Display_Visiteurs();
            while (S6);
        }
        else
            EdgeS6 = 0;
    }
}

void _Display_Visiteurs(void)
{   
    sprintf(LcdBuffL1,"Present : %6d", Presents);
    LcdToDoL1 = ON;
    sprintf(LcdBuffL2,"Total : %8d", Total);
    LcdToDoL2 = ON;
}


/* Handler for Uart2 */
void main_Uart2_Receive_Handler(unsigned char Car)    // Perform this character 
{   uart2_Put_Char(Car);    // Echo for the last character  
    /* Nothing to do */
}


// End of Main.c