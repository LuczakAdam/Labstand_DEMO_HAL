/*
* Adam Luczak
* Laboratory stand with Cortex-M0 Microcontroller and E-Paper display
* Master Thesis 2018
* High-level version using STM Hardware Abstract Layer
*/

/* Include section */
#include <led.h>

/* Variables */
GPIO_TypeDef* ledPort[8] = {LED1_PORT,LED2_PORT,LED3_PORT,LED4_PORT,LED5_PORT,LED6_PORT,LED7_PORT,LED8_PORT};
uint16_t ledPin[8] = {LED1_PIN,LED2_PIN,LED3_PIN,LED4_PIN,LED5_PIN,LED6_PIN,LED7_PIN,LED8_PIN};



/* LEDs controlling GPIO pins init */
void Labstand_LED_Init(void) {

	GPIO_InitTypeDef GPIO_InitStruct;
	uint8_t i = 0;

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();

	for (i = 0 ; i < 8; i++)	{
		HAL_GPIO_WritePin(ledPort[i],ledPin[i], GPIO_PIN_RESET);
		GPIO_InitStruct.Pin = ledPin[i];
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(ledPort[i], &GPIO_InitStruct);
	}

};


/* LEDs controlling GPIO pins deinit */
void Labstand_LED_DeInit(void) {

	uint8_t i = 0;

	for (i = 0 ; i < 8; i++)
		HAL_GPIO_DeInit(ledPort[i],ledPin[i]);

};



/* Write status on LEDs */
void Labstand_LED_Write(uint8_t status) {

	uint8_t i = 0;

	for (i = 0 ; i < 8; i++)
		HAL_GPIO_WritePin(ledPort[i],ledPin[i], (status >> i) & 1);
};


/* Clear all LEDs */
void Labstand_LED_Clear(void) {

	Labstand_LED_Write(0);

};
