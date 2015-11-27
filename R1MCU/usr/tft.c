/****************************************************************************
* Copyright (C), 2011 �ܶ�Ƕ��ʽ������ www.ourstm.net
*
* �������� �ܶ���STM32������V2,2.1,V3,MINI�ϵ���ͨ��           
* QQ: 9191274, ������sun68, Email: sun68@163.com 
* �Ա����̣�ourstm.taobao.com  
* �ܶ�����̳��www.ourstm.net  
*
* �ļ���: main.c
* ���ݼ���:
*       ��������ʾ����3��TFT������ʾһ��16λɫͼƬ������ͼƬ��͸������������ͬ��ʾ������ַ���
*       ͼƬ��С��Χ400X240֮�ڡ� �ַ���ȡģ�ߴ磺��ɫ400X240 ֮�� ȡģX�᳤��Ϊ8������������
*       ͼƬȡģ�����img2lcd
*       �ַ�ȡģ�����ZIMO3
*
* �ļ���ʷ:
* �汾��  ����       ����    ˵��
* v0.1    2011-12-5 sun68  �������ļ�
*/

//#include "fsmc_sram.h" 
//#include "stm32f10x_fsmc.h"
#include "tft.h"
#include "stdio.h"
#include "string.h"

unsigned long color1=0;
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
	color1=0; 
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


//��ʾ����
void LCD_test(void)
{

	unsigned char *p;


// 	lcd_DrawPicture(0,0,0,a1);	    	//��ĳһָ��λ����ʾͼƬ�� ͼƬ�ĳߴ�Ҫ��240X400��Χ�ڡ�  
// 	Delay(0x1afffff);
// 	lcd_DrawPicture(0,0,1,a2);	    	//��ĳһָ��λ����ʾͼƬ�� ͼƬ�ĳߴ�Ҫ��400X240��Χ�ڡ�      

// 	Delay(0x1afffff);

//     lcd_wr_zf(0,0,280,32,color1,1,&zf3[0]);  		//��ʾ�ַ���
	
	//��ʾ5λ�����֣� ��ֵ��COLOR1ֵ���ڱ仯
	p=num_pub((color1/10000));
	lcd_wr_zf(50,30,24,32,color1,1,p);  
	
	p=num_pub((color1%10000)/1000);
	lcd_wr_zf(74,30,24,32,color1,1,p);  
	
	p=num_pub(((color1%10000)%1000)/100);
	lcd_wr_zf(98,30,24,32,color1,1,p);  

	p=num_pub((((color1%10000)%1000)%100)/10);
	lcd_wr_zf(122,30,24,32,color1,1,p);  

	p=num_pub((color1%10));
	lcd_wr_zf(146,30,24,32,color1,1,p);  
	
	//lcd_wr_zf(202,30,8,16,color1,1,&TestFont[0]);  
// 	lcd_wr_zf(202,30,8,16,color1,1,TestFont+16);  
// 	lcd_wr_zf(210,30,8,16,color1,1,TestFont+32);  
	
	TFT_Write_Char(194,0,24,32,color1,0,'1');  
	TFT_Write_Char(194,30,24,32,color1,0,'2');  
	
	TFT_Write_Char(194,60,24,32,color1,0,'3');  
	TFT_Write_Char(194,30,24,32,color1,1,'_'); 
	TFT_Write_Char(194,90,24,32,color1,0,'A');  
	TFT_Write_Char(194,120,24,32,color1,0,'B');  
	TFT_Write_Char(194,150,24,32,color1,0,'C');  
	TFT_Write_Char(194,180,24,32,color1,0,'a');  
	TFT_Write_Char(218,180,24,32,color1,0,'a');  
	TFT_Write_Char(242,180,24,32,color1,0,'a');  
	TFT_Write_Char(266,180,24,32,color1,0,'a');  
	TFT_Write_Char(194,208,24,32,color1,0,'b');  
	
	TFT_ShowText(0,0,color1,"Test");
	TFT_ShowText(1,1,color1,"Test");
	TFT_ShowText(2,2,color1,"Test");
	TFT_ShowText(3,3,color1,"Test");
	TFT_ShowText(4,4,color1,"Test");
	TFT_ShowText(5,5,color1,"Test");
	TFT_ShowText(6,6,color1,"Test");
	TFT_ShowText(7,7,color1,"Test");
	

	//��ɫ����
	color1++; 
	if(color1>=65536) color1=0;  
	Delay(0xffff);			
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
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
