//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//测试硬件：单片机STM32F429IGT6,正点原子Apollo STM32F4/F7开发板,主频180MHZ，晶振12MHZ
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
#include "touch.h" 
#include "ft5426.h"
#include "rtp.h"	
    
//初始化触摸屏控制器参数
_m_tp_dev tp_dev=
{
 	0,
	0,
	0,
	0,
	0,
	0,	  	 		
	0,
	0,	  	 		
};					

/*****************************************************************************
 * @name       :u8 TP_Init(void)
 * @date       :2020-05-13 
 * @function   :DInitialization  touch screen 
 * @parameters :None
 * @retvalue   :0-Initialization successful
								1-initialization failed
******************************************************************************/
u8 TP_Init(void)
{
	u8 ret=0;
#if TP_TYPE
	ret=FT5426_Init();
	tp_dev.touchtype|=0x80;			//电容屏 
#else	
	ret=RTP_Init();
	tp_dev.touchtype|=0x00;			//电阻屏
#endif
	return ret;
}
