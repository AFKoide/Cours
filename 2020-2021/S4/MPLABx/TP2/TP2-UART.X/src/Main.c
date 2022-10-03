/* 
 * File:    main.c              
 * Author:  Michel GALLIEN      
 * Company: UM HLEE407          
 *
 * Main function  : TP2 -  UART 
 * 
 * Target : PIC24FV             
 * IDE :    MPLabX v5.45        
 * Compiler :   XC16 v1.60      
 * Created on   : 23 mars 2021  
 */


/*  Specicfications   */
/*====================*/
/* D3 = S4              
 * "Bonjour" for Reset  
 * S3 send "Salut"      
 * D4 = is blinking 1s  
 * Type "A" -> D5 = 1   
 * Type "E" -> D5 = 0 */


/* Discoveries on TP2 */
/*====================*/
/* UART Registers               
 * UART Initialization          
 * Using interrupts             
 * Function assignment to pin   
 * Using Pointer, Array         
 * Select by switch instruction 
 * Scanning or polling or IRQ   
 */


/* Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <p24FJ1024GB610.h>

/* Dependencies */
#include "Gpio.h"
#include "Main.h"

/* Own Declarations */
#include "Uart1.h"

/* Own prototypes */
void _set_Up(void);     // General initialization       
void _loop(void);       // Background task in main loop 
void _exo_1(unsigned char);
void _exo_2(unsigned char *);
void _exo_3(unsigned char, unsigned char *);
void _manage_Frame(void);

/* Global memories */
unsigned short Compt;   // Timer counter    
unsigned char EdgeS3, EdgeS4;
unsigned char Chaine[] = {"\r\nSalut"};
unsigned char IdxMess, IdxFrame;
unsigned char Frame [FRAME_SIZE];


/* Main program zone */
int main(void)
{   _set_Up();  // General initialization   
    printf ("\r\nBonjour");
    while(1)
    _loop();    // Background task          
    return (1);
}


/* Initalize application */
void _set_Up(void)
{   gpio_Init();    // Initialize IO
    uart1_Open(_9600Bds); // Uart   
    Compt    = 0;   // Time Counter 
    EdgeS3   = FALSE;   
    EdgeS4   = FALSE;
    IdxMess  = 0;
    IdxFrame = 0;
}


/* Main loop */
void _loop(void)
{   if (S4) gpio_Set(D3, ON);       // Test S4 button status        
    else    gpio_Set(D3, OFF);

    if (S3)
    {   if (!EdgeS3)
        {   EdgeS3 =1;
            uart1_Put_Str(Chaine);  // Output string on PC display  
            //printf ("%s", Chaine);        // Alternativ           
            //uart1_Put_Str(&Chaine[0]);    // Alternativ           

            while (S3);     // Wait S3 released
        }
    }
    else    EdgeS3 = 0;

    Compt++;    // Loops counter for elapsed  time                  
    if (Compt > 10000)
    {   Compt = 0;
        gpio_Toggle(D4);    // Blink Led4 every second              
    }

    _exo_2(&IdxMess);       // Select message to display on PC      
}


/* Main Interrupt handler for Uart1 */
void main_Uart1_RX_Handler(unsigned char Data)
{   uart1_Put_Char(Data);   // Echo to PC of last character received
    _exo_1(Data);   // Activate D5 if Type 'A' on PC keyboard       
    _exo_3(Data, &IdxFrame);// Activate one Led with specific frame 
}


//==============================================================================

/*  EXERCISE 1  */
/*==============*/
/* Use uppercase or lowercase                   
 * 'A' ou 'a' for lighting et 'E' ou 'e' to stop
 * 3 possible methods -> it must work!          
 */

void _exo_1(unsigned char Data)
{   /*  Solution-1 */
    
    /*  & Mask */
   

    /*  Solution-2  */
    /* "if" complex */
    

    /*   Solution-3   */
    
    switch (Data)
    {
    case 'a' :
        gpio_Set(D5,ON);
        break;

    case 'A' :
        gpio_Set(D5,ON);
        break;


    case 'e' :
        gpio_Set(D5,OFF);
        break;

    case 'E' :
        gpio_Set(D5,OFF);
        break;

    default:
        break;
    }
    /* Using "switch" */
}


//==============================================================================

/*  EXERCISE 2  */
/*==============*/
/* Display 4 messages                       
 * Pressing S4 changes the message among 4  
 * Using array and pointer
 */

const unsigned char Message1[] = {"Premier"};
const unsigned char Message2[] = {"Deuxieme"};
const unsigned char Message3[] = {"Troisieme"};
const unsigned char Message4[] = {"Quatrieme"};

#define NB_MESS 4   // Number of messages in array of pointer messages  
unsigned char *Array[NB_MESS] = {Message1, Message2, Message3, Message4};

void _exo_2(unsigned char *Idx)
{
    if(S4)
    {
        if(!EdgeS4)
        {
            EdgeS4 = 0;
            
            if(*Idx >= NB_MESS)
                *Idx = 0;
            
            uart1_Put_Str(Array[*Idx]);
            *Idx = *Idx+1;
            
            while(S4);
        }    
    }
}


//==============================================================================

/* EXERCISE 3 */
/* Pilot the Leds by the PC keyborad                            
 * Send a frame as :                                            
 * "Dn=X\r"                                                     
 * where 'n' is number of the Led and 'X' is either 1 or 0      
 * if 'X' = 0 switch off the Led and if X=1 switch on the Led   
 * Detection of Frame Header 'D' => prepare receive buffer      
 * Filling the buffer via UART received interrupt               
*/

void _exo_3(unsigned char Data, unsigned char *Idx)
{
    unsigned char I;
    
    if(Data == 'D'||Data == 'd')
    {
        for(I = 0; I<5; I++)
            Frame[I] = 0;
        Frame[0] = Data;
        *Idx = 1;
    }
    if(*Idx)
    {
        if('\r' == Data)
            _manage_Frame();
        else
        {
            Frame[*Idx] = Data;
            *Idx += 1;
            if(*Idx>=5)
                *Idx = 0;
        }
    }
}

/* Analyzing the frame to control the LEDS */

void _manage_Frame(void)
{  
    switch(Frame[1])
    {
        case '3':
            gpio_Set(D3,Frame[3] & 0x01);
            break;
        
        case '4':
            gpio_Set(D4,Frame[3] & 0x01);
            break;
            
        case '5':
            gpio_Set(D5,Frame[3] & 0x01);
            break;
            
        case '6':
            gpio_Set(D6,Frame[3] & 0x01);
            break;
            
        case '7':
            gpio_Set(D7,Frame[3] & 0x01);
            break;
            
        case '8':
            gpio_Set(D8,Frame[3] & 0x01);
            break;
            
        case '9':
            gpio_Set(D9,Frame[3] & 0x01);
            break;
        
        default :
            break;
    }
}

//==============================================================================


// End of Main.c