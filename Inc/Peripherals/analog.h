/*
* Adam Luczak
* Laboratory stand with Cortex-M0 Microcontroller and E-Paper display
* Master Thesis 2018
* High-level version using STM Hardware Abstract Layer
*/

/* Include section */
#include <stm32l0xx_hal.h>

/* Macros */
#define VDD 3.3
#define ADC_OFFSET 60
#define SIGNAL_TABLE_SIZE 64
#define DAC_DMA_FREQUENCY 10

/* Function prototypes */
void Labstand_ADC_Init(void);
void Labstand_ADC_DMA_Init(void);
float Labstand_ADC_GetVoltage(void);
void Labstand_DAC_SetVoltage(float);
void Labstand_DAC_GenerateSignal(void);
void Labstand_DAC_Clear(void);
void Labstand_DAC_Init(void);
void Labstand_DAC_DMA_Init(void);
void Labstand_TIM6_Init(void);
