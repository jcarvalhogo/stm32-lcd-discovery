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
#ifndef __FT5426_H
#define __FT5426_H	
#include "sys.h"	

//����ݴ��������ӵ�оƬ����(δ����IIC����) 
//IO��������	 
#define FT_RST  PDout(11)//FT5426��λ����
#define FT_INT  PDin(13) //FT5426������	
//I2C��д����	
#define FT_CMD_WR 				0X70    	//д����
#define FT_CMD_RD 				0X71		//������
  
//FT5426 ���ּĴ������� 
#define FT_DEVIDE_MODE 			0x00   		//FT5426ģʽ���ƼĴ���
#define FT_REG_NUM_FINGER   0x02		  //����״̬�Ĵ���

#define FT_TP1_REG 				0X03	  	//��һ�����������ݵ�ַ
#define FT_TP2_REG 				0X09		  //�ڶ������������ݵ�ַ
#define FT_TP3_REG 				0X0F		  //���������������ݵ�ַ
#define FT_TP4_REG 				0X15		  //���ĸ����������ݵ�ַ
#define FT_TP5_REG 				0X1B		  //��������������ݵ�ַ  
 

#define	FT_ID_G_LIB_VERSION		0xA1		  //�汾		
#define FT_ID_G_MODE 			    0xA4   		//FT5426�ж�ģʽ���ƼĴ���
#define FT_ID_G_THGROUP			  0x80   		//������Чֵ���üĴ���
#define FT_ID_G_PERIODACTIVE	0x88   		//����״̬�������üĴ���


u8 FT5426_WR_Reg(u16 reg,u8 *buf,u8 len);
void FT5426_RD_Reg(u16 reg,u8 *buf,u8 len);
u8 FT5426_Init(void);
u8 FT5426_Scan(void);

#endif

















