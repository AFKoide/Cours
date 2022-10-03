#ifndef _LCD_H_
#define _LCD_H_

/* 
 * Header-File:     Lcd.h               

 * Author:  Michel GALLIEN              
 * Company: HLEE407  UM                 
 * Created on : 10 avril 2021           

 * Platform:    Explorer-16-32          
 * Device used: PIC24FJ1024GB610        
 * Environment: MPLabX  v4.15           
 * Compiler:    XC16    v1.31           

 ************************************   
 * Routine library:   Managing Lcd  *   
 ************************************   

 */

/* Libraries */
#include <p24FJ1024GB610.h>

/* General Constants */
#define LCD_CPL 16  // Number of characters by line on LCD


/* Statments zone */


/* Usefull variables */
extern char LcdBuffL1[];   // tampon des caratères à écrire sur Ligne1
extern char LcdBuffL2[];   // tampon des caratères à écrire sur Ligne1
extern char LcdToDoL1;     // Demande d'affichage sur la ligne 1 en TdF
extern char LcdToDoL2;     // Demande d'affichage sur la ligne 2 en TdF

/* Prototypes for Lcd Display */
void lcd_Init(void);    // Initialize Lcd display           
void lcd_Task(void);    // Background task for Lcd display  

#endif // __LCD_H_	// end of the definitions of Lcd.h 


/* End of file */                                                      
