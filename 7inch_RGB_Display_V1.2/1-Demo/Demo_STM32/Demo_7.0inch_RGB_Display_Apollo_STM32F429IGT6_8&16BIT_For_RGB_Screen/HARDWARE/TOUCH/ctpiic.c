//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//测试硬件：单片机STM32F103RCT6,正点原子MiniSTM32开发板,主频72MHZ，晶振12MHZ
//QDtech-TFT液晶驱动 for STM32 IO模拟
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
//此模块可以直接插入正点原子MiniSTM32开发板TFTLCD插槽，无需手动接线
//STM32连接引脚是指TFTLCD插槽引脚内部连接的STM32引脚
//=========================================电源接线================================================//
//     LCD模块             TFTLCD插槽引脚        STM32连接引脚
//      VDD       --->         5V/3.3              DC5V/3.3V          //电源
//      GND       --->          GND                  GND              //电源地
//=======================================液晶屏数据线接线==========================================//
//本模块默认数据总线类型为16位并口总线
//     LCD模块             TFTLCD插槽引脚        STM32连接引脚
//     D0~D15     --->         D0~D15               PB0~PB15          //液晶屏16位并口数据信号（8位模式：DB0~DB7接D0~D7）
//=======================================液晶屏控制线接线==========================================//
//     LCD模块 			       TFTLCD插槽引脚        STM32连接引脚 
//       WR       --->          WR                   PC7             //液晶屏写数据控制信号
//       RD       --->          RD                   PC6             //液晶屏读数据控制信号
//       RS       --->          RS                   PC8             //液晶屏数据/命令控制信号
//       RST      --->          RST                  PC4             //液晶屏复位控制信号
//       CS       --->          CS                   PC9             //液晶屏片选控制信号
//       BL       --->          BL                   PC10            //液晶屏背光控制信号
//=========================================触摸屏触接线=========================================//
//如果模块不带触摸功能或者带有触摸功能，但是不需要触摸功能，则不需要进行触摸屏接线
//	   LCD模块             TFTLCD插槽引脚        STM32连接引脚 
//      INI       --->          PEN                  PC1             //触摸屏触摸中断信号
//      SDA       --->          MOSI                 PC3             //触摸屏IIC总线数据信号
//      CRST      --->          TCS                  PC13            //触摸屏复位信号
//      CLK       --->          CLK                  PC0             //触摸屏IIC总线时钟信号
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
#include "ctpiic.h"
#include "delay.h"	 

/*****************************************************************************
 * @name       :void CTP_Delay(void)
 * @date       :2020-05-13 
 * @function   :Delay in controlling IIC speed
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void CTP_Delay(void)
{
	delay_us(2);
} 

/*****************************************************************************
 * @name       :void CTP_IIC_Init(void)
 * @date       :2020-05-13 
 * @function   :Initialize IIC
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void CTP_IIC_Init(void)
{	
      GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOH_CLK_ENABLE();			//开启GPIOH时钟
	__HAL_RCC_GPIOI_CLK_ENABLE();			//开启GPIOI时钟
    
    GPIO_Initure.Pin=GPIO_PIN_6;            //PH6
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;    //高速
    HAL_GPIO_Init(GPIOH,&GPIO_Initure);     //初始化
	
    GPIO_Initure.Pin=GPIO_PIN_3;            //PI3
    HAL_GPIO_Init(GPIOI,&GPIO_Initure);     //初始化	
}

/*****************************************************************************
 * @name       :void CTP_IIC_Start(void)
 * @date       :2020-05-13 
 * @function   :Generating IIC starting signal
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void CTP_IIC_Start(void)
{
	CTP_SDA_OUT();     //sda线输出
	CTP_IIC_SDA=1;	  	  
	CTP_IIC_SCL=1;
	delay_us(30);
 	CTP_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	CTP_Delay();
	CTP_IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	

/*****************************************************************************
 * @name       :void CTP_IIC_Stop(void)
 * @date       :2020-05-13 
 * @function   :Generating IIC stop signal
 * @parameters :None
 * @retvalue   :None
******************************************************************************/   
void CTP_IIC_Stop(void)
{ 
	CTP_SDA_OUT();//sda线输出
	CTP_IIC_SCL=0;
	CTP_IIC_SDA=0; //STOP:when CLK is high DATA change form low to high
	delay_us(30);
	CTP_IIC_SCL=1;
	CTP_Delay();
	CTP_IIC_SDA=1; //发送I2C总线结束信号 
}

/*****************************************************************************
 * @name       :u8 CTP_IIC_Wait_Ack(void)
 * @date       :2020-05-13 
 * @function   :Wait for the response signal
 * @parameters :None
 * @retvalue   :0-receive response signal successfully
								1-receive response signal unsuccessfully
******************************************************************************/ 
u8 CTP_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	CTP_SDA_IN();      //SDA设置为输入  
	CTP_IIC_SDA=1;	   
	CTP_IIC_SCL=1;	
	CTP_Delay();	
	while(CTP_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			CTP_IIC_Stop();
			return 1;
		} 
		CTP_Delay();
	}
	CTP_IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 

/*****************************************************************************
 * @name       :void CTP_IIC_Ack(void)
 * @date       :2020-05-13 
 * @function   :Generate ACK response signal
 * @parameters :None
 * @retvalue   :None
******************************************************************************/ 
void CTP_IIC_Ack(void)
{
	CTP_IIC_SCL=0;
	CTP_SDA_OUT();
	CTP_IIC_SDA=0;
	CTP_Delay();
	CTP_IIC_SCL=1;
	CTP_Delay();
	CTP_IIC_SCL=0;
}

/*****************************************************************************
 * @name       :void CTP_IIC_NAck(void)
 * @date       :2020-05-13 
 * @function   :Don't generate ACK response signal
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	    
void CTP_IIC_NAck(void)
{
	CTP_IIC_SCL=0;
	CTP_SDA_OUT();
	CTP_IIC_SDA=1;
	CTP_Delay();
	CTP_IIC_SCL=1;
	CTP_Delay();
	CTP_IIC_SCL=0;
}	

/*****************************************************************************
 * @name       :void CTP_IIC_Send_Byte(u8 txd)
 * @date       :2020-05-13 
 * @function   :send a byte data by IIC bus
 * @parameters :txd:Data to be sent
 * @retvalue   :None
******************************************************************************/					 				     		  
void CTP_IIC_Send_Byte(u8 txd)
{                        
  u8 t;   
	CTP_SDA_OUT(); 	    
  CTP_IIC_SCL=0;//拉低时钟开始数据传输
  for(t=0;t<8;t++)
  {              
    CTP_IIC_SDA=(txd&0x80)>>7;
    txd<<=1; 	
		CTP_Delay();		
		CTP_IIC_SCL=1;
		CTP_Delay();
		CTP_IIC_SCL=0;	
		CTP_Delay();
  }	 
} 	

/*****************************************************************************
 * @name       :u8 CTP_IIC_Read_Byte(unsigned char ack)
 * @date       :2020-05-13 
 * @function   :read a byte data by IIC bus
 * @parameters :ack:0-send nACK
									  1-send ACK
 * @retvalue   :Data to be read
******************************************************************************/	    
u8 CTP_IIC_Read_Byte(unsigned char ack)
{
	u8 i,receive=0;
 	CTP_SDA_IN();//SDA设置为输入
	delay_us(30);
  for(i=0;i<8;i++ )
	{
    CTP_IIC_SCL=0; 	    	   
		CTP_Delay();
		CTP_IIC_SCL=1;  
		receive<<=1;
		if(CTP_READ_SDA)receive++;
		CTP_Delay();			
	}	  				 
	if (!ack)CTP_IIC_NAck();//发送nACK
	else CTP_IIC_Ack(); //发送ACK   
 	return receive;
}




























