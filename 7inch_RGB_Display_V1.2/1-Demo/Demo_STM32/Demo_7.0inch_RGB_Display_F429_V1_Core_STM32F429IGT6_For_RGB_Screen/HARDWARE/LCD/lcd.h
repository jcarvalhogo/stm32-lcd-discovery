//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//����Ӳ������Ƭ��STM32H743IIT6,����ԭ��Apollo STM32F4/F7������,��Ƶ400MHZ������12MHZ
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
#ifndef __LCD_H
#define __LCD_H
#include "sys.h"

//TFTLCD������Ҫ���õĺ���		   
extern u32  POINT_COLOR;//Ĭ�Ϻ�ɫ    
extern u32  BACK_COLOR; //������ɫ.Ĭ��Ϊ��ɫ

//����LCD��ʾ����
#define USE_HORIZONTAL  	  1 //����Һ����˳ʱ����ת���� 	0-0����ת��1-90����ת��2-180����ת��3-270����ת

#define LCD_LED  PDout(7)
#define LCD_DISP  PDout(4)

typedef struct
{
	u16 width;			//LCD ���
	u16 height;			//LCD �߶�
	u16 id;				  //LCD ID
	u8  dir;			  //LCD ��ʾ����0��0����ת��1��˳ʱ��90����ת��2��˳ʱ��180����ת��3��˳ʱ��270����ת
	u8  type;       //LCD �ӿ����ͣ�0��MCU LCD��1��RGB LCD
}_lcd_dev;

extern _lcd_dev lcd_dev;

//RGB LCD ��Ҫ������
typedef struct  
{							 
	u32 pwidth;			//LCD���Ŀ��,�̶�����,������ʾ����ı�,���Ϊ0,˵��û���κ�RGB������
	u32 pheight;		//LCD���ĸ߶�,�̶�����,������ʾ����ı�
	u16 hsw;			//ˮƽͬ�����
	u16 vsw;			//��ֱͬ�����
	u16 hbp;			//ˮƽ����
	u16 vbp;			//��ֱ����
	u16 hfp;			//ˮƽǰ��
	u16 vfp;			//��ֱǰ�� 
	u8 activelayer;		//��ǰ����:0/1	
	u8 dir;				//0,����;1,����;
	u16 width;			//LCD���
	u16 height;			//LCD�߶�
	u32 pixsize;		//ÿ��������ռ�ֽ���
}rgblcd_dev; 

extern rgblcd_dev rgblcd;		            //����RGB LCD����
extern LTDC_HandleTypeDef RGBLCD_Handler;	    //RGB LCD���
extern DMA2D_HandleTypeDef DMA2D_Handler;   //DMA2D���

#define LCD_PIXFORMAT_ARGB8888       0X00    
#define LCD_PIXFORMAT_RGB888         0X01    
#define LCD_PIXFORMAT_RGB565         0X02       
  
//������ɫ���ظ�ʽ,һ��ʹ��RGB565
#define LCD_PIXFORMAT		LCD_PIXFORMAT_RGB565

//������ɫ
#if (LCD_PIXFORMAT==LCD_PIXFORMAT_ARGB8888)
#define WHITE       0xFFFFFFFF
#define BLACK      	0xFF000000	  
#define BLUE       	0xFF0000FF  
#define BRED        0XFFFF00FF
#define GRED 			 	0XFFFFFF00
#define GBLUE			 	0XFF00FFFF
#define RED         0xFFFF0000
#define MAGENTA     0xFFFF00FF
#define GREEN       0xFF00FF00
#define CYAN        0xFF7FFFFF
#define YELLOW      0xFFFFFF00
#define BROWN 			0XFFBF8A00 //��ɫ
#define BRRED 			0XFFFF803F //�غ�ɫ
#define GRAY  			0XFF808580 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 0XFF003A7F	//����ɫ
#define LIGHTBLUE      	 0XFF7FAFE4	//ǳ��ɫ  
#define GRAYBLUE       	 0XFF528AC0 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	0XFF8080FF //ǳ��ɫ
#define LIGHTGRAY     0XFFEDEADB //ǳ��ɫ(PANNEL)
#define LGRAY 			 		0XFFC0C0C0 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE      	0XFFA4CA89 //ǳ����ɫ(�м����ɫ)
#define LBBLUE          0XFF2D6092 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)

#elif (LCD_PIXFORMAT==LCD_PIXFORMAT_RGB888)
#define WHITE       0xFFFFFF
#define BLACK      	0x000000	  
#define BLUE       	0x0000FF  
#define BRED        0XFF00FF
#define GRED 			 	0XFFFF00
#define GBLUE			 	0X00FFFF
#define RED         0xFF0000
#define MAGENTA     0xFF00FF
#define GREEN       0x00FF00
#define CYAN        0x7FFFFF
#define YELLOW      0xFFFF00
#define BROWN 			0XBF8A00 //��ɫ
#define BRRED 			0XFF803F //�غ�ɫ
#define GRAY  			0X808580 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 0X003A7F	//����ɫ
#define LIGHTBLUE      	 0X7FAFE4	//ǳ��ɫ  
#define GRAYBLUE       	 0X528AC0 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	0X8080FF //ǳ��ɫ
#define LIGHTGRAY     0XEDEADB //ǳ��ɫ(PANNEL)
#define LGRAY 			 		0XC0C0C0 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE      	0XA4CA89 //ǳ����ɫ(�м����ɫ)
#define LBBLUE          0X2D6092 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)
#else
#define WHITE       0xFFFF
#define BLACK      	0x0000	  
#define BLUE       	0x001F  
#define BRED        0XF81F
#define GRED 			 	0XFFE0
#define GBLUE			 	0X07FF
#define RED         0xF800
#define MAGENTA     0xF81F
#define GREEN       0x07E0
#define CYAN        0x7FFF
#define YELLOW      0xFFE0
#define BROWN 			0XBC40 //��ɫ
#define BRRED 			0XFC07 //�غ�ɫ
#define GRAY  			0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	0X841F //ǳ��ɫ
#define LIGHTGRAY     0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 			 		0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE      	0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE          0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)     
#endif
///////////////////////////////////////////////////////////////////////
//�û��޸����ò���:

//������ɫ���ظ�ʽ,һ����RGB565
//#define RGBLCD_PIXFORMAT				RGBLCD_PIXFORMAT_RGB888//RGBLCD_PIXFORMAT_RGB565	
//����Ĭ�ϱ�������ɫ
#define RGBLCD_BACKLAYERCOLOR			0X00000000	
//LCD֡�������׵�ַ,���ﶨ����SDRAM����.
#define RGBLCD_FRAME_BUF_ADDR			0XD0000000  

//rgb lcd ��������
void LCD_Switch(u8 sw);
void LCD_Layer_Switch(u8 layerx,u8 sw);
void LCD_Select_Layer(u8 layerx);
void LCD_Set_Direction(u8 dir);
void LCD_Draw_Point(u16 x,u16 y,u32 color);
u32 LCD_Read_Point(u16 x,u16 y);
void LCD_Fill_color(u16 sx,u16 sy,u16 ex,u16 ey,u32 color);
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u32 *color);
void LCD_Clear(u32 color);
u8 LCD_Clk_Set(u32 pll3m,u32 pll3n,u32 pll3r);
void LCD_Layer_Window_Config(u8 layerx,u16 sx,u16 sy,u16 width,u16 height);
void LCD_Layer_Parameter_Config(u8 layerx,u32 bufaddr,u8 pixformat,u8 alpha,u8 alpha0,u8 bfac1,u8 bfac2,u32 bkcolor);
u16 LCD_Read_ID(void);
void LCD_Init(void);
#endif
