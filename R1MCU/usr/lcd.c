#include "lcd.h"
#include "string.h"
extern void Delay_us(__IO uint32_t nTime);
void SPI2_Config(void) 
{ 
//   GPIO_InitTypeDef  GPIO_InitStructure; 
//   SPI_InitTypeDef   SPI_InitStructure; 

//   /* SPI2 ʱ��ʹ�� */
//   RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE); 
//  
//   /* SPI2 SCK(PB13)��MISO(PB14)��MOSI(PB15) ���� */
//   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
//   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//�����ٶ�50MHZ
//   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	        //����ģʽ
//   GPIO_Init(GPIOB, &GPIO_InitStructure);

//   /* SPI2 ����оƬ��Ƭѡ�������� PB0 */
//   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
//   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//�����ٶ�50MHZ 
//   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			//�������ģʽ
//   GPIO_Init(GPIOB, &GPIO_InitStructure);
//   
//   
// //   /* ����SPI1�����Ϲҽ���4�����裬������ʹ�ô�����ʱ����Ҫ��ֹ����3��SPI1 ���裬 ������������ */  
// //   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;           		//SPI1 SST25VF016BƬѡ 
// //   GPIO_Init(GPIOC, &GPIO_InitStructure);
// //   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;                //SPI1 VS1003Ƭѡ 
// //   GPIO_Init(GPIOB, &GPIO_InitStructure);
// //   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;                 //SPI1 ����ģ��Ƭѡ
// //   GPIO_Init(GPIOA, &GPIO_InitStructure);
// //   GPIO_SetBits(GPIOC, GPIO_Pin_4);							//SPI CS1
// //   GPIO_SetBits(GPIOB, GPIO_Pin_12);							//SPI CS4
// //   GPIO_SetBits(GPIOA, GPIO_Pin_4);							//SPI NSS	   
//   
//    /* SPI1���� ���� */ 
//   SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   //ȫ˫��  
//   SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						   //��ģʽ
//   SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					   //8λ
//   SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;						   //ʱ�Ӽ��� ����״̬ʱ��SCK���ֵ͵�ƽ
//   SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;						   //ʱ����λ ���ݲ����ӵ�һ��ʱ�ӱ��ؿ�ʼ
//   SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							   //�������NSS
//   SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;  //�����ʿ��� SYSCLK/64
//   SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;				   //���ݸ�λ��ǰ
//   SPI_InitStructure.SPI_CRCPolynomial = 7;							   //CRC����ʽ�Ĵ�����ʼֵΪ7 
//   SPI_Init(SPI2, &SPI_InitStructure);
//   
//   /* SPI1 ʹ�� */  
//   SPI_Cmd(SPI2,ENABLE);  


	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	
	//����A0 RST
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				     
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //���߷�ת�ٶ�Ϊ50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	
		//����A1 CS
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				     
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //���߷�ת�ٶ�Ϊ50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
		//����A5 CLK
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				     
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //���߷�ת�ٶ�Ϊ50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
		//����A7 Data
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_6;				     
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //���߷�ת�ٶ�Ϊ50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	//����E0Ϊ MzL05LCD��RST  //�ݽ�ط�
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				     
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //���߷�ת�ٶ�Ϊ50MHz
  GPIO_Init(GPIOE, &GPIO_InitStructure);	
	
	
}

/****************************************************************************
* ��    �ƣ�unsigned char SPI_WriteByte(unsigned char data) 
* ��    �ܣ�SPI1 д����
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷�����
****************************************************************************/  
unsigned char SPI2_WriteByte(unsigned char data) 
{ 
//  unsigned char Data = 0; 

// 	//GPIO_SetBits(GPIOB, GPIO_Pin_0);
//   //�ȴ����ͻ�������
//   while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==RESET); 
//   // ����һ���ֽ�  
// 	
//   SPI_I2S_SendData(SPI2,data); 

//    //�ȴ��Ƿ���յ�һ���ֽ� 
//   while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==RESET); 
//   // ��ø��ֽ�
//   Data = SPI_I2S_ReceiveData(SPI2); 
// 	//GPIO_ResetBits(GPIOB, GPIO_Pin_0);
//   // �����յ����ֽ� 
//   return Data; 
	
	  uint8_t i;
	uint8_t bbyte;
	bbyte = data;
  for(i=0;i<8;i++)
  {
   if(bbyte&0x80)
	 {
		 GPIO_SetBits(GPIOA,GPIO_Pin_7);
	 }
	 else
	 {
		 GPIO_ResetBits(GPIOA,GPIO_Pin_7);
	 }
   GPIO_SetBits(GPIOA,GPIO_Pin_5);
   Delay_us(1);
   
		GPIO_ResetBits(GPIOA,GPIO_Pin_5);
   Delay_us(1);
   bbyte<<=1; //??
  }  
	
	return bbyte;
}

void WriteLCD(uint8_t start, uint8_t ddata) 
{
  uint8_t start_data,Hdata,Ldata;

  if(start==0)                   //11111,(0),(0),0
   start_data=0xf8;	             //???
   else                          //11111,(0),(1),0
   start_data=0xfa;              //???

  Hdata=ddata&0xf0;		         //????
  Ldata=(ddata<<4)&0xf0;         //????
  SPI2_WriteByte(start_data);	         //??????
  Delay_us(2);
  SPI2_WriteByte(Hdata);	             //?????
  Delay_us(2);
  SPI2_WriteByte(Ldata);		         //?????
  Delay_us(10);
}


void LCD_Config(void)
{


	GPIO_SetBits(GPIOA, GPIO_Pin_0);
	Delay_us(2);
	GPIO_ResetBits(GPIOA, GPIO_Pin_0);
	Delay_us(2);
	GPIO_SetBits(GPIOA, GPIO_Pin_0);
	
	GPIO_SetBits(GPIOA, GPIO_Pin_1);
	//GPIO_SetBits(GPIOB, GPIO_Pin_0);
	Delay_us(22);
	WriteLCD(0,0x30);     //??????
  WriteLCD(0,0x0c);     //????,???,???
  WriteLCD(0,0x01);     //??,?DDRAM????????
  WriteLCD(0,0x06);  
	Delay_us(20);
	//GPIO_ResetBits(GPIOB, GPIO_Pin_0);
	
}

void LCD_Show(void)
{
	
		uint8_t i=0;
	uint8_t DIS1[] = {" OK212E SYSTEM  "};
uint8_t DIS2[] = {" OK200E SYSTEM  "};
uint8_t DIS3[] = {" www.aokemcu.cn "};
uint8_t DIS4[] = {"TEL:0535-2600876"};
	//GPIO_SetBits(GPIOB, GPIO_Pin_0);
//Delay_us(2);
				 WriteLCD(0,0x30);          //??????
//Delay_us(20);
		 WriteLCD(0,0x01);          //?? 
//Delay_us(20);
		 WriteLCD(0,0x13); 
		 WriteLCD(0,0x80); 
//Delay_us(20);
		 for(i=0;i<16;i++) WriteLCD(1,DIS1[i]);
		 WriteLCD(0,0x90); 
//Delay_us(20);
		 for(i=0;i<16;i++) WriteLCD(1,DIS2[i]);
		 WriteLCD(0,0x88); 
//Delay_us(20);
		 for(i=0;i<16;i++) WriteLCD(1,DIS3[i]);
		 WriteLCD(0,0x98); 
//Delay_us(20);
		 for(i=0;i<16;i++) WriteLCD(1,DIS4[i]);
//	Delay_us(20);
	//GPIO_ResetBits(GPIOB, GPIO_Pin_0);
// 	WriteLCD(0,0x34); 

}

void LCD_ShowText(uint8_t x,uint8_t y, char *text)
{
	uint8_t i;
	x &= 0x0f; 
	y &= 0x0f;
	switch(y)
	{
		 case 0:x+=0x80;
					 break;
		 case 1:x+=0x90;
					 break;
		 case 2:x+=0x88;
					 break;
		 case 3:x+=0x98;
					 break;
		 default:break;
	} 
	WriteLCD(0,x); 
	for(i=0;i<strlen(text);i++) 
	{
		WriteLCD(1,text[i]);
	}
	WriteLCD(0,0x8A); 
	WriteLCD(0,0x0F); 
}
