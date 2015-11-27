#ifndef __DISP_H
#define __DISP_H

#include "stm32f10x.h"

//#define DISP_TYPE_TFT
#define DISP_TYPE_MzL05LCD
// #define DISP_TYPE_ST7920

#ifdef DISP_TYPE_TFT   //���ʹ��TFT��ʾģ��

#define Bank1_LCD_D    ((uint32_t)0x60020000)    //��ʾ�����ݵ�ַ	  
#define Bank1_LCD_C    ((uint32_t)0x60000000)	 //��ʾ��ָ���ַ

#include "stm32f10x_fsmc.h"

void TFT_Init(void);
void LCD_WR_REG(unsigned int index);
void LCD_WR_CMD(unsigned int index,unsigned int val);
void LCD_WR_Data(unsigned int val);

void LCD_test(void);
void LCD_clear(unsigned int p);
void lcd_wr_zf(u16 StartX, u16 StartY, u16 X, u16 Y, u16 Color, u8 Dir, u8 *chr); 
unsigned char *num_pub(unsigned int a);
void TFT_Write_Char(u16 StartX, u16 StartY, u16 X, u16 Y, u16 Color, u8 Dir, u8 chr);
void TFT_ShowText(uint8_t Line,uint8_t Col,uint16_t Color,char *Str);
void TFT_SetCursor(uint8_t Line,uint8_t Col,uint16_t Color);
void TFT_Clear(void);

extern unsigned char zm9[];
extern unsigned char zm8[];
extern unsigned char zm7[];
extern unsigned char zm6[];
extern unsigned char zm5[];
extern unsigned char zm4[];
extern unsigned char zm3[];
extern unsigned char zm2[];
extern unsigned char zm1[];
extern unsigned char zm0[];
extern unsigned char ASCII_Font[];
extern unsigned char ASCII1832[];
// extern unsigned char a1[];

extern unsigned char zf2[];
extern unsigned char zf3[];	
// extern unsigned char pic[];
// extern unsigned char a2[];
// extern unsigned char a3[];

unsigned int LCD_RD_data(void);
void lcd_rst(void);
void Delay(__IO uint32_t nCount);
void FSMC_LCD_Init(void);

#elif defined	 DISP_TYPE_MzL05LCD	//���ʹ��MzL05LCD��ʾģ��

#define MzL05LCD_CS	GPIOC,GPIO_Pin_13
#define MzL05LCD_RES GPIOB,GPIO_Pin_15
#define MzL05LCD_A0  GPIOB,GPIO_Pin_14
#define MzL05LCD_CLK GPIOB,GPIO_Pin_13
#define MzL05LCD_SDI GPIOC,GPIO_Pin_0

//˽�к�������
void MzL05LCD_Init(void);
void SPI_Config(void);
void MzL05LCD_TimeDelay(int Time);
void MzL05LCD_RegWrite(unsigned char Command);
void MzL05LCD_DataWrite(unsigned char Dat);
void MzL05LCD_Fill(unsigned char Data);
void MzL05LCD_ClrScreen(void);
void MzL05LCD_FontSet(unsigned char Font_NUM);
void MzL05LCD_PutChar(unsigned char x,unsigned char y,char a,unsigned char inv);
void MzL05LCD_PutString(unsigned char x,unsigned char y,char *p,unsigned char inv);



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



#elif defined	 DISP_TYPE_ST7920	//���ʹ��ST7920��ʾģ��

void SPI2_Config(void);
unsigned char SPI2_WriteByte(unsigned char data);
void LCD_Config(void);
void WriteLCD(uint8_t start, uint8_t ddata);
void delay(uint16_t n);
void LCD_Show(void);
void LCD_ShowText(uint8_t x,uint8_t y, char *text);


#endif

//���ú�������
void DispInit(void);
void ShowDebugInfo(uint8_t line,uint8_t col, uint8_t status);
void ShowStatusInfo(uint8_t line,uint8_t col, char* info);
void ShowBatteryInfo(uint8_t val);
void ShowCurrentTime(void);
void ShowDefaultScreen(unsigned char cPower,unsigned char cWorkMode);
void SetCurrentDateTime(void);
void SetWorkingTime(void);
void SetParam(void);
void DispClear(void);
void DispClose(unsigned char bClose);

#endif /* __DISP_H */
