#include "stm32f10x.h"
#include "Lcd_Driver.h"
#include "GUI.h"
//#include "SysTickDelay.h"
#include "clock_calendar.h"
#include "Font.h"
#include "eeprom.h"
#include "R1DEF.h"
#include "key.h"
//从ILI93xx读出的数据为GBR格式，而我们写入的时候为RGB格式。
//通过该函数转换
//c:GBR格式的颜色值
//返回值：RGB格式的颜色值
u16 LCD_BGR2RGB(u16 c)
{
  u16  r,g,b,rgb;   
  b=(c>>0)&0x1f;
  g=(c>>5)&0x3f;
  r=(c>>11)&0x1f;	 
  rgb=(b<<11)+(g<<5)+(r<<0);		 
  return(rgb);

}




void Gui_Circle(u16 X,u16 Y,u16 R,u16 fc) 
{//Bresenham算法 
    unsigned short  a,b; 
    int c; 
    a=0; 
    b=R; 
    c=3-2*R; 
    while (a<b) 
    { 
        Gui_DrawPoint(X+a,Y+b,fc);     //        7 
        Gui_DrawPoint(X-a,Y+b,fc);     //        6 
        Gui_DrawPoint(X+a,Y-b,fc);     //        2 
        Gui_DrawPoint(X-a,Y-b,fc);     //        3 
        Gui_DrawPoint(X+b,Y+a,fc);     //        8 
        Gui_DrawPoint(X-b,Y+a,fc);     //        5 
        Gui_DrawPoint(X+b,Y-a,fc);     //        1 
        Gui_DrawPoint(X-b,Y-a,fc);     //        4 

        if(c<0) c=c+4*a+6; 
        else 
        { 
            c=c+4*(a-b)+10; 
            b-=1; 
        } 
       a+=1; 
    } 
    if (a==b) 
    { 
        Gui_DrawPoint(X+a,Y+b,fc); 
        Gui_DrawPoint(X+a,Y+b,fc); 
        Gui_DrawPoint(X+a,Y-b,fc); 
        Gui_DrawPoint(X-a,Y-b,fc); 
        Gui_DrawPoint(X+b,Y+a,fc); 
        Gui_DrawPoint(X-b,Y+a,fc); 
        Gui_DrawPoint(X+b,Y-a,fc); 
        Gui_DrawPoint(X-b,Y-a,fc); 
    } 
	
} 
//画线函数，使用Bresenham 画线算法
void Gui_DrawLine(u16 x0, u16 y0,u16 x1, u16 y1,u16 Color)   
{
int dx,             // difference in x's
    dy,             // difference in y's
    dx2,            // dx,dy * 2
    dy2, 
    x_inc,          // amount in pixel space to move during drawing
    y_inc,          // amount in pixel space to move during drawing
    error,          // the discriminant i.e. error i.e. decision variable
    index;          // used for looping	


	Lcd_SetXY(x0,y0);
	dx = x1-x0;//计算x距离
	dy = y1-y0;//计算y距离

	if (dx>=0)
	{
		x_inc = 1;
	}
	else
	{
		x_inc = -1;
		dx    = -dx;  
	} 
	
	if (dy>=0)
	{
		y_inc = 1;
	} 
	else
	{
		y_inc = -1;
		dy    = -dy; 
	} 

	dx2 = dx << 1;
	dy2 = dy << 1;

	if (dx > dy)//x距离大于y距离，那么每个x轴上只有一个点，每个y轴上有若干个点
	{//且线的点数等于x距离，以x轴递增画点
		// initialize error term
		error = dy2 - dx; 

		// draw the line
		for (index=0; index <= dx; index++)//要画的点数不会超过x距离
		{
			//画点
			Gui_DrawPoint(x0,y0,Color);
			
			// test if error has overflowed
			if (error >= 0) //是否需要增加y坐标值
			{
				error-=dx2;

				// move to next line
				y0+=y_inc;//增加y坐标值
			} // end if error overflowed

			// adjust the error term
			error+=dy2;

			// move to the next pixel
			x0+=x_inc;//x坐标值每次画点后都递增1
		} // end for
	} // end if |slope| <= 1
	else//y轴大于x轴，则每个y轴上只有一个点，x轴若干个点
	{//以y轴为递增画点
		// initialize error term
		error = dx2 - dy; 

		// draw the line
		for (index=0; index <= dy; index++)
		{
			// set the pixel
			Gui_DrawPoint(x0,y0,Color);

			// test if error overflowed
			if (error >= 0)
			{
				error-=dy2;

				// move to next line
				x0+=x_inc;
			} // end if error overflowed

			// adjust the error term
			error+=dx2;

			// move to the next pixel
			y0+=y_inc;
		} // end for
	} // end else |slope| > 1
}



void Gui_box(u16 x, u16 y, u16 w, u16 h,u16 bc)
{
	Gui_DrawLine(x,y,x+w,y,0xEF7D);
	Gui_DrawLine(x+w-1,y+1,x+w-1,y+1+h,0x2965);
	Gui_DrawLine(x,y+h,x+w,y+h,0x2965);
	Gui_DrawLine(x,y,x,y+h,0xEF7D);
    Gui_DrawLine(x+1,y+1,x+1+w-2,y+1+h-2,bc);
}
void Gui_box2(u16 x,u16 y,u16 w,u16 h, u8 mode)
{
	if (mode==0)	{
		Gui_DrawLine(x,y,x+w,y,0xEF7D);
		Gui_DrawLine(x+w-1,y+1,x+w-1,y+1+h,0x2965);
		Gui_DrawLine(x,y+h,x+w,y+h,0x2965);
		Gui_DrawLine(x,y,x,y+h,0xEF7D);
		}
	if (mode==1)	{
		Gui_DrawLine(x,y,x+w,y,0x2965);
		Gui_DrawLine(x+w-1,y+1,x+w-1,y+1+h,0xEF7D);
		Gui_DrawLine(x,y+h,x+w,y+h,0xEF7D);
		Gui_DrawLine(x,y,x,y+h,0x2965);
	}
	if (mode==2)	{
		Gui_DrawLine(x,y,x+w,y,0xffff);
		Gui_DrawLine(x+w-1,y+1,x+w-1,y+1+h,0xffff);
		Gui_DrawLine(x,y+h,x+w,y+h,0xffff);
		Gui_DrawLine(x,y,x,y+h,0xffff);
	}
}


/**************************************************************************************
功能描述: 在屏幕显示一凸起的按钮框
输    入: u16 x1,y1,x2,y2 按钮框左上角和右下角坐标
输    出: 无
**************************************************************************************/
void DisplayButtonDown(u16 x1,u16 y1,u16 x2,u16 y2)
{
	Gui_DrawLine(x1,  y1,  x2,y1, GRAY2);  //H
	Gui_DrawLine(x1+1,y1+1,x2,y1+1, GRAY1);  //H
	Gui_DrawLine(x1,  y1,  x1,y2, GRAY2);  //V
	Gui_DrawLine(x1+1,y1+1,x1+1,y2, GRAY1);  //V
	Gui_DrawLine(x1,  y2,  x2,y2, WHITE);  //H
	Gui_DrawLine(x2,  y1,  x2,y2, WHITE);  //V
}

/**************************************************************************************
功能描述: 在屏幕显示一凹下的按钮框
输    入: u16 x1,y1,x2,y2 按钮框左上角和右下角坐标
输    出: 无
**************************************************************************************/
void DisplayButtonUp(u16 x1,u16 y1,u16 x2,u16 y2)
{
	Gui_DrawLine(x1,  y1,  x2,y1, WHITE); //H
	Gui_DrawLine(x1,  y1,  x1,y2, WHITE); //V
	
	Gui_DrawLine(x1+1,y2-1,x2,y2-1, GRAY1);  //H
	Gui_DrawLine(x1,  y2,  x2,y2, GRAY2);  //H
	Gui_DrawLine(x2-1,y1+1,x2-1,y2, GRAY1);  //V
    Gui_DrawLine(x2  ,y1  ,x2,y2, GRAY2); //V
}


void Gui_DrawFont_GBK16(u16 x, u16 y, u16 fc, u16 bc, u8 *s)
{
	unsigned char i,j;
	unsigned short k,x0;
	x0=x;

	while(*s) 
	{	
		if((*s) < 128) 
		{
			k=*s;
			if (k==13) 
			{
				x=x0;
				y+=16;
			}
			else 
			{
				if (k>32) k-=32; else k=0;
	
			    for(i=0;i<16;i++)
				for(j=0;j<8;j++) 
					{
				    	if(asc16[k*16+i]&(0x80>>j))	Gui_DrawPoint(x+j,y+i,fc);
						else 
						{
							if (fc!=bc) Gui_DrawPoint(x+j,y+i,bc);
						}
					}
				x+=8;
			}
			s++;
		}
			
		else 
		{
		

			for (k=0;k<hz16_num;k++) 
			{
			  if ((hz16[k].Index[0]==*(s))&&(hz16[k].Index[1]==*(s+1)))
			  { 
				    for(i=0;i<16;i++)
				    {
						for(j=0;j<8;j++) 
							{
						    	if(hz16[k].Msk[i*2]&(0x80>>j))	Gui_DrawPoint(x+j,y+i,fc);
								else {
									if (fc!=bc) Gui_DrawPoint(x+j,y+i,bc);
								}
							}
						for(j=0;j<8;j++) 
							{
						    	if(hz16[k].Msk[i*2+1]&(0x80>>j))	Gui_DrawPoint(x+j+8,y+i,fc);
								else 
								{
									if (fc!=bc) Gui_DrawPoint(x+j+8,y+i,bc);
								}
							}
				    }
				}
			  }
			s+=2;x+=16;
		} 
		
	}
}

void Gui_DrawFont_GBK24(u16 x, u16 y, u16 fc, u16 bc, u8 *s)
{
	unsigned char i,j;
	unsigned short k;

	while(*s) 
	{
		if( *s < 0x80 ) 
		{
			k=*s;
			if (k>32) k-=32; else k=0;

		    for(i=0;i<16;i++)
			for(j=0;j<8;j++) 
				{
			    	if(asc16[k*16+i]&(0x80>>j))	
					Gui_DrawPoint(x+j,y+i,fc);
					else 
					{
						if (fc!=bc) Gui_DrawPoint(x+j,y+i,bc);
					}
				}
			s++;x+=8;
		}
		else 
		{

			for (k=0;k<hz24_num;k++) 
			{
			  if ((hz24[k].Index[0]==*(s))&&(hz24[k].Index[1]==*(s+1)))
			  { 
				    for(i=0;i<24;i++)
				    {
						for(j=0;j<8;j++) 
							{
						    	if(hz24[k].Msk[i*3]&(0x80>>j))
								Gui_DrawPoint(x+j,y+i,fc);
								else 
								{
									if (fc!=bc) Gui_DrawPoint(x+j,y+i,bc);
								}
							}
						for(j=0;j<8;j++) 
							{
						    	if(hz24[k].Msk[i*3+1]&(0x80>>j))	Gui_DrawPoint(x+j+8,y+i,fc);
								else {
									if (fc!=bc) Gui_DrawPoint(x+j+8,y+i,bc);
								}
							}
						for(j=0;j<8;j++) 
							{
						    	if(hz24[k].Msk[i*3+2]&(0x80>>j))	
								Gui_DrawPoint(x+j+16,y+i,fc);
								else 
								{
									if (fc!=bc) Gui_DrawPoint(x+j+16,y+i,bc);
								}
							}
				    }
			  }
			}
			s+=2;x+=24;
		}
	}
}

void Gui_DrawFont_GBK32(u16 x, u16 y, u16 fc, u16 bc, u8 *s)
{
	unsigned char i,j;
	unsigned short k;

	while(*s) 
	{
		if( *s < 0x80 ) 
		{
			k=*s;
			if (k>32) k-=32; else k=0;

		    for(i=0;i<16;i++)
			for(j=0;j<8;j++) 
				{
			    	if(asc16[k*16+i]&(0x80>>j))	
					Gui_DrawPoint(x+j,y+i,fc);
					else 
					{
						if (fc!=bc) Gui_DrawPoint(x+j,y+i,bc);
					}
				}
			s++;x+=8;
		}
		else 
		{

			for (k=0;k<hz32_num;k++) 
			{
			  if ((hz32[k].Index[0]==*(s))&&(hz32[k].Index[1]==*(s+1)))
			  { 
				    for(i=0;i<32;i++)
				    {
						for(j=0;j<8;j++) 
							{
						    	if(hz32[k].Msk[i*4]&(0x80>>j))
								Gui_DrawPoint(x+j,y+i,fc);
								else 
								{
									if (fc!=bc) Gui_DrawPoint(x+j,y+i,bc);
								}
							}
						for(j=0;j<8;j++) 
							{
						    	if(hz32[k].Msk[i*4+1]&(0x80>>j))	Gui_DrawPoint(x+j+8,y+i,fc);
								else {
									if (fc!=bc) Gui_DrawPoint(x+j+8,y+i,bc);
								}
							}
						for(j=0;j<8;j++) 
							{
						    	if(hz32[k].Msk[i*4+2]&(0x80>>j))	
								Gui_DrawPoint(x+j+16,y+i,fc);
								else 
								{
									if (fc!=bc) Gui_DrawPoint(x+j+16,y+i,bc);
								}
							}
						for(j=0;j<8;j++) 
						{
								if(hz32[k].Msk[i*4+3]&(0x80>>j))	
							Gui_DrawPoint(x+j+24,y+i,fc);
							else 
							{
								if (fc!=bc) Gui_DrawPoint(x+j+24,y+i,bc);
							}
						}
				    }
			  }
			}
			s+=2;x+=32;
		}
	}
}

void Gui_DrawFont_Num32(u16 x, u16 y, u16 fc, u16 bc, u16 num)
{
	unsigned char i,j,k,c;
	//lcd_text_any(x+94+i*42,y+34,32,32,0x7E8,0x0,sz32,knum[i]);
//	w=w/8;

    for(i=0;i<32;i++)
	{
		for(j=0;j<4;j++) 
		{
			c=*(sz32+num*32*4+i*4+j);
			for (k=0;k<8;k++)	
			{
	
		    	if(c&(0x80>>k))	Gui_DrawPoint(x+j*8+k,y+i,fc);
				else {
					if (fc!=bc) Gui_DrawPoint(x+j*8+k,y+i,bc);
				}
			}
		}
	}
}

void Gui_DrawFont_Num32_Int(u16 x, u16 y, u16 fc, u16 bc, int16_t num, u8 len)
{
	unsigned char i;
	u16 k,c;
	k=0;
	if(num<0)
	{
		c=-num;
		Gui_DrawFont_Num32(x,y,fc,bc,14);
	}
	else
	{
		c=num;
		Gui_DrawFont_Num32(x,y,fc,bc,0);
	}
	
	for(i=len; i>0;i--)
	{
		k= c%10;
		
		c=c/10;
		
		Gui_DrawFont_Num32(x+(i)*26,y,fc,bc,k);
	}

}

void Gui_DrawDefaultScreen(unsigned char cWorkMode)
{
	Lcd_Clear(GRAY0);
//	Gui_DrawFont_GBK32(16,6,BLUE,GRAY0,"长度：");
//	Gui_DrawFont_GBK32(256,6,BLUE,GRAY0,"米");

//	Gui_DrawLine(95,49,224,49,BLACK);
//	Gui_DrawLine(95,49,95,178,BLACK);
//	Gui_DrawLine(96,178,224,178,BLACK);
//	Gui_DrawLine(224,49,224,178,BLACK);
	
	//MzL05LCD_PutString(0*MzL05LCD_X_Width,7*MzL05LCD_Y_Width,"setting",0x00);
	Gui_DrawFont_GBK16(216,200,BLUE,GRAY0,"setting");
	switch(cWorkMode)
	{
		case 1:
			//MzL05LCD_PutString(14*MzL05LCD_X_Width,7*MzL05LCD_Y_Width,"manual",0x00);
			Gui_DrawFont_GBK16(16,200,BLUE,GRAY0,"manual");
			break;
		case 2:
			//MzL05LCD_PutString(14*MzL05LCD_X_Width,7*MzL05LCD_Y_Width,"  home",0x00);
			Gui_DrawFont_GBK16(16,200,BLUE,GRAY0,"  home");
			break;
		case 3:
			//MzL05LCD_PutString(14*MzL05LCD_X_Width,7*MzL05LCD_Y_Width,"  loop",0x00);
			Gui_DrawFont_GBK16(16,200,BLUE,GRAY0,"  loop");
			break;
		default:
			//MzL05LCD_PutString(14*MzL05LCD_X_Width,7*MzL05LCD_Y_Width,"  auto",0x00);
				Gui_DrawFont_GBK16(16,200,BLUE,GRAY0,"  auto");
			break;
	}
}

void Gui_SetParam(void)
{
	//unsigned char box_x_count, box_y_count, box_x_len, box_y_len;
	unsigned char box_x_count_temp, box_y_count_temp;
	unsigned int box_x_len_temp, box_y_len_temp;
	unsigned char set_index,refresh;
	Lcd_Clear(GRAY0);
	Gui_DrawFont_GBK16(100,16,BLUE,GRAY0,"Configuration");
	
	box_x_count_temp = sR1Mower.sRunParam.DrvSpeedL;
	box_y_count_temp = sR1Mower.sRunParam.BsStrenghL;
	box_x_len_temp = sR1Mower.sRunParam.KP1 * 200;
	box_y_len_temp = sR1Mower.sRunParam.KD1 * 200;
	
	set_index = 0;
	refresh = 1;
	//设置BOX_X_COUNT
	
	while(1)
	{
		IWDG_ReloadCounter(); //喂狗
		if(Key_Release(KEY_OK))  //按下ok键
		{
			switch(set_index)
			{
				case 0:
					sR1Mower.sRunParam.DrvSpeedL = box_x_count_temp;
					break;
				case 1:
					sR1Mower.sRunParam.BsStrenghL = box_y_count_temp;
					break;
				case 2:
					sR1Mower.sRunParam.KP1 = box_x_len_temp / 200.0;
					break;
				case 3:
					sR1Mower.sRunParam.KD1 = box_y_len_temp / 200.0;
					break;
				default:
					break;
			}
			EE_SaveParams();
			return;
		}
		if(Key_Release(KEY_SET))
		{
			return;
		}
		
		if(Key_Press(KEY_RIGHT))
		{
			set_index++;
			if(set_index>3)set_index=0;
			refresh = 1;
		}
		if(Key_Press(KEY_LEFT))
		{
			if(set_index == 0)set_index=3;
			else set_index--;
			refresh = 1;
		}
		if(Key_Press(KEY_DOWN))
		{
			switch(set_index)
			{
				case 0:
					box_x_count_temp--;
					if(box_x_count_temp < 1) box_x_count_temp = 99;
					break;
				case 1:
					box_y_count_temp--;
					if(box_y_count_temp < 1) box_y_count_temp = 99;
					break;
				case 2:
					box_x_len_temp-=2;
					if(box_x_len_temp < 10)box_x_len_temp = 500;
					break;
				case 3:
					box_y_len_temp-=2;
					if(box_y_len_temp < 10 ) box_y_len_temp = 500;
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
					box_x_count_temp++;
					if(box_x_count_temp>99) box_x_count_temp = 0;
					break;
				case 1:
					box_y_count_temp++ ;
					if(box_y_count_temp > 99) box_y_count_temp = 0;
					break;
				case 2:
					box_x_len_temp+=2;
					if(box_x_len_temp > 500)box_x_len_temp = 10;
					break;
				case 3:
					box_y_len_temp+=2;
					if(box_y_len_temp > 500) box_y_len_temp = 10;
					break;
				
				default:
					
					break;
			}
			refresh = 1;
		}
		if(refresh)
		{
			switch(set_index)
			{
				case 0:
					Gui_DrawFont_GBK16(100,46,BLUE,GRAY0,"BOX X COUNT  ");
					Gui_DrawFont_Num32_Int(98,78,RED,GRAY0,box_x_count_temp,3);	
					break;
				case 1:
					Gui_DrawFont_GBK16(100,46,BLUE,GRAY0,"BOX Y COUNT  ");
					Gui_DrawFont_Num32_Int(98,78,RED,GRAY0,box_y_count_temp,3);	
					break;
				case 2:
					Gui_DrawFont_GBK16(100,46,BLUE,GRAY0,"BOX X LENGTH ");
					Gui_DrawFont_Num32_Int(98,78,RED,GRAY0,box_x_len_temp,3);	
					break;
				case 3:
					Gui_DrawFont_GBK16(100,46,BLUE,GRAY0,"BOX Y LENGTH ");
					Gui_DrawFont_Num32_Int(98,78,RED,GRAY0,box_y_len_temp,3);	
					break;
				
				default:
					
					break;
			}
			Gui_DrawFont_GBK16(16,200,BLUE,GRAY0,"OK");
			Gui_DrawFont_GBK16(216,200,BLUE,GRAY0,"CANCEL");

			refresh = 0;
		}
	}
	
}

void Gui_DispClear(void)
{
	Lcd_Clear(GRAY0);
}

