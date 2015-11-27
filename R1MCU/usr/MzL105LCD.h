#ifndef __MzL05_LCD_H
#define __MzL05_LCD_H
//MzL05-12864 �����ֿ�LCD��ʾ����
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


//�������壬ԭLCD_Config.h�ļ�
#define LCD_X_MAX			128-1			//��Ļ��X���������
#define LCD_Y_MAX			64-1			//��Ļ��Y���������
			
#define LCD_XY_Switch		0				//��ʾʱX���Y�ɽ���
#define LCD_X_Rev			0				//��ʾʱX�ᷴת
#define LCD_Y_Rev			0				//��ʾʱY�ᷴת

#if LCD_XY_Switch == 0
	#define Dis_X_MAX		LCD_X_MAX
	#define Dis_Y_MAX		LCD_Y_MAX	
#endif

#if LCD_XY_Switch == 1
	#define Dis_X_MAX		LCD_Y_MAX
	#define Dis_Y_MAX		LCD_X_MAX	
#endif

#define LCD_INITIAL_COLOR	0x00			//����LCD����ʼ��ʱ�ı���ɫ

//���¶���Ϊ�����SPLC501�Ĺ���ָ����ж���ģ��ֲ����޸�~
//LCD�����ƽѡ��
#define		M_LCD_VDD_SET	M_LCD_SETR_4		//3.3V����ʱѡ�˶���
#define		M_LCD_VDD		M_LCD_BIAS_9		//....
//#define		M_LCD_VDD_SET	M_LCD_SETR_4	//5.0V����ʱѡ�˶���
//#define		M_LCD_VDD		M_LCD_BIAS_9	//...
//LCDָ��
//LCD��������
#define		M_LCD_ON		0xaf
#define		M_LCD_OFF		0xae
//�����ϵ����ģʽ
#define		M_LCD_POWER_BC	0x2c
#define		M_LCD_POWER_VR	0x2a
#define		M_LCD_POWER_VC	0x29
#define		M_LCD_POWER_ALL	0x2f
//V5�ڲ���ѹ���ڵ������á���
#define		M_LCD_SETR_0	0x20
#define		M_LCD_SETR_1	0x21
#define		M_LCD_SETR_2	0x22
#define		M_LCD_SETR_3	0x23
#define		M_LCD_SETR_4	0x24
#define		M_LCD_SETR_5	0x25
#define		M_LCD_SETR_6	0x26
#define		M_LCD_SETR_7	0x27
//...end
#define		M_LCD_ELE_VOL	0x81		//��������ģʽ����ʾ���ȣ�
//ƫѹ����
#define		M_LCD_BIAS_9	0xa2		//V5ʱѡ��ѡ������
#define		M_LCD_BIAS_7	0xa1		//V3ʱѡ��ѡ������
//Com ɨ�跽ʽ��������
#define		M_LCD_COM_NOR	0xc0		//������ʽ
#define		M_LCD_COM_REV	0xc8		//����
//Segment����ѡ��
#define		M_LCD_SEG_NOR	0xa0		//����
#define		M_LCD_SEG_REV	0xa1		//����
//ȫ������/�䰵ָ��
#define		M_LCD_ALL_LIGNT	0xa5		//LCD ALL paint ON
#define		M_LCD_ALL_LOW	0xa4		//Normal Display mode
//���෴����ʾ����ָ�RAM�����ݲ���
#define		M_LCD_ALL_NOR	0xa6		//����
#define		M_LCD_ALL_REV	0xa7		//����
//��ָ̬ʾ������ָ��
#define		M_LCD_STATIC_ON	0xad		//ON
#define		M_LCD_STATIC_OFF 0xac		//OFF
//������ʾ��ʼ�ж�ӦRAM�к�
#define		M_LCD_BEGIN_LINE 0x40		//����������ɼӵ�β����Ϊ0~63
//���õ�ǰҳ����
#define		M_LCD_COL_PAGE	0xb0		//����ָ���ɼ�β��0~8
//���õ�ǰ�л���
#define		M_LCD_COL_LINE_LOW	0x04		//����ָ�����λ��Ч
#define		M_LCD_COL_LINE_HIG	0x10		//����ָ�����λ��Ч


//�������壬ԭLCD_Dis.h�ļ�
//LCD ��ʼ��������һ������LCD���ļĴ�����ʼ��
extern void LCD_Init(void);	//��LCD_Driver_User.c�ж���ĺ��������б�Ҫ�ú�����������˿ڳ�ʼ������λ�Ȳ���

//��ֱ�ߺ�����s_x��s_yΪ��ʼ�㣬e_x��e_yΪ������
//extern void Line(unsigned char s_x,unsigned char s_y,unsigned char e_x,unsigned char e_y);
//��ͼģʽ����ǰ��ͼ��ɫ����
extern void SetPaintMode(unsigned char Mode,unsigned char Color);
//��׼�ַ����ã��������ͺ�ASCII��Ĵ�С�ߴ磬�Լ�������ɫ������
extern void FontSet(unsigned char Font_NUM,unsigned char Color);
//��x��y��������д��һ����׼�ַ�
extern void PutChar(unsigned char x,unsigned char y,char a);
//��x��y������Ϊ��ʼд��һ����׼�ַ���
extern void PutString(unsigned char x,unsigned char y,char *p);
//��x��y������Ϊ���ģ�����һ��Բ�߻�ʵ��Բ
//extern void PutPixel(unsigned char x,unsigned char y);
//extern void Circle(unsigned char x,unsigned char y,unsigned char r,unsigned char mode);
//����һ����left��top��right��bottomΪ�����Խǵľ��ο����ʵ�ľ���
extern void Rectangle(unsigned char left,unsigned char top,unsigned char right,unsigned char bottom);
//����������ִ��ȫ��Ļ��������ǰ��ɫ
void ClrScreen(unsigned char Mode);

//���º����Լ�����������һ�㽨���û���Ҫ���ã������߼��û������б�д������ʾЧ���Լ�����Ӧ�ó���ʱʹ��
//
//extern unsigned char code Asii16[];		//8X16��ASII�ַ���
//extern unsigned char code GB32[];		//�Զ����32X29���ֿ�
//extern unsigned char code GB48[];		//�Զ����48X55���ֿ�

extern unsigned char X_Witch;					//�ַ�д��ʱ�Ŀ��
extern unsigned char Y_Witch;					//�ַ�д��ʱ�ĸ߶�
extern unsigned char Font_Wrod;				//����Ĵ�
extern unsigned char *Char_TAB;		//�ֿ�ָ��
//extern unsigned char Plot_Mode;				//��ͼģʽ
extern unsigned char BMP_Color;
extern unsigned char Char_Color;

//extern void Pos_Switch(unsigned int * x,unsigned int * y);
//extern void Writ_Dot(int x,int y,unsigned int Color);
//extern void Line_f(int s_x,int s_y,int e_x);
//extern unsigned int Get_Dot(int x,int y);
//extern void Clear_Dot(int x,int y);
//extern void Set_Dot_Addr(int x,int y);


#endif /* __MzL05_LCD_H */
