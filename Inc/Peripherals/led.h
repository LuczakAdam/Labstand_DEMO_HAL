/*
* Adam Luczak
* Laboratory stand with Cortex-M0 Microcontroller and E-Paper display
* Master Thesis 2018
* High-level version using STM Hardware Abstract Layer
*/

/* Include section */
#include <stm32l0xx_hal.h>

/* Macros */
#define LED1_PORT GPIOC
#define LED1_PIN GPIO_PIN_14

#define LED2_PORT GPIOC
#define LED2_PIN GPIO_PIN_15

#define LED3_PORT GPIOH
#define LED3_PIN GPIO_PIN_1

#define LED4_PORT GPIOA
#define LED4_PIN GPIO_PIN_14

#define LED5_PORT GPIOA
#define LED5_PIN GPIO_PIN_13

#define LED6_PORT GPIOA
#define LED6_PIN GPIO_PIN_10

#define LED7_PORT GPIOA
#define LED7_PIN GPIO_PIN_9

#define LED8_PORT GPIOB
#define LED8_PIN GPIO_PIN_15

/* Function prototypes */
void Labstand_LED_Init(void);
void Labstand_LED_DeInit(void);
void Labstand_LED_Write(uint8_t);
void Labstand_LED_Clear(void);
