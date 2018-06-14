/*
* Adam Luczak
* Laboratory stand with Cortex-M0 Microcontroller and E-Paper display
* Master Thesis 2018
* High-level version using STM Hardware Abstract Layer
*/

/* Include section */
#include <epaper.h>
#include <string.h>

/* Variables */
uint8_t text[EPD_HEIGHT][EPD_WIDTH];


/* Write text on Discovery E-Paper display */
void BSP_EPD_Write_Text(char data[]){

	  //set first line from top
	  int line = EPD_HEIGHT-1;
	  int index = 0;
	  int posInLine = 0;

	  //clear whole display
	  BSP_EPD_Clear(EPD_COLOR_WHITE);

	  //for every char in data
	  for (index = 0 ; index < strlen(data); index++){

		//if encountered line feed, decrement line number, go to first sign
	  	if(data[index] == 10){

	  		line--;
	  		posInLine = 0;

	  	}
	  	else if (data[index] == 92 && data[index+1] == 110){
	  		line--;
	  		posInLine = 0;
	  		index++;
	  	}
	  	//else
	  	else{
	  		//if encountered end of line, decrement line number, go to first sign
	  		if (posInLine == EPD_WIDTH){
	  		   line--;
	  		   posInLine = 0;
	  		}

	  		//append char from data to displayed text
	  		text[line][posInLine++] += data[index];
	  	}
	  }

	  //display formatted text, and clear memory after
	  for (index = EPD_HEIGHT-1; index >= line; index--){
		  BSP_EPD_DisplayStringAtLine(index,text[index]);
		  memset(&text[index], '\0', sizeof(text[index]));
	  }

	  //refresh display and wait 1 second
	  BSP_EPD_RefreshDisplay();

}
