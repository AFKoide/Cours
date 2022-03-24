#include "stm32l1xx.h"

#define POINT 100
#define TRAIT 300

typedef struct {
    char lettre;
    char morse[10];
}code;

void TIM5_Config();
void TIM5_delay_ms(int delay);
void Init_GPIO();
void Convertisseur(char* morse);

int main()
{
    TIM5_Config();
    Init_GPIO();

    //int NB_CODES = 26;
    code alphabet[26]= {
        {'A',"._"   }, {'B',"_..." },  {'C',"_._." },
        {'D',"_.."  }, {'E',"."    },  {'F',".._." },
        {'G',"__."  }, {'H',"...." },  {'I',".."   },
        {'J',".___" }, {'K',"_._"  },  {'L',"._.." },
        {'M',"__"   }, {'N',"_."   },  {'O',"___"  },
        {'P',".__." }, {'Q',"__._" },  {'R',"._."  },
        {'S',"..."  }, {'T',"_"    },  {'U',".._"  },
        {'V',"..._" }, {'W',".__"  },  {'X',"_.._" },
        {'Y',"_.__" }, {'Z',"__.." }
    };

    char test[] = "CURTIS";

    while(1) {
        for(int i = 0; test[i] != 0; i++) {
            if(test[i] == ' ')
                TIM5_delay_ms(1000);
            else {
                char* morse = alphabet[test[i]-'A'].morse;
                Convertisseur(morse);
                TIM5_delay_ms(200);
            }
        }
        TIM5_delay_ms(2000);
    }
    return 0;
}

void Convertisseur(char* morse)
{
    for(int i = 0; morse[i] != 0; i++) {
    GPIO_SetBits(GPIOB,GPIO_Pin_7);

    switch((int)(morse[i])){
        case '_' :
            TIM5_delay_ms(TRAIT);
            break;

        case '.' :
            TIM5_delay_ms(POINT);
            break;
        }
        GPIO_ResetBits(GPIOB,GPIO_Pin_7);

        TIM5_delay_ms(100);
    }
}

void Init_GPIO() {
    /* Activer GPIOB sur AHB */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);
    /* Configurer PB7 */
    GPIO_InitTypeDef gpio_b;
    GPIO_StructInit(&gpio_b);
    gpio_b.GPIO_Mode = GPIO_Mode_OUT;
    gpio_b.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOB,&gpio_b);
}

void TIM5_Config()
{
    /* Activer TIM5 sur APB1 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
    /* Configurer TIM5 : prescalaire à 1 ms, periode au maximum */

    TIM_TimeBaseInitTypeDef DelayTimer; // Crée la structure de configuration
    TIM_TimeBaseStructInit(&DelayTimer); // Initialise la structure
    DelayTimer.TIM_Prescaler = 16000-1; // Défini le prescalaire
    DelayTimer.TIM_Period = 0xFFFFFFFFU; // Taille du compteur (pleine)
    TIM_TimeBaseInit(TIM5,&DelayTimer); // Configure le timer

    /* Tprescalaire = Prescalaire/16x10^6 (fréquence de l'horloge du microcontroleur) */
}

// Fonction Delai en millisecondes
void TIM5_delay_ms(int delay)
{
    TIM_SetCounter(TIM5,0); // Initialise le compteur
    TIM_Cmd(TIM5, ENABLE); // Active le compteur
    while(TIM_GetCounter(TIM5)<delay) {} // Attend que l'on atteigne le delai
    TIM_Cmd(TIM5, DISABLE); // Désactive le compteur
}
