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
#ifndef __MCU_LCD_H
#define __MCU_LCD_H		
#include "sys.h"	 
#include "stdlib.h"

//LCD重要参数集
typedef struct  
{										    
	u16 width;			//LCD 宽度
	u16 height;			//LCD 高度
	u8  dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。	
	u16	 wramcmd;		//开始写gram指令
	u16  rramcmd;   //开始读gram指令
	u16  setxcmd;		//设置x坐标指令
	u16  setycmd;		//设置y坐标指令	 
}_mculcd_dev; 	

/////////////////////////////////////用户配置区///////////////////////////////////	 
#define MCULCD_USE8BIT_MODEL   1	//定义数据总线是否使用8位模式 0,使用16位模式.1,使用8位模式

//////////////////////////////////////////////////////////////////////////////////	  
//定义LCD的尺寸
#define LCD_W 480
#define LCD_H 800

//LCD MPU保护参数
#define MCULCD_REGION_NUMBER		MPU_REGION_NUMBER0		//LCD使用region0
#define MCULCD_ADDRESS_START		(0X60000000)			//LCD区的首地址
#define MCULCD_REGION_SIZE			MPU_REGION_SIZE_256MB   //LCD区大小

////////////////////////////////////////////////////////////////////
//-----------------LCD端口定义---------------- 
//背光控制
#define MCULCD_LED(n) (n?HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET)) 	

//复位控制
#define MCULCD_RST(n) (n?HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_RESET)) 	

//LCD地址结构体
typedef struct
{
#if MCULCD_USE8BIT_MODEL 
	vu8 MCULCD_REG;
	vu8 MCULCD_RAM;
#else
	vu16 MCULCD_REG;
	vu16 MCULCD_RAM;		
#endif
} MCULCD_TypeDef;
//使用NOR/SRAM的 Bank1.sector4,地址位HADDR[27,26]=11 A18作为数据命令区分线 
#if MCULCD_USE8BIT_MODEL
//使用8位模式时，STM32内部地址不需要右移一位 
#define MCULCD_BASE        ((u32)(0x60000000 | 0x0003FFFF))
#else
//使用16位模式时，注意设置时STM32内部会右移一位对齐! 			    
#define MCULCD_BASE        ((u32)(0x60000000 | 0x0007FFFE))
#endif
#define MCULCD             ((MCULCD_TypeDef *) MCULCD_BASE)
    															  
void MCULCD_Init(void);
u16 MCULCD_Read(void);
void MCULCD_Clear(u16 Color);	 
void MCULCD_Set_Cursor(u16 Xpos, u16 Ypos);
void MCULCD_Draw_Point(u16 x,u16 y, u16 color);//画点
u16  MCULCD_Read_Point(u16 x,u16 y); //读点	   
void MCULCD_Set_Windows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd);
u16 MCULCD_RD_DATA(void);//读取LCD数据								    
void MCULCD_Write_Reg(u16 LCD_Reg, u16 LCD_RegValue);
void MCULCD_WR_REG(u16 data);
void MCULCD_WR_DATA(u16 data);
void MCULCD_Read_Reg(u16 LCD_Reg,u8 *Rval,int n);
void MCULCD_Write_RAM_Prepare(void);
void MCULCD_Read_RAM_Prepare(void);   
void MCULCD_Write_Data_16Bit(u16 Data);
u16 MCULCD_Read_Data_16Bit(void);
void MCULCD_Set_Direction(u8 direction );
u16 Color_To_565(u8 r, u8 g, u8 b);
u16 MCULCD_Read_ID(void);
void MCULCD_Set_PWM_BackLight(u8 pwm);
void MCULCD_Fill_Color(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);

//LCD分辨率设置
#define SSD_HOR_RESOLUTION		800		//LCD水平分辨率
#define SSD_VER_RESOLUTION		480		//LCD垂直分辨率
//LCD驱动参数设置
#define SSD_HOR_PULSE_WIDTH		1		//水平脉宽
#define SSD_HOR_BACK_PORCH		46		//水平前廊
#define SSD_HOR_FRONT_PORCH		210		//水平后廊

#define SSD_VER_PULSE_WIDTH		1		//垂直脉宽
#define SSD_VER_BACK_PORCH		23		//垂直前廊
#define SSD_VER_FRONT_PORCH		22		//垂直前廊
//如下几个参数，自动计算
#define SSD_HT	(SSD_HOR_RESOLUTION+SSD_HOR_BACK_PORCH+SSD_HOR_FRONT_PORCH)
#define SSD_HPS	(SSD_HOR_BACK_PORCH)
#define SSD_VT 	(SSD_VER_RESOLUTION+SSD_VER_BACK_PORCH+SSD_VER_FRONT_PORCH)
#define SSD_VPS (SSD_VER_BACK_PORCH)
				  		 
#endif  
	 
	 



