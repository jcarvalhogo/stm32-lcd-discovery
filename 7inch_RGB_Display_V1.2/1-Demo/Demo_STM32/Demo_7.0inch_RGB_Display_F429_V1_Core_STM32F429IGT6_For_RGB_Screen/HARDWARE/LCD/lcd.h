//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//测试硬件：单片机STM32H743IIT6,正点原子Apollo STM32F4/F7开发板,主频400MHZ，晶振12MHZ
//QDtech-TFT液晶驱动 for STM32 FSMC
//xiao冯@ShenZhen QDtech co.,LTD
//公司网站:www.qdtft.com
//淘宝网站：http://qdtech.taobao.com
//wiki技术网站：http://www.lcdwiki.com
//我司提供技术支持，任何技术问题欢迎随时交流学习
//固话(传真) :+86 0755-23594567 
//手机:15989313508（冯工） 
//邮箱:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com 
//技术支持QQ:3002773612  3002778157
//技术交流QQ群:324828016
//创建日期:2018/08/09
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 深圳市全动电子技术有限公司 2018-2028
//All rights reserved
/****************************************************************************************************
//此模块可以直接插入正点原子Apollo STM32F4/F7开发板TFTLCD插槽，无需手动接线
//STM32连接引脚是指TFTLCD插槽引脚内部连接的STM32引脚
//=========================================电源接线================================================//
//     LCD模块             TFTLCD插槽引脚        STM32连接引脚
//      VDD       --->         5V/3.3              DC5V/3.3V          //电源
//      GND       --->          GND                  GND              //电源地
//=======================================液晶屏数据线接线==========================================//
//本模块默认数据总线类型为16位并口总线
//     LCD模块             TFTLCD插槽引脚        STM32连接引脚
//      D0        --->          D0                   PD14        -|   
//      D1        --->          D1                   PD15         |  
//      D2        --->          D2                   PD0          | 
//      D3        --->          D3                   PD1          | 
//      D4        --->          D4                   PE7          |
//      D5        --->          D5                   PE8          |
//      D6        --->          D6                   PE9          |
//      D7        --->          D7                   PE10         |
//如果使用8位模式，则下面的数据线没有使用                         |===>液晶屏16位并口数据信号
//      D8        --->          D8                   PE11         |
//      D9        --->          D9                   PE12         |
//      D10       --->          D10                  PE13         |
//      D11       --->          D11                  PE14         |
//      D12       --->          D12                  PE15         |
//      D13       --->          D13                  PD8          |
//      D14       --->          D14                  PD9          |
//      D15       --->          D15                  PD10        -|
//=======================================液晶屏控制线接线==========================================//
//     LCD模块 				     TFTLCD插槽引脚        STM32连接引脚 
//       WR       --->          WR                   PD5             //液晶屏写数据控制信号
//       RD       --->          RD                   PD4             //液晶屏读数据控制信号
//       RS       --->          RS                   PD13            //液晶屏数据/命令控制信号
//       RST      --->          RST                复位引脚          //液晶屏复位控制信号
//       CS       --->          CS                   PD7             //液晶屏片选控制信号
//       BL       --->          BL                   PB5             //液晶屏背光控制信号
//=========================================触摸屏触接线=========================================//
//如果模块不带触摸功能或者带有触摸功能，但是不需要触摸功能，则不需要进行触摸屏接线
//	   LCD模块             TFTLCD插槽引脚        STM32连接引脚 
//      INI       --->          PEN                  PH7             //触摸屏触摸中断信号
//      SDA       --->          MOSI                 PI3             //触摸屏IIC总线写信号
//      CRST      --->          TCS                  PI8             //触摸屏复位控制信号
//      CLK       --->          CLK                  PH6             //触摸屏IIC总线时钟信号
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

//TFTLCD部分外要调用的函数		   
extern u32  POINT_COLOR;//默认红色    
extern u32  BACK_COLOR; //背景颜色.默认为白色

//设置LCD显示方向
#define USE_HORIZONTAL  	  1 //定义液晶屏顺时针旋转方向 	0-0度旋转，1-90度旋转，2-180度旋转，3-270度旋转

#define LCD_LED  PDout(7)
#define LCD_DISP  PDout(4)

typedef struct
{
	u16 width;			//LCD 宽度
	u16 height;			//LCD 高度
	u16 id;				  //LCD ID
	u8  dir;			  //LCD 显示方向：0：0度旋转，1：顺时针90度旋转，2：顺时针180度旋转，3：顺时针270度旋转
	u8  type;       //LCD 接口类型：0：MCU LCD，1：RGB LCD
}_lcd_dev;

extern _lcd_dev lcd_dev;

//RGB LCD 重要参数集
typedef struct  
{							 
	u32 pwidth;			//LCD面板的宽度,固定参数,不随显示方向改变,如果为0,说明没有任何RGB屏接入
	u32 pheight;		//LCD面板的高度,固定参数,不随显示方向改变
	u16 hsw;			//水平同步宽度
	u16 vsw;			//垂直同步宽度
	u16 hbp;			//水平后廊
	u16 vbp;			//垂直后廊
	u16 hfp;			//水平前廊
	u16 vfp;			//垂直前廊 
	u8 activelayer;		//当前层编号:0/1	
	u8 dir;				//0,竖屏;1,横屏;
	u16 width;			//LCD宽度
	u16 height;			//LCD高度
	u32 pixsize;		//每个像素所占字节数
}rgblcd_dev; 

extern rgblcd_dev rgblcd;		            //管理RGB LCD参数
extern LTDC_HandleTypeDef RGBLCD_Handler;	    //RGB LCD句柄
extern DMA2D_HandleTypeDef DMA2D_Handler;   //DMA2D句柄

#define LCD_PIXFORMAT_ARGB8888       0X00    
#define LCD_PIXFORMAT_RGB888         0X01    
#define LCD_PIXFORMAT_RGB565         0X02       
  
//定义颜色像素格式,一般使用RGB565
#define LCD_PIXFORMAT		LCD_PIXFORMAT_RGB565

//画笔颜色
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
#define BROWN 			0XFFBF8A00 //棕色
#define BRRED 			0XFFFF803F //棕红色
#define GRAY  			0XFF808580 //灰色
//GUI颜色

#define DARKBLUE      	 0XFF003A7F	//深蓝色
#define LIGHTBLUE      	 0XFF7FAFE4	//浅蓝色  
#define GRAYBLUE       	 0XFF528AC0 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	0XFF8080FF //浅绿色
#define LIGHTGRAY     0XFFEDEADB //浅灰色(PANNEL)
#define LGRAY 			 		0XFFC0C0C0 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE      	0XFFA4CA89 //浅灰蓝色(中间层颜色)
#define LBBLUE          0XFF2D6092 //浅棕蓝色(选择条目的反色)

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
#define BROWN 			0XBF8A00 //棕色
#define BRRED 			0XFF803F //棕红色
#define GRAY  			0X808580 //灰色
//GUI颜色

#define DARKBLUE      	 0X003A7F	//深蓝色
#define LIGHTBLUE      	 0X7FAFE4	//浅蓝色  
#define GRAYBLUE       	 0X528AC0 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	0X8080FF //浅绿色
#define LIGHTGRAY     0XEDEADB //浅灰色(PANNEL)
#define LGRAY 			 		0XC0C0C0 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE      	0XA4CA89 //浅灰蓝色(中间层颜色)
#define LBBLUE          0X2D6092 //浅棕蓝色(选择条目的反色)
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
#define BROWN 			0XBC40 //棕色
#define BRRED 			0XFC07 //棕红色
#define GRAY  			0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	0X841F //浅绿色
#define LIGHTGRAY     0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 		0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE      	0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE          0X2B12 //浅棕蓝色(选择条目的反色)     
#endif
///////////////////////////////////////////////////////////////////////
//用户修改配置部分:

//定义颜色像素格式,一般用RGB565
//#define RGBLCD_PIXFORMAT				RGBLCD_PIXFORMAT_RGB888//RGBLCD_PIXFORMAT_RGB565	
//定义默认背景层颜色
#define RGBLCD_BACKLAYERCOLOR			0X00000000	
//LCD帧缓冲区首地址,这里定义在SDRAM里面.
#define RGBLCD_FRAME_BUF_ADDR			0XD0000000  

//rgb lcd 操作函数
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
