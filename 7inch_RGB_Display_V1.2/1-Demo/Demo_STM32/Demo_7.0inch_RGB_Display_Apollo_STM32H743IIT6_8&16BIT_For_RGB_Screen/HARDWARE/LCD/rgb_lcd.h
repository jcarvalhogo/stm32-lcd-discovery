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
#ifndef __RGB_LCD_H
#define __RGB_LCD_H
#include "sys.h"

#define RGBLCD_LED(n)    (n?HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET))

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
extern u32 *rgblcd_framebuf[2];					//RGB LCD֡��������ָ��,����ָ���Ӧ��С���ڴ�����

#define RGBLCD_PIXEL_FORMAT_ARGB8888       0X00    
#define RGBLCD_PIXEL_FORMAT_RGB888         0X01    
#define RGBLCD_PIXEL_FORMAT_RGB565         0X02       
#define RGBLCD_PIXEL_FORMAT_ARGB1555       0X03      
#define RGBLCD_PIXEL_FORMAT_ARGB4444       0X04     
#define RGBLCD_PIXEL_FORMAT_L8             0X05     
#define RGBLCD_PIXEL_FORMAT_AL44           0X06     
#define RGBLCD_PIXEL_FORMAT_AL88           0X07      

///////////////////////////////////////////////////////////////////////
//�û��޸����ò���:

//������ɫ���ظ�ʽ,һ����RGB565
#define RGBLCD_PIXFORMAT				RGBLCD_PIXEL_FORMAT_RGB565	
//����Ĭ�ϱ�������ɫ
#define RGBLCD_BACKLAYERCOLOR			0X00000000	
//LCD֡�������׵�ַ,���ﶨ����SDRAM����.
#define RGBLCD_FRAME_BUF_ADDR			0XC0000000  

//rgb lcd ��������
void RGBLCD_Switch(u8 sw);
void RGBLCD_Layer_Switch(u8 layerx,u8 sw);
void RGBLCD_Select_Layer(u8 layerx);
void RGBLCD_Display_Dir(u8 dir);
void RGBLCD_Draw_Point(u16 x,u16 y,u32 color);
u32 RGBLCD_Read_Point(u16 x,u16 y);
void RGBLCD_Fill_color(u16 sx,u16 sy,u16 ex,u16 ey,u32 color);
void RGBLCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color);
void RGBLCD_Clear(u32 color);
u8 RGBLCD_Clk_Set(u32 pll3m,u32 pll3n,u32 pll3r);
void RGBLCD_Layer_Window_Config(u8 layerx,u16 sx,u16 sy,u16 width,u16 height);
void RGBLCD_Layer_Parameter_Config(u8 layerx,u32 bufaddr,u8 pixformat,u8 alpha,u8 alpha0,u8 bfac1,u8 bfac2,u32 bkcolor);
u16 RGBLCD_Read_ID(void);
void RGBLCD_Init(void);
#endif
