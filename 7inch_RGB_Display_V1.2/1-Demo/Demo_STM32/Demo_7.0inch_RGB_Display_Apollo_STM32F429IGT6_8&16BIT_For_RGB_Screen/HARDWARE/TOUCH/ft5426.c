//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//����Ӳ������Ƭ��STM32F103RCT6,����ԭ��MiniSTM32������,��Ƶ72MHZ������12MHZ
//QDtech-TFTҺ������ for STM32 IOģ��
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
//��ģ�����ֱ�Ӳ�������ԭ��MiniSTM32������TFTLCD��ۣ������ֶ�����
//STM32����������ָTFTLCD��������ڲ����ӵ�STM32����
//=========================================��Դ����================================================//
//     LCDģ��             TFTLCD�������        STM32��������
//      VDD       --->         5V/3.3              DC5V/3.3V          //��Դ
//      GND       --->          GND                  GND              //��Դ��
//=======================================Һ���������߽���==========================================//
//��ģ��Ĭ��������������Ϊ16λ��������
//     LCDģ��             TFTLCD�������        STM32��������
//     D0~D15     --->         D0~D15               PB0~PB15          //Һ����16λ���������źţ�8λģʽ��DB0~DB7��D0~D7��
//=======================================Һ���������߽���==========================================//
//     LCDģ�� 			       TFTLCD�������        STM32�������� 
//       WR       --->          WR                   PC7             //Һ����д���ݿ����ź�
//       RD       --->          RD                   PC6             //Һ���������ݿ����ź�
//       RS       --->          RS                   PC8             //Һ��������/��������ź�
//       RST      --->          RST                  PC4             //Һ������λ�����ź�
//       CS       --->          CS                   PC9             //Һ����Ƭѡ�����ź�
//       BL       --->          BL                   PC10            //Һ������������ź�
//=========================================������������=========================================//
//���ģ�鲻���������ܻ��ߴ��д������ܣ����ǲ���Ҫ�������ܣ�����Ҫ���д���������
//	   LCDģ��             TFTLCD�������        STM32�������� 
//      INI       --->          PEN                  PC1             //�����������ж��ź�
//      SDA       --->          MOSI                 PC3             //������IIC���������ź�
//      CRST      --->          TCS                  PC13            //��������λ�ź�
//      CLK       --->          CLK                  PC0             //������IIC����ʱ���ź�
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
	CTP_IIC_Send_Byte(FT_CMD_WR);	//����д���� 	 
	CTP_IIC_Wait_Ack(); 	 										  		   
	CTP_IIC_Send_Byte(reg&0XFF);   	//���͵�8λ��ַ
	CTP_IIC_Wait_Ack();  
	for(i=0;i<len;i++)
	{	   
		CTP_IIC_Send_Byte(buf[i]);  	//������
		ret=CTP_IIC_Wait_Ack();
		if(ret)
		{
			break;
		}
	}
  CTP_IIC_Stop();					//����һ��ֹͣ����	    
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
 	CTP_IIC_Send_Byte(FT_CMD_WR);   	//����д���� 	 
	CTP_IIC_Wait_Ack(); 	 										  		   
 	CTP_IIC_Send_Byte(reg&0XFF);   	//���͵�8λ��ַ
	CTP_IIC_Wait_Ack();  
 	CTP_IIC_Start();  	 	   
	CTP_IIC_Send_Byte(FT_CMD_RD);   	//���Ͷ�����		   
	CTP_IIC_Wait_Ack();	   
	for(i=0;i<len;i++)
	{	   
    	buf[i]=CTP_IIC_Read_Byte(i==(len-1)?0:1); //������	  
	} 
  CTP_IIC_Stop();//����һ��ֹͣ����     
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
 
  __HAL_RCC_GPIOH_CLK_ENABLE();			//����GPIOHʱ��
  __HAL_RCC_GPIOI_CLK_ENABLE();			//����GPIOIʱ��
                
  //PH7
  GPIO_Initure.Pin=GPIO_PIN_7;            //PH7
  GPIO_Initure.Mode=GPIO_MODE_INPUT;      //����
  GPIO_Initure.Pull=GPIO_PULLUP;          //����
  GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
  HAL_GPIO_Init(GPIOH,&GPIO_Initure);     //��ʼ��
            
  //PI8
  GPIO_Initure.Pin=GPIO_PIN_8;            //PI8
  GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
  HAL_GPIO_Init(GPIOI,&GPIO_Initure);     //��ʼ��		
#if SCAN_TYPE
	Touch_EXTI_Init();
#endif	
	
 
	CTP_IIC_Init();      	//��ʼ����������I2C����  
	FT_RST=0;				//��λ
	delay_ms(20);
 	FT_RST=1;				//�ͷŸ�λ		    
	delay_ms(50);  	
	temp[0]=0;
	FT5426_WR_Reg(FT_DEVIDE_MODE,temp,1);	//������������ģʽ 
#if SCAN_TYPE
	temp[0]=1;
#endif
	FT5426_WR_Reg(FT_ID_G_MODE,temp,1);		//��ѯģʽ 
	temp[0]=22;								//������Чֵ��22��ԽСԽ����	
	FT5426_WR_Reg(FT_ID_G_THGROUP,temp,1);	//���ô�����Чֵ
	temp[0]=12;								//�������ڣ�����С��12�����14
	FT5426_WR_Reg(FT_ID_G_PERIODACTIVE,temp,1); 
	//��ȡ�汾�ţ��ο�ֵ��0x3003
	FT5426_RD_Reg(FT_ID_G_LIB_VERSION,&temp[0],2);  
	if((temp[0]==0X30&&temp[1]==0X03)||temp[1]==0X01||temp[1]==0X02)//�汾:0X3003
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
	static u8 t=0;//���Ʋ�ѯ���,�Ӷ�����CPUռ����   
	t++;
	if((t%10)==0||t<10)//����ʱ,ÿ����10��CTP_Scan�����ż��1��,�Ӷ���ʡCPUʹ����
#endif
	{
		FT5426_RD_Reg(FT_REG_NUM_FINGER,&mode,1);//��ȡ�������״̬  
		if((mode&0XF)&&((mode&0XF)<6))
		{
			temp=0XFF<<(mode&0XF);//����ĸ���ת��Ϊ1��λ��,ƥ��tp_dev.sta���� 
			tp_dev.sta=(~temp)|TP_PRES_DOWN|TP_CATH_PRES; 
			for(i=0;i<CTP_MAX_TOUCH;i++)
			{
				if(tp_dev.sta&(1<<i))	//������Ч?
				{
					FT5426_RD_Reg(FT5426_TPX_TBL[i],buf,4);	//��ȡXY����ֵ 
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
					if((buf[0]&0XF0)!=0X80)tp_dev.x[i]=tp_dev.y[i]=0;//������contact�¼�������Ϊ��Ч
					//printf("x[%d]:%d,y[%d]:%d\r\n",i,tp_dev.x[i],i,tp_dev.y[i]);
				}			
			} 
			res=1;
			if(tp_dev.x[0]==0 && tp_dev.y[0]==0)mode=0;	//���������ݶ���0,����Դ˴�����
#if !SCAN_TYPE
			t=0;		//����һ��,��������������10��,�Ӷ����������
#endif
		}
	}
	if((mode&0X1F)==0)//�޴����㰴��
	{ 
#if SCAN_TYPE
		touch_flag=0;
#endif
		if(tp_dev.sta&TP_PRES_DOWN)	//֮ǰ�Ǳ����µ�
		{
			tp_dev.sta&=~(1<<7);	//��ǰ����ɿ�
		}else						//֮ǰ��û�б�����
		{ 
			tp_dev.x[0]=0xffff;
			tp_dev.y[0]=0xffff;
			tp_dev.sta&=0XE0;	//�������Ч���	
		}	 
	} 
#if !SCAN_TYPE	
	if(t>240)t=10;//���´�10��ʼ����
#endif
	return res;
}
 





































