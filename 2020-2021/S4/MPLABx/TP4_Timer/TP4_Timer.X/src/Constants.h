/*******************************************************************************
 Library generic "Constants.h" Header File

 * Company:     GrandSud Automation 
 * Author:      Michel GALLIEN      
 * Create:      14/08/2017          
 * File Name:   "_Constants.h"      
 * Environment: MPLab-X v4.00       
 * Compiler:    Any                 
 * Device:      Any                 
 * 
 * Summary:      This file contains the generic constants for All applications  
 ******************************************************************************/
#ifndef __CONSTANTS_H_
#define __CONSTANTS_H_


// Global Constants
//-----------------
#define _FALSE	0   // False logical condition  
#define _TRUE 	1   // True  logical condition  
#define _FAUX	0	// False logical condition  
#define _VRAI	1	// True  logical condition  

#define _KO 0		// No validate state        
#define _OK 1		// Validate state           

#define _TODO	1	// Action to do             
#define _AVAILABLE 0// Ressource available      

#define _OFF	 0	// Element not set          
#define	_ON		 1	// Element set              
#define _PULSE	-1	// Command for one pulse    
#define _FLASH	-1	// Command for Leds         


//=========================================================
//=====   Times count in Mtt ~ BDT = Timebase = 100ms =====
//=========================================================


#define T100ms	1*T10ms    // 100ms =1   in Mtt   
#define T200ms	2*T10ms
#define T300ms	3*T10ms
#define T400ms	4*T10ms
#define T500ms	5*T10ms
#define T600ms	6*T10ms
#define T700ms	7*T10ms
#define T800ms	8*T10ms
#define T900ms	9*T10ms

#define T1s    10*T100ms    //    1s =10 in Mtt   
#define T2s	   20*T100ms
#define T3s	   30*T100ms
#define T4s	   40*T100ms
#define T5s	   50*T100ms
#define T6s	   60*T100ms
#define T7s	   70*T100ms
#define T8s	   80*T100ms
#define T9s	   90*T100ms
#define T10s  100*T100ms
#define T11s  110*T100ms
#define T12s  120*T100ms
#define T13s  130*T100ms
#define T14s  140*T100ms
#define T15s  150*T100ms
#define T16s  160*T100ms
#define T17s  170*T100ms
#define T18s  180*T100ms
#define T19s  190*T100ms

#define T20s  200*T100ms    // 20s =200 in Mtt
#define T25s  250*T100ms
#define T30s  300*T100ms
#define T35s  350*T100ms
#define T40s  400*T100ms
#define T45s  450*T100ms	
#define T50s  500*T100ms
#define T55s  550*T100ms
#define T60s  600*T100ms

#define T1mn   600*T100ms	//  1' =600  in Mtt   
#define T2mn  1200*T100ms
#define T3mn  1800*T100ms
#define T4mn  2400*T100ms
#define T5mn  3000*T100ms
#define T10mn 6000*T100ms

#endif		// End of __CONSTANTS_H_


/* End of file */
