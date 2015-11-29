#ifndef __MzL05_LCD_H
#define __MzL05_LCD_H
//MzL05-12864 不带字库LCD显示控制
#include "stm32f10x.h"


#define MzL05LCD_CS	GPIOA,GPIO_Pin_1
#define MzL05LCD_RES GPIOE,GPIO_Pin_0
#define MzL05LCD_A0  GPIOA,GPIO_Pin_6
#define MzL05LCD_CLK GPIOA,GPIO_Pin_5
#define MzL05LCD_SDI GPIOA,GPIO_Pin_7

void MzL05LCD_Init(void);
void MzL05LCD_TimeDelay(int Time);
void MzL05LCD_RegWrite(unsigned char Command);
void MzL05LCD_DataWrite(unsigned char Dat);
void MzL05LCD_Fill(unsigned char Data);
void MzL05LCD_ClrScreen(void);
void MzL05LCD_FontSet(unsigned char Font_NUM);
void MzL05LCD_PutChar(unsigned char x,unsigned char y,char a);
void MzL05LCD_PutString(unsigned char x,unsigned char y,char *p);


//常量定义，原LCD_Config.h文件
#define LCD_X_MAX			128-1			//屏幕的X轴的物理宽度
#define LCD_Y_MAX			64-1			//屏幕的Y轴的物理宽度
			
#define LCD_XY_Switch		0				//显示时X轴和Y由交换
#define LCD_X_Rev			0				//显示时X轴反转
#define LCD_Y_Rev			0				//显示时Y轴反转

#if LCD_XY_Switch == 0
	#define Dis_X_MAX		LCD_X_MAX
	#define Dis_Y_MAX		LCD_Y_MAX	
#endif

#if LCD_XY_Switch == 1
	#define Dis_X_MAX		LCD_Y_MAX
	#define Dis_Y_MAX		LCD_X_MAX	
#endif

#define LCD_INITIAL_COLOR	0x00			//定义LCD屏初始化时的背景色

//以下定义为针对于SPLC501的功能指令进行定义的，局部可修改~
//LCD供电电平选择
#define		M_LCD_VDD_SET	M_LCD_SETR_4		//3.3V供电时选此二项
#define		M_LCD_VDD		M_LCD_BIAS_9		//....
//#define		M_LCD_VDD_SET	M_LCD_SETR_4	//5.0V供电时选此二项
//#define		M_LCD_VDD		M_LCD_BIAS_9	//...
//LCD指令
//LCD开关命令
#define		M_LCD_ON		0xaf
#define		M_LCD_OFF		0xae
//设置上电控制模式
#define		M_LCD_POWER_BC	0x2c
#define		M_LCD_POWER_VR	0x2a
#define		M_LCD_POWER_VC	0x29
#define		M_LCD_POWER_ALL	0x2f
//V5内部电压调节电阻设置……
#define		M_LCD_SETR_0	0x20
#define		M_LCD_SETR_1	0x21
#define		M_LCD_SETR_2	0x22
#define		M_LCD_SETR_3	0x23
#define		M_LCD_SETR_4	0x24
#define		M_LCD_SETR_5	0x25
#define		M_LCD_SETR_6	0x26
#define		M_LCD_SETR_7	0x27
//...end
#define		M_LCD_ELE_VOL	0x81		//电量设置模式（显示亮度）
//偏压设置
#define		M_LCD_BIAS_9	0xa2		//V5时选此选项设置
#define		M_LCD_BIAS_7	0xa1		//V3时选此选项设置
//Com 扫描方式设置命令
#define		M_LCD_COM_NOR	0xc0		//正常方式
#define		M_LCD_COM_REV	0xc8		//反相
//Segment方向选择
#define		M_LCD_SEG_NOR	0xa0		//正常
#define		M_LCD_SEG_REV	0xa1		//反向
//全屏点亮/变暗指令
#define		M_LCD_ALL_LIGNT	0xa5		//LCD ALL paint ON
#define		M_LCD_ALL_LOW	0xa4		//Normal Display mode
//正相反相显示控制指令，RAM中数据不变
#define		M_LCD_ALL_NOR	0xa6		//正相
#define		M_LCD_ALL_REV	0xa7		//反相
//静态指示器控制指令
#define		M_LCD_STATIC_ON	0xad		//ON
#define		M_LCD_STATIC_OFF 0xac		//OFF
//设置显示起始行对应RAM行号
#define		M_LCD_BEGIN_LINE 0x40		//基数，后面可加的尾数可为0~63
//设置当前页基数
#define		M_LCD_COL_PAGE	0xb0		//基数指令，后可加尾数0~8
//设置当前列基数
#define		M_LCD_COL_LINE_LOW	0x04		//基数指令，低四位有效
#define		M_LCD_COL_LINE_HIG	0x10		//基数指令，低四位有效


//函数定义，原LCD_Dis.h文件
//LCD 初始化函数，一般用于LCD屏的寄存器初始化
extern void LCD_Init(void);	//在LCD_Driver_User.c中定义的函数，如有必要该函数还会包含端口初始化、复位等操作

//画直线函数，s_x、s_y为起始点，e_x、e_y为结束点
//extern void Line(unsigned char s_x,unsigned char s_y,unsigned char e_x,unsigned char e_y);
//绘图模式、当前绘图颜色设置
extern void SetPaintMode(unsigned char Mode,unsigned char Color);
//标准字符设置，包括两型号ASCII码的大小尺寸，以及字体颜色的设置
extern void FontSet(unsigned char Font_NUM,unsigned char Color);
//于x、y的坐标上写入一个标准字符
extern void PutChar(unsigned char x,unsigned char y,char a);
//于x、y的坐标为起始写入一串标准字符串
extern void PutString(unsigned char x,unsigned char y,char *p);
//于x、y的坐标为中心，绘制一个圆边或实心圆
//extern void PutPixel(unsigned char x,unsigned char y);
//extern void Circle(unsigned char x,unsigned char y,unsigned char r,unsigned char mode);
//绘制一个以left、top和right、bottom为两个对角的矩形框或者实心矩形
extern void Rectangle(unsigned char left,unsigned char top,unsigned char right,unsigned char bottom);
//清屏函数，执行全屏幕清除或填充前景色
void ClrScreen(unsigned char Mode);

//以下函数以及变量的声明一般建议用户不要调用，仅供高级用户在自行编写特殊显示效果以及特性应用程序时使用
//
//extern unsigned char code Asii16[];		//8X16的ASII字符库
//extern unsigned char code GB32[];		//自定义的32X29汉字库
//extern unsigned char code GB48[];		//自定义的48X55汉字库

extern unsigned char X_Witch;					//字符写入时的宽度
extern unsigned char Y_Witch;					//字符写入时的高度
extern unsigned char Font_Wrod;				//字体的大
extern unsigned char *Char_TAB;		//字库指针
//extern unsigned char Plot_Mode;				//绘图模式
extern unsigned char BMP_Color;
extern unsigned char Char_Color;

//extern void Pos_Switch(unsigned int * x,unsigned int * y);
//extern void Writ_Dot(int x,int y,unsigned int Color);
//extern void Line_f(int s_x,int s_y,int e_x);
//extern unsigned int Get_Dot(int x,int y);
//extern void Clear_Dot(int x,int y);
//extern void Set_Dot_Addr(int x,int y);


#endif /* __MzL05_LCD_H */
