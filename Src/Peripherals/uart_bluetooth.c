/*
* Adam Luczak
* Laboratory stand with Cortex-M0 Microcontroller and E-Paper display
* Master Thesis 2018
* High-level version using STM Hardware Abstract Layer
*/

/* Include section */
#include <uart_bluetooth.h>

/* UART handle */
UART_HandleTypeDef huart;


/* MCU UART init */
void Labstand_UART_Init() {
	GPIO_InitTypeDef GPIO_InitStruct;

	__HAL_RCC_USART1_CLK_ENABLE();

	GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF0_USART1;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART1_IRQn);

	huart.Instance = USART1;
	huart.Init.BaudRate = BAUDRATE;
	huart.Init.WordLength = UART_WORDLENGTH_8B;
	huart.Init.StopBits = UART_STOPBITS_1;
	huart.Init.Parity = UART_PARITY_NONE;
	huart.Init.Mode = UART_MODE_TX_RX;
	huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart.Init.OverSampling = UART_OVERSAMPLING_16;
	huart.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

	HAL_UART_Init(&huart);
}


/* MCU UART deinit */
void Labstand_UART_DeInit(){

	HAL_UART_DeInit(&huart);
}

/* Bluetooth reset and start inquiring devices */
void HC05_Reset(){

	char command[20];

	sprintf(command,"AT+RESET\r\n");
	HAL_UART_Transmit(&huart, (uint8_t*)command, strlen(command), TIMEOUT);
	HAL_Delay(500);

	sprintf(command,"AT+INIT\r\n");
	HAL_UART_Transmit(&huart, (uint8_t*)command, strlen(command), TIMEOUT);
	HAL_Delay(500);

	sprintf(command,"AT+INQ\r\n");
	HAL_UART_Transmit(&huart, (uint8_t*)command, strlen(command), TIMEOUT);
	HAL_Delay(500);

}

/* Bluetooth name change */
void HC05_Name(unsigned char* name){

	char command[20];
	sprintf(command,"AT+NAME=%s",name);

	HAL_UART_Transmit(&huart, (uint8_t*)command, strlen(command), TIMEOUT);
	HAL_Delay(500);

}

/* Bluetooth UART parameters config */
void HC05_UART_Config(int baudrate, int parityBits, int stopBits){

	char command[20];
	sprintf(command,"AT+UART=%i,%i,%i",baudrate,parityBits,stopBits);

	HAL_UART_Transmit(&huart, (uint8_t*)command, strlen(command), TIMEOUT);
	HAL_Delay(500);

}
