/* 
 * File:    main.c              
 * Author:  Michel GALLIEN & Curtis MARTELET    
 * Company: UM                  
 *
 * Application  : GPIO Test     
 * Created on   : 11 Mars 2021   
 */

/* Functions : GPIO Test
 * D3 = on switch S4    
 * D4 = is blinking 1s  
 * By Curtis
 * D5 = on if S4 =0 0 and S6 = 1
 * D5 = off if S4 = 1
 * 
 */

/* Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <p24FJ1024GB610.h>

/* Dependencies */
#include "Gpio.h"
#include "Main.h"

/* Own prototypes */
void _setUp(void);      // Genral initialization        
void _loop(void);       // Background task in main loop 

/* Global memories */
unsigned short Compt;   // Timer counter    

/* Private memories */
unsigned char EdgeS3;   // for rise edge detection on S3        
unsigned char EdgeS4;   // for rise edge detection on S4        

/* Main program zone */
int main(void)
{   _setUp();   // General initialization   

    while(1)    _loop();    // Background task          
    
    return (EXIT_SUCCESS);
}


/* Initalize application */
void _setUp(void)
{   Compt = 0;      // Time Counter 
    EdgeS3 = FALSE; // S3 not yet detected  
    EdgeS4 = FALSE; // S4 not yet detected  
    gpio_Init();    // Initialize IO
}


/* Main loop */
void _loop(void)
{   if (S4) gpio_Set(D3, ON);  // Test S4 button status     
    else    gpio_Set(D3, OFF);
    
    Allume();
    Minuterie();

    Compt++;        // Loops counter for elapsed  time      
    if (Compt > 10000)
    {   Compt = 0;
        gpio_Toggle(D4);    // Blink Led4 every second      
    }
}


//========================================

/*  EXERCICE 1  */
/* Fonction M/A */
// Fonction Marche / Arret (Memorisation contacteur) avec sï¿½curitï¿½ ï¿½lectrique   
// S6 (appuyï¿½) Allume D5 et S4 (relachï¿½) Eteind D5 et prioritï¿½ ï¿½ l'arrï¿½t        
// Realisez l'analyse du systeme (organiqgramme)                                
// Decrire les particularites du programme et les solutions trouvees            

void Allume(void)
{
    if(S4)
    {
        if(S6)
            gpio_Set(D5,ON);
        gpio_Set(D5,OFF);
    }
}

//========================================

/* EXERCICE 2 */
/* Minuterie  */
// A l'appui sur S3 on arme une minuterie qui allume D6 pendant 5s environ      
// Employez une boucle programmee, pour "perdre du temps".                      
// Realisez l'analyse du systeme (organiqramme)                                
// Decrire les particularites du programme et les solutions trouvees            

void Boucle(unsigned short Compteur)
{
    int i=0;
    
}

void Minuterie(void)
{
    unsigned short Compteur = 0;
    Compteur = 5;
    int i = 0;
    if(S3)
    {
        while (i < Compteur*10000)
        {
            gpio_Set(D6,ON);
            i++;
        }
        
        Boucle(Compteur);

        gpio_Set(D6,OFF);       
    }
}



//========================================

/* EXERCICE-3- */
/* Telerupteur */
// A chaque appuie sur S3 D7 change d'etat                                      
// Realisez l'analyse du systeme (organiqramme)                                
// Decrire les particularites du programme et les solutions trouvees            

void Bascule(void)
{
    if(S3)
    {
        gpio_Toggle(D7);
    }
}

//========================================

/* EXERCICE-4- */
/*   Decalage  */
// A chaque impulsion sur S3 on decale un point lumineux a gauche               
// A chaque impulsion sur S4 on decale un point lumineux a droite               
// Realisez l'analyse du systeme (organiqramme)                                
// Decrire les particularites du programme et les solutions trouvees            

/*
 * void Décallage(void)
 * {
 *  if (0 == LATA) LATA = 0b00000001;   // First initialization                 

 *  if (S3) // Shift Left   
 *  {   if (FALSE == EdgeS3)
 *      {   EdgeS3 = TRUE;  // Rising edge detected on S3                       
 *          if (LATAbits.LATA7) LATA = 0b00000001; // the last on the left      
 *          else LATA = LATA << 1;  // Shift on the left                        
 *      }
 *  }
 *  else EdgeS3 = FALSE;    

 *  if (S4) // Shift Right  
 *  {   if (FALSE == EdgeS4)
 *      {   EdgeS4 = TRUE;  // Rising edge detected on S4                       
 *          if (LATAbits.LATA0) LATA = 0b10000000;  // the last on the right    
 *          else LATA = LATA >> 1;  // Shift on the right                       
 *      }
 *  }
 *  else EdgeS4 = FALSE;    
 * }
 */


//void Decallage(void)
//{
//    int DATA;
//    
//    if(DATA == 0)
//        DATA = 0b000001;
//    
//    if(S3)
//        DATA = >>1;
//    else if(S4)
//        DATA = <<1;
//    
//    }
//}

//========================================

// End of Main.c
