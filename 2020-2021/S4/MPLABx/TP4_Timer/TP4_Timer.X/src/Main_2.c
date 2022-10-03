/* 
 * File:    Main.c              
 * Author:  Michel GALLIEN      
 * Company: UM HLEE407          
 *
 * Application  : TP Timer      *** Correction Chrono & RTCC ***  
 * Created on   : 6 Avril 2021  
 * 
 * Platform:    Explorer-16-32  
 * Device used: PIC24FJ1024GB610
 * Environment: MPLabX  v5.45   
 * Compiler:    XC16    v1.60   
 */

/* Specifications :
 * **************  
 * Modular programming          
 * Initialize GPIOs             
 * Initialize Uart2             
 * Initialize LCD               
 * Initialize Timer             
 * Welcome message on LCD  5s   
 * Display Date / Heure         
 * Blinking D4 by timer         
 * D3  = S4                     
 */

/* Exercise-1   
 * **********   
 * Build Timekeeping device     
 * S3 = Start count             
 * S4 = Stop                    
 * S5 = Clear                   
 * S6 = Intermediate time       

 * Exercise-2   
 * **********   
 * Setup RTCC Clock Calendar        
 * Use PC Keyboard (Uart2)          
 * by sequence "=../../.. ..:..\r"  
 */

/* What's new today...
 * ****************   
 * Understand Timer structure   
 * Timer1, Timer2/3             
 * Input Capture mode           
 * Output Compare mode          
 * PWM Mode                     
 * Real-Time & Multitask        
 * Using Pointer for parameter  
 * Using Index for Array        
 * Using data structur          
 */


/* Libraries */
#include <stdio.h>
#include <libpic30.h>
#include <stdlib.h>
#include <xc.h>

/* Bits-Configuration of Microcontroler */
#include "ConfigBits.h"

/* Dependencies */
    #include "Gpio.h"
#include "Uart2.h"
#include "Lcd.h"
#include "Timer.h"

/* Own defines */
#include "Main.h"


/* Own prototypes */
void _setUp(void);  // Initialization           
void _loop(void);   // Background Task          
void _tempos();     // Counts Timers & Delays   
void _clock_Calendar(); // Count Hordatage      
void _chrono();         // Count Timekeeper     
void _display_Chrono(unsigned char *);  // Display Timekeeper
void _display_Rtcc(void);  // Display RTCC      
void _setup_HoroDat(unsigned char);
void _clr_Frame(void);
void _manage_Frame(void);
unsigned char _conv2Ascii(unsigned char *);


/* Memories */
unsigned short TWelcome;// Temporisation Welcome mes
unsigned short TBlinkD4;// Temporisation for D4 Led 
unsigned char EdgeS3;   // Detection of Edge on BpS3
unsigned char EdgeS4;   // Detection of Edge on BpS4
unsigned char EdgeS6;   // Detection of Edge on BpS6
unsigned char ChronoStatus;

/* Time memories using structur "time" */
time RtccTime;  // Registers for RTCC Real time 
date RtccDate;  // Registers for RTCC Date      
time SetUpTime; // Registers for setting RTCC   
date SetUpDate; // Registers for setting RTCC   
time ChronoTime;// Registers for Timekeeper     
time InterTime; // Intermediate Time            

/* Frame for receiving setup from PC */
unsigned char Frame[FRAME_SIZE];
unsigned char FrameIdx;


/* Code */
int main(void)
{   __C30_UART = 2;         // Allow using of "printf" on Uart2 by xc16 
    _setUp();               // General initializations                  
    while (TWelcome);       // Wait delay to display "Welcome message"  
    while(1)    _loop();    // Background task ! always !               
    return (0);
}


/* Initalize the application */
void _setUp(void)
{   gpio_Init();            // Initializing IO                  
    uart2_Open(_9600Bds);   // Initializing Serial communication
    timer_Init(TIMER_TICK); // Initializing Timer1 for IT 100ms 
    lcd_Init();             // Initializing Lcd Display         

    RtccTime.count  = 0;   // Setup Rtcc registers 
    RtccTime.second = 0;
    RtccTime.minute = 0;
    RtccTime.hour   = 0;

    RtccDate.day   = 6;     // 6 Avril 2021 
    RtccDate.month = 4;
    RtccDate.year  = 21;

    SetUpDate  = RtccDate;  // Init Date     registers
    SetUpTime  = RtccTime;  // Clear SetUp   registers
    ChronoTime = RtccTime;  // Clear Chrono  registers
    InterTime  = RtccTime;  // Clear Inermed registers
    
    TWelcome = T_5s;        // Setup Times & Delays   
    TBlinkD4 = T_500ms;  

    EdgeS3   = 0;           // Prepare Edges detection
    EdgeS4   = 0;
    EdgeS6   = 0;
    
    ChronoStatus = RTCC;
    
    /* Welcome messages */

    sprintf (LcdBuffL1, "-  Test Timer  -");
    LcdToDoL1 = ON;
    sprintf (LcdBuffL2, "-TP4-Curtis-");
    LcdToDoL2 = ON;   
    lcd_Task(); // Display welcome message on LCD   

    printf ("\r\n---TP4- TP-Timer Curtis---\r\n");
}

/* Background task */
void _loop(void)        // Main loop                            
{   lcd_Task();         // Background task for Lcd display      

/*
    if (RTCC == ChronoStatus)
        _display_Rtcc();
    else
        _display_Chrono(&ChronoStatus);

   if (S3)     // Action on S3 button  => Start Timekeeper 
    {   if (!EdgeS3)
        {   EdgeS3 = ON;
            if ((RTCC == ChronoStatus) || (STOP == ChronoStatus)) ChronoStatus = GO;    // Start Timekeeper 
        }
    }
    else EdgeS3 = OFF;

   if (S4)     // Action on S4 button  => Stop Timekeeper & Return RTCC 
    {   gpio_Set(D3, ON);  // Led status          
       if (!EdgeS4)
        {   EdgeS4 = ON;
            if (ChronoStatus == STOP) ChronoStatus = RTCC;      // Display HoroDate 
            if (ChronoStatus == GO)   ChronoStatus = STOP;      // Stop TimeKeeper  
        }
    }
    else
    {   gpio_Set(D3, OFF);
        EdgeS4 = OFF;
    }

   if (S5)     // Action on S5 button  => Raz Timekeeper    
    {   ChronoTime.minute = 0;
        ChronoTime.second = 0;                  // Raz Timekeeper       
        ChronoTime.count  = 0;
        InterTime.minute  = 0;
        InterTime.second  = 0;                  // Raz Intermediate time
        InterTime.count   = 0;
        ChronoStatus = INTER;   // to display Line 2 Intermediate time  
    }

    if ((S6) && (GO == ChronoStatus)) // Action on S6 button => Intermediate time  
    {   InterTime = ChronoTime;             // Temps Tour  + Chrono 
        ChronoStatus = INTER;               // Mode Intermediaire   
    }
*/
    if(S3)
    {
        if (!EdgeS3)
        {
            EdgeS3 = 1;

            if(ChronoStatus == STOP || ChronoStatus == RTCC)
                ChronoStatus  = GO;
        }
    }
    else
        EdgeS3 = 0;

    if(S4)
    {
        if (!EdgeS4)
        {
            EdgeS4 = 1;

            if(ChronoStatus == GO)
                ChronoStatus = STOP;
            else if (ChronoStatus == STOP)
                ChronoStatus = GO;
        }
    }
    else
        EdgeS4 = 0;

    if(S5)
    {
        ChronoTime.count = 0;
        ChronoTime.second = 0;
        ChronoTime.minute = 0;
        InterTime.count = 0;
        InterTime.second = 0;
        InterTime.minute = 0;

        ChronoStatus = INTER;
    }

    if(S6 && ChronoStatus == GO)
    {
        if (!EdgeS6)
        {
            ChronoStatus = INTER;

            InterTime = ChronoTime;
        }
    }

    if(ChronoStatus == RTCC)
        _display_Rtcc();
    else
        _display_Chrono(&ChronoStatus);
}


/* Display Chrono */
unsigned char * Chrono[] = {"Non", "Stp", "Cpt", "Int", "Eff"};  
void _display_Chrono(unsigned char *Stat)
{   
    sprintf(LcdBuffL1,"Chrono.%s %2d:%02d", Chrono[*Stat], ChronoTime.minute, ChronoTime.second);
    LcdToDoL1 = ON;

    if(*Stat == INTER)
    {
        sprintf(LcdBuffL2,"Tour : %2d:%02d", InterTime.minute, InterTime.second);
        LcdToDoL2 = ON;
        *Stat = GO;
    }
}


/* Display RTCC */
void _display_Rtcc(void)
{ 
    sprintf(LcdBuffL1,"%2d/%02d/%02d %2d:%02d:%02d", RtccDate.day, RtccDate.month, RtccDate.year, RtccTime.hour, RtccTime.minute, RtccTime.second);
    LcdToDoL1 = ON;
}


/* Handler for Uart2 */
void main_Uart2_Receive_Handler(unsigned char Car)    // Perform this character 
{   uart2_Put_Char(Car);    // Echo for the last character      
    _setup_HoroDat(Car);
}


/* Handler for Timer1 */
/*  Call every 100ms  */
void main_Timer1_Handler(void)     // Manage elapse time by Timer1 (10ms)       
{   _tempos();          // Manage Temporisations
    _clock_Calendar();  // Manage RTCC (Time)   
    _chrono();          // Manage Timekeeper    
}


/* Temporisations & Delays */
/*    call every  200ms    */
void _tempos(void)
{   /* Wait time before start main loop */
    if (TWelcome) TWelcome--;
    
    /* Regular Blinking D4 */
    if (0 == TBlinkD4)
    {   TBlinkD4 = T_500ms;  
        gpio_Toggle(D4);
    }
    else TBlinkD4--;
}


/* Time manager (RTCC) */
/*   Call every 200ms  */
void _clock_Calendar(void)
{   if (RtccTime.count < 4) RtccTime.count++;
    else
    {   RtccTime.count = 0;
        if (RtccTime.second < 59) RtccTime.second++;
        else
        {   RtccTime.second = 0;
            if (RtccTime.minute < 59) RtccTime.minute++;
            else
            {   RtccTime.minute = 0;
                if (RtccTime.hour < 23) RtccTime.hour++;
                else
                {   RtccTime.hour = 0;
                    if (RtccDate.day < 30) RtccDate.day++;
                    else
                    {   RtccDate.day = 1;
                        if (RtccDate.month < 12) RtccDate.month++;
                        else 
                        {   RtccDate.month = 1;
                            RtccDate.year++;
                        }
                    }
                }
            }
        }
    }
}


/* Manage Timekeeper */
void _chrono(void)
{   if (GO == ChronoStatus || RTCC == ChronoStatus)
    {    if (ChronoTime.count < 4) ChronoTime.count++;
        else
        {   ChronoTime.count = 0;
            if (ChronoTime.second < 59) ChronoTime.second++;
            else
            {   ChronoTime.second = 0;
                if (ChronoTime.minute < 59) ChronoTime.minute++;
                else    ChronoTime.minute = 0;
            }
        }
    }
}


/* Manage setup of Horodatage */
void _setup_HoroDat(unsigned char Car)
{   
/*
    if (FRAME_HEADER == Car)
    {  _clr_Frame();
                // Header of frame detected 
        Frame[FrameIdx++] = Car;
    }
    else
    {   if (FRAME_ENDER == Car)
        {       // End of frame detected    
            if (FrameIdx >= FRAME_SIZE - 2) _manage_Frame();    // Read the frame received  
            else printf ("\r\n!!! ERREUR de saisie...");
            FrameIdx = 0;
        }
        else    // Not the end => Save the present character in the Frame   
        {   if ((FrameIdx > 0) && (FrameIdx < FRAME_SIZE))   Frame[FrameIdx++] = Car; 
        }
    }
*/
    if(FRAME_HEADER == Car)
    {
        _clr_Frame();
        Frame[FrameIdx++] = Car;
    }
    else
    {
        if(FRAME_ENDER == Car)
        {
            if(FrameIdx >= FRAME_SIZE - 2)
                _manage_Frame();
            else
                printf("!!-Erreur: Taille-!!");
            FrameIdx = 0;
        }   
        else
        {
            if(FrameIdx > 0 && FrameIdx < FRAME_SIZE)
                Frame[FrameIdx++] = Car;
        }
    }
}


/* Prepare Frame memories to receive follow characters */
void _clr_Frame(void)
{ unsigned char I;
    for (I = 0; I < FRAME_SIZE; I++) Frame[I] = 0;
        FrameIdx = 0;
}


/* Manage frame for Setup Hordatage */
void _manage_Frame(void)
{   // FrameIdx 0 1 2 3 4 5 6  7  8 9 10 11 12  
    // Frame = "=,d,d,m,m,y,y,' ',h,h,m, m, \r" 
    SetUpDate.day   = _conv2Ascii(&Frame[FRAME_DAY]);
    SetUpDate.month = _conv2Ascii(&Frame[FRAME_MONTH]);
    SetUpDate.year  = _conv2Ascii(&Frame[FRAME_YEAR]);
    SetUpTime.hour  = _conv2Ascii(&Frame[FRAME_HOUR]);
    SetUpTime.minute= _conv2Ascii(&Frame[FRAME_MINUTE]);
    SetUpTime.second= 0;

    RtccDate = SetUpDate;
    RtccTime = SetUpTime;
}


/* Convert 2 ascii to unsigned decimal character */
unsigned char _conv2Ascii(unsigned char *Data)
{ unsigned char Value;
    Value  = ((*Data++) & 0x0f);
    Value *= 10;
    Value += ((*Data) & 0x0f);
    return Value;
}


// End of Main.c
