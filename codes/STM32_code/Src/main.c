/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32l475e_iot01.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TERMINAL_USE

#define PASSWORD   "12345678"
#define RemotePORT	1234

#define WIFI_WRITE_TIMEOUT 10000
#define WIFI_READ_TIMEOUT  11000

#define CONNECTION_TRIAL_MAX          3

#if defined (TERMINAL_USE)
#define TERMOUT(...)  printf(__VA_ARGS__)
#else
#define TERMOUT(...)
#endif
#define DATA_RECEIVE_DURATION 5000



/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

ADC_HandleTypeDef hadc1;



I2C_HandleTypeDef hi2c2;



SPI_HandleTypeDef hspi3;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart3;



/* USER CODE BEGIN PV */
#if defined (TERMINAL_USE)
extern UART_HandleTypeDef hDiscoUart;
#endif /* TERMINAL_USE */
static uint8_t RxData [1000];     //receive buffer
static uint8_t Rxdata [1000];      //chosen equipment buffer
static uint8_t TxData [1000];       //send buffer
extern  SPI_HandleTypeDef hspi;
uint8_t RemoteIP_AP[] = {192,168,4,1};
uint8_t  IP_Addr[4] = {0};
		  int32_t Socket = -1;
		  uint16_t Datalen; // length of data
		  int32_t ret;
		  int16_t Trials = 3; // number of connecting tries
char SSID [12][20];
/*float sensetivity=0.0066;
float offset=2.5;*/
int mode=0;
int adc_val;
ADC_ChannelConfTypeDef sConfig;
uint8_t byte;          // commande buffer
//int count=0;
uint8_t a=0;
uint8_t j;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

void PeriphCommonClock_Config(void);
static void MX_GPIO_Init(void);

static void MX_I2C2_Init(void);

static void MX_SPI3_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART3_UART_Init(void);

static void MX_ADC1_Init(void);
/* USER CODE BEGIN PFP */
static void SystemClock_Config(void);
#if defined (TERMINAL_USE)
#ifdef __GNUC__
/* When using GCC compiler, redirect the standard output (e.g., printf)
   by defining __io_putchar(). */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else /* For other compilers, use fputc() to redirect standard output. */
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
#endif /* TERMINAL_USE */
#if defined (TERMINAL_USE)

/**
  * @brief  Retargets the C library TERMOUT function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART1 and Loop until the end of transmission */
	/* Transmit the character over UART using HAL driver.
	     Loops until transmission is complete. */
  HAL_UART_Transmit(&hDiscoUart, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}
#endif /* TERMINAL_USE */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
mode=0;

	  /* Configure the system clock */

	  /* Configure LED2 */


  /* USER CODE END 1 */


  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  //MX_DFSDM1_Init();
  MX_I2C2_Init();
  //MX_QUADSPI_Init();
  MX_SPI3_Init();
 MX_USART1_UART_Init();
  MX_USART3_UART_Init();
 // MX_USB_OTG_FS_PCD_Init();
 //MX_ADC1_Init();
  /* USER CODE BEGIN 2 */

  // controling the number of equipments with name ESP32_AP
  for (int i=0;i<3;i++){
  	sprintf(SSID[i],"ESP32_AP%d",i+1);


  }
  BSP_LED_Init(LED2);

 	#if defined (TERMINAL_USE)
 	  /* Initialize all configured peripherals */
 	  hDiscoUart.Instance = USART1;
 	  hDiscoUart.Init.BaudRate = 115200;
 	  hDiscoUart.Init.WordLength = UART_WORDLENGTH_8B;
 	  hDiscoUart.Init.StopBits = UART_STOPBITS_1;
 	  hDiscoUart.Init.Parity = UART_PARITY_NONE;
 	  hDiscoUart.Init.Mode = UART_MODE_TX_RX;
 	  hDiscoUart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
 	  hDiscoUart.Init.OverSampling = UART_OVERSAMPLING_16;
 	  hDiscoUart.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
 	  hDiscoUart.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

 	  BSP_COM_Init(COM1, &hDiscoUart);
 	#endif /* TERMINAL_USE */
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {



    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  //TERMOUT("chose CONTROL mode");
	  //HAL_UART_Receive(&hDiscoUart, &a, 1, 1000);
	  //if(a==0){

	 //TERMOUT("simple CONTROL");
	  if(mode==1){
	  		//for(int i=0;i<2;i++){
		  TERMOUT("bouton 1");
	  			  connect_and_receive_EQP1(SSID[1], RemoteIP_AP);
	  			 // mode=0;
	  		  }
		else if(mode==3){
				//for(int i=0;i<2;i++){
					  TERMOUT("bouton 3");
		  		 for(int i=0;i<3;i++){
		  			 connect_and_receive(SSID[i], RemoteIP_AP);
		  			//chek_by_EQP (SSID[0], RemoteIP_AP);
		  		  }}
		  else if(mode==4){
				//for(int i=0;i<2;i++){
					  TERMOUT("bouton 2");
		  		  for(int i=0;i<2;i++){
		  		 connect_and_send(SSID[1], RemoteIP_AP);
		  	  }}
		  else if (mode==2){
				for(int i=0;i<2;i++){
					  TERMOUT("bouton 4");
		  	  		  for(int i=0;i<2;i++) {
		  	  		chek_by_EQP (SSID[i], RemoteIP_AP);}}//}
	 // }
/*if(a==1){
	 TERMOUT("advanced CONTROL");
	  	  if(mode==1){
	  		 TERMOUT("Chose number");
	  		 HAL_UART_Receive(&hDiscoUart, &j, 1, 1000);
	  		 for(int i=0;i<j;i++){
	  			chek_by_EQP (SSID[i], RemoteIP_AP);
	  	  }}


	  	 else if (mode==2){
	  		 TERMOUT("Chose number");
	  		 HAL_UART_Receive(&hDiscoUart, &j, 1, 1000);
	  		for(int i=0;i<j;i++){
	  			  			  connect_and_receive(SSID[i], RemoteIP_AP);
	  	 }}
	  	 else if (mode==3){
	  		TERMOUT("Chose number");
	  			  		 HAL_UART_Receive(&hDiscoUart, &j, 1, 1000);
	  			  	 connect_and_receive(SSID[j], RemoteIP_AP);
	  	 }
	  	 else if (mode==4){
	  		TERMOUT("Chose number");
	  			  			  		 HAL_UART_Receive(&hDiscoUart, &j, 1, 1000);
	  			  			  	 connect_and_send(SSID[j], RemoteIP_AP);
	  	 }}*/

	  	  //else

	  	 //mode=0; }
	  	  //else{break;}



	  	// }//}
  /* USER CODE END 3 */



}
}
}
/**
  * @brief System Clock Configuration
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* MSI is enabled after System reset, activate PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLP = 7;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB|RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI1;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLLSAI1;
  PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_MSI;
  PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
  PeriphClkInit.PLLSAI1.PLLSAI1N = 24;
  PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
  PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_48M2CLK|RCC_PLLSAI1_ADC1CLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_MultiModeTypeDef multimode = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the ADC multi-mode
  */
  multimode.Mode = ADC_MODE_INDEPENDENT;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;

  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief DFSDM1 Initialization Function
  * @param None
  * @retval None
  */


/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x00000E14;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief QUADSPI Initialization Function
  * @param None
  * @retval None
  */


/**
  * @brief SPI3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI3_Init(void)
{

  /* USER CODE BEGIN SPI3_Init 0 */

  /* USER CODE END SPI3_Init 0 */

  /* USER CODE BEGIN SPI3_Init 1 */

  /* USER CODE END SPI3_Init 1 */
  /* SPI3 parameter configuration*/
  hspi3.Instance = SPI3;
  hspi3.Init.Mode = SPI_MODE_MASTER;
  hspi3.Init.Direction = SPI_DIRECTION_2LINES;
  hspi3.Init.DataSize = SPI_DATASIZE_4BIT;
  hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi3.Init.NSS = SPI_NSS_SOFT;
  hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi3.Init.CRCPolynomial = 7;
  hspi3.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi3.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI3_Init 2 */

  /* USER CODE END SPI3_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief USB_OTG_FS Initialization Function
  * @param None
  * @retval None
  */


/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, M24SR64_Y_RF_DISABLE_Pin|M24SR64_Y_GPO_Pin|ISM43362_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, ARD_D10_Pin|SPBTLE_RF_RST_Pin|ARD_D9_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, ARD_D8_Pin|ISM43362_BOOT0_Pin|ISM43362_WAKEUP_Pin|LED2_Pin
                          |SPSGRF_915_SDN_Pin|ARD_D5_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, USB_OTG_FS_PWR_EN_Pin|PMOD_RESET_Pin|STSAFE_A100_RESET_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SPBTLE_RF_SPI3_CSN_GPIO_Port, SPBTLE_RF_SPI3_CSN_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, VL53L0X_XSHUT_Pin|LED3_WIFI__LED4_BLE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SPSGRF_915_SPI3_CSN_GPIO_Port, SPSGRF_915_SPI3_CSN_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(ISM43362_SPI3_CSN_GPIO_Port, ISM43362_SPI3_CSN_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : M24SR64_Y_RF_DISABLE_Pin M24SR64_Y_GPO_Pin ISM43362_RST_Pin ISM43362_SPI3_CSN_Pin */
  GPIO_InitStruct.Pin = M24SR64_Y_RF_DISABLE_Pin|M24SR64_Y_GPO_Pin|ISM43362_RST_Pin|ISM43362_SPI3_CSN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : USB_OTG_FS_OVRCR_EXTI3_Pin SPSGRF_915_GPIO3_EXTI5_Pin SPBTLE_RF_IRQ_EXTI6_Pin ISM43362_DRDY_EXTI1_Pin */
  GPIO_InitStruct.Pin = USB_OTG_FS_OVRCR_EXTI3_Pin|SPSGRF_915_GPIO3_EXTI5_Pin|SPBTLE_RF_IRQ_EXTI6_Pin|ISM43362_DRDY_EXTI1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : BUTTON_MODE_3_Pin */
  GPIO_InitStruct.Pin = BUTTON_MODE_3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BUTTON_MODE_3_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ARD_D1_Pin ARD_D0_Pin */
  GPIO_InitStruct.Pin = ARD_D1_Pin|ARD_D0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : ARD_D10_Pin SPBTLE_RF_RST_Pin ARD_D9_Pin */
  GPIO_InitStruct.Pin = ARD_D10_Pin|SPBTLE_RF_RST_Pin|ARD_D9_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : ARD_D4_Pin */
  GPIO_InitStruct.Pin = ARD_D4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
  HAL_GPIO_Init(ARD_D4_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ARD_D13_Pin ARD_D12_Pin ARD_D11_Pin */
  GPIO_InitStruct.Pin = ARD_D13_Pin|ARD_D12_Pin|ARD_D11_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : ARD_D3_Pin */
  GPIO_InitStruct.Pin = ARD_D3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(ARD_D3_GPIO_Port, &GPIO_InitStruct);



  /*Configure GPIO pins : ARD_D8_Pin ISM43362_BOOT0_Pin ISM43362_WAKEUP_Pin LED2_Pin
                           SPSGRF_915_SDN_Pin ARD_D5_Pin SPSGRF_915_SPI3_CSN_Pin */
  GPIO_InitStruct.Pin = ARD_D8_Pin|ISM43362_BOOT0_Pin|ISM43362_WAKEUP_Pin|LED2_Pin
                          |SPSGRF_915_SDN_Pin|ARD_D5_Pin|SPSGRF_915_SPI3_CSN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : LPS22HB_INT_DRDY_EXTI0_Pin BUTTON_MODE_5_Pin ARD_D2_Pin HTS221_DRDY_EXTI15_Pin
                           BUTTON_MODE_4_Pin */
  GPIO_InitStruct.Pin = LPS22HB_INT_DRDY_EXTI0_Pin|BUTTON_MODE_5_Pin|ARD_D2_Pin|HTS221_DRDY_EXTI15_Pin
                          |BUTTON_MODE_4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : USB_OTG_FS_PWR_EN_Pin SPBTLE_RF_SPI3_CSN_Pin PMOD_RESET_Pin STSAFE_A100_RESET_Pin */
  GPIO_InitStruct.Pin = USB_OTG_FS_PWR_EN_Pin|SPBTLE_RF_SPI3_CSN_Pin|PMOD_RESET_Pin|STSAFE_A100_RESET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : VL53L0X_XSHUT_Pin LED3_WIFI__LED4_BLE_Pin */
  GPIO_InitStruct.Pin = VL53L0X_XSHUT_Pin|LED3_WIFI__LED4_BLE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : BUUTON_MODE1_Pin BUTTON_MODE_2_Pin*/
  GPIO_InitStruct.Pin = ARD_D3_Pin|BUTTON_MODE_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PMOD_SPI2_SCK_Pin*/
 /* GPIO_InitStruct.Pin = PMOD_SPI2_SCK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(PMOD_SPI2_SCK_GPIO_Port, &GPIO_InitStruct);*/
  GPIO_InitStruct.Pin = PMOD_SPI2_SCK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING; // Or GPIO_MODE_IT_FALLING or GPIO_MODE_IT_RISING_FALLING
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(PMOD_SPI2_SCK_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PMOD_UART2_CTS_Pin PMOD_UART2_RTS_Pin PMOD_UART2_TX_Pin PMOD_UART2_RX_Pin */
  GPIO_InitStruct.Pin = PMOD_UART2_CTS_Pin|PMOD_UART2_RTS_Pin|PMOD_UART2_TX_Pin|PMOD_UART2_RX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : ARD_D15_Pin ARD_D14_Pin */
  GPIO_InitStruct.Pin = ARD_D15_Pin|ARD_D14_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
   HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

   HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
      HAL_NVIC_EnableIRQ(EXTI2_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void EXTI2_IRQHandler(void){
	HAL_GPIO_EXTI_IRQHandler(BUTTON_MODE_4_Pin);
}
void EXTI0_IRQHandler(void){
	HAL_GPIO_EXTI_IRQHandler(ARD_D3_Pin);

}
void EXTI15_10_IRQHandler(void)
{
HAL_GPIO_EXTI_IRQHandler(ARD_D2_Pin);
HAL_GPIO_EXTI_IRQHandler(BUTTON_MODE_3_Pin);
}
//////////// EXTRENAL INTERRUPTS WITH BUTTONS/////////
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

  if (GPIO_Pin==ARD_D2_Pin){
  		mode=1;
  	}
  	if (GPIO_Pin==ARD_D3_Pin){
  			mode=2;
  		}
  	/*if (GPIO_Pin==b){
  			mode=3;
  		}*/
  	if (GPIO_Pin==BUTTON_MODE_4_Pin){
  			mode=3;
  		}
  	if (GPIO_Pin==BUTTON_MODE_3_Pin){
  			mode=4;
  		}
    switch (GPIO_Pin)
    {
      case (GPIO_PIN_1):
      {
        SPI_WIFI_ISR();
        break;
      }
      default:
      {
        break;
      }
    }
}
void connect_and_receive(char* ssid, uint8_t*remoteIP)
{
	Socket=-1;


  if(WIFI_Init() ==  WIFI_STATUS_OK)
    {
      //TERMOUT(" WIFI Module Initialized.\n");

      if( WIFI_Connect(ssid, PASSWORD, WIFI_ECN_WPA2_PSK) == WIFI_STATUS_OK)
      {
       // TERMOUT(" es-wifi module connected \n");
        if(WIFI_GetIP_Address(IP_Addr, sizeof(IP_Addr)) == WIFI_STATUS_OK)
        {
         /* TERMOUT(" es-wifi module got IP Address : %d.%d.%d.%d\n",
                 IP_Addr[0],
                 IP_Addr[1],
                 IP_Addr[2],
                 IP_Addr[3]);

          TERMOUT(" Trying to connect to Server: %d.%d.%d.%d:%d ...\n",
        		  remoteIP[0],
				  remoteIP[1],
				  remoteIP[2],
				  remoteIP[3],
  							 RemotePORT);*/

          while (Trials--)
          {
            if( WIFI_OpenClientConnection(0, WIFI_TCP_PROTOCOL, "TCP_CLIENT", remoteIP, RemotePORT, 0) == WIFI_STATUS_OK)
            {
              TERMOUT("> TCP Connection opened successfully to %s\n",ssid);
              Socket = 0;
              break;
            }
          }
          if(Socket == -1)
          {
            TERMOUT("> ERROR : Cannot open Connection\n");
           // BSP_LED_On(LED2);
          }
        }
        else
        {
          //TERMOUT("> ERROR : es-wifi module CANNOT get IP address\n");
          //BSP_LED_On(LED2);
        }
      }
      else
      {
        TERMOUT("> ERROR : esp32  module NOT connected\n");
        //BSP_LED_On(LED2);
      }
    }
    else
    {
      TERMOUT("> ERROR : WIFI Module cannot be initialized.\n");
     // BSP_LED_On(LED2);
    }

  if(Socket != -1)
 	   { memset(Rxdata, 0, sizeof(Rxdata));
	 // HAL_UART_Receive(&hDiscoUart, Rxdata, 2, HAL_MAX_DELAY);
	  strcpy(Rxdata,ssid);

 	  if((strcmp(((char*)Rxdata),"ESP32_AP2")==0)||(strcmp(((char*)Rxdata),"ESP32_AP1")==0)){
 		  //if(strcmp(((char*)Rxdata),"OK")==0){

 		  BSP_LED_On(LED2);
	  uint32_t startTick = HAL_GetTick();
 	   while ((HAL_GetTick() - startTick) < DATA_RECEIVE_DURATION){
 	      ret = WIFI_ReceiveData(Socket, RxData, sizeof(RxData)-1, &Datalen, WIFI_READ_TIMEOUT);
 	      if(ret == WIFI_STATUS_OK)
 	      {
 	        if(Datalen > 0)
 	        {

 	          RxData[Datalen]=0;
 	          TERMOUT(RxData);}



 	      }




 	   }
             }
 	   else {
 		   TERMOUT("not chosen equipment \n");
             }}

  //}
 	      else
 	      {
 	        TERMOUT("> ERROR : Failed to Receive Data, connection closed\n");
 	        //break;
 	      }

 	     WIFI_CloseClientConnection(0);
 	     //TERMOUT("> Closed connection  \n");

 	      WIFI_Disconnect();
 	      TERMOUT("> Disconnected \n");

 	    BSP_LED_Off(LED2);

 	    }
//}



void connect_and_receive_EQP1(char* ssid, uint8_t*remoteIP)
{
	Socket=-1;


  if(WIFI_Init() ==  WIFI_STATUS_OK)
    {
      //TERMOUT(" WIFI Module Initialized.\n");

      if( WIFI_Connect(ssid, PASSWORD, WIFI_ECN_WPA2_PSK) == WIFI_STATUS_OK)
      {
       // TERMOUT(" es-wifi module connected \n");
        if(WIFI_GetIP_Address(IP_Addr, sizeof(IP_Addr)) == WIFI_STATUS_OK)
        {
         /* TERMOUT(" es-wifi module got IP Address : %d.%d.%d.%d\n",
                 IP_Addr[0],
                 IP_Addr[1],
                 IP_Addr[2],
                 IP_Addr[3]);

          TERMOUT(" Trying to connect to Server: %d.%d.%d.%d:%d ...\n",
        		  remoteIP[0],
				  remoteIP[1],
				  remoteIP[2],
				  remoteIP[3],
  							 RemotePORT);*/

          while (Trials--)
          {
            if( WIFI_OpenClientConnection(0, WIFI_TCP_PROTOCOL, "TCP_CLIENT", remoteIP, RemotePORT, 0) == WIFI_STATUS_OK)
            {
              TERMOUT("> TCP Connection opened successfully to %s\n",ssid);
              Socket = 0;
              break;
            }
          }
          if(Socket == -1)
          {
            //TERMOUT("> ERROR : Cannot open Connection\n");
           // BSP_LED_On(LED2);
          }
        }
        else
        {
          //TERMOUT("> ERROR : es-wifi module CANNOT get IP address\n");
          //BSP_LED_On(LED2);
        }
      }
      else
      {
        TERMOUT("> ERROR : es-wifi module NOT connected\n");
        //BSP_LED_On(LED2);
      }
    }
    else
    {
      TERMOUT("> ERROR : WIFI Module cannot be initialized.\n");
     // BSP_LED_On(LED2);
    }


  if(Socket != -1)
 	   { memset(Rxdata, 0, sizeof(Rxdata));
	  //HAL_UART_Receive(&hDiscoUart, Rxdata, 2, HAL_MAX_DELAY);
	  strcpy(Rxdata,ssid);
 	   //if(strcmp(((char*)Rxdata),"OK")==0){
	  if((strcmp(((char*)Rxdata),"ESP32_AP2")==0)||(strcmp(((char*)Rxdata),"ESP32_AP1")==0)){
 		  BSP_LED_On(LED2);
	  uint32_t startTick = HAL_GetTick();
 	   while ((HAL_GetTick() - startTick) < DATA_RECEIVE_DURATION){
 	      ret = WIFI_ReceiveData(Socket, RxData, sizeof(RxData)-1, &Datalen, WIFI_READ_TIMEOUT);
 	      if(ret == WIFI_STATUS_OK)
 	      {
 	        if(Datalen > 0)
 	        {

 	          RxData[Datalen]=0;
 	          TERMOUT(RxData);}



 	      }


 	   }
	  }
 	   else
 	   {
 		   BSP_LED_On(LED2);
 	  BSP_LED_Off(LED2);
 	  BSP_LED_On(LED2);
 		   TERMOUT("not chosen equipment \n");
             }}

  //}
 	      else
 	      {
 	        TERMOUT("> ERROR : Failed to Receive Data, connection closed\n");
 	        //break;
 	      }

 	     WIFI_CloseClientConnection(0);
 	     //TERMOUT("> Closed connection  \n");

 	      WIFI_Disconnect();
 	      TERMOUT("> Disconnected \n");

 	    //BSP_LED_Off(LED2);

 	    }
void connect_and_receive_EQP1_EQP2(char* ssid, uint8_t*remoteIP)
{ Socket=-1;
  if(WIFI_Init() ==  WIFI_STATUS_OK)
    {
      //TERMOUT(" WIFI Module Initialized.\n");

      if( WIFI_Connect(ssid, PASSWORD, WIFI_ECN_WPA2_PSK) == WIFI_STATUS_OK)
      {
       // TERMOUT(" es-wifi module connected \n");
        if(WIFI_GetIP_Address(IP_Addr, sizeof(IP_Addr)) == WIFI_STATUS_OK)
        {
         /* TERMOUT(" es-wifi module got IP Address : %d.%d.%d.%d\n",
                 IP_Addr[0],
                 IP_Addr[1],
                 IP_Addr[2],
                 IP_Addr[3]);

          TERMOUT(" Trying to connect to Server: %d.%d.%d.%d:%d ...\n",
        		  remoteIP[0],
				  remoteIP[1],
				  remoteIP[2],
				  remoteIP[3],
  							 RemotePORT);*/

          while (Trials--)
          {
            if( WIFI_OpenClientConnection(0, WIFI_TCP_PROTOCOL, "TCP_CLIENT", remoteIP, RemotePORT, 0) == WIFI_STATUS_OK)
            {
              TERMOUT("> TCP Connection opened successfully to %s\n",ssid);
              Socket = 0;
              break;
            }
          }
          if(Socket == -1)
          {
            //TERMOUT("> ERROR : Cannot open Connection\n");
           // BSP_LED_On(LED2);
          }
        }
        else
        {
          //TERMOUT("> ERROR : es-wifi module CANNOT get IP address\n");
          //BSP_LED_On(LED2);
        }
      }
      else
      {
        TERMOUT("> ERROR : es-wifi module NOT connected\n");
        //BSP_LED_On(LED2);
      }
    }
    else
    {
      TERMOUT("> ERROR : WIFI Module cannot be initialized.\n");
     // BSP_LED_On(LED2);
    }

 // uint32_t startTick = HAL_GetTick();
    //while ((HAL_GetTick() - startTick) < DATA_RECEIVE_DURATION){
  if(Socket != -1)
 	   { memset(Rxdata, 0, sizeof(Rxdata));
	  HAL_UART_Receive(&hDiscoUart, Rxdata, 1, HAL_MAX_DELAY);
	  //strcpy(Rxdata,ssid);
 	   //if(strcmp(((char*)Rxdata),"OK")==0){
	  if((strcmp(((char*)Rxdata),"ESP32_AP2")==0)||(strcmp(((char*)Rxdata),"ESP32_AP1")==0)){
 		  BSP_LED_On(LED2);
	  uint32_t startTick = HAL_GetTick();
 	   while ((HAL_GetTick() - startTick) < DATA_RECEIVE_DURATION){
 	      ret = WIFI_ReceiveData(Socket, RxData, sizeof(RxData)-1, &Datalen, WIFI_READ_TIMEOUT);
 	      if(ret == WIFI_STATUS_OK)
 	      {
 	        if(Datalen > 0)
 	        {

 	          RxData[Datalen]=0;
 	          TERMOUT(RxData);}



 	      }
//////////////////current calculation with stm32/////////////////

 	    /* if (strcmp((char*)Rxdata, "ESP32_AP2") == 0) {
 	     	         sConfig.Channel = ADC_CHANNEL_1; // IN1 (PC0)
 	     	         sConfig.Rank = ADC_REGULAR_RANK_1;
 	     	         sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
 	     	         sConfig.SingleDiff = ADC_SINGLE_ENDED;
 	     	         sConfig.OffsetNumber = ADC_OFFSET_NONE;
 	     	         sConfig.Offset = 0;

 	     	         HAL_ADC_ConfigChannel(&hadc1, &sConfig);

 	     	         HAL_ADC_Start(&hadc1);
 	     	         HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
 	     	         int adc_val = HAL_ADC_GetValue(&hadc1);
 	     	         HAL_ADC_Stop(&hadc1);

 	     	         float voltage = (adc_val * 3.3f) / 4095.0f;
 	     	         float current = (voltage - offset) /sensetivity;
 	     	         if (current < 0) current = 0;

 	     	         sprintf(buffer, "%.2f", current);
 	     	         TERMOUT("CURRENT AP2: %s\n", buffer);
 	     	     }else if (strcmp((char*)Rxdata, "ESP32_AP1") == 0) {
 	     	        sConfig.Channel = ADC_CHANNEL_2; // IN2 (PC1)
 	     	        sConfig.Rank = ADC_REGULAR_RANK_1;
 	     	        sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
 	     	        sConfig.SingleDiff = ADC_SINGLE_ENDED;
 	     	        sConfig.OffsetNumber = ADC_OFFSET_NONE;
 	     	        sConfig.Offset = 0;

 	     	        HAL_ADC_ConfigChannel(&hadc1, &sConfig);

 	     	        HAL_ADC_Start(&hadc1);
 	     	        HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
 	     	        int adc_val = HAL_ADC_GetValue(&hadc1);
 	     	        HAL_ADC_Stop(&hadc1);

 	     	        float voltage = (adc_val * 3.3f) / 4095.0f;
 	     	        float current = (voltage - offset) /sensetivity;
 	     	        if (current < 0) current = 0;

 	     	        sprintf(buffer, "%.2f", current);
 	     	        TERMOUT("CURRENT AP1: %s\n", buffer);
 	     	    }*/
}
	  }
 	   else
 	   {
 		   BSP_LED_On(LED2);
 	  BSP_LED_Off(LED2);
 	  BSP_LED_On(LED2);
 		   TERMOUT("not chosen equipment \n");
             }}

  //}
 	      else
 	      {
 	        TERMOUT("> ERROR : Failed to Receive Data, connection closed\n");
 	        //break;
 	      }

 	     WIFI_CloseClientConnection(0);
 	     //TERMOUT("> Closed connection  \n");

 	      WIFI_Disconnect();
 	      TERMOUT("> Disconnected \n");

 	    //BSP_LED_Off(LED2);

 	    }
void connect_and_send(char* ssid, uint8_t*remoteIP)
{
	Socket=-1;


  if(WIFI_Init() ==  WIFI_STATUS_OK)
    {
      //TERMOUT(" WIFI Module Initialized.\n");

      if( WIFI_Connect(ssid, PASSWORD, WIFI_ECN_WPA2_PSK) == WIFI_STATUS_OK)
      {
       // TERMOUT(" es-wifi module connected \n");
        if(WIFI_GetIP_Address(IP_Addr, sizeof(IP_Addr)) == WIFI_STATUS_OK)
        {
         /* TERMOUT(" es-wifi module got IP Address : %d.%d.%d.%d\n",
                 IP_Addr[0],
                 IP_Addr[1],
                 IP_Addr[2],
                 IP_Addr[3]);

          TERMOUT(" Trying to connect to Server: %d.%d.%d.%d:%d ...\n",
        		  remoteIP[0],
				  remoteIP[1],
				  remoteIP[2],
				  remoteIP[3],
  							 RemotePORT);*/

          while (Trials--)
          {
            if( WIFI_OpenClientConnection(0, WIFI_TCP_PROTOCOL, "TCP_CLIENT", remoteIP, RemotePORT, 0) == WIFI_STATUS_OK)
            {
              TERMOUT("> TCP Connection opened successfully to %s\n",ssid);
              Socket = 0;
              break;
            }
          }
          if(Socket == -1)
          {
            //TERMOUT("> ERROR : Cannot open Connection\n");
           // BSP_LED_On(LED2);
          }
        }
        else
        {
          TERMOUT("> ERROR : es-wifi module CANNOT get IP address\n");
          //BSP_LED_On(LED2);
        }
      }
      else
      {
        //TERMOUT("> ERROR : es-wifi module NOT connected\n");
        //BSP_LED_On(LED2);
      }
    }
    else
    {
      TERMOUT("> ERROR : WIFI Module cannot be initialized.\n");
     // BSP_LED_On(LED2);
    }


  if(Socket != -1)
 	   {   	HAL_UART_Receive(&hDiscoUart, &byte, 1, 1000) ;

 	   //byte=0b00010001;


 	  switch(byte){

 	  case 0b000000000:
	  		  strcpy(TxData,"CELL0_OF");
	  		  WIFI_SendData(Socket, TxData, strlen(TxData), &Datalen, WIFI_READ_TIMEOUT);
	  		       TERMOUT ("CELL0_OF")  ;
	  		       HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 1);
	  		       break;
	  	  case 0b00000001:
	  	  		  strcpy(TxData,"CELL1_OF");
	  	  		  WIFI_SendData(Socket, TxData, strlen(TxData), &Datalen, WIFI_READ_TIMEOUT);
	  	  		       TERMOUT ("CELL1_OF")  ;
	  	  		       break;
	  	  case 0b00000010:
	  	  		  strcpy(TxData,"CELL1_OF");
	  	  		  WIFI_SendData(Socket, TxData, strlen(TxData), &Datalen, WIFI_READ_TIMEOUT);
	  	  		       TERMOUT ("CELL1_OF")  ;
	  	  		       break;
	  	  case 0b00000011:
	  	  		  strcpy(TxData,"CELL2_OF");
	  	  		  WIFI_SendData(Socket, TxData, strlen(TxData), &Datalen, WIFI_READ_TIMEOUT);
	  	  		       TERMOUT ("CELL2_OF")  ;
	  	  		       break;
	  	  case 0b00010000:
	  	  		  strcpy(TxData,"CELL0_ON");
	  	  		  WIFI_SendData(Socket, TxData, strlen(TxData), &Datalen, WIFI_READ_TIMEOUT);
	  	  		       TERMOUT ("CELL0_ON")  ;
	  	  		       break;
	  	  case 0b00010001:
	  	  		  strcpy(TxData,"CELL1_ON");
	  	  		  WIFI_SendData(Socket, TxData, strlen(TxData), &Datalen, WIFI_READ_TIMEOUT);
	  	  		       TERMOUT ("CELL1_ON")  ;
	  	  		       break;
	  	  case 0b00010010:
	  	 	  		  strcpy(TxData,"CELL2_ON");
	  	 	  		  WIFI_SendData(Socket, TxData, strlen(TxData), &Datalen, WIFI_READ_TIMEOUT);
	  	 	  		       TERMOUT ("CELL2_ON")  ;
	  	 	  		       break;
	  	  case 0b00010011:
	  	 	  		  strcpy(TxData,"CELL3_ON");
	  	 	  		  WIFI_SendData(Socket, TxData, strlen(TxData), &Datalen, WIFI_READ_TIMEOUT);
	  	 	  		       TERMOUT ("CELL3_ON")  ;
	  	 	  		       break;
	  	  case 0b00001000:
	  	 	  		  strcpy(TxData,"LOAD_OFF");
	  	 	  		  WIFI_SendData(Socket, TxData, strlen(TxData), &Datalen, WIFI_READ_TIMEOUT);
	  	 	  		       TERMOUT ("LOAD_OFF")  ;
	  	 	  		       break;
	  	  case 0b00011000:
	  	 	  		  strcpy(TxData,"LOAD_ON");
	  	 	  		  WIFI_SendData(Socket, TxData, strlen(TxData), &Datalen, WIFI_READ_TIMEOUT);
	  	 	  		       TERMOUT ("LOAD_ON")  ;
	  	 	  		       break;
	  	  case 0b00011001:
	  	 	  		  strcpy(TxData,"CHARGER_ON");
	  	 	  		  WIFI_SendData(Socket, TxData, strlen(TxData), &Datalen, WIFI_READ_TIMEOUT);
	  	 	  		       TERMOUT ("CHARGER_ON")  ;
	  	 	  		       break;
	  	  case 0b00001001:
	  	 	  		  strcpy(TxData,"CHARGER_OFF");
	  	 	  		  WIFI_SendData(Socket, TxData, strlen(TxData), &Datalen, WIFI_READ_TIMEOUT);
	  	 	  		       TERMOUT ("CHARGER_OFF")  ;
 	 	  		       break;

 	  default:
 		  break;}

 	   }else {
 		  TERMOUT("> ERROR : Failed to send Data, connection closed\n");}
 	     WIFI_CloseClientConnection(0);
 	     //TERMOUT("> Closed connection  \n");

 	      WIFI_Disconnect();
 	      TERMOUT("> Disconnected \n");

 	    //BSP_LED_Off(LED2);

 	    }
  void chek_by_EQP(char* ssid, uint8_t*remoteIP)
  {
	  Socket=-1;


    if(WIFI_Init() ==  WIFI_STATUS_OK)
      {
        //TERMOUT(" WIFI Module Initialized.\n");

        if( WIFI_Connect(ssid, PASSWORD, WIFI_ECN_WPA2_PSK) == WIFI_STATUS_OK)
        {
         // TERMOUT(" es-wifi module connected \n");
          if(WIFI_GetIP_Address(IP_Addr, sizeof(IP_Addr)) == WIFI_STATUS_OK)
          {
           /* TERMOUT(" es-wifi module got IP Address : %d.%d.%d.%d\n",
                   IP_Addr[0],
                   IP_Addr[1],
                   IP_Addr[2],
                   IP_Addr[3]);

            TERMOUT(" Trying to connect to Server: %d.%d.%d.%d:%d ...\n",
          		  remoteIP[0],
  				  remoteIP[1],
  				  remoteIP[2],
  				  remoteIP[3],
    							 RemotePORT);*/

            while (Trials--)
            {
              if( WIFI_OpenClientConnection(0, WIFI_TCP_PROTOCOL, "TCP_CLIENT", remoteIP, RemotePORT, 0) == WIFI_STATUS_OK)
              {
                TERMOUT("> TCP Connection opened successfully to %s\n",ssid);
                Socket = 0;
                break;
              }
            }
            if(Socket == -1)
            {
              //TERMOUT("> ERROR : Cannot open Connection\n");
             // BSP_LED_On(LED2);
            }
          }
          else
          {
            //TERMOUT("> ERROR : es-wifi module CANNOT get IP address\n");
            //BSP_LED_On(LED2);
          }
        }
        else
        {
          TERMOUT("> ERROR : es-wifi module NOT connected\n");
          //BSP_LED_On(LED2);
        }
      }
      else
      {
        TERMOUT("> ERROR : WIFI Module cannot be initialized.\n");
       // BSP_LED_On(LED2);
      }


    if(Socket != -1)
   	   { memset(Rxdata, 0, sizeof(Rxdata));
  	  //HAL_UART_Receive(&hDiscoUart, Rxdata, 2, HAL_MAX_DELAY);
  	  strcpy(Rxdata,ssid);
  	 	 if((strcmp(((char*)Rxdata),"ESP32_AP2")==0)||(strcmp(((char*)Rxdata),"ESP32_AP1")==0)){
   	   //if(Rxdata==0b11111){
   		  BSP_LED_On(LED2);
  	  uint32_t startTick = HAL_GetTick();
   	   while ((HAL_GetTick() - startTick) < DATA_RECEIVE_DURATION){
   	      ret = WIFI_ReceiveData(Socket, RxData, sizeof(RxData)-1, &Datalen, WIFI_READ_TIMEOUT);
   	      if(ret == WIFI_STATUS_OK)
   	      {
   	        if(Datalen > 0)
   	        {

   	          RxData[Datalen]=0;
   	          TERMOUT(RxData);}


   	          //ret = WIFI_SendData(Socket, TxData, sizeof(TxData), &Datalen, WIFI_WRITE_TIMEOUT);
   	          /*if (ret != WIFI_STATUS_OK)
   	          {
   	            TERMOUT("> ERROR : Failed to Send Data, connection closed\n");
   	            break;
   	          }*/
   	      }
   	   /*if (strcmp((char*)Rxdata, "ESP32_AP2") == 0) {
   	   	         sConfig.Channel = ADC_CHANNEL_1;
   	   	         sConfig.Rank = ADC_REGULAR_RANK_1;
   	   	         sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
   	   	         sConfig.SingleDiff = ADC_SINGLE_ENDED;
   	   	         sConfig.OffsetNumber = ADC_OFFSET_NONE;
   	   	         sConfig.Offset = 0;

   	   	         HAL_ADC_ConfigChannel(&hadc1, &sConfig);

   	   	         HAL_ADC_Start(&hadc1);
   	   	         HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
   	   	         int adc_val = HAL_ADC_GetValue(&hadc1);
   	   	         HAL_ADC_Stop(&hadc1);

   	   	         float voltage = (adc_val * 3.3f) / 4095.0f;
   	   	         float current = (voltage - offset) / sensetivity;
   	   	         if (current < 0) current = 0;

   	   	         sprintf(buffer, "%.2f", current);
   	   	         TERMOUT("CURRENT AP2: %s\n", buffer);
   	   	     }else if (strcmp((char*)Rxdata, "ESP32_AP1") == 0) {
   	   	        sConfig.Channel = ADC_CHANNEL_2;
   	   	        sConfig.Rank = ADC_REGULAR_RANK_1;
   	   	        sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
   	   	        sConfig.SingleDiff = ADC_SINGLE_ENDED;
   	   	        sConfig.OffsetNumber = ADC_OFFSET_NONE;
   	   	        sConfig.Offset = 0;

   	   	        HAL_ADC_ConfigChannel(&hadc1, &sConfig);

   	   	        HAL_ADC_Start(&hadc1);
   	   	        HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
   	   	        int adc_val = HAL_ADC_GetValue(&hadc1);
   	   	        HAL_ADC_Stop(&hadc1);

   	   	        float voltage = (adc_val * 3.3f) / 4095.0f;
   	   	        float current = (voltage - offset) / sensetivity;
   	   	        if (current < 0) current = 0;

   	   	        sprintf(buffer, "%.2f", current);
   	   	        TERMOUT("CURRENT AP1: %s\n", buffer);
   	   	    }*/


   	   }
               }
   	   else {
   		   TERMOUT("not chosen equipment \n");
               }}

    //}
   	      else
   	      {
   	        TERMOUT("> ERROR : Failed to Receive Data, connection closed\n");
   	        //break;
   	      }
    HAL_Delay(1000);
    if(Socket != -1)
    	   {

	  	  HAL_UART_Receive(&hDiscoUart, &byte, 1,1000 );

	   	  // byte=0b00011000;
	  	  switch(byte){
	  	  case 0b000000000:
	  		  strcpy(TxData,"CELL0_OF");
	  		  WIFI_SendData(Socket, TxData, strlen(TxData), &Datalen, WIFI_READ_TIMEOUT);
	  		       TERMOUT ("CELL0_OF")  ;
	  		       HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 1);
	  		       break;
	  	  case 0b00000001:
	  	  		  strcpy(TxData,"CELL1_OF");
	  	  		  WIFI_SendData(Socket, TxData, strlen(TxData), &Datalen, WIFI_READ_TIMEOUT);
	  	  		       TERMOUT ("CELL1_OF")  ;
	  	  		       break;
	  	  case 0b00000010:
	  	  		  strcpy(TxData,"CELL1_OF");
	  	  		  WIFI_SendData(Socket, TxData, strlen(TxData), &Datalen, WIFI_READ_TIMEOUT);
	  	  		       TERMOUT ("CELL1_OF")  ;
	  	  		       break;
	  	  case 0b00000011:
	  	  		  strcpy(TxData,"CELL2_OF");
	  	  		  WIFI_SendData(Socket, TxData, strlen(TxData), &Datalen, WIFI_READ_TIMEOUT);
	  	  		       TERMOUT ("CELL2_OF")  ;
	  	  		       break;
	  	  case 0b00010000:
	  	  		  strcpy(TxData,"CELL0_ON");
	  	  		  WIFI_SendData(Socket, TxData, strlen(TxData), &Datalen, WIFI_READ_TIMEOUT);
	  	  		       TERMOUT ("CELL0_ON")  ;
	  	  		       break;
	  	  case 0b00010001:
	  	  		  strcpy(TxData,"CELL1_ON");
	  	  		  WIFI_SendData(Socket, TxData, strlen(TxData), &Datalen, WIFI_READ_TIMEOUT);
	  	  		       TERMOUT ("CELL1_ON")  ;
	  	  		       break;
	  	  case 0b00010010:
	  	 	  		  strcpy(TxData,"CELL2_ON");
	  	 	  		  WIFI_SendData(Socket, TxData, strlen(TxData), &Datalen, WIFI_READ_TIMEOUT);
	  	 	  		       TERMOUT ("CELL2_ON")  ;
	  	 	  		       break;
	  	  case 0b00010011:
	  	 	  		  strcpy(TxData,"CELL3_ON");
	  	 	  		  WIFI_SendData(Socket, TxData, strlen(TxData), &Datalen, WIFI_READ_TIMEOUT);
	  	 	  		       TERMOUT ("CELL3_ON")  ;
	  	 	  		       break;
	  	  case 0b00001000:
	  	 	  		  strcpy(TxData,"LOAD_OFF");
	  	 	  		  WIFI_SendData(Socket, TxData, strlen(TxData), &Datalen, WIFI_READ_TIMEOUT);
	  	 	  		       TERMOUT ("LOAD_OFF")  ;
	  	 	  		       break;
	  	  case 0b00011000:
	  	 	  		  strcpy(TxData,"LOAD_ON");
	  	 	  		  WIFI_SendData(Socket, TxData, strlen(TxData), &Datalen, WIFI_READ_TIMEOUT);
	  	 	  		       TERMOUT ("LOAD_ON")  ;
	  	 	  		       break;
	  	  case 0b00011001:
	  	 	  		  strcpy(TxData,"CHARGER_ON");
	  	 	  		  WIFI_SendData(Socket, TxData, strlen(TxData), &Datalen, WIFI_READ_TIMEOUT);
	  	 	  		       TERMOUT ("CHARGER_ON")  ;
	  	 	  		       break;
	  	  case 0b00001001:
	  	 	  		  strcpy(TxData,"CHARGER_OFF");
	  	 	  		  WIFI_SendData(Socket, TxData, strlen(TxData), &Datalen, WIFI_READ_TIMEOUT);
	  	 	  		       TERMOUT ("CHARGER_FF")  ;
	  	 	  		       break;

	  	  default:
	  		  break;}}
	      else
	      {
	        TERMOUT("> ERROR : Failed to send Data, connection closed\n");
	        //break;
	      }



    	     WIFI_CloseClientConnection(0);
    	     //TERMOUT("> Closed connection  \n");

    	      WIFI_Disconnect();
    	      TERMOUT("> Disconnected \n");




  }


void Error_Handler(void)
{
  // USER CODE BEGIN Error_Handler_Debug
  // User can add his own implementation to report the HAL error return state
  __disable_irq();
  while (1)
  {
  }
  // USER CODE END Error_Handler_Debug
}
void SPI3_IRQHandler(void)
{
  HAL_SPI_IRQHandler(&hspi);
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */


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
    ex: TERMOUT("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

