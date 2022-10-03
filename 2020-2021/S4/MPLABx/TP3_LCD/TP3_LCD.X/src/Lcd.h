#ifndef _LCD_H_
#define _LCD_H_

/* 
 * Header-File: "Lcd.h"         

 * Author:  Michel GALLIEN      
 * Company: UM HLEE407          
 * Created on : 27 mars 2021    

 * Platform:    Explorer-16-32  
 * Device used: PIC24FJ1024GB610
 * Environment: MPLabX  v5.45   
 * Compiler:    XC16    v1.60   

 ************************************
 * Routine library:   Managing Lcd  *
 ************************************

 */

/* Libraries */
#include <xc.h>

/* General Constants */
#define LCD_CPL 16  // Number of characters by line on LCD


/* Statments zone */


/* Usefull variables */
extern char LcdBuffL1[];   // Tampon des caratères à écrire sur Ligne1 
extern char LcdBuffL2[];   // Tampon des caratères à écrire sur Ligne2 
extern char LcdToDoL1;     // Demande d'affichage sur la ligne 1 en TdF
extern char LcdToDoL2;     // Demande d'affichage sur la ligne 2 en TdF

/* Prototypes for Lcd Display */
void lcd_Init(void);    // Initialize Lcd display           
void lcd_Task(void);    // Background task for Lcd display  

#endif //   __LCD_H_	// end of the defines for Lcd.h     


/* End of file */                                                      
