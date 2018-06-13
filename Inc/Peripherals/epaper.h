/* Include section */
#include <stm32l0538_discovery_epd.h>

/* Macros */
#define EPD_WIDTH 24
#define EPD_HEIGHT 6
#define DATA_SIZE 200
#define WELCOME_TEXT "Stanowisko\nlaboratoryjne\nARM STM32 oraz E-Paper\n\nAdam Luczak\nPraca Magisterska 2018"

/* Function prototypes */
void BSP_EPD_Write_Text(char[]);
