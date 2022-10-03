/* 
 * File:  "Lcd.c"               

 * Author:  Michel GALLIEN      
 * Company: UM HLEE407          
 * Created on : 27 mars 2021    

 * Platform:    Explorer-16-32  
 * Device used: PIC24FJ1024GB610
 * Environment: MPLabX  v5.45   
 * Compiler:    XC16    v1.60   

 ******************************************** 
 * Routine library:   Managing Lcd Display  * 
 ******************************************** 

 */

/* Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>


/* Dependencies */
#include "Main.h"
#include "Gpio.h"

/* Defines for Lcd Diplay */
#include "Lcd.h"


/* Own Prototypes for module Lcd8b */
void _lcd_Wait(unsigned short);
void _lcd_Clear(void);
void _clock(void);
void _data(unsigned char);
void _cmd(unsigned char);
void _home_1(void);
void _home_2(void);
void _line1(char []);
void _line2(char []);


/* M�moires globales RAM du module Lcd */
char LcdBuffL1[LCD_CPL + 1];   // Buffer for characters to Line1
char LcdBuffL2[LCD_CPL + 1];   // Buffer for characters to Line2
char LcdToDoL1;                // Ask to display on line 1      
char LcdToDoL2;                // Ask to display on line 2      


/* Code */

/* Initialize Lcd on Explorer16-32 */
void lcd_Init(void)
// Input:  None                             
// Output: None                             
// Resume: LCD 2x16 characters mode 8bits   
{	LCD_E  = 0;     // Reset Clock line     
	LCD    = 0;     // Reset Data lines     
	LCD_RW = 0;     // Write mode           
	LCD_RS = 0;
    _lcd_Clear();   // Clear buffer for Lcd 

	_lcd_Wait(2500);// Reset Afficheur Lcd  

	LCD =0x03;		// Set function 8-bits  
	_lcd_Wait(40);
	_clock();
	_lcd_Wait(5);
	_clock();
	_lcd_Wait(5);
	_clock();
	_lcd_Wait(2);

	_cmd(0x38);     // Set function 8-bits 2-lines
	_lcd_Wait(250); 

	_cmd(0x0F);     // Set display on cursor on blink on
	_lcd_Wait(250);

	_cmd(0x06);     // Cursor shift direction 
	_lcd_Wait(250);

	_cmd(0x01);     // Clear lcd
	_lcd_Wait(2500);
}

/* Clear 2 lines of display */
void _lcd_Clear(void)
{ unsigned char i;
	LcdToDoL1 =0;	// Pas de demande d'affichage sur Ligne 1
	for (i =0; i < LCD_CPL; i++) LcdBuffL1[i] =' ';	// Efface Buffer ligne1
	LcdBuffL1[LCD_CPL] ='\0';   // Fin de message sur ligne 1

	LcdToDoL2 =0;	// Pas de demande d'affichage sur Ligne 2
	for ( i=0; i < LCD_CPL; i++) LcdBuffL2[i] =' ';	// Efface Buffer ligne2
	LcdBuffL2[LCD_CPL] ='\0';   // Fin de message sur ligne 2
}


/* Background task for Lcd display */
void lcd_Task(void)
{	if (ON == LcdToDoL1)	// Display on LCD Line 1
	{	_line1(LcdBuffL1);
		LcdToDoL1 =OFF;
	}
	if (ON == LcdToDoL2)	// Display on LCD Line 2
	{	_line2(LcdBuffL2);
		LcdToDoL2 =OFF;
	}
}


/* Clock E on LCD */
void _clock(void)
{	LCD_E =1;
	_lcd_Wait(10);
	LCD_E = 0;
}


// Function: 	void _cmd(unsigned char Cmd)
// Description: Write a command on Lcd
// Parameter:	b Value 8bits
// Return:		None
void _cmd(unsigned char Cmd)
{	LCD    =Cmd;
	LCD_RS =0;
	_clock();
	_lcd_Wait(10);
}


// Write data (RS=1)
// Function: 	void Lcd_cmd(unsigned char Data)
// Description: Write data on Lcd
// Param�tre:	b Data 8bits
// Return:		None
void _data(unsigned char Data)
{	LCD    = Data;
	LCD_RS = 1;
	_clock();
	_lcd_Wait(10);
}


/* Delay for Lcd command */
void _lcd_Wait(unsigned short x)
{ unsigned short i, j;
	for (i=x ; i>1; i--) for (j=0; j<=110; j++);
}

/* Cursor Home line 1 */
void _home1(void)
{	_cmd(0x80);		// Curseur home line 1 '00'
}


/* Cursor Home line 2 */
void _home2(void)
{	_cmd(0xC0);		// Curseur en position '64' !
}


/* Write buffer on line 1 */
void _line1(char *Buff)
{ unsigned char i;
	_home1();
	for (i =0; i < LCD_CPL; i++) _data(*Buff++);
}


/* Write buffer on line 2 */
void _line2(char Buff[])
{ unsigned char i;
	_home2();
	for (i =0; i < LCD_CPL; i++) _data(Buff[i]);
}                                                