/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "ring_buffer.h"

#include "keyboard.h"

#include "stdio.h"

#include "ssd1306.h"

#include "ssd1306_fonts.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint8_t rx_buffer[5];
ring_buffer_t ring_buffer_password;

uint8_t rx_data;

uint16_t key_event = 0xFF;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

int _write(int file, char *ptr, int len)
{
	HAL_UART_Transmit(&huart2, (uint8_t *)ptr, len, HAL_MAX_DELAY); //el asterisco del uint8 es para evitar warnings por diferencia de tipos de dato
	return len;
}

void              HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){

	key_event = GPIO_Pin ;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  ring_buffer_init(&ring_buffer_password, rx_buffer, 5);//initialize the ring buffer.

  HAL_UART_Receive_IT(&huart2, &rx_data, 1);//when data is received it goes to the ring buffer.


  /* USER CODE END 2 */

  /* Infinite loop */
  keyboard_init();

  ssd1306_Init();
  ssd1306_Fill(Magenta);
  ssd1306_SetCursor(20, 25);
  ssd1306_WriteString("Bienvenido.", Font_7x10, Black);
  ssd1306_DrawRectangle(8, 38, 113, 47, Black);
  for(int s = 10;s <= 110; s+=20){
		ssd1306_FillRectangle(10, 40, s, 45, Black);
		 ssd1306_UpdateScreen();
		 HAL_Delay(300);
 	}
    ssd1306_Init();
  	ssd1306_Fill(Magenta);
  	ssd1306_SetCursor(0, 5);
  	ssd1306_WriteString("Digite su contrasena.", Font_6x8, Black);
  	ssd1306_SetCursor(0, 45);
  	ssd1306_WriteString(" A o B para reiniciar.", Font_6x8, Black);
  	ssd1306_SetCursor(0, 55);
  	ssd1306_WriteString(" * para confirmar.", Font_6x8, Black);
  	ssd1306_UpdateScreen();
  	 printf("Digite su contraseña. \r\n");

  int pass = 0;
  char *show;
  int space = 40;
  while (1)
  {

	 if (key_event != 0xFF) { // check if there is a event from the EXTi callback
		  uint16_t key_pressed = keypad_handler(key_event);
		  ring_buffer_put(&ring_buffer_password, key_pressed);
		  printf("Key pressed: %x\r\n", key_pressed);// print the key pressed

		  if(key_pressed == 0x01){
			  show = "1";
		  }
		  if(key_pressed == 0x02){
			  show = "2";
		  }
		  if(key_pressed == 0x03){
			  show = "3";
		  }
		  if(key_pressed == 0x04){
			  show = "4";
		  }
		  if(key_pressed == 0x05){
			  show = "5";
		  }
		  if(key_pressed == 0x06){
			  show = "6";
		  }
		  if(key_pressed == 0x07){
			  show = "7";
		  }
		  if(key_pressed == 0x08){
			  show = "8";
		  }
		  if(key_pressed == 0x09){
			  show = "9";
		  }
		  if(key_pressed == 0x0E){
			  show = "*";
		  }
		  if(key_pressed == 0x0A){
			  show = "A";
		  }
		  if(key_pressed == 0x0B){
			  show = "B";
		  }
		  if(key_pressed == 0x0C){
			  show = "C";
		  }
		  if(key_pressed == 0x0D){
			  show = "D";
		  }

		  ssd1306_SetCursor(space, 26);
		  ssd1306_WriteString(show, Font_11x18, Black);
		  ssd1306_UpdateScreen();
		  HAL_Delay(400);
		  ssd1306_SetCursor(space, 26);
		  ssd1306_WriteString("*", Font_11x18, Black);
		  ssd1306_UpdateScreen();
		  space = space + 10;

		  if (key_pressed == 0x0B || key_pressed == 0x0A){
		 		 printf("Borrando datos... \r\n");
		 		 printf("Ingrese la contraseña de nuevo. \r\n");

				pass=0;
				 ring_buffer_reset(&ring_buffer_password);
				 space = 40;

		 		 ssd1306_Init();
		 		 ssd1306_Fill(Magenta);
		 		 ssd1306_SetCursor(5, 25);
		 		 ssd1306_WriteString("Borrando datos...", Font_7x10, Black);
		 		 ssd1306_DrawRectangle(8, 38, 113, 47, Black);
		 		 for(int s = 10;s <= 110; s+=20){
		 			ssd1306_FillRectangle(10, 40, s, 45, Black);
					 ssd1306_UpdateScreen();
					 HAL_Delay(300);
		 		 }

		 		 ssd1306_Init();
				 ssd1306_Fill(Magenta);
				 ssd1306_SetCursor(0, 0);
				 ssd1306_WriteString("Ingrese los datos", Font_6x8, Black);
				 ssd1306_SetCursor(0, 10);
				 ssd1306_WriteString("nuevamente, por favor.", Font_6x8, Black);
				 ssd1306_UpdateScreen();
				 HAL_Delay(2000);
		 	 }


		  HAL_Delay(600);
		  key_event = 0xFF; // clean the event
	 }



	 if (ring_buffer_size(&ring_buffer_password) == 5){
		 for (int i = 0; i <= 3; i++){
				 pass = pass *10 + (rx_buffer[i]-0);
			 }
		  printf("Your password is:  %u\r\n", pass);
		  if ((rx_buffer[4] == 0x0E) && (pass == 1999)){
			  printf("Pass. \r\n");
			  ssd1306_Init();
			  ssd1306_Fill(Magenta);
			  ssd1306_SetCursor(35, 25);
			  ssd1306_DrawCircle( 67, 35, 28,Black);
			  ssd1306_DrawCircle( 67, 35, 29,Black);
			  ssd1306_DrawCircle( 67, 35, 30,Black);
			  ssd1306_DrawCircle( 67, 35, 31,Black);
			  ssd1306_DrawCircle( 67, 35, 32,Black);
			  ssd1306_WriteString("PASS.", Font_16x26, Black);
			  ssd1306_UpdateScreen();
			  HAL_Delay(2000);
		  }
		  else{
			  printf("Fail. \r\n");
			  ssd1306_Init();
			  ssd1306_Fill(Magenta);
			  ssd1306_SetCursor(35, 25);
			  ssd1306_WriteString("FAIL.", Font_16x26, Black);
			  ssd1306_UpdateScreen();
			  HAL_Delay(2000);
		  }
		  ring_buffer_reset(&ring_buffer_password);
		  pass = 0;
		  space = 40;
		  ssd1306_Init();
		ssd1306_Fill(Magenta);
		ssd1306_SetCursor(0, 5);
		ssd1306_WriteString(" Digite su contrasena.", Font_6x8, Black);
		ssd1306_SetCursor(0, 45);
		ssd1306_WriteString(" A o B para reiniciar.", Font_6x8, Black);
		ssd1306_SetCursor(0, 55);
		ssd1306_WriteString(" * para confirmar.", Font_6x8, Black);
		ssd1306_UpdateScreen();
		 printf("Digite su contraseña. \r\n");
	 }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  /* USER CODE END 3 */
}
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x10909CEC;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LD2_Pin|ROW_1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, ROW_2_Pin|ROW_4_Pin|ROW_3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD2_Pin ROW_1_Pin */
  GPIO_InitStruct.Pin = LD2_Pin|ROW_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : COLUMN_1_Pin */
  GPIO_InitStruct.Pin = COLUMN_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(COLUMN_1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : COLUMN_4_Pin */
  GPIO_InitStruct.Pin = COLUMN_4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(COLUMN_4_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : COLUMN_2_Pin COLUMN_3_Pin */
  GPIO_InitStruct.Pin = COLUMN_2_Pin|COLUMN_3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : ROW_2_Pin ROW_4_Pin ROW_3_Pin */
  GPIO_InitStruct.Pin = ROW_2_Pin|ROW_4_Pin|ROW_3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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
  /* USER CODE END Error_Handler_Debug */
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
