#include "adc.h"


vu16 ADC_ConvertedValue[10];

/****************************************************************************
* ��    �ƣ�void ADC_Configuration(void)
* ��    �ܣ�ADC ���ú���
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷�����
****************************************************************************/ 
void ADC_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
// 	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

//     //����ADģ������˿�Ϊ���� 1·AD ����ͨ��
//   	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
//   	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
//   	GPIO_Init(GPIOC, &GPIO_InitStructure);
	/* Enable DMA clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

   /* Enable ADC1 and GPIOC clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

  	/* DMA channel1 configuration ----------------------------------------------*/
	//ʹ��DMA
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;			            //DMAͨ��1�ĵ�ַ 
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue[0];	            //DMA���͵�ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;					            //���ͷ���
	DMA_InitStructure.DMA_BufferSize = 10;								            //�����ڴ��С��100��16λ
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	 
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;				            //�����ڴ��ַ����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;		//ADC1ת����������16λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;				//���͵�Ŀ�ĵ�ַ��16λ���
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;									//ѭ��
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    
	/* ����DMA1ͨ��1��������ж� */
	//DMA_ITConfig(DMA1_Channel1,DMA_IT_TC, ENABLE);


	//ʹ��DMAͨ��1
	DMA_Cmd(DMA1_Channel1, ENABLE); 
  
	//����PA2ģ�����룬����ص�ѹ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure); // PA2,����ʱ������������
  
	//ADC����
	/* Resets ADC1 */ 
	ADC_DeInit(ADC1);


	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC1�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;		//ģ��ת��������ɨ��ģʽ����ͨ�������ǵ��Σ���ͨ����ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//ģ��ת��������ɨ��ģʽ����ͨ�������ǵ��Σ���ͨ����ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 2;               //�涨��˳����й���ת����ADCͨ������Ŀ�������Ŀ��ȡֵ��Χ��1��16
	ADC_Init(ADC1, &ADC_InitStructure);
	
	/* ADC1 regular channels configuration [����ģʽͨ������]*/ 

	//ADC1 ����ͨ������
	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_239Cycles5);	  //ͨ��16��ʱ�� 239.5����
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 2, ADC_SampleTime_239Cycles5);	  //ͨ��16��ʱ�� 239.5����
	
	ADC_TempSensorVrefintCmd(ENABLE); 

	//ʹ��ADC1 DMA 
	ADC_DMACmd(ADC1, ENABLE);
	//ʹ��ADC1
	ADC_Cmd(ADC1, ENABLE);	
	
	// ��ʼ��ADC1У׼�Ĵ���
	ADC_ResetCalibration(ADC1);
	//���ADC1У׼�Ĵ�����ʼ���Ƿ����
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	//��ʼУ׼ADC1
	ADC_StartCalibration(ADC1);
	//����Ƿ����У׼
	while(ADC_GetCalibrationStatus(ADC1));
	
	//ADC1ת������
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	 
}

int8_t GetTemperature(void)
{
	//ADCת��ֵDMA������
	uint16_t tmin,tmax;
	int8_t temp,i;
	uint16_t val;
	
	
	val=tmin=tmax=ADC_ConvertedValue[0];

	for(i=1;i<5;i++) 
	{
		val+=ADC_ConvertedValue[2*i];
		if(ADC_ConvertedValue[2*i]> tmax)tmax=ADC_ConvertedValue[2*i];
		if(ADC_ConvertedValue[2*i]< tmin)tmin=ADC_ConvertedValue[2*i];
	}
	val = (val-tmin-tmax)/3;
	temp = (0x6EF-val)*10 / 43 + 25;
// 	temp = (1.43-val*3/4096)*1000 / 4.3 + 25;
// 	temp = (0x6EF- ADC_ConvertedValue[0])*10 / 0x2B + 25;
	return temp;
}

int16_t GetBatVoltage(void)
{
	//ADCת��ֵDMA������
	uint16_t vmin,vmax;
	int16_t temp,i;
	uint16_t val;
	
	val=vmin=vmax=ADC_ConvertedValue[1];

	for(i=1;i<5;i++) 
	{
		val+=ADC_ConvertedValue[2*i+1];
		if(ADC_ConvertedValue[2*i+1]> vmax)vmax=ADC_ConvertedValue[2*i+1];
		if(ADC_ConvertedValue[2*i+1]< vmin)vmin=ADC_ConvertedValue[2*i+1];
	}
	val = (val-vmin-vmax)/3;

	temp = val * 3300/4096;
	return temp;
}
