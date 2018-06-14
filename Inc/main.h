/*
* Adam Luczak
* Laboratory stand with Cortex-M0 Microcontroller and E-Paper display
* Master Thesis 2018
* High-level version using STM Hardware Abstract Layer
*/

#ifndef __MAIN_H__
#define __MAIN_H__



#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif

#define DO_NOTHING	asm("nop");
#define SEND_COMMAND_INVALID HAL_UART_Transmit(&huart, (unsigned char*)COMMAND_INVALID_MSG, strlen(COMMAND_INVALID_MSG), TIMEOUT);
#define SEND_COMMAND_RECEIVED HAL_UART_Transmit(&huart, (uint8_t*) COMMAND_RECV_MSG,strlen(COMMAND_RECV_MSG), TIMEOUT);
