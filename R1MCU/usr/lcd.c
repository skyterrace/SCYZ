#include "lcd.h"
#include "string.h"
extern void Delay_us(__IO uint32_t nTime);
void SPI2_Config(void) 
{ 
//   GPIO_InitTypeDef  GPIO_InitStructure; 
//   SPI_InitTypeDef   SPI_InitStructure; 

//   /* SPI2 时钟使能 */
//   RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE); 
//  
//   /* SPI2 SCK(PB13)、MISO(PB14)、MOSI(PB15) 设置 */
//   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
//   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//口线速度50MHZ
//   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	        //复用模式
//   GPIO_Init(GPIOB, &GPIO_InitStructure);

//   /* SPI2 触摸芯片的片选控制设置 PB0 */
//   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
//   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//口线速度50MHZ 
//   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			//推挽输出模式
//   GPIO_Init(GPIOB, &GPIO_InitStructure);
//   
//   
// //   /* 由于SPI1总线上挂接了4个外设，所以在使用触摸屏时，需要禁止其余3个SPI1 外设， 才能正常工作 */  
// //   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;           		//SPI1 SST25VF016B片选 
// //   GPIO_Init(GPIOC, &GPIO_InitStructure);
// //   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;                //SPI1 VS1003片选 
// //   GPIO_Init(GPIOB, &GPIO_InitStructure);
// //   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;                 //SPI1 网络模块片选
// //   GPIO_Init(GPIOA, &GPIO_InitStructure);
// //   GPIO_SetBits(GPIOC, GPIO_Pin_4);							//SPI CS1
// //   GPIO_SetBits(GPIOB, GPIO_Pin_12);							//SPI CS4
// //   GPIO_SetBits(GPIOA, GPIO_Pin_4);							//SPI NSS	   
//   
//    /* SPI1总线 配置 */ 
//   SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   //全双工  
//   SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						   //主模式
//   SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					   //8位
//   SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;						   //时钟极性 空闲状态时，SCK保持低电平
//   SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;						   //时钟相位 数据采样从第一个时钟边沿开始
//   SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							   //软件产生NSS
//   SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;  //波特率控制 SYSCLK/64
//   SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;				   //数据高位在前
//   SPI_InitStructure.SPI_CRCPolynomial = 7;							   //CRC多项式寄存器初始值为7 
//   SPI_Init(SPI2, &SPI_InitStructure);
//   
//   /* SPI1 使能 */  
//   SPI_Cmd(SPI2,ENABLE);  


	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	
	//配置A0 RST
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				     
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //口线翻转速度为50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	
		//配置A1 CS
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				     
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //口线翻转速度为50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
		//配置A5 CLK
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				     
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //口线翻转速度为50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
		//配置A7 Data
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_6;				     
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //口线翻转速度为50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	//配置E0为 MzL05LCD的RST  //暂借地方
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				     
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //口线翻转速度为50MHz
  GPIO_Init(GPIOE, &GPIO_InitStructure);	
	
	
}

/****************************************************************************
* 名    称：unsigned char SPI_WriteByte(unsigned char data) 
* 功    能：SPI1 写函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：
****************************************************************************/  
unsigned char SPI2_WriteByte(unsigned char data) 
{ 
//  unsigned char Data = 0; 

// 	//GPIO_SetBits(GPIOB, GPIO_Pin_0);
//   //等待发送缓冲区空
//   while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==RESET); 
//   // 发送一个字节  
// 	
//   SPI_I2S_SendData(SPI2,data); 

//    //等待是否接收到一个字节 
//   while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==RESET); 
//   // 获得该字节
//   Data = SPI_I2S_ReceiveData(SPI2); 
// 	//GPIO_ResetBits(GPIOB, GPIO_Pin_0);
//   // 返回收到的字节 
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
