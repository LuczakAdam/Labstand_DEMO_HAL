/*
* Adam Luczak
* Laboratory stand with Cortex-M0 Microcontroller and E-Paper display
* Master Thesis 2018
* High-level version using STM Hardware Abstract Layer
*/

/* Include section */
#include <analog.h>

/* Variables */
ADC_HandleTypeDef hadc;
DMA_HandleTypeDef hdma_adc;
DAC_HandleTypeDef hdac;
DMA_HandleTypeDef hdma_dac;
TIM_HandleTypeDef htim;
volatile uint16_t adcValue = 0;
volatile uint16_t dacValue = 0;


/* DAC signal generation look-up table - triangular waveform */
const uint16_t signalTable[SIGNAL_TABLE_SIZE] =
		{
		2048,2248,2447,2642,2831,3013,3185,3346,3495,3630,
		3750,3853,3939,4007,4056,4085,4095,4085,4056,4007,
		3939,3853,3750,3630,3495,3346,3185,3013,2831,2642,
		2447,2248,2048,1847,1648,1453,1264,1082,910,749,
		600,465,345,242,156,88,39,10,0,10, 39,88,156,242,
		345,465,600,749,910,1082,1264,1453,1648,1847
		};

/* Convert ADC value to voltage */
float Labstand_ADC_GetVoltage() {

	return (float)(adcValue-ADC_OFFSET)*(VDD/4096.0);

}

/* MCU analog-to-digital converter init */
void Labstand_ADC_Init() {

	ADC_ChannelConfTypeDef sConfig;

	hadc.Instance = ADC1;
	hadc.Init.OversamplingMode = DISABLE;
	hadc.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV64;
	hadc.Init.Resolution = ADC_RESOLUTION_12B;
	hadc.Init.SamplingTime = ADC_SAMPLETIME_160CYCLES_5;
	hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
	hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc.Init.ContinuousConvMode = ENABLE;
	hadc.Init.DiscontinuousConvMode = DISABLE;
	hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc.Init.DMAContinuousRequests = ENABLE;
	hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
	hadc.Init.LowPowerAutoWait = DISABLE;
	hadc.Init.LowPowerFrequencyMode = DISABLE;
	hadc.Init.LowPowerAutoPowerOff = DISABLE;
	HAL_ADC_Init(&hadc);

	sConfig.Channel = ADC_CHANNEL_0;
	sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
	HAL_ADC_ConfigChannel(&hadc, &sConfig);

	Labstand_ADC_DMA_Init();
	HAL_ADC_Start_DMA(&hadc,(uint32_t*)&adcValue,1);
}

/* ADC direct memory access configuarion */
void Labstand_ADC_DMA_Init(){

	__HAL_RCC_DMA1_CLK_ENABLE();
	HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

	hdma_adc.Instance = DMA1_Channel1;
	hdma_adc.Init.Request = DMA_REQUEST_0;
	hdma_adc.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_adc.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_adc.Init.MemInc = DMA_MINC_ENABLE;
	hdma_adc.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	hdma_adc.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	hdma_adc.Init.Mode = DMA_CIRCULAR;
	hdma_adc.Init.Priority = DMA_PRIORITY_MEDIUM;
	HAL_DMA_Init(&hdma_adc);

	__HAL_LINKDMA(&hadc,DMA_Handle,hdma_adc);

}

/* Set constant DAC voltage */
void Labstand_DAC_SetVoltage(float voltage){

	if ( voltage > VDD ) voltage = VDD;

	dacValue = (voltage/VDD)*4095;

	HAL_TIM_Base_Start(&htim);
	HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t*)&dacValue,1,DAC_ALIGN_12B_R);
}

/* Start generating waveform defined in argument array */
void Labstand_DAC_GenerateSignal(){

	HAL_TIM_Base_Start(&htim);
	HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t*)signalTable, SIGNAL_TABLE_SIZE, DAC_ALIGN_12B_R);

}

/* Stop generating signal and/or set output voltage to 0 */
void Labstand_DAC_Clear(){

	HAL_TIM_Base_Stop(&htim);
	HAL_DAC_Stop_DMA(&hdac, DAC_CHANNEL_1);

}

/* MCU digital-to-analog converter init */
void Labstand_DAC_Init(){

	DAC_ChannelConfTypeDef sConfig;

	hdac.Instance = DAC;
	HAL_DAC_Init(&hdac);

	sConfig.DAC_Trigger = DAC_TRIGGER_T6_TRGO;
	sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
	HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1);

	Labstand_DAC_DMA_Init();
	Labstand_TIM6_Init();
}

/* DAC direct memory access configuration */
void Labstand_DAC_DMA_Init(){

	__HAL_RCC_DMA1_CLK_ENABLE();
	HAL_NVIC_SetPriority(DMA1_Channel2_3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);

	hdma_dac.Instance = DMA1_Channel2;
	hdma_dac.Init.Request = DMA_REQUEST_9;
	hdma_dac.Init.Direction = DMA_MEMORY_TO_PERIPH;
	hdma_dac.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_dac.Init.MemInc = DMA_MINC_ENABLE;
	hdma_dac.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	hdma_dac.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	hdma_dac.Init.Mode = DMA_CIRCULAR;
	hdma_dac.Init.Priority = DMA_PRIORITY_MEDIUM;
	HAL_DMA_Init(&hdma_dac);

	__HAL_LINKDMA(&hdac,DMA_Handle1,hdma_dac);
}

/* Timer 6 used in DAC signal generation init */
void Labstand_TIM6_Init(){

	  TIM_MasterConfigTypeDef sMasterConfig;

	  htim.Instance = TIM6;
	  htim.Init.Prescaler = 16000;
	  htim.Init.CounterMode = TIM_COUNTERMODE_UP;
	  htim.Init.Period = 1000/DAC_DMA_FREQUENCY;
	  HAL_TIM_Base_Init(&htim);

	  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
	  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	  HAL_TIMEx_MasterConfigSynchronization(&htim, &sMasterConfig);

}
