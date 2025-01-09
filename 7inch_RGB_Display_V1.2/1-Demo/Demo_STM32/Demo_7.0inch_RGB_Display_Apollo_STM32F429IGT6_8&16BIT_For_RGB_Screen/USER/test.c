//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//����Ӳ������Ƭ��STM32F429IGT6,����ԭ��Apollo STM32F4/F7������,��Ƶ180MHZ������12MHZ
//QDtech-TFTҺ������ for STM32 FSMC
//xiao��@ShenZhen QDtech co.,LTD
//��˾��վ:www.qdtft.com
//�Ա���վ��http://qdtech.taobao.com
//wiki������վ��http://www.lcdwiki.com
//��˾�ṩ����֧�֣��κμ������⻶ӭ��ʱ����ѧϰ
//�̻�(����) :+86 0755-23594567 
//�ֻ�:15989313508���빤�� 
//����:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com 
//����֧��QQ:3002773612  3002778157
//��������QQȺ:324828016
//��������:2018/08/09
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������ȫ�����Ӽ������޹�˾ 2018-2028
//All rights reserved
/****************************************************************************************************
//��ģ�����ֱ�Ӳ�������ԭ��Apollo STM32F4/F7������TFTLCD��ۣ������ֶ�����
//STM32����������ָTFTLCD��������ڲ����ӵ�STM32����
//=========================================��Դ����================================================//
//     LCDģ��             TFTLCD�������        STM32��������
//      VDD       --->         5V/3.3              DC5V/3.3V          //��Դ
//      GND       --->          GND                  GND              //��Դ��
//=======================================Һ���������߽���==========================================//
//��ģ��Ĭ��������������Ϊ16λ��������
//     LCDģ��             TFTLCD�������        STM32��������
//      D0        --->          D0                   PD14        -|   
//      D1        --->          D1                   PD15         |  
//      D2        --->          D2                   PD0          | 
//      D3        --->          D3                   PD1          | 
//      D4        --->          D4                   PE7          |
//      D5        --->          D5                   PE8          |
//      D6        --->          D6                   PE9          |
//      D7        --->          D7                   PE10         |
//���ʹ��8λģʽ���������������û��ʹ��                         |===>Һ����16λ���������ź�
//      D8        --->          D8                   PE11         |
//      D9        --->          D9                   PE12         |
//      D10       --->          D10                  PE13         |
//      D11       --->          D11                  PE14         |
//      D12       --->          D12                  PE15         |
//      D13       --->          D13                  PD8          |
//      D14       --->          D14                  PD9          |
//      D15       --->          D15                  PD10        -|
//=======================================Һ���������߽���==========================================//
//     LCDģ�� 				     TFTLCD�������        STM32�������� 
//       WR       --->          WR                   PD5             //Һ����д���ݿ����ź�
//       RD       --->          RD                   PD4             //Һ���������ݿ����ź�
//       RS       --->          RS                   PD13            //Һ��������/��������ź�
//       RST      --->          RST                ��λ����          //Һ������λ�����ź�
//       CS       --->          CS                   PD7             //Һ����Ƭѡ�����ź�
//       BL       --->          BL                   PB5             //Һ������������ź�
//=========================================������������=========================================//
//���ģ�鲻���������ܻ��ߴ��д������ܣ����ǲ���Ҫ�������ܣ�����Ҫ���д���������
//	   LCDģ��             TFTLCD�������        STM32�������� 
//      INI       --->          PEN                  PH7             //�����������ж��ź�
//      SDA       --->          MOSI                 PI3             //������IIC����д�ź�
//      CRST      --->          TCS                  PI8             //��������λ�����ź�
//      CLK       --->          CLK                  PH6             //������IIC����ʱ���ź�
**************************************************************************************************/	
 /* @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
**************************************************************************************************/		
#include <stdio.h>
#include <string.h>
#include "lcd.h"
#include "delay.h"
#include "gui.h"
#include "test.h"
#include "touch.h"
#include "key.h" 
#include "led.h"
#include "pic.h"
#include "ft5426.h"
#include "rtp.h"
#include "exti.h"

#define OTT_MAX_TOUCH  5    		//������֧�ֵĵ���,�̶�Ϊ5��

//========================variable==========================//
u16 ColorTab[5]={RED,GREEN,BLUE,YELLOW,BRED};//������ɫ����
const u16 POINT_COLOR_TBL[OTT_MAX_TOUCH]={RED,GREEN,BLUE,BROWN,GRED}; //���ݴ���5����ɫ����
u16 ColornTab[8]={RED,MAGENTA,GREEN,DARKBLUE,BLUE,BLACK,LIGHTGREEN};
//=====================end of variable======================//

/*****************************************************************************
 * @name       :void DrawTestPage(u8 *str)
 * @date       :2018-08-09 
 * @function   :Drawing test interface
 * @parameters :str:the start address of the Chinese and English strings
 * @retvalue   :None
******************************************************************************/ 
void DrawTestPage(u8 *str)
{
//���ƹ̶���up
LCD_Clear(WHITE);
LCD_Fill(0,0,lcd_dev.width-1,20,BLUE);
//���ƹ̶���down
LCD_Fill(0,lcd_dev.height-20,lcd_dev.width-1,lcd_dev.height-1,BLUE);
POINT_COLOR=WHITE;
Gui_StrCenter(0,2,WHITE,BLUE,str,16,1);//������ʾ
Gui_StrCenter(0,lcd_dev.height-18,WHITE,BLUE,"http://www.lcdwiki.com",16,1);//������ʾ
//���Ʋ�������
//LCD_Fill(0,20,lcddev.width,lcddev.height-20,WHITE);
}

/*****************************************************************************
 * @name       :void main_test(void)
 * @date       :2018-08-09 
 * @function   :Drawing the main Interface of the Comprehensive Test Program
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void main_test(void)
{
	DrawTestPage("ȫ�������ۺϲ��Գ���");	
	Gui_StrCenter(0,30,RED,BLUE,"ȫ������",16,1);//������ʾ
	Gui_StrCenter(0,60,RED,BLUE,"�ۺϲ��Գ���",16,1);//������ʾ	
	Gui_StrCenter(0,90,MAGENTA,BLUE,"7.0\" SSD1963 480X800",16,1);//������ʾ
	Gui_StrCenter(0,120,BLUE,BLUE,"xiaoFeng@QDtech 2020-05-13",16,1);//������ʾ
	delay_ms(1500);		
	delay_ms(1500);
}

/*****************************************************************************
 * @name       :void Test_Color(void)
 * @date       :2018-08-09 
 * @function   :Color fill test(white,black,red,green,blue)
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Test_Color(void)
{
	//DrawTestPage("����1:��ɫ������");
	LCD_Fill(0,0,lcd_dev.width-1,lcd_dev.height-1,WHITE);
	Show_Str(20,30,BLUE,YELLOW,"BL Test",16,1);delay_ms(800);
	LCD_Fill(0,0,lcd_dev.width-1,lcd_dev.height-1,RED);
	Show_Str(20,30,BLUE,YELLOW,"RED ",16,1);delay_ms(800);
	LCD_Fill(0,0,lcd_dev.width-1,lcd_dev.height-1,GREEN);
	Show_Str(20,30,BLUE,YELLOW,"GREEN ",16,1);delay_ms(800);
	LCD_Fill(0,0,lcd_dev.width-1,lcd_dev.height-1,BLUE);
	Show_Str(20,30,RED,YELLOW,"BLUE ",16,1);delay_ms(800);
}

/*****************************************************************************
 * @name       :void Test_FillRec(void)
 * @date       :2018-08-09 
 * @function   :Rectangular display and fill test
								Display red,green,blue,yellow,pink rectangular boxes in turn,
								1500 milliseconds later,
								Fill the rectangle in red,green,blue,yellow and pink in turn
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Test_FillRec(void)
{
	u8 i=0;
	DrawTestPage("����3:GUI����������");
	LCD_Fill(0,20,lcd_dev.width-1,lcd_dev.height-20,WHITE);
	for (i=0; i<5; i++) 
	{
		POINT_COLOR=ColorTab[i];
		LCD_DrawRectangle(lcd_dev.width/2-110+(i*30),lcd_dev.height/2-110+(i*30),lcd_dev.width/2-110+(i*30)+100,lcd_dev.height/2-110+(i*30)+100); 
	}
	delay_ms(1500);	
	LCD_Fill(0,20,lcd_dev.width-1,lcd_dev.height-20,WHITE); 
	for (i=0; i<5; i++) 
	{
		POINT_COLOR=ColorTab[i];
		LCD_DrawFillRectangle(lcd_dev.width/2-110+(i*30),lcd_dev.height/2-110+(i*30),lcd_dev.width/2-110+(i*30)+100,lcd_dev.height/2-110+(i*30)+100); 
	}
	delay_ms(1500);
}

/*****************************************************************************
 * @name       :void Test_Circle(void)
 * @date       :2018-08-09 
 * @function   :circular display and fill test
								Display red,green,blue,yellow,pink circular boxes in turn,
								1500 milliseconds later,
								Fill the circular in red,green,blue,yellow and pink in turn
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Test_Circle(void)
{
	u8 i=0;
	DrawTestPage("����4:GUI��Բ������");
	LCD_Fill(0,20,lcd_dev.width-1,lcd_dev.height-20,WHITE);
	for (i=0; i<5; i++)  
		gui_circle(lcd_dev.width/2-70+(i*35),lcd_dev.height/2-70+(i*35),ColorTab[i],50,0);
	delay_ms(1500);	
	LCD_Fill(0,20,lcd_dev.width-1,lcd_dev.height-20,WHITE); 
	for (i=0; i<5; i++) 
	  	gui_circle(lcd_dev.width/2-70+(i*35),lcd_dev.height/2-70+(i*35),ColorTab[i],50,1);
	delay_ms(1500);
}

/*****************************************************************************
 * @name       :void English_Font_test(void)
 * @date       :2018-08-09 
 * @function   :English display test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void English_Font_test(void)
{
	DrawTestPage("����6:Ӣ����ʾ����");
	Show_Str(10,30,BLUE,YELLOW,"6X12:abcdefghijklmnopqrstuvwxyz0123456789",12,0);
	Show_Str(10,45,BLUE,YELLOW,"6X12:ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789",12,1);
	Show_Str(10,60,BLUE,YELLOW,"6X12:~!@#$%^&*()_+{}:<>?/|-+.",12,0);
	Show_Str(10,80,BLUE,YELLOW,"8X16:abcdefghijklmnopqrstuvwxyz0123456789",16,0);
	Show_Str(10,100,BLUE,YELLOW,"8X16:ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789",16,1);
	Show_Str(10,120,BLUE,YELLOW,"8X16:~!@#$%^&*()_+{}:<>?/|-+.",16,0); 
	delay_ms(1200);
}

/*****************************************************************************
 * @name       :void Test_Triangle(void)
 * @date       :2018-08-09 
 * @function   :triangle display and fill test
								Display red,green,blue,yellow,pink triangle boxes in turn,
								1500 milliseconds later,
								Fill the triangle in red,green,blue,yellow and pink in turn
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Test_Triangle(void)
{
	u8 i=0;
	DrawTestPage("����5:GUI Triangle������");
	LCD_Fill(0,20,lcd_dev.width-1,lcd_dev.height-20,WHITE);
	for(i=0;i<5;i++)
	{
		POINT_COLOR=ColorTab[i];
		Draw_Triangel(lcd_dev.width/2-110+(i*30),lcd_dev.height/2-17+(i*30),lcd_dev.width/2-60-1+(i*30),lcd_dev.height/2-103+(i*30),lcd_dev.width/2-10-1+(i*30),lcd_dev.height/2-17+(i*30));
	}
	delay_ms(1500);	
	LCD_Fill(0,20,lcd_dev.width-1,lcd_dev.height-20,WHITE); 
	for(i=0;i<5;i++)
	{
		POINT_COLOR=ColorTab[i];
		Fill_Triangel(lcd_dev.width/2-110+(i*30),lcd_dev.height/2-17+(i*30),lcd_dev.width/2-60-1+(i*30),lcd_dev.height/2-103+(i*30),lcd_dev.width/2-10-1+(i*30),lcd_dev.height/2-17+(i*30));
	}
	delay_ms(1500);
}

/*****************************************************************************
 * @name       :void Chinese_Font_test(void)
 * @date       :2018-08-09 
 * @function   :chinese display test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Chinese_Font_test(void)
{	
	DrawTestPage("����7:������ʾ����");
	Show_Str(10,30,BLUE,YELLOW,"16X16:ȫ�����Ӽ������޹�˾��ӭ��",16,0);
	Show_Str(10,50,BLUE,YELLOW,"16X16:Welcomeȫ������",16,0);
	Show_Str(10,70,BLUE,YELLOW,"24X24:���������Ĳ���",24,1);
	Show_Str(10,100,BLUE,YELLOW,"32X32:�������",32,1);
	delay_ms(1200);
}

/*****************************************************************************
 * @name       :void Pic_test(void)
 * @date       :2018-08-09 
 * @function   :picture display test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Pic_test(void)
{
	DrawTestPage("����8:ͼƬ��ʾ����");
	//LCD_Fill(0,20,lcddev.width,lcddev.height-20,WHITE);
	Gui_Drawbmp16(30,30,40,40,gImage_qq);
	Show_Str(30+12,75,BLUE,YELLOW,"QQ",16,1);
	Gui_Drawbmp16(90,30,40,40,gImage_qq);
	Show_Str(90+12,75,BLUE,YELLOW,"QQ",16,1);
	Gui_Drawbmp16(150,30,40,40,gImage_qq);
	Show_Str(150+12,75,BLUE,YELLOW,"QQ",16,1);
	delay_ms(1200);
}

/*****************************************************************************
 * @name       :void Rotate_Test(void)
 * @date       :2018-08-09 
 * @function   :rotate test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Rotate_Test(void)
{
	u8 i=0;
	u8 *Direction[4]={"Rotation:0","Rotation:90","Rotation:180","Rotation:270"};
	for(i=0;i<4;i++)
	{
		LCD_Set_Direction(i);
		DrawTestPage("����9:��Ļ��ת����");
		Show_Str(20,30,BLUE,YELLOW,Direction[i],16,1);
		Gui_Drawbmp16(30,50,40,40,gImage_qq);
		delay_ms(1000);delay_ms(1000);
	}
	LCD_Set_Direction(USE_HORIZONTAL);
}

/*****************************************************************************
 * @name       :void Rtp_Test(void)
 * @date       :2020-05-13 
 * @function   :Resistive touch screen test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Rtp_Test(void)
{
	u8 key;
	u16 i=0;
	u16 j=0;
	u16 colorTemp=RED;
	LED_Init();
	KEY_Init();
	DrawTestPage("����10:Touch(��KEY0У׼)      ");
	LCD_ShowString(lcd_dev.width-24,0,16,"RST",1);//��ʾ��������
	POINT_COLOR=RED;
	LCD_Fill(lcd_dev.width-50,2,lcd_dev.width-50+22,18,RED); 	
	while(1)
	{
	 	key=KEY_Scan(1);
		RTP_Scan(0); 		 
		if(tp_dev.sta&TP_PRES_DOWN)			//������������
		{	
		 	if(tp_dev.x[0]<lcd_dev.width&&tp_dev.y[0]<lcd_dev.height)
			{	
				if(tp_dev.x[0]>(lcd_dev.width-24)&&tp_dev.y[0]<16)
				{
					DrawTestPage("����10:Touch(��KEY0У׼)      ");//���
					LCD_ShowString(lcd_dev.width-24,0,16,"RST",1);//��ʾ��������
					POINT_COLOR=colorTemp;
					LCD_Fill(lcd_dev.width-50,2,lcd_dev.width-50+22,18,POINT_COLOR); 
				}
				else if((tp_dev.x[0]>(lcd_dev.width-50)&&tp_dev.x[0]<(lcd_dev.width-50+22))&&tp_dev.y[0]<20)
				{
				LCD_Fill(lcd_dev.width-50,2,lcd_dev.width-50+22,18,ColorTab[j%5]); 
				POINT_COLOR=ColorTab[(j++)%5];
				colorTemp=POINT_COLOR;
				delay_ms(10);
				}

				else RTP_Draw_Big_Point(tp_dev.x[0],tp_dev.y[0],POINT_COLOR);		//��ͼ	  			   
			}
		}else delay_ms(10);	//û�а������µ�ʱ�� 	    
		if(key==1)	//KEY_RIGHT����,��ִ��У׼����
		{

			LCD_Clear(WHITE);//����
		  RTP_Adjust();  //��ĻУ׼ 
			RTP_Save_Adjdata();	 
			DrawTestPage("����10:Touch(��KEY0У׼)      ");
			LCD_ShowString(lcd_dev.width-24,0,16,"RST",1);//��ʾ��������
			POINT_COLOR=colorTemp;
			LCD_Fill(lcd_dev.width-50,2,lcd_dev.width-50+22,18,POINT_COLOR); 
		}
		i++;
		if(i==30)
		{
			i=0;
			LED0=!LED0;
			//break;
		}
	}   
}

/*****************************************************************************
 * @name       :void Ctp_Test(void)
 * @date       :2020-05-13 
 * @function   :Capacitive touch screen test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Ctp_Test(void)
{
	u8 t=0;
	u8 i=0,j=0;	 
 	u16 lastpos[5][2];		//���һ�ε����� 
	LED_Init();
	DrawTestPage("����10:����Touch����(֧��5�㴥��)  ");
	LCD_ShowString(lcd_dev.width-24,0,16,"RST",1);//��ʾ��������
	POINT_COLOR=RED;//���û�����ɫ //���
	while(1)
	{
		j++;
		FT5426_Scan();
		for(t=0;t<OTT_MAX_TOUCH;t++)//���5�㴥��
		{
			if((tp_dev.sta)&(1<<t))//�ж��Ƿ��е㴥����
			{
				if(tp_dev.x[t]<lcd_dev.width&&tp_dev.y[t]<lcd_dev.height)//��LCD��Χ��
				{
					if(lastpos[t][0]==0XFFFF)
					{
						lastpos[t][0] = tp_dev.x[t];
						lastpos[t][1] = tp_dev.y[t];
					}
					if(tp_dev.x[t]>(lcd_dev.width-24)&&tp_dev.y[t]<16)
					{
							if(j>1) //��ֹ���һ�Σ��������
							{
								continue;
							}
							DrawTestPage("����10:����Touch����(֧��5�㴥��)  ");
							LCD_ShowString(lcd_dev.width-24,0,16,"RST",1);//��ʾ��������
							POINT_COLOR=RED;//���û�����ɫ //���
					}
					else
					{
							LCD_DrawLine2(lastpos[t][0],lastpos[t][1],tp_dev.x[t],tp_dev.y[t],2,POINT_COLOR_TBL[t]);//����
					}
					lastpos[t][0]=tp_dev.x[t];
					lastpos[t][1]=tp_dev.y[t];
				}
			}else lastpos[t][0]=0XFFFF;
		}		
		delay_ms(5);i++;
		if(i%30==0)LED0=!LED0;
		if(j>4)
		{
			j=0;
		}
	}  
}

/*****************************************************************************
 * @name       :void Touch_Test(void)
 * @date       :2018-08-09 
 * @function   :touch test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Touch_Test(void)
{
	TP_Init();
	if(tp_dev.touchtype&0x80)
	{
		Ctp_Test();
	}
	else
	{
		Rtp_Test();			
	}
}

/*****************************************************************************
 * @name       :void Test_Read(void)
 * @date       :2018-11-13 
 * @function   :read test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Test_Read(void)
{
	u16 color;
	u8 buf[10] = {0},i;
	u8 cbuf[35] = {0};
	DrawTestPage("����2:��ID����ɫֵ����");
	LCD_Fill(0,20,lcd_dev.width-1,lcd_dev.height-20,WHITE);
	if(lcd_dev.id ==0x5761)
	{
		lcd_dev.id = 0x1963;
	}
	sprintf((char *)buf,"ID:0x%x",lcd_dev.id);
	Show_Str(50,25,BLUE,YELLOW,buf,16,1);
	for (i=0; i<7; i++) 
	{
		POINT_COLOR=ColornTab[i];
		LCD_DrawFillRectangle(40-10,55+i*25-10,40+10,55+i*25+10);
		color = LCD_Read_Point(40,55+i*25);
		if(POINT_COLOR==color)
		{
			strcpy((char*)buf, "OK");
		}
		else
		{
			strcpy((char*)buf, "ERROR");
		}
		sprintf((char *)cbuf,"read color:0x%04X      %s",color, buf);
		Show_Str(40+20,55+i*25-8,POINT_COLOR,YELLOW,cbuf,16,1);
	}
	delay_ms(1500);
}



