
#include "MzL105LCD.h"

extern unsigned char MzL05LCD_Asii0608[];			//6X8的ASII字符库
extern unsigned char MzL05LCD_Asii0816[];			//8X16的ASII字符库
extern unsigned char MzL05LCD_GB1616[];				//16*16自定义的汉字库
unsigned char MzL05LCD_X_Witch;									//字符写入时的宽度
unsigned char MzL05LCD_Y_Witch;									//字符写入时的高度
unsigned char MzL05LCD_Font_Wrod;								//字体的每个字模占用多少个存储单元数
unsigned char *MzL05LCD_Char_TAB;						//字库指针


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
	GPIO_ResetBits(MzL05LCD_A0);
	for(Num=0;Num<8;Num++)
	{
		//MzL05LCD_TimeDelay(20);
		if((Command&0x80) == 0)	GPIO_ResetBits(MzL05LCD_SDI);
		else GPIO_SetBits(MzL05LCD_SDI);
		Command = Command << 1;
		GPIO_ResetBits(MzL05LCD_CLK);
		//MzL05LCD_TimeDelay(20);
		GPIO_SetBits(MzL05LCD_CLK);
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
	GPIO_SetBits(MzL05LCD_A0);
	for(Num=0;Num<8;Num++)
	{
		if((Dat&0x80) == 0)	GPIO_ResetBits(MzL05LCD_SDI);
		else GPIO_SetBits(MzL05LCD_SDI);
		Dat = Dat << 1;
		GPIO_ResetBits(MzL05LCD_CLK);
		GPIO_SetBits(MzL05LCD_CLK);
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
		MzL05LCD_RegWrite(0x01);
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
				MzL05LCD_X_Witch = 8;
				MzL05LCD_Y_Witch = 2;
				//Char_Color = Color;
				MzL05LCD_Char_TAB = (unsigned char *)(MzL05LCD_Asii0816 - 32*16);
		break;
		case 1: MzL05LCD_Font_Wrod = 6;	//ASII字符B
				MzL05LCD_X_Witch = 6;
				MzL05LCD_Y_Witch = 1;
				//Char_Color = Color;
				MzL05LCD_Char_TAB = (unsigned char *)(MzL05LCD_Asii0608 - (32*6));
		break;		
		case 2: MzL05LCD_Font_Wrod = 32;	//汉字A
				MzL05LCD_X_Witch = 16;
				MzL05LCD_Y_Witch = 2;
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
void MzL05LCD_PutChar(unsigned char x,unsigned char y,char a)       
{
	unsigned char i,j;
	unsigned char *p_data; 
	p_data = MzL05LCD_Char_TAB + a*MzL05LCD_Font_Wrod;	//要写字符的首地址
	x = x+1;							//Edit by xinqiang
	for(j=0;j<MzL05LCD_Y_Witch;j++)
	{
		if((y+j) < (Dis_Y_MAX/8))
		MzL05LCD_RegWrite(y+j+0xb0);
		MzL05LCD_RegWrite(((x&0xf0) >> 4)|0x10);	//设置初始地址 
		MzL05LCD_RegWrite(x&0x0f);
		for(i=0;i<MzL05LCD_X_Witch;i++)
		{  
			if(((x+i) < Dis_X_MAX)&&((y+j) < (Dis_Y_MAX/8)))
			MzL05LCD_DataWrite(*p_data++);
		}
	}
}

//========================================================================
// 函数: void PutString(unsigned char x,unsigned char y,char *p)
// 描述: 在x、y为起始坐标处写入一串标准字符
// 参数: x  X轴坐标     y  Y轴坐标(0~7页)
//		 p  要显示的字符串  
// 返回: 无
// 备注: 仅能用于自带的ASCII字符串显示
// 版本:
//      2006/10/15      First version
//========================================================================
void MzL05LCD_PutString(unsigned char x,unsigned char y,char *p)
{
	while(*p!=0)
	{
		MzL05LCD_PutChar(x,y,*p);
		x += MzL05LCD_X_Witch;
		if((x + MzL05LCD_X_Witch) > Dis_X_MAX)
		{
			x = 0;
			if((Dis_Y_MAX - y) < MzL05LCD_Y_Witch) break;
			else y += MzL05LCD_Y_Witch;
		}
		p++;
	}
}

