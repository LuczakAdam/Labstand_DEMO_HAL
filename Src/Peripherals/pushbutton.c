/* Include section */
#include <pushbutton.h>


/* Buttons GPIO pins init as external interrupt */
void Labstand_Pushbutton_Init(void)
{

	  GPIO_InitTypeDef GPIO_InitStruct;

	  __HAL_RCC_GPIOB_CLK_ENABLE();

	  GPIO_InitStruct.Pin = B1_PIN|B2_PIN|B3_PIN;
	  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  HAL_GPIO_Init(BUTTON_GPIO_PORT, &GPIO_InitStruct);

	  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
	  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

};


/* Buttons GPIO pins deinit */
void Labstand_Pushbutton_DeInit(void){


	  HAL_GPIO_DeInit(BUTTON_GPIO_PORT, B1_PIN|B2_PIN|B3_PIN);

};
