#ifndef _ADC12B_H_
#define _ADC12B_H_

/* 
 * Header-File:     Adc12b.h            

 * Author:  Michel GALLIEN              
 * Company: HLEE407  UM                 
 * Created on : 10 Avril 2021           

 * Platform:    Explorer-16-32          
 * Device used: PIC24FJ1024GB610        
 * Environment: MPLabX  v4.15           
 * Compiler:    XC16    v1.31           

 ************************************   
 * Routine library:   Managing Adc  *   
 ************************************   

 */

/* Libraries */
#include <p24FJ1024GB610.h>

/* General Constants */
enum
{   ADC_AN0,  ADC_AN1,  ADC_AN2,  ADC_AN3,  ADC_AN4,  ADC_AN5,
    ADC_AN6,  ADC_AN7,  ADC_AN8,  ADC_AN9,  ADC_AN10, ADC_AN11,
    ADC_AN12, ADC_AN13, ADC_AN14, ADC_AN15, ADC_AN16, ADC_AN17,
    ADC_AN18, ADC_AN19, ADC_AN20, ADC_AN21, ADC_AN22, ADC_AN23,
    ADC_MAX
};

#define ADC_POTAR   ADC_AN5
#define ADC_TEMP    ADC_AN4
#define ADC_TP      ADC_AN22
#define ADC_CTMU    ADC_AN23

#define AVERAGE_SIZE 8


/* Usefull variables */


/* Prototypes for Lcd Display */
void adc_Init(void);    // Initialize AD converter 12bits       
void adc_Task(void);    // Background task for adc 12bits       
unsigned short adc_Read_Channel(unsigned char);                 
unsigned short adc_Average_Channel(unsigned char);

#endif // __ADC12B_H_	// end of the definitions of Adc12b.h   


/* End of file */                                                      
