/* 
 * File:    Adc12b.c                    

 * Author:  Michel GALLIEN              
 * Company: HLEE407  UM                 
 * Created on : 20 avril 2021           

 * Platform:    Explorer-16-32          
 * Device used: PIC24FJ1024GB610        
 * Environment: MPLabX  v4.15           
 * Compiler:    XC16    v1.31           

 ******************************************** 
 * Routine library:   Managing AD Converter * 
 ******************************************** 

 */

/* Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <p24FJ1024GB610.h>

/* Dependencies */
#include "Main.h"
#include "Gpio.h"

/* Defines for AD Converter */
#include "Adc12b.h"


/* Own Prototypes for module AD Converter */
void _sample_Delay(void);
unsigned short _get_Channel(unsigned char);


/* Mémoires globales RAM du module AD Converter */
unsigned short _TabAve[ADC_MAX][AVERAGE_SIZE];

/* Program zone */
/****************/

/* Initialize Lcd on Explorer16-32 */
void adc_Init(void)
// Input:  None                             
// Output: None                             
// Resume: Init for 12bits resolution       
{ unsigned char I, J;
    ANSBbits.ANSB4 = _PIN_ANALOG;
    ANSBbits.ANSB5 = _PIN_ANALOG;

    AD1CON1 = 0; // Settings mode (0x8400)                      
    AD1CON1bits.ADON = 1;       // Turn on ADC12bits            
    AD1CON1bits.MODE12 = 1;     // Mode 12bits                  
    AD1CON1bits.FORM = 0b00;    // Abs Dec unsig right justified
    AD1CON1bits.SSRC = 0b0000;  // SAMP                         

    AD1CON2 = 0; // Select voltage reference source AVdd AVss   
    AD1CON2bits.BUFREGEN = 1;   // =0 in Fifo;  =1 // in Buffer 
    AD1CON2bits.CSCNA = 0;      // Not autoscan inputs          
    AD1CON2bits.ALTS = 0;       // =0 use chanel input          

    AD1CON3 = 0; // Clock system (0x0200) or (0x0304)           
    AD1CON3bits.SAMC = 0b00010;     // 3 x Tad                  
    AD1CON3bits.ADCS = 0b00000000;  // Tad = 1Tcy               

    AD1CON4 = 0;    // No DMA                                   

    AD1CON5 = 0;    // No AutoScan system & No Interrupt & No CM
    
    AD1CHS  = 0;            // Sample select register           
    AD1CHSbits.CH0NB = 0;   // Sample B Channel 0 Negativ input 
    AD1CHSbits.CH0SB = 0;   // Sample B Channel 0 Positiv input 
    AD1CHSbits.CH0NA = 0;   // Sample A Channel 0 Negativ input 
    AD1CHSbits.CH0SA = 0;   // Sample A Channel 0 Positiv input 

    ANCFG = 0;              // Band gap disable                 
    
    AD1CHITH = 0;           // Scan Compare disable HighByte    
    AD1CHITL = 0;           // Scan Compare disable LowByte     

    AD1CSSH = 0;            // Input Scan Select HighByte       
    AD1CSSL = 0;            // Input Scan Select LowByte        

    AD1CTMENH = 0;          // CTMU not used  HighByte          
    AD1CTMENL = 0;          // CTMU not used  LowByte           

    IFS0bits.AD1IF = 0;     // Clear interrupt flag             
    IEC0bits.AD1IE = 0;     // Disable interrupt                
    
    for ( J=0; J < ADC_MAX; J++)    for (I=0; I < AVERAGE_SIZE; I++)    _TabAve[J][I] = 0;
}


/* Read ADC channel */
unsigned short adc_Read_Channel(unsigned char Channel)
{   AD1CHS = Channel;       // Select the channel (0-23)        
    AD1CON1bits.SAMP = 1;   // Start Sample                     
    _sample_Delay();        // Wait Sample delay                
    AD1CON1bits.SAMP = 0;   // Stop Sample & Start conversion   
    while (!AD1CON1bits.DONE);      // Wait conversion completed
    return _get_Channel(Channel);   // Read the result          
}


//* Read averaging value for one channel */
unsigned short adc_Average_Channel(unsigned char Channel)
{ unsigned short I, Somme;
    for (I=AVERAGE_SIZE; I > 0; I--)
    {    _TabAve[Channel][I] = _TabAve[Channel][I-1];   // Shift average array  
    }
    _TabAve[Channel][0] = adc_Read_Channel(Channel);    // Write the last value 
    Somme = 0;
    for (I=0; I < AVERAGE_SIZE; I++)    // Add for the average                  
    {   Somme += _TabAve[Channel][I];
    }
    return (Somme / AVERAGE_SIZE);      // Calculate the average                
}


/* Get result value */
unsigned short _get_Channel(unsigned char Channel)
{unsigned short Data = 0;
    switch (Channel)
    {   case ADC_AN0:
            Data = ADC1BUF0;
            break;

        case ADC_AN1:
            Data = ADC1BUF1;
            break;

        case ADC_AN2:
            Data = ADC1BUF2;
            break;

        case ADC_AN3:
            Data = ADC1BUF3;
            break;

        case ADC_AN4:
            Data = ADC1BUF4;
            break;

        case ADC_AN5:
            Data = ADC1BUF5;
            break;

        case ADC_AN6:
            Data = ADC1BUF6;
            break;

        case ADC_AN7:
            Data = ADC1BUF7;
            break;

        case ADC_AN8:
            Data = ADC1BUF8;
            break;

        case ADC_AN9:
            Data = ADC1BUF9;
            break;

        case ADC_AN10:
            Data = ADC1BUF10;
            break;

        case ADC_AN11:
            Data = ADC1BUF11;
            break;

        case ADC_AN12:
            Data = ADC1BUF12;
            break;

        case ADC_AN13:
            Data = ADC1BUF13;
            break;

        case ADC_AN14:
            Data = ADC1BUF14;
            break;

        case ADC_AN15:
            Data = ADC1BUF15;
            break;

        case ADC_AN16:
            Data = ADC1BUF16;
            break;

        case ADC_AN17:
            Data = ADC1BUF17;
            break;

        case ADC_AN18:
            Data = ADC1BUF18;
            break;

        case ADC_AN19:
            Data = ADC1BUF19;
            break;

        case ADC_AN20:
            Data = ADC1BUF20;
            break;

        case ADC_AN21:
            Data = ADC1BUF21;
            break;

        case ADC_AN22:
            Data = ADC1BUF22;
            break;

        case ADC_AN23:
            Data = ADC1BUF23;
            break;
    }
    return Data;
}


/* Background task forAD Converter */
void adc_Task(void)
{	
}


/* Delay for Sample time */
void _sample_Delay(void)
{ unsigned short I;
    for (I=0; I<10000; I++)
    {
    }
}


// End of file "Adc12b.c"                                                       
