#include "DISP.h"
#include "stdio.h"
#include "string.h"
#include "key.h"
#include "clock_calendar.h"
// #include "R1_MCU.h"
#include "eeprom.h"
#include "R1DEF.h"
//extern struct Param_s sParam;

#ifdef DISP_TYPE_TFT   //���ʹ��TFT��ʾģ��

extern unsigned char ASCII1832[];


char strbuff[18];
/****************************************************************************
* ��    �ƣ�void lcd_DrawPicture(u16 StartX,u16 StartY, u8 Dir��u8 *pic)
* ��    �ܣ���ָ�����귶Χ��ʾһ��ͼƬ
* ��ڲ�����StartX     ����ʼ����
*           StartY     ����ʼ����
*			Dir		   ͼ����ʾ����       
*           pic        ͼƬͷָ��
* ���ڲ�������
* ˵    ����ͼƬȡģ��ʽΪˮƽɨ�裬16λ��ɫģʽ  ȡģ���img2LCD
* ���÷�����lcd_DrawPicture(0,0,0,(u16*)demo);
****************************************************************************/
void lcd_DrawPicture(u16 StartX,u16 StartY,u8 Dir,u8 *pic)
{
  u32  i=8, len;
  u16 temp,x,y;
  	  
  
  x=((uint16_t)(pic[2]<<8)+pic[3])-1;	  	//��ͼ��������ȡ��ͼ��ĳ���
  y=((uint16_t)(pic[4]<<8)+pic[5])-1;	  	//��ͼ��������ȡ��ͼ��ĸ߶�
  if(Dir==0){
	LCD_WR_CMD(0x0003,0x1030);   			//ͼ����ʾ����Ϊ������  �е���  �еݼ�
    LCD_WR_CMD(0x0210, StartX); 			//ˮƽ��ʾ����ʼ��ַ 0-239
  	LCD_WR_CMD(0x0211, StartX+x);           	//ˮƽ��ʾ��������ַ 0-239
  	LCD_WR_CMD(0x0212, StartY);     		//��ֱ��ʾ����ʼ��ַ 0-399
  	LCD_WR_CMD(0x0213, StartY+y);         	//��ֱ��ʾ��������ַ 0-399
  
  	LCD_WR_CMD(0x0200, StartX);		          	//ˮƽ��ʾ����ַ
  	LCD_WR_CMD(0x0201, StartY);		      		//��ֱ��ʾ����ַ
  }	 
  else if(Dir==1){
	LCD_WR_CMD(0x0003,0x1018);   			//ͼ����ʾ����Ϊ������  �е���  �еݼ�
    LCD_WR_CMD(0x0210, StartY); 			//ˮƽ��ʾ����ʼ��ַ 0-239
  	LCD_WR_CMD(0x0211, StartY+y);           //ˮƽ��ʾ��������ַ 0-239
  	LCD_WR_CMD(0x0212, 399-(x+StartX));     //��ֱ��ʾ����ʼ��ַ 0-399
  	LCD_WR_CMD(0x0213, 399-StartX);         //��ֱ��ʾ��������ַ 0-399
  
  	LCD_WR_CMD(0x200, StartY);		          	//ˮƽ��ʾ����ַ
  	LCD_WR_CMD(0x201, 399-StartX);		      	//��ֱ��ʾ����ַ
  }	 
  LCD_WR_REG(0x0202);				          	//д���ݵ���ʾ��

  len=2*((uint16_t)(pic[2]<<8)+pic[3])*((uint16_t)(pic[4]<<8)+pic[5]);   //�����ͼ����ռ���ֽ���  
  while(i<(len+8)) {							 //��ͼ������ĵ�9λ��ʼ����
  	temp=(uint16_t)( pic[i]<<8)+pic[i+1];		 //16λ���ߣ� ��Ҫһ�η���2���ֽڵ�����
  	LCD_WR_Data(temp);							 //��ȡ����16λ��������������ʾ��
	i=i+2;										 //ȡģλ�ü�2����Ϊ��ȡ��һ����������
  }
}

//д�Ĵ�����ַ����
void LCD_WR_REG(unsigned int index)
{
	*(__IO uint16_t *) (Bank1_LCD_C)= index;  

}
//д�Ĵ������ݺ���
//���룺dbw ����λ����1Ϊ16λ��0Ϊ8λ��
void LCD_WR_CMD(unsigned int index,unsigned int val)
{	
	*(__IO uint16_t *) (Bank1_LCD_C)= index;	
	*(__IO uint16_t *) (Bank1_LCD_D)= val;
}

//����ʾ��ַ����
unsigned int LCD_RD_data(void){
	unsigned int a=0;
	a=*(__IO uint16_t *) (Bank1_LCD_D);   //�ղ���
	a=*(__IO uint16_t *) (Bank1_LCD_D);   //������ʵ��16λ��������	  
	return(a);	
}
/****************************************************************************
* ��    �ƣ�u16 ili9320_BGR2RGB(u16 c)
* ��    �ܣ�RRRRRGGGGGGBBBBB ��Ϊ BBBBBGGGGGGRRRRR ��ʽ
* ��ڲ�����c      BRG ��ɫֵ
* ���ڲ�����RGB ��ɫֵ
* ˵    �����ڲ���������
* ���÷�����
****************************************************************************/
u16 lcd_BGR2RGB(u16 c)
{
  u16  r, g, b;

  b = (c>>0)  & 0x1f;
  g = (c>>5)  & 0x3f;
  r = (c>>11) & 0x1f;
  
  return( (b<<11) + (g<<5) + (r<<0) );
}

//д16λ�������ݺ���
void    LCD_WR_Data(unsigned int val)
{   
	*(__IO uint16_t *) (Bank1_LCD_D)= val; 	
}



/****************************************************************************
* ��    �ƣ�void LCD_Init(void)
* ��    �ܣ�3 ��TFT(R61509) ��ʼ������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
void TFT_Init(void)
{
	unsigned int i;
	GPIO_ResetBits(GPIOE, GPIO_Pin_1);	  //Ӳ����λ
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
	*(__IO uint16_t *) (Bank1_LCD_C)= 0x202;	 //׼��д������ʾ��
	for(i=0;i<96000;i++)
	{
	  LCD_WR_Data(0xFFFF);					 //�ú�ɫ����
	}  
}



/****************************************************************************
* ��    �ƣ�lcd_wr_zf(u16 StartX, u16 StartY, u16 X, u16 Y, u16 Color, u8 Dir, u8 *chr) 
* ��    �ܣ���ָ��������ʾһ���ַ�͸�������ڱ���ͼƬ��
* ��ڲ�����StartX     ����ʼ����	 0-239
*           StartY     ����ʼ����	 0-399
*           X          ��(Ϊ8�ı�����0-400
*           Y          ��			 0-240
*			Color      ��ɫ0-65535
*           Dir		   ͼ����ʾ����
*           chr        �ַ���ָ��
* ���ڲ�������
* ˵    �����ַ�ȡģ��ʽΪ��ɫ��ģ������ȡģ���ֽ�����  ȡģ�����ZIMO3
* ���÷�����lcd_wr_zf(0,0,100,100,(u16*)demo);
****************************************************************************/
//+++++++++++++++++++++++LCDд�ַ��ӳ���
void lcd_wr_zf(u16 StartX, u16 StartY, u16 X, u16 Y, u16 Color, u8 Dir, u8 *chr)
{	unsigned int temp=0,num,R_dis_mem=0,Size=0,x=0,y=0,i=0;


	if(Dir==2) LCD_WR_CMD(0x0003,0x1010);   //ͼ����ʾ����Ϊ������  �еݼ�  �е���  AM=0  I/D[1:0]=00	<--
	else if(Dir==3) LCD_WR_CMD(0x0003,0x1028);   //ͼ����ʾ����Ϊ������  �еݼ�  �е���  AM=1  I/D[1:0]=10	V
  	if(Dir==0){
		LCD_WR_CMD(0x0003,0x1030);		  //ͼ����ʾ����Ϊ������  �е���  �е���  AM=0  I/D[1:0]=11  -->
		LCD_WR_CMD(0x0210, StartX); 	  //ˮƽ��ʾ����ʼ��ַ 0-239
  		LCD_WR_CMD(0x0211, StartX+X-1);   //ˮƽ��ʾ��������ַ 0-239
  		LCD_WR_CMD(0x0212, StartY);       //��ֱ��ʾ����ʼ��ַ 0-399
  		LCD_WR_CMD(0x0213, StartY+Y-1);   //��ֱ��ʾ��������ַ 0-399
		LCD_WR_CMD(0x0200, StartX);		  //ˮƽ��ʾ����ַ
  		LCD_WR_CMD(0x0201, StartY);		  //��ֱ��ʾ����ַ	
		LCD_WR_REG(0x0202);               //׼��д������ʾ��
		Size=X*Y;						  //�ַ������ַ�ռ�õ����سߴ�
		while(i<Size){					  
			temp=*chr++;				  //һ���ֽڴ���8�����أ���˼�1������������8������
			for(num=0; num<8; num++){	  //�����ÿ���ֽڴ�����8������	   
											
				if((temp&0x80)>0){		  //���ֽڵĸ�λ�����жϣ�Ϊ1���ô��������16λ��ɫֵ��ʾ��д�뵽����λ�á�						 
				
					LCD_WR_Data(Color); 		
				}
				else{
					LCD_WR_CMD(0x0200, StartX+x);		//ˮƽ��ʾ����ַ
  					LCD_WR_CMD(0x0201, StartY+y);		//��ֱ��ʾ����ַ								
					LCD_WR_REG(0x0202);					//׼����������ʾ��
					R_dis_mem=LCD_RD_data();		  	//��ȡ����ɫ��Ϊ���Ӳ���͸��Ч����׼��	
					LCD_WR_Data(R_dis_mem); 		//���ֽڵĸ�λ�����жϣ�Ϊ0���õ�ǰ��������16λ��ɫֵ��ʾ�� 						
				}
				temp=temp<<1; 		  				//�ֽڸ�λ���Ƴ�
				x++;
				if(x>=X){x=0; y++;}				    //�������ص���Ϊ��ǰ��x��y��Ϊ��ǰ���ض�������ɫ��׼��
				i++;	
			}				  
							
		}
	}
	else if(Dir==1){
		LCD_WR_CMD(0x0003,0x1018);   	  		//ͼ����ʾ����Ϊ������  �е���  �еݼ�  AM=1  I/D[1:0]=01	A
		LCD_WR_CMD(0x0210, StartY); 	  		//ˮƽ��ʾ����ʼ��ַ 0-239
  		LCD_WR_CMD(0x0211, StartY+Y-1);    		//ˮƽ��ʾ��������ַ 0-239
  		LCD_WR_CMD(0x0212, 399-(StartX+X-1));   //��ֱ��ʾ����ʼ��ַ 0-399
  		LCD_WR_CMD(0x0213, 399-StartX);    		//��ֱ��ʾ��������ַ 0-399
		LCD_WR_CMD(0x0200, StartY);		  	  	//ˮƽ��ʾ����ַ
  		LCD_WR_CMD(0x0201, 399-StartX);	 	  	//��ֱ��ʾ����ַ	
		LCD_WR_REG(0x0202);                   	//׼��д������ʾ��

		Size=X*Y;						  		//�ַ������ַ�ռ�õ����سߴ�
		while(i<Size){					  
			temp=*chr++;				  		//һ���ֽڴ���8�����أ���˼�1������������8������
			for(num=0; num<8; num++){	  		//�����ÿ���ֽڴ�����8������	    
				if((temp&0x80)>0){		  		//���ֽڵĸ�λ�����жϣ�Ϊ1���ô��������16λ��ɫֵ��ʾ��д�뵽����λ�á�						 
				
					LCD_WR_Data(Color); 		
				}
				else{
// 					LCD_WR_CMD(0x0200, StartY+y);		//ˮƽ��ʾ����ַ
//   					LCD_WR_CMD(0x0201, 399-(StartX+x));	//��ֱ��ʾ����ַ								
// 					LCD_WR_REG(0x0202);					//׼����������ʾ��
// 					R_dis_mem=LCD_RD_data();		  	//��ȡ����ɫ��Ϊ���Ӳ���͸��Ч����׼��	
// 					LCD_WR_Data(R_dis_mem); 		//���ֽڵĸ�λ�����жϣ�Ϊ0���õ�ǰ��������16λ��ɫֵ��ʾ�� 
						LCD_WR_Data(0xffff);					
				}
				temp=temp<<1; 		  				//�ֽڸ�λ���Ƴ�
				x++;
				if(x>=X){x=0; y++;}				    //�������ص���Ϊ��ǰ��x��y��Ϊ��ǰ���ض�������ɫ��׼��
				i++;	
			}							
		}
	}
}


//++++++++������ֵ��ȡ��λ�������ַ������׵�ַ+++++++++++++++++++++++++++++++++++++++++++
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
// ++++++++++++++++TFT ��λ����
void lcd_rst(void){
	GPIO_ResetBits(GPIOE, GPIO_Pin_1);	      //PE1 ΪLCD ��λ�ź�
    Delay(0xAFFFFf);					   
    GPIO_SetBits(GPIOE, GPIO_Pin_1 );		 	 
	Delay(0xAFFFFf);	
}

void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}
//////////FSMC �ӿ�����///////////////////////////////////////////
void FSMC_LCD_Init(void)
{
  FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef  p;	
  GPIO_InitTypeDef GPIO_InitStructure;	    
  //ʹ��FSMC����ʱ��
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);   
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC |
                         RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE , ENABLE);  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; 			  //LCD�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ; 	 		  //LCD��λ
  GPIO_Init(GPIOE, &GPIO_InitStructure);   	   	
  // ���ö˿�ΪFSMC�ӿ� FSMC-D0--D15
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
  //FSMC NE1  LCDƬѡ
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  //FSMC RS---LCDָ�� ָ��/����	�л�
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ; 
  GPIO_Init(GPIOD, &GPIO_InitStructure); 	
  GPIO_SetBits(GPIOD, GPIO_Pin_13);			           //LCD�����
  
  
  //FSMC�ӿ���������
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

		LCD_WR_CMD(0x0003,0x1018);   	  		//ͼ����ʾ����Ϊ������  �е���  �еݼ�  AM=1  I/D[1:0]=01	A
		LCD_WR_CMD(0x0210, StartY); 	  		//ˮƽ��ʾ����ʼ��ַ 0-239
  	LCD_WR_CMD(0x0211, StartY+Y-1);    		//ˮƽ��ʾ��������ַ 0-239
  	LCD_WR_CMD(0x0212, 399-(StartX+X-1));   //��ֱ��ʾ����ʼ��ַ 0-399
  	LCD_WR_CMD(0x0213, 399-StartX);    		//��ֱ��ʾ��������ַ 0-399
		LCD_WR_CMD(0x0200, StartY);		  	  	//ˮƽ��ʾ����ַ
  	LCD_WR_CMD(0x0201, 399-StartX);	 	  	//��ֱ��ʾ����ַ	
		LCD_WR_REG(0x0202);                   	//׼��д������ʾ��

		Size=X*Y;						  		//�ַ������ַ�ռ�õ����سߴ�
		while(i<Size){		
		char_index = (chr-0x20)*96+i/8;			
		temp=ASCII1832[char_index];				  		//һ���ֽڴ���8�����أ���˼�1������������8������
		
			for(num=0; num<8; num++){	  		//�����ÿ���ֽڴ�����8������	    
				if((temp&0x80)>0){		  		//���ֽڵĸ�λ�����жϣ�Ϊ1���ô��������16λ��ɫֵ��ʾ��д�뵽����λ�á�						 
					LCD_WR_Data(Color); 		
				}
				else{
				if(Trans)
				{
					LCD_WR_CMD(0x0200, StartY+y);		//ˮƽ��ʾ����ַ
 					LCD_WR_CMD(0x0201, 399-(StartX+x));	//��ֱ��ʾ����ַ								
					LCD_WR_REG(0x0202);					//׼����������ʾ��
					R_dis_mem=LCD_RD_data();		  	//��ȡ����ɫ��Ϊ���Ӳ���͸��Ч����׼��	
					LCD_WR_Data(R_dis_mem); 		//���ֽڵĸ�λ�����жϣ�Ϊ0���õ�ǰ��������16λ��ɫֵ��ʾ�� 
				}
				else
				{
					LCD_WR_Data(0xffff); 	
				}
				}
				temp=temp<<1; 		  				//�ֽڸ�λ���Ƴ�
				x++;
				if(x>=X){x=0; y++;}				    //�������ص���Ϊ��ǰ��x��y��Ϊ��ǰ���ض�������ɫ��׼��
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


//���ú���
void DispInit(void)
{
	FSMC_LCD_Init();							 //FSMC��������		  
  TFT_Init();								 //Һ����ʼ��		
}

//��ʾ������Ϣ��status��ת����ʮ��������ʾ
void ShowDebugInfo(uint8_t line,uint8_t col, uint8_t status)
{
// 	char strbuff[2];
	sprintf(strbuff,"%0.2X",status);
	TFT_ShowText(line,col,0xff,strbuff);
}

//�ڵ�line����ʾһ���ַ���Ϣinfo
void ShowStatusInfo(uint8_t line,uint8_t col,char* info)
{
	TFT_ShowText(line,col,0x00,info);
}

//��ʾ�����Ϣ
void ShowBatteryInfo(uint8_t val)
{
// 	char strbuff[10];
	sprintf(strbuff," Power:%0.3d%c",val,'%');
	TFT_ShowText(3,0,0x00,strbuff);
}

//��ʾ��ǰ����ʱ��
void ShowCurrentTime(void)
{
		CalculateTime();
		sprintf(strbuff,"%0.4d-%0.2d-%0.2d %0.2d:%0.2d",s_DateStructVar.Year,s_DateStructVar.Month,s_DateStructVar.Day,
		s_TimeStructVar.HourHigh*10+s_TimeStructVar.HourLow, s_TimeStructVar.MinHigh*10+s_TimeStructVar.MinLow
		);
		
		TFT_ShowText(0,0,0x00,strbuff);
}

//��ʾĬ��ҳ��
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

//���õ�ǰ����ʱ��
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
		IWDG_ReloadCounter(); //ι��
		if(Key_Release(KEY_OK))  //�������õ�����
		{
			
			SetDate(s_DateStructTemp.Day,s_DateStructTemp.Month,s_DateStructTemp.Year);
			SetTime(HourTemp,MinTemp,00);
// 			TFT_Init();
			return;
			
		}
		if(Key_Release(KEY_SET)) //�����淵��
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


//���ù���ʱ��
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
		IWDG_ReloadCounter(); //ι��
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
	
	paramtemp[0] = sR1Mower.sRunParam.DrvSpeedH;  //��������ٶȸ�
	paramtemp[1] = sR1Mower.sRunParam.DrvSpeedM ;//��������ٶ���
	paramtemp[2] = sR1Mower.sRunParam.DrvSpeedL;//��������ٶȵ�
	
	paramtemp[3] = sR1Mower.sRunParam.BsStrenghH ;//�߽��ź�ǿ�ȸ�
	paramtemp[4] = sR1Mower.sRunParam.BsStrenghM;  //�߽��ź�ǿ����
	paramtemp[5] = sR1Mower.sRunParam.BsStrenghL;//�߽��ź�ǿ�ȵ�
	
	
	paramtemp[6] = sR1Mower.sRunParam.KP1*100;//kp1
	paramtemp[7] = sR1Mower.sRunParam.KD1*100;//kd1
	paramtemp[8] = sR1Mower.sRunParam.KI1*100;//ki1
	
	paramtemp[9] = sR1Mower.sRunParam.KP2*100;//kp2
	paramtemp[10] = sR1Mower.sRunParam.KD2*100;//kd2
	paramtemp[11] = sR1Mower.sRunParam.KI2*100;//ki2
	
	paramtemp[12] = sR1Mower.sRunParam.MowSpeed; //��ݵ���ٶ�
	paramtemp[13] = sR1Mower.sRunParam.PowerLimit ;//���ʣ�������ֵ
	
	DispClear();
	TFT_ShowText(0,2,0x00,"Configuration");
	
	set_index = 0;
		
	while(1)
	{
		IWDG_ReloadCounter(); //ι��
		if(Key_Release(KEY_OK))  //��������
		{
			sR1Mower.sRunParam.DrvSpeedH = paramtemp[0];  //��������ٶȸ�
			sR1Mower.sRunParam.DrvSpeedM = paramtemp[1] ;//��������ٶ���
			sR1Mower.sRunParam.DrvSpeedL = paramtemp[2] ;//��������ٶȵ�
			
			sR1Mower.sRunParam.BsStrenghH = paramtemp[3];//�߽��ź�ǿ�ȸ�
			sR1Mower.sRunParam.BsStrenghM = paramtemp[4];  //�߽��ź�ǿ����
			sR1Mower.sRunParam.BsStrenghL = paramtemp[5];//�߽��ź�ǿ�ȵ�
			
			
			sR1Mower.sRunParam.KP1 = paramtemp[6]/100.0;//kp1
			sR1Mower.sRunParam.KD1 = paramtemp[7]/100.0;//kd1
			sR1Mower.sRunParam.KI1 = paramtemp[8]/100.0;//ki1
			
			sR1Mower.sRunParam.KP2 = paramtemp[9]/100.0 ;//kp2
			sR1Mower.sRunParam.KD2 = paramtemp[10]/100.0;//kd2
			sR1Mower.sRunParam.KI2 = paramtemp[11]/100.0 ;//ki2
			
			sR1Mower.sRunParam.MowSpeed = paramtemp[12]; //��ݵ���ٶ�
			sR1Mower.sRunParam.PowerLimit = paramtemp[13] ;//���ʣ�������ֵ
			
			SaveParams();
			return;
			
		}
		if(Key_Release(KEY_SET)) //�����淵��
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
		//��������ٶ�
		sprintf(strbuff,"SH%0.3dSM%0.3dSL%0.3d",paramtemp[0],paramtemp[1],paramtemp[2]);
		TFT_ShowText(1,0,0x00,strbuff);
		
		//��ʾ�߽�ǿ��
		sprintf(strbuff,"BH%0.3dBM%0.3dBL%0.3d",paramtemp[3],paramtemp[4],paramtemp[5]);
		TFT_ShowText(2,0,0x00,strbuff);


				//��ʾkp1 kd1 ki1
		sprintf(strbuff,"P1%0.3dD1%0.3dI1%0.3d",paramtemp[6],paramtemp[7],paramtemp[8]);
		TFT_ShowText(3,0,0x00,strbuff);
		
				//��ʾkp2 kd2 ki2
		sprintf(strbuff,"P2%0.3dD2%0.3dI2%0.3d",paramtemp[9],paramtemp[10],paramtemp[11]);
		TFT_ShowText(4,0,0x00,strbuff);

		//��ʾ��ݵ���ٶȺ͵�����ֵ
		sprintf(strbuff,"MS%0.3dPL%0.3d",paramtemp[12],paramtemp[13]);
		TFT_ShowText(5,0,0x00,strbuff);
			

		//��ʾѡ����
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


#elif defined	 DISP_TYPE_MzL05LCD	//���ʹ��MzL05LCD��ʾģ��


extern unsigned char MzL05LCD_Asii0608[];			//6X8��ASII�ַ���
extern unsigned char MzL05LCD_Asii0816[];			//8X16��ASII�ַ���
extern unsigned char MzL05LCD_GB1616[];				//16*16�Զ���ĺ��ֿ�

unsigned char MzL05LCD_X_Width;									//�ַ�д��ʱ�Ŀ��
unsigned char MzL05LCD_Y_Width;									//�ַ�д��ʱ�ĸ߶�
unsigned char MzL05LCD_Font_Wrod;								//�����ÿ����ģռ�ö��ٸ��洢��Ԫ��
unsigned char *MzL05LCD_Char_TAB;						//�ֿ�ָ��
char strbuff[22];

//��ʼ��
void MzL05LCD_Init(void)
{
	//LCD������ʹ�õ��Ķ˿ڵĳ�ʼ��������б�Ҫ�Ļ���
	MzL05LCD_TimeDelay(200);
	GPIO_ResetBits(MzL05LCD_RES);
	MzL05LCD_TimeDelay(200);
	GPIO_SetBits(MzL05LCD_RES);
	MzL05LCD_TimeDelay(20);
		
	MzL05LCD_RegWrite(M_LCD_ON);							//LCD On
	MzL05LCD_RegWrite(M_LCD_POWER_ALL);					//�����ϵ����ģʽ
	
	MzL05LCD_RegWrite(M_LCD_ELE_VOL);					//��������ģʽ����ʾ���ȣ�
	MzL05LCD_RegWrite(0x1f);								//ָ������0x0000~0x003f
	
	MzL05LCD_RegWrite(M_LCD_VDD_SET);					//V5�ڲ���ѹ���ڵ�������
	MzL05LCD_RegWrite(M_LCD_VDD);						//LCDƫѹ���ã�V3ʱѡ
	
	MzL05LCD_RegWrite(M_LCD_COM_REV);					//Com ɨ�跽ʽ����
	MzL05LCD_RegWrite(M_LCD_SEG_NOR);					//Segment����ѡ��
	MzL05LCD_RegWrite(M_LCD_ALL_LOW);					//ȫ������/�䰵ָ��
	MzL05LCD_RegWrite(M_LCD_ALL_NOR);					//��������ʾ����ָ��
	
	MzL05LCD_RegWrite(M_LCD_STATIC_OFF);					//�رվ�ָ̬ʾ��
	MzL05LCD_RegWrite(0x00);								//ָ������
	
	MzL05LCD_RegWrite(M_LCD_BEGIN_LINE);				//������ʾ��ʼ�ж�ӦRAM
	MzL05LCD_Fill(LCD_INITIAL_COLOR);
}

void SPI_Config(void) 
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
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);	
	
	
		//����B12 CS
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;				     
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //���߷�ת�ٶ�Ϊ50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
		//����B13 CLK
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				     
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //���߷�ת�ٶ�Ϊ50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
		//����B15 Data
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;				     
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //���߷�ת�ٶ�Ϊ50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	//����C6Ϊ MzL05LCD��RST
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_13;				     
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //���߷�ת�ٶ�Ϊ50MHz
  GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
}


//��ʱ����
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
// ����: void LCD_RegWrite(unsigned char Command)
// ����: дһ���ֽڵ�������LCD�еĿ��ƼĴ�������
// ����: Command		д������ݣ��Ͱ�λ��Ч��byte�� 
// ����: ��
// ��ע: 
// �汾:
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
// ����: void LCD_DataWrite(unsigned int Data)
// ����: дһ���ֽڵ���ʾ������LCD�е���ʾ����RAM����
// ����: Data д������� 
// ����: ��
// ��ע: ��
// �汾:
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
// ����: void LCD_Fill(unsigned int Data)
// ����: ���������Data��������������
// ����: Data   Ҫ������ɫ����
// ����: ��
// ��ע: ����LCD��ʼ�������е���
// �汾:
//      2006/10/15      First version
//		2007/01/09		V1.2 
//========================================================================
void MzL05LCD_Fill(unsigned char Data)
{
	unsigned char i,j;
	unsigned char uiTemp;
	uiTemp = Dis_Y_MAX;
	uiTemp = uiTemp>>3;
	for(i=0;i<=uiTemp;i++)								//��LCD������ʼ������ʾ����
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
// ����: void ClrScreen(unsigned int Mode)
// ����: ����������ִ��ȫ��Ļ��������ǰ��ɫ
// ����: Mode   0:ȫ������
//				1:ȫ�����ǰ��ɫ
// ����: ��
// ��ע: 
// �汾:
//      2007/01/11      First version
//========================================================================
void MzL05LCD_ClrScreen(void)
{
	MzL05LCD_Fill(LCD_INITIAL_COLOR);
}

//========================================================================
// ����: void FontSet(int Font_NUM,unsigned int Color)
// ����: �ı���������
// ����: Font_NUM ����ѡ��,�������������ֿ�Ϊ׼
//		 Color  �ı���ɫ,���������Դ��ֿ�  
// ����: ��
// ��ע: 
// �汾:
//      2006/10/15      First version
//========================================================================
void MzL05LCD_FontSet(unsigned char Font_NUM)
{
	switch(Font_NUM)
	{
		case 0: MzL05LCD_Font_Wrod = 16;	//ASII�ַ�A
				MzL05LCD_X_Width = 8;
				MzL05LCD_Y_Width = 2;
				//Char_Color = Color;
				MzL05LCD_Char_TAB = (unsigned char *)(MzL05LCD_Asii0816 - 32*16);
		break;
		case 1: MzL05LCD_Font_Wrod = 6;	//ASII�ַ�B
				MzL05LCD_X_Width = 6;
				MzL05LCD_Y_Width = 1;
				//Char_Color = Color;
				MzL05LCD_Char_TAB = (unsigned char *)(MzL05LCD_Asii0608 - (32*6));
		break;		
		case 2: MzL05LCD_Font_Wrod = 32;	//����A
				MzL05LCD_X_Width = 16;
				MzL05LCD_Y_Width = 2;
				//Char_Color = Color;
				MzL05LCD_Char_TAB = (unsigned char *)MzL05LCD_GB1616;
		break;
		default: break;
	}
}

//========================================================================
// ����: void PutChar(unsigned char x,unsigned char y,char a)  
// ����: д��һ����׼�ַ�
// ����: x  X������     y  Y������(0~7ҳ)
//		 a  Ҫ��ʾ���ַ����ֿ��е�ƫ����  
// ����: ��
// ��ע: ASCII�ַ���ֱ������ASCII�뼴��
// �汾:
//      2006/10/15      First version
//		2007/05/17		V2.1  for seriea port LCD
//========================================================================
void MzL05LCD_PutChar(unsigned char x,unsigned char y,char a,unsigned char inv)       
{
	unsigned char i,j;
	unsigned char *p_data; 
	p_data = MzL05LCD_Char_TAB + a*MzL05LCD_Font_Wrod;	//Ҫд�ַ����׵�ַ
	x = x+1;							//Edit by xinqiang
	for(j=0;j<MzL05LCD_Y_Width;j++)
	{
		if((y+j) < (Dis_Y_MAX/8))
		MzL05LCD_RegWrite(y+j+0xb0);
		MzL05LCD_RegWrite(((x&0xf0) >> 4)|0x10);	//���ó�ʼ��ַ 
		MzL05LCD_RegWrite(x&0x0f);
		for(i=0;i<MzL05LCD_X_Width;i++)
		{  
			if(((x+i) < Dis_X_MAX)&&((y+j) < (Dis_Y_MAX/8)))
			MzL05LCD_DataWrite((*p_data++)|inv);  //��inv=0x80ʱ,���»���;��inv=0x01ʱ,���ϻ���
// 				MzL05LCD_DataWrite((*p_data++)^inv); //���ԣ���inv=0xFFʱ
// 			MzL05LCD_DataWrite(*p_data++);
		}
	}
}

//========================================================================
// ����: void PutString(unsigned char x,unsigned char y,char *p)
// ����: ��x��yΪ��ʼ���괦д��һ����׼�ַ�
// ����: x  X������     y  Y������(0~7ҳ)
//		 p  Ҫ��ʾ���ַ���  inv �Ƿ���
// ����: ��
// ��ע: ���������Դ���ASCII�ַ�����ʾ
// �汾:
//      2006/10/15      First version
//========================================================================
void MzL05LCD_PutString(unsigned char x,unsigned char y,char *p,unsigned char inv)
{
// 	MzL05LCD_RegWrite(M_LCD_BEGIN_LINE);				//������ʾ��ʼ�ж�ӦRAM
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


//���ú���
void DispInit(void)
{
	SPI_Config(); //��ʼ���˿�
	MzL05LCD_Init();
	MzL05LCD_FontSet(1);				//ѡ��8X16��ASCII�ַ�
}
//��ʾ������Ϣ��status��ת����ʮ��������ʾ
void ShowDebugInfo(uint8_t line,uint8_t col, uint8_t status)
{
	//char strbuff[3];
	sprintf(strbuff,"%0.2X",status);
	MzL05LCD_PutString(col*MzL05LCD_X_Width,line*MzL05LCD_Y_Width,strbuff,0x00);
}
//�ڵ�line����ʾһ���ַ���Ϣinfo
void ShowStatusInfo(uint8_t line, uint8_t col,char* info)
{
	MzL05LCD_PutString(col*MzL05LCD_X_Width,line*MzL05LCD_Y_Width,info,0x00);
}
//��ʾ�����Ϣ
void ShowBatteryInfo(uint8_t val)
{
	//char strbuff[11];
	sprintf(strbuff," Power:%0.3d%c",val,'%');
	MzL05LCD_PutString(0*MzL05LCD_X_Width,3*MzL05LCD_Y_Width,strbuff,0x00);
}
//��ʾ��ǰ����ʱ��
void ShowCurrentTime(void)
{
		//char strbuff[17];
		CalculateTime();
		sprintf(strbuff,"%0.4d-%0.2d-%0.2d %0.2d:%0.2d",s_DateStructVar.Year,s_DateStructVar.Month,s_DateStructVar.Day,
		s_TimeStructVar.HourHigh*10+s_TimeStructVar.HourLow, s_TimeStructVar.MinHigh*10+s_TimeStructVar.MinLow
		);
		MzL05LCD_PutString(2*MzL05LCD_X_Width,0*MzL05LCD_Y_Width,strbuff,0x00);
}
//��ʾĬ��ҳ��
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
		IWDG_ReloadCounter(); //ι��
		if(Key_Release(KEY_OK))  //�������õ�����
		{
			
			SetDate(dttemp[2],dttemp[1],2000+dttemp[0]);
			SetTime(dttemp[3],dttemp[4],00);
			return;
			
		}
		if(Key_Release(KEY_SET)) //�����淵��
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
		//��ʾ����
		sprintf(strbuff,"20%0.2d-%0.2d-%0.2d",dttemp[0],dttemp[1],dttemp[2]);
		MzL05LCD_PutString(5*MzL05LCD_X_Width,2*MzL05LCD_Y_Width,strbuff,0x00);
		
		//��ʾʱ��
		sprintf(strbuff,"%0.2d:%0.2d",dttemp[3],dttemp[4]);
		MzL05LCD_PutString(8*MzL05LCD_X_Width,4*MzL05LCD_Y_Width,strbuff,0x00);

		//��ʾ��ǰ���λ��
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
		IWDG_ReloadCounter(); //ι��
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
	
	paramtemp[0] = sR1Mower.sRunParam.DrvSpeedH;  //��������ٶȸ�
	paramtemp[1] = sR1Mower.sRunParam.DrvSpeedM ;//��������ٶ���
	paramtemp[2] = sR1Mower.sRunParam.DrvSpeedL;//��������ٶȵ�
	
	paramtemp[3] = sR1Mower.sRunParam.BsStrenghH ;//�߽��ź�ǿ�ȸ�
	paramtemp[4] = sR1Mower.sRunParam.BsStrenghM;  //�߽��ź�ǿ����
	paramtemp[5] = sR1Mower.sRunParam.BsStrenghL;//�߽��ź�ǿ�ȵ�
	
	
	paramtemp[6] = sR1Mower.sRunParam.KP1*100;//kp1
	paramtemp[7] = sR1Mower.sRunParam.KD1*100;//kd1
	paramtemp[8] = sR1Mower.sRunParam.KI1*100;//ki1
	
	paramtemp[9] = sR1Mower.sRunParam.KP2*100;//kp2
	paramtemp[10] = sR1Mower.sRunParam.KD2*100;//kd2
	paramtemp[11] = sR1Mower.sRunParam.KI2*100;//ki2
	
	paramtemp[12] = sR1Mower.sRunParam.BladeSpeed; //��ݵ���ٶ�
	paramtemp[13] = sR1Mower.sRunParam.PowerLimit ;//���ʣ�������ֵ
	
	paramtemp[14] = sR1Mower.sRunParam.BehaviorTO;
	paramtemp[15] = sR1Mower.sRunParam.MotionSense/32;
	
	DispClear();
	MzL05LCD_PutString(4*MzL05LCD_X_Width,0*MzL05LCD_Y_Width,"Configuration",0x00);
	
	set_index = 0;
		
	while(1)
	{
		IWDG_ReloadCounter(); //ι��
		if(Key_Release(KEY_OK))  //��������
		{
			sR1Mower.sRunParam.DrvSpeedH = paramtemp[0];  //��������ٶȸ�
			sR1Mower.sRunParam.DrvSpeedM = paramtemp[1] ;//��������ٶ���
			sR1Mower.sRunParam.DrvSpeedL = paramtemp[2] ;//��������ٶȵ�
			
			sR1Mower.sRunParam.BsStrenghH = paramtemp[3];//�߽��ź�ǿ�ȸ�
			sR1Mower.sRunParam.BsStrenghM = paramtemp[4];  //�߽��ź�ǿ����
			sR1Mower.sRunParam.BsStrenghL = paramtemp[5];//�߽��ź�ǿ�ȵ�
			
			
			sR1Mower.sRunParam.KP1 = paramtemp[6]/100.0;//kp1
			sR1Mower.sRunParam.KD1 = paramtemp[7]/100.0;//kd1
			sR1Mower.sRunParam.KI1 = paramtemp[8]/100.0;//ki1
			
			sR1Mower.sRunParam.KP2 = paramtemp[9]/100.0 ;//kp2
			sR1Mower.sRunParam.KD2 = paramtemp[10]/100.0;//kd2
			sR1Mower.sRunParam.KI2 = paramtemp[11]/100.0 ;//ki2
			
			sR1Mower.sRunParam.BladeSpeed = paramtemp[12]; //��ݵ���ٶ�
			sR1Mower.sRunParam.PowerLimit = paramtemp[13] ;//���ʣ�������ֵ
			
			sR1Mower.sRunParam.BehaviorTO = paramtemp[14];
			sR1Mower.sRunParam.MotionSense = paramtemp[15] * 32;
			
			EE_SaveParams();
			return;
			
		}
		if(Key_Release(KEY_SET)) //�����淵��
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
		//��������ٶ�
		sprintf(strbuff,"SH:%0.3d SM:%0.3d SL:%0.3d",paramtemp[0],paramtemp[1],paramtemp[2]);
		MzL05LCD_PutString(0*MzL05LCD_X_Width,1*MzL05LCD_Y_Width,strbuff,0x00);
		
		//��ʾ�߽�ǿ��
		sprintf(strbuff,"BH:%0.3d BM:%0.3d BL:%0.3d",paramtemp[3],paramtemp[4],paramtemp[5]);
		MzL05LCD_PutString(0*MzL05LCD_X_Width,2*MzL05LCD_Y_Width,strbuff,0x00);


				//��ʾkp1 kd1 ki1
		sprintf(strbuff,"P1:%0.3d D1:%0.3d I1:%0.3d",paramtemp[6],paramtemp[7],paramtemp[8]);
		MzL05LCD_PutString(0*MzL05LCD_X_Width,3*MzL05LCD_Y_Width,strbuff,0x00);
		
				//��ʾkp2 kd2 ki2
		sprintf(strbuff,"P2:%0.3d D2:%0.3d I2:%0.3d",paramtemp[9],paramtemp[10],paramtemp[11]);
		MzL05LCD_PutString(0*MzL05LCD_X_Width,4*MzL05LCD_Y_Width,strbuff,0x00);

		//��ʾ��ݵ���ٶȺ͵�����ֵ
		sprintf(strbuff,"MS:%0.3d PL:%0.3d TO:%0.3d",paramtemp[12],paramtemp[13],paramtemp[14]);
		MzL05LCD_PutString(0*MzL05LCD_X_Width,5*MzL05LCD_Y_Width,strbuff,0x00);
		
		sprintf(strbuff,"CS:%0.3d",paramtemp[15]);
		MzL05LCD_PutString(0*MzL05LCD_X_Width,6*MzL05LCD_Y_Width,strbuff,0x00);
			

		//��ʾѡ����
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

#elif defined	 DISP_TYPE_ST7920	//���ʹ��ST7920��ʾģ��

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
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				     
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //���߷�ת�ٶ�Ϊ50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);	

	
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

//���ú���
void DispInit(void)
{
	SPI2_Config(); //��ʼ���˿�
	LCD_Config();
}
//��ʾ������Ϣ��status��ת����ʮ��������ʾ
void ShowDebugInfo(uint8_t line,uint8_t col, uint8_t status)
{


}
//�ڵ�line����ʾһ���ַ���Ϣinfo
void ShowStatusInfo(uint8_t line, uint8_t col,char* info)
{

}
//��ʾ�����Ϣ
void ShowBatteryInfo(uint8_t val)
{


}
//��ʾ��ǰ����ʱ��
void ShowCurrentTime(void)
{

}
//��ʾĬ��ҳ��
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
