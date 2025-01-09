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

#include "rgb_lcd.h"
#include "lcd.h"

LTDC_HandleTypeDef  RGBLCD_Handler;	    //RGBLCD句柄
DMA2D_HandleTypeDef DMA2D_Handler; 	    //DMA2D句柄

//根据不同的颜色格式,定义帧缓存数组
#if RGBLCD_PIXFORMAT==RGBLCD_PIXFORMAT_ARGB8888||RGBLCD_PIXFORMAT==RGBLCD_PIXFORMAT_RGB888
	u32 rgb_lcd_framebuf[1024][600] __attribute__((at(RGBLCD_FRAME_BUF_ADDR)));	//定义最大屏分辨率时,RGBLCD所需的帧缓存数组大小
#else
	u16 rgb_lcd_framebuf[1024][600] __attribute__((at(RGBLCD_FRAME_BUF_ADDR)));	//定义最大屏分辨率时,RGBLCD所需的帧缓存数组大小
#endif

u32 *rgblcd_framebuf[2];					//RGB LCD帧缓存数组指针,必须指向对应大小的内存区域
rgblcd_dev rgblcd;						    //管理RGB LCD的重要参数

//打开LCD开关
//lcd_switch:1 打开,0，关闭
void RGBLCD_Switch(u8 sw)
{
	if(sw==1)
	{	
		__HAL_LTDC_ENABLE(&RGBLCD_Handler);
	}
	else if(sw==0)
	{
		__HAL_LTDC_DISABLE(&RGBLCD_Handler);
	}
}

//开关指定层
//layerx:层号,0,第一层; 1,第二层
//sw:1 打开;0关闭
void RGBLCD_Layer_Switch(u8 layerx,u8 sw)
{
	if(sw==1)
	{	
		__HAL_LTDC_LAYER_ENABLE(&RGBLCD_Handler,layerx);
	}
	else if(sw==0) 
	{
		__HAL_LTDC_LAYER_DISABLE(&RGBLCD_Handler,layerx);
	}
	__HAL_LTDC_RELOAD_CONFIG(&RGBLCD_Handler);
}

//选择层
//layerx:层号;0,第一层;1,第二层;
void RGBLCD_Select_Layer(u8 layerx)
{
	rgblcd.activelayer=layerx;
}

//设置LCD显示方向
//dir:0,竖屏；1,横屏
void RGBLCD_Display_Dir(u8 dir)
{
  rgblcd.dir=dir%4; 	//显示方向
	if(dir==0||dir==2)			//竖屏
	{
		rgblcd.width=rgblcd.pheight;
		rgblcd.height=rgblcd.pwidth;	
	}else if(dir==1||dir==3)	//横屏
	{
		rgblcd.width=rgblcd.pwidth;
		rgblcd.height=rgblcd.pheight;
	}
	lcd_dev.dir = rgblcd.dir;
	lcd_dev.width = rgblcd.width;
	lcd_dev.height = rgblcd.height;
}

//画点函数
//x,y:坐标
//color:颜色值
void RGBLCD_Draw_Point(u16 x,u16 y,u32 color)
{ 
#if RGBLCD_PIXFORMAT==RGBLCD_PIXFORMAT_ARGB8888||RGBLCD_PIXFORMAT==RGBLCD_PIXFORMAT_RGB888
	switch(rgblcd.dir)
	{
		case 0:
			*(u32*)((u32)rgblcd_framebuf[rgblcd.activelayer]+rgblcd.pixsize*(rgblcd.pwidth*(rgblcd.pheight-x-1)+y))=color; 
		break;
		case 1:
			*(u32*)((u32)rgblcd_framebuf[rgblcd.activelayer]+rgblcd.pixsize*(rgblcd.pwidth*y+x))=color;
		break;
		case 2:
			*(u32*)((u32)rgblcd_framebuf[rgblcd.activelayer]+rgblcd.pixsize*(rgblcd.pwidth*x+rgblcd.pwidth-y-1))=color;
		break;
		case 3:
			*(u32*)((u32)rgblcd_framebuf[rgblcd.activelayer]+rgblcd.pixsize*(rgblcd.pwidth*(rgblcd.pheight-y-1)+rgblcd.pwidth-x-1))=color;
		break;
	}
#else
	switch(rgblcd.dir)
	{
		case 0:
			*(u16*)((u32)rgblcd_framebuf[rgblcd.activelayer]+rgblcd.pixsize*(rgblcd.pwidth*(rgblcd.pheight-x-1)+y))=color; 
		break;
		case 1:
			*(u16*)((u32)rgblcd_framebuf[rgblcd.activelayer]+rgblcd.pixsize*(rgblcd.pwidth*y+x))=color;
		break;
		case 2:
			*(u16*)((u32)rgblcd_framebuf[rgblcd.activelayer]+rgblcd.pixsize*(rgblcd.pwidth*x+rgblcd.pwidth-y-1))=color;
		break;
		case 3:
			*(u16*)((u32)rgblcd_framebuf[rgblcd.activelayer]+rgblcd.pixsize*(rgblcd.pwidth*(rgblcd.pheight-y-1)+rgblcd.pwidth-x-1))=color;
		break;
	}
#endif
}

//读点函数
//返回值:颜色值
u32 RGBLCD_Read_Point(u16 x,u16 y)
{ 
	u32 color=0;
#if RGBLCD_PIXFORMAT==RGBLCD_PIXFORMAT_ARGB8888||RGBLCD_PIXFORMAT==RGBLCD_PIXFORMAT_RGB888
	switch(rgblcd.dir)
	{
		case 0:
			color	= *(u32*)((u32)rgblcd_framebuf[rgblcd.activelayer]+rgblcd.pixsize*(rgblcd.pwidth*(rgblcd.pheight-x-1)+y)); 
		break;
		case 1:
			color	= *(u32*)((u32)rgblcd_framebuf[rgblcd.activelayer]+rgblcd.pixsize*(rgblcd.pwidth*y+x));
		break;
		case 2:
			color	= *(u32*)((u32)rgblcd_framebuf[rgblcd.activelayer]+rgblcd.pixsize*(rgblcd.pwidth*x+rgblcd.pwidth-y-1));
		break;
		case 3:
			color	= *(u32*)((u32)rgblcd_framebuf[rgblcd.activelayer]+rgblcd.pixsize*(rgblcd.pwidth*(rgblcd.pheight-y-1)+rgblcd.pwidth-x-1));
		break;
	}
#else
	switch(rgblcd.dir)
	{
		case 0:
			color	= *(u16*)((u32)rgblcd_framebuf[rgblcd.activelayer]+rgblcd.pixsize*(rgblcd.pwidth*(rgblcd.pheight-x-1)+y)); 
		break;
		case 1:
			color	= *(u16*)((u32)rgblcd_framebuf[rgblcd.activelayer]+rgblcd.pixsize*(rgblcd.pwidth*y+x));
		break;
		case 2:
			color	= *(u16*)((u32)rgblcd_framebuf[rgblcd.activelayer]+rgblcd.pixsize*(rgblcd.pwidth*x+rgblcd.pwidth-y-1));
		break;
		case 3:
			color	= *(u16*)((u32)rgblcd_framebuf[rgblcd.activelayer]+rgblcd.pixsize*(rgblcd.pwidth*(rgblcd.pheight-y-1)+rgblcd.pwidth-x-1));
		break;
	}
#endif
	return color;
}

//LTDC填充矩形,DMA2D填充
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//注意:sx,ex,不能大于lcddev.width-1;sy,ey,不能大于lcddev.height-1!!!
//color:要填充的颜色
void RGBLCD_Fill_color(u16 sx,u16 sy,u16 ex,u16 ey,u32 color)
{ 
	u32 psx,psy,pex,pey;	//以LCD面板为基准的坐标系,不随横竖屏变化而变化
	u32 timeout=0; 
	u16 offline;
	u32 addr; 
	//坐标系转换
	switch(rgblcd.dir)
	{
		case 0:
		{
			psx=sy;
			psy=rgblcd.pheight-ex-1;
			pex=ey;
			pey=rgblcd.pheight-sx-1;
		}
		break;
		case 1:
		{
			psx=sx;
			psy=sy;
			pex=ex;
			pey=ey;
		}
		break;
		case 2:
		{
			psx=rgblcd.pwidth-ey-1;
			psy=sx;
			pex=rgblcd.pwidth-sy-1;
			pey=ex;
		}
		break;
		case 3:
		{
			psx=rgblcd.pwidth-ex-1;
			psy=rgblcd.pheight-ey-1;
			pex=rgblcd.pwidth-sx-1;
			pey=rgblcd.pheight-sy-1;
		}
		break;
	}
	offline=rgblcd.pwidth-(pex-psx+1);
	addr=((u32)rgblcd_framebuf[rgblcd.activelayer]+rgblcd.pixsize*(rgblcd.pwidth*psy+psx));
	__HAL_RCC_DMA2D_CLK_ENABLE();	//使能DM2D时钟
	DMA2D->CR&=~(DMA2D_CR_START);	//先停止DMA2D
	DMA2D->CR=DMA2D_R2M;			//寄存器到存储器模式
	DMA2D->OPFCCR=RGBLCD_PIXFORMAT;	//设置颜色格式
	DMA2D->OOR=offline;				//设置行偏移 

	DMA2D->OMAR=addr;				//输出存储器地址
	DMA2D->NLR=(pey-psy+1)|((pex-psx+1)<<16);	//设定行数寄存器
	DMA2D->OCOLR=color;						//设定输出颜色寄存器 
	DMA2D->CR|=DMA2D_CR_START;				//启动DMA2D
	while((DMA2D->ISR&(DMA2D_FLAG_TC))==0)	//等待传输完成
	{
		timeout++;
		if(timeout>0X1FFFFF)break;	//超时退出
	}  
	DMA2D->IFCR|=DMA2D_FLAG_TC;		//清除传输完成标志 	
}


//在指定区域内填充指定颜色块,DMA2D填充	
//此函数仅支持u16,RGB565格式的颜色数组填充.
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)  
//注意:sx,ex,不能大于lcddev.width-1;sy,ey,不能大于lcddev.height-1!!!
//color:要填充的颜色数组首地址
void RGBLCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
{
	u32 psx,psy,pex,pey;	//以LCD面板为基准的坐标系,不随横竖屏变化而变化
	u32 timeout=0; 
	u16 offline;
	u32 addr; 
	//坐标系转换
		switch(rgblcd.dir)
	{
		case 0:
		{
			psx=sy;
			psy=rgblcd.pheight-ex-1;
			pex=ey;
			pey=rgblcd.pheight-sx-1;
		}
		break;
		case 1:
		{
			psx=sx;
			psy=sy;
			pex=ex;
			pey=ey;
		}
		break;
		case 2:
		{
			psx=rgblcd.pwidth-ey-1;
			psy=sx;
			pex=rgblcd.pwidth-sy-1;
			pey=ex;
		}
		break;
		case 3:
		{
			psx=rgblcd.pwidth-sx-1;
			psy=rgblcd.pheight-sy-1;
			pex=rgblcd.pwidth-ex-1;
			pey=rgblcd.pheight-ey-1;
		}
		break;
	}
	offline=rgblcd.pwidth-(pex-psx+1);
	addr=((u32)rgblcd_framebuf[rgblcd.activelayer]+rgblcd.pixsize*(rgblcd.pwidth*psy+psx));
	__HAL_RCC_DMA2D_CLK_ENABLE();	//使能DM2D时钟
	DMA2D->CR&=~(DMA2D_CR_START);	//先停止DMA2D
	DMA2D->CR=DMA2D_M2M;			//存储器到存储器模式
	DMA2D->FGPFCCR=RGBLCD_PIXFORMAT;	//设置颜色格式
	DMA2D->FGOR=0;					//前景层行偏移为0
	DMA2D->OOR=offline;				//设置行偏移 

	DMA2D->FGMAR=(u32)color;		//源地址
	DMA2D->OMAR=addr;				//输出存储器地址
	DMA2D->NLR=(pey-psy+1)|((pex-psx+1)<<16);	//设定行数寄存器 
	DMA2D->CR|=DMA2D_CR_START;					//启动DMA2D
	while((DMA2D->ISR&(DMA2D_FLAG_TC))==0)	//等待传输完成
	{
		timeout++;
		if(timeout>0X1FFFFF)break;	//超时退出
	} 
	DMA2D->IFCR|=DMA2D_FLAG_TC;				//清除传输完成标志  	
} 

//LCD清屏
//color:颜色值
void RGBLCD_Clear(u32 color)
{
	RGBLCD_Fill_color(0,0,rgblcd.width-1,rgblcd.height-1,color);
}

//LTDC时钟(Fdclk)设置函数
//PLL3_VCO Input=HSE_VALUE/PLL3M
//PLL3_VCO Output=PLL3_VCO Input * PLL3N
//PLLLCDCLK = PLL3_VCO Output/PLL3R
//假如HSE_VALUE=25MHz，PLL3M=5，PLL3N=160,PLL3R=88
//LTDCLK=PLLLCDCLK=25/5*160/88=9MHz
//返回值:0,成功;1,失败。
u8 RGBLCD_Clk_Set(u32 pllsain,u32 pllsair,u32 pllsaidivr)
{
	RCC_PeriphCLKInitTypeDef PeriphClkIniture;
	
	//LTDC输出像素时钟，需要根据自己所使用的LCD数据手册来配置！
  PeriphClkIniture.PeriphClockSelection=RCC_PERIPHCLK_LTDC;	//LTDC时钟 	
	PeriphClkIniture.PLLSAI.PLLSAIN=pllsain;    
	PeriphClkIniture.PLLSAI.PLLSAIR=pllsair;  
	PeriphClkIniture.PLLSAIDivR=pllsaidivr;
	if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkIniture)==HAL_OK) //配置像素时钟，这里配置为时钟为18.75MHZ
    {
        return 0;   //成功
    }
    else return 1;  //失败    
}

//LTDC,层颜窗口设置,窗口以LCD面板坐标系为基准
//注意:此函数必须在LTDC_Layer_Parameter_Config之后再设置.
//layerx:层值,0/1.
//sx,sy:起始坐标
//width,height:宽度和高度
void RGBLCD_Layer_Window_Config(u8 layerx,u16 sx,u16 sy,u16 width,u16 height)
{
    HAL_LTDC_SetWindowPosition(&RGBLCD_Handler,sx,sy,layerx);  //设置窗口的位置
    HAL_LTDC_SetWindowSize(&RGBLCD_Handler,width,height,layerx);//设置窗口大小    
}

//LTDC,基本参数设置.
//注意:此函数,必须在LTDC_Layer_Window_Config之前设置.
//layerx:层值,0/1.
//bufaddr:层颜色帧缓存起始地址
//pixformat:颜色格式.0,ARGB8888;1,RGB888;2,RGB565;3,ARGB1555;4,ARGB4444;5,L8;6;AL44;7;AL88
//alpha:层颜色Alpha值,0,全透明;255,不透明
//alpha0:默认颜色Alpha值,0,全透明;255,不透明
//bfac1:混合系数1,4(100),恒定的Alpha;6(101),像素Alpha*恒定Alpha
//bfac2:混合系数2,5(101),恒定的Alpha;7(111),像素Alpha*恒定Alpha
//bkcolor:层默认颜色,32位,低24位有效,RGB888格式
//返回值:无
void RGBLCD_Layer_Parameter_Config(u8 layerx,u32 bufaddr,u8 pixformat,u8 alpha,u8 alpha0,u8 bfac1,u8 bfac2,u32 bkcolor)
{
	LTDC_LayerCfgTypeDef pLayerCfg;
	
	pLayerCfg.WindowX0=0;                       //窗口起始X坐标
	pLayerCfg.WindowY0=0;                       //窗口起始Y坐标
	pLayerCfg.WindowX1=rgblcd.pwidth;          //窗口终止X坐标
	pLayerCfg.WindowY1=rgblcd.pheight;         //窗口终止Y坐标
	pLayerCfg.PixelFormat=pixformat;		    //像素格式
	pLayerCfg.Alpha=alpha;				        //Alpha值设置，0~255,255为完全不透明
	pLayerCfg.Alpha0=alpha0;			        //默认Alpha值
	pLayerCfg.BlendingFactor1=(u32)bfac1<<8;    //设置层混合系数
	pLayerCfg.BlendingFactor2=(u32)bfac2<<8;	//设置层混合系数
	pLayerCfg.FBStartAdress=bufaddr;	        //设置层颜色帧缓存起始地址
	pLayerCfg.ImageWidth=rgblcd.pwidth;        //设置颜色帧缓冲区的宽度    
	pLayerCfg.ImageHeight=rgblcd.pheight;      //设置颜色帧缓冲区的高度
	pLayerCfg.Backcolor.Red=(u8)(bkcolor&0X00FF0000)>>16;   //背景颜色红色部分
	pLayerCfg.Backcolor.Green=(u8)(bkcolor&0X0000FF00)>>8;  //背景颜色绿色部分
	pLayerCfg.Backcolor.Blue=(u8)bkcolor&0X000000FF;        //背景颜色蓝色部分
	HAL_LTDC_ConfigLayer(&RGBLCD_Handler,&pLayerCfg,layerx);   //设置所选中的层
}  

//读取面板参数,新增了判断悬空全一的排查
//PG6=R7(M0);PI2=G7(M1);PI7=B7(M2);
//M2:M1:M0
//0 :0 :0	//4.3寸480*272 RGB屏,ID=0X4342
//0 :0 :1	//7寸800*480 RGB屏,ID=0X7084
//0 :1 :0	//7寸1024*600 RGB屏,ID=0X7016
//0 :1 :1	//7寸1280*800 RGB屏,ID=0X7018
//1 :0 :0	//8寸1024*600 RGB屏,ID=0X8016
//1 :0 :1   //10.1寸1280*800,RGB屏,ID=0X1018
//1 :1 :1   //VGA显示器
//读取面板参数
//PG6=R7(M0);PI2=G7(M1);PI7=B7(M2);
//M2:M1:M0
//0 :0 :0	//4.3寸480*272 RGB屏,ID=0X4342
//0 :0 :1	//7寸800*480 RGB屏,ID=0X7084
//0 :1 :0	//7寸1024*600 RGB屏,ID=0X7016
//0 :1 :1	//7寸1280*800 RGB屏,ID=0X7018
//1 :0 :0	//8寸1024*768 RGB屏,ID=0X8017 
//返回值:LCD ID:0,非法;其他值,ID;
u16 RGBLCD_Read_ID(void)
{
	u8 idx=0;
  GPIO_InitTypeDef GPIO_Initure;
  __HAL_RCC_GPIOG_CLK_ENABLE();       //使能GPIOG时钟
	__HAL_RCC_GPIOI_CLK_ENABLE();       //使能GPIOI时钟
    
  GPIO_Initure.Pin=GPIO_PIN_6;        //PG6
  GPIO_Initure.Mode=GPIO_MODE_INPUT;  //输入
  GPIO_Initure.Pull=GPIO_PULLUP;      //上拉
  GPIO_Initure.Speed=GPIO_SPEED_HIGH; //高速
  HAL_GPIO_Init(GPIOG,&GPIO_Initure); //初始化   
  GPIO_Initure.Pin=GPIO_PIN_2|GPIO_PIN_7; //PI2,7
  HAL_GPIO_Init(GPIOI,&GPIO_Initure);     //初始化  
  
  idx=(u8)HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_6); //读取M0
  idx|=(u8)HAL_GPIO_ReadPin(GPIOI,GPIO_PIN_2)<<1;//读取M1
  idx|=(u8)HAL_GPIO_ReadPin(GPIOI,GPIO_PIN_7)<<2;//读取M2

	if(idx==1)return 0X7584;	//7寸屏,800*480分辨率
	else if(idx==2)return 0X7016;	//7寸屏,1024*600分辨率	
	else return 0;
}

//LCD初始化函数
void RGBLCD_Init(void)
{   
	u16 lcdid=0;
	
	lcdid=RGBLCD_Read_ID();			//读取LCD面板ID	
	if(lcdid==0X7584)
	{
		rgblcd.pwidth=800;			    //面板宽度,单位:像素
		rgblcd.pheight=480;		    //面板高度,单位:像素
		rgblcd.hsw=1;				    //水平同步宽度
		rgblcd.vsw=1;				    //垂直同步宽度
		rgblcd.hbp=46;				    //水平后廊
		rgblcd.vbp=23;				    //垂直后廊
		rgblcd.hfp=210;			    //水平前廊
		rgblcd.vfp=22;				    //垂直前廊
		RGBLCD_Clk_Set(396,3,RCC_PLLSAIDIVR_4);			//设置像素时钟 33M(如果开双显,需要降低DCLK到:18.75Mhz  pll3r=43,才会比较好)
	}else if(lcdid==0X7016)		
	{
		rgblcd.pwidth=1024;			//面板宽度,单位:像素
		rgblcd.pheight=600;			//面板高度,单位:像素
    rgblcd.hsw=20;				    //水平同步宽度
		rgblcd.vsw=3;				    //垂直同步宽度
		rgblcd.hbp=140;			    //水平后廊
		rgblcd.vbp=20;				    //垂直后廊
		rgblcd.hfp=160;			    //水平前廊
		rgblcd.vfp=12;				    //垂直前廊
		RGBLCD_Clk_Set(360,2,RCC_PLLSAIDIVR_4);			//设置像素时钟  45Mhz 
		//其他参数待定.
	}
   
#if RGBLCD_PIXFORMAT==RGBLCD_PIXFORMAT_ARGB8888||RGBLCD_PIXFORMAT==RGBLCD_PIXFORMAT_RGB888 
	rgblcd_framebuf[0]=(u32*)&ltdc_lcd_framebuf;
	lcdltdc.pixsize=4;				//每个像素占4个字节
#else 
  rgblcd.pixsize=2;				//每个像素占2个字节
	rgblcd_framebuf[0]=(u32*)&rgb_lcd_framebuf;
#endif 	
    
    //LTDC配置
    RGBLCD_Handler.Instance=LTDC;
    RGBLCD_Handler.Init.HSPolarity=LTDC_HSPOLARITY_AL;         //水平同步极性
    RGBLCD_Handler.Init.VSPolarity=LTDC_VSPOLARITY_AL;         //垂直同步极性
    RGBLCD_Handler.Init.DEPolarity=LTDC_DEPOLARITY_AL;         //数据使能极性
    RGBLCD_Handler.Init.PCPolarity=LTDC_PCPOLARITY_IPC;        //像素时钟极性
    RGBLCD_Handler.Init.HorizontalSync=rgblcd.hsw-1;          //水平同步宽度
    RGBLCD_Handler.Init.VerticalSync=rgblcd.vsw-1;            //垂直同步宽度
    RGBLCD_Handler.Init.AccumulatedHBP=rgblcd.hsw+rgblcd.hbp-1; //水平同步后沿宽度
    RGBLCD_Handler.Init.AccumulatedVBP=rgblcd.vsw+rgblcd.vbp-1; //垂直同步后沿高度
    RGBLCD_Handler.Init.AccumulatedActiveW=rgblcd.hsw+rgblcd.hbp+rgblcd.pwidth-1;//有效宽度
    RGBLCD_Handler.Init.AccumulatedActiveH=rgblcd.vsw+rgblcd.vbp+rgblcd.pheight-1;//有效高度
    RGBLCD_Handler.Init.TotalWidth=rgblcd.hsw+rgblcd.hbp+rgblcd.pwidth+rgblcd.hfp-1;   //总宽度
    RGBLCD_Handler.Init.TotalHeigh=rgblcd.vsw+rgblcd.vbp+rgblcd.pheight+rgblcd.vfp-1;  //总高度
    RGBLCD_Handler.Init.Backcolor.Red=0;           //屏幕背景层红色部分
    RGBLCD_Handler.Init.Backcolor.Green=0;         //屏幕背景层绿色部分
    RGBLCD_Handler.Init.Backcolor.Blue=0;          //屏幕背景色蓝色部分
    HAL_LTDC_Init(&RGBLCD_Handler);
 	
	 
	//层配置
	RGBLCD_Layer_Parameter_Config(0,(u32)rgblcd_framebuf[0],RGBLCD_PIXFORMAT,255,0,6,7,0X000000);//层参数配置
	RGBLCD_Layer_Window_Config(0,0,0,rgblcd.pwidth,rgblcd.pheight);	//层窗口配置,以LCD面板坐标系为基准,不要随便修改!	
	 	
 	RGBLCD_Display_Dir(USE_HORIZONTAL);			//设置显示方向
	RGBLCD_Select_Layer(0); 			//选择第1层
  RGBLCD_LED=1;         		    //点亮背光
  RGBLCD_Clear(0XFFFFFFFF);			//清屏
}

//LTDC底层IO初始化和时钟使能
//此函数会被HAL_LTDC_Init()调用
//hltdc:LTDC句柄
void HAL_LTDC_MspInit(LTDC_HandleTypeDef* hltdc)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_LTDC_CLK_ENABLE();                //使能LTDC时钟
    __HAL_RCC_DMA2D_CLK_ENABLE();               //使能DMA2D时钟
    __HAL_RCC_GPIOB_CLK_ENABLE();               //使能GPIOB时钟
    __HAL_RCC_GPIOF_CLK_ENABLE();               //使能GPIOF时钟
    __HAL_RCC_GPIOG_CLK_ENABLE();               //使能GPIOG时钟
    __HAL_RCC_GPIOH_CLK_ENABLE();               //使能GPIOH时钟
    __HAL_RCC_GPIOI_CLK_ENABLE();               //使能GPIOI时钟
    
    //初始化PB5，背光引脚
    GPIO_Initure.Pin=GPIO_PIN_5;                //PB5推挽输出，控制背光
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;      //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;              //上拉        
    GPIO_Initure.Speed=GPIO_SPEED_HIGH; //高速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
    
    //初始化PF10
    GPIO_Initure.Pin=GPIO_PIN_10; 
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;          //复用
    GPIO_Initure.Pull=GPIO_NOPULL;              
    GPIO_Initure.Speed=GPIO_SPEED_HIGH; //高速
    GPIO_Initure.Alternate=GPIO_AF14_LTDC;      //复用为LTDC
    HAL_GPIO_Init(GPIOF,&GPIO_Initure);
    
    //初始化PG6,7,11
    GPIO_Initure.Pin=GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_11;
    HAL_GPIO_Init(GPIOG,&GPIO_Initure);
    
    //初始化PH9,10,11,12,13,14,15
    GPIO_Initure.Pin=GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|\
                     GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
    HAL_GPIO_Init(GPIOH,&GPIO_Initure);
    
    //初始化PI0,1,2,4,5,6,7,9,10
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_4|GPIO_PIN_5|\
                     GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_9|GPIO_PIN_10;
    HAL_GPIO_Init(GPIOI,&GPIO_Initure); 
}
