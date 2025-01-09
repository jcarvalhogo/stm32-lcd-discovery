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
#include "ft5426.h"
#include "touch.h"
#include "ctpiic.h"
#include "delay.h" 
#include "string.h" 
#include "lcd.h"
#include "exti.h"

extern u8 touch_flag;

/*****************************************************************************
 * @name       :u8 FT5426_WR_Reg(u16 reg,u8 *buf,u8 len)
 * @date       :2020-05-13 
 * @function   :Write data to ft5426 once
 * @parameters :reg:Start register address for written
								buf:the buffer of data written
								len:Length of data written
 * @retvalue   :0-Write succeeded 
								1-Write failed
******************************************************************************/ 
u8 FT5426_WR_Reg(u16 reg,u8 *buf,u8 len)
{
	u8 i;
	u8 ret=0;
	CTP_IIC_Start();	 
	CTP_IIC_Send_Byte(FT_CMD_WR);	//发送写命令 	 
	CTP_IIC_Wait_Ack(); 	 										  		   
	CTP_IIC_Send_Byte(reg&0XFF);   	//发送低8位地址
	CTP_IIC_Wait_Ack();  
	for(i=0;i<len;i++)
	{	   
		CTP_IIC_Send_Byte(buf[i]);  	//发数据
		ret=CTP_IIC_Wait_Ack();
		if(ret)
		{
			break;
		}
	}
  CTP_IIC_Stop();					//产生一个停止条件	    
	return ret; 
}

/*****************************************************************************
 * @name       :void FT5426_RD_Reg(u16 reg,u8 *buf,u8 len)
 * @date       :2020-05-13 
 * @function   :Read data to ft5426 once
 * @parameters :reg:Start register address for read
								buf:the buffer of data read
								len:Length of data read
 * @retvalue   :none
******************************************************************************/			  
void FT5426_RD_Reg(u16 reg,u8 *buf,u8 len)
{
	u8 i; 
 	CTP_IIC_Start();	
 	CTP_IIC_Send_Byte(FT_CMD_WR);   	//发送写命令 	 
	CTP_IIC_Wait_Ack(); 	 										  		   
 	CTP_IIC_Send_Byte(reg&0XFF);   	//发送低8位地址
	CTP_IIC_Wait_Ack();  
 	CTP_IIC_Start();  	 	   
	CTP_IIC_Send_Byte(FT_CMD_RD);   	//发送读命令		   
	CTP_IIC_Wait_Ack();	   
	for(i=0;i<len;i++)
	{	   
    	buf[i]=CTP_IIC_Read_Byte(i==(len-1)?0:1); //发数据	  
	} 
  CTP_IIC_Stop();//产生一个停止条件     
} 

/*****************************************************************************
 * @name       :u8 FT5426_Init(void)
 * @date       :2020-05-13 
 * @function   :Initialize the ft5426 touch screen
 * @parameters :none
 * @retvalue   :0-Initialization successful
								1-initialization failed
******************************************************************************/		
u8 FT5426_Init(void)
{
	u8 temp[2]; 
	GPIO_InitTypeDef GPIO_Initure;
 
  __HAL_RCC_GPIOH_CLK_ENABLE();			//开启GPIOH时钟
  __HAL_RCC_GPIOI_CLK_ENABLE();			//开启GPIOI时钟
                
  //PH7
  GPIO_Initure.Pin=GPIO_PIN_7;            //PH7
  GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
  GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
  GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
  HAL_GPIO_Init(GPIOH,&GPIO_Initure);     //初始化
            
  //PI8
  GPIO_Initure.Pin=GPIO_PIN_8;            //PI8
  GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
  HAL_GPIO_Init(GPIOI,&GPIO_Initure);     //初始化		
#if SCAN_TYPE
	Touch_EXTI_Init();
#endif	
	
 
	CTP_IIC_Init();      	//初始化电容屏的I2C总线  
	FT_RST=0;				//复位
	delay_ms(20);
 	FT_RST=1;				//释放复位		    
	delay_ms(50);  	
	temp[0]=0;
	FT5426_WR_Reg(FT_DEVIDE_MODE,temp,1);	//进入正常操作模式 
#if SCAN_TYPE
	temp[0]=1;
#endif
	FT5426_WR_Reg(FT_ID_G_MODE,temp,1);		//查询模式 
	temp[0]=22;								//触摸有效值，22，越小越灵敏	
	FT5426_WR_Reg(FT_ID_G_THGROUP,temp,1);	//设置触摸有效值
	temp[0]=12;								//激活周期，不能小于12，最大14
	FT5426_WR_Reg(FT_ID_G_PERIODACTIVE,temp,1); 
	//读取版本号，参考值：0x3003
	FT5426_RD_Reg(FT_ID_G_LIB_VERSION,&temp[0],2);  
	if((temp[0]==0X30&&temp[1]==0X03)||temp[1]==0X01||temp[1]==0X02)//版本:0X3003
	{ 
//		printf("CTP ID:%x\r\n",((u16)temp[0]<<8)+temp[1]);
		return 0;
	} 
	return 1;
}

const u16 FT5426_TPX_TBL[5]={FT_TP1_REG,FT_TP2_REG,FT_TP3_REG,FT_TP4_REG,FT_TP5_REG};

/*****************************************************************************
 * @name       :u8 FT5426_Scan(void)
 * @date       :2020-05-13 
 * @function   :Scan touch screen (query mode)
 * @parameters :none
 * @retvalue   :Current touch screen status
								0-No touch
								1-With touch
******************************************************************************/	
u8 FT5426_Scan(void)
{
	u8 buf[4];
	u8 i=0;
	u8 res=0;
	u8 temp;
	u8 mode;
#if SCAN_TYPE
	if(touch_flag)
#else
	static u8 t=0;//控制查询间隔,从而降低CPU占用率   
	t++;
	if((t%10)==0||t<10)//空闲时,每进入10次CTP_Scan函数才检测1次,从而节省CPU使用率
#endif
	{
		FT5426_RD_Reg(FT_REG_NUM_FINGER,&mode,1);//读取触摸点的状态  
		if((mode&0XF)&&((mode&0XF)<6))
		{
			temp=0XFF<<(mode&0XF);//将点的个数转换为1的位数,匹配tp_dev.sta定义 
			tp_dev.sta=(~temp)|TP_PRES_DOWN|TP_CATH_PRES; 
			for(i=0;i<CTP_MAX_TOUCH;i++)
			{
				if(tp_dev.sta&(1<<i))	//触摸有效?
				{
					FT5426_RD_Reg(FT5426_TPX_TBL[i],buf,4);	//读取XY坐标值 
					switch(lcd_dev.dir)
					{
						case 0:
							tp_dev.x[i]=lcd_dev.width-(((u16)(buf[0]&0X0F)<<8)+buf[1]);
							tp_dev.y[i]=((u16)(buf[2]&0X0F)<<8)+buf[3];						
							break;
						case 1:
							tp_dev.y[i]=((u16)(buf[0]&0X0F)<<8)+buf[1];
							tp_dev.x[i]=((u16)(buf[2]&0X0F)<<8)+buf[3];						
							break;
						case 2:
							tp_dev.x[i]=((u16)(buf[0]&0X0F)<<8)+buf[1];
							tp_dev.y[i]=lcd_dev.height-(((u16)(buf[2]&0X0F)<<8)+buf[3]);								
							break;
						case 3:
							tp_dev.y[i]=lcd_dev.height-(((u16)(buf[0]&0X0F)<<8)+buf[1]);
							tp_dev.x[i]=lcd_dev.width-(((u16)(buf[2]&0X0F)<<8)+buf[3]);	
							break;
					} 
					if((buf[0]&0XF0)!=0X80)tp_dev.x[i]=tp_dev.y[i]=0;//必须是contact事件，才认为有效
					//printf("x[%d]:%d,y[%d]:%d\r\n",i,tp_dev.x[i],i,tp_dev.y[i]);
				}			
			} 
			res=1;
			if(tp_dev.x[0]==0 && tp_dev.y[0]==0)mode=0;	//读到的数据都是0,则忽略此次数据
#if !SCAN_TYPE
			t=0;		//触发一次,则会最少连续监测10次,从而提高命中率
#endif
		}
	}
	if((mode&0X1F)==0)//无触摸点按下
	{ 
#if SCAN_TYPE
		touch_flag=0;
#endif
		if(tp_dev.sta&TP_PRES_DOWN)	//之前是被按下的
		{
			tp_dev.sta&=~(1<<7);	//标记按键松开
		}else						//之前就没有被按下
		{ 
			tp_dev.x[0]=0xffff;
			tp_dev.y[0]=0xffff;
			tp_dev.sta&=0XE0;	//清除点有效标记	
		}	 
	} 
#if !SCAN_TYPE	
	if(t>240)t=10;//重新从10开始计数
#endif
	return res;
}
 





































