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

#include "rgb_lcd.h"
#include "lcd.h"

LTDC_HandleTypeDef  RGBLCD_Handler;	    //RGBLCD���
DMA2D_HandleTypeDef DMA2D_Handler; 	    //DMA2D���

//���ݲ�ͬ����ɫ��ʽ,����֡��������
#if RGBLCD_PIXFORMAT==RGBLCD_PIXFORMAT_ARGB8888||RGBLCD_PIXFORMAT==RGBLCD_PIXFORMAT_RGB888
	u32 rgb_lcd_framebuf[1024][600] __attribute__((at(RGBLCD_FRAME_BUF_ADDR)));	//����������ֱ���ʱ,RGBLCD�����֡���������С
#else
	u16 rgb_lcd_framebuf[1024][600] __attribute__((at(RGBLCD_FRAME_BUF_ADDR)));	//����������ֱ���ʱ,RGBLCD�����֡���������С
#endif

u32 *rgblcd_framebuf[2];					//RGB LCD֡��������ָ��,����ָ���Ӧ��С���ڴ�����
rgblcd_dev rgblcd;						    //����RGB LCD����Ҫ����

//��LCD����
//lcd_switch:1 ��,0���ر�
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

//����ָ����
//layerx:���,0,��һ��; 1,�ڶ���
//sw:1 ��;0�ر�
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

//ѡ���
//layerx:���;0,��һ��;1,�ڶ���;
void RGBLCD_Select_Layer(u8 layerx)
{
	rgblcd.activelayer=layerx;
}

//����LCD��ʾ����
//dir:0,������1,����
void RGBLCD_Display_Dir(u8 dir)
{
  rgblcd.dir=dir%4; 	//��ʾ����
	if(dir==0||dir==2)			//����
	{
		rgblcd.width=rgblcd.pheight;
		rgblcd.height=rgblcd.pwidth;	
	}else if(dir==1||dir==3)	//����
	{
		rgblcd.width=rgblcd.pwidth;
		rgblcd.height=rgblcd.pheight;
	}
	lcd_dev.dir = rgblcd.dir;
	lcd_dev.width = rgblcd.width;
	lcd_dev.height = rgblcd.height;
}

//���㺯��
//x,y:����
//color:��ɫֵ
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

//���㺯��
//����ֵ:��ɫֵ
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

//LTDC������,DMA2D���
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)   
//ע��:sx,ex,���ܴ���lcddev.width-1;sy,ey,���ܴ���lcddev.height-1!!!
//color:Ҫ������ɫ
void RGBLCD_Fill_color(u16 sx,u16 sy,u16 ex,u16 ey,u32 color)
{ 
	u32 psx,psy,pex,pey;	//��LCD���Ϊ��׼������ϵ,����������仯���仯
	u32 timeout=0; 
	u16 offline;
	u32 addr; 
	//����ϵת��
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
	__HAL_RCC_DMA2D_CLK_ENABLE();	//ʹ��DM2Dʱ��
	DMA2D->CR&=~(DMA2D_CR_START);	//��ֹͣDMA2D
	DMA2D->CR=DMA2D_R2M;			//�Ĵ������洢��ģʽ
	DMA2D->OPFCCR=RGBLCD_PIXFORMAT;	//������ɫ��ʽ
	DMA2D->OOR=offline;				//������ƫ�� 

	DMA2D->OMAR=addr;				//����洢����ַ
	DMA2D->NLR=(pey-psy+1)|((pex-psx+1)<<16);	//�趨�����Ĵ���
	DMA2D->OCOLR=color;						//�趨�����ɫ�Ĵ��� 
	DMA2D->CR|=DMA2D_CR_START;				//����DMA2D
	while((DMA2D->ISR&(DMA2D_FLAG_TC))==0)	//�ȴ��������
	{
		timeout++;
		if(timeout>0X1FFFFF)break;	//��ʱ�˳�
	}  
	DMA2D->IFCR|=DMA2D_FLAG_TC;		//���������ɱ�־ 	
}


//��ָ�����������ָ����ɫ��,DMA2D���	
//�˺�����֧��u16,RGB565��ʽ����ɫ�������.
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)  
//ע��:sx,ex,���ܴ���lcddev.width-1;sy,ey,���ܴ���lcddev.height-1!!!
//color:Ҫ������ɫ�����׵�ַ
void RGBLCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
{
	u32 psx,psy,pex,pey;	//��LCD���Ϊ��׼������ϵ,����������仯���仯
	u32 timeout=0; 
	u16 offline;
	u32 addr; 
	//����ϵת��
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
	__HAL_RCC_DMA2D_CLK_ENABLE();	//ʹ��DM2Dʱ��
	DMA2D->CR&=~(DMA2D_CR_START);	//��ֹͣDMA2D
	DMA2D->CR=DMA2D_M2M;			//�洢�����洢��ģʽ
	DMA2D->FGPFCCR=RGBLCD_PIXFORMAT;	//������ɫ��ʽ
	DMA2D->FGOR=0;					//ǰ������ƫ��Ϊ0
	DMA2D->OOR=offline;				//������ƫ�� 

	DMA2D->FGMAR=(u32)color;		//Դ��ַ
	DMA2D->OMAR=addr;				//����洢����ַ
	DMA2D->NLR=(pey-psy+1)|((pex-psx+1)<<16);	//�趨�����Ĵ��� 
	DMA2D->CR|=DMA2D_CR_START;					//����DMA2D
	while((DMA2D->ISR&(DMA2D_FLAG_TC))==0)	//�ȴ��������
	{
		timeout++;
		if(timeout>0X1FFFFF)break;	//��ʱ�˳�
	} 
	DMA2D->IFCR|=DMA2D_FLAG_TC;				//���������ɱ�־  	
} 

//LCD����
//color:��ɫֵ
void RGBLCD_Clear(u32 color)
{
	RGBLCD_Fill_color(0,0,rgblcd.width-1,rgblcd.height-1,color);
}

//LTDCʱ��(Fdclk)���ú���
//PLL3_VCO Input=HSE_VALUE/PLL3M
//PLL3_VCO Output=PLL3_VCO Input * PLL3N
//PLLLCDCLK = PLL3_VCO Output/PLL3R
//����HSE_VALUE=25MHz��PLL3M=5��PLL3N=160,PLL3R=88
//LTDCLK=PLLLCDCLK=25/5*160/88=9MHz
//����ֵ:0,�ɹ�;1,ʧ�ܡ�
u8 RGBLCD_Clk_Set(u32 pllsain,u32 pllsair,u32 pllsaidivr)
{
	RCC_PeriphCLKInitTypeDef PeriphClkIniture;
	
	//LTDC�������ʱ�ӣ���Ҫ�����Լ���ʹ�õ�LCD�����ֲ������ã�
  PeriphClkIniture.PeriphClockSelection=RCC_PERIPHCLK_LTDC;	//LTDCʱ�� 	
	PeriphClkIniture.PLLSAI.PLLSAIN=pllsain;    
	PeriphClkIniture.PLLSAI.PLLSAIR=pllsair;  
	PeriphClkIniture.PLLSAIDivR=pllsaidivr;
	if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkIniture)==HAL_OK) //��������ʱ�ӣ���������Ϊʱ��Ϊ18.75MHZ
    {
        return 0;   //�ɹ�
    }
    else return 1;  //ʧ��    
}

//LTDC,���մ�������,������LCD�������ϵΪ��׼
//ע��:�˺���������LTDC_Layer_Parameter_Config֮��������.
//layerx:��ֵ,0/1.
//sx,sy:��ʼ����
//width,height:��Ⱥ͸߶�
void RGBLCD_Layer_Window_Config(u8 layerx,u16 sx,u16 sy,u16 width,u16 height)
{
    HAL_LTDC_SetWindowPosition(&RGBLCD_Handler,sx,sy,layerx);  //���ô��ڵ�λ��
    HAL_LTDC_SetWindowSize(&RGBLCD_Handler,width,height,layerx);//���ô��ڴ�С    
}

//LTDC,������������.
//ע��:�˺���,������LTDC_Layer_Window_Config֮ǰ����.
//layerx:��ֵ,0/1.
//bufaddr:����ɫ֡������ʼ��ַ
//pixformat:��ɫ��ʽ.0,ARGB8888;1,RGB888;2,RGB565;3,ARGB1555;4,ARGB4444;5,L8;6;AL44;7;AL88
//alpha:����ɫAlphaֵ,0,ȫ͸��;255,��͸��
//alpha0:Ĭ����ɫAlphaֵ,0,ȫ͸��;255,��͸��
//bfac1:���ϵ��1,4(100),�㶨��Alpha;6(101),����Alpha*�㶨Alpha
//bfac2:���ϵ��2,5(101),�㶨��Alpha;7(111),����Alpha*�㶨Alpha
//bkcolor:��Ĭ����ɫ,32λ,��24λ��Ч,RGB888��ʽ
//����ֵ:��
void RGBLCD_Layer_Parameter_Config(u8 layerx,u32 bufaddr,u8 pixformat,u8 alpha,u8 alpha0,u8 bfac1,u8 bfac2,u32 bkcolor)
{
	LTDC_LayerCfgTypeDef pLayerCfg;
	
	pLayerCfg.WindowX0=0;                       //������ʼX����
	pLayerCfg.WindowY0=0;                       //������ʼY����
	pLayerCfg.WindowX1=rgblcd.pwidth;          //������ֹX����
	pLayerCfg.WindowY1=rgblcd.pheight;         //������ֹY����
	pLayerCfg.PixelFormat=pixformat;		    //���ظ�ʽ
	pLayerCfg.Alpha=alpha;				        //Alphaֵ���ã�0~255,255Ϊ��ȫ��͸��
	pLayerCfg.Alpha0=alpha0;			        //Ĭ��Alphaֵ
	pLayerCfg.BlendingFactor1=(u32)bfac1<<8;    //���ò���ϵ��
	pLayerCfg.BlendingFactor2=(u32)bfac2<<8;	//���ò���ϵ��
	pLayerCfg.FBStartAdress=bufaddr;	        //���ò���ɫ֡������ʼ��ַ
	pLayerCfg.ImageWidth=rgblcd.pwidth;        //������ɫ֡�������Ŀ��    
	pLayerCfg.ImageHeight=rgblcd.pheight;      //������ɫ֡�������ĸ߶�
	pLayerCfg.Backcolor.Red=(u8)(bkcolor&0X00FF0000)>>16;   //������ɫ��ɫ����
	pLayerCfg.Backcolor.Green=(u8)(bkcolor&0X0000FF00)>>8;  //������ɫ��ɫ����
	pLayerCfg.Backcolor.Blue=(u8)bkcolor&0X000000FF;        //������ɫ��ɫ����
	HAL_LTDC_ConfigLayer(&RGBLCD_Handler,&pLayerCfg,layerx);   //������ѡ�еĲ�
}  

//��ȡ������,�������ж�����ȫһ���Ų�
//PG6=R7(M0);PI2=G7(M1);PI7=B7(M2);
//M2:M1:M0
//0 :0 :0	//4.3��480*272 RGB��,ID=0X4342
//0 :0 :1	//7��800*480 RGB��,ID=0X7084
//0 :1 :0	//7��1024*600 RGB��,ID=0X7016
//0 :1 :1	//7��1280*800 RGB��,ID=0X7018
//1 :0 :0	//8��1024*600 RGB��,ID=0X8016
//1 :0 :1   //10.1��1280*800,RGB��,ID=0X1018
//1 :1 :1   //VGA��ʾ��
//��ȡ������
//PG6=R7(M0);PI2=G7(M1);PI7=B7(M2);
//M2:M1:M0
//0 :0 :0	//4.3��480*272 RGB��,ID=0X4342
//0 :0 :1	//7��800*480 RGB��,ID=0X7084
//0 :1 :0	//7��1024*600 RGB��,ID=0X7016
//0 :1 :1	//7��1280*800 RGB��,ID=0X7018
//1 :0 :0	//8��1024*768 RGB��,ID=0X8017 
//����ֵ:LCD ID:0,�Ƿ�;����ֵ,ID;
u16 RGBLCD_Read_ID(void)
{
	u8 idx=0;
  GPIO_InitTypeDef GPIO_Initure;
  __HAL_RCC_GPIOG_CLK_ENABLE();       //ʹ��GPIOGʱ��
	__HAL_RCC_GPIOI_CLK_ENABLE();       //ʹ��GPIOIʱ��
    
  GPIO_Initure.Pin=GPIO_PIN_6;        //PG6
  GPIO_Initure.Mode=GPIO_MODE_INPUT;  //����
  GPIO_Initure.Pull=GPIO_PULLUP;      //����
  GPIO_Initure.Speed=GPIO_SPEED_HIGH; //����
  HAL_GPIO_Init(GPIOG,&GPIO_Initure); //��ʼ��   
  GPIO_Initure.Pin=GPIO_PIN_2|GPIO_PIN_7; //PI2,7
  HAL_GPIO_Init(GPIOI,&GPIO_Initure);     //��ʼ��  
  
  idx=(u8)HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_6); //��ȡM0
  idx|=(u8)HAL_GPIO_ReadPin(GPIOI,GPIO_PIN_2)<<1;//��ȡM1
  idx|=(u8)HAL_GPIO_ReadPin(GPIOI,GPIO_PIN_7)<<2;//��ȡM2

	if(idx==1)return 0X7584;	//7����,800*480�ֱ���
	else if(idx==2)return 0X7016;	//7����,1024*600�ֱ���	
	else return 0;
}

//LCD��ʼ������
void RGBLCD_Init(void)
{   
	u16 lcdid=0;
	
	lcdid=RGBLCD_Read_ID();			//��ȡLCD���ID	
	if(lcdid==0X7584)
	{
		rgblcd.pwidth=800;			    //�����,��λ:����
		rgblcd.pheight=480;		    //���߶�,��λ:����
		rgblcd.hsw=1;				    //ˮƽͬ�����
		rgblcd.vsw=1;				    //��ֱͬ�����
		rgblcd.hbp=46;				    //ˮƽ����
		rgblcd.vbp=23;				    //��ֱ����
		rgblcd.hfp=210;			    //ˮƽǰ��
		rgblcd.vfp=22;				    //��ֱǰ��
		RGBLCD_Clk_Set(396,3,RCC_PLLSAIDIVR_4);			//��������ʱ�� 33M(�����˫��,��Ҫ����DCLK��:18.75Mhz  pll3r=43,�Ż�ȽϺ�)
	}else if(lcdid==0X7016)		
	{
		rgblcd.pwidth=1024;			//�����,��λ:����
		rgblcd.pheight=600;			//���߶�,��λ:����
    rgblcd.hsw=20;				    //ˮƽͬ�����
		rgblcd.vsw=3;				    //��ֱͬ�����
		rgblcd.hbp=140;			    //ˮƽ����
		rgblcd.vbp=20;				    //��ֱ����
		rgblcd.hfp=160;			    //ˮƽǰ��
		rgblcd.vfp=12;				    //��ֱǰ��
		RGBLCD_Clk_Set(360,2,RCC_PLLSAIDIVR_4);			//��������ʱ��  45Mhz 
		//������������.
	}
   
#if RGBLCD_PIXFORMAT==RGBLCD_PIXFORMAT_ARGB8888||RGBLCD_PIXFORMAT==RGBLCD_PIXFORMAT_RGB888 
	rgblcd_framebuf[0]=(u32*)&ltdc_lcd_framebuf;
	lcdltdc.pixsize=4;				//ÿ������ռ4���ֽ�
#else 
  rgblcd.pixsize=2;				//ÿ������ռ2���ֽ�
	rgblcd_framebuf[0]=(u32*)&rgb_lcd_framebuf;
#endif 	
    
    //LTDC����
    RGBLCD_Handler.Instance=LTDC;
    RGBLCD_Handler.Init.HSPolarity=LTDC_HSPOLARITY_AL;         //ˮƽͬ������
    RGBLCD_Handler.Init.VSPolarity=LTDC_VSPOLARITY_AL;         //��ֱͬ������
    RGBLCD_Handler.Init.DEPolarity=LTDC_DEPOLARITY_AL;         //����ʹ�ܼ���
    RGBLCD_Handler.Init.PCPolarity=LTDC_PCPOLARITY_IPC;        //����ʱ�Ӽ���
    RGBLCD_Handler.Init.HorizontalSync=rgblcd.hsw-1;          //ˮƽͬ�����
    RGBLCD_Handler.Init.VerticalSync=rgblcd.vsw-1;            //��ֱͬ�����
    RGBLCD_Handler.Init.AccumulatedHBP=rgblcd.hsw+rgblcd.hbp-1; //ˮƽͬ�����ؿ��
    RGBLCD_Handler.Init.AccumulatedVBP=rgblcd.vsw+rgblcd.vbp-1; //��ֱͬ�����ظ߶�
    RGBLCD_Handler.Init.AccumulatedActiveW=rgblcd.hsw+rgblcd.hbp+rgblcd.pwidth-1;//��Ч���
    RGBLCD_Handler.Init.AccumulatedActiveH=rgblcd.vsw+rgblcd.vbp+rgblcd.pheight-1;//��Ч�߶�
    RGBLCD_Handler.Init.TotalWidth=rgblcd.hsw+rgblcd.hbp+rgblcd.pwidth+rgblcd.hfp-1;   //�ܿ��
    RGBLCD_Handler.Init.TotalHeigh=rgblcd.vsw+rgblcd.vbp+rgblcd.pheight+rgblcd.vfp-1;  //�ܸ߶�
    RGBLCD_Handler.Init.Backcolor.Red=0;           //��Ļ�������ɫ����
    RGBLCD_Handler.Init.Backcolor.Green=0;         //��Ļ��������ɫ����
    RGBLCD_Handler.Init.Backcolor.Blue=0;          //��Ļ����ɫ��ɫ����
    HAL_LTDC_Init(&RGBLCD_Handler);
 	
	 
	//������
	RGBLCD_Layer_Parameter_Config(0,(u32)rgblcd_framebuf[0],RGBLCD_PIXFORMAT,255,0,6,7,0X000000);//���������
	RGBLCD_Layer_Window_Config(0,0,0,rgblcd.pwidth,rgblcd.pheight);	//�㴰������,��LCD�������ϵΪ��׼,��Ҫ����޸�!	
	 	
 	RGBLCD_Display_Dir(USE_HORIZONTAL);			//������ʾ����
	RGBLCD_Select_Layer(0); 			//ѡ���1��
  RGBLCD_LED=1;         		    //��������
  RGBLCD_Clear(0XFFFFFFFF);			//����
}

//LTDC�ײ�IO��ʼ����ʱ��ʹ��
//�˺����ᱻHAL_LTDC_Init()����
//hltdc:LTDC���
void HAL_LTDC_MspInit(LTDC_HandleTypeDef* hltdc)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_LTDC_CLK_ENABLE();                //ʹ��LTDCʱ��
    __HAL_RCC_DMA2D_CLK_ENABLE();               //ʹ��DMA2Dʱ��
    __HAL_RCC_GPIOB_CLK_ENABLE();               //ʹ��GPIOBʱ��
    __HAL_RCC_GPIOF_CLK_ENABLE();               //ʹ��GPIOFʱ��
    __HAL_RCC_GPIOG_CLK_ENABLE();               //ʹ��GPIOGʱ��
    __HAL_RCC_GPIOH_CLK_ENABLE();               //ʹ��GPIOHʱ��
    __HAL_RCC_GPIOI_CLK_ENABLE();               //ʹ��GPIOIʱ��
    
    //��ʼ��PB5����������
    GPIO_Initure.Pin=GPIO_PIN_5;                //PB5������������Ʊ���
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;      //�������
    GPIO_Initure.Pull=GPIO_PULLUP;              //����        
    GPIO_Initure.Speed=GPIO_SPEED_HIGH; //����
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
    
    //��ʼ��PF10
    GPIO_Initure.Pin=GPIO_PIN_10; 
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;          //����
    GPIO_Initure.Pull=GPIO_NOPULL;              
    GPIO_Initure.Speed=GPIO_SPEED_HIGH; //����
    GPIO_Initure.Alternate=GPIO_AF14_LTDC;      //����ΪLTDC
    HAL_GPIO_Init(GPIOF,&GPIO_Initure);
    
    //��ʼ��PG6,7,11
    GPIO_Initure.Pin=GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_11;
    HAL_GPIO_Init(GPIOG,&GPIO_Initure);
    
    //��ʼ��PH9,10,11,12,13,14,15
    GPIO_Initure.Pin=GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|\
                     GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
    HAL_GPIO_Init(GPIOH,&GPIO_Initure);
    
    //��ʼ��PI0,1,2,4,5,6,7,9,10
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_4|GPIO_PIN_5|\
                     GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_9|GPIO_PIN_10;
    HAL_GPIO_Init(GPIOI,&GPIO_Initure); 
}
