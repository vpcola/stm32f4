
#include "stm32f4xx.h"
#include "device.h"

static void SensorGPIO_Configuration(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

#define ADC1_DR_Address    ((uint32_t)0x40012308)
#define TX_FRONT(n)		GPIO_WriteBit(GPIOB, GPIO_Pin_8, (BitAction)(n))
#define TX_90DEG(n)		GPIO_WriteBit(GPIOA, GPIO_Pin_8, (BitAction)(n))
#define TX_45DEG(n)		GPIO_WriteBit(GPIOB, GPIO_Pin_8, (BitAction)(n))

volatile uint16_t ADC_DualConvertedValueTab[10];

static void DMA2_Configuration(void) {
	DMA_InitTypeDef DMA_InitStructure;

	DMA_InitStructure.DMA_Channel = DMA_Channel_0;
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC_DualConvertedValueTab;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = 8;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);
	DMA_Cmd(DMA2_Stream0, ENABLE);
}

static void ADC1_Config(void) {
  ADC_InitTypeDef ADC_InitStructure;

  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Left;
  ADC_InitStructure.ADC_NbrOfConversion = 4;
  ADC_Init(ADC1, &ADC_InitStructure);

  ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_112Cycles);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 2, ADC_SampleTime_112Cycles);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 3, ADC_SampleTime_112Cycles);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 4, ADC_SampleTime_112Cycles);
}

static void ADC2_Config(void) {
  ADC_InitTypeDef ADC_InitStructure;

  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Left;
  ADC_InitStructure.ADC_NbrOfConversion = 4;
  ADC_Init(ADC2, &ADC_InitStructure);

  ADC_RegularChannelConfig(ADC2, ADC_Channel_0, 1, ADC_SampleTime_112Cycles);
  ADC_RegularChannelConfig(ADC2, ADC_Channel_1, 2, ADC_SampleTime_112Cycles);
  ADC_RegularChannelConfig(ADC2, ADC_Channel_2, 3, ADC_SampleTime_112Cycles);
  ADC_RegularChannelConfig(ADC2, ADC_Channel_7, 4, ADC_SampleTime_112Cycles);
}

void Sensor_Configuration(void) {
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	SensorGPIO_Configuration();
	DMA2_Configuration();

	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 |
									GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	ADC_CommonInitStructure.ADC_Mode = ADC_DualMode_RegSimult;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	ADC1_Config();
	ADC2_Config();

	ADC_MultiModeDMARequestAfterLastTransferCmd(ENABLE);

	ADC_Cmd(ADC1, ENABLE);
	ADC_Cmd(ADC2, ENABLE);

	ADC_DMACmd(ADC1, ENABLE);

	ADC_SoftwareStartConv(ADC1);
}

void ReadSensor(int16_t *adc) {
 //uint32_t dly;

  TX_FRONT(1);
  delay(1);
  adc[0] = ADC_DualConvertedValueTab[0]>>4;
  adc[1] = ADC_DualConvertedValueTab[1]>>4;
  TX_FRONT(0);

  TX_90DEG(1);
  delay(1);
  adc[2] = ADC_DualConvertedValueTab[2]>>4;
  adc[3] = ADC_DualConvertedValueTab[3]>>4;
  TX_90DEG(0);

  TX_45DEG(1);
  delay(1);
  adc[4] = ADC_DualConvertedValueTab[4]>>4;
  adc[5] = ADC_DualConvertedValueTab[5]>>4;
  TX_45DEG(0);

  adc[6] = ADC_DualConvertedValueTab[6]>>4;
  adc[7] = ADC_DualConvertedValueTab[7]>>4;
}
