//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//����Ӳ������Ƭ��STM32F767IGT6,����ԭ��Apollo STM32F4/F7������,��Ƶ216MHZ������12MHZ
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
//      CRST      --->          TCS                  PI8             //������Ƭ��λ���ź�
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
#ifndef __TOUCH_H__
#define __TOUCH_H__
#include "sys.h"


//���ݴ������͵��败����ѡ����
#define TP_TYPE 1    //0-���败������1-���ݴ�����

//����������ģʽѡ����
#define SCAN_TYPE 1   //0-��ѯģʽ��1-�ж�ģʽ

//���ݴ��������֧�ֵĴ�������
#define CTP_MAX_TOUCH     5

#define TP_PRES_DOWN 0x80  //����������	  
#define TP_CATH_PRES 0x40  //�а��������� 	  
										    
//������������
typedef struct
{ 
	u16 x[CTP_MAX_TOUCH]; 		  //��ǰ����(�˴�ɨ��ʱ,����������)
	u16 y[CTP_MAX_TOUCH];						   	    
	u8  sta;					          //�ʵ�״̬ 
								              //b7:����1/�ɿ�0; 
	                            //b6:0,û�а�������;1,�а�������.
															//b5:����
															//b4~b0:���ݴ��������µĵ���(0,��ʾδ����,1��ʾ����)															
////////////////////////������������У׼����/////////////////////////								
	float xfac;					
	float yfac;
	short xoff;
	short yoff;	   
//b0~6:����.
//b7:0,������
//   1,������ 
	u8 touchtype;
}_m_tp_dev;

extern _m_tp_dev tp_dev;	 	//������������touch.c���涨��
    
u8 TP_Init(void);								//��ʼ��
																 		  
#endif
















