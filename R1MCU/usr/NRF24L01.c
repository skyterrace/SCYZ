#include "NRF24L01.h"
#include "clock_calendar.h"

unsigned char TX_ADDRESS0[TX_ADR_WIDTH]; // Define a static TX address
unsigned char TX_ADDRESS1[TX_ADR_WIDTH]; // Define a static TX address
unsigned char TX_ADDRESS2[TX_ADR_WIDTH]; // Define a static TX address
unsigned char TX_ADDRESS3[TX_ADR_WIDTH]; // Define a static TX address
unsigned char TX_ADDRESS4[TX_ADR_WIDTH]; // Define a static TX address
unsigned char TX_ADDRESS5[TX_ADR_WIDTH]; // Define a static TX address

unsigned char rx_buf[TX_PLOAD_WIDTH];
unsigned char tx_buf[TX_PLOAD_WIDTH];
unsigned char status_buf[TX_PLOAD_WIDTH];
unsigned char flag,nrf_baud,nrf_Pipe,nrf_Pipe_r;


/****************************************************************************
* ��    �ƣ�void MODE_CE(unsigned char a)
* ��    �ܣ�NRF24L01 ��/��ģʽ��Чѡ��
* ��ڲ�����a:  1��NRF24L01 ��/����Ч   0����
* ���ڲ�������
* ˵    ����
* ���÷�����MODE_CE(1); 
****************************************************************************/  
void MODE_CE(unsigned char a){			            //NRF24L01 MODE-CE 
	if(a==1) GPIO_SetBits(GPIOC, GPIO_Pin_4);	    //On
	else GPIO_ResetBits(GPIOC, GPIO_Pin_4);			//Off
}


/****************************************************************************
* ��    �ƣ�void SPI2_NRF24L01_Init(void)
* ��    �ܣ�NRF24L01 SPI2�ӿڳ�ʼ��
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷�����SPI2_NRF24L01_Init(); 
****************************************************************************/  
void SPI1_NRF24L01_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;	   
 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);	   //ʹ��SPI1����ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA ,ENABLE);


	/* ���� SPI1 ����: SCK, MISO and MOSI��PA5, PA6, PA7) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;          //���ù��ܣ����죩���  SPI2
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* ����SPI1 NRF24L01+Ƭѡ  PA4 */						   //PB0--NRF24L01+Ƭѡ	 PB12--VS1003Ƭѡ
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;//|GPIO_Pin_12;			  					 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		   //���ģʽ����ٶ�50MHz
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		   //ͨ���������ģʽ
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  
  /* ����NRF24L01+ ģʽѡ��  PC4 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;                //NRF24L01  MODE-CE
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		   //���ģʽ����ٶ�50MHz
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		   //ͨ���������ģʽ
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  /* ����NRF24L01+ �ж��źŲ������ӵ�  PC5 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;		 		  //NRF24L01 IRQ 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			  //��������ģʽ
  GPIO_Init(GPIOC, &GPIO_InitStructure);


  //��ֹSPI1 NRF24L01+��Ƭѡ��
  NotSelect_NRF();

  /* SPI1 ���� */ 
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   //ȫ˫��  
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						   //��ģʽ
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					   //8λ
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;						   //ʱ�Ӽ��� ����״̬ʱ��SCK���ֵ͵�ƽ
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;						   //ʱ����λ ���ݲ����ӵ�һ��ʱ�ӱ��ؿ�ʼ
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							   //��������NSS
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;  //�����ʿ��� SYSCLK/16
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;				   //���ݸ�λ��ǰ
  SPI_InitStructure.SPI_CRCPolynomial = 7;							   //CRC����ʽ�Ĵ�����ʼֵΪ7 
  SPI_Init(SPI1, &SPI_InitStructure);
  
  /* ʹ��SPI1  */
  SPI_Cmd(SPI1, ENABLE);   
}


/****************************************************************************
* ��    �ƣ�unsigned char SPI2_NRF_SendByte(unsigned char byte)
* ��    �ܣ�ͨ��SPI2 ����һ���ֽڵ����ݡ�
* ��ڲ�����byte�� 	���͵�����
* ���ڲ��������յ����ֽ�
* ˵    ����
* ���÷�����SPI2_NRF_SendByte(data1); 
****************************************************************************/  
unsigned char SPI1_NRF_SendByte(unsigned char byte)
{
  /* ѭ����ⷢ�ͻ������Ƿ��ǿ� */
  while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

  /* ͨ��SPI2���跢������ */
  SPI_I2S_SendData(SPI1, byte);

  /* �ȴ��������ݣ�ѭ�����������ݻ����� */
  while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

  /* ���ض��������� */
  return SPI_I2S_ReceiveData(SPI1);
}



/****************************************************************************
* ��    �ƣ�unsigned char SPI_RW_Reg(unsigned char data1,unsigned char data2) 
* ��    �ܣ�ͨ��SPI2 �����ֽ�д�뵽NRF24L01+ָ���ļĴ����
* ��ڲ�����data1�� 	NRF24L01�Ĵ���
			data2: 	    ���ֽ�����
* ���ڲ��������յ����ֽ�
* ˵    ����
* ���÷�����SPI_RW_Reg(WRITE_REG1 + EN_AA, 0x3f);  
****************************************************************************/  
unsigned char SPI_RW_Reg(unsigned char data1,unsigned char data2) 
{ 
  unsigned int Data = 0;   
  Select_NRF();			    			 //ѡ��NRF24L01Ƭѡ
  Data=SPI1_NRF_SendByte(data1);		 //ָ��NRF24L01�Ĵ���
  SPI1_NRF_SendByte(data2);				 //д������ 
  NotSelect_NRF(); 						 //��ֹNRF24L01Ƭѡ
  return(Data);							 //����NRF24L01 д�Ĵ�����״̬��Ϣ
 
}  


/****************************************************************************
* ��    �ƣ�unsigned char SPI_Write_Buf(BYTE reg, BYTE *pBuf, BYTE bytes)
* ��    �ܣ�ͨ��SPI2 �������������д�뵽NRF24L01+ָ���ļĴ����
* ��ڲ�����reg�� 	NRF24L01�Ĵ���
			pBuf: 	����
		    bytes��	д����ֽ���
* ���ڲ��������յ����ֽ�
* ˵    ����
* ���÷�����SPI_Write_Buf(WRITE_REG1 + RX_ADDR_P0, TX_ADDRESS0, TX_ADR_WIDTH);
****************************************************************************/  
unsigned char SPI_Write_Buf(BYTE reg, BYTE *pBuf, BYTE bytes)
{
	unsigned char status,byte_ctr;

  	Select_NRF();                     //ѡ��NRF24L01Ƭѡ
  	status=SPI1_NRF_SendByte(reg);	  //ָ��NRF24L01�Ĵ���
  	
  	for(byte_ctr=0; byte_ctr<bytes; byte_ctr++)    //д��ָ�����ȵ����� 
    { 
		SPI1_NRF_SendByte(*pBuf++);  	
	}	
  	NotSelect_NRF();                  //��ֹNRF24L01Ƭѡ
  	return(status);          		  //����NRF24L01 д�Ĵ�����״̬��Ϣ
}



/****************************************************************************
* ��    �ƣ�unsigned char SPI_Read(BYTE reg)
* ��    �ܣ�ͨ��SPI2 ��NRF24L01+ָ���ļĴ��������һ���ֽڡ�
* ��ڲ�����reg�� 	NRF24L01�Ĵ��� 		
* ���ڲ�����ָ��NRF24L01�Ĵ�����״̬��Ϣ
* ˵    ����
* ���÷�����status=SPI_Read(READ_REG1+STATUS);
****************************************************************************/  
unsigned char SPI_Read(BYTE reg)
{
  unsigned char Data;
  Select_NRF();						    //ѡ��NRF24L01Ƭѡ
  SPI1_NRF_SendByte(reg);			    //ָ��NRF24L01�Ĵ���
  Data=SPI1_NRF_SendByte(0);			//��������
  NotSelect_NRF(); 					    //��ֹNRF24L01Ƭѡ
  return (Data); 
}


/****************************************************************************
* ��    �ƣ�unsigned char SPI_Read_Buf(BYTE reg, BYTE *pBuf, BYTE bytes)
* ��    �ܣ�ͨ��SPI2 ��NRF24L01+ָ���ļĴ���������ݶ���ָ�����ȵ�ָ���������
* ��ڲ�����reg�� 	NRF24L01�Ĵ���
			pBuf��  ����
			bytes�� ���� 		
* ���ڲ�����ָ��NRF24L01�Ĵ�����״̬��Ϣ
* ˵    ����
* ���÷�����SPI_Read_Buf(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH);
****************************************************************************/ 
unsigned char SPI_Read_Buf(BYTE reg, BYTE *pBuf, BYTE bytes)
{
	unsigned char status,i;

  	Select_NRF();              			//ѡ��NRF24L01Ƭѡ
  	status=SPI1_NRF_SendByte(reg);	   	//����ָ��NRF24L01�Ĵ�����״̬��Ϣ
  	for(i=0; i<bytes; i++)              //����ָ�����ȵ����� 
    {  		
		pBuf[i]=SPI1_NRF_SendByte(0);  		
	}	
  	NotSelect_NRF();                    //��ֹNRF24L01Ƭѡ 
  	return(status);          		    //����ָ��NRF24L01�Ĵ�����״̬��Ϣ
}



// /****************************************************************************
// * ��    �ƣ�delay_ms(unsigned int x)
// * ��    �ܣ���ʱ����Ϊ1�������
// * ��ڲ�����x   ��ʱ�ĺ�����
// * ���ڲ�������
// * ˵    ������
// * ���÷�����delay_ms(1);
// ****************************************************************************/  
// void delay_ms(unsigned int x)
// {
//     unsigned int i,j;
//     i=0;
//     for(i=0;i<x;i++)
//     {
//        j=108;
//        while(j--);
//     }
// }

/****************************************************************************
* ��    �ƣ�RX_Mode(void)
* ��    �ܣ�����NRF24L01+�Ľ���ģʽ
* ��ڲ�������
* ���ڲ�������
* ˵    ����������6������ͨ����ַ�����ݿ���32�������Զ�Ӧ��6������ͨ��ʹ�ܡ�
*			��ƵƵ��0��16λCRC���շ��жϡ�����0dB�ȵ�
* ���÷�����RX_Mode();
****************************************************************************/

void RX_Mode(void)
{
	nrf_baud=1;							//Ĭ������2Mbps
	TX_ADDRESS0[0]=0x00;	            //ͨ��0 �����ַ
	TX_ADDRESS0[1]=0x00;
	TX_ADDRESS0[2]=0x00;
	TX_ADDRESS0[3]=0x00;
	TX_ADDRESS0[4]=0x05;	 

	TX_ADDRESS1[0]=0x00;				//ͨ��1 �����ַ
	TX_ADDRESS1[1]=0x00;
	TX_ADDRESS1[2]=0x00;
	TX_ADDRESS1[3]=0x00;
	TX_ADDRESS1[4]=0x05;

	TX_ADDRESS2[0]=0x00;			   //ͨ��2 �����ַ
	TX_ADDRESS2[1]=0x00;
	TX_ADDRESS2[2]=0x00;
	TX_ADDRESS2[3]=0x00;
	TX_ADDRESS2[4]=0x05;

	TX_ADDRESS3[0]=0x00;			   //ͨ��3 �����ַ
	TX_ADDRESS3[1]=0x00;
	TX_ADDRESS3[2]=0x00;
	TX_ADDRESS3[3]=0x00;
	TX_ADDRESS3[4]=0x05;

	TX_ADDRESS4[0]=0x00;			   //ͨ��4 �����ַ
	TX_ADDRESS4[1]=0x00;
	TX_ADDRESS4[2]=0x00;
	TX_ADDRESS4[3]=0x00;
	TX_ADDRESS4[4]=0x05;

	TX_ADDRESS5[0]=0x00;			   //ͨ��5 �����ַ
	TX_ADDRESS5[1]=0x00;
	TX_ADDRESS5[2]=0x00;
	TX_ADDRESS5[3]=0x00;
	TX_ADDRESS5[4]=0x05;
	MODE_CE(0);				 

	SPI_Write_Buf(WRITE_REG1 + RX_ADDR_P0, TX_ADDRESS0, TX_ADR_WIDTH); //����ͨ��0���յ�ַ�����5���ֽڣ� �˴����յ�ַ�ͷ��͵�ַ��ͬ 
// 	SPI_Write_Buf(WRITE_REG1 + RX_ADDR_P1, TX_ADDRESS1, TX_ADR_WIDTH); //����ͨ��1���յ�ַ�����5���ֽڣ� �˴����յ�ַ�ͷ��͵�ַ��ͬ 
//     SPI_Write_Buf(WRITE_REG1 + RX_ADDR_P2, TX_ADDRESS2, 1);	//����ͨ��2���յ�ַ��5���ֽڣ� ���ֽ���TX_ADDRESS1[39:8]��ͬ�����ֽ�ͬTX_ADDRESS2[0]  
// 	SPI_Write_Buf(WRITE_REG1 + RX_ADDR_P3, TX_ADDRESS3, 1);	//����ͨ��3���յ�ַ��5���ֽڣ� ���ֽ���TX_ADDRESS1[39:8]��ͬ�����ֽ�ͬTX_ADDRESS3[0]  
// 	SPI_Write_Buf(WRITE_REG1 + RX_ADDR_P4, TX_ADDRESS4, 1);	//����ͨ��4���յ�ַ��5���ֽڣ� ���ֽ���TX_ADDRESS1[39:8]��ͬ�����ֽ�ͬTX_ADDRESS4[0]  
// 	SPI_Write_Buf(WRITE_REG1 + RX_ADDR_P5, TX_ADDRESS5, 1);	//����ͨ��5���յ�ַ��5���ֽڣ� ���ֽ���TX_ADDRESS1[39:8]��ͬ�����ֽ�ͬTX_ADDRESS5[0]  

	SPI_RW_Reg(WRITE_REG1 + RX_PW_P0, TX_PLOAD_WIDTH); // ��������ͨ��0��Ч���ݿ���32   ��Χ1-32
// 	SPI_RW_Reg(WRITE_REG1 + RX_PW_P1, TX_PLOAD_WIDTH); // ��������ͨ��1��Ч���ݿ���32   ��Χ1-32
// 	SPI_RW_Reg(WRITE_REG1 + RX_PW_P2, TX_PLOAD_WIDTH); // ��������ͨ��2��Ч���ݿ���32   ��Χ1-32
// 	SPI_RW_Reg(WRITE_REG1 + RX_PW_P3, TX_PLOAD_WIDTH); // ��������ͨ��3��Ч���ݿ���32   ��Χ1-32
// 	SPI_RW_Reg(WRITE_REG1 + RX_PW_P4, TX_PLOAD_WIDTH); // ��������ͨ��4��Ч���ݿ���32   ��Χ1-32
// 	SPI_RW_Reg(WRITE_REG1 + RX_PW_P5, TX_PLOAD_WIDTH); // ��������ͨ��5��Ч���ݿ���32   ��Χ1-32
  	
// 	SPI_RW_Reg(WRITE_REG1 + EN_AA, 0x3f);      // ʹ��ͨ��0-ͨ��5�����Զ�Ӧ��
//   	SPI_RW_Reg(WRITE_REG1 + EN_RXADDR, 0x3f);  // ����ͨ��0-5 ʹ��
	SPI_RW_Reg(WRITE_REG1 + EN_AA, 0x00);      // ʹ��ͨ��0-ͨ��5�����Զ�Ӧ��
  SPI_RW_Reg(WRITE_REG1 + EN_RXADDR, 0x01);  // ����ͨ��0-5 ʹ��
	SPI_RW_Reg(WRITE_REG1 + RF_CH, 1);         // ѡ����Ƶ����Ƶ��0   ��Χ0-127  
  	
	if(nrf_baud==0) SPI_RW_Reg(WRITE_REG1 + RF_SETUP, 0x0f);   // 0db, 2MPS   ��Ƶ�Ĵ���   ��������bit5:bit3		   ���书��bit2-bit1   
                                                               //                           00: 1M BPS	                 00:-18dB
																															 //                           01: 2M BPS	                 01:-12dB	   
																															 //                           10: 250K BPS	             10:-6dB
																															 //                           11������                     11:0dB  
															   
	else SPI_RW_Reg(WRITE_REG1 + RF_SETUP, 0x07);   // 0db, 1MPS
  	SPI_RW_Reg(WRITE_REG1 + CONFIG, 0x3f);     // bit6 �����жϲ���ʱ��IRQ���Ų����͵�ƽ
																							 // bit5 �����жϲ���ʱ��IRQ���Ų����͵�ƽ
																							 // bit4 ����ظ����ʹ������ʱ IRQ���Ų����͵�ƽ 
																							 // bit3 CRCУ������
																							 // bit2 16λCRC
																							 // bit1 �ϵ�
																							 // bit0 ����ģʽ  	

// 	SPI_RW_Reg(WRITE_REG1 + CONFIG, 0x0A); 

  	MODE_CE(1);								   // ʹ�ܽ���ģʽ					
}


/****************************************************************************
* ��    �ƣ�TX_Mode(void)
* ��    �ܣ�����NRF24L01+�ķ���ģʽ
* ��ڲ�������
* ���ڲ�������
* ˵    ����������6������ͨ����ַ����ƵƵ��0��16λCRC���շ��жϡ�����0dB�ȵ�
* ���÷�����TX_Mode();
****************************************************************************/
void TX_Mode(void)
{	
	NotSelect_NRF();
	MODE_CE(0);
	nrf_Pipe=0;

  	
//   	SPI_RW_Reg(WRITE_REG1 + SETUP_RETR, 0x1a); // �Զ��ط���ʱ500us + 86us,  �Զ��ط�����10��
	SPI_RW_Reg(WRITE_REG1 + SETUP_RETR, 0x00);                         // �Զ��ط���ʱ��500us + 86us, �Զ��ط�10��
  

	if(nrf_Pipe==0) SPI_Write_Buf(WRITE_REG1 + TX_ADDR, TX_ADDRESS0, TX_ADR_WIDTH);         //����ͨ��0���͵�ַ�����5���ֽ�
	else if(nrf_Pipe==1) SPI_Write_Buf(WRITE_REG1 + TX_ADDR, TX_ADDRESS1, TX_ADR_WIDTH);    //����ͨ��1���͵�ַ�����5���ֽ�
	else if(nrf_Pipe==2) SPI_Write_Buf(WRITE_REG1 + TX_ADDR, TX_ADDRESS2, TX_ADR_WIDTH);    //����ͨ��2���͵�ַ�����5���ֽ�
	else if(nrf_Pipe==3) SPI_Write_Buf(WRITE_REG1 + TX_ADDR, TX_ADDRESS3, TX_ADR_WIDTH);    //����ͨ��3���͵�ַ�����5���ֽ�
	else if(nrf_Pipe==4) SPI_Write_Buf(WRITE_REG1 + TX_ADDR, TX_ADDRESS4, TX_ADR_WIDTH);    //����ͨ��4���͵�ַ�����5���ֽ�
	else if(nrf_Pipe==5) SPI_Write_Buf(WRITE_REG1 + TX_ADDR, TX_ADDRESS5, TX_ADR_WIDTH);    //����ͨ��5���͵�ַ�����5���ֽ�
	
	if(nrf_Pipe==0) SPI_Write_Buf(WRITE_REG1 + RX_ADDR_P0, TX_ADDRESS0, TX_ADR_WIDTH);      // ��0ͨ���Ľ��յ�ַ����Ϊ 0ͨ���ķ����ַ
	else if(nrf_Pipe==1) SPI_Write_Buf(WRITE_REG1 + RX_ADDR_P0, TX_ADDRESS1, TX_ADR_WIDTH); // ��0ͨ���Ľ��յ�ַ����Ϊ 1ͨ���ķ����ַ
	else if(nrf_Pipe==2) SPI_Write_Buf(WRITE_REG1 + RX_ADDR_P0, TX_ADDRESS2, TX_ADR_WIDTH);	// ��0ͨ���Ľ��յ�ַ����Ϊ 2ͨ���ķ����ַ
	else if(nrf_Pipe==3) SPI_Write_Buf(WRITE_REG1 + RX_ADDR_P0, TX_ADDRESS3, TX_ADR_WIDTH);	// ��0ͨ���Ľ��յ�ַ����Ϊ 3ͨ���ķ����ַ
	else if(nrf_Pipe==4) SPI_Write_Buf(WRITE_REG1 + RX_ADDR_P0, TX_ADDRESS4, TX_ADR_WIDTH);	// ��0ͨ���Ľ��յ�ַ����Ϊ 4ͨ���ķ����ַ
	else if(nrf_Pipe==5) SPI_Write_Buf(WRITE_REG1 + RX_ADDR_P0, TX_ADDRESS5, TX_ADR_WIDTH);	// ��0ͨ���Ľ��յ�ַ����Ϊ 5ͨ���ķ����ַ


  SPI_RW_Reg(WRITE_REG1 + CONFIG, 0x0e);     // bit6 �����жϲ���ʱ��IRQ���Ų����͵�ƽ
	                                           // bit5 �����жϲ���ʱ��IRQ���Ų����͵�ƽ
											   // bit4 ����ظ����ʹ������ʱ IRQ���Ų����͵�ƽ 
											   // bit3 CRCУ������
											   // bit2 16λCRC
											   // bit1 �ϵ�
											   // bit0 ����ģʽ 
// 												 SPI_RW_Reg(WRITE_REG1 + CONFIG, 0x0B); 
// 	temp = SPI_Read(READ_REG1 + SETUP_AW);
	MODE_CE(1);								   // ʹ�ܷ���ģʽ

}


/****************************************************************************
* ��    �ƣ�USB_To_NRF_Send_Data(uint8_t* data_buffer, uint8_t Nb_bytes)
* ��    �ܣ���������USB���ջ�������32�ֽڵ�����ͨ��NRF24L01+���ͳ�ȥ
* ��ڲ�����data_buffer   USB���ջ�����
			Nb_bytes	  USB������յ����ֽ���
* ���ڲ�������
* ˵    ���������յ���USB���⴮������С��32������Ч������Ŀռ���0����
* ���÷�����RX_Mode();
****************************************************************************/
void NRF_Send_Data(uint8_t* data_buffer, uint8_t Nb_bytes)
{  
	uchar i=0;  
	MODE_CE(0);								 //NRF ģʽ����	 

	SPI_RW_Reg(WRITE_REG1+STATUS,0xff);	     //����״̬�Ĵ�����ʼ��
	SPI_RW_Reg(0xe1,0);						 //���TX FIFO�Ĵ���
	SPI_RW_Reg(0xe2,0);		    			 //���RX FIFO�Ĵ���
	TX_Mode();								 //����Ϊ����ģʽ
	Delay_ms(1);
	if(Nb_bytes<32){						 //�����յ���USB���⴮������С��32������Ч������Ŀռ���0����
		for(i=Nb_bytes;i<32;i++) data_buffer[i]=0;
	}
	MODE_CE(0);
  	SPI_Write_Buf(WR_TX_PLOAD, data_buffer, TX_PLOAD_WIDTH);        //����32�ֽڵĻ��������ݵ�NRF24L01
	MODE_CE(1);														//����10us���ϣ������ݷ��ͳ�ȥ
	Delay_ms(1);
	
}

