#include "DISP.h"
#include "stdio.h"
#include "string.h"
#include "key.h"
#include "clock_calendar.h"
// #include "R1_MCU.h"
#include "eeprom.h"
#include "R1DEF.h"
//extern struct Param_s sParam;

#ifdef DISP_TYPE_TFT   //如果使用TFT显示模块

extern unsigned char ASCII1832[];


char strbuff[18];
/****************************************************************************
* 名    称：void lcd_DrawPicture(u16 StartX,u16 StartY, u8 Dir，u8 *pic)
* 功    能：在指定座标范围显示一副图片
* 入口参数：StartX     行起始座标
*           StartY     列起始座标
*			Dir		   图像显示方向       
*           pic        图片头指针
* 出口参数：无
* 说    明：图片取模格式为水平扫描，16位颜色模式  取模软件img2LCD
* 调用方法：lcd_DrawPicture(0,0,0,(u16*)demo);
****************************************************************************/
void lcd_DrawPicture(u16 StartX,u16 StartY,u8 Dir,u8 *pic)
{
  u32  i=8, len;
  u16 temp,x,y;
  	  
  
  x=((uint16_t)(pic[2]<<8)+pic[3])-1;	  	//从图像数组里取出图像的长度
  y=((uint16_t)(pic[4]<<8)+pic[5])-1;	  	//从图像数组里取出图像的高度
  if(Dir==0){
	LCD_WR_CMD(0x0003,0x1030);   			//图像显示方向为左下起  行递增  列递减
    LCD_WR_CMD(0x0210, StartX); 			//水平显示区起始地址 0-239
  	LCD_WR_CMD(0x0211, StartX+x);           	//水平显示区结束地址 0-239
  	LCD_WR_CMD(0x0212, StartY);     		//垂直显示区起始地址 0-399
  	LCD_WR_CMD(0x0213, StartY+y);         	//垂直显示区结束地址 0-399
  
  	LCD_WR_CMD(0x0200, StartX);		          	//水平显示区地址
  	LCD_WR_CMD(0x0201, StartY);		      		//垂直显示区地址
  }	 
  else if(Dir==1){
	LCD_WR_CMD(0x0003,0x1018);   			//图像显示方向为左下起  行递增  列递减
    LCD_WR_CMD(0x0210, StartY); 			//水平显示区起始地址 0-239
  	LCD_WR_CMD(0x0211, StartY+y);           //水平显示区结束地址 0-239
  	LCD_WR_CMD(0x0212, 399-(x+StartX));     //垂直显示区起始地址 0-399
  	LCD_WR_CMD(0x0213, 399-StartX);         //垂直显示区结束地址 0-399
  
  	LCD_WR_CMD(0x200, StartY);		          	//水平显示区地址
  	LCD_WR_CMD(0x201, 399-StartX);		      	//垂直显示区地址
  }	 
  LCD_WR_REG(0x0202);				          	//写数据到显示区

  len=2*((uint16_t)(pic[2]<<8)+pic[3])*((uint16_t)(pic[4]<<8)+pic[5]);   //计算出图像所占的字节数  
  while(i<(len+8)) {							 //从图像数组的第9位开始递增
  	temp=(uint16_t)( pic[i]<<8)+pic[i+1];		 //16位总线， 需要一次发送2个字节的数据
  	LCD_WR_Data(temp);							 //将取出的16位像素数据送入显示区
	i=i+2;										 //取模位置加2，以为获取下一个像素数据
  }
}

//写寄存器地址函数
void LCD_WR_REG(unsigned int index)
{
	*(__IO uint16_t *) (Bank1_LCD_C)= index;  

}
//写寄存器数据函数
//输入：dbw 数据位数，1为16位，0为8位。
void LCD_WR_CMD(unsigned int index,unsigned int val)
{	
	*(__IO uint16_t *) (Bank1_LCD_C)= index;	
	*(__IO uint16_t *) (Bank1_LCD_D)= val;
}

//读显示地址数据
unsigned int LCD_RD_data(void){
	unsigned int a=0;
	a=*(__IO uint16_t *) (Bank1_LCD_D);   //空操作
	a=*(__IO uint16_t *) (Bank1_LCD_D);   //读出的实际16位像素数据	  
	return(a);	
}
/****************************************************************************
* 名    称：u16 ili9320_BGR2RGB(u16 c)
* 功    能：RRRRRGGGGGGBBBBB 改为 BBBBBGGGGGGRRRRR 格式
* 入口参数：c      BRG 颜色值
* 出口参数：RGB 颜色值
* 说    明：内部函数调用
* 调用方法：
****************************************************************************/
u16 lcd_BGR2RGB(u16 c)
{
  u16  r, g, b;

  b = (c>>0)  & 0x1f;
  g = (c>>5)  & 0x3f;
  r = (c>>11) & 0x1f;
  
  return( (b<<11) + (g<<5) + (r<<0) );
}

//写16位像素数据函数
void    LCD_WR_Data(unsigned int val)
{   
	*(__IO uint16_t *) (Bank1_LCD_D)= val; 	
}



/****************************************************************************
* 名    称：void LCD_Init(void)
* 功    能：3 寸TFT(R61509) 初始化函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void TFT_Init(void)
{
	unsigned int i;
	GPIO_ResetBits(GPIOE, GPIO_Pin_1);	  //硬件复位
    Delay(0x1AFFf);			   
    GPIO_SetBits(GPIOE, GPIO_Pin_1 );		 
	Delay(0x1AFFf);	

	LCD_WR_CMD(0x0000, 0x00000);
    LCD_WR_CMD(0x0000, 0x00000);
	LCD_WR_CMD(0x0000, 0x00000);
	LCD_WR_CMD(0x0000, 0x00000);

	LCD_WR_CMD(0x0400, 0x06200);
	LCD_WR_CMD(0x0008, 0x00808);
	LCD_WR_CMD(0x0300, 0x00C00);//gamma
	LCD_WR_CMD(0x0301, 0x05A0B);
	LCD_WR_CMD(0x0302, 0x00906);
	LCD_WR_CMD(0x0303, 0x01017);
	LCD_WR_CMD(0x0304, 0x02300);
	LCD_WR_CMD(0x0305, 0x01700);
	LCD_WR_CMD(0x0306, 0x06309);
	LCD_WR_CMD(0x0307, 0x00C09);
	LCD_WR_CMD(0x0308, 0x0100C);
	LCD_WR_CMD(0x0309, 0x02232);
	LCD_WR_CMD(0x0010, 0x00016);//69.5Hz
	LCD_WR_CMD(0x0011, 0x00101);//
	LCD_WR_CMD(0x0012, 0x00000);//
	LCD_WR_CMD(0x0013, 0x00001);//
	LCD_WR_CMD(0x0100, 0x00330);//BT,AP
	LCD_WR_CMD(0x0101, 0x00237);//DC0,DC1,VC
	LCD_WR_CMD(0x0103, 0x00F00);//VDV
	LCD_WR_CMD(0x0280, 0x06100);//VCM
	LCD_WR_CMD(0x0102, 0x0C1B0);//VRH,VCMR,PSON,PON

	LCD_WR_CMD(0x0001, 0x00100);
	LCD_WR_CMD(0x0002, 0x00100);
	LCD_WR_CMD(0x0003, 0x01030);
	LCD_WR_CMD(0x0009, 0x00001);
	LCD_WR_CMD(0x000C, 0x00000);
	LCD_WR_CMD(0x0090, 0x08000);
	LCD_WR_CMD(0x000F, 0x00000);
	LCD_WR_CMD(0x0210, 0x00000);
	LCD_WR_CMD(0x0211, 0x000EF);
	LCD_WR_CMD(0x0212, 0x00000);
	LCD_WR_CMD(0x0213, 0x0018F);//432=1AF, 400=18F
	LCD_WR_CMD(0x0500, 0x00000);
	LCD_WR_CMD(0x0501, 0x00000);
	LCD_WR_CMD(0x0502, 0x0005F);
	LCD_WR_CMD(0x0401, 0x00001);
	LCD_WR_CMD(0x0404, 0x00000);

	LCD_WR_CMD(0x0007, 0x00100);//BASEE

	LCD_WR_CMD(0x0200, 0x00000);
	LCD_WR_CMD(0x0201, 0x00000);
    LCD_WR_CMD(0x200, 0);
    LCD_WR_CMD(0x201, 0);
	*(__IO uint16_t *) (Bank1_LCD_C)= 0x202;	 //准备写数据显示区
	for(i=0;i<96000;i++)
	{
	  LCD_WR_Data(0xFFFF);					 //用黑色清屏
	}  
}



/****************************************************************************
* 名    称：lcd_wr_zf(u16 StartX, u16 StartY, u16 X, u16 Y, u16 Color, u8 Dir, u8 *chr) 
* 功    能：在指定座标显示一串字符透明叠加在背景图片上
* 入口参数：StartX     行起始座标	 0-239
*           StartY     列起始座标	 0-399
*           X          长(为8的倍数）0-400
*           Y          宽			 0-240
*			Color      颜色0-65535
*           Dir		   图像显示方向
*           chr        字符串指针
* 出口参数：无
* 说    明：字符取模格式为单色字模，横向取模，字节正序  取模软件：ZIMO3
* 调用方法：lcd_wr_zf(0,0,100,100,(u16*)demo);
****************************************************************************/
//+++++++++++++++++++++++LCD写字符子程序
void lcd_wr_zf(u16 StartX, u16 StartY, u16 X, u16 Y, u16 Color, u8 Dir, u8 *chr)
{	unsigned int temp=0,num,R_dis_mem=0,Size=0,x=0,y=0,i=0;


	if(Dir==2) LCD_WR_CMD(0x0003,0x1010);   //图像显示方向为右下起  行递减  列递增  AM=0  I/D[1:0]=00	<--
	else if(Dir==3) LCD_WR_CMD(0x0003,0x1028);   //图像显示方向为右上起  行递减  列递增  AM=1  I/D[1:0]=10	V
  	if(Dir==0){
		LCD_WR_CMD(0x0003,0x1030);		  //图像显示方向为左上起  行递增  列递增  AM=0  I/D[1:0]=11  -->
		LCD_WR_CMD(0x0210, StartX); 	  //水平显示区起始地址 0-239
  		LCD_WR_CMD(0x0211, StartX+X-1);   //水平显示区结束地址 0-239
  		LCD_WR_CMD(0x0212, StartY);       //垂直显示区起始地址 0-399
  		LCD_WR_CMD(0x0213, StartY+Y-1);   //垂直显示区结束地址 0-399
		LCD_WR_CMD(0x0200, StartX);		  //水平显示区地址
  		LCD_WR_CMD(0x0201, StartY);		  //垂直显示区地址	
		LCD_WR_REG(0x0202);               //准备写数据显示区
		Size=X*Y;						  //字符串或字符占用的像素尺寸
		while(i<Size){					  
			temp=*chr++;				  //一个字节代表8个像素，因此加1代表索引到下8个像素
			for(num=0; num<8; num++){	  //数组的每个字节代表了8个像素	   
											
				if((temp&0x80)>0){		  //对字节的各位进行判断，为1的用带入参数的16位颜色值标示，写入到像素位置。						 
				
					LCD_WR_Data(Color); 		
				}
				else{
					LCD_WR_CMD(0x0200, StartX+x);		//水平显示区地址
  					LCD_WR_CMD(0x0201, StartY+y);		//垂直显示区地址								
					LCD_WR_REG(0x0202);					//准备读数据显示区
					R_dis_mem=LCD_RD_data();		  	//读取背景色，为叠加产生透明效果作准备	
					LCD_WR_Data(R_dis_mem); 		//对字节的各位进行判断，为0的用当前背景像素16位颜色值标示。 						
				}
				temp=temp<<1; 		  				//字节各位的移出
				x++;
				if(x>=X){x=0; y++;}				    //计算像素递增为当前的x和y，为当前像素读背景颜色做准备
				i++;	
			}				  
							
		}
	}
	else if(Dir==1){
		LCD_WR_CMD(0x0003,0x1018);   	  		//图像显示方向为左下起  行递增  列递减  AM=1  I/D[1:0]=01	A
		LCD_WR_CMD(0x0210, StartY); 	  		//水平显示区起始地址 0-239
  		LCD_WR_CMD(0x0211, StartY+Y-1);    		//水平显示区结束地址 0-239
  		LCD_WR_CMD(0x0212, 399-(StartX+X-1));   //垂直显示区起始地址 0-399
  		LCD_WR_CMD(0x0213, 399-StartX);    		//垂直显示区结束地址 0-399
		LCD_WR_CMD(0x0200, StartY);		  	  	//水平显示区地址
  		LCD_WR_CMD(0x0201, 399-StartX);	 	  	//垂直显示区地址	
		LCD_WR_REG(0x0202);                   	//准备写数据显示区

		Size=X*Y;						  		//字符串或字符占用的像素尺寸
		while(i<Size){					  
			temp=*chr++;				  		//一个字节代表8个像素，因此加1代表索引到下8个像素
			for(num=0; num<8; num++){	  		//数组的每个字节代表了8个像素	    
				if((temp&0x80)>0){		  		//对字节的各位进行判断，为1的用带入参数的16位颜色值标示，写入到像素位置。						 
				
					LCD_WR_Data(Color); 		
				}
				else{
// 					LCD_WR_CMD(0x0200, StartY+y);		//水平显示区地址
//   					LCD_WR_CMD(0x0201, 399-(StartX+x));	//垂直显示区地址								
// 					LCD_WR_REG(0x0202);					//准备读数据显示区
// 					R_dis_mem=LCD_RD_data();		  	//读取背景色，为叠加产生透明效果作准备	
// 					LCD_WR_Data(R_dis_mem); 		//对字节的各位进行判断，为0的用当前背景像素16位颜色值标示。 
						LCD_WR_Data(0xffff);					
				}
				temp=temp<<1; 		  				//字节各位的移出
				x++;
				if(x>=X){x=0; y++;}				    //计算像素递增为当前的x和y，为当前像素读背景颜色做准备
				i++;	
			}							
		}
	}
}


//++++++++根据数值获取各位的数字字符数据首地址+++++++++++++++++++++++++++++++++++++++++++
unsigned char *num_pub(unsigned  int a){
	unsigned char *b;
	switch(a){
		case 0x01: b=&ASCII1832[17*96]; break;
 		case 0x02: b=&ASCII1832[18*96]; break;
		case 0x03: b=&ASCII1832[19*96]; break;
		case 0x04: b=&ASCII1832[20*96]; break;
		case 0x05: b=&ASCII1832[21*96]; break;
		case 0x06: b=&ASCII1832[22*96]; break;
		case 0x07: b=&ASCII1832[23*96]; break;
		case 0x08: b=&ASCII1832[24*96]; break;
		case 0x09: b=&ASCII1832[25*96]; break;
		case 0x00: b=&ASCII1832[16*96]; break;
		default: b=&ASCII1832[0*96];break;
	}
	return(b);
}
// ++++++++++++++++TFT 复位操作
void lcd_rst(void){
	GPIO_ResetBits(GPIOE, GPIO_Pin_1);	      //PE1 为LCD 复位信号
    Delay(0xAFFFFf);					   
    GPIO_SetBits(GPIOE, GPIO_Pin_1 );		 	 
	Delay(0xAFFFFf);	
}

void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}
//////////FSMC 接口设置///////////////////////////////////////////
void FSMC_LCD_Init(void)
{
  FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef  p;	
  GPIO_InitTypeDef GPIO_InitStructure;	    
  //使能FSMC外设时钟
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);   
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC |
                         RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE , ENABLE);  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; 			  //LCD背光控制
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ; 	 		  //LCD复位
  GPIO_Init(GPIOE, &GPIO_InitStructure);   	   	
  // 复用端口为FSMC接口 FSMC-D0--D15
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 |
                                GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_14 | 
                                GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);   
   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | 
                                GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
                                GPIO_Pin_15;
  GPIO_Init(GPIOE, &GPIO_InitStructure);    
  //FSMC NE1  LCD片选
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  //FSMC RS---LCD指令 指令/数据	切换
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ; 
  GPIO_Init(GPIOD, &GPIO_InitStructure); 	
  GPIO_SetBits(GPIOD, GPIO_Pin_13);			           //LCD背光打开
  
  
  //FSMC接口特性配置
  p.FSMC_AddressSetupTime = 0x02;
  p.FSMC_AddressHoldTime = 0x00;
  p.FSMC_DataSetupTime = 0x05;
  p.FSMC_BusTurnAroundDuration = 0x00;
  p.FSMC_CLKDivision = 0x00;
  p.FSMC_DataLatency = 0x00;
  p.FSMC_AccessMode = FSMC_AccessMode_B;

 
  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
  FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
  FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;
 
  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 		
  /* Enable FSMC Bank1_SRAM Bank */
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);  
}


void TFT_Write_Char(u16 StartX, u16 StartY, u16 X, u16 Y, u16 Color, u8 Trans, u8 chr)
{	
		unsigned int temp=0,num,R_dis_mem=0,Size=0,x=0,y=0,i=0;
		unsigned int char_index=0;

		LCD_WR_CMD(0x0003,0x1018);   	  		//图像显示方向为左下起  行递增  列递减  AM=1  I/D[1:0]=01	A
		LCD_WR_CMD(0x0210, StartY); 	  		//水平显示区起始地址 0-239
  	LCD_WR_CMD(0x0211, StartY+Y-1);    		//水平显示区结束地址 0-239
  	LCD_WR_CMD(0x0212, 399-(StartX+X-1));   //垂直显示区起始地址 0-399
  	LCD_WR_CMD(0x0213, 399-StartX);    		//垂直显示区结束地址 0-399
		LCD_WR_CMD(0x0200, StartY);		  	  	//水平显示区地址
  	LCD_WR_CMD(0x0201, 399-StartX);	 	  	//垂直显示区地址	
		LCD_WR_REG(0x0202);                   	//准备写数据显示区

		Size=X*Y;						  		//字符串或字符占用的像素尺寸
		while(i<Size){		
		char_index = (chr-0x20)*96+i/8;			
		temp=ASCII1832[char_index];				  		//一个字节代表8个像素，因此加1代表索引到下8个像素
		
			for(num=0; num<8; num++){	  		//数组的每个字节代表了8个像素	    
				if((temp&0x80)>0){		  		//对字节的各位进行判断，为1的用带入参数的16位颜色值标示，写入到像素位置。						 
					LCD_WR_Data(Color); 		
				}
				else{
				if(Trans)
				{
					LCD_WR_CMD(0x0200, StartY+y);		//水平显示区地址
 					LCD_WR_CMD(0x0201, 399-(StartX+x));	//垂直显示区地址								
					LCD_WR_REG(0x0202);					//准备读数据显示区
					R_dis_mem=LCD_RD_data();		  	//读取背景色，为叠加产生透明效果作准备	
					LCD_WR_Data(R_dis_mem); 		//对字节的各位进行判断，为0的用当前背景像素16位颜色值标示。 
				}
				else
				{
					LCD_WR_Data(0xffff); 	
				}
				}
				temp=temp<<1; 		  				//字节各位的移出
				x++;
				if(x>=X){x=0; y++;}				    //计算像素递增为当前的x和y，为当前像素读背景颜色做准备
				i++;	
			}							
		}
}

void TFT_ShowText(uint8_t Line,uint8_t Col,uint16_t Color,char* Str)
{
	uint8_t count,i,chr;
	uint16_t x,y;
	count = strlen(Str);
	y=Line*29.5;
	for(i=0;i<count;i++)
	{
		chr=Str[i];
		if(chr>=0x20 && chr <=0x7E)
		{
		x=(Col+i)*24;

		TFT_Write_Char(x, y, 24, 32, Color, 0, chr);
		}
	}
}
void TFT_SetCursor(uint8_t Line,uint8_t Col,uint16_t Color)
{
	uint16_t x,y;
	y=Line*29.5;
	x=Col*24;

	TFT_Write_Char(x, y, 24, 32, Color, 1, '_');
}

void TFT_Clear(void)
{
  TFT_Init();
}


//公用函数
void DispInit(void)
{
	FSMC_LCD_Init();							 //FSMC总线配置		  
  TFT_Init();								 //液晶初始化		
}

//显示调试信息，status将转换成十六进制显示
void ShowDebugInfo(uint8_t line,uint8_t col, uint8_t status)
{
// 	char strbuff[2];
	sprintf(strbuff,"%0.2X",status);
	TFT_ShowText(line,col,0xff,strbuff);
}

//在第line行显示一个字符信息info
void ShowStatusInfo(uint8_t line,uint8_t col,char* info)
{
	TFT_ShowText(line,col,0x00,info);
}

//显示电池信息
void ShowBatteryInfo(uint8_t val)
{
// 	char strbuff[10];
	sprintf(strbuff," Power:%0.3d%c",val,'%');
	TFT_ShowText(3,0,0x00,strbuff);
}

//显示当前日期时间
void ShowCurrentTime(void)
{
		CalculateTime();
		sprintf(strbuff,"%0.4d-%0.2d-%0.2d %0.2d:%0.2d",s_DateStructVar.Year,s_DateStructVar.Month,s_DateStructVar.Day,
		s_TimeStructVar.HourHigh*10+s_TimeStructVar.HourLow, s_TimeStructVar.MinHigh*10+s_TimeStructVar.MinLow
		);
		
		TFT_ShowText(0,0,0x00,strbuff);
}

//显示默认页面
void ShowDefaultScreen(uint8_t cPower,uint8_t cWorkMode)
{
// 	lcd_rst();
	ShowCurrentTime();
	ShowBatteryInfo(cPower);
	TFT_ShowText(7,0,0x00,"setting");
	switch(cWorkMode)
	{
		case 1:
			TFT_ShowText(7,10,0x00,"manual");
			break;
		case 2:
			TFT_ShowText(7,10,0x00,"  home");
			break;
		default:
			TFT_ShowText(7,10,0x00,"  auto");
			break;
	}
	
}

//设置当前日期时间
void SetCurrentDateTime(void)
{
	uint8_t set_index,HourTemp,MinTemp;

	struct Date_s s_DateStructTemp;

	uint8_t refresh = 1;
	
	char txtBuffer[10];
	
	
	HourTemp =s_TimeStructVar.HourHigh*10+s_TimeStructVar.HourLow;
	MinTemp = s_TimeStructVar.MinHigh*10+s_TimeStructVar.MinLow;
	
	s_DateStructTemp.Year = s_DateStructVar.Year;
	s_DateStructTemp.Month = s_DateStructVar.Month;
	s_DateStructTemp.Day = s_DateStructVar.Day;
	
	set_index = 0;
	TFT_Clear();	
	while(1)
	{
		IWDG_ReloadCounter(); //喂狗
		if(Key_Release(KEY_OK))  //保存设置的日期
		{
			
			SetDate(s_DateStructTemp.Day,s_DateStructTemp.Month,s_DateStructTemp.Year);
			SetTime(HourTemp,MinTemp,00);
// 			TFT_Init();
			return;
			
		}
		if(Key_Release(KEY_SET)) //不保存返回
		{
// 			TFT_Init();
			return;
		}

		if(Key_Press(KEY_RIGHT))
		{
			set_index++;
			if(set_index>4)set_index=0;
			refresh = 1;
		}
		if(Key_Press(KEY_LEFT))
		{
			if(set_index == 0)set_index=4;
			else set_index--;
			refresh = 1;
		}
		if(Key_Press(KEY_DOWN))
		{
			switch(set_index)
			{
				case 0:
					s_DateStructTemp.Year--;
					if(s_DateStructTemp.Year<2012) s_DateStructTemp.Year = 2099;
					break;
				case 1:
					s_DateStructTemp.Month-- ;
					if(s_DateStructTemp.Month < 1) s_DateStructTemp.Month = 12;
					break;
				case 2:
					s_DateStructTemp.Day--;
					if(s_DateStructTemp.Day < 1) s_DateStructTemp.Day= 31;
					break;
				case 3:
					if(HourTemp<1) HourTemp = 23;
					else HourTemp--;
					break;
				case 4:
					if(MinTemp<1) MinTemp=59;
					else MinTemp--;
					break;
				default:
					break;
			}
			refresh = 1;
		}
		if(Key_Press(KEY_UP))
		{
			switch(set_index)
			{
				case 0:
					s_DateStructTemp.Year++;
					if(s_DateStructTemp.Year>2099) s_DateStructTemp.Year = 2012;
					break;
				case 1:
					s_DateStructTemp.Month++;
					if(s_DateStructTemp.Month >12) s_DateStructTemp.Month = 1;
					break;
				case 2:
					s_DateStructTemp.Day++;
					if(s_DateStructTemp.Day >31) s_DateStructTemp.Day = 1;
					break;
				case 3:
					HourTemp++;
					if(HourTemp>23) HourTemp = 0;
					 
					break;
				case 4:
					MinTemp++;
					if(MinTemp>59) MinTemp=0;
					 
					break;
				default:
					break;
			}
			refresh = 1;
		}

	if(refresh)
	{
		sprintf(txtBuffer,"%0.4d-%0.2d-%0.2d",s_DateStructTemp.Year,s_DateStructTemp.Month,s_DateStructTemp.Day);
		TFT_ShowText(2,2,0x00,txtBuffer);
		
		sprintf(txtBuffer,"%0.2d:%0.2d",HourTemp,MinTemp);
		TFT_ShowText(3,4,0x00,txtBuffer);

		switch(set_index)
		{
			case 0:
				TFT_SetCursor(2,4,0x00);
				TFT_SetCursor(2,5,0x00);
				break;
			case 1:
				TFT_SetCursor(2,7,0x00);
				TFT_SetCursor(2,8,0x00);
				break;
			case 2:
				TFT_SetCursor(2,10,0x00);
				TFT_SetCursor(2,11,0x00);
				break;
			case 3:
				TFT_SetCursor(3,4,0x00);
				TFT_SetCursor(3,5,0x00);
				break;
			case 4:
				TFT_SetCursor(3,7,0x00);
				TFT_SetCursor(3,8,0x00);
				break;
			default:
				break;
			
		}

		TFT_ShowText(7,0,0x00,"cancel");
		TFT_ShowText(7,14,0x00,"ok");
		

		refresh = 0;
	}
	}

}


//设置工作时间
void SetWorkingTime(void)
{
	unsigned char weekday,i;
	unsigned char set_index;
	unsigned char refresh = 1;
	
	char txtBuffer[11];
	
	struct WorkTime_s s_WorkTimeTemp[7];
	
	for(i=0;i<7;i++)
	{
		s_WorkTimeTemp[i].StartHour = s_WorkTimeVar[i].StartHour;
		s_WorkTimeTemp[i].StartMin = s_WorkTimeVar[i].StartMin;
		s_WorkTimeTemp[i].EndHour = s_WorkTimeVar[i].EndHour;
		s_WorkTimeTemp[i].EndMin = s_WorkTimeVar[i].EndMin;
	}
	
	set_index = 0;
	TFT_Clear();	

	while(1)
	{
		IWDG_ReloadCounter(); //喂狗
		if(Key_Release(KEY_OK))
		{
				for(i=0;i<7;i++)
				{
					s_WorkTimeVar[i].StartHour = s_WorkTimeTemp[i].StartHour;
					s_WorkTimeVar[i].StartMin = s_WorkTimeTemp[i].StartMin;
					s_WorkTimeVar[i].EndHour = s_WorkTimeTemp[i].EndHour;
					s_WorkTimeVar[i].EndMin = s_WorkTimeTemp[i].EndMin;
				}
				SaveWorkTime();
			return;
		}
		if(Key_Release(KEY_SET))
		{
			return;
		}

		if(Key_Press(KEY_RIGHT))
		{
			set_index++;
			if(set_index>27)set_index=0;
			refresh = 1;
		}
		if(Key_Press(KEY_LEFT))
		{
			if(set_index == 0)set_index=27;
			else set_index--;
			refresh = 1;
		}
		if(Key_Press(KEY_DOWN))
		{
			weekday = (int) set_index/4;
			switch(set_index%4)
			{
				case 0:
					if(s_WorkTimeTemp[weekday].StartHour<1) s_WorkTimeTemp[weekday].StartHour = 23;
					else s_WorkTimeTemp[weekday].StartHour--;

					break;
				case 1:
					if(s_WorkTimeTemp[weekday].StartMin<1) s_WorkTimeTemp[weekday].StartMin = 59;
					else s_WorkTimeTemp[weekday].StartMin--;
					break;
				case 2:
					if(s_WorkTimeTemp[weekday].EndHour<1) s_WorkTimeTemp[weekday].EndHour = 23;
					else s_WorkTimeTemp[weekday].EndHour--;
					break;
				case 3:
					if(s_WorkTimeTemp[weekday].EndMin<1) s_WorkTimeTemp[weekday].EndMin = 59;
					else s_WorkTimeTemp[weekday].EndMin--;
					break;
				default:
					break;
			}
			refresh = 1;
		}
		if(Key_Press(KEY_UP))
		{
			weekday = (int) set_index/4;
			switch(set_index%4)
			{
				case 0:
					s_WorkTimeTemp[weekday].StartHour++;
					if(s_WorkTimeTemp[weekday].StartHour>23) s_WorkTimeTemp[weekday].StartHour = 0;

					break;
				case 1:
					s_WorkTimeTemp[weekday].StartMin++;
					if(s_WorkTimeTemp[weekday].StartMin>59) s_WorkTimeTemp[weekday].StartMin = 0;
					break;
				case 2:
					s_WorkTimeTemp[weekday].EndHour++;
					if(s_WorkTimeTemp[weekday].EndHour>23) s_WorkTimeTemp[weekday].EndHour = 0;
					
					break;
				case 3:
					s_WorkTimeTemp[weekday].EndMin++;
					if(s_WorkTimeTemp[weekday].EndMin>23) s_WorkTimeTemp[weekday].EndMin = 0;
					break;
				default:
					break;
			}
			refresh = 1;
		}

	if(refresh)
	{

		TFT_ShowText(0,0,0x00,"MON:");
		TFT_ShowText(1,0,0x00,"TUE:");
		TFT_ShowText(2,0,0x00,"WED:");
		TFT_ShowText(3,0,0x00,"THU:");
		TFT_ShowText(4,0,0x00,"FRI:");
		TFT_ShowText(5,0,0x00,"SAT:");
		TFT_ShowText(6,0,0x00,"SUN:");


		for(i=0;i<7;i++)
		{
			sprintf(txtBuffer,"%0.2d:%0.2d-%0.2d:%0.2d",s_WorkTimeTemp[i].StartHour,s_WorkTimeTemp[i].StartMin,s_WorkTimeTemp[i].EndHour,s_WorkTimeTemp[i].EndMin);
			

			TFT_ShowText(i,4,0x00,txtBuffer);
		}
		
		TFT_ShowText(7,0,0x00,"cancel");
		TFT_ShowText(7,14,0x00,"ok");
		
		weekday = (int) set_index/4;
		switch(set_index%4)
		{
			case 0:
				TFT_SetCursor(weekday,4,0x00);
				TFT_SetCursor(weekday,5,0x00);
				break;
			case 1:
				TFT_SetCursor(weekday,7,0x00);
				TFT_SetCursor(weekday,8,0x00);
				break;
			case 2:
				TFT_SetCursor(weekday,10,0x00);
				TFT_SetCursor(weekday,11,0x00);
				break;
			case 3:
				TFT_SetCursor(weekday,13,0x00);
				TFT_SetCursor(weekday,14,0x00);
				break;
			default:
				break;
			
		}
	refresh = 0;
		}
	}
}

void DispClear(void)
{
	TFT_Init();
}
void SetParam(void)
{
	uint8_t set_index;
	uint8_t paramtemp[14];

	uint8_t refresh = 1;
	
// 	LoadParams();
	
	paramtemp[0] = sR1Mower.sRunParam.DrvSpeedH;  //驱动电机速度高
	paramtemp[1] = sR1Mower.sRunParam.DrvSpeedM ;//驱动电机速度中
	paramtemp[2] = sR1Mower.sRunParam.DrvSpeedL;//驱动电机速度低
	
	paramtemp[3] = sR1Mower.sRunParam.BsStrenghH ;//边界信号强度高
	paramtemp[4] = sR1Mower.sRunParam.BsStrenghM;  //边界信号强度中
	paramtemp[5] = sR1Mower.sRunParam.BsStrenghL;//边界信号强度低
	
	
	paramtemp[6] = sR1Mower.sRunParam.KP1*100;//kp1
	paramtemp[7] = sR1Mower.sRunParam.KD1*100;//kd1
	paramtemp[8] = sR1Mower.sRunParam.KI1*100;//ki1
	
	paramtemp[9] = sR1Mower.sRunParam.KP2*100;//kp2
	paramtemp[10] = sR1Mower.sRunParam.KD2*100;//kd2
	paramtemp[11] = sR1Mower.sRunParam.KI2*100;//ki2
	
	paramtemp[12] = sR1Mower.sRunParam.MowSpeed; //割草电机速度
	paramtemp[13] = sR1Mower.sRunParam.PowerLimit ;//电池剩余电量限值
	
	DispClear();
	TFT_ShowText(0,2,0x00,"Configuration");
	
	set_index = 0;
		
	while(1)
	{
		IWDG_ReloadCounter(); //喂狗
		if(Key_Release(KEY_OK))  //保存设置
		{
			sR1Mower.sRunParam.DrvSpeedH = paramtemp[0];  //驱动电机速度高
			sR1Mower.sRunParam.DrvSpeedM = paramtemp[1] ;//驱动电机速度中
			sR1Mower.sRunParam.DrvSpeedL = paramtemp[2] ;//驱动电机速度低
			
			sR1Mower.sRunParam.BsStrenghH = paramtemp[3];//边界信号强度高
			sR1Mower.sRunParam.BsStrenghM = paramtemp[4];  //边界信号强度中
			sR1Mower.sRunParam.BsStrenghL = paramtemp[5];//边界信号强度低
			
			
			sR1Mower.sRunParam.KP1 = paramtemp[6]/100.0;//kp1
			sR1Mower.sRunParam.KD1 = paramtemp[7]/100.0;//kd1
			sR1Mower.sRunParam.KI1 = paramtemp[8]/100.0;//ki1
			
			sR1Mower.sRunParam.KP2 = paramtemp[9]/100.0 ;//kp2
			sR1Mower.sRunParam.KD2 = paramtemp[10]/100.0;//kd2
			sR1Mower.sRunParam.KI2 = paramtemp[11]/100.0 ;//ki2
			
			sR1Mower.sRunParam.MowSpeed = paramtemp[12]; //割草电机速度
			sR1Mower.sRunParam.PowerLimit = paramtemp[13] ;//电池剩余电量限值
			
			SaveParams();
			return;
			
		}
		if(Key_Release(KEY_SET)) //不保存返回
		{
			return;
		}

		if(Key_Press(KEY_RIGHT))
		{
			set_index++;
			if(set_index>13)set_index=0;
			refresh = 1;
		}
		if(Key_Press(KEY_LEFT))
		{
			if(set_index == 0)set_index=13;
			else set_index--;
			refresh = 1;
		}
		if(Key_Press(KEY_DOWN))
		{
			switch(set_index)
			{
				case 0:
					paramtemp[0]--;
					if(paramtemp[0]<paramtemp[1]) paramtemp[0] = 60;
					break;
				case 1:
					paramtemp[1]-- ;
					if(paramtemp[1] < paramtemp[2]) paramtemp[1] = paramtemp[0];
					break;
				case 2:
					paramtemp[2]--;
					if(paramtemp[2] < 1) paramtemp[2] = paramtemp[1];
					break;
				case 3:
					paramtemp[3]--;
					if(paramtemp[3] < paramtemp[4]) paramtemp[3] = 100;
					break;
				case 4:
					paramtemp[4]--;
					if(paramtemp[4] < paramtemp[5]) paramtemp[4] = paramtemp[3];
					break;
				case 5:
					paramtemp[5]--;
					if(paramtemp[5] < 1) paramtemp[5] = paramtemp[4];
					break;
				case 12:
					if(paramtemp[12] == 0) paramtemp[12] = 60;
					else paramtemp[12]--;
					break;
				case 13:
					if(paramtemp[13] == 0) paramtemp[13] = 100;
					else paramtemp[13]--;
					break;
				default:
					paramtemp[set_index]--;
					break;
			}
			refresh = 1;
		}
		if(Key_Press(KEY_UP))
		{
			switch(set_index)
			{
				case 0:
					paramtemp[0]++;
					if(paramtemp[0]>60) paramtemp[0] = paramtemp[1];
					break;
				case 1:
					paramtemp[1]++ ;
					if(paramtemp[1] > paramtemp[0]) paramtemp[1] = paramtemp[2];
					break;
				case 2:
					paramtemp[2]++;
					if(paramtemp[2] > paramtemp[1]) paramtemp[2] = 1;
					break;
				case 3:
					paramtemp[3]++;
					if(paramtemp[3] >100 ) paramtemp[3] = paramtemp[4];
					break;
				case 4:
					paramtemp[4]++;
					if(paramtemp[4] > paramtemp[3]) paramtemp[4] = paramtemp[5];
					break;
				case 5:
					paramtemp[5]++;
					if(paramtemp[5] > paramtemp[4]) paramtemp[5] = 1;
					break;
				case 12:
					paramtemp[12]++;
					if(paramtemp[12] > 60 ) paramtemp[12] = 0;
					break;
				case 13:
					paramtemp[13]++;
					if(paramtemp[13] >100) paramtemp[13] = 0;
					break;
				default:
					paramtemp[set_index]++;
					break;
			}
			refresh = 1;
		}

	if(refresh)
	{
		//驱动电机速度
		sprintf(strbuff,"SH%0.3dSM%0.3dSL%0.3d",paramtemp[0],paramtemp[1],paramtemp[2]);
		TFT_ShowText(1,0,0x00,strbuff);
		
		//显示边界强度
		sprintf(strbuff,"BH%0.3dBM%0.3dBL%0.3d",paramtemp[3],paramtemp[4],paramtemp[5]);
		TFT_ShowText(2,0,0x00,strbuff);


				//显示kp1 kd1 ki1
		sprintf(strbuff,"P1%0.3dD1%0.3dI1%0.3d",paramtemp[6],paramtemp[7],paramtemp[8]);
		TFT_ShowText(3,0,0x00,strbuff);
		
				//显示kp2 kd2 ki2
		sprintf(strbuff,"P2%0.3dD2%0.3dI2%0.3d",paramtemp[9],paramtemp[10],paramtemp[11]);
		TFT_ShowText(4,0,0x00,strbuff);

		//显示割草电机速度和电量限值
		sprintf(strbuff,"MS%0.3dPL%0.3d",paramtemp[12],paramtemp[13]);
		TFT_ShowText(5,0,0x00,strbuff);
			

		//显示选中项
// 		sprintf(strbuff,"%0.3d",paramtemp[set_index]);
// 		TFT_ShowText((3+(set_index%3)*7)*MzL05LCD_X_Width,(set_index/3+1)*MzL05LCD_Y_Width,strbuff,0x80);
		TFT_SetCursor((set_index/3+1),(2+(set_index%3)*5),0x00);
		TFT_SetCursor((set_index/3+1),(3+(set_index%3)*5),0x00);
		TFT_SetCursor((set_index/3+1),(4+(set_index%3)*5),0x00);
		
		TFT_ShowText(7,0,0x00,"cancel");
		TFT_ShowText(7,10,0x00,"    ok");

		refresh = 0;
	}
	}
}
void DispClose(unsigned char bClose)
{
}


#elif defined	 DISP_TYPE_MzL05LCD	//如果使用MzL05LCD显示模块


extern unsigned char MzL05LCD_Asii0608[];			//6X8的ASII字符库
extern unsigned char MzL05LCD_Asii0816[];			//8X16的ASII字符库
extern unsigned char MzL05LCD_GB1616[];				//16*16自定义的汉字库

unsigned char MzL05LCD_X_Width;									//字符写入时的宽度
unsigned char MzL05LCD_Y_Width;									//字符写入时的高度
unsigned char MzL05LCD_Font_Wrod;								//字体的每个字模占用多少个存储单元数
unsigned char *MzL05LCD_Char_TAB;						//字库指针
char strbuff[22];

//初始化
void MzL05LCD_Init(void)
{
	//LCD驱动所使用到的端口的初始化（如果有必要的话）
	MzL05LCD_TimeDelay(200);
	GPIO_ResetBits(MzL05LCD_RES);
	MzL05LCD_TimeDelay(200);
	GPIO_SetBits(MzL05LCD_RES);
	MzL05LCD_TimeDelay(20);
		
	MzL05LCD_RegWrite(M_LCD_ON);							//LCD On
	MzL05LCD_RegWrite(M_LCD_POWER_ALL);					//设置上电控制模式
	
	MzL05LCD_RegWrite(M_LCD_ELE_VOL);					//电量设置模式（显示亮度）
	MzL05LCD_RegWrite(0x1f);								//指令数据0x0000~0x003f
	
	MzL05LCD_RegWrite(M_LCD_VDD_SET);					//V5内部电压调节电阻设置
	MzL05LCD_RegWrite(M_LCD_VDD);						//LCD偏压设置，V3时选
	
	MzL05LCD_RegWrite(M_LCD_COM_REV);					//Com 扫描方式设置
	MzL05LCD_RegWrite(M_LCD_SEG_NOR);					//Segment方向选择
	MzL05LCD_RegWrite(M_LCD_ALL_LOW);					//全屏点亮/变暗指令
	MzL05LCD_RegWrite(M_LCD_ALL_NOR);					//正向反向显示控制指令
	
	MzL05LCD_RegWrite(M_LCD_STATIC_OFF);					//关闭静态指示器
	MzL05LCD_RegWrite(0x00);								//指令数据
	
	MzL05LCD_RegWrite(M_LCD_BEGIN_LINE);				//设置显示起始行对应RAM
	MzL05LCD_Fill(LCD_INITIAL_COLOR);
}

void SPI_Config(void) 
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
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);	
	
	
		//配置B12 CS
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;				     
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //口线翻转速度为50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
		//配置B13 CLK
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				     
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //口线翻转速度为50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
		//配置B15 Data
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;				     
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //口线翻转速度为50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	//配置C6为 MzL05LCD的RST
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_13;				     
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //口线翻转速度为50MHz
  GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
}


//延时程序
void MzL05LCD_TimeDelay(int Time)
{
	int i;
	while(Time > 0)
	{
		for(i = 0;i < 800;i++)
		{
			__NOP();
		}
		Time --;
	}
}

//========================================================================
// 函数: void LCD_RegWrite(unsigned char Command)
// 描述: 写一个字节的数据至LCD中的控制寄存器当中
// 参数: Command		写入的数据，低八位有效（byte） 
// 返回: 无
// 备注: 
// 版本:
//      2007/01/09      First version
//========================================================================
void MzL05LCD_RegWrite(unsigned char Command)
{
	unsigned char Num;
	GPIO_ResetBits(MzL05LCD_CS);
// 	MzL05LCD_TimeDelay(2);
	GPIO_ResetBits(MzL05LCD_A0);
// 	MzL05LCD_TimeDelay(2);
	for(Num=0;Num<8;Num++)
	{
		
		if((Command&0x80) == 0)	GPIO_ResetBits(MzL05LCD_SDI);
		else GPIO_SetBits(MzL05LCD_SDI);
// 		MzL05LCD_TimeDelay(2);
		Command = Command << 1;
		GPIO_ResetBits(MzL05LCD_CLK);
// 		MzL05LCD_TimeDelay(2);
		GPIO_SetBits(MzL05LCD_CLK);
// 		MzL05LCD_TimeDelay(2);
	}
	GPIO_SetBits(MzL05LCD_CS);
}

//========================================================================
// 函数: void LCD_DataWrite(unsigned int Data)
// 描述: 写一个字节的显示数据至LCD中的显示缓冲RAM当中
// 参数: Data 写入的数据 
// 返回: 无
// 备注: 无
// 版本:
//      2007/01/09      First version
//========================================================================
void MzL05LCD_DataWrite(unsigned char Dat)//,_Fill_Dot_LCD
{
	unsigned char Num;
	GPIO_ResetBits(MzL05LCD_CS);
// 	MzL05LCD_TimeDelay(2);
	GPIO_SetBits(MzL05LCD_A0);
// 	MzL05LCD_TimeDelay(2);
	for(Num=0;Num<8;Num++)
	{
		if((Dat&0x80) == 0)	GPIO_ResetBits(MzL05LCD_SDI);
		else GPIO_SetBits(MzL05LCD_SDI);
// 		MzL05LCD_TimeDelay(2);
		Dat = Dat << 1;
		GPIO_ResetBits(MzL05LCD_CLK);
// 		MzL05LCD_TimeDelay(2);
		GPIO_SetBits(MzL05LCD_CLK);
// 		MzL05LCD_TimeDelay(2);
	}
	GPIO_SetBits(MzL05LCD_CS);
}

//========================================================================
// 函数: void LCD_Fill(unsigned int Data)
// 描述: 会屏填充以Data的数据至各点中
// 参数: Data   要填充的颜色数据
// 返回: 无
// 备注: 仅在LCD初始化程序当中调用
// 版本:
//      2006/10/15      First version
//		2007/01/09		V1.2 
//========================================================================
void MzL05LCD_Fill(unsigned char Data)
{
	unsigned char i,j;
	unsigned char uiTemp;
	uiTemp = Dis_Y_MAX;
	uiTemp = uiTemp>>3;
	for(i=0;i<=uiTemp;i++)								//往LCD中填充初始化的显示数据
	{
		MzL05LCD_RegWrite(0xb0+i);
		MzL05LCD_RegWrite(0x00);
		MzL05LCD_RegWrite(0x10);
		for(j=0;j<=Dis_X_MAX;j++)
		{
			MzL05LCD_DataWrite(Data);
		}
	}
}

//========================================================================
// 函数: void ClrScreen(unsigned int Mode)
// 描述: 清屏函数，执行全屏幕清除或填充前景色
// 参数: Mode   0:全屏除屏
//				1:全屏填充前景色
// 返回: 无
// 备注: 
// 版本:
//      2007/01/11      First version
//========================================================================
void MzL05LCD_ClrScreen(void)
{
	MzL05LCD_Fill(LCD_INITIAL_COLOR);
}

//========================================================================
// 函数: void FontSet(int Font_NUM,unsigned int Color)
// 描述: 文本字体设置
// 参数: Font_NUM 字体选择,以驱动所带的字库为准
//		 Color  文本颜色,仅作用于自带字库  
// 返回: 无
// 备注: 
// 版本:
//      2006/10/15      First version
//========================================================================
void MzL05LCD_FontSet(unsigned char Font_NUM)
{
	switch(Font_NUM)
	{
		case 0: MzL05LCD_Font_Wrod = 16;	//ASII字符A
				MzL05LCD_X_Width = 8;
				MzL05LCD_Y_Width = 2;
				//Char_Color = Color;
				MzL05LCD_Char_TAB = (unsigned char *)(MzL05LCD_Asii0816 - 32*16);
		break;
		case 1: MzL05LCD_Font_Wrod = 6;	//ASII字符B
				MzL05LCD_X_Width = 6;
				MzL05LCD_Y_Width = 1;
				//Char_Color = Color;
				MzL05LCD_Char_TAB = (unsigned char *)(MzL05LCD_Asii0608 - (32*6));
		break;		
		case 2: MzL05LCD_Font_Wrod = 32;	//汉字A
				MzL05LCD_X_Width = 16;
				MzL05LCD_Y_Width = 2;
				//Char_Color = Color;
				MzL05LCD_Char_TAB = (unsigned char *)MzL05LCD_GB1616;
		break;
		default: break;
	}
}

//========================================================================
// 函数: void PutChar(unsigned char x,unsigned char y,char a)  
// 描述: 写入一个标准字符
// 参数: x  X轴坐标     y  Y轴坐标(0~7页)
//		 a  要显示的字符在字库中的偏移量  
// 返回: 无
// 备注: ASCII字符可直接输入ASCII码即可
// 版本:
//      2006/10/15      First version
//		2007/05/17		V2.1  for seriea port LCD
//========================================================================
void MzL05LCD_PutChar(unsigned char x,unsigned char y,char a,unsigned char inv)       
{
	unsigned char i,j;
	unsigned char *p_data; 
	p_data = MzL05LCD_Char_TAB + a*MzL05LCD_Font_Wrod;	//要写字符的首地址
	x = x+1;							//Edit by xinqiang
	for(j=0;j<MzL05LCD_Y_Width;j++)
	{
		if((y+j) < (Dis_Y_MAX/8))
		MzL05LCD_RegWrite(y+j+0xb0);
		MzL05LCD_RegWrite(((x&0xf0) >> 4)|0x10);	//设置初始地址 
		MzL05LCD_RegWrite(x&0x0f);
		for(i=0;i<MzL05LCD_X_Width;i++)
		{  
			if(((x+i) < Dis_X_MAX)&&((y+j) < (Dis_Y_MAX/8)))
			MzL05LCD_DataWrite((*p_data++)|inv);  //当inv=0x80时,加下划线;当inv=0x01时,加上划线
// 				MzL05LCD_DataWrite((*p_data++)^inv); //反显，当inv=0xFF时
// 			MzL05LCD_DataWrite(*p_data++);
		}
	}
}

//========================================================================
// 函数: void PutString(unsigned char x,unsigned char y,char *p)
// 描述: 在x、y为起始坐标处写入一串标准字符
// 参数: x  X轴坐标     y  Y轴坐标(0~7页)
//		 p  要显示的字符串  inv 是否反显
// 返回: 无
// 备注: 仅能用于自带的ASCII字符串显示
// 版本:
//      2006/10/15      First version
//========================================================================
void MzL05LCD_PutString(unsigned char x,unsigned char y,char *p,unsigned char inv)
{
// 	MzL05LCD_RegWrite(M_LCD_BEGIN_LINE);				//设置显示起始行对应RAM
	while(*p!=0)
	{
		MzL05LCD_PutChar(x,y,*p,inv);
		x += MzL05LCD_X_Width;
		if((x + MzL05LCD_X_Width) > Dis_X_MAX)
		{
			x = 0;
			if((Dis_Y_MAX - y) < MzL05LCD_Y_Width) break;
			else y += MzL05LCD_Y_Width;
		}
		p++;
	}
}


//公用函数
void DispInit(void)
{
	SPI_Config(); //初始化端口
	MzL05LCD_Init();
	MzL05LCD_FontSet(1);				//选择8X16的ASCII字符
}
//显示调试信息，status将转换成十六进制显示
void ShowDebugInfo(uint8_t line,uint8_t col, uint8_t status)
{
	//char strbuff[3];
	sprintf(strbuff,"%0.2X",status);
	MzL05LCD_PutString(col*MzL05LCD_X_Width,line*MzL05LCD_Y_Width,strbuff,0x00);
}
//在第line行显示一个字符信息info
void ShowStatusInfo(uint8_t line, uint8_t col,char* info)
{
	MzL05LCD_PutString(col*MzL05LCD_X_Width,line*MzL05LCD_Y_Width,info,0x00);
}
//显示电池信息
void ShowBatteryInfo(uint8_t val)
{
	//char strbuff[11];
	sprintf(strbuff," Power:%0.3d%c",val,'%');
	MzL05LCD_PutString(0*MzL05LCD_X_Width,3*MzL05LCD_Y_Width,strbuff,0x00);
}
//显示当前日期时间
void ShowCurrentTime(void)
{
		//char strbuff[17];
		CalculateTime();
		sprintf(strbuff,"%0.4d-%0.2d-%0.2d %0.2d:%0.2d",s_DateStructVar.Year,s_DateStructVar.Month,s_DateStructVar.Day,
		s_TimeStructVar.HourHigh*10+s_TimeStructVar.HourLow, s_TimeStructVar.MinHigh*10+s_TimeStructVar.MinLow
		);
		MzL05LCD_PutString(2*MzL05LCD_X_Width,0*MzL05LCD_Y_Width,strbuff,0x00);
}
//显示默认页面
void ShowDefaultScreen(unsigned char cPower,unsigned char cWorkMode)
{
	ShowCurrentTime();
	ShowBatteryInfo(cPower);
	MzL05LCD_PutString(0*MzL05LCD_X_Width,7*MzL05LCD_Y_Width,"setting",0x00);
	switch(cWorkMode)
	{
		case 1:
			MzL05LCD_PutString(14*MzL05LCD_X_Width,7*MzL05LCD_Y_Width,"manual",0x00);
			break;
		case 2:
			MzL05LCD_PutString(14*MzL05LCD_X_Width,7*MzL05LCD_Y_Width,"  home",0x00);
			break;
		case 3:
			MzL05LCD_PutString(14*MzL05LCD_X_Width,7*MzL05LCD_Y_Width,"  loop",0x00);
			break;
		default:
			MzL05LCD_PutString(14*MzL05LCD_X_Width,7*MzL05LCD_Y_Width,"  auto",0x00);
			break;
	}
	
}
void SetCurrentDateTime(void)
{
	
	uint8_t set_index;
	uint8_t dttemp[5];

	uint8_t refresh = 1;
	
	dttemp[3] =s_TimeStructVar.HourHigh*10+s_TimeStructVar.HourLow;
	dttemp[4] = s_TimeStructVar.MinHigh*10+s_TimeStructVar.MinLow;
	
	dttemp[0] = s_DateStructVar.Year%100;
	dttemp[1] = s_DateStructVar.Month;
	dttemp[2] = s_DateStructVar.Day;
	DispClear();
	MzL05LCD_PutString(2*MzL05LCD_X_Width,0*MzL05LCD_Y_Width,"Date&Time Setting",0x00);
	
	set_index = 0;
		
	while(1)
	{
		IWDG_ReloadCounter(); //喂狗
		if(Key_Release(KEY_OK))  //保存设置的日期
		{
			
			SetDate(dttemp[2],dttemp[1],2000+dttemp[0]);
			SetTime(dttemp[3],dttemp[4],00);
			return;
			
		}
		if(Key_Release(KEY_SET)) //不保存返回
		{
			return;
		}

		if(Key_Press(KEY_RIGHT))
		{
			set_index++;
			if(set_index>4)set_index=0;
			refresh = 1;
		}
		if(Key_Press(KEY_LEFT))
		{
			if(set_index == 0)set_index=4;
			else set_index--;
			refresh = 1;
		}
		if(Key_Press(KEY_DOWN))
		{
			switch(set_index)
			{
				case 0:
					dttemp[0]--;
					if(dttemp[0]<12) dttemp[0] = 99;
					break;
				case 1:
					dttemp[1]-- ;
					if(dttemp[1] < 1) dttemp[1] = 12;
					break;
				case 2:
					dttemp[2]--;
					if(dttemp[2] < 1) dttemp[2] = 31;
					break;
				case 3:
					if(dttemp[3] <1) dttemp[3]  = 23;
					else dttemp[3] --;
					break;
				case 4:
					if(dttemp[4] <1) dttemp[4] =59;
					else dttemp[4] --;
					break;
				default:
					break;
			}
			refresh = 1;
		}
		if(Key_Press(KEY_UP))
		{
			switch(set_index)
			{
				case 0:
					dttemp[0]++;
					if(dttemp[0]>99) dttemp[0] = 12;
					break;
				case 1:
					dttemp[1]++;
					if(dttemp[1] >12) dttemp[1] = 1;
					break;
				case 2:
					dttemp[2]++;
					if(dttemp[2]>31) dttemp[2] = 1;
					break;
				case 3:
					dttemp[3]++;
					if(dttemp[3]>23) dttemp[3] = 0;
					 
					break;
				case 4:
					dttemp[4]++;
					if(dttemp[4]>59) dttemp[4]=0;
					 
					break;
				default:
					break;
			}
			refresh = 1;
		}

	if(refresh)
	{
		//显示日期
		sprintf(strbuff,"20%0.2d-%0.2d-%0.2d",dttemp[0],dttemp[1],dttemp[2]);
		MzL05LCD_PutString(5*MzL05LCD_X_Width,2*MzL05LCD_Y_Width,strbuff,0x00);
		
		//显示时间
		sprintf(strbuff,"%0.2d:%0.2d",dttemp[3],dttemp[4]);
		MzL05LCD_PutString(8*MzL05LCD_X_Width,4*MzL05LCD_Y_Width,strbuff,0x00);

		//显示当前光标位置
		sprintf(strbuff,"%0.2d",dttemp[set_index]);
		if(set_index<3)
		{
			MzL05LCD_PutString((7+set_index*3)*MzL05LCD_X_Width,2*MzL05LCD_Y_Width,strbuff,0x80);
		}
		else
		{
			MzL05LCD_PutString((8+(set_index-3)*3)*MzL05LCD_X_Width,4*MzL05LCD_Y_Width,strbuff,0x80);
		}
		
		MzL05LCD_PutString(0*MzL05LCD_X_Width,7*MzL05LCD_Y_Width,"cancel",0x00);
		MzL05LCD_PutString(14*MzL05LCD_X_Width,7*MzL05LCD_Y_Width,"    ok",0x00);

		refresh = 0;
	}
	}

}
void SetWorkingTime(void)
{
	unsigned char weekday,i;
	unsigned char set_index;
	unsigned char refresh = 1;
	
	struct R1_WorkTime_s s_WorkTimeTemp[7];
	
	for(i=0;i<7;i++)
	{
		s_WorkTimeTemp[i].StartHour = sR1Mower.sWorkTime[i].StartHour;
		s_WorkTimeTemp[i].StartMin = sR1Mower.sWorkTime[i].StartMin;
		s_WorkTimeTemp[i].EndHour = sR1Mower.sWorkTime[i].EndHour;
		s_WorkTimeTemp[i].EndMin = sR1Mower.sWorkTime[i].EndMin;
	}
	DispClear();
	MzL05LCD_PutString(0*MzL05LCD_X_Width,1*MzL05LCD_Y_Width,"Work",0x00);
	MzL05LCD_PutString(0*MzL05LCD_X_Width,3*MzL05LCD_Y_Width,"Time",0x00);
	MzL05LCD_PutString(0*MzL05LCD_X_Width,5*MzL05LCD_Y_Width,"Set",0x00);
	
	MzL05LCD_PutString(5*MzL05LCD_X_Width,0*MzL05LCD_Y_Width,"MON ",0x00);
	MzL05LCD_PutString(5*MzL05LCD_X_Width,1*MzL05LCD_Y_Width,"TUE ",0x00);
	MzL05LCD_PutString(5*MzL05LCD_X_Width,2*MzL05LCD_Y_Width,"WED ",0x00);
	MzL05LCD_PutString(5*MzL05LCD_X_Width,3*MzL05LCD_Y_Width,"THU ",0x00);
	MzL05LCD_PutString(5*MzL05LCD_X_Width,4*MzL05LCD_Y_Width,"FRI ",0x00);
	MzL05LCD_PutString(5*MzL05LCD_X_Width,5*MzL05LCD_Y_Width,"SAT ",0x00);
	MzL05LCD_PutString(5*MzL05LCD_X_Width,6*MzL05LCD_Y_Width,"SUN ",0x00);
	
	
	set_index = 0;
		

	while(1)
	{
		IWDG_ReloadCounter(); //喂狗
		if(Key_Release(KEY_OK))
		{
				for(i=0;i<7;i++)
				{
					sR1Mower.sWorkTime[i].StartHour = s_WorkTimeTemp[i].StartHour;
					sR1Mower.sWorkTime[i].StartMin = s_WorkTimeTemp[i].StartMin;
					sR1Mower.sWorkTime[i].EndHour = s_WorkTimeTemp[i].EndHour;
					sR1Mower.sWorkTime[i].EndMin = s_WorkTimeTemp[i].EndMin;
				}
				EE_SaveWorkingTime();
			return;
		}
		if(Key_Release(KEY_SET))
		{
			return;
		}

		if(Key_Press(KEY_RIGHT))
		{
			set_index++;
			if(set_index>27)set_index=0;
			refresh = 1;
		}
		if(Key_Press(KEY_LEFT))
		{
			if(set_index == 0)set_index=27;
			else set_index--;
			refresh = 1;
		}
		if(Key_Press(KEY_DOWN))
		{
			weekday = (int) set_index/4;
			switch(set_index%4)
			{
				case 0:
					if(s_WorkTimeTemp[weekday].StartHour<1) s_WorkTimeTemp[weekday].StartHour = 23;
					else s_WorkTimeTemp[weekday].StartHour--;

					break;
				case 1:
					if(s_WorkTimeTemp[weekday].StartMin<1) s_WorkTimeTemp[weekday].StartMin = 59;
					else s_WorkTimeTemp[weekday].StartMin--;
					break;
				case 2:
					if(s_WorkTimeTemp[weekday].EndHour<1) s_WorkTimeTemp[weekday].EndHour = 23;
					else s_WorkTimeTemp[weekday].EndHour--;
					break;
				case 3:
					if(s_WorkTimeTemp[weekday].EndMin<1) s_WorkTimeTemp[weekday].EndMin = 59;
					else s_WorkTimeTemp[weekday].EndMin--;
					break;
				default:
					break;
			}
			refresh = 1;
		}
		if(Key_Press(KEY_UP))
		{
			weekday = (int) set_index/4;
			switch(set_index%4)
			{
				case 0:
					s_WorkTimeTemp[weekday].StartHour++;
					if(s_WorkTimeTemp[weekday].StartHour>23) s_WorkTimeTemp[weekday].StartHour = 0;

					break;
				case 1:
					s_WorkTimeTemp[weekday].StartMin++;
					if(s_WorkTimeTemp[weekday].StartMin>59) s_WorkTimeTemp[weekday].StartMin = 0;
					break;
				case 2:
					s_WorkTimeTemp[weekday].EndHour++;
					if(s_WorkTimeTemp[weekday].EndHour>23) s_WorkTimeTemp[weekday].EndHour = 0;
					
					break;
				case 3:
					s_WorkTimeTemp[weekday].EndMin++;
					if(s_WorkTimeTemp[weekday].EndMin>23) s_WorkTimeTemp[weekday].EndMin = 0;
					break;
				default:
					break;
			}
			refresh = 1;
		}

	if(refresh)
	{

		for(i=0;i<7;i++)
		{
			sprintf(strbuff,"%0.2d:%0.2d-%0.2d:%0.2d",s_WorkTimeTemp[i].StartHour,s_WorkTimeTemp[i].StartMin,s_WorkTimeTemp[i].EndHour,s_WorkTimeTemp[i].EndMin);
			
			MzL05LCD_PutString(9*MzL05LCD_X_Width,i*MzL05LCD_Y_Width,strbuff,0x00);
		}
		
		MzL05LCD_PutString(0*MzL05LCD_X_Width,7*MzL05LCD_Y_Width,"cancel",0x00);
		MzL05LCD_PutString(14*MzL05LCD_X_Width,7*MzL05LCD_Y_Width,"    ok",0x00);
		
		i = (int) set_index/4;
		switch(set_index%4)
		{
			case 0:
				sprintf(strbuff,"%0.2d",s_WorkTimeTemp[i].StartHour);
				MzL05LCD_PutString(9*MzL05LCD_X_Width,i*MzL05LCD_Y_Width,strbuff,0x80);
				break;
			case 1:
				sprintf(strbuff,"%0.2d",s_WorkTimeTemp[i].StartMin );
				MzL05LCD_PutString(12*MzL05LCD_X_Width,i*MzL05LCD_Y_Width,strbuff,0x80);
				break;
			case 2:
				sprintf(strbuff,"%0.2d",s_WorkTimeTemp[i].EndHour);
				MzL05LCD_PutString(15*MzL05LCD_X_Width,i*MzL05LCD_Y_Width,strbuff,0x80);
				break;
			case 3:
				sprintf(strbuff,"%0.2d",s_WorkTimeTemp[i].EndMin);
				MzL05LCD_PutString(18*MzL05LCD_X_Width,i*MzL05LCD_Y_Width,strbuff,0x80);
				break;
			default:
				break;
			
		}
	refresh = 0;
		}
	}
}
void SetParam(void)
{
	uint8_t set_index;
	uint8_t paramtemp[16];

	uint8_t refresh = 1;
	
// 	LoadParams();
	
	paramtemp[0] = sR1Mower.sRunParam.DrvSpeedH;  //驱动电机速度高
	paramtemp[1] = sR1Mower.sRunParam.DrvSpeedM ;//驱动电机速度中
	paramtemp[2] = sR1Mower.sRunParam.DrvSpeedL;//驱动电机速度低
	
	paramtemp[3] = sR1Mower.sRunParam.BsStrenghH ;//边界信号强度高
	paramtemp[4] = sR1Mower.sRunParam.BsStrenghM;  //边界信号强度中
	paramtemp[5] = sR1Mower.sRunParam.BsStrenghL;//边界信号强度低
	
	
	paramtemp[6] = sR1Mower.sRunParam.KP1*100;//kp1
	paramtemp[7] = sR1Mower.sRunParam.KD1*100;//kd1
	paramtemp[8] = sR1Mower.sRunParam.KI1*100;//ki1
	
	paramtemp[9] = sR1Mower.sRunParam.KP2*100;//kp2
	paramtemp[10] = sR1Mower.sRunParam.KD2*100;//kd2
	paramtemp[11] = sR1Mower.sRunParam.KI2*100;//ki2
	
	paramtemp[12] = sR1Mower.sRunParam.BladeSpeed; //割草电机速度
	paramtemp[13] = sR1Mower.sRunParam.PowerLimit ;//电池剩余电量限值
	
	paramtemp[14] = sR1Mower.sRunParam.BehaviorTO;
	paramtemp[15] = sR1Mower.sRunParam.MotionSense/32;
	
	DispClear();
	MzL05LCD_PutString(4*MzL05LCD_X_Width,0*MzL05LCD_Y_Width,"Configuration",0x00);
	
	set_index = 0;
		
	while(1)
	{
		IWDG_ReloadCounter(); //喂狗
		if(Key_Release(KEY_OK))  //保存设置
		{
			sR1Mower.sRunParam.DrvSpeedH = paramtemp[0];  //驱动电机速度高
			sR1Mower.sRunParam.DrvSpeedM = paramtemp[1] ;//驱动电机速度中
			sR1Mower.sRunParam.DrvSpeedL = paramtemp[2] ;//驱动电机速度低
			
			sR1Mower.sRunParam.BsStrenghH = paramtemp[3];//边界信号强度高
			sR1Mower.sRunParam.BsStrenghM = paramtemp[4];  //边界信号强度中
			sR1Mower.sRunParam.BsStrenghL = paramtemp[5];//边界信号强度低
			
			
			sR1Mower.sRunParam.KP1 = paramtemp[6]/100.0;//kp1
			sR1Mower.sRunParam.KD1 = paramtemp[7]/100.0;//kd1
			sR1Mower.sRunParam.KI1 = paramtemp[8]/100.0;//ki1
			
			sR1Mower.sRunParam.KP2 = paramtemp[9]/100.0 ;//kp2
			sR1Mower.sRunParam.KD2 = paramtemp[10]/100.0;//kd2
			sR1Mower.sRunParam.KI2 = paramtemp[11]/100.0 ;//ki2
			
			sR1Mower.sRunParam.BladeSpeed = paramtemp[12]; //割草电机速度
			sR1Mower.sRunParam.PowerLimit = paramtemp[13] ;//电池剩余电量限值
			
			sR1Mower.sRunParam.BehaviorTO = paramtemp[14];
			sR1Mower.sRunParam.MotionSense = paramtemp[15] * 32;
			
			EE_SaveParams();
			return;
			
		}
		if(Key_Release(KEY_SET)) //不保存返回
		{
			return;
		}

		if(Key_Press(KEY_RIGHT))
		{
			set_index++;
			if(set_index>15)set_index=0;
			refresh = 1;
		}
		if(Key_Press(KEY_LEFT))
		{
			if(set_index == 0)set_index=15;
			else set_index--;
			refresh = 1;
		}
		if(Key_Press(KEY_DOWN))
		{
			switch(set_index)
			{
				case 0:
					paramtemp[0]--;
					if(paramtemp[0]<paramtemp[1]) paramtemp[0] = 60;
					break;
				case 1:
					paramtemp[1]-- ;
					if(paramtemp[1] < paramtemp[2]) paramtemp[1] = paramtemp[0];
					break;
				case 2:
					paramtemp[2]--;
					if(paramtemp[2] < 1) paramtemp[2] = paramtemp[1];
					break;
				case 3:
					paramtemp[3]--;
					if(paramtemp[3] < paramtemp[4]) paramtemp[3] = 100;
					break;
				case 4:
					paramtemp[4]--;
					if(paramtemp[4] < paramtemp[5]) paramtemp[4] = paramtemp[3];
					break;
				case 5:
					paramtemp[5]--;
					if(paramtemp[5] < 1) paramtemp[5] = paramtemp[4];
					break;
				case 12:
					if(paramtemp[12] == 0) paramtemp[12] = 127;
					else paramtemp[12]--;
					break;
				case 13:
					if(paramtemp[13] == 0) paramtemp[13] = 100;
					else paramtemp[13]--;
					break;
				case 14:
					if(paramtemp[14] == 0) paramtemp[14] = 200;
					else paramtemp[14]--;
					break;
				case 15:
					if(paramtemp[15] == 0) paramtemp[13] = 100;
					else paramtemp[15]--;
					break;
				default:
					paramtemp[set_index]--;
					break;
			}
			refresh = 1;
		}
		if(Key_Press(KEY_UP))
		{
			switch(set_index)
			{
				case 0:
					paramtemp[0]++;
					if(paramtemp[0]>60) paramtemp[0] = paramtemp[1];
					break;
				case 1:
					paramtemp[1]++ ;
					if(paramtemp[1] > paramtemp[0]) paramtemp[1] = paramtemp[2];
					break;
				case 2:
					paramtemp[2]++;
					if(paramtemp[2] > paramtemp[1]) paramtemp[2] = 1;
					break;
				case 3:
					paramtemp[3]++;
					if(paramtemp[3] >100 ) paramtemp[3] = paramtemp[4];
					break;
				case 4:
					paramtemp[4]++;
					if(paramtemp[4] > paramtemp[3]) paramtemp[4] = paramtemp[5];
					break;
				case 5:
					paramtemp[5]++;
					if(paramtemp[5] > paramtemp[4]) paramtemp[5] = 1;
					break;
				case 12:
					paramtemp[12]++;
					if(paramtemp[12] > 127 ) paramtemp[12] = 0;
					break;
				case 13:
					paramtemp[13]++;
					if(paramtemp[13] >100) paramtemp[13] = 0;
					break;
				case 14:
					paramtemp[14]++;
					if(paramtemp[14] >200) paramtemp[14] = 0;
					break;
				case 15:
					paramtemp[15]++;
					if(paramtemp[15] >100) paramtemp[15] = 0;
					break;
				default:
					paramtemp[set_index]++;
					break;
			}
			refresh = 1;
		}

	if(refresh)
	{
		//驱动电机速度
		sprintf(strbuff,"SH:%0.3d SM:%0.3d SL:%0.3d",paramtemp[0],paramtemp[1],paramtemp[2]);
		MzL05LCD_PutString(0*MzL05LCD_X_Width,1*MzL05LCD_Y_Width,strbuff,0x00);
		
		//显示边界强度
		sprintf(strbuff,"BH:%0.3d BM:%0.3d BL:%0.3d",paramtemp[3],paramtemp[4],paramtemp[5]);
		MzL05LCD_PutString(0*MzL05LCD_X_Width,2*MzL05LCD_Y_Width,strbuff,0x00);


				//显示kp1 kd1 ki1
		sprintf(strbuff,"P1:%0.3d D1:%0.3d I1:%0.3d",paramtemp[6],paramtemp[7],paramtemp[8]);
		MzL05LCD_PutString(0*MzL05LCD_X_Width,3*MzL05LCD_Y_Width,strbuff,0x00);
		
				//显示kp2 kd2 ki2
		sprintf(strbuff,"P2:%0.3d D2:%0.3d I2:%0.3d",paramtemp[9],paramtemp[10],paramtemp[11]);
		MzL05LCD_PutString(0*MzL05LCD_X_Width,4*MzL05LCD_Y_Width,strbuff,0x00);

		//显示割草电机速度和电量限值
		sprintf(strbuff,"MS:%0.3d PL:%0.3d TO:%0.3d",paramtemp[12],paramtemp[13],paramtemp[14]);
		MzL05LCD_PutString(0*MzL05LCD_X_Width,5*MzL05LCD_Y_Width,strbuff,0x00);
		
		sprintf(strbuff,"CS:%0.3d",paramtemp[15]);
		MzL05LCD_PutString(0*MzL05LCD_X_Width,6*MzL05LCD_Y_Width,strbuff,0x00);
			

		//显示选中项
		sprintf(strbuff,"%0.3d",paramtemp[set_index]);
		MzL05LCD_PutString((3+(set_index%3)*7)*MzL05LCD_X_Width,(set_index/3+1)*MzL05LCD_Y_Width,strbuff,0x80);
		
		MzL05LCD_PutString(0*MzL05LCD_X_Width,7*MzL05LCD_Y_Width,"cancel",0x00);
		MzL05LCD_PutString(14*MzL05LCD_X_Width,7*MzL05LCD_Y_Width,"    ok",0x00);

		refresh = 0;
	}
	}
}
void DispClear(void)
{
	MzL05LCD_Fill(LCD_INITIAL_COLOR);
}

void DispClose(unsigned char bClose)
{
	
	if(!bClose)
	{
		//MzL05LCD_Init();
	}
	//MzL05LCD_RegWrite(0xAE | (bClose & 0x01));
}

#elif defined	 DISP_TYPE_ST7920	//如果使用ST7920显示模块

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
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				     
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //口线翻转速度为50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);	

	
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

//公用函数
void DispInit(void)
{
	SPI2_Config(); //初始化端口
	LCD_Config();
}
//显示调试信息，status将转换成十六进制显示
void ShowDebugInfo(uint8_t line,uint8_t col, uint8_t status)
{


}
//在第line行显示一个字符信息info
void ShowStatusInfo(uint8_t line, uint8_t col,char* info)
{

}
//显示电池信息
void ShowBatteryInfo(uint8_t val)
{


}
//显示当前日期时间
void ShowCurrentTime(void)
{

}
//显示默认页面
void ShowDefaultScreen(unsigned char cPower,unsigned char cWorkMode)
{

	
}
void SetCurrentDateTime(void)
{
}
void SetWorkingTime(void)
{
}

void DispClear(void)
{
}


#endif
