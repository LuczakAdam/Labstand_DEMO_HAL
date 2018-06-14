/*
* Adam Luczak
* Laboratory stand with Cortex-M0 Microcontroller and E-Paper display
* Master Thesis 2018
* High-level version using STM Hardware Abstract Layer
*/

/* Include section */
#include "stm32l0xx_hal.h"

/* Macros */
#define BUTTON_GPIO_PORT GPIOB
#define B1_PIN GPIO_PIN_12
#define B2_PIN GPIO_PIN_13
#define B3_PIN GPIO_PIN_14

/* Function prototypes */
void Labstand_Pushbutton_Init(void);
void Labstand_Pushbutton_DeInit(void);

