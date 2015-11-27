
#include "MzL105LCD.h"

extern unsigned char MzL05LCD_Asii0608[];			//6X8��ASII�ַ���
extern unsigned char MzL05LCD_Asii0816[];			//8X16��ASII�ַ���
extern unsigned char MzL05LCD_GB1616[];				//16*16�Զ���ĺ��ֿ�
unsigned char MzL05LCD_X_Witch;									//�ַ�д��ʱ�Ŀ��
unsigned char MzL05LCD_Y_Witch;									//�ַ�д��ʱ�ĸ߶�
unsigned char MzL05LCD_Font_Wrod;								//�����ÿ����ģռ�ö��ٸ��洢��Ԫ��
unsigned char *MzL05LCD_Char_TAB;						//�ֿ�ָ��


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
		MzL05LCD_RegWrite(0x01);
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
				MzL05LCD_X_Witch = 8;
				MzL05LCD_Y_Witch = 2;
				//Char_Color = Color;
				MzL05LCD_Char_TAB = (unsigned char *)(MzL05LCD_Asii0816 - 32*16);
		break;
		case 1: MzL05LCD_Font_Wrod = 6;	//ASII�ַ�B
				MzL05LCD_X_Witch = 6;
				MzL05LCD_Y_Witch = 1;
				//Char_Color = Color;
				MzL05LCD_Char_TAB = (unsigned char *)(MzL05LCD_Asii0608 - (32*6));
		break;		
		case 2: MzL05LCD_Font_Wrod = 32;	//����A
				MzL05LCD_X_Witch = 16;
				MzL05LCD_Y_Witch = 2;
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
void MzL05LCD_PutChar(unsigned char x,unsigned char y,char a)       
{
	unsigned char i,j;
	unsigned char *p_data; 
	p_data = MzL05LCD_Char_TAB + a*MzL05LCD_Font_Wrod;	//Ҫд�ַ����׵�ַ
	x = x+1;							//Edit by xinqiang
	for(j=0;j<MzL05LCD_Y_Witch;j++)
	{
		if((y+j) < (Dis_Y_MAX/8))
		MzL05LCD_RegWrite(y+j+0xb0);
		MzL05LCD_RegWrite(((x&0xf0) >> 4)|0x10);	//���ó�ʼ��ַ 
		MzL05LCD_RegWrite(x&0x0f);
		for(i=0;i<MzL05LCD_X_Witch;i++)
		{  
			if(((x+i) < Dis_X_MAX)&&((y+j) < (Dis_Y_MAX/8)))
			MzL05LCD_DataWrite(*p_data++);
		}
	}
}

//========================================================================
// ����: void PutString(unsigned char x,unsigned char y,char *p)
// ����: ��x��yΪ��ʼ���괦д��һ����׼�ַ�
// ����: x  X������     y  Y������(0~7ҳ)
//		 p  Ҫ��ʾ���ַ���  
// ����: ��
// ��ע: ���������Դ���ASCII�ַ�����ʾ
// �汾:
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

