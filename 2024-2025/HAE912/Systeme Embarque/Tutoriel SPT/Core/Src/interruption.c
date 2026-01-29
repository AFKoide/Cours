/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"  // Définitions des structures SPI_HandleTypeDef et UART_HandleTypeDef
#include <stdint.h>
#include "main.h"

RTC_HandleTypeDef hrtc;
SPI_HandleTypeDef hspi1;
UART_HandleTypeDef huart2;  // UART2 pour la communication série


void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_RTC_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART2_UART_Init(void);


void Enter_Sleep_Mode(void);
void Wakeup_Callback(void);


/* Main Program --------------------------------------------------------------*/
int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_RTC_Init();
  MX_SPI1_Init();
  MX_USART2_UART_Init();


  /* Initialisation du message */
  uint8_t cardID[5];
  // uint8_t uartBuffer[7];  // Taille fixe précise pour délimiteurs (2) et UID (5)
  uint8_t Startmsg = '<'; // Délimiteur de début
  uint8_t Endmsg   = '>'; // Délimiteur de fin



  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) {
    Enter_Sleep_Mode(); // Met le microcontrôleur en mode sommeil

    if(RFID_Request(0x26, cardID) == 0) { // Carte détectée
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET); // Allume la LED

      // Transfert avec UART
      uint8_t uartBuffer[7] = {Startmsg, cardID[0], cardID[1], cardID[2], cardID[3], cardID[4], Endmsg};
      HAL_UART_Transmit(&huart2, uartBuffer, sizeof(uartBuffer), HAL_MAX_DELAY); // Envoie le message
    }
    else {
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); // Éteint la LED
    }
  }
}


/**
 * @brief Configure le mode Sleep.
 */
void Enter_Sleep_Mode()
{
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); // Éteindre la LED avant d'entrer en veille

  // Activer une interruption externe sur une ligne spécifique (ex : PA0)
  __HAL_GPIO_EXTI_CLEAR_FLAG(GPIO_PIN_0); // Nettoyer le flag d'interruption
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);         // Activer l'IRQ

  // Configurer le microcontrôleur pour entrer en mode veille
  HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
}


/**
 * @brief Gestion de l'interruption (EXTI0).
 */
void EXTI0_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0); // Gérer l'interruption pour PA0
}

/**
 * @brief Callback appelé lorsque l'interruption de réveil se produit.
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == GPIO_PIN_0) // Vérifier si l'interruption vient de PA0
  {
    Wakeup_Callback();
  }
}

/**
 * @brief Fonction appelée après un réveil.
 */
void Wakeup_Callback(void)
{
  // Réactiver les périphériques si nécessaire
  HAL_NVIC_DisableIRQ(EXTI0_IRQn); // Désactiver l'interruption pour éviter les doublons
}





/* Configuration de la carte -------------------------------------------------*/
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}



/**
  * @brief RTC Initialization Function
  * @retval None
  */
static void MX_RTC_Init(void)
{
  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_ALARM;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
}



/**
  * @brief GPIO Initialization Function
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Activer les horloges pour les ports GPIO */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();



  /* Configuration de la LED (PC13) */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // Sortie push-pull
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); // LED éteinte par défaut


  /* Configuration des broches SPI (PA7 - SDA/MOSI, PA5 - SCK, PA3 - MISO, PA4 - NSS) */
  GPIO_InitStruct.Pin = GPIO_PIN_7 | GPIO_PIN_5 | GPIO_PIN_3; // SDA/MOSI, SCK, MISO
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP; // Mode alternatif push-pull pour SPI
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_4; // NSS
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // NSS en sortie push-pull
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET); // NSS désactivé par défaut


  /* Configuration des broches UART (PB8 - TX, PB6 - RX) */
  GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_6; // TX, RX
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP; // Mode alternatif push-pull pour UART
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/**
  * @brief USART2 Initialization Function
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{
  huart2.Instance = USART2; // Instance UART2

  huart2.Init.BaudRate = 115200; // Débit en bauds
  huart2.Init.WordLength = UART_WORDLENGTH_8B; // Longueur des données
  huart2.Init.StopBits = UART_STOPBITS_1; // Nombre de bits de stop
  huart2.Init.Parity = UART_PARITY_NONE; // Aucune parité
  huart2.Init.Mode = UART_MODE_TX_RX; // Transmission et réception
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE; // Pas de contrôle de flux matériel
  huart2.Init.OverSampling = UART_OVERSAMPLING_16; // Suréchantillonnage

  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
      Error_Handler();
  }
}

/**
 * @brief SPI1 Initialization Function
 */
static void MX_SPI1_Init(void)
{
  hspi1.Instance = SPI1; // Instance SPI1

  hspi1.Init.Mode = SPI_MODE_MASTER;                        // Mode maître
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;              // Transmission/réception en 2 lignes
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;                  // Taille des données : 8 bits
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;                // Polarité de l'horloge : basse
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;                    // Phase de l'horloge : premier front
  hspi1.Init.NSS = SPI_NSS_SOFT;                            // NSS géré par logiciel
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;  // Fréquence horloge divisée par 16
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;                   // Transmission du bit de poids fort en premier
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;                   // Mode TI désactivé
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;   // CRC désactivé
  hspi1.Init.CRCPolynomial = 10;                            // Polynôme CRC (inutile ici car désactivé)

  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
      Error_Handler();
  }
}






/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
}



#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
