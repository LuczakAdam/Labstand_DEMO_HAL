/*
* Adam Luczak
* Laboratory stand with Cortex-M0 Microcontroller and E-Paper display
* Master Thesis 2018
* High-level version using STM Hardware Abstract Layer
*/

/* Include section */
#include <stm32l0xx_hal.h>
#include <string.h>
#include <stdlib.h>

/* Macros */
#define INIT_MSG "Initialization done.\r\n"
#define COMMAND_RECV_MSG "Command received.\r\n"
#define COMMAND_INVALID_MSG "Invalid command.\r\n"
#define BAUDRATE 38400
#define TIMEOUT 20

/* Function prototypes */
void Labstand_UART_Init();
void HC05_Reset();
void HC05_Name(unsigned char*);
void HC05_UART_Config(int, int, int);
