/*
* Adam Luczak
* Laboratory stand with Cortex-M0 Microcontroller and E-Paper display
* Master Thesis 2018
* High-level version using STM Hardware Abstract Layer
*/

/* Standard C and STM32 libraries include section */
#include "main.h"
#include "stm32l0xx_hal.h"
#include "stdio.h"
#include "stdlib.h"

/* Labstand peripherals include section */
#include "epaper.h"
#include "led.h"
#include "uart_bluetooth.h"
#include "pushbutton.h"
#include "analog.h"

/* UART handle */
extern UART_HandleTypeDef huart;

/* Function prototypes */
void SystemClock_Config(void);
void Labstand_Parse_Command(char*);
void Labstand_LED_Command(char*);
void Labstand_EPD_Command(char*);
void Labstand_ADC_Command(char*);
void Labstand_DAC_Command(char*);

/* Global variables */
char rxByte[2];
char rxData[256];
uint8_t uartIntFlag = 0;
uint8_t pbIntFlag = 0;
uint8_t adcOnLED = 0;
uint8_t i = 0;
extern uint16_t adcValue;



/* Main function */
int main(void)
{

  HAL_Init();
  SystemClock_Config();

  Labstand_Pushbutton_Init();
  Labstand_ADC_Init();
  Labstand_DAC_Init();
  Labstand_LED_Init();

  BSP_EPD_Init();
  BSP_EPD_Write_Text((char*)WELCOME_TEXT);

  Labstand_UART_Init();
  HAL_UART_Transmit(&huart,(unsigned char*)INIT_MSG,strlen(INIT_MSG),TIMEOUT);
  HAL_UART_Receive_IT(&huart,(unsigned char*)rxByte, 1);


  while (1)
  {

		if (uartIntFlag) {

			if (rxByte[0] == '\r') {

				Labstand_Parse_Command(rxData);
				i = 0;
				memset(rxData, '\0', strlen(rxData));
				memset(rxByte, '\0', strlen(rxByte));

			} else {

				if (rxByte[0] != '\r' && rxByte[0] != '\n') {
					rxData[i++] = rxByte[0];
					memset(rxByte, '\0', sizeof(rxByte));
				}

			}
			uartIntFlag = 0;
			HAL_UART_AbortReceive_IT(&huart);
			HAL_UART_Receive_IT(&huart, (unsigned char*)rxByte, 1);
		}

		if (pbIntFlag){

			char msg[32];
			sprintf(msg,"Interrupt from button B%i.\r\n",pbIntFlag);
			HAL_UART_Transmit(&huart,(unsigned char*)msg,strlen(msg),TIMEOUT);
			pbIntFlag = 0;
		}

		if (adcOnLED){

			Labstand_LED_Write( 0b11111111 >> (8-((adcValue+1)/512)));

		}
  }
}



/* Function parsing commands sent by Bluetooth */
void Labstand_Parse_Command(char* command){

	if (!strncmp(command, "LED:" , 4)){
		Labstand_LED_Command(command+4);
	}
	else if (!strncmp(command, "EPD:" , 4)){
		Labstand_EPD_Command(command+4);
	}
	else if (!strncmp(command, "ADC:" , 4)){
		Labstand_ADC_Command(command+4);
	}
	else if  (!strncmp(command, "DAC:" , 4)){
		Labstand_DAC_Command(command+4);
	}
	else if (!strcmp(command,"RESET")){
		SEND_COMMAND_RECEIVED
		NVIC_SystemReset();
	}
	else{
		SEND_COMMAND_INVALID
	}

}

/* Function executed after LED command is sent by Bluetooth */
void Labstand_LED_Command(char* value){

	uint8_t i = 0;
	uint8_t LED_values = 0;

	if (!strcmp(value,"CLEAR")){
		DO_NOTHING
	}
	else if (strlen(value)==8)
	{
		for(; i < 8; i++){

			if (value[i] != '1' && value[i] != '0') {

				SEND_COMMAND_INVALID
				return;

			} else if (value[i] == '1')
				LED_values += 1 << (7 - i);

		}
	}
	else{
		SEND_COMMAND_INVALID
		return;
	}

	SEND_COMMAND_RECEIVED
	adcOnLED = 0;
	Labstand_LED_Write(LED_values);

}

/* Function executed after E-Paper command is sent by Bluetooth */
void Labstand_EPD_Command(char* value){

	if(strlen(value) > 0){

		SEND_COMMAND_RECEIVED

		if (!strcmp(value,"CLEAR")){
			BSP_EPD_Clear(EPD_COLOR_WHITE);
			BSP_EPD_RefreshDisplay();
		}
		else BSP_EPD_Write_Text(value);

	}
	else SEND_COMMAND_INVALID

}

/* Function executed after analog-to-digital converter command is sent by Bluetooth */
void Labstand_ADC_Command(char* value){

	char msg[20];

	if (!strcmp(value,"READ")){

		sprintf(msg, "ADC Voltage: %.3fV\r\n", Labstand_ADC_GetVoltage());
		HAL_UART_Transmit(&huart, (uint8_t*)msg,strlen(msg), TIMEOUT);

	}
	else if (!strcmp(value,"LED")){

		SEND_COMMAND_RECEIVED
		adcOnLED = 1;

	}
	else SEND_COMMAND_INVALID
}

/* Function executed after digital-to-analog converter command is sent by Bluetooth */
void Labstand_DAC_Command(char* value){

	if (!strcmp(value,"START")){

		SEND_COMMAND_RECEIVED
		Labstand_DAC_GenerateSignal();

	}
	else if (!strcmp(value,"CLEAR")){

		SEND_COMMAND_RECEIVED
		Labstand_DAC_Clear();

	}
	else {

		float dacVoltage = 0;
		sscanf(value,"%f",&dacVoltage);

		if (dacVoltage >= 0.0 && dacVoltage <= 3.3){

			SEND_COMMAND_RECEIVED
			Labstand_DAC_SetVoltage(dacVoltage);

		}
		else SEND_COMMAND_INVALID
	}
}

/* Pushbutton interrupt handler */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){

	switch(GPIO_Pin){
		case B1_PIN:
			pbIntFlag = 1;
			break;
		case B2_PIN:
			pbIntFlag = 2;
			break;
		case B3_PIN:
			pbIntFlag = 3;
			break;
		default:
			return;
	}


}

/* UART receive complete interrupt handler */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){

	if (huart->Instance == USART1)
		uartIntFlag = 1;
}

/* MCU clock configuration */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLLMUL_3;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLLDIV_3;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);

}

