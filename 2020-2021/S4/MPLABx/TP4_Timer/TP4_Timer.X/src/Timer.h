#ifndef _TIMER_H_
#define _TIMER_H_

/* 
 * Header-File:     Timer.h             

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
#include <xc.h>

/* General Constants */
#define TIMER_TICK  100  // 100ms
#define T_200ms  1  // TIMER_TICK
#define T_500ms  2  // TIMER_TICK
#define T_1s     5  // TIMER_TICK
#define T_2s    10  // TIMER_TICK
#define T_5s    25  // TIMER_TICK


/* Declaration of Structure "time" */
struct time
{   unsigned char count;
    unsigned char second;
    unsigned char minute;
    unsigned char hour;
};

typedef struct time time;


/* Declaration of Structure "date" */
struct date
{   unsigned char day;
    unsigned char month;
    unsigned char year;
};

typedef struct date date;


/* Prototypes for Timer */
void timer_Init(unsigned short);    // Initialize Timer     

#endif // __TIMER_H_	// end of the definitions of Timer.h


/* End of file */                                                      
